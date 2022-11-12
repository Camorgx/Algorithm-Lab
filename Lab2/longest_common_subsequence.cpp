#include "longest_common_subsequence.h"

namespace longest_common_subsequence {
	int solve(const std::string& x, const std::string& y,
		std::vector<std::vector<char>>& b) {
		size_t m = x.length(), n = y.length();
		std::vector<std::vector<int>> c;
		b.clear();
		for (size_t i = 0; i <= m; ++i) {
			b.emplace_back(n + 1);
			c.emplace_back(n + 1);
		}
		for (size_t i = 1; i <= m; ++i) {
			for (size_t j = 1; j <= n; ++j) {
				if (x[i - 1] == y[j - 1]) {
					c[i][j] = c[i - 1][j - 1] + 1;
					b[i][j] = 0; // Left Up
				}
				else if (c[i - 1][j] >= c[i][j - 1]) {
					c[i][j] = c[i - 1][j];
					b[i][j] = 1; // Up
				}
				else {
					c[i][j] = c[i][j - 1];
					b[i][j] = 2; // Left
				}
			}
		}
		return c[m][n];
	}

	std::string _get_result(const std::vector<std::vector<char>>& b,
		const std::string& x, size_t i, size_t j) {
		if (i == 0 || j == 0) return "";
		if (b[i][j] == 0) // Left Up
			return _get_result(b, x, i - 1, j - 1) + x[i - 1];
		else if (b[i][j] == 1) // Up
			return _get_result(b, x, i - 1, j);
		else return _get_result(b, x, i, j - 1);
	}

	std::string get_result(const std::vector<std::vector<char>>& b,
		const std::string& x, const std::string& y) {
		return _get_result(b, x, x.length(), y.length());
	}
}
