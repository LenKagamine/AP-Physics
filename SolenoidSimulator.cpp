#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
using namespace std;

double R = 0.07; //y coefficient
double f = 320 / 0.022; //frequency
double w = 2 * 3.141592 * f; //angular frequency

double x(double t) {
    return R * cos(w * t) + 0.10;
}

double y(double t) {
    return R * sin(w * t) + 0.10;
}

double z(double t) {
    return t + 0.03;
}

double xp(double t) {
    return -w * R * sin(w * t);
}

double yp(double t) {
    return w * R * cos(w * t);
}

double zp(double t) {
    return 1;
}

double x2(double t) {
    return -t + 0.192;
}

double y2(double t) {
    return R * sin(w * t) + 0.10;
}

double z2(double t) {
    return R * cos(w * t) + 0.10;
}

double x2p(double t) {
    return -1;
}

double y2p(double t) {
    return w * R * cos(w * t);
}

double z2p(double t) {
    return -w * R * sin(w * t);
}

class Vector3D{
public:
    double x, y, z;
    Vector3D(){}
    Vector3D(double xx, double yy, double zz): x(xx), y(yy), z(zz){}

    Vector3D operator+(const Vector3D& vec){
        return Vector3D(x + vec.x, y + vec.y, z + vec.z);
    }

    Vector3D operator-(const Vector3D& vec){
        return Vector3D(x - vec.x, y - vec.y, z - vec.z);
    }

    Vector3D operator*(double scalar){
        return Vector3D(scalar * x, scalar * y, scalar * z);
    }

    Vector3D cross(Vector3D vec){
        return Vector3D(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);
    }

    double magnitude(){
        return sqrt(x * x + y * y + z * z);
    }
};
int main(){
    double mu = 0.00000125663;  //vacuum permeability
    double pi = 3.141592;       //pi
    double i = 1.5;             //current through coils
    double dt = 0.000002;       //dt for numerical integration

    double Tstart = 0;          //starting t
    double Tend = 0.022;        //ending t
    int sizex = 24;             //length in x (cm)
    int sizey = 24;             //length in y (cm)
    int sizez = 24;             //length in z (cm)

    int unit = 1;               //
    int unit2 = 1;              //

    ofstream file("data.csv"); //create csv file
    if(!file.is_open()){
        cout << "Output csv file cannot be created.\n";
        return -1;
    }

    vector<vector<vector<Vector3D>>> bfield(sizex / unit * unit2 + 1,
                                     vector<vector<Vector3D>>(sizey / unit * unit2 + 1,
                                     vector<Vector3D>(sizez / unit * unit2 + 1, Vector3D(0,0,0))));

    double constant = mu * i / 4 / pi;          //mu / 4pi * i
    double sizeConst = 0.01 * unit / unit2;     //size constant of r' vector

    Vector3D l;         //l = wire element
    Vector3D l2;        //l (2nd solenoid)
    Vector3D rprime;    //r' = displacement vector from l to where field is being computed (r)
    Vector3D rprime2;   //r' (2nd solenoid)
    Vector3D dl;        //dl = length of differental element of wire
    Vector3D dl2;       //dl (2nd solenoid)
    Vector3D integral;  //line integral using Biot-Savart Law
    Vector3D integral2; //integral (2nd solenoid)

    cout << "Starting simulation.\n";
    for(double t = Tstart; t <= Tend; t += dt){ //integrating around the solenoids
        //reusable calculations for faster simulation
        l = Vector3D(x(t), y(t), z(t));
        l2 = Vector3D(x2(t), y2(t), z2(t));

        dl = Vector3D(xp(t), yp(t), zp(t)) * dt;
        dl2 = Vector3D(x2p(t), y2p(t), z2p(t)) * dt;

        for(int fieldx = 0; fieldx < bfield.size(); fieldx++){ //x coordinate
            for(int fieldy = 0; fieldy < bfield[0].size(); fieldy++){ //y coordinate
                for(int fieldz = 0; fieldz < bfield[0][0].size(); fieldz++){ //z coordinate
                    //first solenoid
                    rprime = Vector3D(fieldx, fieldy, fieldz) * sizeConst - l;
                    integral = dl.cross(rprime) * (constant / pow(rprime.magnitude(), 3.0));

                    //second solenoid
                    rprime2 = Vector3D(fieldx, fieldy, fieldz) * sizeConst - l2;
                    integral2 = dl2.cross(rprime) * (constant / pow(rprime.magnitude(), 3.0));

                    //sum the integrands
                    bfield[fieldx][fieldy][fieldz] = bfield[fieldx][fieldy][fieldz] + integral + integral2;
                }
            }
        }
        //cout << t << endl;
    }

    //output to csv
    file << "x,y,z,Bx,By,Bz\n";
    for(int fieldx = 0; fieldx < bfield.size(); fieldx++){
        for(int fieldy = 0; fieldy < bfield[0].size(); fieldy++){
            for(int fieldz = 0; fieldz < bfield[0][0].size(); fieldz++){
                file << fieldx * sizeConst << "," <<
                        fieldy * sizeConst << "," <<
                        fieldz * sizeConst << "," <<
                        bfield[fieldx][fieldy][fieldz].x << "," <<
                        bfield[fieldx][fieldy][fieldz].y << "," <<
                        bfield[fieldx][fieldy][fieldz].z << endl;
            }
        }
    }
    file.close();
    cout << "Simulation complete.";
    return 0;
}
