#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

using namespace std;

class force{ //Force
public:
    double x, y;
    force(double x, double y){
        this->x = x;
        this->y = y;
    }
};

class ball{ //Ball
public:
    double sx, sy; //Translational motion
    double vx, vy;
    double ax, ay;
    double omega, alpha; //Rotational motion
    double m; // Yellow ball is 57.2 +- .1
    double k;
    double r;
    double inertia;

    ball(){ //Default constructor
        sx = sy = vx = vy = m = k = r = omega = alpha = inertia = 0;
    }
    ball(double sx, double sy, double vx, double vy, double m, double k, double r){ //Constructor
        this->sx = sx;
        this->sy = sy;
        this->vx = vx;
        this->vy = vy;
        ax = ay = 0;
    	omega = alpha = 0;
        this->m = m;
        this->k = k;
        this->r = r;
        inertia = 2*m*r*r/5;
    }

    void addForce(force f){ //Apply force
    	ax += f.x / m;
    	ay += f.y / m;
    }

    void applyTorque(double f, double theta){ //Apply torque
        alpha = f * r * sin(theta) / inertia;
    }

    void update(double dt){ //Update ball with forces
        vx += ax*dt;
        vy += ay*dt;
        sx += vx*dt;
        sy += vy*dt;
        ax = ay = 0;

        omega += alpha*dt;
        alpha = 0;
    }
};
istream &operator >>(istream &input, ball &b){ //Input
    cout << "sx: ";
	input >> b.sx;
    cout << "sy: ";
	input >> b.sy;
    cout << "vx: ";
    input >> b.vx;
    cout << "vy: ";
    input >> b.vy;
    cout << "m: ";
    input >> b.m;
    cout << "k: ";
    input >> b.k;
    cout << "r: ";
    input >> b.r;
	return input;
}
ostream &operator <<(ostream &output, ball &b){ //Output
    output << b.sx << "," << b.sy << "," << b.vx << "," << b.vy;
	return output;
}

force getForce(ball b1, ball b2){ //Get force from collision
    double dx = b1.sx - b2.sx;
    double dy = b1.sy - b2.sy;
    double d = sqrt(pow(dx,2) + pow(dy,2));
    double comp = b1.r + b2.r - d;
    if(comp <= 0) return force(0,0);

    double fmag = b1.k*comp;
    return force(fmag*dx/d,fmag*dy/d);
}

bool running(vector<ball> balls, double t){ //Whether to keep on running simulation
    return t < 4;
}

int main(){
    double dt = 0.0001;
    int num;
    cout << "Number of balls: ";
    cin >> num;
    vector<ball> balls(num);
    for(int i=0; i<num; ++i){
      	cout << "Enter ball " << i+1 << ":\n";
      	cin >> balls[i];
    }

	ofstream output("data.csv");
	if(!output) cout << "Output file notcreated.";
	output << "t";
	for(int i=1; i<=num; ++i){
        output << ",sx" << i;
        output << ",sy" << i;
        output << ",vx" << i;
        output << ",vy" << i;
	}
	output << endl;

	for(double t=0; running(balls, t); t+=dt){
        for(int i=0; i<num; ++i){
            for(int j=0; j<num; ++j){
                if(i != j){
                    force f = getForce(balls[i], balls[j]);
                    balls[i].addForce(f);
                }
            }
        }

		output << t;
		for(int i=0; i<num; ++i){
            balls[i].update(dt);
        	output << "," << balls[i];
		}
		output << endl;
    }

    cout << "Simulation complete.\nFinal values:\n";
    for(int i=0; i<num; ++i){
        cout << i+1 << ": " << balls[i] << endl;
    }
    output.close();
    return 0;
}
