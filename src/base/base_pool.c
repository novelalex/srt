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
    void* allocated = null;
    if (pool->free_list_head == null) {
        // TODO:(Novel) Log error here
        return null; 
    }
    allocated = pool->free_list_head;
    pool->free_list_head = *((void**)(pool->free_list_head));

    pool->used_size += pool->object_size;
    pool->peak_size = NB_MAX(pool->peak_size, pool->used_size);

    return allocated;
}

internal b32 nb_pool_validate_ptr(Pool* pool, void* ptr) {
    if (ptr == null) {
        // TODO:(Novel) Logging
        return false;
    }

    uptr ptr_to_check = (uptr)ptr;

    uptr start_buf = (uptr)pool->mem;
    uptr end_buf = start_buf + pool->size;
    b32 is_ptr_in_buf = (ptr_to_check >= start_buf) &&
        (ptr_to_check < end_buf);
    
    ptrdiff offset = ptr_to_check - start_buf;
    b32 isPtrAlligned =
        (offset % pool->object_size) == 0;

    return is_ptr_in_buf && isPtrAlligned;
}  


void nb_pool_free(Pool* pool, void* ptr) {
    #ifdef NB_BUILD_DEBUG
        // For debug builds we can take some time to check if
        // the free is valid.
        NB_ASSERT_MSG(nb_pool_validate_ptr(pool, ptr),
            "Pool Allocator: Invalid pointer free");
    #endif

    // Make the freed object the head of the free ptr list
    *(uptr*)ptr = (uptr)(pool->free_list_head);
    pool->free_list_head = ptr;

    pool->used_size -= pool->object_size;
}

usize nb_pool_used_memory(Pool* pool) {
    return pool->used_size;
}
usize nb_pool_peak_memory(Pool* pool) {
    return pool->peak_size;
}
