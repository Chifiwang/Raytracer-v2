#ifndef RAY_HPP
#define RAY_HPP

#include "vec3.hpp"

class ray {
    vec3 m_d;
    point3 m_p;

public:
    ray(point3 point, vec3 direction)
        : m_d(direction)
        , m_p(point)
    {
    }
    point3 at(double t) const;
    point3 origin() const;
    vec3 direction() const;
};

#endif // !RAY_HPP
