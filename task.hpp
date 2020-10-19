#ifndef TASK_HPP_INCLUDED
#define TASK_HPP_INCLUDED

class Task{
public:
    Task(){}
    Task(double w, double up, double dn, double r, int origin):w(w), up(up), dn(dn), r(r), invested_time(0), origin(origin){}
    double remaining_time(double s, bool migrate) const;
    double upcom_time(double s, bool migrate) const;
    double exec_time(double s, bool migrate) const;
    double dncom_time(double s, bool migrate) const;
    bool is_on_cloud() const;
    bool is_executed() const;
    bool is_finished(double s, bool migrate) const;
    bool execute(double t, double s, bool migrate);
    bool upcommunicate(double t);
    bool downcommunicate(double t);
    double w;
    double up;
    double dn;
    double r;
    double invested_time;
    int origin;
};

#endif // TASK_HPP_INCLUDED
