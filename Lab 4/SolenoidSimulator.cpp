#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
using namespace std;
//constants and values
double R = 0.07;                //radius of Helmholtz coil
double L = 0.022;               //length of coil
double pi = 3.141592;           //pi
double w = 2 * pi * 320 / L;    //angular frequency
double theta = -72;             //rotation of first coil (about z-axis)
double theta2 = -120;           //rotation of second coil

double mu = 0.00000125663;  //vacuum permeability
double i = 0.5733;             //current through coils
double dt = 0.000002;       //dt for numerical integration

double sizex = 0.24;            //length in x (cm)
double sizey = 0.24;            //length in y (cm)
double sizez = 0.24;            //length in z (cm)
double scale = 100;             //unit conversion (cm -> m)

class Vector3D{ //3-D vector class
public:
    double x, y, z; //vector components
    Vector3D(){ //default constructor
        x = y = z = 0;
    }
    Vector3D(double xx, double yy, double zz): x(xx), y(yy), z(zz){} //constructor with parameters

    Vector3D operator+(const Vector3D& vec){ //addition of two vectors
        return Vector3D(x + vec.x, y + vec.y, z + vec.z);
    }

    Vector3D operator-(const Vector3D& vec){ //subtraction of two vectors
        return Vector3D(x - vec.x, y - vec.y, z - vec.z);
    }

    Vector3D operator*(double scalar){ //multiplication by a scalar
        return Vector3D(scalar * x, scalar * y, scalar * z);
    }

    Vector3D cross(Vector3D vec){ //cross product
        return Vector3D(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);
    }

    double magnitude(){ //magnitude of a vector
        return sqrt(x * x + y * y + z * z);
    }
};

Vector3D coil(double t){ //default (not rotated) position vector of Helmholtz coil
    double x = R * cos(w * t);
    double y = t;
    double z = R * sin(w * t);
    return Vector3D(x, y, z);
}

Vector3D dcoil(double t){ //derivative of default coil
    double xp = -w * R * sin(w * t);
    double yp = 1;
    double zp = w * R * cos(w * t);
    return Vector3D(xp, yp, zp);
}

Vector3D rot(Vector3D pos, double theta){ //rotates a position vector by an angle
    double s = sin(theta);
    double c = cos(theta);
    double rotMatrix[][3] = { //z-axis rotation matrix
        {c, -s, 0},
        {s, c, 0},
        {0, 0, 1}
    };
    Vector3D newRot;
    newRot.x = rotMatrix[0][0] * pos.x + rotMatrix[0][1] * pos.y + rotMatrix[0][2] * pos.z;
    newRot.y = rotMatrix[1][0] * pos.x + rotMatrix[1][1] * pos.y + rotMatrix[1][2] * pos.z;
    newRot.z = rotMatrix[2][0] * pos.x + rotMatrix[2][1] * pos.y + rotMatrix[2][2] * pos.z;
    return newRot;
}

Vector3D pos(double t){ //position vector of first coil (rotated by theta)
    Vector3D sol = rot(coil(t), theta*pi/180);
    sol.x += sizex/2 + 0.04;
    sol.y += sizey/2 - L/2;
    sol.z += sizez/2;
    return sol;
}

Vector3D pos2(double t){ //position vector of second coil (rotated by theta2)
    Vector3D sol = rot(coil(t), theta2*pi/180);
    sol.x += sizex/2 - 0.04;
    sol.y += sizey/2 - L/2 + 0.01;
    sol.z += sizez/2;
    return sol;
}

Vector3D dpos(double t){ //derivative of first coil
    return rot(dcoil(t), theta*pi/180);
}

Vector3D dpos2(double t){ //derivative of second coil
    return rot(dcoil(t), theta2*pi/180);
}

int main(){
    ofstream file("simData.csv"); //create csv file
    if(!file.is_open()){ //csv file error
        cout << "Output csv file cannot be created.\n";
        return -1;
    }

    //magnetic field of 3-D grid
    vector<vector<vector<Vector3D>>> bfield(sizex * scale + 1,
                                     vector<vector<Vector3D>>(sizey * scale + 1,
                                     vector<Vector3D>(sizez * scale + 1, Vector3D(0,0,0))));

    double Tstart = 0;          //starting t
    double Tend = L;            //ending t
    double t = Tstart;          //t for integrating along wire

    double constant = mu * i / 4 / pi; //mu i / (4 pi), calculated beforehand

    Vector3D l;         //l = wire element
    Vector3D l2;        //l (2nd solenoid)
    Vector3D rprime;    //r' = displacement vector from l to where field is being computed (r)
    Vector3D rprime2;   //r' (2nd solenoid)
    Vector3D dl;        //dl = length of differental element of wire
    Vector3D dl2;       //dl (2nd solenoid)
    Vector3D integral;  //line integral using Biot-Savart Law
    Vector3D integral2; //integral (2nd solenoid)

    cout << "Starting simulation.\n";
    while(Tstart <= t && t <= Tend){ //integrating around the solenoids
        //redundant calculations for faster simulation
        l = pos(t);
        l2 = pos2(t);
        dl = dpos(t) * dt;
        dl2 = dpos2(t) * dt;

        for(int fieldx = 0; fieldx < bfield.size(); fieldx++){ //x coordinate
            for(int fieldy = 0; fieldy < bfield[0].size(); fieldy++){ //y coordinate
                for(int fieldz = 0; fieldz < bfield[0][0].size(); fieldz++){ //z coordinate
                    //first solenoid
                    rprime = Vector3D(fieldx, fieldy, fieldz) * (1 / scale) - l; //distance to dl
                    integral = dl.cross(rprime) * (constant / pow(rprime.magnitude(), 3.0)); //integrand

                    //second solenoid
                    rprime2 = Vector3D(fieldx, fieldy, fieldz) * (1 / scale) - l2;
                    integral2 = dl2.cross(rprime2) * (constant / pow(rprime2.magnitude(), 3.0));

                    //if(abs(pow(rprime.magnitude(), 3.0)) ==0 || abs(pow(rprime2.magnitude(), 3.0)) ==0) cout << "(x,y,z) = " << fieldx << " " << fieldy << " " << fieldz << endl;

                    //sum the integrands
                    bfield[fieldx][fieldy][fieldz] = bfield[fieldx][fieldy][fieldz] + integral + integral2;
                }
            }
        }
        cout << t << endl;
        t += dt; //step
    }

    //output to csv
    file << "x,y,z,Bx,By,Bz\n";
    for(int fieldx = 0; fieldx < bfield.size(); fieldx++){
        for(int fieldy = 0; fieldy < bfield[0].size(); fieldy++){
            for(int fieldz = 0; fieldz < bfield[0][0].size(); fieldz++){
                file << fieldx / scale << "," <<                    //x coordinate
                        fieldy / scale << "," <<                    //y coordinate
                        fieldz / scale << "," <<                    //z coordinate
                        bfield[fieldx][fieldy][fieldz].x << "," <<  //magnetic field (x)
                        bfield[fieldx][fieldy][fieldz].y << "," <<  //magnetic field (y)
                        bfield[fieldx][fieldy][fieldz].z << endl;   //magnetic field (z)
            }
        }
    }
    file.close(); //close csv file
    cout << "Simulation complete.";
    return 0;
}
