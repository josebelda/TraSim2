#include "Vector2D.h"

#include <cmath>


Vector2D *Vector2D::add(Vector2D *v){
    return new Vector2D(this->x+v->x, this->y+v->y);
}

Vector2D *Vector2D::sub(Vector2D *v){
    return new Vector2D(this->x-v->x, this->y-v->y);
}

ostream &operator<<(ostream &o, Vector2D *v){
    o << "(" << v->x << "," << v->y << ")";
    return o;
}

double Vector2D::mul(Vector2D *v){
    return this->x*v->x+this->y*v->y;
}

Vector2D *Vector2D::mul(double k){
    return new Vector2D(k*this->x,k*this->y);
}

double Vector2D::sqModule(){
    return this->mul(this);
}

double Vector2D::module(){
    return sqrt(this->sqModule());
}

double Vector2D::cosAngle(Vector2D *v){
    return (this->mul(v))/(this->module()*v->module());
}

double Vector2D::angle(Vector2D *v){
    return acos(this->cosAngle(v));
}

double Vector2D::distance(Vector2D *v){
    Vector2D *d = this->sub(v);
    double dist = d->module();
    delete d;
    return dist;
}

bool Vector2D::hasSameSense(Vector2D *v){
    return this->cosAngle(v)>0;
}

Vector2D *Vector2D::unitary(){
    return mul(1/module());
}
