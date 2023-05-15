#ifndef header
#define header
#include <iostream>
#include <cmath>

namespace ariel
{
    
}
class Point
{
    public:
        double x;
        double y;

        Point();
        Point(double xcd, double ycd);

        double getX();
        double getY();
        double distance(const Point &pnt) const;
        void print();
        static Point moveTowards(const Point &_p1, const Point &_p2, double dist);
};
#endif