//
// Created by azuloo on 16.12.21.
//

#ifndef PLATFORMER_GAME_MATH_H
#define PLATFORMER_GAME_MATH_H

#include "../Types.h"

namespace lg {

/// -------------------------------------------------
inline lgVec2 abs(const lgVec2& v)
{
    return { fabs(v.x), fabs(v.y) };
}

/// -------------------------------------------------
inline lgMat2x2 abs(const lgMat2x2& mat)
{
    return { abs({ mat[0][0], mat[0][1] }), abs({ mat[1][0], mat[1][1] }) };
}

/// -------------------------------------------------
inline lgMat2x2 getRotationMat(lgFloat angle)
{
    lgFloat c = cosf(angle);
    lgFloat s = sinf(angle);

    return { c, s, -s, c };
}

/// -------------------------------------------------
inline float cross(const lgVec2& a, const lgVec2& b)
{
    return a.x * b.y - a.y * b.x;
}

/// -------------------------------------------------
inline lgVec2 cross(const lgVec2& a, lgFloat s)
{
    return { s * a.y, -s * a.x };
}

/// -------------------------------------------------
inline lgVec2 cross(float s, const lgVec2& a)
{
    return { -s * a.y, s * a.x };
}

/// -------------------------------------------------
inline float clamp(float a, float low, float high)
{
    return std::max(low, std::min(a, high));
}

}

#endif //PLATFORMER_GAME_MATH_H
