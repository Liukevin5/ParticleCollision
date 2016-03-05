#include "Point.h"
#include <cmath>

Point::Point(){
x =0; y =0;
}

Point::Point(double x0, double y0):x(x0), y(y0)
{
}


double Point::setX(double d){
    x = d;
}
double Point::setY(double d){
    y = d;
}
double Point::getX() const
{
    return x;
}

double Point::getY() const
{
    return y;
}


double Point::distanceFrom(Point p) const{
    double x2 = p.getX();
    double y2 = p.getY();
    double distance = pow(((x-x2)*(x-x2) + (y-y2)*(y-y2)), .5);
    return distance;
}

double Point::distanceFrom(double x2, double y2) const{
    double distance = pow(((x-x2)*(x-x2) + (y-y2)*(y-y2)), .5);
    return distance;
}


