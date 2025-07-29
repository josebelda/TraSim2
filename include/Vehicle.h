#ifndef VEHICLE_H
#define VEHICLE_H

#include <ctime>
#include <random>

#include "Simulator.h"
#include "Via.h"
#include "Node.h"

class Via;
class Node;
class Simulator;
class Connection;

using namespace std;

class SituationAhead{
    public:
        double distanceAhead;
        double distanceToEndOfVia;
        double celerityVehicleAhead;

        SituationAhead():
            distanceAhead(10000),
            distanceToEndOfVia(10000),
            celerityVehicleAhead(-1){}
        virtual ~SituationAhead(){}
};

class AccelerationAndOther{
    public:
        double a, distanceToEndOfVia;
        bool mustStopAbruptly;

    AccelerationAndOther(){}
     virtual ~AccelerationAndOther(){}
};

class Vehicle
{
    public:
        static const double DEFAULT_LENGTH;
        static const double MAX_ACCELERATION_FORWARD;
        static const double MAX_ACCELERATION_BRAKE;
        static const double MODERATE_ACCELERATION_BRAKE;
        static const double SAFETY_DISTANCE;
        static const double DT;
        static const double PSICOLOGICAL_MINIMUM_DISTANCE;
        static const double VDELTA_MAX;

        Simulator *simulator;

        Via *via = nullptr; // via is where the vehicle is circulating
        size_t lane; // lane the vehicle is occupying
        Vector2D *position = nullptr; // current position of the vehicle

        Via *nextVia = nullptr; // vie the vehicle is going to enter next when the vehicle is inside a node
        size_t nextLane; // Lane the vehicle will enter in the next via
        double celerityToEnter = 0; // Celerity with which the vehicle will enter the next via
        size_t laneToMoveTo= -1; // Lane which the vehicle has to move to fromthe current one.

        Via *previousVia = nullptr; // Via the vehicle came from in the last Node. (In order to prevent the vehicle from doing a U turn at the next node)

        Node *node = nullptr; // node the vehicle is crossing
        double timeToCrossNode = 0; // time needed to cross the node the vehicle is in

        double a; // Acceleration
        double celerity = 0; // celerity of the vehicle
        time_t ts; // last timestamp when the vehicle moved

        double length = DEFAULT_LENGTH; // length of the vehicle



        bool die = false;
        int c = 0;

        default_random_engine gen();
        uniform_int_distribution<int> *discUniform;


        Vehicle(Simulator *sim):simulator(sim){}
        virtual ~Vehicle(){}

        void kill(){ die = true; }
        Vector2D *getRear();
        double getSubjectiveSafetyDistance(){
            return (-0.5 * celerity * celerity / MODERATE_ACCELERATION_BRAKE);
        }

        double getDistance(Vehicle *v);
        SituationAhead *getDistanceAhead(Via *via=nullptr);
        SituationAhead *calcAcceleration();
        void move(double dt);
        void draw(){}
        void decideNextVia();


        void run();

        bool positionIsInVia();
        friend ostream &operator<<(ostream &o, Vehicle *n);

    protected:

    private:
        void tryToChangeLane();

};

void start_vehicle(Vehicle * v);

#endif // VEHICLE_H
