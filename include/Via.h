#ifndef VIA_H
#define VIA_H

#include <string>
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
        static double SAFETY_DISTANCE_TO_INCORPORATE;

        static double GENERIC_SPEED_LIMIT;


        string id;
        Node *node1;
        Node *node2;
        double vehiclesCreationRate;
        double vehiclesVanishingRate;
        string name;
        size_t numberOfLanes;

        Simulator *simulator;
        Vector2D *n;
        double length;
        double speedLimit;
        time_t lastTimeVehicleCreated;


        default_random_engine gen;


        Via(Node *n1,
            Node *n2,
            double vcr,
            double vvr,
            string n,
            size_t nLanes,
            Simulator *sim);
        virtual ~Via(){}

        bool canIncorporateToPosition(vector<PositionVehicle*> *ps, double p, double length);
        Vector2D *getFreePosition(double length, size_t lane);
        void incorporateVehicleProb();
        void vanishVehicleProb();
        bool canEnter(double length, size_t lane);

        friend ostream &operator<<(ostream &o, Via *v);

        static void fromCsv(string line, Simulator *sim);

        Vehicle *incorporateVehicle();
        void vanishVehicle();

    protected:

    private:

        vector<PositionVehicle*> *getPositionsVehicles(size_t lane);
};

#endif // VIA_H
