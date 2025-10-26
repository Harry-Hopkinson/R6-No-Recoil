#pragma once

/**
 * @file CompilerUtils.h
 * @brief Cross-platform compiler-specific utilities and macros
 */

// Cross-platform force inline macro
#if defined(_MSC_VER)
    // MSVC
    #define FORCE_INLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
    // GCC or Clang
    #define FORCE_INLINE inline __attribute__((always_inline))
#else
    // Fallback to standard inline
    #define FORCE_INLINE inline
#endif
