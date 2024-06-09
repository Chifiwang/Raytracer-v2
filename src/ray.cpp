#include "ray.hpp"

point3 ray::at(double t) const { return m_p + t * m_d; }
point3 ray::origin() const { return m_p; }
vec3 ray::direction() const { return m_d; }
