#include <iostream>
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
    double omega; //Rotational motion
    double m; // Yellow ball is 57.2 +- .1
    double r;
    double inertia;

    ball(){ //Default constructor
        sx = sy = vx = vy = m = r = omega = inertia = 0;
    }
    ball(double sx, double sy, double vx, double vy, double m){ //Constructor
        this->sx = sx;
        this->sy = sy;
        this->vx = vx;
        this->vy = vy;
    	omega = 0;
        this->m = m;
        this->r = r;
        inertia = 2*m*r*r/5;
    }

    void applyForce(double fx, double fy, double dt){ //Apply force
    	double ax = fx / m;
    	double ay = fy / m;
        vx += ax * dt;
        vy += ay * dt;
        sx += vx * dt;
        sy += vy * dt;
    }

    void applyTorque(double force, double angle, double dt){ //Apply torque
        double alpha = force * r * sin(angle) / inertia;
        omega += alpha * dt;
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

    double fmag = 85000 * comp * comp + 410 * comp; //from stress ball
    return force(fmag*dx/d,fmag*dy/d);
}

bool running(ball b1, ball b2, double t){ //Whether to keep on running simulation
    return t < 4;
}

int main(){
    double dt = 0.0001; //Timestep
    ball b1, b2; //Two balls
    cout << "Enter ball 1:\n";
    cin >> b1;
    cout << "Enter ball 2:\n";
    cin >> b2;

    ofstream output("data.csv"); //Create output data file
    if(!output) cout << "Output file not created."; //Warning if data file couldn't be created
    output << "t,sx1,sy1,vx1,vy1,sx2,sy2,vx2,vy2\n"; //Column titles

    for(double t=0; running(b1, b2, t); t+=dt){
        force f = getForce(b1, b2);

        b1.applyForce(f.x, f.y, dt);
        b2.applyForce(-f.x, -f.y, dt);

        output << t << "," << b1 << "," << b2 << endl;
    }

    output.close();

    cout << "Simulation complete.\nFinal values:\n";
    cout << "Ball 1: " << b1 << endl;
    cout << "Ball 2: " << b2 << endl;
    return 0;
}
