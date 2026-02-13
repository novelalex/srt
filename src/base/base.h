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

// If parent arena is null this will use platform's allocator
Arena*      nb_arena_create(Arena* parent, usize size);

// This is safe to call on non platform allocated arenas
void        nb_arena_destroy(Arena* arena);

void*       nb_arena_alloc(Arena* arena, usize size);
void*       nb_arena_alloc_aligned(Arena* arena, usize size, usize align);
void        nb_arena_clear(Arena* arena);
usize       nb_arena_get_marker(Arena* arena);
void        nb_arena_free_to_marker(Arena* arena, usize marker);

usize       nb_arena_used_memory(Arena* arena);
usize       nb_arena_peak_memory(Arena* arena);

// Pool Allocator ------------------------------------------------------

typedef struct Pool Pool;

Pool*       nb_pool_create(Arena *a, usize size, usize count);
Pool*       nb_pool_create_aligned(Arena *a, usize size, usize count, usize align);

void*       nb_pool_alloc(Pool* pool);  
void        nb_pool_clear(Pool* pool);
void        nb_pool_free(Pool* pool, void* ptr);

usize       nb_pool_used_memory(Pool* pool);
usize       nb_pool_peak_memory(Pool* pool);

// Hashtable -----------------------------------------------------------

typedef struct Hashtable Hashtable;

Hashtable*  nb_hashtable_create(Arena* arena);
void*       nb_hashtable_get(Hashtable* ht, const char* key);
const char* nb_hashtable_set(Hashtable* ht, Arena* arena, const char* key, void* value);
usize       nb_hashtable_length(Hashtable *ht);

typedef struct HashtableIterator {
    const char* key;
    void* value;
    Hashtable* _table;
    size_t _index;
} HashtableIterator;

HashtableIterator   nb_hashtable_iterator(Hashtable *ht);
b8                  nb_hashtable_next(HashtableIterator *hti);



#endif // BASE_H_