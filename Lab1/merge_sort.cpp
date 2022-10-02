#include <vector>
#include <limits>

using std::vector;
using std::numeric_limits;

namespace merge {
    // merge two sorted parts
    // arr[l, q]: the first part
    // arr[q + 1, r]: the second part
    void merge(vector<int>& arr, int l, int q, int r) {
        vector<int> L(arr.begin() + l, arr.begin() + q + 1);
        vector<int> R(arr.begin() + q + 1, arr.begin() + r + 1);
        L.push_back(numeric_limits<int>::max());
        R.push_back(numeric_limits<int>::max());
        auto pL = L.begin(), pR = R.begin();
        for (int i = l; i <= r; ++i)
            arr[i] = (*pL <= *pR) ? *pL++ : *pR++;
    }

    // merge sort recursive function
    void sort(vector<int>& arr, int l, int r) {
        if (l < r) {
            int q = (l + r) / 2;
            sort(arr, l, q);
            sort(arr, q + 1, r);
            merge(arr, l, q, r);
        }
    }
}

void merge_sort(vector<int>& arr) {
    merge::sort(arr, 0, static_cast<int>(arr.size()) - 1);
}
