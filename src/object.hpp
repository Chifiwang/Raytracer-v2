#ifndef OBJECT_HPP
#define OBJECT_HPP
// IWYU pragma: private, include "raytracer.hpp"
#include "interval.hpp"
#include "material.hpp"
#include "ray.hpp"
#include <vector>

class Texture;

struct collision_history {
    point3 collision;
    vec3 normal;
    Material* mat;
    Texture* tex;
    Interval min_dist = Interval(0.001, infty);
};

struct object_props {
    Material* material;
    Texture* texture;
};

class Object {
public:
    virtual ~Object() = default;
    virtual bool hit(const ray& r, collision_history& data) const = 0;
    virtual point3 random_point() const = 0;
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
    point3 random_point() const override;
};

class Sphere : public Object {
    double m_radius;
    point3 m_centre;
    object_props m_props;

public:
    ~Sphere() = default;
    Sphere(double radius, point3 centre, Material* mat, Texture* tex)
        : m_radius(radius)
        , m_centre(centre)
    {
        m_props.material = mat;
        m_props.texture = tex;
    }
    Sphere(point3 centre, double radius, Material* mat, Texture* tex)
        : m_radius(radius)
        , m_centre(centre)
    {
        m_props.material = mat;
        m_props.texture = tex;
    }
    bool hit(const ray& r, collision_history& data) const override;
    point3 random_point() const override;
};

#endif // !OBJECT_HPP
