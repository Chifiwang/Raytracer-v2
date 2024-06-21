#ifndef RAY_HPP
#define RAY_HPP

#include "vec3.hpp"

struct ray {
    vec3 direction;
    point3 origin;

public:
    ray(point3 point, vec3 direction)
        : direction(direction)
        , origin(point)
    {
    }
    point3 at(double t) const;
};

#endif // !RAY_HPP
