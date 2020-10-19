/* Copyright (C) Redouane ELGHAZI
 * TBD
 */
#include <limits>
#include <iostream>

#include "scheduler.hpp"

Scheduler::Scheduler(int pc, std::vector<double> &S, std::vector<Task> &tasks): tasks(tasks){
    Pc = std::vector<Processor> (pc, Processor(1, Processor::CLOUD));
    Pe = std::vector<Processor> ();
    for(double s:S){
        Pe.emplace_back(s, Processor::EDGE);
    }
    int n = tasks.size();
    for(int i = 0; i<n; ++i){
        release_times.emplace(tasks[i].r, i);
    }
    execute_cloud = std::vector<int> (Pc.size(), -1);
    execute_edge = std::vector<int> (Pe.size(), -1);
    upcommunicate = std::vector<int> (Pc.size(), -1);
    downcommunicate = std::vector<int> (Pc.size(), -1);

    end_times = std::vector<double> (n, -1);
    min_processing_times = std::vector<double> (n, -1);
    stretch = std::vector<double> (n, -1);
    max_stretch = 0;
    time = 0;
}

double Scheduler::get_max_stretch() const{
    return max_stretch;
}

double Scheduler::calc_stretch(int i, double f) const{
    return (f-tasks[i].r)/min_processing_times[i];
}

double Scheduler::advance(double new_time){
    if(new_time-time<1e-10)
        new_time = time+1e-10;
    if(new_time<time)
        return time;
    int pc = Pc.size(), pe = Pe.size();
    std::vector<std::pair<int, double> > finished_tasks;
    for(int j = 0; j<pe; ++j){
        int i = execute_edge[j];
        if(i != -1 and Pe[j].tasks.find(i)!=Pe[j].tasks.end()){
//std::cerr << "exe edge " << i << " on " << j << " from " << time << " to " << new_time << " - " << Pe[j].exec_time(i) << std::endl;
            Pe[j].execute(i, new_time - time);
//std::cerr << Pe[j].exec_time(i) << std::endl;
            if(Pe[j].is_finished(i)){
                finished_tasks.emplace_back(i, new_time);
            }
        }
    }
    for(int j = 0; j<pc; ++j){
        int i_up = upcommunicate[j];
        if(i_up != -1 and Pc[j].tasks.find(i_up)!=Pc[j].tasks.end()){
//std::cerr << "up  cloud " << i_up << " on " << j << " from " << time << " to " << new_time << " - " << Pc[j].upcom_time(i_up) << std::endl;
            Pc[j].upcommunicate(i_up, new_time - time);
//std::cerr << Pc[j].upcom_time(i_up) << std::endl;
            if(Pc[j].is_finished(i_up)){
                finished_tasks.emplace_back(i_up, new_time);
            }
        }
        int i_exec = execute_cloud[j];
        if(i_exec != -1 and i_exec!=i_up and Pc[j].tasks.find(i_exec)!=Pc[j].tasks.end()){
//std::cerr << "exe cloud " << i_exec << " on " << j << " from " << time << " to " << new_time << " - " << Pc[j].exec_time(i_exec) << std::endl;
            Pc[j].execute(i_exec, new_time - time);
//std::cerr << Pc[j].exec_time(i_exec) << std::endl;
            if(Pc[j].is_finished(i_exec)){
                finished_tasks.emplace_back(i_exec, new_time);
            }
        }
        int i_dn = downcommunicate[j];
        if(i_dn != -1 and i_dn!=i_exec and i_dn!=i_up and Pc[j].tasks.find(i_dn)!=Pc[j].tasks.end()){
//std::cerr << "dn  cloud " << i_dn << " on " << j << " from " << time << " to " << new_time << " - " << Pc[j].dncom_time(i_dn) << std::endl;
            Pc[j].downcommunicate(i_dn, new_time - time);
//std::cerr << Pc[j].dncom_time(i_dn) << " " << Pc[j].is_finished(i_dn)<< std::endl;
            if(Pc[j].is_finished(i_dn)){
                finished_tasks.emplace_back(i_dn, new_time);
            }
        }
    }
    for(std::pair<int, double> p:finished_tasks){
        int i = p.first;
        double t = p.second;
        if(end_times[i] == -1 or end_times[i]>t){
            end_times[i] = t;
        }
        deactivate_task(i);
    }
    time = new_time;
    while(!release_times.empty() and new_time>=(*release_times.begin()).first){
        int i = (*release_times.begin()).second;
        activate_task(i);
        release_times.erase(release_times.begin());
    }
    return new_time;
}

double Scheduler::next_release(){
    if(!release_times.empty()){
        return (*release_times.begin()).first;
    }
    else{
        return std::numeric_limits<double>::infinity();
    }
}

bool Scheduler::empty(){
    return next_release()==std::numeric_limits<double>::infinity() and
           active_tasks.empty();
}

void Scheduler::activate_task(int i){
    new_tasks.push_back(i);
    active_tasks.insert(i);
    int pc = Pc.size();
    min_processing_times[i] = std::numeric_limits<double>::infinity();
    for(int j = 0; j<pc; ++j){
        Pc[j].add_task(i, tasks[i]);
        min_processing_times[i] = std::min(min_processing_times[i], Pc[j].remaining_time(i));
    }
    Pe[tasks[i].origin].add_task(i, tasks[i]);
    min_processing_times[i] = std::min(min_processing_times[i], Pe[tasks[i].origin].remaining_time(i));
}

void Scheduler::deactivate_task(int i){
    active_tasks.erase(i);
    int pc = Pc.size(), pe = Pe.size();
    for(int j = 0; j<pe; ++j){
        Pe[j].remove_task(i);
    }
    for(int j = 0; j<pc; ++j){
        Pc[j].remove_task(i);
    }
    stretch[i] = calc_stretch(i, end_times[i]);
    /*std::cerr << "Task " << i << " was released at " << tasks[i].r <<
                 " and finished at " << end_times[i] <<
                 " with stretch " << stretch[i] <<
                 " (could finish at " << tasks[i].r+min_processing_times[i] << ")" << std::endl;
    */
    max_stretch = std::max(stretch[i], max_stretch);
}

std::vector<int> Scheduler::get_new_tasks(){
    std::vector<int> res(new_tasks);
    new_tasks.clear();
    return res;
}
