#ifndef GMATH_MATRIX_H_
#define GMATH_MATRIX_H_


#include "gmath_types.h"
#include "gmath_math.h"
#include "gmath_quaternion.h"

// Matrix4 Functions ------------------------------------------------------

static inline Matrix4 gm_matrix4(f32 (*m)[16]) {
    Matrix4 result;
    result.m[0]  = (*m)[0];  result.m[1]  = (*m)[1];  result.m[2]  = (*m)[2];  result.m[3]  = (*m)[3];
    result.m[4]  = (*m)[4];  result.m[5]  = (*m)[5];  result.m[6]  = (*m)[6];  result.m[7]  = (*m)[7];
    result.m[8]  = (*m)[8];  result.m[9]  = (*m)[9];  result.m[10] = (*m)[10]; result.m[11] = (*m)[11];
    result.m[12] = (*m)[12]; result.m[13] = (*m)[13]; result.m[14] = (*m)[14]; result.m[15] = (*m)[15];
    return result;
}

static inline Matrix4 gm_matrix4_identity() {
    Matrix4 m = {0};
    m.m[0] = 1.0f;
    m.m[5] = 1.0f;
    m.m[10] = 1.0f;
    m.m[15] = 1.0f;
    return m;
}

static inline Matrix4 gm_matrix4_translation(f32 x, f32 y, f32 z) {
    Matrix4 m = gm_matrix4_identity();
    m.m[12] = x;
    m.m[13] = y;
    m.m[14] = z;
    return m;
}

static inline Matrix4 gm_matrix4_scaling(f32 x, f32 y, f32 z) {
    Matrix4 m = {0};
    m.m[0] = x;
    m.m[5] = y;
    m.m[10] = z;
    m.m[15] = 1.0f;
    return m;
}

static inline Matrix4 gm_matrix4_rotation(f32 angle_degrees, Vector3 axis) {
    f32 angle_radians = gm_deg2rad(angle_degrees);
    Quaternion q = gm_quaternion_from_angle_axis(angle_radians, gm_normalize3(axis));
    return gm_quaternion_as_matrix4(q);
}

static inline Matrix4 gm_matrix4_rotation_x(f32 angle_degrees) {
    f32 angle_rad = gm_deg2rad(angle_degrees);
    f32 c = gm_cos(angle_rad);
    f32 s = gm_sin(angle_rad);

    Matrix4 m = gm_matrix4_identity();
    m.m[5] = c;
    m.m[6] = s;
    m.m[9] = -s;
    m.m[10] = c;
    return m;
}

static inline Matrix4 gm_matrix4_rotation_y(f32 angle_degrees) {
    f32 angle_rad = gm_deg2rad(angle_degrees);
    f32 c = gm_cos(angle_rad);
    f32 s = gm_sin(angle_rad);

    Matrix4 m = gm_matrix4_identity();
    m.m[0] = c;
    m.m[2] = -s;
    m.m[8] = s;
    m.m[10] = c;
    return m;
}

static inline Matrix4 gm_matrix4_rotation_z(f32 angle_degrees) {
    f32 angle_rad = gm_deg2rad(angle_degrees);
    f32 c = gm_cos(angle_rad);
    f32 s = gm_sin(angle_rad);

    Matrix4 m = gm_matrix4_identity();
    m.m[0] = c;
    m.m[1] = s;
    m.m[4] = -s;
    m.m[5] = c;
    return m;
}

static inline Matrix4 gm_matrix4_shearing(f32 xy, f32 xz, f32 yx, f32 yz, f32 zx, f32 zy) {
    Matrix4 m = {0};
    m.m[0] = 1.0f;  m.m[4] = yx;  m.m[8]  = zx;
    m.m[1] = xy;    m.m[5] = 1.0f; m.m[9]  = zy;
    m.m[2] = xz;    m.m[6] = yz;   m.m[10] = 1.0f;
    m.m[15] = 1.0f;
    return m;
}

static inline Matrix4 gm_matrix4_look_at(Vector3 eye, Vector3 target, Vector3 up) {
    Vector3 forward = gm_normalize3(gm_sub3(target, eye));
    Vector3 right = gm_normalize3(gm_cross3(forward, up));
    Vector3 camera_up = gm_cross3(right, forward);
    
    Matrix4 result = {0};
    
    result.m[0] = right.x;
    result.m[1] = right.y;
    result.m[2] = right.z;
    result.m[3] = 0.0f;
    
    result.m[4] = camera_up.x;
    result.m[5] = camera_up.y;
    result.m[6] = camera_up.z;
    result.m[7] = 0.0f;
    
    result.m[8]  = -forward.x;
    result.m[9]  = -forward.y;
    result.m[10] = -forward.z;
    result.m[11] = 0.0f;
    
    result.m[12] = -gm_dot3(right, eye);
    result.m[13] = -gm_dot3(camera_up, eye);
    result.m[14] = gm_dot3(forward, eye);
    result.m[15] = 1.0f;
    
    return result;
}

static inline Matrix4 gm_matrix4_mult(const Matrix4 *a, const Matrix4 *b) {
    Matrix4 r;
    r.m[0]  = a->m[0]*b->m[0]  + a->m[4]*b->m[1]  + a->m[8]*b->m[2]  + a->m[12]*b->m[3];
    r.m[1]  = a->m[1]*b->m[0]  + a->m[5]*b->m[1]  + a->m[9]*b->m[2]  + a->m[13]*b->m[3];
    r.m[2]  = a->m[2]*b->m[0]  + a->m[6]*b->m[1]  + a->m[10]*b->m[2]  + a->m[14]*b->m[3];
    r.m[3]  = a->m[3]*b->m[0]  + a->m[7]*b->m[1]  + a->m[11]*b->m[2]  + a->m[15]*b->m[3];

    r.m[4]  = a->m[0]*b->m[4]  + a->m[4]*b->m[5]  + a->m[8]*b->m[6]  + a->m[12]*b->m[7];
    r.m[5]  = a->m[1]*b->m[4]  + a->m[5]*b->m[5]  + a->m[9]*b->m[6]  + a->m[13]*b->m[7];
    r.m[6]  = a->m[2]*b->m[4]  + a->m[6]*b->m[5]  + a->m[10]*b->m[6] + a->m[14]*b->m[7];
    r.m[7]  = a->m[3]*b->m[4]  + a->m[7]*b->m[5]  + a->m[11]*b->m[6] + a->m[15]*b->m[7];

    r.m[8]  = a->m[0]*b->m[8]  + a->m[4]*b->m[9]  + a->m[8]*b->m[10] + a->m[12]*b->m[11];
    r.m[9]  = a->m[1]*b->m[8]  + a->m[5]*b->m[9]  + a->m[9]*b->m[10] + a->m[13]*b->m[11];
    r.m[10] = a->m[2]*b->m[8]  + a->m[6]*b->m[9]  + a->m[10]*b->m[10] + a->m[14]*b->m[11];
    r.m[11] = a->m[3]*b->m[8]  + a->m[7]*b->m[9]  + a->m[11]*b->m[10] + a->m[15]*b->m[11];

    r.m[12] = a->m[0]*b->m[12] + a->m[4]*b->m[13] + a->m[8]*b->m[14] + a->m[12]*b->m[15];
    r.m[13] = a->m[1]*b->m[12] + a->m[5]*b->m[13] + a->m[9]*b->m[14] + a->m[13]*b->m[15];
    r.m[14] = a->m[2]*b->m[12] + a->m[6]*b->m[13] + a->m[10]*b->m[14] + a->m[14]*b->m[15];
    r.m[15] = a->m[3]*b->m[12] + a->m[7]*b->m[13] + a->m[11]*b->m[14] + a->m[15]*b->m[15];
    return r;
}

static inline Vector4 gm_matrix4_mult_vector4(const Matrix4 *m, Vector4 v) {
    return (Vector4){
        m->m[0] * v.x + m->m[4] * v.y + m->m[8]  * v.z + m->m[12] * v.w,
        m->m[1] * v.x + m->m[5] * v.y + m->m[9]  * v.z + m->m[13] * v.w,
        m->m[2] * v.x + m->m[6] * v.y + m->m[10] * v.z + m->m[14] * v.w,
        m->m[3] * v.x + m->m[7] * v.y + m->m[11] * v.z + m->m[15] * v.w
    };
}

static inline Matrix4 gm_matrix4_transpose(const Matrix4 *m) {
    Matrix4 result;
    result.m[0]  = m->m[0];
    result.m[1]  = m->m[4];
    result.m[2]  = m->m[8];
    result.m[3]  = m->m[12];
    result.m[4]  = m->m[1];
    result.m[5]  = m->m[5];
    result.m[6]  = m->m[9];
    result.m[7]  = m->m[13];
    result.m[8]  = m->m[2];
    result.m[9]  = m->m[6];
    result.m[10] = m->m[10];
    result.m[11] = m->m[14];
    result.m[12] = m->m[3];
    result.m[13] = m->m[7];
    result.m[14] = m->m[11];
    result.m[15] = m->m[15];
    return result;
}

static inline void gm_matrix4_inverse_out(Matrix4 *out, const Matrix4 *m) {
    f32 m00 = m->m[0],  m01 = m->m[1],  m02 = m->m[2],  m03 = m->m[3];
    f32 m10 = m->m[4],  m11 = m->m[5],  m12 = m->m[6],  m13 = m->m[7];
    f32 m20 = m->m[8],  m21 = m->m[9],  m22 = m->m[10], m23 = m->m[11];
    f32 m30 = m->m[12], m31 = m->m[13], m32 = m->m[14], m33 = m->m[15];

    f32 b00 = m00 * m11 - m01 * m10;
    f32 b01 = m00 * m12 - m02 * m10;
    f32 b02 = m00 * m13 - m03 * m10;
    f32 b03 = m01 * m12 - m02 * m11;
    f32 b04 = m01 * m13 - m03 * m11;
    f32 b05 = m02 * m13 - m03 * m12;
    f32 b06 = m20 * m31 - m21 * m30;
    f32 b07 = m20 * m32 - m22 * m30;
    f32 b08 = m20 * m33 - m23 * m30;
    f32 b09 = m21 * m32 - m22 * m31;
    f32 b10 = m21 * m33 - m23 * m31;
    f32 b11 = m22 * m33 - m23 * m32;

    f32 det = b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;
    NB_ASSERT_MSG(det != 0.0f, "Tried to invert a singular matrix");

    f32 inv_det = 1.0f / det;

    out->m[0]  = (m11 * b11 - m12 * b10 + m13 * b09) * inv_det;
    out->m[1]  = (m02 * b10 - m01 * b11 - m03 * b09) * inv_det;
    out->m[2]  = (m31 * b05 - m32 * b04 + m33 * b03) * inv_det;
    out->m[3]  = (m22 * b04 - m21 * b05 - m23 * b03) * inv_det;
    out->m[4]  = (m12 * b08 - m10 * b11 - m13 * b07) * inv_det;
    out->m[5]  = (m00 * b11 - m02 * b08 + m03 * b07) * inv_det;
    out->m[6]  = (m32 * b02 - m30 * b05 - m33 * b01) * inv_det;
    out->m[7]  = (m20 * b05 - m22 * b02 + m23 * b01) * inv_det;
    out->m[8]  = (m10 * b10 - m11 * b08 + m13 * b06) * inv_det;
    out->m[9]  = (m01 * b08 - m00 * b10 - m03 * b06) * inv_det;
    out->m[10] = (m30 * b04 - m31 * b02 + m33 * b00) * inv_det;
    out->m[11] = (m21 * b02 - m20 * b04 - m23 * b00) * inv_det;
    out->m[12] = (m11 * b07 - m10 * b09 - m12 * b06) * inv_det;
    out->m[13] = (m00 * b09 - m01 * b07 + m02 * b06) * inv_det;
    out->m[14] = (m31 * b01 - m30 * b03 - m32 * b00) * inv_det;
    out->m[15] = (m20 * b03 - m21 * b01 + m22 * b00) * inv_det;
}

static inline Matrix4 gm_matrix4_inverse(const Matrix4 *m) {
    Matrix4 result;
    gm_matrix4_inverse_out(&result, m);
    return result;
}

static inline void gm_matrix4_mult_out(Matrix4 *out, const Matrix4 *a, const Matrix4 *b) {
    out->m[0]  = a->m[0]*b->m[0]  + a->m[4]*b->m[1]  + a->m[8]*b->m[2]  + a->m[12]*b->m[3];
    out->m[1]  = a->m[1]*b->m[0]  + a->m[5]*b->m[1]  + a->m[9]*b->m[2]  + a->m[13]*b->m[3];
    out->m[2]  = a->m[2]*b->m[0]  + a->m[6]*b->m[1]  + a->m[10]*b->m[2]  + a->m[14]*b->m[3];
    out->m[3]  = a->m[3]*b->m[0]  + a->m[7]*b->m[1]  + a->m[11]*b->m[2]  + a->m[15]*b->m[3];

    out->m[4]  = a->m[0]*b->m[4]  + a->m[4]*b->m[5]  + a->m[8]*b->m[6]  + a->m[12]*b->m[7];
    out->m[5]  = a->m[1]*b->m[4]  + a->m[5]*b->m[5]  + a->m[9]*b->m[6]  + a->m[13]*b->m[7];
    out->m[6]  = a->m[2]*b->m[4]  + a->m[6]*b->m[5]  + a->m[10]*b->m[6] + a->m[14]*b->m[7];
    out->m[7]  = a->m[3]*b->m[4]  + a->m[7]*b->m[5]  + a->m[11]*b->m[6] + a->m[15]*b->m[7];

    out->m[8]  = a->m[0]*b->m[8]  + a->m[4]*b->m[9]  + a->m[8]*b->m[10] + a->m[12]*b->m[11];
    out->m[9]  = a->m[1]*b->m[8]  + a->m[5]*b->m[9]  + a->m[9]*b->m[10] + a->m[13]*b->m[11];
    out->m[10] = a->m[2]*b->m[8]  + a->m[6]*b->m[9]  + a->m[10]*b->m[10] + a->m[14]*b->m[11];
    out->m[11] = a->m[3]*b->m[8]  + a->m[7]*b->m[9]  + a->m[11]*b->m[10] + a->m[15]*b->m[11];

    out->m[12] = a->m[0]*b->m[12] + a->m[4]*b->m[13] + a->m[8]*b->m[14] + a->m[12]*b->m[15];
    out->m[13] = a->m[1]*b->m[12] + a->m[5]*b->m[13] + a->m[9]*b->m[14] + a->m[13]*b->m[15];
    out->m[14] = a->m[2]*b->m[12] + a->m[6]*b->m[13] + a->m[10]*b->m[14] + a->m[14]*b->m[15];
    out->m[15] = a->m[3]*b->m[12] + a->m[7]*b->m[13] + a->m[11]*b->m[14] + a->m[15]*b->m[15];
}

static inline bool gm_matrix4_equal(const Matrix4 *a, const Matrix4 *b, f32 epsilon) {
    return gm_abs(a->m[0]  - b->m[0])  <= epsilon && gm_abs(a->m[1]  - b->m[1])  <= epsilon &&
           gm_abs(a->m[2]  - b->m[2])  <= epsilon && gm_abs(a->m[3]  - b->m[3])  <= epsilon &&
           gm_abs(a->m[4]  - b->m[4])  <= epsilon && gm_abs(a->m[5]  - b->m[5])  <= epsilon &&
           gm_abs(a->m[6]  - b->m[6])  <= epsilon && gm_abs(a->m[7]  - b->m[7])  <= epsilon &&
           gm_abs(a->m[8]  - b->m[8])  <= epsilon && gm_abs(a->m[9]  - b->m[9])  <= epsilon &&
           gm_abs(a->m[10] - b->m[10]) <= epsilon && gm_abs(a->m[11] - b->m[11]) <= epsilon &&
           gm_abs(a->m[12] - b->m[12]) <= epsilon && gm_abs(a->m[13] - b->m[13]) <= epsilon &&
           gm_abs(a->m[14] - b->m[14]) <= epsilon && gm_abs(a->m[15] - b->m[15]) <= epsilon;
}

#endif //GMATH_MATRIX_H_
