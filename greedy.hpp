#ifndef GREEDY_HPP_INCLUDED
#define GREEDY_HPP_INCLUDED

#include "scheduler.hpp"

class Greedy{
public:
    Greedy(Scheduler scheduler):scheduler(scheduler){}
    double solve();
private:
    Scheduler scheduler;
};

#endif // GREEDY_HPP_INCLUDED
