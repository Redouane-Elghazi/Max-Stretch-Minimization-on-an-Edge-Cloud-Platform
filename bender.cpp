/* Copyright (C) Redouane ELGHAZI
 * TBD
 */
#include <limits>
#include <algorithm>
#include <map>
#include <queue>
#include <iostream>
#include <tuple>

#include "bender.hpp"
//TODO: CLEAN
#define eps 1e-10

using std::pair;
using std::max;
using std::min;

namespace {
double assignResource(int i, double t, double w,
                      std::set<pair<double, double> > &not_busy,
                      std::set<std::pair<std::pair<double, double>, int> > &busy,
                      bool simulate){
    if(simulate){
        if(w != 0){
            double done = 0;
            auto it = not_busy.begin();
            auto p = *it;
            while(max(0., p.second-max(p.first, t))<(w-done)){
                done += max(0., p.second-max(p.first, t));
                ++it;
                p = *it;
            }
            return max(p.first, t) + (w-done);
        }
        else{
            return t;
        }
    }
    else{
        if(w != 0){
            std::set<pair<double, double> > toRem, toAdd;
            double done = 0;
            auto it = not_busy.begin();
            auto p = *it;
            while(max(0., p.second-max(p.first, t))<(w-done)){
                if(p.second-max(p.first, t) > 0){
                    toRem.emplace(p);
                    if(p.first != max(p.first, t)){
                        toAdd.emplace(p.first, t);
                    }
                    busy.emplace(std::make_pair(max(p.first, t), p.second), i);
                    done += p.second-max(p.first, t);
                }
                ++it;
                p = *it;
            }
            double res = max(p.first, t) + (w-done);
            if(w!=done){
                toRem.emplace(p);
                if(p.first < max(p.first, t)){
                    toAdd.emplace(p.first, max(p.first, t));
                }
                if(p.second > res){
                    toAdd.emplace(res, p.second);
                }
                busy.emplace(std::make_pair(max(p.first, t), res), i);
            }
            for(auto p:toRem)
                not_busy.erase(p);
            for(auto p:toAdd)
                not_busy.insert(p);
            return res;
        }
        else{
            return t;
        }
    }
}

double assignEdge(int i, double t, double w,
                  std::set<pair<double, double> > &not_executing,
                  std::set<std::pair<std::pair<double, double>, int> > &executing,
                  bool simulate){
    double end_exe = 0;
    end_exe = assignResource(i, t, w, not_executing, executing, simulate);

    return end_exe;
}

bool schedule(
        std::vector<std::map<int, double> > &wE,
        std::map<int, double> &min_processing_times,
        std::map<int, double> &r, std::map<int, int> &o, double t, double S,
        std::vector<std::set<std::pair<std::pair<double, double>, int> > > &executingE){
    int pe = executingE.size();
    for(int j = 0; j<pe; ++j){
        executingE[j].clear();
    }
    std::vector<std::set<pair<double, double> > > not_executingE(pe);
    for(int j = 0; j<pe; ++j){
        not_executingE[j].emplace(t, std::numeric_limits<double>::infinity());
    }
    std::set<std::pair<double, int> > deadlines;
    for(auto x:r){
        int i = x.first;
        deadlines.emplace(S*min_processing_times[i]+r[i], i);
    }
    for(auto p:deadlines){
        int i = p.second;
        double te = assignEdge(i, t, wE[o[i]][i], not_executingE[o[i]], executingE[o[i]], true);
        assignEdge(i, t, wE[o[i]][i], not_executingE[o[i]], executingE[o[i]], false);
        double finishing_time = te;
        if(finishing_time > p.first){
            return false;
        }
    }
    return true;
}
///TODO: add the current S (useless to try to do better than that)
void schedule(
        std::vector<std::map<int, double> > &wE,
        std::map<int, double> &min_processing_times,
        std::map<int, double> &r, std::map<int, int> &o, double t,
        std::vector<std::set<std::pair<std::pair<double, double>, int> > > &executingE,
        double curS){
    double a = 1, b = 1;
    while(!schedule(wE, min_processing_times, r, o, t, b, executingE)){
        b*=2;
    }
    while(b-a>eps){
        double S = (a+b)/2;
        if(!schedule(wE, min_processing_times, r, o, t, S, executingE)){
            a = S;
        }
        else{
            b = S;
        }
    }
    double S = b;
    schedule(wE, min_processing_times, r, o, t, S, executingE);
}
}

double Bender::solve(){
    std::set<std::tuple<double, bool, int, bool, int> > affectation;
    std::vector<Processor> &Pe = scheduler.Pe;
    int pe = Pe.size();
    double time = 0;
    while(!scheduler.empty()){
        double next_release = scheduler.next_release();
        std::vector<int> &execute_cloud = scheduler.execute_cloud,
                         &execute_edge = scheduler.execute_edge,
                         &upcommunicate = scheduler.upcommunicate,
                         &downcommunicate = scheduler.downcommunicate;
        for(int &i:execute_cloud)
            i = -1;
        for(int &i:execute_edge){
            i = -1;
        }
        for(int &i:upcommunicate)
            i = -1;
        for(int &i:downcommunicate)
            i = -1;
        if(!scheduler.active_tasks.empty()){
            std::vector<std::map<int, double> > wE(pe);
            std::map<int, double> min_processing_times, r;
            std::map<int, int> o;
            for(int j = 0; j<pe; ++j){
                auto &P = scheduler.Pe[j];
                for(auto x:P.tasks){
                    int i = x.first;
                    Task &T = x.second;
                    wE[j][i] = P.exec_time(i);
                    min_processing_times[i] = scheduler.min_processing_times[i];
                    r[i] = T.r;
                    o[i] = T.origin;
                }
            }
            std::vector<std::set<std::pair<std::pair<double, double>, int> > > executingE(pe);
            double curS = scheduler.get_max_stretch();
            schedule(wE, min_processing_times, r, o, time, executingE, curS);
            std::set<std::tuple<double, int, bool, int, int> > events; //time, task, proctype, proc, eventtype
            for(int j = 0; j<pe; ++j){
                for(auto x:executingE[j]){
                    events.emplace(x.first.first, x.second, Processor::EDGE, j, 1);
                    events.emplace(x.first.second, x.second, Processor::EDGE, j, 4);
                }
            }
            for(auto e:events){
                double new_time = std::get<0>(e);
                if(new_time>next_release){
                    break;
                }
                else if(new_time != time){
                    new_time = scheduler.advance(new_time);
                    time = new_time;
                }
                int i = std::get<1>(e);
                bool proctype = std::get<2>(e);
                int j = std::get<3>(e);
                int eventtype = std::get<4>(e);
                if(eventtype == 0){
                    upcommunicate[j] = i;
                }
                else if(eventtype == 1){
                    if(proctype == Processor::EDGE){
                        execute_edge[j] = i;
                    }
                    else{
                        execute_cloud[j] = i;
                    }
                }
                else if(eventtype == 2){
                    downcommunicate[j] = i;
                }
                else if(eventtype == 3 and upcommunicate[j] == i){
                    upcommunicate[j] = -1;
                }
                else if(eventtype == 4){
                    if(proctype == Processor::EDGE and execute_edge[j] == i){
                        execute_edge[j] = -1;
                    }
                    if(proctype == Processor::CLOUD and execute_cloud[j] == i){
                        execute_cloud[j] = -1;
                    }
                }
                else if(eventtype == 5 and downcommunicate[j] == i){
                    downcommunicate[j] = -1;
                }
            }
        }
        next_release = scheduler.advance(next_release);
        time = next_release;
    }
    return scheduler.get_max_stretch();
}
