#pragma once

#include <string>
#include <vector>

namespace matrix_chain_order {
	int64_t solve(const std::vector<int>& p,
		std::vector<std::vector<int64_t>>& m, std::vector<std::vector<int>>& s);
	
	std::string get_result(const std::vector<std::vector<int>>& s);
}
