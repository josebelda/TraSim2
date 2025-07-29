#include <string>

#include "Node.h"
#include "Utils.h"

using namespace std;

int Node::UNDEFINED_CONNECTION_EXCEPTION = 1000;
int Node::VEHICLE_NOT_CROSSING_EXCEPTION = 1001;

void Node::insertConnection(Via *vIn, Via *vOut, size_t lIn, size_t lOut, double timeToCross, double celerityToEnter){

    if( connections.find(vIn->id)==connections.end() ){
        connections[vIn->id] = new map<size_t, vector<Connection*>*>();
    }
    if(connections[vIn->id]->find(lIn) == connections[vIn->id]->end()){
       (*connections[vIn->id])[lIn] = new vector<Connection*>();
    }
    (*connections[vIn->id])[lIn]->push_back(new Connection(vIn, vOut, lIn, lOut, timeToCross,celerityToEnter));

}


vector<Connection*> *Node::possibleViasOut(Via *vIn, size_t lIn, Via *previousVia){

     if(connections.find(vIn->id)==connections.end()){
        return nullptr;
    }
    else{
        if(connections[vIn->id]->find(lIn)==connections[vIn->id]->end()){
            return nullptr;
        }
        else{
            vector<Connection*> *cons = (*connections[vIn->id])[lIn];
            vector<Connection*> *r = new vector<Connection*>();
            for(Connection *c : (*cons) ){
                // The vehicle cannot return to its previous via
                if(c->viaOut->id!=previousVia->id){
                    r->push_back(c);;
                }
            }
            return r;
        }
    }
}


void Node::fromCsv(string line, Simulator *sim){

    // Format
    // "POINT (<longitude> <latitude>)",Point <number>,<name>

    vector<string> fields = Utils::split(line, ',');
    string coords = fields[0].substr(8, fields[0].length()-10);
    vector<string> c = Utils::split(coords, ' ');
    double lo = stod(c[0]);
    double lat = stod(c[1]);
    Coordinate *coord = new Coordinate(lat, lo);
    Vector2D *position = coord->translate(sim->origin);
    vector<string> aux = Utils::split(fields[1], ' ');
    string id = aux[1];
    string name = fields[2].substr(0, fields[2].length()-1);
    sim->insertNode( new Node(id, name, position, coord) );

}


ostream &operator<<(ostream &o, Node *n){
    o << "Node id: " << n->id << endl;
    o << "Node name:" << n->name << endl;
    o << "Node coordinates:" << n->coordinates << endl;
    o << "Node position:" << n->position << endl;


    o << "Connections: " << endl;
    for(map<string,map<size_t,vector<Connection*>*>*>::iterator it = n->connections.begin(); it!=n->connections.end(); it++){
        o << "Via in: " << it->first << endl;
        for(map<size_t, vector<Connection*>*>::iterator it2 = it->second->begin(); it2!=it->second->end(); it2++){
            o << "    Lane in: " << it2->first << endl;
            for(Connection *c : (*it2->second)){
                o << "        Via out: " << c->viaOut->id << endl;
                o << "        Lane out: " << c->laneOut << endl;
            }
        }

    }
    return o;
}

ostream &operator<<(ostream &o, Connection *c){
    o << "Connection: " << c->viaIn->id << "," << c->laneIn << "," << c->viaOut->id << "," << c->laneOut;
    return o;
}

bool Node::canEnterConnection(Via *vIn, Via * vOut, size_t laneIn, size_t laneOut, double length){

    Light light = simulator->semaphores->getLight(vIn,vOut,laneIn,laneOut);
    if(light == RED){
        return false;
    }
    else{
        vector<Connection*> *conts = (*connections[vIn->id])[laneIn];
        for(Connection *c : *conts){
            if(c->viaOut==vOut && c->laneOut==laneOut ){
                return ((c->vehicleIn == nullptr) && c->viaOut->canEnter(length, laneOut));
            }
        }
        throw UNDEFINED_CONNECTION_EXCEPTION;
    }


}

void Node::vehicleStartCrossing(Via *vIn, Via * vOut, size_t laneIn, size_t laneOut, Vehicle *v){
    vector<Connection*> *conns =  (*connections[vIn->id])[laneIn];
    for(Connection *c : *conns){
        if(c->viaOut==vOut && c->laneOut==laneOut){
            c->vehicleIn = v;
            return;
        }
    }
    throw UNDEFINED_CONNECTION_EXCEPTION;
}

void Node::vehicleExitsCrossing(Vehicle * v){
    for(map<string, map<size_t,vector<Connection*>*>*>::iterator it = connections.begin();
         it!=connections.end();
         it++){
        for(map<size_t,vector<Connection*>*>::iterator it2 = it->second->begin(); it2!=it->second->end(); it2++){
             for(Connection *c : *(it2->second)){
                if(c->vehicleIn==v){
                    c->vehicleIn = nullptr;
                    return;
                }
             }
         }
    }
    cout << "Vehicle not crossing reporrted exit crossing." << endl;
    throw VEHICLE_NOT_CROSSING_EXCEPTION;
}
