#ifndef POINT_H
#define POINT_H
#include <cmath>
class Point
{

private:
    double x;
    double y;

public:
    Point();
    Point(double x, double y);
    double getX() const;
    double getY() const;
    double setX(double d);
    double setY(double d);
    double distanceFrom(Point p) const;
    double distanceFrom(double x, double y) const;
};
#endif // POINT_H

