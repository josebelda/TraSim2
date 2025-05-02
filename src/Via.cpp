#include <cmath>

#include "Via.h"



int Via::nextId = 0;

Via::Via(Node *n1,
        Node *n2,
        double vcr,
        double vvr,
        string nam,
        Simulator *sim):
        node1(n1),node2(n2),vehiclesCreationRate(vcr),vehiclesVanishingRate(vvr),name(nam),simulator(sim)
{

    discUniform = new uniform_int_distribution<int>(0,99);

    stringstream ss;
    ss << nextId;
    nextId++;
    id = ss.str();

    Vector2D *d = node2->position->sub(node1->position);
    n = d->unitary();
    length = d->module();
    speedLimit = GENERIC_SPEED_LIMIT;
    lastTimeVehicleCreated = simulator->t0;
}

bool Via::canIncorporateToPosition(vector<PositionVehicle*> *ps, double p, double length){

    for(PositionVehicle *posv : *ps){
        if( posv->position > p + posv->vehicle->length){
            return true;
        }
        else{
            if( p - length - posv->position <= SAFETY_DISTANCE_TO_INCORPORATE){
                return false;
            }
        }
    }
    return true;
}

Vector2D *Via::getFreePosition(double length){
    vector<PositionVehicle*> *ps = new vector<PositionVehicle*>();
    for( Vehicle *v : *(simulator->vehicles)){
        if(v->via == this){
            Vector2D *aux1 = v->position->sub(node1->position);
            double p = aux1->module();
            delete aux1;
            if(p>=SAFETY_DISTANCE_TO_INCORPORATE+length){
                vector<PositionVehicle*>::iterator it = ps->begin();
                while(it!=ps->end() && (*it)->position<p){
                    it++;
                }
                PositionVehicle *pv = new PositionVehicle(p,v);
                if(it==ps->end()){
                    ps->push_back(pv);
                }
                else{
                    ps->insert(it, pv);
                }
            }
        }
    }
    uniform_real_distribution<> dis(0, this->length);
    double p = dis(gen);
    int tries = 20;
    while(tries>0){
        if(p>=SAFETY_DISTANCE_TO_INCORPORATE + length){
            if(canIncorporateToPosition(ps, p, length)){
                Vector2D *aux1 = n->mul(p);
                Vector2D *aux2 = node1->position->add(aux1);
                delete aux1;
                return aux2;
            }
        }
        tries--;
    }
    return nullptr;
}

void Via::incorporateVehicleProb(){
    int x = simulator->t - simulator->t0;
    double fa = 1 - exp(-vehiclesCreationRate * x);
    int n = round(100*fa);
    int r = (*discUniform)(gen);
    if(r<=n){
        Vector2D *p = getFreePosition(Vehicle::DEFAULT_LENGTH);
        if(p!=nullptr){
            Vehicle *v = new Vehicle(simulator);
            v->via=this;
            v->position = p;
            simulator->vehicles->push_back(v);
            start_vehicle(v);
        }
    }

}
