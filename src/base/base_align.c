#include "base.h"

// Aligned Allocator --------------------------------------------

// This is the implementation from the book "Game Engine Architecture" by Jason Gregory

uptr nb_align_address(uptr addr, usize align) {
    const usize mask = align - 1;
    NB_ASSERT_MSG((align & mask) == 0, "'align' must be a power of 2");
    return (addr + mask) & ~mask;
}

void* nb_align_pointer(void* ptr, usize align) {
    const uptr addr = (uptr)ptr;
    const uptr addr_aligned = nb_align_address(addr, align);
    return (void*)addr_aligned;
}

void* nb_aligned_allocate(usize bytes, usize align) {
    NB_ASSERT_MSG(align <= 256, "AlignedAllocator only supports up to 256-byte alignment");

    usize actual_bytes = bytes + align;
    u8* raw_mem = platform_memory_reserve(actual_bytes);
    if (!raw_mem) {
        return null;
    }

    platform_memory_commit(raw_mem, actual_bytes);
    
    u8* aligned_mem = nb_align_pointer(raw_mem, align);
    if (aligned_mem == raw_mem) {
        aligned_mem += align;
    }
    ptrdiff shift = aligned_mem - raw_mem;

    NB_ASSERT_MSG((shift > 0 && shift <= 256), "Max diffrence in alignment is 256");

    aligned_mem[-1] = (u8)(shift & 0xFF);

    return aligned_mem;
}

void nb_aligned_free(void* ptr, usize size, usize align) {
    if (ptr) {
        u8* aligned_mem = (u8*)ptr;
        ptrdiff shift = aligned_mem[-1];
        if (shift == 0) {
            shift = 256;
        }

        u8* raw_mem = aligned_mem - shift;
        usize actual_bytes = size + align;
        platform_memory_decommit(raw_mem, actual_bytes);
        platform_memory_release(raw_mem, actual_bytes);
    }
}