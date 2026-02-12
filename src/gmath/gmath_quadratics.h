#ifndef GMATH_QUADRATICS_H_
#define GMATH_QUADRATICS_H_

#include "gmath_types.h"
#include "gmath_math.h"

static inline QuadraticSolution gm_quadratics_solve(f32 a, f32 b, f32 c) {
    f32 discriminant = (b * b) - (4.0f * a * c);
    if (discriminant < -NB_EPSILON_F) {
        return (QuadraticSolution){
            0, {}
        };
    } else if (gm_approx_eq(discriminant, 0.0f)) {
        return (QuadraticSolution){
            1, {(-b) / (2.0f * a)}
        };
    } 
    return (QuadraticSolution){
        2, {
            ((-b) + gm_sqrt(discriminant)) / (2.0f * a),
            ((-b) - gm_sqrt(discriminant)) / (2.0f * a),
        }
    };
}

#endif //GMATH_QUADRATICS_H_
