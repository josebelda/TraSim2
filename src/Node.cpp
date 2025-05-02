#include "Node.h"

void Node::insertConnection(Via *vIn, Via *vOut, double timeToCross, double celerityToEnter){
    map<string, vector<Connection*>*>::iterator c = connections.find(vIn->name);
    if(c==connections.end()){
        connections[vIn->name]->push_back(new Connection(vIn, vOut, timeToCross,celerityToEnter));
    }
    else{
        vector<Connection*> *v  = new vector<Connection*>();
        v->push_back(new Connection(vIn, vOut, timeToCross,celerityToEnter));
        connections[vIn->name] = v;
    }
}


vector<Connection*> *Node::possibleViasOut(Via *vIn){
    map<string, vector<Connection*>*>::iterator c = connections.find(vIn->name);
    if(c==connections.end()){
        return nullptr;
    }
    else{
        return connections[vIn->name];
    }
}
