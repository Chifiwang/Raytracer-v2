#include "material.hpp"
#include "object.hpp"
#include "raytracer.hpp"
#include "texture.hpp"
#include "vec3.hpp"

inline color diffuse_lobe(const collision_history& data, double NoL)
{
    return data.tex->sample(data.collision) * NoL;
}

inline double specular_lobe(double a, double NoH)
{
    // GGX
    double NoH2 = NoH * NoH;
    double tan2 = (1 - NoH2) / NoH2;

    return 1 / pi * sq(a / (NoH2 * (a * a + tan2)));
}

vec3 scatter(const vec3& v, const collision_history& data)
{
    vec3 diffuse_dir = normalize(data.normal + random_unit_vec3());
    vec3 specular_dir = reflect(v, data.normal);
    specular_dir = normalize(lerp(specular_dir, diffuse_dir, sq(data.mat->roughness)));
    return normalize(lerp(diffuse_dir, specular_dir, random_double() < data.mat->specular));
}

color brdf(const vec3& l, const vec3& v, const collision_history& data)
{
    // double NoL = dot(data.normal, l);
    // if (NoL < 0) {
    //     return color(0);
    // }

    // vec3 h = normalize(-l + v);
    // double d = specular_lobe(sq(data.mat->roughness), dot(data.normal, h));

    // return diffuse_lobe(data, NoL);
    return data.tex->sample(data.collision);
}
