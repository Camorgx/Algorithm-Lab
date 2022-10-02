#include <vector>

using std::vector;

namespace heap {
	inline int left(int i) { return i * 2 + 1; }

	inline int right(int i) { return i * 2 + 2; }

	inline int parent(int i) { return (i - 1) / 2; }

	class Heap {
		vector<int>& data;
	public:
		int size = 0;
		int& operator[](int i) { return data[i]; }
		Heap(vector<int>& arr);
	};

	void max_heapify(Heap& heap, int i) {
		int l = left(i), r = right(i);
		int largest;
		if (l < heap.size && heap[l] > heap[i])
			largest = l;
		else largest = i;
		if (r < heap.size && heap[r] > heap[largest])
			largest = r;
		if (largest != i) {
			std::swap(heap[i], heap[largest]);
			max_heapify(heap, largest);
		}
	}

	// build heap from an array
	Heap::Heap(vector<int>& arr) : data(arr) {
		size = static_cast<int>(data.size());
		for (int i = static_cast<int>(data.size()) / 2 - 1; i >= 0; --i)
			max_heapify(*this, i);
	}

	void sort(vector<int>& arr) {
		Heap heap(arr);
		for (int i = static_cast<int>(arr.size()) - 1; i >= 1; --i) {
			std::swap(arr[0], arr[i]);
			--heap.size;
			max_heapify(heap, 0);
		}
	}
}

void heap_sort(vector<int>& arr) {
	heap::sort(arr);
}