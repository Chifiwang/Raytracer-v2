#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "vec3.hpp"

class mat3x3 {
    vec3 m_basis[3];
public:
    mat3x3() {
        m_basis[0] = vec3(1, 0, 0);
        m_basis[1] = vec3(0, 1, 0);
        m_basis[2] = vec3(0, 0, 1);
    }

    mat3x3(const vec3& i, const vec3 &j, const vec3 &k) {
        m_basis[0] = i;
        m_basis[1] = j;
        m_basis[2] = k;
    }
};

#endif // !MATRIX_HPP
