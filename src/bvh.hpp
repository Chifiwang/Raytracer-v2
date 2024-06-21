#ifndef BVH_HPP
#define BVH_HPP

#include "ray.hpp"

struct aabb {
    point3 m_min;
    point3 m_max;
    int m_major_axis;

    aabb() { }
    aabb(const point3& min, const point3& max)
        : m_min(min)
        , m_max(max)
    {
        int x = max.x - min.x;
        int y = max.y - min.y;
        int z = max.z - min.z;

        if (x > y) {
            m_major_axis = (x > z) ? 0 : 2;
        } else {
            m_major_axis = (y > z) ? 1 : 2;
        }
    }
    bool contains(const ray& r) const;
    void expand(const point3& p);
    void expand(const aabb& bbox);

    friend class Object;
};

#endif // !BVH_HPP
