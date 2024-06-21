#ifndef OBJECT_HPP
#define OBJECT_HPP
// IWYU pragma: private, include "raytracer.hpp"
#include "bvh.hpp"
#include "interval.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "vec3.hpp"
#include <algorithm>

class Texture;

struct collision_history {
    point3 collision;
    vec3 normal;
    Material* mat;
    Texture* tex;
    Interval min_dist = Interval(0.00001, infty);
    bool outwards_normal;
    double u, v;
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
    virtual aabb bounding_box() const = 0;
};

class Collection : public Object {
    std::vector<Object*> m_objects;
    aabb m_bbox;

public:
    ~Collection() = default;
    Collection() = default;
    Collection(Object* obj)
    {
        m_objects.push_back(obj);
        m_bbox = obj->bounding_box();
    }
    void add(Object* obj);
    bool hit(const ray& r, collision_history& data) const override;
    point3 random_point() const override;
    aabb bounding_box() const override;

    friend class bvh;
};

class Sphere : public Object {
    double m_radius;
    point3 m_centre;
    object_props m_props;
    aabb m_bbox;

public:
    ~Sphere() = default;
    Sphere(double radius, point3 centre, Material* mat, Texture* tex)
        : m_radius(radius)
        , m_centre(centre)
    {
        m_props.material = mat;
        m_props.texture = tex;
        m_bbox = aabb(m_centre - vec3(m_radius), m_centre + vec3(m_radius));
    }
    Sphere(point3 centre, double radius, Material* mat, Texture* tex)
        : m_radius(radius)
        , m_centre(centre)
    {
        m_props.material = mat;
        m_props.texture = tex;
        m_bbox = aabb(m_centre - vec3(m_radius), m_centre + vec3(m_radius));
    }
    bool hit(const ray& r, collision_history& data) const override;
    point3 random_point() const override;
    aabb bounding_box() const override;
};

class Quad : public Object {
    vec3 m_point;
    vec3 m_w;
    vec3 m_h;
    vec3 m_n;
    vec3 m_s;
    double m_d;
    object_props m_props;
    aabb m_bbox;

public:
    Quad(point3 point, vec3 w, vec3 h, Material* mat, Texture* tex)
        : m_point(point)
        , m_w(w)
        , m_h(h)
    {
        vec3 n = cross(w, h);
        m_s = n / dot(n, n);
        m_n = normalize(n);
        m_d = dot(m_n, point);
        m_props.material = mat;
        m_props.texture = tex;
        m_bbox = aabb(point, point + w + h);
        m_bbox.expand(aabb(point + w, point + h));
    }
    bool hit(const ray& r, collision_history& data) const override;
    point3 random_point() const override;
    aabb bounding_box() const override;

};

class bvh : public Object {
    Object* left;
    Object* right;
    bool is_leaf = false;
    aabb m_bbox;

    bvh(std::vector<Object*>& objs, size_t start, size_t end)
    {
        size_t span = end - start;
        switch (span) {
        case 1:
            left = right = objs[start];
            is_leaf = true;
            break;
        case 2:
            left = objs[start];
            right = objs[start + 1];
            is_leaf = true;
            break;
        default:
            int maj_axis = m_bbox.m_major_axis;
            std::sort(objs.begin() + start, objs.begin() + end,
                [maj_axis](const Object* a, const Object* b) -> bool {
                    return a->bounding_box().m_min[maj_axis]
                        < b->bounding_box().m_min[maj_axis];
                });
            size_t mid = start + (span >> 2);
            left = new bvh(objs, start, mid);
            right = new bvh(objs, mid, end);
            break;
        }
        m_bbox = left->bounding_box();
        m_bbox.expand(right->bounding_box());
    }

public:
    bvh(Collection objs)
        : bvh(objs.m_objects, 0, objs.m_objects.size())
    {
    }
    ~bvh()
    {
        if (!is_leaf) {
            delete left;
            delete right;
        }
    }
    bool hit(const ray& r, collision_history& data) const override;
    point3 random_point() const override;
    aabb bounding_box() const override;
};

#endif // !OBJECT_HPP
