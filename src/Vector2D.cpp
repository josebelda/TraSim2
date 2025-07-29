#include "Vector2D.h"

#include <cmath>
#include <iostream>

#include "Utils.h"

using namespace std;

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

bool Vector2D::areLinearDependent(Vector2D *v1, Vector2D *v2, Vector2D *v3){


    Vector2D *aux1 = v2->sub(v1);
    Vector2D *aux2 = v3->sub(v1);
    Vector2D *n1 = aux1->unitary();
    Vector2D *n2 = aux2->unitary();


    bool r = n1->equals(n2);

    delete aux1;
    delete aux2;
    delete n1;
    delete n2;
    return r;
}

bool Vector2D::equals(Vector2D *v){
    return Utils::roundPrec(x,PREC)==Utils::roundPrec(v->x,PREC) && Utils::roundPrec(y,PREC)==Utils::roundPrec(v->y,PREC);
}
