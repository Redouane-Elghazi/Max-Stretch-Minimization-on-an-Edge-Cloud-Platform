#ifndef KANGINSTANCE_HPP_INCLUDED
#define KANGINSTANCE_HPP_INCLUDED
#include <random>

#include "scheduler.hpp"
#include "task.hpp"

class KangInstance{
public:
    static const int WIFI, LTE, HSPA, GPU, CPU, CLOUD;
    static const double LATENCY[3];
    static const double EXECTIME[3];
    KangInstance(int seed = 0){random_engine.seed(seed);};
    Task create_task(double r_max, int p, int commtype);
    Scheduler create_instance(double load, int n, int pe, int pc);
private:
    std::default_random_engine random_engine;
};

#endif // KANGINSTANCE_HPP_INCLUDED
