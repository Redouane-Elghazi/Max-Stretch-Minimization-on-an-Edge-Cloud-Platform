#ifndef TASKCREATOR_HPP_INCLUDED
#define TASKCREATOR_HPP_INCLUDED
#include <random>

#include "task.hpp"

class TaskCreator{
public:
    TaskCreator(int seed = 0){random_engine.seed(seed);};
    Task create_task(double w_min, double w_max, double up_min, double up_max,
                     double dn_min, double dn_max, double r_min, double r_max,
                     int nb_p);
private:
    std::default_random_engine random_engine;
};

#endif // TASKCREATOR_HPP_INCLUDED
