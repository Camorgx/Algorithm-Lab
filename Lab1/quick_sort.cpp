#include <vector>

using std::vector;

namespace quick {
	int partition(vector<int>& arr, int l, int r) {
		int pos = l - 1;
		for (int i = l; i < r; ++i) 
			if (arr[i] <= arr[r])
				std::swap(arr[++pos], arr[i]);
		std::swap(arr[++pos], arr[r]);
		return pos;
	}

	void sort(vector<int>& arr, int l, int r) {
		if (l < r) {
			int q = partition(arr, l, r);
			sort(arr, l, q - 1);
			sort(arr, q + 1, r);
		}
	}
}

void quick_sort(vector<int>& arr) {
	quick::sort(arr, 0, static_cast<int>(arr.size()) - 1);
}
