
#ifdef __cplusplus
#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include <algorithm>
#include "utility.hpp"


namespace glm::hs
{
    using std::clamp;
#else
#define _glsl
#endif


#ifdef _glsl
#define inline
#endif

    inline float safe_acos(float v)
    {
        return acos(clamp(v, -1.0f, 1.0f));
    }


    inline float pi()
    {
        return radians(180.0f);
    }

    // TODO: class 3: define where stuff needs to be normalized and minimize use of normalize()

    inline vec4 cross3(const vec4 a, const vec4 b, const vec4 c)
    {
        vec4 ret;
        ret.x = (a.w * b.y * c.z - a.w * b.z * c.y - a.y * b.w * c.z + a.y * b.z * c.w + a.z * b.w * c.y - a.z * b.y * c.w) /
                (a.w * b.x * c.y - a.w * b.y * c.x - a.x * b.w * c.y + a.x * b.y * c.w + a.y * b.w * c.x - a.y * b.x * c.w
                 - a.w * b.x * c.z + a.w * b.z * c.x + a.x * b.w * c.z - a.x * b.z * c.w - a.z * b.w * c.x + a.z * b.x * c.w
                 + a.w * b.y * c.z - a.w * b.z * c.y - a.y * b.w * c.z + a.y * b.z * c.w + a.z * b.w * c.y - a.z * b.y * c.w
                 - a.x * b.y * c.z + a.x * b.z * c.y + a.y * b.x * c.z - a.y * b.z * c.x - a.z * b.x * c.y + a.z * b.y * c.x);
        ret.y = -(a.w * b.x * c.z - a.w * b.z * c.x - a.x * b.w * c.z + a.x * b.z * c.w + a.z * b.w * c.x - a.z * b.x * c.w) /
                (a.w * b.x * c.y - a.w * b.y * c.x - a.x * b.w * c.y + a.x * b.y * c.w + a.y * b.w * c.x - a.y * b.x * c.w
                 - a.w * b.x * c.z + a.w * b.z * c.x + a.x * b.w * c.z - a.x * b.z * c.w - a.z * b.w * c.x + a.z * b.x * c.w
                 + a.w * b.y * c.z - a.w * b.z * c.y - a.y * b.w * c.z + a.y * b.z * c.w + a.z * b.w * c.y - a.z * b.y * c.w
                 - a.x * b.y * c.z + a.x * b.z * c.y + a.y * b.x * c.z - a.y * b.z * c.x - a.z * b.x * c.y + a.z * b.y * c.x);
        ret.z = (a.w * b.x * c.y - a.w * b.y * c.x - a.x * b.w * c.y + a.x * b.y * c.w + a.y * b.w * c.x - a.y * b.x * c.w) /
                (a.w * b.x * c.y - a.w * b.y * c.x - a.x * b.w * c.y + a.x * b.y * c.w + a.y * b.w * c.x - a.y * b.x * c.w
                 - a.w * b.x * c.z + a.w * b.z * c.x + a.x * b.w * c.z - a.x * b.z * c.w - a.z * b.w * c.x + a.z * b.x * c.w
                 + a.w * b.y * c.z - a.w * b.z * c.y - a.y * b.w * c.z + a.y * b.z * c.w + a.z * b.w * c.y - a.z * b.y * c.w
                 - a.x * b.y * c.z + a.x * b.z * c.y + a.y * b.x * c.z - a.y * b.z * c.x - a.z * b.x * c.y + a.z * b.y * c.x);
        ret.w = -(a.x * b.y * c.z - a.x * b.z * c.y - a.y * b.x * c.z + a.y * b.z * c.x + a.z * b.x * c.y - a.z * b.y * c.x) /
                (a.w * b.x * c.y - a.w * b.y * c.x - a.x * b.w * c.y + a.x * b.y * c.w + a.y * b.w * c.x - a.y * b.x * c.w
                 - a.w * b.x * c.z + a.w * b.z * c.x + a.x * b.w * c.z - a.x * b.z * c.w - a.z * b.w * c.x + a.z * b.x * c.w
                 + a.w * b.y * c.z - a.w * b.z * c.y - a.y * b.w * c.z + a.y * b.z * c.w + a.z * b.w * c.y - a.z * b.y * c.w
                 - a.x * b.y * c.z + a.x * b.z * c.y + a.y * b.x * c.z - a.y * b.z * c.x - a.z * b.x * c.y + a.z * b.y * c.x);

        ret = normalize(ret);
        return ret;
    }

    //finds plane that is orthogonal to t and v
    inline mat2x4 orthogonalPlane(const vec4 t, const vec4 v)
    {
        mat2x4 ret;
        ret[0].x = -(t.w * v.y - t.y * v.w + t.y * v.z - t.z * v.y) /
                   (t.w * v.x - t.x * v.w - t.w * v.y + t.y * v.w + t.x * v.z - t.z * v.x - t.y * v.z + t.z * v.y);
        ret[0].y = (t.w * v.x - t.x * v.w + t.x * v.z - t.z * v.x) /
                   (t.w * v.x - t.x * v.w - t.w * v.y + t.y * v.w + t.x * v.z - t.z * v.x - t.y * v.z + t.z * v.y);
        ret[0].z = -(t.x * v.y - t.y * v.x) / (t.w * v.x - t.x * v.w - t.w * v.y + t.y * v.w + t.x * v.z - t.z * v.x - t.y * v.z + t.z * v.y);
        ret[0].w = (t.x * v.y - t.y * v.x) / (t.w * v.x - t.x * v.w - t.w * v.y + t.y * v.w + t.x * v.z - t.z * v.x - t.y * v.z + t.z * v.y);
        float sigma_1 = t.x * t.z * v.y * v.y;
        float sigma_2 = t.x * t.z * v.w * v.w;
        ret[1].x = -(v.x * t.w * t.w * v.z - v.x * t.w * t.y * v.y - v.x * t.w * t.z * v.w - v.x * t.w * t.z * v.z - t.x * t.w * v.w * v.z +
                     t.x * t.w * v.y * v.y + t.x * t.w * v.z * v.z + v.x * t.y * t.y * v.w + v.x * t.y * t.y * v.z - v.x * t.y * t.z * v.y -
                     t.x * t.y * v.w * v.y - t.x * t.y * v.y * v.z + v.x * t.z * t.z * v.w + sigma_2 - t.x * t.z * v.w * v.z + sigma_1) /
                   (t.w * t.w * v.x * v.x - t.w * t.w * v.x * v.z + t.w * t.w * v.y * v.y - t.w * t.w * v.y * v.z - 2 * t.w * t.x * v.w * v.x +
                    t.w * t.x * v.w * v.z + t.w * t.x * v.x * v.y + 2 * t.w * t.x * v.x * v.z - t.w * t.x * v.y * v.y - t.w * t.x * v.z * v.z -
                    2 * t.w * t.y * v.w * v.y + t.w * t.y * v.w * v.z - t.w * t.y * v.x * v.x + t.w * t.y * v.x * v.y + 2 * t.w * t.y * v.y * v.z -
                    t.w * t.y * v.z * v.z + t.w * t.z * v.w * v.x + t.w * t.z * v.w * v.y - 2 * t.w * t.z * v.x * v.x + t.w * t.z * v.x * v.z -
                    2 * t.w * t.z * v.y * v.y + t.w * t.z * v.y * v.z + t.x * t.x * v.w * v.w - t.x * t.x * v.w * v.y - 2 * t.x * t.x * v.w * v.z +
                    2 * t.x * t.x * v.y * v.y - t.x * t.x * v.y * v.z + t.x * t.x * v.z * v.z + t.x * t.y * v.w * v.x + t.x * t.y * v.w * v.y -
                    4 * t.x * t.y * v.x * v.y + t.x * t.y * v.x * v.z + t.x * t.y * v.y * v.z - sigma_2 + 2 * t.x * t.z * v.w * v.x +
                    t.x * t.z * v.w * v.z + t.x * t.z * v.x * v.y - 2 * t.x * t.z * v.x * v.z - sigma_1 + t.y * t.y * v.w * v.w -
                    t.y * t.y * v.w * v.x - 2 * t.y * t.y * v.w * v.z + 2 * t.y * t.y * v.x * v.x - t.y * t.y * v.x * v.z + t.y * t.y * v.z * v.z -
                    t.y * t.z * v.w * v.w + 2 * t.y * t.z * v.w * v.y + t.y * t.z * v.w * v.z - t.y * t.z * v.x * v.x + t.y * t.z * v.x * v.y -
                    2 * t.y * t.z * v.y * v.z - t.z * t.z * v.w * v.x - t.z * t.z * v.w * v.y + t.z * t.z * v.x * v.x + t.z * t.z * v.y * v.y);
        float sigma_3 = t.y * t.z * v.x * v.x;
        float sigma_4 = t.y * t.z * v.w * v.w;
        ret[1].y = -(v.y * t.w * t.w * v.z - v.y * t.w * t.x * v.x - v.y * t.w * t.z * v.w - v.y * t.w * t.z * v.z - t.y * t.w * v.w * v.z +
                     t.y * t.w * v.x * v.x + t.y * t.w * v.z * v.z + v.y * t.x * t.x * v.w + v.y * t.x * t.x * v.z - v.y * t.x * t.z * v.x -
                     t.y * t.x * v.w * v.x - t.y * t.x * v.x * v.z + v.y * t.z * t.z * v.w + sigma_4 - t.y * t.z * v.w * v.z + sigma_3) /
                   (t.w * t.w * v.x * v.x - t.w * t.w * v.x * v.z + t.w * t.w * v.y * v.y - t.w * t.w * v.y * v.z - 2 * t.w * t.x * v.w * v.x +
                    t.w * t.x * v.w * v.z + t.w * t.x * v.x * v.y + 2 * t.w * t.x * v.x * v.z - t.w * t.x * v.y * v.y - t.w * t.x * v.z * v.z -
                    2 * t.w * t.y * v.w * v.y + t.w * t.y * v.w * v.z - t.w * t.y * v.x * v.x + t.w * t.y * v.x * v.y + 2 * t.w * t.y * v.y * v.z -
                    t.w * t.y * v.z * v.z + t.w * t.z * v.w * v.x + t.w * t.z * v.w * v.y - 2 * t.w * t.z * v.x * v.x + t.w * t.z * v.x * v.z -
                    2 * t.w * t.z * v.y * v.y + t.w * t.z * v.y * v.z + t.x * t.x * v.w * v.w - t.x * t.x * v.w * v.y - 2 * t.x * t.x * v.w * v.z +
                    2 * t.x * t.x * v.y * v.y - t.x * t.x * v.y * v.z + t.x * t.x * v.z * v.z + t.x * t.y * v.w * v.x + t.x * t.y * v.w * v.y -
                    4 * t.x * t.y * v.x * v.y + t.x * t.y * v.x * v.z + t.x * t.y * v.y * v.z - t.x * t.z * v.w * v.w + 2 * t.x * t.z * v.w * v.x +
                    t.x * t.z * v.w * v.z + t.x * t.z * v.x * v.y - 2 * t.x * t.z * v.x * v.z - t.x * t.z * v.y * v.y + t.y * t.y * v.w * v.w -
                    t.y * t.y * v.w * v.x - 2 * t.y * t.y * v.w * v.z + 2 * t.y * t.y * v.x * v.x - t.y * t.y * v.x * v.z + t.y * t.y * v.z * v.z -
                    sigma_4 + 2 * t.y * t.z * v.w * v.y + t.y * t.z * v.w * v.z - sigma_3 + t.y * t.z * v.x * v.y - 2 * t.y * t.z * v.y * v.z -
                    t.z * t.z * v.w * v.x - t.z * t.z * v.w * v.y + t.z * t.z * v.x * v.x + t.z * t.z * v.y * v.y);
        float sigma_5 = t.y * t.y * v.w * v.w;
        float sigma_6 = t.w * t.w * v.y * v.y;
        float sigma_7 = t.x * t.x * v.w * v.w;
        float sigma_8 = t.w * t.w * v.x * v.x;
        float sigma_9 = 2 * t.w * t.y * v.w * v.y;
        float sigma_10 = 2 * t.w * t.x * v.w * v.x;
        ret[1].z =
            (sigma_8 + sigma_6 - sigma_10 + v.z * t.w * t.x * v.x - sigma_9 + v.z * t.w * t.y * v.y - t.z * t.w * v.x * v.x - t.z * t.w * v.y * v.y +
             sigma_7 - v.z * t.x * t.x * v.w + t.x * t.x * v.y * v.y - 2 * t.x * t.y * v.x * v.y + t.z * t.x * v.w * v.x + sigma_5 -
             v.z * t.y * t.y * v.w + t.y * t.y * v.x * v.x + t.z * t.y * v.w * v.y) /
            (sigma_8 - t.w * t.w * v.x * v.z + sigma_6 - t.w * t.w * v.y * v.z - sigma_10 + t.w * t.x * v.w * v.z + t.w * t.x * v.x * v.y +
             2 * t.w * t.x * v.x * v.z - t.w * t.x * v.y * v.y - t.w * t.x * v.z * v.z - sigma_9 + t.w * t.y * v.w * v.z - t.w * t.y * v.x * v.x +
             t.w * t.y * v.x * v.y + 2 * t.w * t.y * v.y * v.z - t.w * t.y * v.z * v.z + t.w * t.z * v.w * v.x + t.w * t.z * v.w * v.y -
             2 * t.w * t.z * v.x * v.x + t.w * t.z * v.x * v.z - 2 * t.w * t.z * v.y * v.y + t.w * t.z * v.y * v.z + sigma_7 - t.x * t.x * v.w * v.y -
             2 * t.x * t.x * v.w * v.z + 2 * t.x * t.x * v.y * v.y - t.x * t.x * v.y * v.z + t.x * t.x * v.z * v.z + t.x * t.y * v.w * v.x +
             t.x * t.y * v.w * v.y - 4 * t.x * t.y * v.x * v.y + t.x * t.y * v.x * v.z + t.x * t.y * v.y * v.z - t.x * t.z * v.w * v.w +
             2 * t.x * t.z * v.w * v.x + t.x * t.z * v.w * v.z + t.x * t.z * v.x * v.y - 2 * t.x * t.z * v.x * v.z - t.x * t.z * v.y * v.y + sigma_5 -
             t.y * t.y * v.w * v.x - 2 * t.y * t.y * v.w * v.z + 2 * t.y * t.y * v.x * v.x - t.y * t.y * v.x * v.z + t.y * t.y * v.z * v.z -
             t.y * t.z * v.w * v.w + 2 * t.y * t.z * v.w * v.y + t.y * t.z * v.w * v.z - t.y * t.z * v.x * v.x + t.y * t.z * v.x * v.y -
             2 * t.y * t.z * v.y * v.z - t.z * t.z * v.w * v.x - t.z * t.z * v.w * v.y + t.z * t.z * v.x * v.x + t.z * t.z * v.y * v.y);
        float sigma_11 = t.z * t.z * v.y * v.y;
        float sigma_12 = t.y * t.y * v.z * v.z;
        float sigma_13 = t.z * t.z * v.x * v.x;
        float sigma_14 = t.x * t.x * v.z * v.z;
        float sigma_15 = 2 * t.y * t.z * v.y * v.z;
        float sigma_16 = 2 * t.x * t.z * v.x * v.z;
        ret[1].w = (t.x * t.x * v.y * v.y + sigma_14 - v.w * t.x * t.x * v.z - 2 * t.x * t.y * v.x * v.y - sigma_16 + v.w * t.x * t.z * v.x +
                    t.w * t.x * v.x * v.z + t.y * t.y * v.x * v.x + sigma_12 - v.w * t.y * t.y * v.z - sigma_15 + v.w * t.y * t.z * v.y +
                    t.w * t.y * v.y * v.z + sigma_13 + sigma_11 - t.w * t.z * v.x * v.x - t.w * t.z * v.y * v.y) /
                   (t.w * t.w * v.x * v.x - t.w * t.w * v.x * v.z + t.w * t.w * v.y * v.y - t.w * t.w * v.y * v.z - 2 * t.w * t.x * v.w * v.x +
                    t.w * t.x * v.w * v.z + t.w * t.x * v.x * v.y + 2 * t.w * t.x * v.x * v.z - t.w * t.x * v.y * v.y - t.w * t.x * v.z * v.z -
                    2 * t.w * t.y * v.w * v.y + t.w * t.y * v.w * v.z - t.w * t.y * v.x * v.x + t.w * t.y * v.x * v.y + 2 * t.w * t.y * v.y * v.z -
                    t.w * t.y * v.z * v.z + t.w * t.z * v.w * v.x + t.w * t.z * v.w * v.y - 2 * t.w * t.z * v.x * v.x + t.w * t.z * v.x * v.z -
                    2 * t.w * t.z * v.y * v.y + t.w * t.z * v.y * v.z + t.x * t.x * v.w * v.w - t.x * t.x * v.w * v.y - 2 * t.x * t.x * v.w * v.z +
                    2 * t.x * t.x * v.y * v.y - t.x * t.x * v.y * v.z + sigma_14 + t.x * t.y * v.w * v.x + t.x * t.y * v.w * v.y -
                    4 * t.x * t.y * v.x * v.y + t.x * t.y * v.x * v.z + t.x * t.y * v.y * v.z - t.x * t.z * v.w * v.w + 2 * t.x * t.z * v.w * v.x +
                    t.x * t.z * v.w * v.z + t.x * t.z * v.x * v.y - sigma_16 - t.x * t.z * v.y * v.y + t.y * t.y * v.w * v.w - t.y * t.y * v.w * v.x -
                    2 * t.y * t.y * v.w * v.z + 2 * t.y * t.y * v.x * v.x - t.y * t.y * v.x * v.z + sigma_12 - t.y * t.z * v.w * v.w +
                    2 * t.y * t.z * v.w * v.y + t.y * t.z * v.w * v.z - t.y * t.z * v.x * v.x + t.y * t.z * v.x * v.y - sigma_15 -
                    t.z * t.z * v.w * v.x - t.z * t.z * v.w * v.y + sigma_13 + sigma_11);

        return ret;
    }

    // finds plane that lies in t and v
    inline mat2x4 plane(const vec4 t, const vec4 v)
    {
        mat2x4 ret;
        ret[0] = v;
        ret[1].x = (t.x * v.w * v.w - t.w * v.x * v.w + t.x * v.y * v.y - t.y * v.x * v.y + t.x * v.z * v.z - t.z * v.x * v.z) /
                   (v.w * v.w + v.x * v.x + v.y * v.y + v.z * v.z);
        ret[1].y = (t.y * v.w * v.w - t.w * v.y * v.w + t.y * v.x * v.x - t.x * v.y * v.x + t.y * v.z * v.z - t.z * v.y * v.z) /
                   (v.w * v.w + v.x * v.x + v.y * v.y + v.z * v.z);
        ret[1].z = (t.z * v.w * v.w - t.w * v.z * v.w + t.z * v.x * v.x - t.x * v.z * v.x + t.z * v.y * v.y - t.y * v.z * v.y) /
                   (v.w * v.w + v.x * v.x + v.y * v.y + v.z * v.z);
        ret[1].w = (t.w * v.x * v.x - t.x * v.w * v.x + t.w * v.y * v.y - t.y * v.w * v.y + t.w * v.z * v.z - t.z * v.w * v.z) /
                   (v.w * v.w + v.x * v.x + v.y * v.y + v.z * v.z);
        ret[0] = normalize(ret[0]);
        ret[1] = normalize(ret[1]);
        return ret;
    }

    inline mat4 rotate(mat2x4 p, const float alpha)
    {
        p[0] = normalize(p[0]);
        p[1] = normalize(p[1]);
        mat4 v = outerProduct(p[0], p[0]) + outerProduct(p[1], p[1]);
        mat4 w = outerProduct(p[0], p[1]) - outerProduct(p[1], p[0]);
        return mat4(1.0f) + (cos(alpha) - 1.0f) * v - sin(alpha) * w;
    }

    inline float distanceOnHypersphere(const float radius, const float angle)
    {
        return (2 * pi() * radius) * (angle / radians(360.0f));
    }

    inline float distanceOnHypersphere(const vec4 a_coord, const vec4 b_coord, const float radius)
    {
        float angle = safe_acos(dot(normalize(a_coord), normalize(b_coord)));
        return distanceOnHypersphere(radius, angle);
    }

    inline float angleDistance(const float radius, const float length)
    {
        return radians(360.0f) * (length / (2 * pi() * radius));
    }

    inline const mat4 origin_hypersphere_orientation = mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    );

    inline mat4 getHypersphereOrientation(
        const mat4 from_hypersphere_orientation,
        const vec4 to_hypersphere_coord
    )
    {
        mat4 rotation = mat4(1);
        if (from_hypersphere_orientation[3] != to_hypersphere_coord)
        {
            rotation = rotate(
                plane(
                    to_hypersphere_coord,
                    from_hypersphere_orientation[3]
                ),
                safe_acos(dot(normalize(from_hypersphere_orientation[3]), normalize(to_hypersphere_coord)))
            );
        }
        return mat4(
            normalize(rotation * from_hypersphere_orientation[0]),
            normalize(rotation * from_hypersphere_orientation[1]),
            normalize(rotation * from_hypersphere_orientation[2]),
            normalize(to_hypersphere_coord)
        );
    }

    inline mat4 get3DOffsetRotation(
        const mat4 hypersphere_orientation,
        const mat3 local_orientation_rotation,
        const vec3 offset_3_d, const float radius
    )
    {
        vec4 rotated_vertex_model_position = hypersphere_orientation * vec4(local_orientation_rotation * offset_3_d, 0);

        mat2x4 rotation_plane = plane(hypersphere_orientation[3] + rotated_vertex_model_position, hypersphere_orientation[3]);
        float angle = angleDistance(radius, length(rotated_vertex_model_position));
        mat4 rotation = rotate(rotation_plane, angle);
        if (length(rotated_vertex_model_position) == 0.0)
        {
            rotation = mat4(1.0);
        }
        return rotation;
    }

    inline vec4 getCoord(
        const mat4 hypersphere_orientation,
        const mat3 local_orientation_rotation,
        const vec3 offset_3_d, const float radius
    )
    {
        return get3DOffsetRotation(
            hypersphere_orientation, local_orientation_rotation, offset_3_d, radius
        ) * hypersphere_orientation[3];
    }

    inline mat4 getHypersphereOrientation(
        const mat4 hypersphere_orientation,
        const mat3 local_orientation_rotation,
        const vec3 offset_3_d, const float radius
    )
    {
        return get3DOffsetRotation(
            hypersphere_orientation, local_orientation_rotation, offset_3_d, radius
        ) * hypersphere_orientation;
    }

    inline vec4 getHypersphereCoordinate(const vec3 position_3_d, const float radius)
    {
        return normalize(getCoord(
            origin_hypersphere_orientation,
            mat3(1.0),
            position_3_d, radius
        ));
    }

    inline vec4 projection(const vec4 from, const vec4 onto)
    {
        float length = dot(from, normalize(onto));
        vec4 projection = normalize(onto) * length;
        return projection;
    }

    inline vec4 getLocalDirectionalVector(const vec4 from_coord, const vec4 to_coord)
    {
        mat2x4 axis = orthogonalPlane(from_coord, to_coord);
        vec4 local_direction = normalize(cross3(axis[0], axis[1], from_coord));
        float angle = safe_acos(dot(local_direction, normalize(to_coord - from_coord)));
        if (radians(90.0f) < angle && angle <= radians(180.0f))
        {
            local_direction = -local_direction;
        }
        return local_direction;
    }

    inline vec3 getViewAngles(
        const mat3 local_camera_orientation,
        const mat4 camera_hypersphere_orientation,
        const vec4 object_coord,
        const float radius
    )
    {
        vec4 camera_right = normalize(camera_hypersphere_orientation * vec4(local_camera_orientation * vec3(1.0, 0.0, 0.0), 0.0));
        vec4 camera_up = normalize(camera_hypersphere_orientation * vec4(local_camera_orientation * vec3(0.0, 1.0, 0.0), 0.0));
        vec4 camera_z = normalize(camera_hypersphere_orientation * vec4(local_camera_orientation * vec3(0.0, 0.0, 1.0), 0.0));

        vec4 view_vector = getLocalDirectionalVector(camera_hypersphere_orientation[3], object_coord);
        vec4 proj_right = projection(view_vector, camera_right);
        vec4 proj_up = projection(view_vector, camera_up);
        vec4 proj_z = projection(view_vector, camera_z);

        float horizontal_angle = safe_acos(dot(normalize(camera_z), normalize(proj_right + proj_z)));
        if (dot(normalize(camera_right), normalize(proj_right)) < 0.0f)
            // if proj_right looks in the opposite direction as camera orientation_right
        {
            horizontal_angle = -horizontal_angle;
        }

        float vertical_angle = safe_acos(dot(normalize(camera_z), normalize(proj_up + proj_z)));
        if (dot(normalize(camera_up), normalize(proj_up)) < 0.0f)
            // if proj_up looks in the opposite direction as camera orientation_up
        {
            vertical_angle = -vertical_angle;
        }


        float object_view_distance = distanceOnHypersphere(camera_hypersphere_orientation[3], object_coord, radius);
        if (dot(normalize(camera_z), normalize(proj_z)) < 0.0f)
            // if view vector is in the opposite direction as the camera look at vector
        {
            object_view_distance = 2 * pi() * radius - object_view_distance;
            horizontal_angle = -horizontal_angle;
            vertical_angle = -vertical_angle;

            if (horizontal_angle < 0.0f)
            {
                horizontal_angle = -radians(180.0f) - horizontal_angle;
            }
            else
            {
                horizontal_angle = radians(180.0f) - horizontal_angle;
            }
            if (vertical_angle < 0.0f)
            {
                vertical_angle = -radians(180.0f) - vertical_angle;
            }
            else
            {
                vertical_angle = radians(180.0f) - vertical_angle;
            }
        }

        return vec3(horizontal_angle, vertical_angle, object_view_distance);
    }

    inline vec3 getViewSpaceCoords(const vec3 view_angles, const float field_of_view, const float aspect_ratio, const float far_plane)
    {
        float vertical_view_angle = field_of_view / 2;
        float horizontal_view_angle = aspect_ratio * field_of_view / 2;
        vec3 view_space_coord = vec3(
            -view_angles.x / horizontal_view_angle,
            view_angles.y / vertical_view_angle,
            view_angles.z / far_plane
        );
        if (view_space_coord != view_space_coord)
        {
#ifdef __cplusplus
            std::cout << "cpu nan values: " << utility::toString(view_space_coord) << std::endl;
#else
            printf("gpu nan values: %.1^3f\n", view_space_coord);
#endif
        }
        //view_space_coord.z = ((1.0/(1.0-0.01))*(1.0 - 0.01/view_space_coord.z));
        return view_space_coord;
    }

#ifdef __cplusplus
}//namespace hs
#endif
