#include "matrix_chain_order.h"

#include <limits>
#include <format>

namespace matrix_chain_order {
	int64_t solve(const std::vector<int>& p, 
		std::vector<std::vector<int64_t>>& m, std::vector<std::vector<int>>& s) {
		size_t n = p.size() - 1;
		m.clear(); s.clear();
		for (int i = 0; i <= n; ++i) {
			m.emplace_back(n + 1);
			s.emplace_back(n + 1);
		}
		for (int l = 2; l <= n; ++l) {
			for (size_t i = 1; i <= n - l + 1; ++i) {
				size_t j = i + l - 1;
				m[i][j] = std::numeric_limits<int64_t>::max() >> 1; // 任何一个足够大的值
				for (size_t k = i; k < j; ++k) {
					int64_t q = m[i][k] + m[k + 1][j] 
						+ static_cast<int64_t>(p[i - 1]) * p[k] * p[j];
					if (q < m[i][j]) {
						m[i][j] = q;
						s[i][j] = static_cast<int>(k);
					}
				}
			}
		}
		return m[1][n];
	}

	std::string _get_result(const std::vector<std::vector<int>>& s,
		int i, int j) {
		if (i == j) return std::format("A{0}", i);
		return std::format("({0}{1})", _get_result(s, i, s[i][j]),
			_get_result(s, s[i][j] + 1, j));
	}

	std::string get_result(const std::vector<std::vector<int>>& s) {
		return _get_result(s, 1, static_cast<int>(s.size() - 1));
	}
}
