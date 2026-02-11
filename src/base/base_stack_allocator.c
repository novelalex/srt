#include "base.h"

// Stack Allocator --------------------------------------------

struct BaseStackAllocator {
    void* backing_buffer;
    usize current_marker;
    usize total_size_bytes;
    usize peak_size_bytes;
    usize align;
};

BaseStackAllocator* nb_stack_allocator_create(usize stack_size_bytes);

void                nb_stack_allocator_destroy(BaseStackAllocator* allocator);

void*               nb_stack_allocator_allocate(BaseStackAllocator* allocator, usize size_bytes, usize align);
usize               nb_stack_allocator_marker(BaseStackAllocator* allocator);
void                nb_stack_allocator_free_to_marker(BaseStackAllocator* allocator, usize marker);
void                nb_stack_allocator_clear(BaseStackAllocator* allocator);

usize               nb_stack_allocator_used_memory(BaseStackAllocator* allocator);
usize               nb_stack_allocator_peak_memory(BaseStackAllocator* allocator);