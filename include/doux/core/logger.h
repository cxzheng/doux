#pragma once

#include "doux/core/platform.h"
#include "fmt/color.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

NAMESPACE_BEGIN(doux)
NAMESPACE_BEGIN(log)

void init(const char *filename = nullptr);

[[nodiscard]] DOUX_ATTR(returns_nonnull) inline spdlog::logger *logger() {
  extern std::shared_ptr<spdlog::logger> _glogger;
  if DOUX_UNLIKELY (_glogger == nullptr) {
    init();
  }
  return _glogger.get();
}

NAMESPACE_END(log)

// log_debug
template <typename FormatString, typename... Args>
DOUX_ALWAYS_INLINE void log_debug(const FormatString &fmt_str, Args &&...args) {
  log::logger()->debug(fmt_str, std::forward<Args>(args)...);
}

// log_info
template <typename FormatString, typename... Args>
DOUX_ALWAYS_INLINE void log_info(const FormatString &fmt_str, Args &&...args) {
  log::logger()->info(fmt_str, std::forward<Args>(args)...);
}

// log_warn
template <typename FormatString, typename... Args>
DOUX_ALWAYS_INLINE void log_warn(const FormatString &fmt_str, Args &&...args) {
  log::logger()->warn(fmt_str, std::forward<Args>(args)...);
}

// log_error
template <typename FormatString, typename... Args>
DOUX_ALWAYS_INLINE void log_err(const FormatString &fmt_str, Args &&...args) {
  log::logger()->error(fmt_str, std::forward<Args>(args)...);
}

template <typename FormatString, typename... Args>
DOUX_ALWAYS_INLINE void console_warn(const FormatString &fmt_str,
                                    Args &&...args) {
#ifdef DOUX_ENABLE_STDOUT
  fmt::print(fmt::fg(fmt::color::yellow), "WARNING: ");
  fmt::print(fmt_str, std::forward<Args>(args)...);
#endif
}

template <typename FormatString, typename... Args>
DOUX_ALWAYS_INLINE void console_err(const FormatString &fmt_str,
                                   Args &&...args) {
#ifdef DOUX_ENABLE_STDOUT
  fmt::print(fmt::fg(fmt::color::red), "ERROR: ");
  fmt::print(fmt_str, std::forward<Args>(args)...);
#endif
}

NAMESPACE_END(doux)
