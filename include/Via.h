#ifndef VIA_H
#define VIA_H

#include <string>
#include <sstream>
#include <ctime>
#include <random>

#include "Node.h"
#include "Vehicle.h"
#include "Simulator.h"

using namespace std;

class Node;
class Vehicle;
class Simulator;

class PositionVehicle{
    public:
        double position;
        Vehicle *vehicle;


        PositionVehicle(double pos, Vehicle *v):position(pos),vehicle(v){}

};

class Via
{
    public:
        const double SAFETY_DISTANCE_TO_INCORPORATE = 50.0;

        const double GENERIC_SPEED_LIMIT = 11.11; // 11.11 m/s = approx 40 Km/h

        static int nextId;

        string id;
        Node *node1;
        Node *node2;
        double vehiclesCreationRate;
        double vehiclesVanishingRate;
        string name;

        Simulator *simulator;
        Vector2D *n;
        double length;
        double speedLimit;
        time_t lastTimeVehicleCreated;


        default_random_engine gen;
        uniform_int_distribution<int> *discUniform;

        Via(Node *n1,
            Node *n2,
            double vcr,
            double vvr,
            string n,
            Simulator *sim);
        virtual ~Via(){}

        bool canIncorporateToPosition(vector<PositionVehicle*> *ps, double p, double length);
        Vector2D *getFreePosition(double length);
        void incorporateVehicleProb();

    protected:

    private:
};

#endif // VIA_H
