#ifndef BASE_H_
#define BASE_H_

#include "../common.h"
#include "../platform/platform.h"


// Aligned Allocator --------------------------------------------

uptr    nb_align_address(uptr addr, usize align);
void*   nb_align_pointer(void* ptr, usize align);

void*   nb_aligned_allocate(usize size, usize align);
void    nb_aligned_free(void* ptr, usize size, usize align);

// Arena Allocator --------------------------------------------

typedef struct Arena Arena;

Arena*      nb_arena_create(usize size);
void        nb_arena_destroy(Arena* arena);

void*       nb_arena_allocate(Arena* arena, usize size, usize align);
void        nb_arena_clear(Arena* arena);
usize       nb_arena_get_marker(Arena* arena);
void        nb_arena_free_to_marker(Arena* arena, usize marker);

usize       nb_arena_used_memory(Arena* arena);
usize       nb_arena_peak_memory(Arena* arena);




#endif // BASE_H_