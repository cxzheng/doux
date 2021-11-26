#pragma once

#include <variant>

#include "doux/doux.h"

NAMESPACE_BEGIN(doux)

// --- the overload pattern for visitors of variants ---
template <class... Ts>
struct overload : Ts... {
  using Ts::operator()...;
};
template <class... Ts>
overload(Ts...) -> overload<Ts...>;

NAMESPACE_END(doux)