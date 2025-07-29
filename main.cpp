#include <iostream>
#include <fstream>
#include <unistd.h>

#include "Simulator.h"
#include "Utils.h"

using namespace std;


// TO DO
/*
    Comprobar que posicion  a la que se incorpora es al azar


*/

void performTests1();
void performTests2();
void performTests3();
void performTests4();
void simulate();

int main()
{
    ofstream f;
    f.open("testsoutput.txt",ofstream::trunc);
    f.close();

    //performTests1();
    //performTests2();
    //performTests3();
    performTests4();

    //simulate();

    return 0;
}

void simulate(){
    time_t t0 = Utils::maketime(2025, 4, 20, 1, 30, 0);
    time_t t1 = Utils::maketime(2025, 4, 20, 2, 30, 0);

        cout << t0 << " " << t1 << endl;


    Simulator *sim = new Simulator(t0, t1);
    sim->load();
    sim->run();
}


void performTests1(){
    ofstream f;
    f.open("testsoutput.txt",ofstream::app);

    f << "-------------------------------------------- TEST 1 --------------------------------------------------" << endl;

    time_t t0 = Utils::maketime(1015, 4, 20, 1, 30, 15);
    time_t t1 = Utils::maketime(1015, 4, 20, 2, 30, 15);



    Simulator *sim = new Simulator(t0, t1);

    Coordinate *c = Simulator::BASE_COORDINATES->clone();

    f << c << endl;
    c->latitude += 1;
    c->longitude +=1;

    Vector2D *v = c->translate(Simulator::BASE_COORDINATES);

    f << v << endl;
    f << sim << endl;

    sim->load();
    for(Via *v : *(sim->getVias())){

        Vehicle *vehicle = v->incorporateVehicle();
        if(!vehicle->positionIsInVia()){
            f << "Vehicle out of its via:" << endl;
            f << vehicle << endl;
        }


    }
    f << "################## moving vehicle ######################" << endl;




    for(Vehicle * vehicle : *(sim->getAllVehicles())){
        f << "Vehicle in via:  " << vehicle->via->name << endl;
        f << "Via lengh: " << vehicle->via->length << endl;
        f << "Will get to intersection: " << vehicle->via->node2->id << endl;
        f << "Possible next vias: " << endl;
        for(size_t lane=0; lane<vehicle->via->numberOfLanes; lane++){
            vector<Connection*> *connections = vehicle->via->node2->possibleViasOut(vehicle->via, lane, vehicle->previousVia);
            for(Connection *con : *connections ){
                f << "Via in: " << con->viaIn->id << "Lane in: " << con->laneIn << "Via out: " << con->viaOut->id << "Lane out: " << con->laneOut << endl;
            }
        }
        f << endl;
        while(vehicle->via!=nullptr){

            double p = vehicle->via->node1->position->
                distance(vehicle->position);
            f << vehicle->position
                << " " << p << " "
                << vehicle->a
                << " " << vehicle->celerity << endl;

                vehicle->move(0.1);

                //sleep(1);
        }
        if(vehicle->via!=nullptr){
            f << "End of via position: " << vehicle->via->node2->position << endl;
        }
        else{
            f << "Vehicle entering intersection " << vehicle->node->id << endl;
            f << "Acceleration and celerity: " << vehicle->a << " " << vehicle->celerity << endl;
            f << "Intersection position: " << vehicle->node->position << endl;
            f << "Next Via: " << vehicle->nextVia->id << endl;
        }

    }


    f << "$$$$$$$$$$$$$$$$$$$$$ Vehicles crossing intersection $$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
    for(Vehicle * vehicle : *(sim->getAllVehicles())){
        if(vehicle->node!=nullptr){
            f << "Vehicle crossing intersection:  " << vehicle->node->id << endl;
            f << "Next via: " << vehicle->nextVia->id << endl;
            f << "Next Via position: " << vehicle->nextVia->node1->position << endl;
            while(vehicle->via==nullptr){
                f << "Tick" << endl;
                vehicle->move(0.1);
            }
            f << "Vehicle entering via: " << vehicle->via->id << endl;
            f << "Acceleration and celerity: " << vehicle->a << " " << vehicle->celerity << endl;
            f << "Vehicle position: " << vehicle->position << endl;
        }
    }


    f << "------------------------- FINAL SIMULATOR ------------------------------" << endl;
    f << sim << endl;
    f.close();
}


void performTests2(){
    ofstream f;
    f.open("testsoutput.txt",ofstream::app);

    f << "-------------------------------------------- TEST 2 --------------------------------------------------" << endl;

    time_t t0 = Utils::maketime(1015, 4, 20, 1, 30, 15);
    time_t t1 = Utils::maketime(1015, 4, 20, 2, 30, 15);



    Simulator *sim = new Simulator(t0, t1);

    Coordinate *c = Simulator::BASE_COORDINATES->clone();

    f << c << endl;
    c->latitude += 1;
    c->longitude +=1;

    Vector2D *v = c->translate(Simulator::BASE_COORDINATES);

    f << v << endl;
    f << sim << endl;

    sim->load();
    for(Via *v : *(sim->getVias())){
        for(int i = 0 ; i < 2; i++){

            Vehicle *vehicle = v->incorporateVehicle();
            if(!vehicle->positionIsInVia()){
                f << "Vehicle out of its via:" << endl;
                f << vehicle << endl;
            }
        }

    }
    f << "Moving vehicles" << endl;
    f << "################## moving vehicle ######################" << endl;

    for(Via * via : *(sim->getVias())){
        f << "Vehicles in via:  " << via->name << endl;
        f << "Via length: " << via->length << endl;
        f << "Will get to intersection: " << via->node2->id << endl;
        f << "Possible next vias: " << endl;
        for(size_t lane=0; lane<via->numberOfLanes; lane++){
            vector<Connection*> *connections = via->node2->possibleViasOut(via, lane, nullptr);
            for(Connection *con : *connections ){
                f << "Via in: " << con->viaIn->id << " Lane in: " << con->laneIn << " Via a out: " << con->viaOut->id << " Lane out: " << con->laneOut << endl;
            }
        }
        f << endl;
        bool vehicleOut = false;
        while( not vehicleOut){
            int i=0;
            for(Vehicle *v : *(sim->getAllVehicles())){

                if(v->via==via){

                    f << "Vehicle: " << i << " " << endl;
                    v->move(0.1);
                    if(v->via==nullptr){
                        vehicleOut = true;
                        f << " out of via" << endl;
                    }
                    else{
                        double p = v->via->node1->position->
                            distance(v->position);
                        f << v->position << " "
                            << p << " "
                            << v->a << " "
                            << v->celerity << endl;


                    }
                    i++;
                }
                //sleep(1);
            }

        }
        f << "Exit while" << endl;

    }


    f << "$$$$$$$$$$$$$$$$$$$$$ Vehicles crossing intersection $$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
    for(Vehicle * vehicle : *(sim->getAllVehicles())){
        if(vehicle->node!=nullptr){
            f << "Vehicle crossing intersection:  " << vehicle->node->id << endl;
            f << "Next via: " << vehicle->nextVia->id << endl;
            f << "Next Via position: " << vehicle->nextVia->node1->position << endl;
            while(vehicle->via==nullptr){
                f << "Tick" << endl;
                vehicle->move(0.1);
            }
            f << "Vehicle entering via: " << vehicle->via->id << endl;
            f << "Acceleration and celerity: " << vehicle->a << " " << vehicle->celerity << endl;
            f << "Vehicle position: " << vehicle->position << endl;
        }
    }


    f << "------------------------- FINAL SIMULATOR ------------------------------" << endl;
    f << sim << endl;
    f.close();
}

void performTests3(){
    ofstream f;
    f.open("testsoutput.txt",ofstream::app);

    f << "-------------------------------------------- TEST 3 --------------------------------------------------" << endl;

    time_t t0 = Utils::maketime(1015, 4, 20, 1, 30, 15);
    time_t t1 = Utils::maketime(1015, 4, 20, 1, 30, 25);





    Simulator *sim = new Simulator(t0, t1);

    Coordinate *c = Simulator::BASE_COORDINATES->clone();

    f << c << endl;
    c->latitude += 1;
    c->longitude +=1;

    Vector2D *v = c->translate(Simulator::BASE_COORDINATES);

    f << v << endl;
    f << sim << endl;

    sim->load();
    (*(sim->getVias()))[0]->incorporateVehicle();

    f << "################## moving vehicle ######################" << endl;




    Vehicle *vehicle = (*(sim->getAllVehicles()))[0];
    f << "Vehicle in via:  " << vehicle->via->name << endl;
    f << "Via lengh: " << vehicle->via->length << endl;
    f << "Will get to intersection: " << vehicle->via->node2->id << endl;
    f << "Possible next vias: " << endl;
    vector<Connection*> *connections = vehicle->via->node2->possibleViasOut(vehicle->via,vehicle->lane, vehicle->previousVia);
    for(Connection *con : *connections ){
        f << con->viaIn->id << " " << con->viaOut->id << " , ";
    }
    f << endl;
    for(int i = 0; i<10000; i++){

        if(vehicle->via!=nullptr){
            f << "Via: " << vehicle->via->node1->id << " , " << vehicle->via->node2->id << "cel: " << vehicle->celerity << endl;
        }
        else{
            f << "Node: " << vehicle->node->id << endl;
        }

        vehicle->move(0.1);



    }







    f << "------------------------- FINAL SIMULATOR ------------------------------" << endl;
    f << sim << endl;
    f.close();
}


void performTests4(){
    ofstream f;
    f.open("testsoutput.txt");


    time_t t0 = Utils::maketime(1015, 4, 20, 1, 30, 15);
    time_t t1 = Utils::maketime(1015, 4, 20, 1, 30, 25);





    Simulator *sim = new Simulator(t0, t1);
cout << "A" << endl;
    sim->load();
cout << "B" << endl;
    f << sim << endl;
cout << "C" << endl;

    f.close();

}
