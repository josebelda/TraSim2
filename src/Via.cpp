#include <cmath>

#include "Via.h"
#include "Utils.h"



double Via::SAFETY_DISTANCE_TO_INCORPORATE = 50.0;
double Via::GENERIC_SPEED_LIMIT = 11.11; // 11.11 m/s = approx 40 Km/h

Via::Via(Node *n1,
        Node *n2,
        double vcr,
        double vvr,
        string nam,
        size_t nLanes,
        Simulator *sim):
            node1(n1),
            node2(n2),
            vehiclesCreationRate(vcr),
            vehiclesVanishingRate(vvr),
            name(nam),
            numberOfLanes(nLanes),
            simulator(sim)
{


    id = node1->id+"-"+node2->id;

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

vector<PositionVehicle*> *Via::getPositionsVehicles(size_t lane){
    vector<PositionVehicle*> *ps = new vector<PositionVehicle*>();
    for( Vehicle *v : *(simulator->getVehiclesInViaLane(this, lane))){

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
    return ps;
}


Vector2D *Via::getFreePosition(double length, size_t lane){
    vector<PositionVehicle*> *ps = getPositionsVehicles(lane);
    uniform_real_distribution<> dis(length, this->length);
    double p = dis(gen);
    int tries = 20;
    while(tries>0){

        if(canIncorporateToPosition(ps, p, length)){

            Vector2D *aux1 = n->mul(p);
            Vector2D *aux2 = node1->position->add(aux1);
            delete aux1;

            return aux2;
        }

        p = dis(gen);
        tries--;
    }
    return nullptr;
}

void Via::incorporateVehicleProb(){
    int x = simulator->t - simulator->t0;
    // Incorporation time follows an exponential distribution
    double fa = 1 - exp(-vehiclesCreationRate * x);
    int n = round(100*fa);
    int r = rand()%100;

    if(r<=n){
        incorporateVehicle();
    }

}

void Via::vanishVehicleProb(){
    int x = simulator->t - simulator->t0;
    double fa = 1 - exp(-vehiclesVanishingRate * x);
    int n = round(100*fa);
    int r = rand()%100;

    if(r<=n){
        vanishVehicle();
    }

}


ostream &operator<<(ostream &o, Via *v){
    o << "Via: " << endl;
    o << "Id :" << v->id << endl;
    o << "Name :" << v->name << endl;
    o << "Node 1: " << v->node1->id << endl;
    o << "Node 2: " << v->node2->id << endl;
    o << "Vehicles creation rate: " << v->vehiclesCreationRate << endl;
    o << "Vehicles vanishing rate: " << v->vehiclesVanishingRate << endl;
    o << "Length: " << v->length << endl;
    o << "Speed limit: " << v->speedLimit << endl;

    return o;
}

void Via::fromCsv(string line, Simulator * sim){

    // Format
    //<idNode1>;<idNode2>;<S/D single sense, double sendse>;;<vehicleCreationRate>;<vehiclesVanishingRate>;;;<name>;<number of lanes>"

    vector<string> fields = Utils::split(line, ';');
    string in1 = fields[0];
    string in2 = fields[1];
    string senses = fields[2];
    double vehiclesCreationRate;
    if(fields.size()<5 || fields[4].compare("")!=0){
        vehiclesCreationRate= 0.001;
    }
    else{
        vehiclesCreationRate = stod(fields[4]);
    }

    double vehiclesVanishingRate;
    if(fields.size()<6 || fields[5].compare("")!=0){
        vehiclesVanishingRate = vehiclesCreationRate;
    }
    else {
        vehiclesVanishingRate = stod(fields[5]);
    }

    string name;
    if(fields.size()<9 || fields[8].compare("")!=0){
        name = "";
    }
    else{
        name = fields[8];
    }

    size_t nLanes;
    if(fields.size()<10 || fields[8].compare("")!=0){
        nLanes = 1;
    }
    else{
        nLanes = stoi(fields[9]);
    }

    Node *n1 = (*(sim->nodes))[in1];
    Node *n2 = (*(sim->nodes))[in2];
    sim->insertVia(
        new Via(n1, n2, vehiclesCreationRate, vehiclesVanishingRate, name, nLanes, sim)
    );
    if(senses.compare("D")==0){
        sim->insertVia(
            new Via(n2, n1, vehiclesCreationRate, vehiclesVanishingRate, name, nLanes, sim)
        );
    }
}


bool Via::canEnter(double length, size_t lane){
    vector<PositionVehicle*> *ps = getPositionsVehicles(lane);
    bool ci =  canIncorporateToPosition(ps, length, length);
    delete ps;
    return ci;
}

Vehicle *Via::incorporateVehicle(){
    Vector2D *p = getFreePosition(Vehicle::DEFAULT_LENGTH, 0);

    if(p!=nullptr){

        Vehicle *v = new Vehicle(simulator);
        v->via=this;
        v->position = p;
        v->celerity = 5.55; // aprox 20Km/h
        v->a = 0;
        v->lane=0;
        simulator->insertVehicleInViaLane(this, v);
        v->decideNextVia();
        return v;
    }

    return nullptr;
}

void Via::vanishVehicle(){

    simulator->removeRandomVehicleFromSimulationLane0(this);
}
