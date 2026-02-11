#ifndef BASE_H_
#define BASE_H_

#include "../common/common.h"
#include "../platform/platform.h"

// Aligned Allocator --------------------------------------------

uptr    nb_align_address(uptr addr, usize align);
void*   nb_align_pointer(void* ptr, usize align);

void*   nb_aligned_allocate(usize bytes, usize align);
void    nb_aligned_free(void* ptr, usize size, usize align);

// Stack Allocator --------------------------------------------

typedef struct BaseStackAllocator BaseStackAllocator;

BaseStackAllocator* nb_stack_allocator_create(usize stack_size_bytes);
void                nb_stack_allocator_destroy(BaseStackAllocator* allocator);

void*               nb_stack_allocator_allocate(BaseStackAllocator* allocator, usize size_bytes, usize align);
usize               nb_stack_allocator_marker(BaseStackAllocator* allocator);
void                nb_stack_allocator_free_to_marker(BaseStackAllocator* allocator, usize marker);
void                nb_stack_allocator_clear(BaseStackAllocator* allocator);

usize               nb_stack_allocator_used_memory(BaseStackAllocator* allocator);
usize               nb_stack_allocator_peak_memory(BaseStackAllocator* allocator);


#endif // BASE_H_