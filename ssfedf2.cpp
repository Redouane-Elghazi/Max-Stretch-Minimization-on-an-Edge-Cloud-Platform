/* Copyright (C) Redouane ELGHAZI
 * TBD
 */
#include <limits>
#include <algorithm>
#include <map>
#include <queue>
#include <iostream>
#include <tuple>

#include "ssfedf2.hpp"
//TODO: CLEAN
#define eps 1e-10

using std::pair;
using std::max;
using std::min;

namespace {
double assignResource(int i, double t, double w,
                      std::set<pair<double, double> > &not_busyC,
                      std::set<std::pair<std::pair<double, double>, int> > &busyC,
                      std::set<pair<double, double> > &not_busyE,
                      std::set<std::pair<std::pair<double, double>, int> > &busyE,
                      bool simulate){
    if(simulate){
        if(w != 0){
            double done = 0;
            auto itC = not_busyC.begin(),
                 itE = not_busyE.begin();
            auto pC = *itC;
            auto pE = *itE;
            while(max(0., min(pC.second, pE.second)-max(max(pC.first, pE.first), t))<(w-done)){
                done += max(0., min(pC.second, pE.second)-max(max(pC.first, pE.first), t));
                if(pC.second>pE.second){
                    ++itE;
                    pE = *itE;
                }
                else{
                    ++itC;
                    pC = *itC;
                }
            }
            return max(max(pC.first, pE.first), t) + (w-done);
        }
        else{
            return t;
        }
    }
    else{
        if(w != 0){
            double done = 0;
            auto itC = not_busyC.begin(),
                 itE = not_busyE.begin();
            auto pC = *itC;
            auto pE = *itE;
            while(max(0., min(pC.second, pE.second)-max(max(pC.first, pE.first), t))<(w-done)){
                if(min(pC.second, pE.second)-max(max(pC.first, pE.first), t)>0){
                    itC = not_busyC.erase(itC);
                    itE = not_busyE.erase(itE);
                    if(pC.first < max(max(pC.first, pE.first), t)){
                        itC = not_busyC.emplace(pC.first, max(max(pC.first, pE.first), t)).first;
                    }
                    if(pE.first < max(max(pC.first, pE.first), t)){
                        itE = not_busyE.emplace(pE.first, max(max(pC.first, pE.first), t)).first;
                    }
                    if(pC.second > min(pC.second, pE.second)){
                        itC = not_busyC.emplace(min(pC.second, pE.second), pC.second).first;
                    }
                    if(pE.second > min(pC.second, pE.second)){
                        itE = not_busyE.emplace(min(pC.second, pE.second), pE.second).first;
                    }
                    busyC.emplace(std::make_pair(max(max(pC.first, pE.first), t), min(pC.second, pE.second)), i);
                    busyE.emplace(std::make_pair(max(max(pC.first, pE.first), t), min(pC.second, pE.second)), i);
                    done += min(pC.second, pE.second)-max(max(pC.first, pE.first), t);
                    pC = *itC;
                    pE = *itE;
                }
                else if(pC.second>pE.second){
                    ++itE;
                    pE = *itE;
                }
                else{
                    ++itC;
                    pC = *itC;
                }
            }
            double res = max(max(pC.first, pE.first), t) + (w-done);
            if(w!=done){
                itC = not_busyC.erase(itC);
                itE = not_busyE.erase(itE);
                if(pC.first < max(max(pC.first, pE.first), t)){
                    itC = not_busyC.emplace(pC.first, max(max(pC.first, pE.first), t)).first;
                }
                if(pE.first < max(max(pC.first, pE.first), t)){
                    itE = not_busyE.emplace(pE.first, max(max(pC.first, pE.first), t)).first;
                }
                if(pC.second > res){
                    itC = not_busyC.emplace(res, pC.second).first;
                }
                if(pE.second > res){
                    itE = not_busyE.emplace(res, pE.second).first;
                }
                busyC.emplace(std::make_pair(max(max(pC.first, pE.first), t), res), i);
                busyE.emplace(std::make_pair(max(max(pC.first, pE.first), t), res), i);
            }
            return res;
        }
        else{
            return t;
        }
    }
}

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

double assignCloud(int i, double t, double w, double up, double dn,
                   std::set<pair<double, double> > &not_executing,
                   std::set<std::pair<std::pair<double, double>, int> > &executing,
                   std::set<pair<double, double> > &not_upcomC,
                   std::set<std::pair<std::pair<double, double>, int> > &upcomC,
                   std::set<pair<double, double> > &not_upcomE,
                   std::set<std::pair<std::pair<double, double>, int> > &upcomE,
                   std::set<pair<double, double> > &not_dncomC,
                   std::set<std::pair<std::pair<double, double>, int> > &dncomC,
                   std::set<pair<double, double> > &not_dncomE,
                   std::set<std::pair<std::pair<double, double>, int> > &dncomE,
                   bool simulate){
    double end_up = 0, end_exe = 0, end_dn = 0;
    end_up = assignResource(i, t, up, not_upcomC, upcomC, not_upcomE, upcomE, simulate);
    end_exe = assignResource(i, end_up, w, not_executing, executing, simulate);
    end_dn = assignResource(i, end_exe, dn, not_dncomC, dncomC, not_dncomE, dncomE, simulate);

    return end_dn;
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
        std::vector<std::map<int, double> > &upC, std::vector<std::map<int, double> > &wC,
        std::vector<std::map<int, double> > &dnC, std::vector<std::map<int, double> > &wE,
        std::map<int, double> &min_processing_times,
        std::map<int, double> &r, std::map<int, int> &o, double t, double S,
        std::vector<std::set<std::pair<std::pair<double, double>, int> > > &executingC,
        std::vector<std::set<std::pair<std::pair<double, double>, int> > > &executingE,
        std::vector<std::set<std::pair<std::pair<double, double>, int> > > &upcomC,
        std::vector<std::set<std::pair<std::pair<double, double>, int> > > &upcomE,
        std::vector<std::set<std::pair<std::pair<double, double>, int> > > &dncomC,
        std::vector<std::set<std::pair<std::pair<double, double>, int> > > &dncomE){
    int pc = executingC.size(), pe = executingE.size();
    for(int j = 0; j<pc; ++j){
        executingC[j].clear();
        upcomC[j].clear();
        dncomC[j].clear();
    }
    for(int j = 0; j<pe; ++j){
        executingE[j].clear();
        upcomE[j].clear();
        dncomE[j].clear();
    }
    std::vector<std::set<pair<double, double> > > not_executingC(pc), not_executingE(pe), not_upcomC(pc), not_upcomE(pe), not_dncomC(pc), not_dncomE(pe);
    for(int j = 0; j<pc; ++j){
        not_executingC[j].emplace(t, std::numeric_limits<double>::infinity());
        not_upcomC[j].emplace(t, std::numeric_limits<double>::infinity());
        not_dncomC[j].emplace(t, std::numeric_limits<double>::infinity());
    }
    for(int j = 0; j<pe; ++j){
        not_executingE[j].emplace(t, std::numeric_limits<double>::infinity());
        not_upcomE[j].emplace(t, std::numeric_limits<double>::infinity());
        not_dncomE[j].emplace(t, std::numeric_limits<double>::infinity());
    }
    std::set<std::pair<double, int> > deadlines;
    for(auto x:r){
        int i = x.first;
        deadlines.emplace(S*min_processing_times[i]+r[i], i);
    }
    for(auto p:deadlines){
        int i = p.second;
        int jc = 0;
        double tc = std::numeric_limits<double>::infinity();
        for(int j = 0; j<pc; ++j){
            double new_tc = assignCloud(i, t, wC[j][i], upC[j][i], dnC[j][i], not_executingC[j], executingC[j],
                                        not_upcomC[j], upcomC[j], not_upcomE[o[i]], upcomE[o[i]], not_dncomC[j],
                                        dncomC[j], not_dncomE[o[i]], dncomE[o[i]], true);
            if(new_tc < tc){
                tc = new_tc;
                jc = j;
            }
        }
        double te = assignEdge(i, t, wE[o[i]][i], not_executingE[o[i]], executingE[o[i]], true);
        if(tc < te){
            assignCloud(i, t, wC[jc][i], upC[jc][i], dnC[jc][i], not_executingC[jc], executingC[jc],
                        not_upcomC[jc], upcomC[jc], not_upcomE[o[i]], upcomE[o[i]], not_dncomC[jc],
                        dncomC[jc], not_dncomE[o[i]], dncomE[o[i]], false);
        }
        else{
            assignEdge(i, t, wE[o[i]][i], not_executingE[o[i]], executingE[o[i]], false);
        }
        double finishing_time = min(tc, te);
        if(finishing_time > p.first){
//std::cerr << min_processing_times[i] << std::endl;
//std::cerr << "time " << t << std::endl;
//std::cerr << "fail with " << finishing_time << " instead of " << p.first << std::endl;
            return false;
        }
    }
    return true;
}
///TODO: add the current S (useless to try to do better than that)
void schedule(
        std::vector<std::map<int, double> > &upC, std::vector<std::map<int, double> > &wC,
        std::vector<std::map<int, double> > &dnC, std::vector<std::map<int, double> > &wE,
        std::map<int, double> &min_processing_times,
        std::map<int, double> &r, std::map<int, int> &o, double t,
        std::vector<std::set<std::pair<std::pair<double, double>, int> > > &executingC,
        std::vector<std::set<std::pair<std::pair<double, double>, int> > > &executingE,
        std::vector<std::set<std::pair<std::pair<double, double>, int> > > &upcomC,
        std::vector<std::set<std::pair<std::pair<double, double>, int> > > &upcomE,
        std::vector<std::set<std::pair<std::pair<double, double>, int> > > &dncomC,
        std::vector<std::set<std::pair<std::pair<double, double>, int> > > &dncomE,
        double curS){
    double a = 1, b = 1;
    while(!schedule(upC, wC, dnC, wE, min_processing_times, r, o, t, b, executingC, executingE, upcomC, upcomE, dncomC, dncomE)){
        b*=2;
        //std::cerr << b << std::endl;
    }
    while(b-a>eps){
        double S = (a+b)/2;
        if(!schedule(upC, wC, dnC, wE, min_processing_times, r, o, t, S, executingC, executingE, upcomC, upcomE, dncomC, dncomE)){
            a = S;
        }
        else{
            b = S;
        }
    }
    double S = b;
    schedule(upC, wC, dnC, wE, min_processing_times, r, o, t, S, executingC, executingE, upcomC, upcomE, dncomC, dncomE);
}
}

double SSFEDF2::solve(){
    std::set<std::tuple<double, bool, int, bool, int> > affectation;
    std::vector<Processor> &Pe = scheduler.Pe,
                           &Pc = scheduler.Pc;
    int pe = Pe.size(), pc = Pc.size();
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
            std::vector<std::map<int, double> > upC(pc), wC(pc), dnC(pc), wE(pe);
            std::map<int, double> min_processing_times, r;
            std::map<int, int> o;
            for(int j = 0; j<pc; ++j){
                auto &P = scheduler.Pc[j];
                for(auto x:P.tasks){
                    int i = x.first;

                    Task &T = x.second;
                    upC[j][i] = P.upcom_time(i);
                    wC[j][i] = P.exec_time(i);
                    dnC[j][i] = P.dncom_time(i);
                    min_processing_times[i] = scheduler.min_processing_times[i];
                    r[i] = T.r;
                    o[i] = T.origin;
                }
            }
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
            std::vector<std::set<std::pair<std::pair<double, double>, int> > > executingC(pc), executingE(pe), upcomC(pc), upcomE(pe), dncomC(pc), dncomE(pe);
            double curS = scheduler.get_max_stretch();
            //std::cerr << "ffoo" << std::endl;
            schedule(upC, wC, dnC, wE, min_processing_times, r, o, time, executingC, executingE, upcomC, upcomE, dncomC, dncomE, curS);
            //std::cerr << "bbar" << std::endl;
            std::set<std::tuple<double, int, bool, int, int> > events; //time, task, proctype, proc, eventtype
            for(int j = 0; j<pc; ++j){
                for(auto x:upcomC[j]){
                    events.emplace(x.first.first, x.second, Processor::CLOUD, j, 0);
                    events.emplace(x.first.second, x.second, Processor::CLOUD, j, 3);
                }
                for(auto x:executingC[j]){
                    events.emplace(x.first.first, x.second, Processor::CLOUD, j, 1);
                    events.emplace(x.first.second, x.second, Processor::CLOUD, j, 4);
                }
                for(auto x:dncomC[j]){
                    events.emplace(x.first.first, x.second, Processor::CLOUD, j, 2);
                    events.emplace(x.first.second, x.second, Processor::CLOUD, j, 5);
                }
            }
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
