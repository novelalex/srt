#ifndef GMATH_MATH_H_
#define GMATH_MATH_H_

#include <math.h>
#include "gmath_types.h"

static inline f32 gm_sqrt(f32 x) {
  return sqrtf(x);
}

static inline f32 gm_abs(f32 x) {
  return fabsf(x);
}

static inline bool gm_approx_eq(f32 a, f32 b) {
  return (gm_abs(a - b) <= NB_EPSILON_F);
}

static inline f32 gm_floor(f32 x) {
  return floorf(x);
}

static inline f32 gm_ceil(f32 x) {
  return ceilf(x);
}

static inline f32 gm_min(f32 a, f32 b) {
  return a < b ? a : b;
}

static inline f32 gm_max(f32 a, f32 b) {
  return a > b ? a : b;
}

static inline f32 gm_clamp(f32 x, f32 min, f32 max) {
  return gm_max(min, gm_min(x, max));
}

static inline f32 gm_sin(f32 x) {
  return sinf(x);
}

static inline f32 gm_cos(f32 x) {
  return cosf(x);
}

static inline f32 gm_acos(f32 x) {
  return acosf(x);
}

static inline f32 gm_deg2rad(f32 degrees) {
  return degrees * 0.017453293f;
}

#endif // GMATH_MATH_H_
