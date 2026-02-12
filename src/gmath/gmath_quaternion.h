#ifndef GMATH_QUATERNION_H_
#define GMATH_QUATERNION_H_

#include "gmath_math.h"
#include "gmath_types.h"
#include "gmath_vector.h"

// Quaternion Functions --------------------------------------------------

static inline Quaternion gm_quaternion(f32 w, f32 i, f32 j, f32 k) {
    return (Quaternion){w, i, j, k};
}

static inline Quaternion gm_quaternion_from_w_ijk(f32 w, Vector3 ijk) {
    return (Quaternion){w, ijk.x, ijk.y, ijk.z};
}

static inline Quaternion gm_quaternion_from_angle_axis(f32 angle_radians, Vector3 axis) {
    f32 half_angle = angle_radians * 0.5f;
    f32 s = gm_sin(half_angle);
    return (Quaternion){
        gm_cos(half_angle),
        axis.x * s,
        axis.y * s,
        axis.z * s
    };
}

static inline Quaternion gm_quaternion_identity() {
    return (Quaternion){1.0f, 0.0f, 0.0f, 0.0f};
}

static inline Quaternion gm_quaternion_mult(Quaternion a, Quaternion b) {
    return (Quaternion){
        a.w * b.w - a.i * b.i - a.j * b.j - a.k * b.k,
        a.w * b.i + a.i * b.w + a.j * b.k - a.k * b.j,
        a.w * b.j - a.i * b.k + a.j * b.w + a.k * b.i,
        a.w * b.k + a.i * b.j - a.j * b.i + a.k * b.w
    };
}

static inline Quaternion gm_quaternion_mult_scalar(Quaternion q, f32 s) {
    return (Quaternion){q.w * s, q.i * s, q.j * s, q.k * s};
}

static inline Quaternion gm_quaternion_div_scalar(Quaternion q, f32 s) {
    return (Quaternion){q.w / s, q.i / s, q.j / s, q.k / s};
}

static inline Vector4 gm_quaternion_as_vector4(Quaternion q) {
    return (Vector4){q.i, q.j, q.k, q.w};
}

static inline f32 gm_quaternion_mag(Quaternion q) {
    return gm_sqrt(q.w * q.w + q.i * q.i + q.j * q.j + q.k * q.k);
}

static inline Quaternion gm_quaternion_normalize(Quaternion q) {
    f32 mag = gm_quaternion_mag(q);
    NB_ASSERT_MSG(mag > 1e-6f, "Tried to normalize quaternion with magnitude too close to zero");
    return mag > 0.0f ? gm_quaternion_div_scalar(q, mag) : gm_quaternion_identity();
}

static inline Quaternion gm_quaternion_conjugate(Quaternion q) {
    return (Quaternion){q.w, -q.i, -q.j, -q.k};
}

static inline Quaternion gm_quaternion_inverse(Quaternion q) {
    Quaternion conj = gm_quaternion_conjugate(q);
    f32 mag_sq = q.w * q.w + q.i * q.i + q.j * q.j + q.k * q.k;
    NB_ASSERT_MSG(mag_sq > 1e-6f, "Tried to invert quaternion with magnitude too close to zero");
    return gm_quaternion_div_scalar(conj, mag_sq);
}

static inline Matrix4 gm_quaternion_as_matrix4(Quaternion q) {
    Quaternion n = gm_quaternion_normalize(q);
    f32 w = n.w, i = n.i, j = n.j, k = n.k;

    f32 ii = i * i, ij = i * j, ik = i * k, iw = i * w;
    f32 jj = j * j, jk = j * k, jw = j * w;
    f32 kk = k * k, kw = k * w;

    Matrix4 m;
    m.m[0]  = 1.0f - 2.0f * (jj + kk);
    m.m[1]  = 2.0f * (ij + kw);
    m.m[2]  = 2.0f * (ik - jw);
    m.m[3]  = 0.0f;

    m.m[4]  = 2.0f * (ij - kw);
    m.m[5]  = 1.0f - 2.0f * (ii + kk);
    m.m[6]  = 2.0f * (jk + iw);
    m.m[7]  = 0.0f;

    m.m[8]  = 2.0f * (ik + jw);
    m.m[9]  = 2.0f * (jk - iw);
    m.m[10] = 1.0f - 2.0f * (ii + jj);
    m.m[11] = 0.0f;

    m.m[12] = 0.0f;
    m.m[13] = 0.0f;
    m.m[14] = 0.0f;
    m.m[15] = 1.0f;

    return m;
}

static inline Vector3 gm_quaternion_rotate_vector3(Quaternion q, Vector3 v) {
    Quaternion n = gm_quaternion_normalize(q);
    Quaternion v_quat = gm_quaternion_from_w_ijk(0.0f, v);
    Quaternion result = gm_quaternion_mult(gm_quaternion_mult(n, v_quat), gm_quaternion_conjugate(n));
    return (Vector3){result.i, result.j, result.k};
}

static inline Quaternion gm_quaternion_slerp(Quaternion a, Quaternion b, f32 t) {
    f32 dot = a.w * b.w + a.i * b.i + a.j * b.j + a.k * b.k;

    Quaternion bb = b;
    if (dot < 0.0f) {
        dot = -dot;
        bb = gm_quaternion_mult_scalar(b, -1.0f);
    }

    f32 half_cos_theta = dot;
    if (half_cos_theta > 0.9995f) {
        return gm_quaternion_normalize((Quaternion){
            a.w * (1.0f - t) + bb.w * t,
            a.i * (1.0f - t) + bb.i * t,
            a.j * (1.0f - t) + bb.j * t,
            a.k * (1.0f - t) + bb.k * t
        });
    }

    f32 half_theta = gm_acos(half_cos_theta);
    f32 sin_theta = gm_sqrt(1.0f - half_cos_theta * half_cos_theta);

    if (sin_theta < 1e-6f) {
        return gm_quaternion_normalize((Quaternion){
            a.w * (1.0f - t) + bb.w * t,
            a.i * (1.0f - t) + bb.i * t,
            a.j * (1.0f - t) + bb.j * t,
            a.k * (1.0f - t) + bb.k * t
        });
    }

    f32 a_scale = gm_sin((1.0f - t) * half_theta) / sin_theta;
    f32 b_scale = gm_sin(t * half_theta) / sin_theta;

    return (Quaternion){
        a.w * a_scale + bb.w * b_scale,
        a.i * a_scale + bb.i * b_scale,
        a.j * a_scale + bb.j * b_scale,
        a.k * a_scale + bb.k * b_scale
    };
}

static inline bool gm_quaternion_equal(Quaternion a, Quaternion b, f32 epsilon) {
    return gm_abs(a.w - b.w) <= epsilon &&
           gm_abs(a.i - b.i) <= epsilon &&
           gm_abs(a.j - b.j) <= epsilon &&
           gm_abs(a.k - b.k) <= epsilon;
}

static inline Quaternion gm_quaternion_add(Quaternion a, Quaternion b) {
    return (Quaternion){a.w + b.w, a.i + b.i, a.j + b.j, a.k + b.k};
}

static inline Quaternion gm_quaternion_sub(Quaternion a, Quaternion b) {
    return (Quaternion){a.w - b.w, a.i - b.i, a.j - b.j, a.k - b.k};
}

static inline f32 gm_quaternion_dot(Quaternion a, Quaternion b) {
    return a.w * b.w + a.i * b.i + a.j * b.j + a.k * b.k;
}

static inline void gm_quaternion_to_angle_axis(Quaternion q, f32* out_angle, Vector3* out_axis) {
    Quaternion n = gm_quaternion_normalize(q);
    *out_angle = 2.0f * gm_acos(n.w);
    f32 sin_half_angle = gm_sqrt(1.0f - n.w * n.w);
    if (sin_half_angle > 1e-6f) {
        *out_axis = (Vector3){n.i / sin_half_angle, n.j / sin_half_angle, n.k / sin_half_angle};
    } else {
        *out_axis = gm_up3();
    }
}

static inline Quaternion gm_quaternion_nlerp(Quaternion a, Quaternion b, f32 t) {
    Quaternion bb = b;
    f32 dot = gm_quaternion_dot(a, b);
    if (dot < 0.0f) {
        dot = -dot;
        bb = gm_quaternion_mult_scalar(b, -1.0f);
    }
    return gm_quaternion_normalize((Quaternion){
        a.w * (1.0f - t) + bb.w * t,
        a.i * (1.0f - t) + bb.i * t,
        a.j * (1.0f - t) + bb.j * t,
        a.k * (1.0f - t) + bb.k * t
    });
}

#endif // GMATH_QUATERNION_H_