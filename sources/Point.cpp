#include "Point.hpp"
#include <iostream>
#include <cmath>

using namespace std;

Point::Point() {
    this->x = 0;
    this->y = 0;
}

Point::Point(double xcd, double ycd) {
    this->x = xcd;
    this->y = ycd;
}

double Point::getX()
{
    return this->x;
}

double Point::getY()
{
    return this->y;
}
double Point::distance(const Point &pnt) const{
    return sqrt(pow(this->x - pnt.x, 2) + pow(this->y - pnt.y, 2));
}

void Point::print() {
    std::cout << "(" << this->x << "," << this->y << ")";
}

Point Point::moveTowards(const Point &_p1, const Point &_p2, double dist) {
    if(dist < 0)
        throw std::invalid_argument("Cannot move negative distance");
    double distance = _p1.distance(_p2);
    if (distance <= dist) {
        return _p2;
    }

    double distx = _p2.x - _p1.x;
    double disty = _p2.y - _p1.y;
    double factor = dist / distance;
    double new_X = _p1.x + distx * factor;
    double new_Y = _p1.y + disty * factor;

    return Point(new_X, new_Y);
}