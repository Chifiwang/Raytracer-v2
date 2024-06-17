#ifndef VEC3_HPP
#define VEC3_HPP
#include "utils.hpp"
#include <ostream>

class vec3 {
    double m_data[3];

public:
    vec3()
    {
        m_data[0] = 0.0;
        m_data[1] = 0.0;
        m_data[2] = 0.0;
    }
    vec3(double x, double y, double z)
    {
        m_data[0] = x;
        m_data[1] = y;
        m_data[2] = z;
    }
    vec3(double value)
    {
        m_data[0] = value;
        m_data[1] = value;
        m_data[2] = value;
    }

    double x() const { return m_data[0]; }
    double y() const { return m_data[1]; }
    double z() const { return m_data[2]; }

    double& operator[](int i);
    double operator[](int i) const { return m_data[i]; }

    friend vec3 operator+(const vec3& u, const vec3& v);
    friend vec3 operator-(const vec3& u, const vec3& v);
    friend vec3 operator-(const vec3& u);
    friend vec3 operator*(double t, const vec3& u);
    friend vec3 operator*(const vec3& u, double t);
    friend vec3 operator/(const vec3& u, double t);

    vec3& operator+=(const vec3& u);
    vec3& operator-=(const vec3& u);
    vec3& operator*=(const double t);
    vec3& operator*=(const vec3& u);
    vec3& operator/=(const double t);

    double magnetude() const;
    bool near_zero() const;

    friend vec3 normalize(const vec3& u);
    friend double dot(const vec3& u, const vec3& v);
    friend vec3 cross(const vec3& u, const vec3& v);
    friend vec3 reflect(const vec3& u, const vec3& n);
    friend vec3 refract(const vec3& inc, const vec3& n, double refraction_ratio);
    friend vec3 lerp(const vec3& u, const vec3& v, double t);

    static vec3 random()
    {
        return vec3(random_double(), random_double(), random_double());
    }

    static vec3 random(double min, double max)
    {
        return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }
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

#endif
