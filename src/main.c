#include "platform/platform.c"
#include "base/base.c"

// ENTRYPOINT --------------------------------------------

int main(int argc, char** argv) {
    Arena* arena = nb_arena_create(null, 1024*1024);
    platform_debug_print("Program arena peak usage: %d bytes\n", nb_arena_peak_memory(arena));
 
    Pool* p = nb_pool_create(arena, sizeof(u64), 1000);

    u64* n1 = nb_pool_alloc(p);
    u64* n2 = nb_pool_alloc(p);
    u64* n3 = nb_pool_alloc(p);
    platform_debug_print("pool usage: %d bytes\n", nb_pool_used_memory(p));
    u64* n4 = nb_pool_alloc(p);
    platform_debug_print("pool usage: %d bytes\n", nb_pool_used_memory(p));
    nb_pool_free(p, n1);
    nb_pool_free(p, n2);
    platform_debug_print("pool usage: %d bytes\n", nb_pool_used_memory(p));
    platform_debug_print("pool peak usage: %d bytes\n", nb_pool_peak_memory(p));



    platform_debug_print("Program arena peak usage: %d bytes\n", nb_arena_peak_memory(arena));
    nb_arena_destroy(arena);
    arena = null;
}