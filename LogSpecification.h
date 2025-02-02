#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <list>

namespace mystd
{
	template <typename T>
	std::ostream& operator << (std::ostream &os, const std::vector<T>& container)
	{
		const size_t size = container.size();
		size_t number = 0;
		os << '[';
		for (auto&& item: container) {
			os << item;
			if (++number != size) {
				os << ',';
			}
		}
		os << ']';
		return os;
	}

	template <typename T>
	std::ostream& operator << (std::ostream &os, const std::list<T>& container)
	{
		const size_t size = container.size();
		size_t number = 0;
		os << '[';
		for (auto&& item: container) {
			os << item;
			if (++number != size) {
				os << ',';
			}
		}
		os << ']';
		return os;
	}
}
