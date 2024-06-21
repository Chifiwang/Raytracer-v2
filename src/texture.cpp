#include "texture.hpp"
#include "vec3.hpp"

color Solid::sample(const point3& p) const
{
    return attenuation;
}

color Gradient::sample(const point3& p) const
{
    double a = 0.5 * (normalize(p).y + 1);
    return (1.0 - a) * primary + a * secondary;
}
