#pragma once

#include "doux/core/platform.h"

NAMESPACE_BEGIN(doux)

template <typename T>
inline void sort_triple(T& a, T& b, T& c) {
  if ( b < a ) std::swap(a, b);
  if ( c < a ) std::swap(a, c);
  if ( c < b ) std::swap(b, c);
}

NAMESPACE_END(doux)