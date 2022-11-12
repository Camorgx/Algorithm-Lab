#pragma once

#include <string>
#include <vector>

namespace longest_common_subsequence {
	int solve(const std::string& x, const std::string& y,
		std::vector<std::vector<char>>& b);

	std::string get_result(const std::vector<std::vector<char>>& b,
		const std::string& x, const std::string& y);
}
