#include "ray.hpp"

point3 ray::at(double t) const { return origin + t * direction; }
