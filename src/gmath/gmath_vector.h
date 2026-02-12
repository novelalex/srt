#ifndef GMATH_VECTOR_H_
#define GMATH_VECTOR_H_

#include "gmath_types.h"
#include "gmath_math.h"

// Vector3 Functions ---------------------------------------------

static inline Vector3 gm_vector3(f32 x, f32 y, f32 z) {
  return (Vector3){x, y, z, 0};
}

static inline Vector3 gm_splat3(f32 s) {
  return (Vector3){s, s, s, 0};
}

static inline Vector4 gm_withw3(Vector3 v, f32 w) {
  return (Vector4){v.x, v.y, v.z, w};
}

static inline Vector3 gm_zero3() {
  return (Vector3){0, 0, 0, 0};
}

static inline Vector3 gm_up3() {
  return (Vector3){0, 1, 0, 0};
}

static inline Vector3 gm_down3() {
  return (Vector3){0, -1, 0, 0};
}

static inline Vector3 gm_left3() {
  return (Vector3){-1, 0, 0, 0};
}

static inline Vector3 gm_right3() {
  return (Vector3){1, 0, 0, 0};
}

static inline Vector3 gm_forward3() {
  return (Vector3){0, 0, -1, 0};
}

static inline Vector3 gm_back3() {
  return (Vector3){0, 0, 1, 0};
}

static inline Vector3 gm_add3(Vector3 a, Vector3 b) {
    return (Vector3){
        a.x + b.x,
        a.y + b.y,
        a.z + b.z
    };
}

static inline Vector3 gm_sub3(Vector3 a, Vector3 b) {
    return (Vector3){
        a.x - b.x,
        a.y - b.y,
        a.z - b.z
    };
}

static inline Vector3 gm_negate3(Vector3 v) {
    return (Vector3){
        -v.x,
        -v.y,
        -v.z
    };
}

static inline Vector3 gm_lerp3(Vector3 a, Vector3 b, f32 t) {
  return (Vector3){
    a.x + t * (b.x - a.x),
    a.y + t * (b.y - a.y),
    a.z + t * (b.z - a.z)
  };
}

static inline Vector3 gm_element_mult3(Vector3 a, Vector3 b) {
    return (Vector3){
        a.x * b.x,
        a.y * b.y,
        a.z * b.z
    };
}

static inline Vector3 gm_mult3(Vector3 v, f32 s) {
    return (Vector3){
        v.x * s,
        v.y * s,
        v.z * s
    };
}

static inline Vector3 gm_div3(Vector3 v, f32 s) {
    return (Vector3){
        v.x / s,
        v.y / s,
        v.z / s
    };
}

static inline f32 gm_dot3(Vector3 a, Vector3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline Vector3 gm_cross3(Vector3 a, Vector3 b) {
    return (Vector3){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

static inline f32 gm_mag_squared3(Vector3 v) {
    return gm_dot3(v, v);
}

static inline f32 gm_mag3(Vector3 v) {
    return gm_sqrt(gm_mag_squared3(v));
}

static inline f32 gm_dist_squared3(Vector3 a, Vector3 b) {
    return gm_mag_squared3(gm_sub3(a, b));
}

static inline f32 gm_dist3(Vector3 a, Vector3 b) {
    return gm_mag3(gm_sub3(a, b));
}

static inline Vector3 gm_from3(Vector3 a, Vector3 b) {
    return gm_sub3(b, a);
}

static inline Vector3 gm_normalize3(Vector3 v) {
    f32 mag = gm_mag3(v);
    NB_ASSERT_MSG(mag > 1e-6, "Tried to normalize vector with magnitude too close to zero");
    return mag > 0.0f ? gm_div3(v, mag) : gm_zero3();
}

static inline Vector3 gm_reflect3(Vector3 v, Vector3 n) {
    return gm_sub3(v, gm_mult3(n, 2.0f * gm_dot3(v, n)));
}

static inline Vector3 gm_scale_add3(Vector3 a, Vector3 b, f32 s) {
    return gm_add3(a, gm_mult3(b, s));
}

static inline Vector3 gm_project3(Vector3 a, Vector3 b) {
    f32 b_mag_sq = gm_mag_squared3(b);
    if (b_mag_sq < 1e-6f) {
        return gm_zero3();
    }
    f32 scale = gm_dot3(a, b) / b_mag_sq;
    return gm_mult3(b, scale);
}

static inline bool gm_equal3(Vector3 a, Vector3 b, f32 epsilon) {
    return gm_abs(a.x - b.x) <= epsilon &&
           gm_abs(a.y - b.y) <= epsilon &&
           gm_abs(a.z - b.z) <= epsilon;
}

// Vector4 functions ------------------------------------------------

static inline Vector4 gm_vector4(f32 x, f32 y, f32 z, f32 w) {
  return (Vector4){x, y, z, w};
}

static inline Vector4 gm_splat4(f32 s) {
  return (Vector4){s, s, s, s};
}

static inline Vector3 gm_dropw4(Vector4 v) {
    return (Vector3){v.x, v.y, v.z, 0};
}

static inline Vector4 gm_zero4() {
  return (Vector4){0, 0, 0, 0};
}

static inline Vector4 gm_add4(Vector4 a, Vector4 b) {
    return (Vector4){
        a.x + b.x,
        a.y + b.y,
        a.z + b.z,
        a.w + b.w
    };
}

static inline Vector4 gm_sub4(Vector4 a, Vector4 b) {
    return (Vector4){
        a.x - b.x,
        a.y - b.y,
        a.z - b.z,
        a.w - b.w
    };
}

static inline Vector4 gm_negate4(Vector4 v) {
    return (Vector4){
        -v.x,
        -v.y,
        -v.z,
        -v.w
    };
}

static inline Vector4 gm_lerp4(Vector4 a, Vector4 b, f32 t) {
  return (Vector4){
    a.x + t * (b.x - a.x),
    a.y + t * (b.y - a.y),
    a.z + t * (b.z - a.z),
    a.w + t * (b.w - a.w)
  };
}

static inline Vector4 gm_element_mult4(Vector4 a, Vector4 b) {
    return (Vector4){
        a.x * b.x,
        a.y * b.y,
        a.z * b.z,
        a.w * b.w
    };
}

static inline Vector4 gm_mult4(Vector4 v, f32 s) {
    return (Vector4){
        v.x * s,
        v.y * s,
        v.z * s,
        v.w * s
    };
}

static inline Vector4 gm_div4(Vector4 v, f32 s) {
    return (Vector4){
        v.x / s,
        v.y / s,
        v.z / s,
        v.w / s
    };
}

static inline f32 gm_dot4(Vector4 a, Vector4 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

static inline Vector4 gm_cross4(Vector4 a, Vector4 b) {
    Vector3 a3 = gm_dropw4(a);
    Vector3 b3 = gm_dropw4(b);
    Vector3 r3 = gm_cross3(a3, b3);
    return gm_withw3(r3, 0.0f);
}

static inline f32 gm_mag_squared4(Vector4 v) {
    return gm_dot4(v, v);
}

static inline f32 gm_mag4(Vector4 v) {
    return gm_sqrt(gm_mag_squared4(v));
}

static inline f32 gm_dist_squared4(Vector4 a, Vector4 b) {
    return gm_mag_squared4(gm_sub4(a, b));
}

static inline f32 gm_dist4(Vector4 a, Vector4 b) {
    return gm_mag4(gm_sub4(a, b));
}

static inline Vector4 gm_from4(Vector4 a, Vector4 b) {
    return gm_sub4(b, a);
}

static inline Vector4 gm_normalize4(Vector4 v) {
    f32 mag = gm_mag4(v);
    NB_ASSERT_MSG(mag > 1e-6, "Tried to normalize vector with magnitude too close to zero");
    return mag > 0.0f ? gm_div4(v, mag) : gm_zero4();
}

static inline Vector4 gm_scale_add4(Vector4 a, Vector4 b, f32 s) {
    return gm_add4(a, gm_mult4(b, s));
}

static inline Vector4 gm_project4(Vector4 a, Vector4 b) {
    f32 b_mag_sq = gm_mag_squared4(b);
    if (b_mag_sq < 1e-6f) {
        return gm_zero4();
    }
    f32 scale = gm_dot4(a, b) / b_mag_sq;
    return gm_mult4(b, scale);
}

static inline bool gm_equal4(Vector4 a, Vector4 b, f32 epsilon) {
    return gm_abs(a.x - b.x) <= epsilon &&
           gm_abs(a.y - b.y) <= epsilon &&
           gm_abs(a.z - b.z) <= epsilon &&
           gm_abs(a.w - b.w) <= epsilon;
}


#endif // GMATH_VECTOR_H_