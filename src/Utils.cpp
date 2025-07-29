#include <iostream>
#include <cmath>

#include "Utils.h"

using namespace std;

Utils::Utils()
{
    //ctor
}

Utils::~Utils()
{
    //dtor
}


string Utils::strip(string s){
    string r;
    for(size_t i = 0 ; i<s.length(); i++){
        if(s[i]!='\r' && s[i]!='\n') r += s[i];
    }
    return r;
}

vector<string> Utils::split(string s,char sep){

    vector<string> r;
    size_t a = 0;
    size_t p = s.find(sep);
    while( p!=string::npos ){
        r.push_back(s.substr(a, p-a));
        a = p+1;
        p = s.find(sep, a);
    }
    r.push_back(s.substr(a, s.length()-a));
    return r;
}

time_t Utils::maketime(int year, int month, int day, int hour, int minute, int second){
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    timeinfo->tm_year = year;
    timeinfo->tm_mon = month;
    timeinfo->tm_mday = day;
    timeinfo->tm_hour = hour;
    timeinfo->tm_min = minute;
    timeinfo->tm_sec = second;

    rawtime = mktime(timeinfo);
    return rawtime;

}


string Utils::timetostring(const time_t t){

    struct tm * timeinfo = localtime(&t);
    char ts[80];
    strftime (ts,80,"%F %T",timeinfo);
    string s(ts);
    return s;
}


ostream &operator<<(ostream &o, vector<string> v){
    for(string s : v){
        o << s << "$$";
    }
    o << endl;
    return o;
}

double Utils::roundPrec(double v, double prec){
    return round(v/prec)*prec;
}
