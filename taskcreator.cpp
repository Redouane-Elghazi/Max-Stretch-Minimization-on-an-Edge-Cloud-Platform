/* Copyright (C) Redouane ELGHAZI
 * TBD
 */
#include <iostream>
#include "taskcreator.hpp"

Task TaskCreator::create_task(
        double w_min, double w_max, double up_min, double up_max,
        double dn_min, double dn_max, double r_min, double r_max,
        int nb_p){
    double w, up, dn, r, origin;
    std::uniform_real_distribution<double> d_w{w_min, w_max},
                                           d_up{up_min, up_max},
                                           d_dn{dn_min, dn_max},
                                           d_r{r_min, r_max};
    std::uniform_int_distribution<int> d_origin(0,nb_p-1);
    w = d_w(random_engine);
    up = d_up(random_engine);
    dn = d_dn(random_engine);
    r = d_r(random_engine);
    origin = d_origin(random_engine);
    return Task(w, up, dn, r, origin);
}
