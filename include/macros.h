#pragma once

#include <cstdlib>  // For std::abort
#include <iostream>

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

#ifndef NDEBUG
#define DLOG(message)                  \
  do {                                 \
    std::cout << message << std::endl; \
  } while (false)
#else
#define DLOG(message) ((void)0)
#endif
