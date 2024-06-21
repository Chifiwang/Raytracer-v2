#include "bvh.hpp"
#include "utils.hpp"

void aabb::expand(const point3& p)
{
    m_min = point3(
        fmin(p.x, m_min.x),
        fmin(p.y, m_min.y),
        fmin(p.z, m_min.z));
    m_max = point3(
        fmax(p.x, m_max.x),
        fmax(p.y, m_max.y),
        fmax(p.z, m_max.z));
}

void aabb::expand(const aabb& bbox)
{
    m_min = point3(
        fmin(bbox.m_min.x, m_min.x),
        fmin(bbox.m_min.y, m_min.y),
        fmin(bbox.m_min.z, m_min.z));
    m_max = point3(
        fmax(bbox.m_max.x, m_max.x),
        fmax(bbox.m_max.y, m_max.y),
        fmax(bbox.m_max.z, m_max.z));
}

bool aabb::contains(const ray& r) const
{
    vec3 t_min = (m_min - r.origin) / r.direction;
    vec3 t_max = (m_max - r.origin) / r.direction;

    vec3 t1 = min(t_min, t_max);
    vec3 t2 = max(t_min, t_max);

    double t_near = fmax(fmax(t1.x, t1.y), t1.z);
    double t_far = fmin(fmin(t2.x, t2.x), t2.z);

    return t_near < t_far;
}
