// This file is part of TetWild, a software for generating tetrahedral meshes.
//
// Copyright (C) 2018 Jeremie Dumas <jeremie.dumas@ens-lyon.org>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
//
// Created by Jeremie Dumas on 09/04/18.
//

#include <tetwild/Logger.h>
#include <tetwild/DisableWarnings.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <tetwild/EnableWarnings.h>
#include <memory>
#include <mutex>
#include <iostream>

namespace tetwild {

std::shared_ptr<spdlog::logger> Logger::logger_;

void Logger::init(bool use_cout, const std::string &filename, bool truncate) {
	std::vector<spdlog::sink_ptr> sinks;
	if (use_cout) {
		sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
	}
	if (!filename.empty()) {
		sinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(filename, truncate));
	}

	logger_ = std::make_shared<spdlog::logger>("tetwild", sinks.begin(), sinks.end());
	logger_->set_level(spdlog::level::trace);
}

} // namespace tetwild