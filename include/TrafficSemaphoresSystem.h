#ifndef TRAFFICSEMAPHORE_H
#define TRAFFICSEMAPHORE_H

#include <ctime>
#include <string>
#include <map>
#include <vector>
#include <fstream>

#include "Simulator.h"
#include "Via.h"

using namespace std;

class Simulator;
class Via;

enum Light{
    RED,
    YELLOW,
    GREEN,
    INTERMITENT_YELLOW,
    NO_SEMAPHORE
};


class TrafficSemaphoresSystem
{
    public:
        static int UNDEFINED_LIGHT_EXCEPTION;

        Simulator *simulator;
        int cycleLength;


        map<
           string,  // via in id
           map<
                size_t, // lane number in
                map<
                    string, // via out id
                    map<
                        size_t,// lane number out
                        map<
                            int,
                            enum Light
                        >  // Lights for every second in the phases
                    >
                >
            >
        > cycle;


        TrafficSemaphoresSystem(Simulator *sim):simulator(sim){};
        virtual ~TrafficSemaphoresSystem(){}

        void addConnectionInterval(
            Via *viaFrom,
            Via *viaTo,
            size_t laneFrom,
            size_t laneTo,
            int timeFrom,
            int timeTo,
            enum Light);

        enum Light getLight(Via *viaFrom, Via *viaTo, size_t laneFrom, size_t laneTo);

        static TrafficSemaphoresSystem *fromCsv(ifstream *f, Simulator *sim);

    protected:

    private:
};

#endif // TRAFFICSEMAPHORE_H
