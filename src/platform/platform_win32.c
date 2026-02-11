#include "platform.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

// GLOBALS --------------------------------------------------

global LARGE_INTEGER win32_time_perf_frequency;
global LARGE_INTEGER win32_time_perf_start;
global b32           win32_time_initialized = false;


// MEMORY ---------------------------------------------------

internal DWORD win32_memory_map_protection(u32 flags)  {
    if (flags == PLATFORM_MEMORY_PROTECTION_NONE) {
        return PAGE_NOACCESS;
    } else if (flags == PLATFORM_MEMORY_PROTECTION_READ) {
        return PAGE_READONLY;
    } else if (flags == (PLATFORM_MEMORY_PROTECTION_READ | PLATFORM_MEMORY_PROTECTION_WRITE)) {
        return PAGE_READWRITE;
    } else if (flags == (PLATFORM_MEMORY_PROTECTION_READ | PLATFORM_MEMORY_PROTECTION_EXEC)) {
        return PAGE_EXECUTE_READ;
    } else if (flags == (PLATFORM_MEMORY_PROTECTION_READ | PLATFORM_MEMORY_PROTECTION_WRITE | PLATFORM_MEMORY_PROTECTION_EXEC)) {
        return PAGE_EXECUTE_READWRITE;
    }

    return 0;
}

void* platform_memory_reserve(usize size) {
    void* ptr = VirtualAlloc(null, size, MEM_RESERVE, PAGE_NOACCESS);
    return ptr;
}

void platform_memory_commit(void* ptr, usize size) {
    VirtualAlloc(ptr, size, MEM_COMMIT, PAGE_READWRITE);
}

void platform_memory_decommit(void* ptr, usize size) {
    VirtualFree(ptr, size, MEM_DECOMMIT);
}

void platform_memory_release(void* ptr, usize size) {
    VirtualFree(ptr, 0, MEM_RELEASE);
}

void platform_memory_set_protection(void* ptr, usize size, u32 flags) {
    DWORD oldflags;
    DWORD newflags = win32_memory_map_protection(flags);
    VirtualProtect(ptr, size, newflags, &oldflags);
}

usize platform_memory_get_page_size(void) {
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    return (usize)info.dwPageSize;
}

// FILE IO --------------------------------------------------------------------------------

struct PlatformFile {
    HANDLE handle;
};

internal wchar_t* win32_utf8_to_utf16(const char *utf8) {
    int count = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, utf8, -1, NULL, 0);
    NB_ASSERT(count != 0);

    wchar_t* wide = (wchar_t*)malloc(count * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wide, count);

    return wide;
}

PlatformFile* platform_file_open(const char* filepath) {
    wchar_t *wide_path = win32_utf8_to_utf16(filepath);

    HANDLE h = CreateFileW(
        wide_path, 
        GENERIC_READ | GENERIC_WRITE, 
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        null,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        null);

    free(wide_path);

    if (h == INVALID_HANDLE_VALUE) {
        return NULL;
    }

    PlatformFile *file = malloc(sizeof(PlatformFile));
    file->handle = h;

    return file;
}

usize platform_file_read(PlatformFile* f, void* buffer, usize size) {
    DWORD result;
    ReadFile(f->handle, buffer, size, &result, null);
    return result;
}

usize platform_file_write(PlatformFile* f, void* buffer, usize size) {
    DWORD result;
    WriteFile(f->handle, buffer, size, &result, null);
    return result;
}

b32 platform_file_close(PlatformFile* f) {
    b32 result = CloseHandle(f->handle);
    if (result) {
        free(f);
    }

    return result;
}

b32 platform_file_exists(const char* filepath) {
    wchar_t *wide_path = win32_utf8_to_utf16(filepath);
    DWORD attribs = GetFileAttributesW(wide_path);
    free(wide_path);

    return (attribs != INVALID_FILE_ATTRIBUTES);
}

// TIMING --------------------------------------------------------

internal void win32_time_init(void) {
    QueryPerformanceFrequency(&win32_time_perf_frequency);
    QueryPerformanceCounter(&win32_time_perf_start);
    win32_time_initialized = true;
}

u64 platform_time_get_ticks(void) {
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return (u64)counter.QuadPart;
}

f64 platform_time_get_seconds(void) {
    if (!win32_time_initialized) {
        win32_time_init();
    }

    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);

    u64 elapsed = now.QuadPart - win32_time_perf_start.QuadPart;
    f64 result = (f64)elapsed / (f64)win32_time_perf_frequency.QuadPart;
    
    return result;
}

void platform_time_sleep(u32 ms) {
    Sleep(ms);
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