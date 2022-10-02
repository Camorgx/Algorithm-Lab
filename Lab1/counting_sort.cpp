#include <vector>

using std::vector;

// k: all items in the array should be between 0 and k
void counting_sort(vector<int>& arr) {
	const int k = (1 << 15) - 1;
	size_t n = arr.size();
	vector<int> b(n), c(k + 1);
	for (int x : arr) ++c[x];
	for (size_t i = 1; i <= k; ++i)
		c[i] += c[i - 1];
	for (auto it = arr.rbegin(); it != arr.rend(); ++it) {
		b[(size_t)(c[*it]) - 1] = *it;
		--c[*it];
	}
	arr = b;
}