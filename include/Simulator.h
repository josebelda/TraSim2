#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <ctime>
#include <vector>
#include <map>
#include <iostream>

#include "Coordinate.h"
#include "Vehicle.h"
#include "Via.h"
#include "TrafficSemaphoresSystem.h"

class Vehicle;
class TrafficSemaphoresSystem;

class Simulator
{
    public:
        static Coordinate *BASE_COORDINATES;
        static double SCALE;

        time_t t, t0 , t1;



        map<string, Node*> *nodes;
        Coordinate *origin;
        TrafficSemaphoresSystem *semaphores;

        Simulator(time_t pt0, time_t pt1):t0(pt0),t1(pt1){
            vias = new vector<Via*>();
            nodes = new map<string, Node*>();
            vehicles = new vector<Vehicle*>();
            viasVehicles = new map<string,vector<vector<Vehicle*>*>*>();
            origin = new Coordinate(BASE_COORDINATES->latitude, BASE_COORDINATES->longitude);
        }
        virtual ~Simulator(){}

        void insertNode(Node *node);
        void insertVia(Via  *via);

        void load();
        void run();
        vector<Via*> *getVias(){return vias;};
        Via *getVia(string id);

        void insertVehicleInViaLane(Via * via, Vehicle * vehicle, bool alreadyExists=false);
        void removeVehicleFromViaLane(Via *via, Vehicle *vehicle);
        vector<Vehicle*> *getVehiclesInViaLane(Via *via, size_t lane);
        vector<Vehicle*> *getAllVehicles();
        // This always removes vehicles in lane 0
        void removeRandomVehicleFromSimulationLane0(Via *via);


        friend ostream &operator<<(ostream &o, Simulator *s);

    protected:

    private:
        vector<Vehicle*> *vehicles;
        // idVia -> lane(right to left starting at 0) -> vehicles
        map<string,vector<vector<Vehicle*>*>*> *viasVehicles;
        vector<Via*> *vias;
};

void start_simulator();

#endif // SIMULATOR_H
