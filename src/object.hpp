#ifndef OBJECT_HPP
#define OBJECT_HPP
// IWYU pragma: private, include "raytracer.hpp"
#include "interval.hpp"
#include "material.hpp"
#include "ray.hpp"
#include <memory>

struct collision_history {
    point3 collision;
    vec3 normal;
    std::shared_ptr<Material> mat;
    Interval min_dist;
};

class Object {
public:
    ~Object() = default;
    virtual bool hit(const ray& r, collision_history& data) const = 0;
};

class Sphere : public Object {
    double m_radius;
    point3 m_centre;
    std::shared_ptr<Material> mat;

public:
    ~Sphere() = default;
    Sphere(double radius, point3 centre, std::shared_ptr<Material> mat)
        : m_radius(radius)
        , m_centre(centre)
        , mat(mat)
    {
    }
    Sphere(point3 centre, double radius, std::shared_ptr<Material> mat)
        : m_radius(radius)
        , m_centre(centre)
        , mat(mat)
    {
    }
    bool hit(const ray& r, collision_history& data) const override;
};

#endif // !OBJECT_HPP
