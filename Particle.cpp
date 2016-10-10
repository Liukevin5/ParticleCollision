#include "Particle.h"
#include "Point.h"
#include <iostream>
#include <cmath>
#include <ctime>
Particle::Particle(Point p, double m): mass(m)
{

    center= Point(p.getX(), p.getY());
    velocity = Point(0,0);
    force = Point(0,0);
    radius = m/3;
}
double Particle::getRadius() const
{
    return radius;
}

double Particle::getMass() const
{
    return mass;
}
double Particle::getXPos()
{
    return center.getX();
}
double Particle::getYPos()
{
    return center.getY();
}
void Particle::updateVel(double t)
{
    double aX = force.getX()/mass;
    double aY = force.getY()/mass;
   //  std::cout<<aX<<"\n";
   //  std::cout<<aY<<"\n";
    double xNew = velocity.getX() + aX*t;
    double yNew = velocity.getY() + aY*t;
    /*
    if(xNew >= 40){
        xNew = 40;
    }
    if(xNew <= -40){
        xNew = -40;
    }
    if(yNew >= 40){
        yNew = 40;
    }
    if(yNew <= -40){
        yNew = -40;
    }
    */
     //std::cout<<xNew<<"\n";
     // std::cout<<yNew<<"\n";
    Point newVelocity(xNew, yNew);
    velocity = newVelocity;
}

void Particle::updatePos(double t)
{
    double xf = center.getX() + velocity.getX()*t + .5* (force.getX()/mass)*t*t;
    //std::cout<<xf<<"\n";
    double yf =  center.getY() + velocity.getY()*t + .5* (force.getY()/mass)*t*t;
    //std::cout<<yf<<"\n";
    Point newCenter(xf, yf);
    center = newCenter;
}

void Particle::setForce(double fx, double fy)
{
    force.setX(fx);
    force.setY(fy);
}

Point Particle::getPos() const
{
    return center;
}
Point Particle::getVeloc() const
{
    return velocity;
}
Point Particle::getForce() const
{
    return force;
}
void Particle::setXPos(double d)
{
    center.setX(d);
}
void Particle::setYPos(double d)
{
    center.setY(d);
}
void Particle::setVeloc(double x, double y)
{
    Point newVeloc(x,y);
    velocity = newVeloc;
}
void Particle::setMass(double m){
    mass = m;
}
void Particle::bounceU()
{
    velocity = Point(velocity.getX(), -velocity.getY());
    setYPos(radius);
}
void Particle::bounceD(double h)
{
    velocity = Point(velocity.getX(), -velocity.getY());
    setYPos(h-radius);
}
void Particle::bounceL()
{
    velocity = Point(-velocity.getX(), velocity.getY());
    setXPos(radius);
}
void Particle::bounceR(double w)
{
    velocity = Point(-velocity.getX(), velocity.getY());
    setXPos(w+radius);
}
