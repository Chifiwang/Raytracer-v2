#ifndef VEC3_HPP
#define VEC3_HPP
#include "utils.hpp"
#include <ostream>

struct vec3 {
    double x, y, z;

    vec3()
    {
        x = 0.0;
        y = 0.0;
        z = 0.0;
    }
    vec3(double i, double j, double k)
    {
        x = i;
        y = j;
        z = k;
    }
    vec3(double value)
    {
        x = value;
        y = value;
        z = value;
    }

    vec3(double min, double max)
    {
        x = random_double(min, max);
        y = random_double(min, max);
        z = random_double(min, max);
    }

    double operator[](int t) const;
    double operator[](int t);
    vec3& operator+=(const vec3& u);
    vec3& operator-=(const vec3& u);
    vec3& operator*=(const double t);
    vec3& operator*=(const vec3& u);
    vec3& operator/=(const double t);

    double magnetude() const;
    bool near_zero() const;
};

using point3 = vec3;
using color = vec3;

vec3 operator+(const vec3& u, const vec3& v);
vec3 operator-(const vec3& u, const vec3& v);
vec3 operator-(const vec3& u);
vec3 operator*(double t, const vec3& u);
vec3 operator*(const vec3& u, double t);
vec3 operator/(const vec3& u, double t);
vec3 operator*(const vec3& u, const vec3& v);
vec3 operator/(const vec3& u, const vec3& v);

std::ostream& operator<<(std::ostream& out, vec3 u);

vec3 normalize(const vec3& u);
double dot(const vec3& u, const vec3& v);
vec3 cross(const vec3& u, const vec3& v);
vec3 reflect(const vec3& u, const vec3& v);
vec3 refract(const vec3& inc, const vec3& n, double refraction_ratio);
vec3 lerp(const vec3& u, const vec3& v, double t);

vec3 random_unit_vec3();
vec3 random_unit_vec3_on_hemisphere(const vec3& norm);
vec3 random_on_unit_disk();
vec3 random_cos_dir();

vec3 min(const vec3&u, const vec3&v);
vec3 max(const vec3&u, const vec3&v);

#endif
