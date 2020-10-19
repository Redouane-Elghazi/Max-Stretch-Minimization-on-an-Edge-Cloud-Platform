/* Copyright (C) Redouane ELGHAZI
 * TBD
 */
#include <iostream>
#include <vector>
#include "kanginstance.hpp"

const int KangInstance::WIFI = 0,
                 KangInstance::LTE = 1,
                 KangInstance::HSPA = 2,
                 KangInstance::CLOUD = 0,
                 KangInstance::GPU = 1,
                 KangInstance::CPU = 2;
const double KangInstance::LATENCY[3] = {95, 180, 870};
const double KangInstance::EXECTIME[3] = {6, 81, 382};

Task KangInstance::create_task(double r_max, int p, int commtype){
    double w, up, dn, r, origin = p;
    std::normal_distribution<double> d_w{EXECTIME[CLOUD]-1, EXECTIME[CLOUD]/3},
                                     d_up{LATENCY[commtype]-1, LATENCY[commtype]/3};
    std::uniform_real_distribution<double> d_r{0, r_max};

    w = abs(d_w(random_engine))+1;
    up = abs(d_up(random_engine))+1;
    dn = 0;
    r = d_r(random_engine);
    return Task(w, up, dn, r, origin);
}

Scheduler KangInstance::create_instance(double load, int n, int pe, int pc){
    std::vector<double> S;
    S.insert(S.end(), pe/2, EXECTIME[CLOUD]/EXECTIME[GPU]);
    S.insert(S.end(), (pe+1)/2, EXECTIME[CLOUD]/EXECTIME[CPU]);
    double edge_power = accumulate(S.begin(), S.end(), 0.);
    std::vector<Task> tasks;
    std::uniform_int_distribution<int> d_origin(0,pe-1);
    double r_max = EXECTIME[CLOUD]*n/load/edge_power;
    for(int i = 0; i<n; ++i){
        int p = d_origin(random_engine);
        tasks.push_back(create_task(r_max, p, p%3));
    }
    return Scheduler(pc, S, tasks);
}
