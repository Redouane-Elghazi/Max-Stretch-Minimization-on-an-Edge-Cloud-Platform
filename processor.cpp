/* Copyright (C) Redouane ELGHAZI
 * TBD
 */
#include <iostream>
#include "processor.hpp"
const bool Processor::EDGE = 0;
const bool Processor::CLOUD = 1;

void Processor::add_task(int i, Task T){
    tasks[i] = T;
}

void Processor::remove_task(int i){
    tasks.erase(i);
}

double Processor::remaining_time(int i) const{
    bool migrate;
    if(type == EDGE)
        migrate = false;
    else
        migrate = true;
    return tasks.at(i).remaining_time(s, migrate);
}

double Processor::upcom_time(int i) const{
    bool migrate;
    if(type == EDGE)
        migrate = false;
    else
        migrate = true;
    return tasks.at(i).upcom_time(s, migrate);
}

double Processor::exec_time(int i) const{
    bool migrate;
    if(type == EDGE)
        migrate = false;
    else
        migrate = true;
    return tasks.at(i).exec_time(s, migrate);
}

double Processor::dncom_time(int i) const{
    bool migrate;
    if(type == EDGE)
        migrate = false;
    else
        migrate = true;
    return tasks.at(i).dncom_time(s, migrate);
}

int Processor::origin(int i) const{
    return tasks.at(i).origin;
}

bool Processor::is_on_cloud(int i) const{
    return tasks.at(i).is_on_cloud();
}

bool Processor::is_executed(int i) const{
    return tasks.at(i).is_executed();
}

bool Processor::is_finished(int i) const{
    bool migrate;
    if(type == EDGE)
        migrate = false;
    else
        migrate = true;
    return tasks.at(i).is_finished(s, migrate);
}

bool Processor::upcommunicate(int i, double t){
    return tasks.at(i).upcommunicate(t);
}

bool Processor::execute(int i, double t){
    bool migrate;
    if(type == EDGE)
        migrate = false;
    else
        migrate = true;
    return tasks.at(i).execute(t, s, migrate);
}

bool Processor::downcommunicate(int i, double t){
    return tasks.at(i).downcommunicate(t);
}
