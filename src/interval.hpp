#ifndef INTERVAL_HPP
#define INTERVAL_HPP
#include "utils.hpp"

class Interval {
    double m_max;
    double m_min;

public:
    Interval()
        : m_max(-infty)
        , m_min(infty)
    {
    }
    Interval(double min, double max)
        : m_max(max)
        , m_min(min)
    {
    }

    void set_min(double x);
    void set_max(double x);

    bool contains(double x) const;
    double clamp(double x) const;
};

#endif // !INTERVAL_HPP
