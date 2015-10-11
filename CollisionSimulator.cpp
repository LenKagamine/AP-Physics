#include <iostream>
#include <vector>
#include <cmath>
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
    double sx, sy;
    double vx, vy;
    double ax, ay;
    double m; // = 57.2; // Yellow ball is 57.2 +- .1
    double k;
    double r;

    ball(){ //Default constructor
        sx = sy = vx = vy = m = k = r = 0;
    }
    ball(double sx, double sy, double vx, double vy, double m, double k, double r){ //Constructor
        this->sx = sx;
        this->sy = sy;
        this->vx = vx;
        this->vy = vy;
        ax = ay = 0;
        this->m = m;
        this->k = k;
        this->r = r;
    }
    
    void addForce(force f){
    	ax += f.x / m;
    	ay += f.y / m;
    }

    void update(double dt){ //Apply force to ball
        vx += ax*dt;
        vy += ay*dt;
        sx += vx*dt;
        sy += vy*dt;
        ax = ay = 0;
    }
};
istream &operator >>(istream &input, ball &b){ //Input
	input >> b.sx >> b.sy >> b.vx >> b.vy >> b.m >> b.k >> b.r;
	return input;
}
ostream &operator <<(ostream &output, ball &b){ //Output
    output << "s = (" << b.sx << "," << b.sy <<")\n";
    output << "v = (" << b.vx << "," << b.vy <<")\n";
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
    double t = 0;
    double dt = 0.05;
    int num;
    cout << "Number of balls: ";
    cin >> num;
    vector<ball> balls(num);
    for(int i=0; i<num; ++i){
      	cout << "Enter ball " << i+1 << "\n";
      	cin >> balls[i];
    }
	
	for(double t=0; running(balls, t); t+=dt){
        for(int i=0; i<num; ++i){
            for(int j=0; j<num; ++j){
                if(i != j){
                    force f = getForce(balls[i], balls[j]);
                    balls[i].addForce(f);
                }
            }
        }
        
		cout << "t = " << t << ":\n";
		for(int i=0; i<num; ++i){
            balls[i].update(dt);
        	cout << "Ball " << i+1 << ":\n" << balls[i];
		}
    }
    return 0;
}
