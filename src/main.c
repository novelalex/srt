#include "platform/platform.c"
#include "base/base_core.c"

// ENTRYPOINT --------------------------------------------

int main(int argc, char** argv) {
    platform_debug_print("Hello World!");

    int* x = nb_aligned_allocate(sizeof(int) * 4, NB_ALIGNMENT);
    *x = 5;
    x[1] = 4;

    platform_debug_print("x[0]: %d, x[1]:%d", x[0], x[1]);

    nb_aligned_free(x, sizeof(int) * 4, NB_ALIGNMENT);
}


