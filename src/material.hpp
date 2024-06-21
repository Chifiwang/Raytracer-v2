#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "vec3.hpp"
struct collision_history;

// enum material_flags {
//     empty = 0b0 << 0,
//     metallic = 0b1 << 0, // 0 is dielectric, 1 is metallic
//     emissive = 0b1 << 1,
//     all = 0b11
// };

struct Material {
    // unsigned properties = material_flags::all;
    double roughness = 1;
    double specular = 0;
    double transmissive = 0;
    double IoR = 1;
    color contribution = color(1);
    color emission_color = color(0);
    double emission_power = 0;
};

vec3 scatter(const vec3& v, const collision_history&data);
double brdf(const vec3& l, const vec3& v, const collision_history& data);

#endif // !MATERIAL_HPP
