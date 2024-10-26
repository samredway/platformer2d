#pragma once

#include <cstdlib>   // For std::abort
#include <iostream>  // for std::cerr

namespace platformer2d {

#define CHECK(condition, errstring)                                    \
  do {                                                                 \
    if (!(condition)) {                                                \
      std::cerr << "Check failed: " << errstring << " in " << __FILE__ \
                << " at line " << __LINE__ << std::endl;               \
      std::abort();                                                    \
    }                                                                  \
  } while (false)

#ifndef NDEBUG
#define DCHECK(condition) CHECK(condition)
#else
#define DCHECK(condition) ((void)0)  // No-op in release builds
#endif

#define PANIC(err_message)                                           \
  do {                                                               \
    std::cerr << "PANIC: " << err_message << " in file " << __FILE__ \
              << " at line " << __LINE__ << std::endl;               \
    std::abort();                                                    \
  } while (false)

#ifndef NDEBUG
#define DLOG(message)                               \
  do {                                              \
    std::cout << "DEBUG: " << message << std::endl; \
  } while (false)
#else
#define DLOG(message) ((void)0)
#endif

}  // namespace platformer2d
