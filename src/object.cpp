#include "object.hpp"
#include "raytracer.hpp"
#include "vec3.hpp"

void Collection::add(Object* obj)
{
    m_objects.push_back(obj);
    m_bbox.expand(obj->bounding_box());
}

bool Collection::hit(const ray& r, collision_history& data) const
{
    bool res = false;
    for (auto obj : m_objects) {
        if (obj->hit(r, data)) {
            res = true;
        }
    }

    return res;
}

point3 Collection::random_point() const
{
    return m_objects[random_int(0, m_objects.size() - 1)]->random_point();
}

aabb Collection::bounding_box() const
{
    return m_bbox;
}

bool Sphere::hit(const ray& r, collision_history& data) const
{
    vec3 p = m_centre - r.origin;
    vec3 d = r.direction;

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
    data.outwards_normal = dot(data.normal, d) > 0;
    data.normal = data.outwards_normal ? -data.normal : data.normal;
    data.mat = m_props.material;
    data.tex = m_props.texture;
    data.min_dist.set_max(t);

    return true;
}

point3 Sphere::random_point() const
{
    return m_centre + random_unit_vec3() * m_radius;
}

aabb Sphere::bounding_box() const
{
    return m_bbox;
}

bool Quad::hit(const ray& r, collision_history& data) const
{
    double t = (m_d - dot(m_n, r.origin)) / dot(m_n, r.direction);

    if (!data.min_dist.contains(t)) {
        return false;
    }

    vec3 collision = r.at(t);
    vec3 p = collision - m_point;
    double a = dot(m_s, cross(p, m_h));
    double b = dot(m_s, cross(m_w, p));

    if (!Interval::Unit.contains(a) || !Interval::Unit.contains(b)) {
        return false;
    }

    data.collision = collision;
    data.outwards_normal = dot(m_n, r.direction) > 0;
    data.normal = data.outwards_normal ? -m_n : m_n;
    data.mat = m_props.material;
    data.tex = m_props.texture;
    data.min_dist.set_max(t);
    data.u = a;
    data.v = b;

    return true;
}

point3 Quad::random_point() const
{
    return m_point + random_double() * m_h + random_double() * m_w;
}

aabb Quad::bounding_box() const
{
    return m_bbox;
}

bool bvh::hit(const ray& r, collision_history& data) const
{
    if (!m_bbox.contains(r)) {
        return false;
    }

    // TODO: make this more optimal using a vector stack in the future
    bool hit_l = left->hit(r, data);
    bool hit_r = right->hit(r, data);

    return hit_l || hit_r;
}

aabb bvh::bounding_box() const
{
    return m_bbox;
}

point3 bvh::random_point() const
{
    return point3(0);
}
