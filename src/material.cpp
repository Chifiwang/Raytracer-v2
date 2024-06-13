#include "material.hpp"
#include "interval.hpp"
#include "object.hpp"
#include "vec3.hpp"

double schlick_fresnel(double cos_t)
{
    Interval i { 0, 1 };
    double m = i.clamp(1 - cos_t);
    double m2 = m * m;
    return m2 * m2 * m; // $m^{5}$;
}

double gtr1(double cos_td, double a) // GTR1
{
    if (a >= 1) {
        return 1 / pi;
    }

    double a2 = a * a;
    double t = 1 + (a2 - 1) * cos_td * cos_td;
    return (a2 - 1) / (pi * log2(a2) * t);
}

double gtr2(double cos_td, double a) // GTR2
{
    double a2 = a * a;
    double t = 1 + (a2 - 1) * cos_td * cos_td;
    return 1 / (pi * t * t);
}

double gtr2_aniso(double cos_td, double cos_tx, double cos_ty, double a_x, double a_y)
{
    return 1 / (pi * a_x * a_y * sq(sq(cos_td / a_x) + sq(cos_td / a_y) + cos_td * cos_td));
}

double smithg_ggx(double cos_t, double a_g)
{
    double a = a_g * a_g;
    double b = cos_t * cos_t;
    return 1 / (cos_t * sqrt(a + b - a * b));
}

double smithg_ggx_aniso(double cos_t, double cos_tx, double cos_ty, double a_x, double a_y)
{
    return 1 / (cos_t + sqrt(sq(cos_tx * a_x) + sq(cos_ty * a_y) + sq(cos_ty)));
}

color mon2lin(const color& c)
{
    return color(
        pow(c.x(), 2.2),
        pow(c.y(), 2.2),
        pow(c.z(), 2.2));
}

color disney_brdf(const vec3& l, const vec3& v, const collision_history &data)
{
    // vec3 n = data.normal;
    // auto mat = data.mat;
    //
    // double cos_tl = dot(n, l);
    // double cos_tv = dot(n, v);
    // if (cos_tl < 0 || cos_tv < 0) {
    //     return color(0);
    // }
    //
    // vec3 h = normalize(l + v);
    // double cos_td = dot(n, h);
    // double cos_tlh = dot(l, h);
    //
    // color cd_lin = mon2lin(mat->base_texture->sample(data.collision));
    // double cd_lum = dot(cd_lin, color(.3, .6, .1)); // luminance approx.
    //
    // // sheen lobe
    // color c_tint = cd_lum > 0 ? cd_lin / cd_lum : color(1);
    // color c_spec0 = lerp(mat->specular * .08 * lerp(color(1), c_tint, mat->specular_tint), cd_lin, mat->metallic);
    // color c_sheen = lerp(color(1), c_tint, mat->sheen_tint);
    //
    // // diffuse lobes
    // double f_rough = cos_tlh * cos_tlh * mat->roughness;
    // double fresnel_l = schlick_fresnel(cos_tl);
    // double fresnel_v = schlick_fresnel(cos_tv);
    // double f_d90 = 0.5 + 2 * f_rough;
    //
    // double diffuse_fresnel = lerp(1, f_d90, fresnel_l) * lerp(1, f_d90, fresnel_v);
    //
    // // subsurface approx lobe
    // double f_ss90 = f_rough;
    // double f_ss = lerp(1, f_ss90, fresnel_l) * lerp(1, f_ss90, fresnel_v);
    // double subsurface = 1.25 * (f_ss * (1 / (cos_tl + cos_tv) - .5) + .5);
    //
    // // Specular lobe
    // double aspect = sqrt(1 - mat->anisotropic * .9);
    // double a_x = fmax(.001, sq(mat->roughness) / aspect);
    // double a_y = fmax(.001, sq(mat->roughness) * aspect);
    // vec3 x = normalize(reflect(-v, n)); // TEMP SOL
    // vec3 y = cross(n, x);
    // double d_s = gtr2_aniso(cos_td, dot(h, x), dot(h, y), a_x, a_y);
    // double f_h = schlick_fresnel(cos_tlh);
    // color f_s = lerp(c_spec0, color(1), f_h);
    // double g_s = smithg_ggx_aniso(cos_tl, dot(l, x), dot(l, y), a_x, a_y);
    // g_s *= smithg_ggx_aniso(cos_tv, dot(v, x), dot(v, y), a_x, a_y);
    //
    // color sheen = f_h * mat->sheen * c_sheen;
    //
    // // clearcoat
    // double dr = gtr1(cos_td, lerp(.1, .001, mat->clearcoat_gloss));
    // double fr = lerp(.04, 1, f_h);
    // double gr = smithg_ggx(cos_tl, .25) * smithg_ggx(cos_tv, .25);
    //
    // return ((1 / pi) * lerp(diffuse_fresnel, subsurface, mat->subsurface) * cd_lin + sheen)
    //     * (1 - mat->metallic)
    //     + g_s * f_s * d_s + .25 * mat->clearcoat * dr * gr * fr;
    Interval i(0, 1);
    double r = i.clamp(dot(l, data.normal));
    return data.mat->base_texture->sample(data.collision) * r;
}
