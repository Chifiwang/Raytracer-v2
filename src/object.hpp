#ifndef OBJECT_HPP
#define OBJECT_HPP
// IWYU pragma: private, include "raytracer.hpp"
#include "ray.hpp"
#include "interval.hpp"

struct collision_history {
    point3 collision;
    vec3 normal;
    color texture;
    Interval min_dist;
};

class Object {
public:
    ~Object() = default;
    virtual bool hit(const ray &r, collision_history &data) const = 0;
};

class Sphere : public Object {
    double m_radius;
    point3 m_centre;
public:
    ~Sphere() = default;
    Sphere(double radius, point3 centre) : m_radius(radius), m_centre(centre) {}
    Sphere(point3 centre, double radius) : m_radius(radius), m_centre(centre) {}
    bool hit(const ray &r, collision_history &data) const override;
};

#endif // !OBJECT_HPP
