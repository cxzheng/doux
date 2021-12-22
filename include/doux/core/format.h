#pragma once

#include <fmt/format.h>

#include "svec.h"

#define FMT_FORMATTER(type)                                  \
  template <typename T_, size_t D_>                          \
  struct fmt::formatter<type<T_, D_>> {                      \
    fmt::formatter<T_> formatter_;                           \
    constexpr auto parse(format_parse_context& ctx) {        \
      return formatter_.parse(ctx);                          \
    }                                                        \
    template <typename FormatContext>                        \
    auto format(const type<T_, D_>& p, FormatContext& ctx) { \
      format_to(ctx.out(), "(");                             \
      ctx.advance_to(formatter_.format(p[0], ctx));          \
      for (size_t i = 1; i < D_; ++i) {                      \
        format_to(ctx.out(), ", ");                          \
        ctx.advance_to(formatter_.format(p[i], ctx));        \
      }                                                      \
      return format_to(ctx.out(), ")");                      \
    }                                                        \
  };

FMT_FORMATTER(doux::SVector)
//FMT_FORMATTER(doux::Point)

#undef FMT_FORMATTER