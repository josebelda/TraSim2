#include "Vehicle.h"


const double Vehicle::DEFAULT_LENGTH = 4.0;
const double Vehicle::ACCELERATION_FORWARD = 1.0;
const double Vehicle::ACCELERATION_BRAKE = -1.0;
const double Vehicle::SAFETY_DISTANCE = 2.0;
const double Vehicle::DT = 1E-3;

Vector2D *Vehicle::getRear(){
    Vector2D *v1 = via->n->mul(length);
    Vector2D *v2 = position->sub(v1);
    delete v1;
    return v2;
}

double Vehicle::getDistance(Vehicle *v){
    Vector2D *v1 = v->getRear();
    Vector2D *v2 = v1->sub(position);
    double d = v2->module();
    delete v1;
    delete v2;
    return d;
}


void start_vehicle(Vehicle * v){

}
