#ifndef UTILS_HPP
#define UTILS_HPP
// IWYU pragma: private, include "raytracer.hpp"
// IWYU pragma: begin_exports
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
// IWYU pragma: end_exports

const double infty = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;
const double inv_pi = 0.31830988618;

inline double to_radians(double deg) { return deg * pi / 180.0; }
inline double random_double() { return rand() / (RAND_MAX + 1.0); }
inline double random_double(double min, double max)
{
    return min + (max - min) * random_double();
}
inline int random_int(int min, int max)
{
    return int(random_double(min, max + 1));
}
inline double sq(double x) { return x * x; }
inline double lerp(double v0, double v1, double t)
{
    return v0 + (v1 - v0) * t;
}

#endif // !UTILS_HPP
