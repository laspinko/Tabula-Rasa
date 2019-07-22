#include "point.h"

#include<cmath>

point::point(double _x, double _y)
{
    x = _x;
    y = _y;
}

point operator+(const point &a, const point &b)
{
    return point(a.x + b.x, a.y + b.y);
}

point operator-(const point &a, const point &b)
{
    return point(a.x - b.x, a.y - b.y);
}

point operator*(const point &a, double b)
{
    return point(a.x * b, a.y * b);
}

point point::operator+=(const point &other)
{
    x += other.x;
    y += other.y;
    return *this;
}

point point::operator-=(const point &other)
{
    x -= other.x;
    y -= other.y;
    return *this;
}

point point::normalize()
{
    double dist = sqrt(x * x + y * y);
    if(dist == 0)   return point(1, 0);
    return point(x / dist, y / dist);
}
