#include "../common.h"

void    gm_vector3f(f32 *out, f32 x, f32 y, f32 z);
void    gm_splat3f(f32 *out, f32 s);
void    gm_zero3f(f32 *out);
void    gm_up3f(f32 *out);
void    gm_down3f(f32 *out);
void    gm_left3f(f32 *out);
void    gm_right3f(f32 *out);
void    gm_forward3f(f32 *out);
void    gm_back3f(f32 *out);

void    gm_add3f(f32 *out, f32 *a, f32 *b);
void    gm_sub3f(f32 *out, f32 *a, f32 *b);
void    gm_negate3f(f32 *out, f32 *v);
void    gm_lerp3f(f32 *out, f32 *a, f32 *b, f32 t);
void    gm_hadamard_mult3f(f32 *out, f32 *a, f32 *b);
void    gm_mult3f(f32 *out, f32 *v, f32 s);
void    gm_div3f(f32 *out, f32 *v, f32 s);
f32     gm_dot3f(f32 *a, f32 *b);
void    gm_cross3f(f32 *out, f32 *a, f32 *b);
f32     gm_mag_squared3f(f32 *v);
f32     gm_mag3f(f32 *v);
f32     gm_dist_squared3f(f32 *a, f32 *b);
f32     gm_dist3f(f32 *a, f32 *b);
void    gm_normalize3f(f32 *out, f32 *v);
void    gm_reflect3f(f32 *out, f32 *v, f32* n);
void    gm_scale_add3f(f32 *out, f32 *a, f32 *b, f32 s);

bool    gm_equals3f(f32 *a, f32 *b, f32 epsilon);



typedef struct Vector4f {
  f32 x;
  f32 y;
  f32 z;
  f32 w;
} Vector4f;

typedef struct Quaternionf {
  f32 w;
  f32 i;
  f32 j;
  f32 k;
} Quaternion;

typedef struct Matrix4f {
  f32 m[16];
} Matrix;

typedef struct Colorf {
  f32 r;
  f32 g;
  f32 b;
  f32 a;
} Color;
