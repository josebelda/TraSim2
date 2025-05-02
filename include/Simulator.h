#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <ctime>
#include <vector>

#include "Coordinate.h"
#include "Vehicle.h"

class Vehicle;

class Simulator
{
    public:
        static const Coordinate *BASE_COORDINATES;
        static const double SCALE;

        time_t t, t0;

        vector<Vehicle*> *vehicles;

        Simulator();
        virtual ~Simulator();

    protected:

    private:
};

#endif // SIMULATOR_H
