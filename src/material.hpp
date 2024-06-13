#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "texture.hpp"
#include "utils.hpp"
struct collision_history;

struct Material {
    std::shared_ptr<Texture> base_texture = std::make_shared<Solid>(color());
    double subsurface = 0;
    double metallic = 0;
    double specular = .5;
    double specular_tint = 0;
    double roughness = .5;
    double anisotropic = 0;
    double sheen = 0;
    double sheen_tint = .5;
    double clearcoat = 0;
    double clearcoat_gloss = 1;
};

double schlick_fresnel(double cos_t);
double gtr1(double cos_td, double a);
double gtr2(double cos_td, double a);
double gtr2_aniso(double cos_td, double cos_tx, double cos_ty, double a_x, double a_y);
double smithg_ggx(double cos_t, double a_g);
double smithg_ggx_aniso(double cos_t, double cos_tx, double cos_ty, double a_x, double a_y);
color mon2lin(const color &c);
color disney_brdf(const vec3 &l, const vec3 &v, const collision_history &data);

#endif // !MATERIAL_HPP
