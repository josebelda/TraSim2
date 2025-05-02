#ifndef VEHICLE_H
#define VEHICLE_H

#include <ctime>

#include "Simulator.h"
#include "Via.h"
#include "Node.h"

class Via;
class Node;
class Simulator;

class Vehicle
{
    public:
        static const double DEFAULT_LENGTH;
        static const double ACCELERATION_FORWARD;
        static const double ACCELERATION_BRAKE;
        static const double SAFETY_DISTANCE;
        static const double DT;

        Simulator *simulator;

        Via *via = nullptr; // via is where the vehicle is circulating
        Node *node = nullptr; // node the vehicle is crossing
        Vector2D *position = nullptr; // current position of the node in m from origin of coordinates of the town
        double celerity = 0; // celerity of the vehicle
        time_t ts; // last timestamp when the vehicle moved
        double length = DEFAULT_LENGTH; // length of the vehicle
        Via *nextVia = nullptr; // vie the vehicle is going to enter next when the vehicle is inside a node
        double celerityToEnter = 0; // Celerity with which the vehicle will enter the next via
        double timeToCrossNode = 0; // time needed to cross the node the vehicle is in
        bool die = false;
        int c = 0;


        Vehicle(Simulator *sim):simulator(sim){}
        virtual ~Vehicle(){}

        void kill(){ die = true; }
        Vector2D *getRear();
        double getSafetyDistance(){ return (-0.5 * celerity * celerity / ab) + SAFETY_DISTANCE; }
        double getDistance(Vehicle *v);

    protected:

    private:
        double af = ACCELERATION_FORWARD; // acceleration the vehicle is capable of forward
        double ab = ACCELERATION_BRAKE; // acceleration the vehicle is capable of when braking
};

void start_vehicle(Vehicle * v);

#endif // VEHICLE_H
