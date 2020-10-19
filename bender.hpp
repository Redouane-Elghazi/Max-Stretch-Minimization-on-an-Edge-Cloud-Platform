#ifndef BENDER_HPP_INCLUDED
#define BENDER_HPP_INCLUDED

#include "scheduler.hpp"

class Bender{
public:
    Bender(Scheduler scheduler):scheduler(scheduler){}
    double solve();
private:
    Scheduler scheduler;
};

#endif // BENDER_HPP_INCLUDED
