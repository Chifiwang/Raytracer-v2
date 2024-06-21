#include "vec3.hpp"
#include "raytracer.hpp"
#include <iostream>

double vec3::operator[](int t) const
{
    switch (t) {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    default:
        __builtin_unreachable();
    }
}
double vec3::operator[](int t)
{
    switch (t) {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    default:
        __builtin_unreachable();
    }
}
vec3 operator+(const vec3& u, const vec3& v)
{
    return vec3(v.x + u.x, v.y + u.y, v.z + u.z);
}
vec3 operator-(const vec3& u, const vec3& v)
{
    return vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}
vec3 operator-(const vec3& u) { return vec3(-u.x, -u.y, -u.z); }
vec3 operator*(double t, const vec3& u)
{
    return vec3(t * u.x, t * u.y, t * u.z);
}
vec3 operator*(const vec3& u, double t) { return t * u; }

vec3 operator*(const vec3& u, const vec3& v)
{
    return vec3(
        u.x * v.x,
        u.y * v.y,
        u.z * v.z);
}

vec3 operator/(const vec3& u, double t) { return (1 / t) * u; }
vec3 operator/(const vec3& u, const vec3& v)
{
    return vec3(
        u.x / v.x,
        u.y / v.y,
        u.z / v.z);
}

vec3& vec3::operator+=(const vec3& u)
{
    x += u.x;
    y += u.y;
    z += u.z;

    return *this;
}
vec3& vec3::operator-=(const vec3& u)
{
    x -= u.x;
    y -= u.y;
    z -= u.z;

    return *this;
}
vec3& vec3::operator*=(const double t)
{
    x *= t;
    y *= t;
    z *= t;

    return *this;
}
vec3& vec3::operator*=(const vec3& u)
{
    x *= u.x;
    y *= u.y;
    z *= u.z;

    return *this;
}
vec3& vec3::operator/=(const double t) { return (*this) *= 1 / t; }

double vec3::magnetude() const { return std::sqrt(dot(*this, *this)); }

bool vec3::near_zero() const
{
    double e = 1e-8;
    return (std::fabs(x) < e && std::fabs(y) < e && std::fabs(z) < e);
}

vec3 normalize(const vec3& u) { return u / u.magnetude(); }

double dot(const vec3& u, const vec3& v)
{
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

vec3 cross(const vec3& u, const vec3& v)
{
    return vec3(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z,
        u.x * v.y - u.y * v.x);
}

vec3 reflect(const vec3& u, const vec3& n) { return u - 2 * dot(u, n) * n; }

vec3 refract(const vec3& inc, const vec3& n, double refraction_ratio)
{
    double cos_t = fmin(dot(-inc, n), 1.0);
    vec3 r_perp = refraction_ratio * (inc + cos_t * n);
    vec3 r_para = -std::sqrt(fabs(1.0 - dot(r_perp, r_perp))) * n;
    return r_perp + r_para;
}

vec3 lerp(const vec3& u, const vec3& v, double t)
{
    return vec3(
        lerp(u.x, v.x, t),
        lerp(u.y, v.y, t),
        lerp(u.z, v.z, t));
}

vec3 random_unit_vec3()
{
    vec3 u = vec3(
        random_double(-1, 1),
        random_double(-1, 1),
        random_double(-1, 1));
    return u / u.magnetude();
}

vec3 random_unit_vec3_on_hemisphere(const vec3& norm)
{
    vec3 u = random_unit_vec3();
    if (dot(u, norm) < 0) {
        return -u;
    }
    return u;
}

vec3 random_on_unit_disk()
{
    double rand_r = sqrt(random_double());
    double rand_t = random_double(0, 2 * pi);

    return vec3(rand_r * cos(rand_t), rand_r * sin(rand_t), 0);
}

vec3 random_cos_dir()
{
    double r1 = random_double();
    double r2 = random_double();

    double phi = 2 * pi * r1;
    double x = cos(phi) * sqrt(r2);
    double y = sin(phi) * sqrt(r2);
    double z = sqrt(1 - r2);

    return vec3(x, y, z);
}

std::ostream& operator<<(std::ostream& out, vec3 u)
{
    out << u.x << ' ' << u.y << ' ' << u.z;
    return out;
}

vec3 min(const vec3& u, const vec3& v)
{
    return vec3(
        fmin(u.x, v.x),
        fmin(u.y, v.y),
        fmin(u.z, v.z));
}
vec3 max(const vec3& u, const vec3& v)
{
    return vec3(
        fmax(u.x, v.x),
        fmax(u.y, v.y),
        fmax(u.z, v.z));
}
