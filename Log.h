#pragma once

#include <iostream>
#include "LogSpecification.h"

namespace mystd
{
	template <typename Arg>
	void ToStreamArgs(std::ostringstream& oss, Arg&& arg) {
		oss << arg;
	}

	template <typename Arg>
	void ToStreamArgs(std::ostringstream& oss, std::string_view&& format, Arg&& arg) {
		const size_t pos = format.find("{}");
		if (pos == format.npos) {
			oss << format << arg;
			return;
		}
		oss << format.substr(0, pos) << arg << format.substr(pos + 2, format.size() - pos);
	}

	template <typename Arg, typename... Args>
	void ToStreamArgs(std::ostringstream& oss, std::string_view&& format, Arg&& arg, Args&&... args) {
		const size_t pos = format.find("{}");
		if (pos == format.npos) {
			oss << format << arg;
			ToStreamArgs(oss, "", std::forward<Args>(args)...);
			return;
		}
		oss << format.substr(0, pos) << arg;
		ToStreamArgs(oss, format.substr(pos + 2, format.size() - pos), std::forward<Args>(args)...);
	}

	template <typename... Args>
	std::string ToString(Args&&... args) {
		std::ostringstream oss;
		ToStreamArgs(oss, std::forward<Args>(args)...);
		return oss.str();
	}

	template <typename... Args>
	void Log(Args&&... args) {
		const std::string text = ToString(std::forward<Args>(args)...);
		std::cout << text << std::endl;

	#ifdef _DEBUG
		_CrtDbgReport(_CRT_WARN, NULL, 0, NULL, "LOG: %s\n", text.c_str());
	#endif // DEBUG
	}
}

#define LOG mystd::Log
#define TO_STRING mystd::ToString
