#include "base.h"

struct Pool {
    void* mem;
    void* free_list_head;
    usize size;
    usize used_size;
    usize peak_size;
    usize object_count;
    usize object_size;
};

Pool* nb_pool_create(Arena *a, usize size, usize count) {
   return nb_pool_create_aligned(a, size, count, NB_DEFAULT_ALIGNMENT);
}

Pool* nb_pool_create_aligned(Arena *a, usize size,  usize count, usize align) {
    usize bytes_needed = 0;
    void* current_ptr = null;
    NB_ASSERT_MSG(size >= sizeof(uptr),
        "Pool allocator only supports objects bigger than the"
        "size of a pointer.");
    bytes_needed = size * count;
    
    void* mem = nb_arena_alloc_aligned(a, sizeof(Pool) + bytes_needed, align);
    Pool* pool = (Pool*)mem;
    pool->mem = (u8*)mem + sizeof(Pool);
    pool->free_list_head = pool->mem;
    pool->size = bytes_needed;
    pool->used_size = 0;
    pool->peak_size = 0;
    pool->object_count = count;
    pool->object_size = size;

    // setup linked list of free ptrs
    current_ptr = pool->free_list_head;
    for (usize i = 0; i < pool->object_count - 1; i++) {
        *(uptr*)current_ptr = (uptr)(current_ptr) + pool->object_size;
        current_ptr = (void*)(*(uptr*)current_ptr);
    }
    *(uptr*)current_ptr = (uptr)null;

    return pool;
}

void* nb_pool_alloc(Pool* pool) {

}
void        nb_pool_clear(Pool* pool);
void        nb_pool_free(Pool* pool, void* ptr);

usize       nb_pool_used_memory(Pool* pool);
usize       nb_pool_peak_memory(Pool* pool);
