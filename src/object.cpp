#include "object.hpp"
#include "raytracer.hpp"
#include "vec3.hpp"

void Collection::add(Object* obj)
{
    objects.push_back(obj);
}

bool Collection::hit(const ray& r, collision_history& data) const
{
    bool res = false;
    for (auto obj : objects) {
        if (obj->hit(r, data)) {
            res = true;
        }
    }

    return res;
}

point3 Collection::random_point() const {
    return objects[random_int(0, objects.size() - 1)]->random_point();
}

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
        return false;
    }

    data.collision = r.at(t);
    data.normal = (data.collision - m_centre) / m_radius;
    data.normal = dot(data.normal, d) > 0 ? -data.normal : data.normal;
    data.mat = m_props.material;
    data.tex = m_props.texture;
    data.min_dist.set_max(t);

    return true;
}

point3 Sphere::random_point() const {
    return m_centre + random_unit_vec3() * m_radius;
}
