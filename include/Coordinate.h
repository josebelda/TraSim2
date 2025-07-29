#ifndef COORDINATE_H
#define COORDINATE_H

#include <iostream>
#include <Vector2D.h>
#include <cmath>

using namespace std;

class Coordinate
{
    public:
        static const double EARTH_RADIUS;
        static const double M_DEGREE;

        double latitude;
        double longitude;

        Coordinate(double lat, double lon) : latitude(lat),longitude(lon){}
        virtual ~Coordinate(){};
        Vector2D *translate(Coordinate *origin);
        Coordinate *clone(){
            return new Coordinate(latitude,longitude);
        }

        friend ostream &operator<<(ostream &o, Coordinate *c);

    protected:

    private:
};

#endif // COORDINATE_H
