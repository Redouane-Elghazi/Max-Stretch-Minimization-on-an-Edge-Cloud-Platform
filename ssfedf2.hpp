#ifndef SSFEDF2_HPP_INCLUDED
#define SSFEDF2_HPP_INCLUDED

#include "scheduler.hpp"

class SSFEDF2{
public:
    SSFEDF2(Scheduler scheduler):scheduler(scheduler){}
    double solve();
private:
    Scheduler scheduler;
};

#endif // SSFEDF2_HPP_INCLUDED
