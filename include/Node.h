#ifndef NODE_H
#define NODE_H

#include <string>
#include <map>
#include <vector>
#include "Simulator.h"
#include "Via.h"
#include "Vector2D.h"
#include "Vehicle.h"

using namespace std;

class Via;
class Simulator;
class Vehicle;
class TrafficSemaphore;
class Connection{

    public:
        Via *viaIn, *viaOut;
        size_t laneIn, laneOut;
        double timeToCross, celerityToEnter;
        Vehicle *vehicleIn;

        Connection(Via *vIn, Via *vOut, size_t lIn, size_t lOut, double timeToCross, double celerityToEnter):
            viaIn(vIn),
            viaOut(vOut),
            laneIn(lIn),
            laneOut(lOut),
            timeToCross(timeToCross),
            celerityToEnter(celerityToEnter),
            vehicleIn(nullptr){}

        friend ostream &operator<<(ostream &o, Connection *c);
};

class Node
{

    static int UNDEFINED_CONNECTION_EXCEPTION;
    static int VEHICLE_NOT_CROSSING_EXCEPTION;

    public:
        string id;
        string name;
        Vector2D *position;
        Coordinate *coordinates;
        bool isContinuity;

        Simulator *simulator;
        map<string,Via*> viasIn;
        map<string,Via*> viasOut;
        map<string,map<size_t,vector<Connection*>*>*> connections;


        Node(string i, string n, Vector2D *pos, Coordinate *coord, bool isCont=false)
            :id(i),
            name(n),
            position(pos),
            coordinates(coord),
            isContinuity(isCont){}

        virtual ~Node(){
            delete position;
            // ADD CODE TO DESTROY -------------------------------------------------------
        }


        void insertConnection(Via *vIn, Via *vOut, size_t lIn, size_t lOut, double timeToCross, double celerityToEnter);
        vector<Connection*> *possibleViasOut(Via *vIn, size_t lIn, Via *previousVia);
        void vehicleStartCrossing(Via *vIn, Via * vOut, size_t laneIn, size_t laneOut, Vehicle * v);
        void vehicleExitsCrossing(Vehicle * v);
        bool canEnterConnection(Via *vIn, Via * vOut, size_t laneIn, size_t laneOut, double length);

        friend ostream &operator<<(ostream &o, Node *n);

        static void fromCsv(string line, Simulator *sim);

    protected:

    private:
};

#endif // NODE_H
