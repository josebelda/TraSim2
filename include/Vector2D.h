#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <ostream>

using namespace std;

class Vector2D
{
    public:
        double x, y;
        Vector2D(double x, double y):x(x),y(y){}
        virtual ~Vector2D(){}


        Vector2D *add(Vector2D *v);
        Vector2D *sub(Vector2D *v);
        double mul(Vector2D *v);
        Vector2D *mul(double k);
        double sqModule();
        double module();
        double cosAngle(Vector2D *v);
        double angle(Vector2D *v);
        double distance(Vector2D *v);
        bool hasSameSense(Vector2D *v);
        Vector2D *unitary();

        friend ostream &operator<<(ostream &o, Vector2D *v);

    protected:

    private:
};

#endif // VECTOR2D_H
