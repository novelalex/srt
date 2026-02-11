#include "platform.h"
#include <sys/mman.h>
#include <unistd.h>
#include <stdarg.h>
#include <fcntl.h>
#include <time.h>

// GLOBALS --------------------------------------------------

global struct timespec linux_time_start;
global b32 linux_time_initialized = false;

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

// FILE IO -------------------------------------------------------

struct PlatformFile{
    int handle;
};

PlatformFile* platform_file_open(const char* filepath) {
    int fd = open(filepath, O_RDWR | O_CREAT, 0644);
    
    if (fd == -1) {
        return NULL;
    }
    
    PlatformFile* pf = malloc(sizeof(PlatformFile));
    pf->handle = fd;
    return pf;
}

usize platform_file_read(PlatformFile* f, void* buffer, usize size) {
    ssize_t result = read(f->handle, buffer, size);
    if (result < 0) {
        return 0;
    }
    return result; 
}

usize platform_file_write(PlatformFile* f, void* buffer, usize size) {
    ssize_t result = write(f->handle, buffer, size);
    if (result < 0) {
        return 0;
    }
    return result;
}

b32 platform_file_close(PlatformFile* f) {
    int result = close(f->handle);
    free(f);
    if (result != 0) {
        return false;
    }
    return true;
}

b32 platform_file_exists(const char* filepath) {
    int result = access(filepath, F_OK);

    if (result == 0) {
        return true;
    }

    return false;
}

// TIMING --------------------------------------------------------

internal void linux_time_init(void) {
    clock_gettime(CLOCK_MONOTONIC, &linux_time_start);
    linux_time_initialized = true;
}

u64 platform_time_get_ticks(void) {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (u64)now.tv_sec * 1000000000ULL + (u64)now.tv_nsec;
}

f64 platform_time_get_seconds(void) {
    if (!linux_time_initialized) {
        linux_time_init();
    }

    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);

    u64 elapsed_sec = now.tv_sec - linux_time_start.tv_sec;
    u64 elapsed_nsec = now.tv_nsec - linux_time_start.tv_nsec;

    f64 result = (f64)elapsed_sec + (f64)elapsed_nsec / 1000000000.0;
    
    return result;
}


void platform_time_sleep(u32 ms) {
    usleep(ms * 1000);
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