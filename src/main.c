#include "platform/platform.c"
#include "base/base.c"

// ENTRYPOINT --------------------------------------------

int main(int argc, char** argv) {
    Arena* arena = nb_arena_create(null, sizeof(u8)*2048);
    platform_debug_print("Program arena peak usage: %d bytes\n", nb_arena_peak_memory(arena));
    Hashtable* t = nb_hashtable_create(arena);
    nb_hashtable_set(t, arena, "novel", (void*)21);
    nb_hashtable_set(t, arena, "adriel", (void*)22);
    platform_debug_print("Program arena peak usage: %d bytes\n", nb_arena_peak_memory(arena));

    platform_debug_print("Novel's age: %d\n", (u64)(nb_hashtable_get(t, "novel")));
    platform_debug_print("Adriel's age: %d\n", (u64)(nb_hashtable_get(t, "adriel")));

    nb_arena_clear(arena);
    platform_debug_print("Program arena peak usage: %d bytes\n", nb_arena_peak_memory(arena));
    nb_arena_destroy(arena);
    arena = null;
}