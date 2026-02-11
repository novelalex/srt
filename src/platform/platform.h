#ifndef PLATFORM_H_
#define PLATFORM_H_

#include "../common/common.h"

// MEMORY -------------------------------------------------------

typedef enum {
    PLATFORM_MEMORY_PROTECTION_NONE    = 0,
    PLATFORM_MEMORY_PROTECTION_READ    = 1 << 0,
    PLATFORM_MEMORY_PROTECTION_WRITE   = 1 << 1,
    PLATFORM_MEMORY_PROTECTION_EXEC    = 1 << 2,
} PlatformMemoryProtection;

void*   platform_memory_reserve(usize size);
void    platform_memory_commit(void* ptr, usize size);
void    platform_memory_decommit(void* ptr, usize size);
void    platform_memory_release(void* ptr, usize size);
void    platform_memory_set_protection(void* ptr, usize size, u32 flags);
usize   platform_memory_get_page_size(void);

// FILE IO -------------------------------------------------------

typedef struct PlatformFile PlatformFile;

PlatformFile*   platform_file_open(const char* filepath);
usize           platform_file_read(PlatformFile* f, void* buffer, usize size);
usize           platform_file_write(PlatformFile* f, void* buffer, usize size);
b32             platform_file_close(PlatformFile* f);
b32             platform_file_exists(const char* filepath);

// TIMING --------------------------------------------------------

u64     platform_time_get_ticks(void);
f64     platform_time_get_seconds(void);
void    platform_time_sleep(u32 ms);

// DEBUG ------------------------------------------------

void platform_debug_print(const char* fmt, ...);
void platform_abort(void);

#endif // PLATFORM_H_