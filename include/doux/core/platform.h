#pragma once

#include "doux/doux.h"

// clang-format off

#ifndef __cplusplus
#   error doux is a C++ library.
#endif

// Macro to indicate 64 bit mode
#if ( defined(__x86_64__) || defined(_M_AMD64) || defined(_M_X64) || defined(__amd64) ) 
#   define DOUX_X86_64 1
#endif

#if (defined(__i386__) || defined(_M_IX86)) && !defined(DOUX_X86_64)
#   define DOUX_X86_32 1
#endif

#ifdef __clang__
#   define DOUX_CLANG __clang_major__
#else
#   define DOUX_CLANG 0
#endif

#if defined(__GNUC__) && !DOUX_CLANG 
#   define DOUX_GCC __GNUC__
#else
#   define DOUX_GCC 0
#endif

//#====================================================================================================================
// Flags for vectorization
#if defined(__AVX__)
#   define DOUX_X86_AVX 1
// we don't use double4_t, instead we use Vector4d
#if 0
typedef double double4_t __attribute__ ((vector_size (4 * sizeof(double))));  // This should be okay for Clang and GCC
#endif
#endif

#if defined(__AVX2__)
#   define DOUX_X86_AVX2 1
#endif

#if defined(__SSE4_2__)
#   define DOUX_X86_SSE42 1
#endif

#if defined(__FMA__)
#   define DOUX_X86_FMA 1
#endif

// The following values of INSTRSET are currently defined:
// 2:  SSE2
// 3:  SSE3
// 4:  SSSE3
// 5:  SSE4.1
// 6:  SSE4.2
// 7:  AVX
// 8:  AVX2
// 9:  AVX512F
// 10: AVX512BW/DQ/VL
// In the future, DOUX_INSTRSET = 11 may include AVX512VBMI and AVX512VBMI2, but this
// decision cannot be made before the market situation for CPUs with these
// instruction sets is better known

// Find instruction set from compiler macros if DOUX_INSTRSET is not defined.
// Note: Some of these macros are not defined in Microsoft compilers
#ifndef DOUX_INSTRSET
#   if defined ( __AVX512VL__ ) && defined ( __AVX512BW__ ) && defined ( __AVX512DQ__ )
#       define DOUX_INSTRSET 10
#   elif defined ( __AVX512F__ ) || defined ( __AVX512__ )
#       define DOUX_INSTRSET 9
#   elif defined ( __AVX2__ )
#       define DOUX_INSTRSET 8
#   elif defined ( __AVX__ )
#       define DOUX_INSTRSET 7
#   elif defined ( __SSE4_2__ )
#       define DOUX_INSTRSET 6
#   elif defined ( __SSE4_1__ )
#       define DOUX_INSTRSET 5
#   elif defined ( __SSSE3__ )
#       define DOUX_INSTRSET 4
#   elif defined ( __SSE3__ )
#       define DOUX_INSTRSET 3
#   elif defined ( __SSE2__ ) || defined ( __x86_64__ )
#       define DOUX_INSTRSET 2
#   elif defined ( __SSE__ )
#       define DOUX_INSTRSET 1
#   elif defined ( _M_IX86_FP )           // Defined in MS compiler. 1: SSE, 2: SSE2
#       define DOUX_INSTRSET _M_IX86_FP
#   else
#       define DOUX_INSTRSET 0
#   endif // instruction set defines
#endif // DOUX_INSTRSET

//#====================================================================================================================
//# CLANG
//#====================================================================================================================

#if DOUX_CLANG
#   define DOUX_PUSH_WARNINGS                      _Pragma("clang diagnostic push")
#   define DOUX_DISABLE_UNKNOWN_ATTR_WARNINGS      _Pragma("clang diagnostic ignored \"-Wunknown-attributes\"")
#   define DOUX_POP_WARNINGS                       _Pragma("clang diagnostic pop")

#   define DOUX_ASSUME(cond)       __builtin_assume(cond)
#   define DOUX_UNREACHABLE        __builtin_unreachable()
#   define DOUX_ATTR(...)          __attribute__((__VA_ARGS__))
#   define DOUX_LIKELY(...)       (__builtin_expect(!!(__VA_ARGS__), 1))
#   define DOUX_UNLIKELY(...)     (__builtin_expect(!!(__VA_ARGS__), 0))

#   ifdef __has_attribute
#       if !defined(DOUX_ALWAYS_INLINE) && __has_attribute(always_inline)
#           define DOUX_ALWAYS_INLINE __attribute__((__always_inline__)) inline
#       endif
#       if !defined(DOUX_NEVER_INLINE) && __has_attribute(noinline)
#           define DOUX_NEVER_INLINE __attribute__((__noinline__))
#       endif
#       if !defined(DOUX_TRIVIAL_ABI) && __has_attribute(trivial_abi)
#           define DOUX_TRIVIAL_ABI __attribute__((__trivial_abi__))
#       endif
#   endif
#endif

//#====================================================================================================================
//# GCC
//#====================================================================================================================

#if DOUX_GCC
#   define DOUX_PUSH_WARNINGS        _Pragma("GCC diagnostic push")
#   define DOUX_POP_WARNINGS         _Pragma("GCC diagnostic pop")

#   define DOUX_ALWAYS_INLINE        __attribute__((__always_inline__)) inline
#   define DOUX_NEVER_INLINE         __attribute__((__noinline__))
#   define DOUX_UNREACHABLE          __builtin_unreachable()
#   define DOUX_ATTR(...)            __attribute__((__VA_ARGS__))
#   define DOUX_LIKELY(...)         (__builtin_expect(!!(__VA_ARGS__), 1))
#   define DOUX_UNLIKELY(...)       (__builtin_expect(!!(__VA_ARGS__), 0))
#endif

#include <fmt/core.h>
#include <fmt/color.h>

#define UNIMPLEMENTED                                                   \
    {                                                                   \
      fmt::print(stderr, fmt::fg(fmt::color::red), "Reached unimplemented code"); \
      fmt::print(stderr, " at {0:s}:L.{1:d}\n", __FILE__, __LINE__);    \
      exit(-3);                                                         \
      DOUX_UNREACHABLE;                                                 \
    }

#define SHOULD_NEVER_REACH(x)                                           \
    {                                                                   \
      fmt::print(stderr, fmt::fg(fmt::color::red), "Should never reach code"); \
      fmt::print(stderr, " at {0:s}:L.{1:d}\n", __FILE__, __LINE__);    \
      exit(x);                                                          \
      DOUX_UNREACHABLE;                                                 \
    }
// clang-format on

// Get rid of likely/unlikely, use [[likely/unlikely]] in C++ 20
//#   define DOUX_LIKELY(...)      (__builtin_expect(!!(__VA_ARGS__), 1))
//#   define DOUX_UNLIKELY(...)    (__builtin_expect(!!(__VA_ARGS__), 0))
