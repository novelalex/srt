#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>
#include <stdint.h>

// ------------------- CONFIG --------------------
#define NB_ASSERTIONS_ENABLED

//---------------------TYPES----------------------

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef unsigned int uint;

typedef size_t usize;
typedef uintptr_t uptr;
typedef ptrdiff_t ptrdiff;

// Floating point types
typedef float f32;
typedef double f64;

// Boolean Types
typedef i8 b8;
typedef i32 b32;

//------------------- MACROS --------------------

#define internal static
#define global static
#define local_persist static

#define true 1
#define false 0
#define null 0

// Platform detection
#if defined(_WIN32) || defined(_WIN64)
#define NB_PLATFORM_WINDOWS 1
#define NB_PLATFORM_NAME "Windows"
#elif defined(__linux__)
#define NB_PLATFORM_LINUX 1
#define NB_PLATFORM_NAME "Linux"
#elif defined(__APPLE__)
#include <TargetConditionals.h>
#if TARGET_OS_MAC
#define NB_PLATFORM_MACOS 1
#define NB_PLATFORM_NAME "macOS"
#endif
#endif

// Architecture detection
#if defined(_M_X64) || defined(__x86_64__)
#define NB_ARCH_X64 1
#define NB_ARCH_NAME "x64"
#elif defined(_M_IX86) || defined(__i386__)
#define NB_ARCH_X86 1
#define NB_ARCH_NAME "x86"
#elif defined(_M_ARM64) || defined(__aarch64__)
#define NB_ARCH_ARM64 1
#define NB_ARCH_NAME "ARM64"
#else
#define NB_ARCH_UNKNOWN 1
#define NB_ARCH_NAME "Unknown"
#endif

// Alignment Detection
#if defined(__AVX512F__)
#define NB_ALIGNMENT 64
#elif defined(__AVX__)
#define NB_ALIGNMENT 32
#elif defined(__SSE__)
#define NB_ALIGNMENT 16
#else
#define NB_ALIGNMENT _Alignof(max_align_t)
#endif

// Compiler detection
#if defined(_MSC_VER)
#define NB_COMPILER_MSVC 1
#define NB_COMPILER_NAME "MSVC"
#define NB_COMPILER_VERSION _MSC_VER
#elif defined(__clang__)
#define NB_COMPILER_CLANG 1
#define NB_COMPILER_NAME "Clang"
#define NB_COMPILER_VERSION                                                    \
  (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
#elif defined(__GNUC__)
#define NB_COMPILER_GCC 1
#define NB_COMPILER_NAME "GCC"
#define NB_COMPILER_VERSION                                                    \
  (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#else
#define NB_COMPILER_UNKNOWN 1
#define NB_COMPILER_NAME "Unknown"
#define NB_COMPILER_VERSION 0
#endif

// Force Inline
#if defined(NB_COMPILER_MSVC)
#define NB_FORCE_INLINE __forceinline

#elif defined(NB_COMPILER_CLANG) || defined(NB_COMPILER_GCC)
#define NB_FORCE_INLINE inline __attribute__((always_inline))

#else
#define NB_FORCE_INLINE inline
#endif

// Build configuration
#ifdef NDEBUG
#define NB_BUILD_RELEASE 1
#define NB_BUILD_NAME "Release"
#else
#define NB_BUILD_DEBUG 1
#define NB_BUILD_NAME "Debug"
#endif

// Assertions

#ifdef NB_ASSERTIONS_ENABLED

#if defined(NB_COMPILER_MSVC)
#define NB_DEBUG_BREAK() __debugbreak()
#elif defined(NB_COMPILER_CLANG) || defined(NB_COMPILER_GCC)
#define NB_DEBUG_BREAK() __builtin_trap()
#else
#define NB_DEBUG_BREAK() abort()
#endif

// TODO:(Novel) Use platform logging instead of stdio
#include <stdio.h>
#include <stdlib.h>

#define NB_ASSERT(expr)                                                        \
  do {                                                                         \
    if (!(expr)) {                                                             \
      fprintf(stderr, "ASSERTION FAILED: %s\n  %s:%d\n", #expr, __FILE__,      \
              __LINE__);                                                       \
      abort();                                                                 \
    }                                                                          \
  } while (0)

#define NB_ASSERT_MSG(expr, msg)                                               \
  do {                                                                         \
    if (!(expr)) {                                                             \
      fprintf(stderr, "ASSERTION FAILED: %s\n  %s\n  %s:%d\n", #expr, msg,     \
              __FILE__, __LINE__);                                             \
      abort();                                                                 \
    }                                                                          \
  } while (0)

#else

#define NB_ASSERT(expr) ((void)0)
#define NB_ASSERT_MSG(expr, msg) ((void)0)

#endif

// Utils

#define NB_MIN(a, b) ((a) < (b) ? (a) : (b))
#define NB_MAX(a, b) ((a) > (b) ? (a) : (b))

#endif // COMMON_H
