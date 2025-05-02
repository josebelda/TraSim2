#include "Coordinate.h"

#include <cmath>

const double Coordinate::EARTH_RADIUS = 6371000.0;
const double Coordinate::M_DEGREE = 2.0*M_PI*EARTH_RADIUS/360.0;

ostream &operator<<(ostream &o, Coordinate *c){
    o << "(lat=" << c->latitude << ",long=" << c->longitude << ")";
    return o;
}

Vector2D *Coordinate::translate(Coordinate *origin){
    return new Vector2D((latitude - origin->latitude)*M_DEGREE, (longitude - origin->longitude)*M_DEGREE);
}
