#include "Vehicle.h"

#include <stdlib.h>

/*
const double Vehicle::DEFAULT_LENGTH = 4.0;
const double Vehicle::ACCELERATION_FORWARD = 5.65; // APROX = g/sqrt(3) produces a pendulum to deviate 30 degrees from vertical
const double Vehicle::MODERATE_ACCELERATION_BRAKE = -5.65; // APROX = g/sqrt(3) produces a pendulum to deviate 30 degrees from vertical
const double Vehicle::SAFETY_DISTANCE = 2.0;
const double Vehicle::DT = 1E-3;
*/

const double Vehicle::DEFAULT_LENGTH = 4.0;
const double Vehicle::MAX_ACCELERATION_FORWARD = 5.65;
const double Vehicle::MAX_ACCELERATION_BRAKE  = -8.0;
const double Vehicle::MODERATE_ACCELERATION_BRAKE = -2;
const double Vehicle::SAFETY_DISTANCE = 2;
const double Vehicle::DT = 1E-3;
const double Vehicle::VDELTA_MAX = 13.88;  // 50Km/h

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

SituationAhead *Vehicle::getDistanceAhead(Via *vc){

    Via *viaToConsider;
    size_t laneToConsider;

    if(vc==nullptr){
        viaToConsider = via;
        laneToConsider = lane;
    }
    else{
        viaToConsider = vc;
        if(viaToConsider->numberOfLanes>lane){
            laneToConsider = lane;
        }
        else{
            laneToConsider = viaToConsider->numberOfLanes-1;
        }
    }

    SituationAhead * sa = new SituationAhead();
    for(Vehicle *v : *(simulator->getVehiclesInViaLane(viaToConsider, laneToConsider))){
        Vector2D *aux = v->position->sub(position);
        if(aux->hasSameSense(via->n)){
            double daux = getDistance(v);
            if(daux<sa->distanceAhead){
                sa->distanceAhead = daux;
                sa->celerityVehicleAhead = v->celerity;
            }
        }
        delete aux;
    }

    sa->distanceToEndOfVia = via->node2->position->distance(position);
    if(sa->distanceAhead >=9999){
        sa->distanceAhead = sa->distanceToEndOfVia;
    }

    return sa;
}

// d = Free distance ahead of vehicle
SituationAhead *Vehicle::calcAcceleration(){

    SituationAhead *sa = getDistanceAhead();
    SituationAhead *saNextVia = getDistanceAhead(nextVia);
    // -------TO DO !!!!!! TAKE INTO ACCOUNT IF NEXT NODE IS CONTINUITY NODE
    if(sa->distanceAhead==sa->distanceToEndOfVia && via->node2->isContinuity){
        sa->distanceAhead = sa->distanceAhead + saNextVia->distanceAhead;
    }
    delete saNextVia;

    double df = getSubjectiveSafetyDistance();
    if(sa->distanceAhead>=df){
        // Calculate ap acceleration forward
        // TO DO Why tthe constants 0.02 and 0.1 ?
        if(0.02*sa->distanceAhead<MAX_ACCELERATION_FORWARD){
            a = 0.1*sa->distanceAhead;
        }
        else{
            a = MAX_ACCELERATION_FORWARD;
        }
    }
    else{
        // CAlculat af braking acceleration
        if(sa->distanceAhead==sa->distanceToEndOfVia ){
            // If close to intersection and his semaphore is green then do not brake
            // TO DO Check if this is correct
            if( !(sa->distanceAhead<10 && simulator->semaphores->getLight(via, nextVia,lane,nextLane)==GREEN) ){
                if(sa->celerityVehicleAhead>=0){
                    double vdelta = celerity - sa->celerityVehicleAhead;
                    a = -0.5*vdelta*vdelta/sa->distanceAhead;
                }
                else{
                    if(celerity <=0 ){
                        a = 0.1*sa->distanceAhead;
                    }
                    else{
                        a = -0.5 * celerity*celerity/sa->distanceAhead;
                    }
                }
            }
        }


    }

    return sa;
}



void Vehicle::tryToChangeLane(){
    vector<Vehicle*> *vehicles = simulator->getVehiclesInViaLane(via, laneToMoveTo);
     if( vehicles->size()>0){
         vector<Vehicle*>::iterator it = vehicles->begin();
         bool currentLanePositionpassed = false;
         Vehicle *v = *it;;
         while( it!=vehicles->end() && !currentLanePositionpassed ){
            Vector2D *aux = v->position->sub(position);
            if( via->n->hasSameSense(aux) ){
                currentLanePositionpassed = true;
            }
            else{
                it++;
            }
            delete aux;
         }

         vector<Vehicle*>::iterator it2 = it-1;
         Vehicle *prev = *it2;
         Vehicle *fol = *it;
         if( prev->position->distance(position)>length+5 ){
            if(fol->position->distance(position)>v->length+5){
                simulator->removeVehicleFromViaLane(via, this);
                lane = laneToMoveTo;
                simulator->insertVehicleInViaLane(via, this, true);
            }
         }
     }
     else{
        simulator->removeVehicleFromViaLane(via, this);
        lane = laneToMoveTo;
        simulator->insertVehicleInViaLane(via, this, true);
     }
}


void Vehicle::move(double dt){
    // TO DO QUE SI EL VEHICULO SE APROXIMA A UNA INTERSECCION Y EL SEMAFORO ESTA VERDE NO FRENE
    if( via!=nullptr ){
        // try to move to another lane if it has to and if it is possible.
        if(lane!=laneToMoveTo){
            tryToChangeLane();
        }
        SituationAhead *sa = calcAcceleration();
        if( sa->distanceToEndOfVia<=1 ){
            if(via->node2->isContinuity){

                simulator->removeVehicleFromViaLane(via,this);
                previousVia = via;
                via = nextVia;
                lane = nextLane;
                simulator->insertVehicleInViaLane(via, this, true);
                node = nullptr;
                Vector2D *aux = via->n->mul(length);
                position = via->node1->position->add(aux);
                delete aux;
                decideNextVia();
            }
            else{

                if(lane == laneToMoveTo){
                    // if it is in the correct lane to go to the destination via and lane
                    if( via->node2->canEnterConnection(via, nextVia, lane, nextLane, length) ){
                        via->node2->vehicleStartCrossing(via, nextVia, lane, nextLane, this);
                        node = via->node2;
                        a = 0;
                        celerity = celerityToEnter;
                        simulator->removeVehicleFromViaLane(via,this);
                        previousVia = via;
                        via = nullptr;
                    }
                    else{
                        celerity = 0;
                        a = 0;
                    }
                }
                else{
                    // Try to enter the next possible via according to current lane
                    lane = laneToMoveTo;
                    vector<Connection*> *possibleViasNext = via->node2->possibleViasOut(via, lane, previousVia);
                    nextVia = (*possibleViasNext)[0]->viaOut;
                    nextLane = (*possibleViasNext)[0]->laneOut;

                    celerityToEnter  = (*possibleViasNext)[0]->celerityToEnter;
                    timeToCrossNode = (*possibleViasNext)[0]->timeToCross;

                    if( via->node2->canEnterConnection(via, nextVia, lane, nextLane, length) ){
                        via->node2->vehicleStartCrossing(via, nextVia, lane, nextLane, this);
                        node = via->node2;
                        a = 0;
                        celerity = celerityToEnter;
                        simulator->removeVehicleFromViaLane(via,this);
                        previousVia = via;
                        via = nullptr;
                    }
                    else{
                        celerity = 0;
                        a = 0;
                    }
                }
            }



        }
        else{
            if(celerity + a*dt<0){
                a = 0;
                celerity = 0;
            }
            else{
                if(celerity+a*dt>via->speedLimit){
                    celerity=via->speedLimit;
                    a=0;
                }
                else{
                    if(sa->distanceAhead<2){
                        a = 0;
                        celerity = sa->celerityVehicleAhead;
                    }
                }
                Vector2D *aux = via->n->mul(celerity * dt + 0.5 * a * dt *dt);
                Vector2D *aux2 = position->add(aux);
                delete position;
                delete aux;
                position = aux2;
                celerity = celerity + a* dt;
            }

        }
        delete sa;
    }
    else{
        timeToCrossNode -= dt;
        if( timeToCrossNode<=0 ){
            node->vehicleExitsCrossing(this);
            via = nextVia;
            lane= nextLane;
            simulator->insertVehicleInViaLane(via, this, true);
            celerity = celerityToEnter;
            a = 0;
            node = nullptr;
            Vector2D *aux = via->n->mul(length);
            position = via->node1->position->add(aux);
            delete aux;
            decideNextVia();
        }
    }
}

void Vehicle::run(){

}
/*
bool Vehicle::positionIsInVia(){

}
*/
void start_vehicle(Vehicle * v){

}


ostream &operator<<(ostream &o, Vehicle *v){
    o << "Vehicle";

    if(v->via!=nullptr){
        o << "Circulating on via: " << endl;
        o << v->via << endl;
    }

    if(v->node!=nullptr){
        o << "Crsossing intersection: " << endl;
        o << v->node <<endl;
        o << "Next Via: " << endl;
        o << v->nextVia << endl;
        o << "Celerity to enter via: " << v->celerityToEnter << endl;
        o << "Time to cross node: " << v->timeToCrossNode << endl;
    }

    o << "Position: " << v->position << endl;
    o << "Acceleration: " << v->a << endl;
    o << "Celerity: " << v->celerity << endl;
    return o;
}

bool Vehicle::positionIsInVia(){
    return Vector2D::areLinearDependent(via->node1->position, via->node2->position, position);
}

void Vehicle::decideNextVia(){

    size_t il = rand()%via->numberOfLanes;
    laneToMoveTo = il;
    vector<Connection*> *possibleViasNext = via->node2->possibleViasOut(via, il, previousVia);

    //default_random_engine gen;
    //uniform_int_distribution<int> *discUniform = new uniform_int_distribution<int>(0,possibleViasnext->size());
    //int iv = (*discUniform)(gen);

    //cout << rand();
    //int iv = (int)(rand()*((double)possibleViasnext->size()));

    int iv = rand()%(possibleViasNext->size());
    nextVia = (*possibleViasNext)[iv]->viaOut;
    nextLane = (*possibleViasNext)[iv]->laneOut;

    celerityToEnter  = (*possibleViasNext)[iv]->celerityToEnter;
    timeToCrossNode = (*possibleViasNext)[iv]->timeToCross;
}
