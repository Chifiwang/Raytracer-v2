#include "interval.hpp"

void Interval::set_max(double x) { m_max = x; }
void Interval::set_min(double x) { m_min = x; }

bool Interval::contains(double x) const { return m_min < x && x < m_max; }
double Interval::clamp(double x) const
{
    if (this->contains(x)) {
        return x;
    }
    if (x <= m_min) {
        return m_min;
    }
    return m_max;
}

Interval Interval::Unit = Interval(0, 1);
