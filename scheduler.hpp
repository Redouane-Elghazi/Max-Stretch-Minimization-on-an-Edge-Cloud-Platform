#ifndef SCHEDULER_HPP_INCLUDED
#define SCHEDULER_HPP_INCLUDED
#include <map>
#include <set>
#include <vector>

#include "processor.hpp"
#include "task.hpp"

class Scheduler{
public:
    Scheduler(int pc, std::vector<double> &S, std::vector<Task> &tasks);
    const std::vector<Processor> &get_Pc() const{return Pc;}
    const std::vector<Processor> &get_Pe() const{return Pe;}
    const std::vector<Task> &get_tasks() const{return tasks;}
    double get_max_stretch() const;
    double calc_stretch(int i, double f) const;
    double advance(double new_time);
    double next_release();
    bool empty();
    void activate_task(int i);
    void deactivate_task(int i);
    std::vector<int> get_new_tasks();
    std::vector<int> execute_cloud;
    std::vector<int> execute_edge;
    std::vector<int> upcommunicate;
    std::vector<int> downcommunicate;

    std::vector<Processor> Pc;
    std::vector<Processor> Pe;
    std::set<int> active_tasks;
    std::vector<int> new_tasks;

    std::set<std::pair<double, int> > release_times;
    std::vector<double> end_times;
    std::vector<double> min_processing_times;
    std::vector<double> stretch;
private:
    std::vector<Task> tasks;
    double max_stretch;
    double time;
};

#endif // SCHEDULER_HPP_INCLUDED
