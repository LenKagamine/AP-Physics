//
//  main.cpp
//  Collision Simulator

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct force{
    double x, y;
    
    force(double x, double y){
        this->x = x;
        this->y = y;
    }
};

class ball {
public:
    double sx, sy;
    double vx, vy;
    double m; // = 57.2; // Yellow ball is 57.2 +- .1
    double k;
    double r;
    
    ball(){
        sx = sy = vx = vy = m = k = r = 0;
    }
    ball(int sx, int sy, int vx, int vy, int m, int k, int r){
        this->sx = sx;
        this->sy = sy;
        this->vx = vx;
        this->vy = vy;
        this->m = m;
        this->k = k;
        this->r = r;
    }
    
    void applyForce(double fx, double fy, double dt){
        double ax = fx / m;
        double ay = fy / m;
        
        vx += ax*dt;
        vy += ay*dt;
        
        sx += vx*dt;
        sy += vy*dt;
    }
    
    void print(){
        cout<<"s = ("<<sx<<","<<sy<<")\n";
        cout<<"v = ("<<vx<<","<<vy<<")\n";
    }
};

bool contin (vector<ball> balls, double t){
    return t < 4;
}

force getForce(ball b1, ball b2){
    // calculate distance
    double dx = b1.sx - b2.sx;
    double dy = b1.sy - b2.sy;
    
    double d = sqrt(pow(dx,2) + pow(dy,2));
    
    double comp = b1.r + b2.r - d;
    
    if (comp <= 0) return force(0,0);
    
    double fmag = b1.k*comp;
    
    force f = force(fmag*dx/d,fmag*dy/d);
    
    return f;
}


// ***** Main Method *****
int main(int argc, const char *argv[]) {
    vector<ball> balls (2);
    
    for(ball b: balls){
        cout<<"enter ball values";
        cin >> b.sx >> b.sy >> b.vx >> b.vy >> b.m >> b.k >> b.r;
    }
    
    double t = 0;
    double dt = 0.05;
    
    while (contin(balls, t)){
        force f = getForce(balls[0],balls[1]);
        
        balls[0].applyForce(f.x,f.y,dt);
        balls[1].applyForce(-f.x,-f.y,dt);
        
        cout<<"t = "<<t<<"\n";
        cout<<"Ball 1:\n";
        balls[0].print();
        cout<<"Ball 2:\n";
        balls[1].print();
        cout<<endl;
        
        t += dt;
    }
    
    return 0;
}

