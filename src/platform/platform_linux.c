#include "platform.h"
#include <sys/mman.h>
#include <unistd.h>
#include <stdarg.h>

// MEMORY ---------------------------------------------------

internal usize linux_memory_round_up_to_page_size(usize size) {
    usize page_size = platform_memory_get_page_size();
    return (size + page_size - 1) & ~(page_size - 1);
}

void* platform_memory_reserve(usize size) {
    usize actual_size = linux_memory_round_up_to_page_size(size);
    void* ptr = mmap(null, actual_size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED) {
        return null;
    }

    return ptr;
}

void platform_memory_commit(void* ptr, usize size) {
    usize actual_size = linux_memory_round_up_to_page_size(size);
    int status = mprotect(ptr, actual_size, PROT_READ | PROT_WRITE);
    NB_ASSERT_MSG(status == 0, "Failed to commit memory");
}
void platform_memory_decommit(void* ptr, usize size) {
    usize actual_size = linux_memory_round_up_to_page_size(size);
    madvise(ptr, actual_size, MADV_DONTNEED);
}
void platform_memory_release(void* ptr, usize size) {
    usize actual_size = linux_memory_round_up_to_page_size(size);
    munmap(ptr, actual_size);
}

void platform_memory_set_protection(void* ptr, usize size, u32 flags) {
    usize actual_size = linux_memory_round_up_to_page_size(size);
    int status = mprotect(ptr, actual_size, flags);
    NB_ASSERT_MSG(status == 0, "Failed to set protection");
}

usize platform_memory_get_page_size(void) {
    return sysconf(_SC_PAGESIZE);
}


// DEBUG ------------------------------------------------

void platform_debug_print(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    fflush(stdout);
}

void platform_abort(void) {
    NB_DEBUG_BREAK();
}