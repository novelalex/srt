#ifndef GMATH_H_
#define GMATH_H_

#include "../common.h"

#include "gmath_types.h"

// Mathf fucntions --------------------------------------------------

static inline f32 gm_sqrt(f32 x);
static inline f32 gm_abs(f32 x);
static inline f32 gm_floor(f32 x);
static inline f32 gm_ceil(f32 x);
static inline f32 gm_min(f32 a, f32 b);
static inline f32 gm_max(f32 a, f32 b);
static inline f32 gm_clamp(f32 x, f32 min, f32 max);
static inline f32 gm_sin(f32 x);
static inline f32 gm_cos(f32 x);
static inline f32 gm_acos(f32 x);


// Vector3 ------------------------------------------------

static inline Vector3    gm_vector3(f32 x, f32 y, f32 z);
static inline Vector3    gm_splat3(f32 s);
static inline Vector4    gm_withw3(Vector3 v, f32 w);

static inline Vector3    gm_zero3();
static inline Vector3    gm_up3();
static inline Vector3    gm_down3();
static inline Vector3    gm_left3();
static inline Vector3    gm_right3();
static inline Vector3    gm_forward3();
static inline Vector3    gm_back3();

static inline Vector3    gm_add3(Vector3 a, Vector3 b);
static inline Vector3    gm_sub3(Vector3 a, Vector3 b);
static inline Vector3    gm_negate3(Vector3 v);
static inline Vector3    gm_lerp3(Vector3 a, Vector3 b, f32 t);
static inline Vector3    gm_element_mult3(Vector3 a, Vector3 b);
static inline Vector3    gm_mult3(Vector3 v, f32 s);
static inline Vector3    gm_div3(Vector3 v, f32 s);
static inline f32        gm_dot3(Vector3 a, Vector3 b);
static inline Vector3    gm_cross3(Vector3 a, Vector3 b);
static inline f32        gm_mag_squared3(Vector3 v);
static inline f32        gm_mag3(Vector3 v);
static inline f32        gm_dist_squared3(Vector3 a, Vector3 b);
static inline f32        gm_dist3(Vector3 a, Vector3 b);
static inline Vector3    gm_from3(Vector3 a, Vector3 b);
static inline Vector3    gm_normalize3(Vector3 v);
static inline Vector3    gm_reflect3(Vector3 v, Vector3 n);
static inline Vector3    gm_scale_add3(Vector3 a, Vector3 b, f32 s);
static inline Vector3    gm_project3(Vector3 a, Vector3 b);

static inline bool        gm_equal3(Vector3 a, Vector3 b, f32 epsilon);

// Vector4 ------------------------------------------------

static inline Vector4    gm_vector4(f32 x, f32 y, f32 z, f32 w);
static inline Vector4    gm_splat4(f32 s);
static inline Vector3    gm_dropw4(Vector4 v);

static inline Vector4    gm_zero4();

static inline Vector4    gm_add4(Vector4 a, Vector4 b);
static inline Vector4    gm_sub4(Vector4 a, Vector4 b);
static inline Vector4    gm_negate4(Vector4 v);
static inline Vector4    gm_lerp4(Vector4 a, Vector4 b, f32 t);
static inline Vector4    gm_element_mult4(Vector4 a, Vector4 b);
static inline Vector4    gm_mult4(Vector4 v, f32 s);
static inline Vector4    gm_div4(Vector4 v, f32 s);
static inline f32        gm_dot4(Vector4 a, Vector4 b);
static inline Vector4    gm_cross4(Vector4 a, Vector4 b);
static inline f32        gm_mag_squared4(Vector4 v);
static inline f32        gm_mag4(Vector4 v);
static inline f32        gm_dist_squared4(Vector4 a, Vector4 b);
static inline f32        gm_dist4(Vector4 a, Vector4 b);
static inline Vector4    gm_from4(Vector4 a, Vector4 b);
static inline Vector4    gm_normalize4(Vector4 v);
static inline Vector4    gm_scale_add4(Vector4 a, Vector4 b, f32 s);
static inline Vector4    gm_project4(Vector4 a, Vector4 b);

static inline bool       gm_equal4(Vector4 a, Vector4 b, f32 epsilon);

// Quaternion --------------------------------------------------

static inline Quaternion       gm_quaternion(f32 w, f32 i, f32 j, f32 k);
static inline Quaternion       gm_quaternion_from_w_ijk(f32 w, Vector3 ijk);
static inline Quaternion       gm_quaternion_from_angle_axis(f32 angle_radians, Vector3 axis);

static inline Quaternion       gm_quaternion_identity();

static inline Quaternion       gm_quaternion_mult(Quaternion a, Quaternion b);
static inline Quaternion       gm_quaternion_mult_scalar(Quaternion q, f32 s);
static inline Quaternion       gm_quaternion_div_scalar(Quaternion q, f32 s);
static inline Vector4          gm_quaternion_as_vector4(Quaternion q);
static inline f32              gm_quaternion_mag(Quaternion q);
static inline Quaternion       gm_quaternion_normalize(Quaternion q);
static inline Quaternion       gm_quaternion_conjugate(Quaternion q);
static inline Quaternion       gm_quaternion_inverse(Quaternion q);
static inline Matrix4          gm_quaternion_as_matrix4(Quaternion q);
static inline Vector3          gm_quaternion_rotate_vector3(Quaternion q, Vector3 v);
static inline Quaternion       gm_quaternion_slerp(Quaternion a, Quaternion b, f32 t);

static inline bool             gm_quaternion_equal(Quaternion a, Quaternion b, f32 epsilon);

static inline Quaternion       gm_quaternion_add(Quaternion a, Quaternion b);
static inline Quaternion       gm_quaternion_sub(Quaternion a, Quaternion b);
static inline f32              gm_quaternion_dot(Quaternion a, Quaternion b);
static inline void             gm_quaternion_to_angle_axis(Quaternion q, f32* out_angle, Vector3* out_axis);
static inline Quaternion       gm_quaternion_nlerp(Quaternion a, Quaternion b, f32 t);

// Matrix4 ------------------------------------------------------

static inline Matrix4          gm_matrix4(f32 (*m)[16]);

static inline Matrix4          gm_matrix4_identity();
static inline Matrix4          gm_matrix4_translation(f32 x, f32 y, f32 z);
static inline Matrix4          gm_matrix4_scaling(f32 x, f32 y, f32 z);
static inline Matrix4          gm_matrix4_rotation(f32 angle_degrees, Vector3 axis);
static inline Matrix4          gm_matrix4_rotation_x(f32 angle_degrees);
static inline Matrix4          gm_matrix4_rotation_y(f32 angle_degrees);
static inline Matrix4          gm_matrix4_rotation_z(f32 angle_degrees);
static inline Matrix4          gm_matrix4_shearing(f32 xy, f32 xz, f32 yx, f32 yz, f32 zx, f32 zy);
static inline Matrix4          gm_matrix4_look_at(Vector3 from, Vector3 to, Vector3 up);

static inline Matrix4          gm_matrix4_mult(const Matrix4 *a, const Matrix4 *b);
static inline Vector4          gm_matrix4_mult_vector4(const Matrix4 *m, Vector4 v);
static inline Matrix4          gm_matrix4_transpose(const Matrix4 *m);
static inline Matrix4          gm_matrix4_inverse(const Matrix4 *m);
static inline void             gm_matrix4_inverse_out(Matrix4 *out, const Matrix4 *m);
static inline void             gm_matrix4_mult_out(Matrix4 *out, const Matrix4 *a, const Matrix4 *b);

static inline bool             gm_matrix4_equal(const Matrix4 *a, const Matrix4 * b, f32 epsilon);

// Quadratics -----------------------------------------------------

static inline QuadraticSolution gm_quadratics_solve(f32 a, f32 b, f32 c);

// Ray ---------------------------------------------------------------------

#include "gmath_math.h"
#include "gmath_vector.h"
#include "gmath_quaternion.h"
#include "gmath_matrix.h"

#endif // GMATH_H_