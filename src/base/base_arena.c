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


// Arena Allocator --------------------------------------------

struct Arena {
    void* mem;
    usize marker;
    usize size;
    usize peak_size;
};

Arena* nb_arena_create(usize size) {
    usize total_size = sizeof(Arena) + size;
    void* mem = platform_memory_reserve(total_size);
    if (!mem) {
        return null;
    }
    platform_memory_commit(mem, total_size);

    Arena* arena = (Arena*)mem;
    arena->mem = (u8*)mem + sizeof(Arena);
    arena->marker = 0;
    arena->size = size;
    arena->peak_size = 0;

    return arena;
    
}

void nb_arena_destroy(Arena* arena) {
    usize total_size = sizeof(Arena) + arena->size;
    platform_memory_decommit(arena, total_size);
    platform_memory_release(arena, total_size);
}

void* nb_arena_allocate(Arena* arena, usize size, usize align) {
    uptr base = (uptr)arena->mem;
    uptr current = base + arena->marker;
    uptr aligned = nb_align_address(current, align);
    usize padding = aligned - current;

    if (arena->marker + padding + size > arena->size) {
        // TODO:(Novel) Replace with logging system
        platform_debug_print(
            "Arena requested more that total size.."
            "\n\tMax size: %d bytes"
            "\n\tRequested: %d bytes"
            "\n\tPadding: %d bytes"
            "\n\tCurrent size: %d"
            "\nOver max by %d bytes",
        arena->size, size, padding, arena->marker, (arena->marker + padding + size) - arena->size);
        NB_ASSERT(false);
        return null;
    }

    arena->marker += padding + size;
    arena->peak_size = NB_MAX(arena->peak_size, arena->marker);

    return (void*)aligned;
}

void nb_arena_clear(Arena* arena) {
    arena->marker = 0;
}

usize nb_arena_get_marker(Arena* arena) {
    return arena->marker;
}

void nb_arena_free_to_marker(Arena* arena, usize marker) {
    NB_ASSERT(marker <= arena->size);
    arena->marker = marker;
}

usize nb_arena_used_memory(Arena* arena) {
    return arena->marker;
}
usize nb_arena_peak_memory(Arena* arena) {
    return arena->peak_size;
}




