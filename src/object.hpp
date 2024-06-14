#ifndef OBJECT_HPP
#define OBJECT_HPP
// IWYU pragma: private, include "raytracer.hpp"
#include "interval.hpp"
#include "material.hpp"
#include "ray.hpp"
#include <vector>

struct collision_history {
    point3 collision;
    vec3 normal;
    Material *mat;
    Interval min_dist = Interval(0.001, infty);
};

class Object {
public:
    virtual ~Object() = default;
    virtual bool hit(const ray& r, collision_history& data) const = 0;
};

class Collection : public Object {
    std::vector<Object*> objects;

public:
    ~Collection() = default;
    Collection() = default;
    Collection(Object* obj)
    {
        objects.push_back(obj);
    }
    void add(Object* obj);
    bool hit(const ray& r, collision_history& data) const override;
};

class Sphere : public Object {
    double m_radius;
    point3 m_centre;
    Material* mat;

public:
    ~Sphere() = default;
    Sphere(double radius, point3 centre, Material* mat)
        : m_radius(radius)
        , m_centre(centre)
        , mat(mat)
    {
    }
    Sphere(point3 centre, double radius, Material* mat)
        : m_radius(radius)
        , m_centre(centre)
        , mat(mat)
    {
    }
    bool hit(const ray& r, collision_history& data) const override;
};

#endif // !OBJECT_HPP
