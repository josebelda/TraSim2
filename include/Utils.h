#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <ctime>
#include <ostream>

using namespace std;


class Utils
{
    public:
        Utils();
        virtual ~Utils();

        static string strip(string s);
        static vector<string> split(string s,char sep);
        static time_t maketime(int year, int month, int day, int hour, int minute, int second);
        static string timetostring(const time_t t);
        static double roundPrec(double v, double prec);

    protected:

    private:
};

ostream &operator<<(ostream &o, vector<string> v);

#endif // UTILS_H
