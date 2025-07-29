
#include <fstream>
#include <thread>

#include "Simulator.h"
#include "Utils.h"

using namespace std;

Coordinate *Simulator::BASE_COORDINATES = new Coordinate(35.8894, -5.3213);
double Simulator::SCALE = 2.0;



void Simulator::insertNode(Node *node){ (*nodes)[node->id] = node; }

void Simulator::insertVia(Via  *via){
    vias->push_back(via);
    (*viasVehicles)[via->id] = new vector<vector<Vehicle*>*>();
    for(size_t i = 0; i<via->numberOfLanes; i++){
        (*viasVehicles)[via->id]->push_back(new vector<Vehicle*>());
    }
}

void Simulator::load(){

    ifstream isn;
    isn.open("/home/jose/externo/projects/codeblocks/TraSim2/min_nodes.csv");
    string  line;
    getline(isn,line);
    while( !isn.eof() ){
        getline(isn,line);
        if(line.compare("")!=0){
            Node::fromCsv(line,this);
        }
    }
    isn.close();

    ifstream isv;
    isv.open("/home/jose/externo/projects/codeblocks/TraSim2/min_adjacency.csv");


    while( !isv.eof() ){
        getline(isv,line);
        cout << line << "*" << endl;
        if(line.compare("")!=0){
            line = Utils::strip(line);
            Via::fromCsv(line, this);
        }
    }
    cout << "K" << endl;
    isv.close();



cout << "D" << endl;
    ifstream isc;
    isc.open("/home/jose/externo/projects/codeblocks/TraSim2/min_connections.csv");
    while( !isc.eof() ){
        getline(isc,line);
        cout << line << "*" << endl;
        if(line.compare("")!=0){

            line = Utils::strip(line);
            vector<string> fields = Utils::split(line,';');
            Via *v1 = getVia(fields[0]+"-"+fields[1]);
            Via *v2 = getVia(fields[1]+"-"+fields[2]);
            Node *n = (*nodes)[fields[1]];
            size_t l1 = stoi(fields[3]);
            size_t l2 = stoi(fields[4]);
            n->insertConnection(v1,v2,l1,l2,3, Via::GENERIC_SPEED_LIMIT);
        }
    }
    isc.close();

    /*
    for(Via *v1 :  *vias){
        for(Via *v2: *vias){
            if(v1->node2->id.compare(v2->node1->id)==0 ){
                Node *n = (*nodes)[v1->node2->id];
                n->insertConnection(v1, v2, 0, 0, 3, Via::GENERIC_SPEED_LIMIT);
            }
        }
    }
    */


    ifstream issem;
    issem.open("/home/jose/externo/projects/codeblocks/TraSim2/min_semaphores.csv");
    semaphores = TrafficSemaphoresSystem::fromCsv(&issem,this);
    issem.close();


}

void Simulator::run(){

    //int *pos;
    t = t0;
    while( t < t1){
 if(t%100==0){
    cout << t0 << " " << t << " " << t1 << " " << vehicles->size() << endl;
 }
        //pos= new int[vehicles->size()];
        for(size_t i = 0 ; i <2; i++){
            /*for(size_t i = 0; i< vehicles->size(); i++){
                pos[i] = i;
            }

            for(size_t i = 0; i< vehicles->size(); i++){
                int j = rand()%(vehicles->size());
                int k = rand()%(vehicles->size());
                int aux = pos[j];
                pos[j] = pos[k];
                pos[k] = aux;
            }
*/
            for(size_t i = 0; i< vehicles->size(); i++){
                //(*vehicles)[pos[i]]->move(0.5);
                (*vehicles)[i]->move(0.5);
            }
        }
        //delete[] pos;

        for( Via *via : *vias){
            via->incorporateVehicleProb();
            via->vanishVehicleProb();
        }

        t++;
    }
    cout << vehicles->size() << endl;

}

Via *Simulator::getVia(string id){
    for(Via *v : *vias){
        if(v->id.compare(id)==0){
            return v;
        }
    }
    return nullptr;
}


ostream &operator<<(ostream &o, Simulator *s){
    o << "Simulator";
    o << "Initial time" << Utils::timetostring(s->t0) << endl;
    o << "End time" << Utils::timetostring(s->t1) << endl;
    o << "Vehicles:" << endl;
    for(Vehicle *v : *(s->vehicles)){
        o << v << endl;
    }

    o << "Nodes:" << endl;
    for(map<string, Node*>::iterator it = s->nodes->begin(); it!=s->nodes->end(); it++){
        o << it->second << endl;
    }

    o << "Vias:" << endl;
    for(Via *v : *(s->vias)){
        o << v << endl;
    }

    o << "Coordinates origin: " << s->origin << endl;
    return o;
}


void start_simulator(){

}


void Simulator::insertVehicleInViaLane(Via * via, Vehicle * vehicle, bool alreadyExists){

    // insert vehicle
    (*(*viasVehicles)[via->id])[vehicle->lane]->push_back(vehicle);

    // insert in general vehicle catalogue if necessary
    if(!alreadyExists){
        vehicles->push_back(vehicle);
    }

}

void Simulator::removeVehicleFromViaLane(Via *via, Vehicle *vehicle){
    vector<Vehicle*> *v = (*(*viasVehicles)[via->id])[vehicle->lane];
    vector<Vehicle*>::iterator it = v->begin();
    while(it != v->end() && (*it)!=vehicle ){
        it++;
    }

    v->erase(it);
}

void Simulator::removeRandomVehicleFromSimulationLane0(Via *via){

    if((*viasVehicles)[via->id]->size()>0){
        vector<Vehicle*> *vs = (*(*viasVehicles)[via->id])[0];
        if(vs->size()>0){
            size_t p = rand()%vs->size();
            vector<Vehicle*>::iterator it = vs->begin();
            it = it+p;
            Vehicle *v = *it;
            vs->erase(it);

            it = vehicles->begin();
            while( it!=vehicles->end() && *it!=v ){
                it++;
            }
            vehicles->erase(it);
        }
    }


}




vector<Vehicle*> *Simulator::getVehiclesInViaLane(Via *via, size_t lane){
    if(viasVehicles->find(via->id)==viasVehicles->end()){
        return new vector<Vehicle*>();
    }
    else{
        return (*(*viasVehicles)[via->id])[lane];
    }

}

vector<Vehicle*> *Simulator::getAllVehicles(){
    return vehicles;
}
