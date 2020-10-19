#ifndef PROCESSOR_HPP_INCLUDED
#define PROCESSOR_HPP_INCLUDED
#include <map>
#include "task.hpp"

class Processor{
public:
    static const bool EDGE;
    static const bool CLOUD;
    Processor(double s, bool type): s(s), type(type){}
    void add_task(int i, Task T);
    void remove_task(int i);
    double remaining_time(int i) const;
    double upcom_time(int i) const;
    double exec_time(int i) const;
    double dncom_time(int i) const;
    int origin(int i) const;
    bool is_on_cloud(int i) const;
    bool is_executed(int i) const;
    bool is_finished(int i) const;
    bool upcommunicate(int i, double t);
    bool execute(int i, double t);
    bool downcommunicate(int i, double t);
    std::map<int, Task> tasks;
private:
    double s;
    bool type;
};


#endif // PROCESSOR_HPP_INCLUDED
