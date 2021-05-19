#include <iostream>

template <class T>
class MinHeap {
private:
	T * ptr;      //动态数组指针
	int maxSize;  //数组最大容量
	int currentSize;   //当前大小
	bool resize();     //扩容, 申请更大的空间
	void filterDown(int start);  //向下调整
	void filterUp(int start);    //向上调整
public:
	MinHeap(int size_ = 10);     //构造函数
	MinHeap(T * p, int length);  //用数组初始化堆
	~MinHeap() { delete[]ptr; }  //析构函数
	bool insert(const T & x);    //插入元素
	bool removeMin(T & min);     //删除堆顶元素
	bool isEmpty() { return currentSize == 0; }  //判断是否为空
	int size() { return currentSize; }   //当前大小
	T getMin() { return ptr[0]; }        //访问堆顶元素
};

template <class T>
MinHeap<T>::MinHeap(int size_) :maxSize(size_), currentSize(0) {
	ptr = new T[maxSize];
	if (ptr == NULL) {
		std::cerr << "存储分配错误!" << std::endl;
		maxSize = 0;
		return;
	}
}

template <class T>
MinHeap<T>::MinHeap(T * p, int length) : maxSize(length + 10), currentSize(length) {
	ptr = new T[maxSize];
	if (ptr == NULL) {
		std::cerr << "存储分配错误!" << std::endl;
		maxSize = currentSize = 0;
		return;
	}
	for (int i = 0; i < length; ++i) {
		ptr[i] = p[i];
	}
	int j = (currentSize - 2) / 2;  //最后的非叶结点
	for (; j > 0; --j) {
		filterDown(j);
	}
}

template <class T>
bool MinHeap<T>::resize() {
	maxSize = currentSize * 2;
	T * newptr = new T[maxSize];
	if (newptr == NULL) {
		std::cerr << "存储分配错误!" << std::endl;
		return false;
	}
	for (int i = 0; i < currentSize; ++i) {
		newptr[i] = ptr[i];
	}
	delete[]ptr;
	ptr = newptr;
	return true;
}

template <class T>
void MinHeap<T>::filterDown(int start) {
	T tmp = ptr[start];
	int i = start, j = 2 * i + 1;  //j指向i的左孩子
	while (j < currentSize) {
		if (j < currentSize - 1 && ptr[j + 1] < ptr[j]) {
			++j;   //j指向较小孩子
		}
		if (tmp < ptr[j])break;
		else {
			ptr[i] = ptr[j];
			i = j;
			j = 2 * i + 1;
		}
	}
	ptr[i] = tmp;
}

template <class T>
void MinHeap<T>::filterUp(int start) {
	T tmp = ptr[start];
	int i = start, j = (i - 1) / 2;   //j指向i的双亲
	while (i > 0) {
		if (tmp < ptr[j]) {
			ptr[i] = ptr[j];
			i = j;
			j = (i - 1) / 2;
		}
		else break;
	}
	ptr[i] = tmp;
}

template <class T>
bool MinHeap<T>::insert(const T & x) {
	if (currentSize >= maxSize) {
		if (!resize()) return false;
	}
	ptr[currentSize++] = x;
	filterUp(currentSize - 1);
	return true;
}

template <class T>
bool MinHeap<T>::removeMin(T & min) {
	if (currentSize == 0) {
		std::cerr << "不能从空堆中删除元素!" << std::endl;
		return false;
	}
	min = ptr[0];
	ptr[0] = ptr[currentSize - 1];
	--currentSize;
	filterDown(0);
	return true;
}

int main() {
	int n, len, cost = 0, x, y;
	std::cin >> n;
	MinHeap<int> heap(n);
	for (int i = 0; i < n; ++i) {
		std::cin >> len;
		heap.insert(len);
	}
	while (heap.size() > 1) {
		heap.removeMin(x);  //最小
		heap.removeMin(y);  //次小
		cost += (x + y);
		heap.insert(x + y);
	}
	std::cout << cost << std::endl;
	return 0;
}