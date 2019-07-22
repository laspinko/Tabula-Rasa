#ifndef POINT_H
#define POINT_H
struct point
{
    double x, y;
    point(double _x = 0, double _y = 0);
    friend point operator+(const point& a, const point& b);
    friend point operator-(const point& a, const point& b);
    friend point operator*(const point& a, double b);

    point operator+=(const point& other);
    point operator-=(const point& other);

    point normalize();
};

#endif // POINT_H
