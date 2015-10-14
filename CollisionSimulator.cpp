/* AP Physics Collision Simulation
 By Michael Kim, Adam Lee, Daniel Pekar, Kaarthic Pulogarajah, and Gordon Winch
 Submitted Wednesday, October 14, 2015 
 
 This program simulates the dynamics of two stress balls colliding, using appropriate spring forces.
 */

#include <iostream>
#include <cmath>
#include <fstream>

using namespace std;

class force{ //Force Class
    // The Force class holds the x and y components of a force.
    
public:
    double x, y; // X and Y components of the force
    
    force(double x, double y){ // Constructor
        this->x = x;
        this->y = y;
    }
};

class ball{ //Ball Class
    
    // The Ball Class holds all the relevant information about
    // a stress ball in the simulation (position, velocity,
    // mass, etc.).
    // It also contains several methods, applyForce and
    // applyTorque (here unused) which are used in calculating
    // the dynamics of the ball.
    
public:
    double sx, sy;  // X and Y components of the position of the ball
    double vx, vy;  // X and Y components of the velocity of the ball
    double omega;   // Rotational velocity of ball (unused)
    double m;       // Mass of ball
    double r;       // Radius of ball
    double inertia; // Rotational Inertia of ball (unused)

    ball(double sx, double sy, double vx, double vy, double m, double r){ //Constructor
        // Here, all data fields are initialized
        this->sx = sx;
        this->sy = sy;
        this->vx = vx;
        this->vy = vy;
    	omega = 0; // Rotational Velocity is set to 0
        this->m = m;
        this->r = r;
        inertia = 2*m*r*r/5; // Rotational Inertia is calculated based on the mass and radius
    }

    void applyForce(double fx, double fy, double dt){ //Apply a net force to the ball over a given timestep
        
        // This method updates the velocity and position of
        // the ball after it has been acted on by the given
        // force over the given time.
        
        // The acceleration in X and Y is calculated from Newton's
        // Second Law, F = ma. We find the component accelerations
        // by dividing the force in X and Y by the mass of the ball.
    	double ax = fx / m;
        double ay = fy / m;
        
        // The new positions are calculated using the kinematics
        // equation s = s0 + v0*t + 0.5*a*t^2. Here, the acceleration
        // is assumed to be constant over the interval of the timestep.
        sx = sx + vx*dt + 0.5*ax*dt*dt;
        sy = sx + vy*dt + 0.5*ay*dt*dt;
        
        // Lastly, we calculate the new velocities in X and Y. This is
        // done using the kinematics equation v = v0 + a*t. These
        // calculations are effected after the position calculations so
        // as to not overwrite the initial velocity value used in the
        // position equation.
        vx = vx + ax * dt;
        vy = vx + ay * dt;
    }

    void applyTorque(double force, double angle, double dt){ //Apply torque (unused)
        double alpha = force * r * sin(angle) / inertia;
        omega += alpha * dt;
    }
};

istream &operator >>(istream &input, ball &b){ // Input method for Ball class
    
    // This overwrites the istream input operator for a ball object, defining
    // the input of all necessary variables.
    
    cout << "sx: "; // Position in X
	input >> b.sx;
    cout << "sy: "; // Position in Y
	input >> b.sy;
    cout << "vx: "; // Velocity in X
    input >> b.vx;
    cout << "vy: "; // Velocity in Y
    input >> b.vy;
    cout << "m: ";  // Mass
    input >> b.m;
    cout << "r: ";  // Radius
    input >> b.r;
	return input;
}
ostream &operator <<(ostream &output, ball &b){ //Output method for Ball class
    
    // This overwrites the ostream output operator for a ball object, writing
    // out the position and velocity values in CSV (comma seperated values)
    // format.
    
    output << b.sx << "," << b.sy << "," << b.vx << "," << b.vy;
	return output;
}

force getForce(ball b1, ball b2){ //Calculate force from collision
    
    // This method is used to calculate the force acting on ball b1 from
    // ball b2. It calculates the distance between balls, and if the balls
    // are touching, calculates the spring force in X and Y.
    
    // First, the distance in X, distance in Y, and magnitude of distance
    // between the two balls is calculated.
    double dx = b1.sx - b2.sx; // distance in X
    double dy = b1.sy - b2.sy; // distance in Y
    double d = sqrt(pow(dx,2) + pow(dy,2)); // Magnitude of distance
    
    // Next, the compression of the balls is calculated. This is found by
    // assuming that the balls compress equally on both sides of the centre,
    // producing a total thickness equal to the distance between the two
    // identical balls (2r). We can then calculate the compression of the
    // balls by subtracting the distance between the balls from the sum of
    // their radii.
    double comp = b1.r + b2.r - d;
    
    // If the compression is less than 0 (i.e. the distance is greater than
    // the sum of the radii of the two balls), then the balls are not in
    // contact and there is no spring force acting on them. If this is the
    // case, we return a force vector of (0,0) N, terminating the execution
    // of the method.
    if(comp <= 0) return force(0,0);

    // Now, we finally calculate the magnitude of the spring force. We know
    // that the balls are touching from the prevous line. We calculate the
    // magnitude using the regressed quadratic force equation found
    // experimentally. Here, our expression for k is k = 85000x + 410, and
    // when multiplied by x in |F| = kx, we get |F| = 85000x^2 + 410x.
    double fmag = 85000 * comp * comp + 410 * comp;
    
    // Lastly, we return this value in the form of a force object. The
    // component forces are calculated in the return statement. As we
    // know from cartesian-polar conversion, Fx = Fcos(theta). However,
    // we also know that cos(theta) is given by adjacent/hypotenuse, or
    // distance in x / distance (dx/d). Substituting this into our
    // expression for force in x, we get Fx = F*dx/d. A similar equation
    // goes for Fy, Fy = F*dy/d. Using these expressions:
    return force(fmag*dx/d,fmag*dy/d);
}

bool running(ball b1, ball b2, double t){ // check if the simulation should continue
    
    // This method checks if the end conditions of the simulation have been
    // met, based on the balls and time. In this terminal condition, it has
    // been decided to end the simulation after four seconds of run time
    // because all events have finished by this time. 
    return t < 4;
}

int main(){ // Main Method
    
    // This is the Main method, which runs the program.
    
    // First, we choose a constant timestep. A low value was chosen to
    // ensure the accuracy of our simulation.
    double dt = 0.0001;
    
    // Next, we create our two ball objects, b1 and b2.
    ball b1, b2;
    
    // We input the two balls from the keyboard user.
    cout << "Enter ball 1:\n";
    cin >> b1;
    cout << "Enter ball 2:\n";
    cin >> b2;

    // Set up output to csv file.
    ofstream output("data.csv"); //Create output data file
    if(!output) cout << "Output file not created."; //Warning if data file couldn't be created
    output << "t,sx1,sy1,vx1,vy1,sx2,sy2,vx2,vy2\n"; //Column titles

    // Simulation loop. This runs, calculating and outputting data, until
    // the end condition given by running() is met.
    for(double t=0; running(b1, b2, t); t+=dt){
        
        // First, we calculate the force acting on ball 1.
        force f = getForce(b1, b2);

        // Next, we apply this force to ball 1.
        b1.applyForce(f.x, f.y, dt);
        
        // We then apply the negative of this force (since the force acting
        // on ball 2 from ball one will be equal to the negative of the force
        // acting on ball 1 from ball 2).
        b2.applyForce(-f.x, -f.y, dt);

        // Lastly, we output the data to the csv file.
        output << t << "," << b1 << "," << b2 << endl;
    }

    // We terminate output to the csv file
    output.close();

    // Lastly, we output final conditions to the console.
    cout << "Simulation complete.\nFinal values:\n";
    cout << "Ball 1: " << b1 << endl;
    cout << "Ball 2: " << b2 << endl;
    return 0;
}
