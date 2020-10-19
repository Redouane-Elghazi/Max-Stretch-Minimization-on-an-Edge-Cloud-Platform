/* Copyright (C) Redouane ELGHAZI
 * TBD
 */
#include <iostream>
#include <vector>
#include <chrono>

#include "kanginstance.hpp"
#include "task.hpp"
#include "processor.hpp"
#include "scheduler.hpp"
#include "srpt.hpp"
#include "greedy.hpp"
#include "ssfedf2.hpp"
#include "bender.hpp"
#include "taskcreator.hpp"

#define KANG 0

using namespace std;

int main(int argc, char *argv[]){ // n, sample, seed
    ///put in instance.cpp
    int pc = 10;
    int pe = 20;//100;
    //vector<double> S(10, 0.01);
    //S.insert(S.end(), 10, 0.1);
    int n = atoi(argv[1]);
    int sample_size = atoi(argv[2]);
    int seed = atoi(argv[3]);
    double ccr = atof(argv[4]);
    double load = 0.05;
    TaskCreator creator(seed);
    KangInstance instanciator(seed);
    double srpt_avg = 0, greedy_avg = 0, ssfedf2_avg = 0, bender_avg = 0;
    double srpt_time = 0, greedy_time = 0, ssfedf2_time = 0, bender_time = 0;
    for(int i = 0; i<sample_size; ++i){
//cerr << "begin___" << i << endl;
#if KANG
        Scheduler scheduler = instanciator.create_instance(load, n, pe, pc);
#else
        vector<double> S(10, 0.1);
        S.insert(S.end(), 10, 0.5);
        pe = S.size();
        double edge_power = accumulate(S.begin(), S.end(), 0.);
        vector<Task> tasks;
        for(int j = 0; j<n; ++j){
            double w_min = 10, w_max = 50;
            double up_min = w_min*ccr/2, up_max = w_max*ccr/2,
                   dn_min = w_min*ccr/2, dn_max = w_max*ccr/2;
            double r_min = 0, r_max = (w_max+w_min)/2*n/load/edge_power;
            tasks.push_back(creator.create_task(w_min, w_max, up_min, up_max,
                                                dn_min, dn_max, r_min, r_max,
                                                pe));
        }
        Scheduler scheduler(pc, S, tasks);
#endif // KANG
        SRPT srpt(scheduler);
        Greedy greedy(scheduler);
        SSFEDF2 ssfedf2(scheduler);
        Bender bender(scheduler);
//cerr << "srpt____"<< endl;
        chrono::steady_clock::time_point start = chrono::steady_clock::now();
        srpt_avg += srpt.solve();
        chrono::steady_clock::time_point finish = chrono::steady_clock::now();
        srpt_time += chrono::duration_cast<chrono::microseconds>(finish - start).count();
//cerr << "greedy__" << endl;
        start = chrono::steady_clock::now();
        greedy_avg += greedy.solve();
        finish = chrono::steady_clock::now();
        greedy_time += chrono::duration_cast<chrono::microseconds>(finish - start).count();
//cerr << "ssfedf__" << endl;
        start = chrono::steady_clock::now();
        ssfedf2_avg += ssfedf2.solve();
        finish = chrono::steady_clock::now();
        ssfedf2_time += chrono::duration_cast<chrono::microseconds>(finish - start).count();
//cerr << "bender__" << endl;
        start = chrono::steady_clock::now();
        bender_avg += bender.solve();
        finish = chrono::steady_clock::now();
        bender_time += chrono::duration_cast<chrono::microseconds>(finish - start).count();
    }
    cout << srpt_avg/sample_size << endl;
    cout << greedy_avg/sample_size << endl;
    cout << ssfedf2_avg/sample_size << endl;
    cout << bender_avg/sample_size << endl;
    cout << srpt_time/sample_size/1000000 << endl;
    cout << greedy_time/sample_size/1000000 << endl;
    cout << ssfedf2_time/sample_size/1000000 << endl;
    cout << bender_time/sample_size/1000000 << endl;
	return 0;
}
