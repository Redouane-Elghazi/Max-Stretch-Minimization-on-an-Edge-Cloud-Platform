#ifndef SRPT_HPP_INCLUDED
#define SRPT_HPP_INCLUDED

#include "scheduler.hpp"

class SRPT{
public:
    SRPT(Scheduler scheduler):scheduler(scheduler){}
    double solve();
private:
    Scheduler scheduler;
};

#endif // SRPT_HPP_INCLUDED
