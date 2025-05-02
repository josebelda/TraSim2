#ifndef NODE_H
#define NODE_H

#include <string>
#include <map>
#include <vector>
#include "Simulator.h"
#include "Via.h"
#include "Vector2D.h"


using namespace std;

class Via;
class Simulator;

class Connection{

    public:
        Via *idIn, *idOut;
        double timeToCross, celerityToEnter;

        Connection(Via *idIn, Via *idOut, double timeToCross, double celerityToEnter):
            idIn(idIn),idOut(idOut),timeToCross(timeToCross),celerityToEnter(celerityToEnter){}

};

class Node
{
    public:
        string id;
        string name;
        Vector2D *position;
        Simulator *simulator;
        map<string,Via*> viasIn;
        map<string,Via*> viasOut;
        map<string,vector<Connection*>*> connections;

        Node(string i, string n, Vector2D *pos):id(i),name(n),position(pos){}
        virtual ~Node(){
            delete position;
            // ADD CODE TO DESTROY -------------------------------------------------------
        }


        void insertConnection(Via *vIn, Via *vOut, double timeToCross, double celerityToEnter);
        vector<Connection*> *possibleViasOut(Via *vIn);

    protected:

    private:
};

#endif // NODE_H
