/* Copyright (C) Redouane ELGHAZI
 * TBD
 */
#include <limits>
#include <algorithm>
#include <iostream>

#include "srpt.hpp"

double SRPT::solve(){
    std::vector<Processor> &Pe = scheduler.Pe,
                           &Pc = scheduler.Pc;
    int pe = Pe.size(), pc = Pc.size();
    double time = 0;
    while(!scheduler.empty()){
        double new_time = scheduler.next_release();
                            //std::cerr << new_time << std::endl;
        std::vector<std::pair<std::pair<double, bool>,
                    std::pair<int, int> > > finishing_times; //TODO: Opti this, set, and remove those that are not relevant anymore
        for(int j = 0; j<pe; ++j){
            for(auto &p:Pe[j].tasks){
                int i = p.first;
                finishing_times.emplace_back(std::make_pair(Pe[j].remaining_time(i), Processor::EDGE),
                                             std::make_pair(i,j));
            }
        }
        for(int j = 0; j<pc; ++j){
            for(auto &p:Pc[j].tasks){
                int i = p.first;
                finishing_times.emplace_back(std::make_pair(Pc[j].remaining_time(i), Processor::CLOUD),
                                             std::make_pair(i,j));
            }
        }
        std::sort(finishing_times.begin(), finishing_times.end());
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
        std::set<int> assigned_tasks;
        std::set<int> upcommunicatingEdge, downcommunicatingEdge;
        for(auto &p:finishing_times){
            bool type = p.first.second;
            int i = p.second.first, j = p.second.second;
            if(assigned_tasks.find(i) == assigned_tasks.end()){
                if(type == Processor::CLOUD){
                    if(!Pc[j].is_on_cloud(i)){
                        if(upcommunicate[j] == -1 and
                           upcommunicatingEdge.find(Pc[j].origin(i))==upcommunicatingEdge.end()){
                            upcommunicate[j] = i;
                            assigned_tasks.insert(i);
                            upcommunicatingEdge.insert(Pc[j].origin(i));
                            new_time = std::min(new_time, time + Pc[j].upcom_time(i));
                            //std::cerr << i << "-" << Pc[j].upcom_time(i) << std::endl;
                        }
                    }
                    else if(!Pc[j].is_executed(i)){
                        if(execute_cloud[j] == -1){
                            execute_cloud[j] = i;
                            assigned_tasks.insert(i);
                            new_time = std::min(new_time, time + Pc[j].exec_time(i));
                            //std::cerr << i << "+" << Pc[j].exec_time(i) << std::endl;
                        }
                    }
                    else{
                        if(downcommunicate[j] == -1 and
                           downcommunicatingEdge.find(Pc[j].origin(i))==downcommunicatingEdge.end()){
                            downcommunicate[j] = i;
                            assigned_tasks.insert(i);
                            downcommunicatingEdge.insert(Pc[j].origin(i));
                            new_time = std::min(new_time, time + Pc[j].dncom_time(i));
                            //std::cerr << i << " " << Pc[j].dncom_time(i) << std::endl;
                        }
                    }
                }
                else{
                    if(execute_edge[j] == -1){
                        execute_edge[j] = i;
                        assigned_tasks.insert(i);
                        new_time = std::min(new_time, time + Pe[j].exec_time(i));
                            //std::cerr << i << "*" << Pe[j].exec_time(i) << std::endl;
                    }
                }
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
