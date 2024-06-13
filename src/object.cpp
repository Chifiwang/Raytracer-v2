#include "object.hpp"
#include "vec3.hpp"

bool Sphere::hit(const ray& r, collision_history& data) const
{
    vec3 p = m_centre - r.origin();
    vec3 d = r.direction();

    double A = dot(d, d);
    double B = dot(d, p);
    double C = dot(p, p) - m_radius * m_radius;

    double D = B * B - A * C;
    if (D < 0) {
        return false;
    }

    double sqrt_D = std::sqrt(D);
    double t = (B - sqrt_D) / A;
    if (!data.min_dist.contains(t)) {
        t = (B + sqrt_D) / A;
        if (!data.min_dist.contains(t)) {
            return false;
        }
    }

    data.collision = r.at(t);
    data.normal = (data.collision - m_centre) / m_radius;
    data.normal = dot(data.normal, d) > 0 ? -data.normal : data.normal;
    data.mat = mat;
    data.min_dist.set_max(t);

    return true;
}
