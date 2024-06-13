#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "vec3.hpp"

class Texture {
public:
    virtual ~Texture() = default;
    virtual color sample(const point3& p) const = 0;
};

class Solid : public Texture {
    color attenuation;

public:
    Solid(const color& attenuation)
        : attenuation(attenuation)
    {
    }
    Solid(double r, double g, double b)
        : attenuation(color(r, g, b))
    {
    }
    Solid(double value)
        : attenuation(color(value))
    {
    }
    color sample(const point3& p) const;
};

class Gradient : public Texture {
    color primary;
    color secondary;

public:
    Gradient(const color& primary, const color& secondary)
        : primary(primary)
        , secondary(secondary)
    {
    }
    color sample(const point3& p) const;
};

#endif // !TEXTURE_HPP
