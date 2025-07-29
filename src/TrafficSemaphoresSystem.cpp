#include <TrafficSemaphoresSystem.h>
#include "Utils.h"

int TrafficSemaphoresSystem::UNDEFINED_LIGHT_EXCEPTION = 2002;

void TrafficSemaphoresSystem::addConnectionInterval(
    Via *viaFrom,
    Via *viaTo,
    size_t laneFrom,
    size_t laneTo,
    int timeFrom,
    int timeTo,
    enum Light light){


    for(int i=timeFrom; i<timeTo; i++){
        cycle[viaFrom->id][laneFrom][viaTo->id][laneTo][i] = light;
    }
}

enum Light TrafficSemaphoresSystem::getLight(Via *viaFrom, Via *viaTo, size_t laneFrom, size_t laneTo){


    if(cycle.find(viaFrom->id)==cycle.end()){
        return NO_SEMAPHORE;
    }
    if(cycle[viaFrom->id].find(laneFrom)==cycle[viaFrom->id].end()){
        return NO_SEMAPHORE;
    }
    if(cycle[viaFrom->id][laneFrom].find(viaTo->id)==cycle[viaFrom->id][laneFrom].end()){
        return NO_SEMAPHORE;
    }
    if(cycle[viaFrom->id][laneFrom][viaTo->id].find(laneTo)==cycle[viaFrom->id][laneFrom][viaTo->id].end()){
        return NO_SEMAPHORE;
    }
    int t = (simulator->t-simulator->t0)%cycleLength;
    return cycle[viaFrom->id][laneFrom][viaTo->id][laneTo][t];

}

TrafficSemaphoresSystem *TrafficSemaphoresSystem::fromCsv(ifstream *f, Simulator *sim){
    // format  viaFromId;laneFrom;viaToId;laneTo;timeFrom;TimeTo,light
    //            0          1        2      3      4        5     6
    TrafficSemaphoresSystem *semaphores = new TrafficSemaphoresSystem(sim);

    string line;
    while( !f->eof() ){
        getline(*f,line);
        if(line.compare("")!=0){
            vector<string> fields = Utils::split(line, ';');
            string nodeid1 = fields[0];
            string nodeid2 = fields[1];
            string nodeid3 = fields[2];

            Via *viaFrom = sim->getVia(nodeid1+"-"+nodeid2);
            Via *viaTo = sim->getVia(nodeid2+"-"+nodeid3);
            size_t laneFrom = stoi(fields[1]);
            size_t laneTo = stoi(fields[3]);
            int timeFrom = stoi(fields[4]);
            int timeTo = stoi(fields[5]);
            Light light = NO_SEMAPHORE;
            if(fields[6].compare("RED")==0){
                light = RED;
            }
            else if(fields[6].compare("YELLOW")==0){
                light = YELLOW;
            }
            else if(fields[6].compare("GREEN")==0){
                light = GREEN;
            }
            else if(fields[6].compare("INTERMITTENT_YELLOW")==0){
                light = INTERMITENT_YELLOW;
            }
            else{
                throw UNDEFINED_LIGHT_EXCEPTION ;
            }
            semaphores->addConnectionInterval(viaFrom, viaTo,laneFrom,laneTo,timeFrom, timeTo, light);
        }
    }
    return semaphores;
}
