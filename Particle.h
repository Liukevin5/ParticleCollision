#ifndef PARTICLE_H
#define PARTICLE_H
#include "Point.h"
#include <cmath>
#include <ctime>
class Particle{


    private:
    double radius;
    double mass;
    Point center;
    Point velocity;
    Point force;
    public:
    double getRadius() const;
    double getMass() const;
    double getXPos();
    double getYPos();
    Point getPos() const;
    Particle(Point p, double m);
    Point getVeloc() const;
    Point getForce() const;
    void setXPos(double d);
    void setYPos(double d);
    void setForce( double fx,double fy);
    void setVeloc(double x, double y);
    void setMass(double m);
    void updateVel(double time);
    void updatePos(double time);
    void bounceU();
    void bounceD(double h);
    void bounceL();
    void bounceR(double w);
};
#endif // PARTICLE_H
