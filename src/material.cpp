#include "material.hpp"
#include "object.hpp"
#include "raytracer.hpp"
#include "texture.hpp"
#include "vec3.hpp"
#include <cmath>

inline double schlick_fresnel(double cos_t)
{
    double c = Interval::Unit.clamp(1 - cos_t);
    double c2 = c * c;
    return c2 * c2 * c;
}

inline double diffuse_lobe(const vec3& l, const vec3& v, const collision_history& data)
{
    vec3 h = normalize(l - v);
    double fd90 = .5 + 2 * data.mat->roughness * sq(dot(l, h));

    double fl = schlick_fresnel(dot(data.normal, l));
    double fv = schlick_fresnel(dot(data.normal, v));
    double fd = lerp(1, fd90, fv) * lerp(1, fd90, fl);

    return inv_pi * fd;
}

inline double specular_lobe(double a, double NoH, double NoL, double NoV, double f0)
{
    double NoH2 = NoH * NoH;

    double pow = (NoH2 - 1) / (a * NoH2);
    double den = pi * a * sq(NoH2);

    double d = std::exp(pow) / den;

    // double g = NoL * NoV; // got canceled out

    double f = 1 - lerp(f0, 1, NoL);

    return f * d / 4;

}

vec3 scatter(const vec3& v, const collision_history& data)
{
    vec3 diffuse_dir = normalize(data.normal + random_unit_vec3());
    vec3 specular_dir = reflect(v, data.normal);
    specular_dir = normalize(lerp(specular_dir, diffuse_dir, sq(data.mat->roughness)));
    vec3 reflected_dir = lerp(diffuse_dir, specular_dir, random_double() < data.mat->specular);

    if (!data.mat->transmissive) {
        return reflected_dir;
    }

    double IoR = (data.outwards_normal) ? data.mat->IoR : 1 / data.mat->IoR;

    vec3 refracted_dir = refract(normalize(v), data.normal, IoR);
    refracted_dir = normalize(lerp(refracted_dir, -diffuse_dir, sq(data.mat->roughness)));
    return lerp(reflected_dir, refracted_dir, random_double() < data.mat->transmissive);
}

double brdf(const vec3& l, const vec3& v, const collision_history& data)
{
    // double NoL = dot(data.normal, l);
    // if (NoL < 0) {
    //     return color(0);
    // }

    vec3 h = normalize(l - v);
    double NoH = dot(data.normal, h);
    double NoL = dot(data.normal, l);
    double NoV = dot(data.normal, -v);
    double a = sq(data.mat->roughness);
    double f0 = sq((1 - data.mat->IoR) / (1 + data.mat->IoR));
    // double d = specular_lobe(sq(data.mat->roughness), dot(data.normal, h));

    return lerp(diffuse_lobe(l, v, data), Interval::Unit.clamp(specular_lobe(a, NoH, NoL, NoV, f0)), data.mat->specular);
    // return diffuse_lobe(l, v, data); // add specular lobe to principled brdf
    // return data.tex->sample(data.collision);
}
