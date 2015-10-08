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
        this->m = m;
        this->k = k;
        this->r = r;
    }

    void applyForce(double fx, double fy, double dt){ //Apply force to ball
        double ax = fx / m;
        double ay = fy / m;
        vx += ax*dt;
        vy += ay*dt;
        sx += vx*dt;
        sy += vy*dt;
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

bool running(ball b1, ball b2, double t){ //Whether to keep on running simulation
    return t < 4;
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

int main(){
    double t = 0;
    double dt = 0.05;
    ball b1, b2;

    cout<<"Enter ball 1:\n";
    cin >> b1;
    cout<<"Enter ball 2:\n";
    cin >> b2;

    while(running(b1, b2, t)){
        force f = getForce(b1,b2);

        b1.applyForce(f.x,f.y,dt);
        b2.applyForce(-f.x,-f.y,dt);

        cout << "t = " << t << ":\n";
        cout << "Ball 1:\n" << b1;
        cout << "Ball 2:\n" << b2;
        cout << endl;

        t += dt;
    }
    return 0;
}
