#ifndef GMATH_TYPES_H_
#define GMATH_TYPES_H_

#include "../common.h"

typedef struct Vector3 {
    f32 x;
    f32 y;
    f32 z;
    f32 _pad;
} Vector3;

typedef struct Vector4 {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} Vector4;

typedef struct Quaternion {
    f32 w;
    f32 i;
    f32 j;
    f32 k;
} Quaternion;

// Matrix is column major
// 0  4  8  12
// 1  5  9  13
// 2  6  10 14
// 3  7  11 15
typedef struct Matrix4 {
    f32 m[16];
} Matrix4;

typedef struct QuadraticSolution {
    usize count;
    f32 solutions[3];
} QuadraticSolution;

typedef struct Ray {
    Vector3 origin;
    Vector3 direction;
} Ray;

typedef struct Sphere {
    f32         radius;
    f32         center;
    Quaternion  orientation;
    Matrix4     transform;
} Sphere;

typedef struct Color {
    f32 r;
    f32 g;
    f32 b;
    f32 a;
} Color;

#endif // GMATH_TYPES_H_