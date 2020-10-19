/* Copyright (C) Redouane ELGHAZI
 * TBD
 */
#include <iostream>
#include "task.hpp"
//TODO: CLEAN
#define eps 1e-5

double max(double a, double b){
    return (a>b)?a:b;
}

double Task::remaining_time(double s, bool migrate) const{
    double t = 0;
    if(migrate and up+dn>eps){
        t += up+dn;
    }
    if(t+w/s>eps){
        t += w/s;
    }
    return t;
}

double Task::upcom_time(double s, bool migrate) const{
    double t = 0;
    if(migrate and up>eps){
        t += up;
    }
    return t;
}

double Task::exec_time(double s, bool migrate) const{
    double t = 0;
    if(w/s > eps){
        t += w/s;
    }
    return t;
}

double Task::dncom_time(double s, bool migrate) const{
    double t = 0;
    if(migrate and dn>eps){
        t += dn;
    }
    return t;
}

bool Task::is_on_cloud() const{
    return up<eps;
}

bool Task::is_executed() const{
    return w<eps;
}

bool Task::is_finished(double s, bool migrate) const{
    if(migrate)
        return up<eps and w<eps and dn<eps;
    else
        return w<eps;
}

bool Task::execute(double t, double s, bool migrate){
    if(!migrate or is_on_cloud()){
        invested_time += t;
        w = max(0, w-s*t);
        if(is_executed())
            return true;
        else
            return false;
    }
    else{
std::cerr << "w0" << std::endl;
        return false;
    }
}

bool Task::upcommunicate(double t){
    invested_time += t;
    up = max(0, up-t);
    if(is_on_cloud())
        return true;
    else{
        return false;
    }
}

bool Task::downcommunicate(double t){
    if(is_on_cloud() and is_executed()){
        invested_time += t;
        dn = max(0, dn-t);
        if(is_finished(1, true))
            return true;
        else
            return false;
    }
    else{
std::cerr << "w2" << std::endl;
        return false;
    }
}
