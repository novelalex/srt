#include "platform/platform.c"
#include "base/base.c"

#include <string.h>

// ENTRYPOINT --------------------------------------------

int main(int argc, char** argv) {
    Arena* arena = nb_arena_create(sizeof(u8)*2048);

    int* x = nb_arena_allocate(arena, sizeof(int), NB_ALIGNMENT);
    *x = 67;

    platform_debug_print("x:%d\n\n", *x);

    usize marker = nb_arena_get_marker(arena);

    int* y = nb_arena_allocate(arena, sizeof(int), NB_ALIGNMENT);
    *y = 89;

    platform_debug_print("y:%d\n\n", *y);

    nb_arena_free_to_marker(arena, marker);

    int* z = nb_arena_allocate(arena, sizeof(int), NB_ALIGNMENT);
    *z = 100;

    platform_debug_print("z:%d\n", *z);
    platform_debug_print("y:%d\n\n", *y);


    nb_arena_clear(arena);

    int* w = nb_arena_allocate(arena, sizeof(int), NB_ALIGNMENT);
    *w = 12;

    platform_debug_print("w:%d\n", *w);
    platform_debug_print("x:%d\n\n", *x);

    nb_arena_destroy(arena);
    arena = null;

    const char* text = "Hello world";
    usize len = strlen(text);

    void* buffer = nb_aligned_allocate(len + 1, 1);
    memcpy(buffer, text, len);

    PlatformFile* f = platform_file_open("helo.txt");
    platform_file_write(f, buffer, len);

    platform_file_close(f);

    nb_aligned_free(buffer, len + 1, 1);
}


