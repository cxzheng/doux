//******************************************************************************
// logger.cpp -- This file is part of Doux, a realtime softbody simulation library
//
// Copyright (C) 2021 Changxi Zheng <cxz@cs.columbia.edu>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
//******************************************************************************

#include "doux/core/logger.h"

#include "spdlog/cfg/env.h"
#include "spdlog/sinks/basic_file_sink.h"

NAMESPACE_BEGIN(doux::log)

std::shared_ptr<spdlog::logger> _glogger;

/*
 * Initialize the logger. This function is supposed to be called only once 
 * during the entire lifetime of a program.
 */
void init(const char* filename) {
  if (_glogger != nullptr) [[unlikely]] {
    throw std::runtime_error("The global logger has already been initialized");
  }
#ifdef DOUX_ENABLE_STDOUT
  fmt::print("+ Initializing the global logger ...\n");
#endif

  spdlog::cfg::load_env_levels();
  if (filename == nullptr) {
    _glogger = spdlog::stdout_color_st("doux");
  } else {
    _glogger = spdlog::basic_logger_st("doux", filename);
  }
  _glogger->set_pattern("[%D %T] [%^%l%$] %v");
}

NAMESPACE_END(doux::log)
