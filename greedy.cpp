/* Copyright (C) Redouane ELGHAZI
 * TBD
 */
#include <limits>
#include <algorithm>
#include <map>
#include <queue>
#include <iostream>

#include "greedy.hpp"


double Greedy::solve(){
    std::vector<Processor> &Pe = scheduler.Pe,
                           &Pc = scheduler.Pc;
    int pe = Pe.size(), pc = Pc.size();
    double time = 0;
    while(!scheduler.empty()){
        double new_time = scheduler.next_release();
        std::priority_queue<std::pair<double,
                                      std::pair<int, int> > > pQueue;
        std::map<int, std::vector<std::pair<double, std::pair<bool, int> > > > stretches;
        for(int i:scheduler.active_tasks){
            stretches[i] = std::vector<std::pair<double, std::pair<bool, int> > >(0);
            int j = scheduler.get_tasks()[i].origin;
            double f = time+Pe[j].remaining_time(i);
            double s = scheduler.calc_stretch(i, f);
            stretches[i].emplace_back(s, std::make_pair(Processor::EDGE,j));
            for(int j = 0; j<pc; ++j){
                double f = time+Pc[j].remaining_time(i);
                double s = scheduler.calc_stretch(i, f);
                stretches[i].emplace_back(s, std::make_pair(Processor::CLOUD, j));
            }
            std::sort(stretches[i].begin(), stretches[i].end());
            pQueue.emplace(stretches[i][0].first, std::make_pair(i,0));
        }
        std::vector<int> &execute_cloud = scheduler.execute_cloud,
                         &execute_edge = scheduler.execute_edge,
                         &upcommunicate = scheduler.upcommunicate,
                         &downcommunicate = scheduler.downcommunicate;
        for(int &i:execute_cloud)
            i = -1;
        for(int &i:execute_edge)
            i = -1;
        for(int &i:upcommunicate)
            i = -1;
        for(int &i:downcommunicate)
            i = -1;
        std::set<int> upcommunicatingEdge, downcommunicatingEdge;
        while(!pQueue.empty()){
            auto p = pQueue.top();
            pQueue.pop();
            int i = p.second.first;
            unsigned int k = p.second.second;
            bool type = stretches[i][k].second.first;
            int j = stretches[i][k].second.second;
            bool assigned = false;
            if(type == Processor::CLOUD){
                if(!Pc[j].is_on_cloud(i)){
                    if(upcommunicate[j] == -1 and
                       upcommunicatingEdge.find(Pc[j].origin(i))==upcommunicatingEdge.end()){
                        upcommunicate[j] = i;
                        assigned = true;
                        upcommunicatingEdge.insert(Pc[j].origin(i));
                        new_time = std::min(new_time, time + Pc[j].upcom_time(i));
                    }
                }
                else if(!Pc[j].is_executed(i)){
                    if(execute_cloud[j] == -1){
                        execute_cloud[j] = i;
                        assigned = true;
                        new_time = std::min(new_time, time + Pc[j].exec_time(i));
                    }
                }
                else{
                    if(downcommunicate[j] == -1 and
                       downcommunicatingEdge.find(Pc[j].origin(i))==downcommunicatingEdge.end()){
                        downcommunicate[j] = i;
                        assigned = true;
                        downcommunicatingEdge.insert(Pc[j].origin(i));
                        new_time = std::min(new_time, time + Pc[j].dncom_time(i));
                    }
                }
            }
            else{
                if(execute_edge[j] == -1){
                    execute_edge[j] = i;
                    assigned = true;
                    new_time = std::min(new_time, time + Pe[j].exec_time(i));
                }
            }
            if(!assigned and k+1<stretches[i].size()){
                pQueue.emplace(stretches[i][k+1].first, std::make_pair(i,k+1));
            }
        }
        //std::cout << time << "->" << new_time << std::endl;
        /*for(int j = 0; j<pc; ++j){
            std::cout << "\tC" << j << ": ex" << execute_cloud[j]
                      << " up" << upcommunicate[j]
                      << " dn" << downcommunicate[j] << std::endl;
        }
        for(int j = 0; j<pe; ++j){
            std::cout << "\tE" << j << ": ex" << execute_edge[j] << std::endl;
        }*/
        new_time = scheduler.advance(new_time);
        time = new_time;
    }
    return scheduler.get_max_stretch();
}
