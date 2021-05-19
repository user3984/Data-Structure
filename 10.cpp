#include <iostream>
#include <ctime>

const int DEFAULTSIZE = 2;

template <class T>
class Array {
private:
	T * ptr;
	int len;
	int maxsize;
	bool resize();
public:
	Array();
	Array(int sz);
	Array(const Array<T> & ar);
	~Array();
	int length() { return len; }
	T & operator[](int n);
	void insert(const T & x, int n);  //将x插入到n之后
	void pushback(const T & x);
	T remove(int n);
	void clear() { len = 0; }
};

template <class T>
Array<T>::Array() :len(0), maxsize(DEFAULTSIZE) {
	ptr = new T[DEFAULTSIZE];
}

template <class T>
Array<T>::Array(int sz) : len(0), maxsize(sz) {
	ptr = new T[sz];
}

template <class T>
Array<T>::Array(const Array<T> & ar) : len(ar.len), maxsize(ar.maxsize) {
	ptr = new T[ar.maxsize];
	T * tmp1 = ptr, *tmp2 = ar.ptr;
	for (int i = 0; i < ar.len; ++i) { *tmp1++ = *tmp2++; }
}

template <class T>
Array<T>::~Array() { delete[]ptr; }

template <class T>
bool Array<T>::resize() {
	T * newptr = new T[maxsize * 2];
	if (newptr == NULL) { return false; }
	for (int i = 0; i < len; ++i) { newptr[i] = ptr[i]; }
	delete[]ptr;
	ptr = newptr;
	maxsize *= 2;
	return true;
}

template <class T>
T & Array<T>::operator[](int n) {
	return ptr[n];
}

template <class T>
void Array<T>::insert(const T & x, int n) {
	if (len >= maxsize) { resize(); }
	for (int i = len; i > n + 1; --i) {
		ptr[i] = ptr[i - 1];
	}
	ptr[n + 1] = x;
	++len;
}

template <class T>
void Array<T>::pushback(const T & x) {
	if (len >= maxsize) { resize(); }
	ptr[len++] = x;
}

template <class T>
T Array<T>::remove(int n) {
	T tmp = ptr[n];
	for (int i = n; i < len - 1; ++i) {
		ptr[i] = ptr[i + 1];
	}
	--len;
	return ptr[n];
}

template <class T>
void swap(T & a, T & b) {
	T tmp = a;
	a = b;
	b = tmp;
}

template <class T>
void insertionSort(Array<T> & l) {
	long long countmove = 0, countcmp = 0;   //仅测试使用，统计交换次数与比较次数
	for (int i = 1; i < l.length(); ++i) {
		int j = 0, tmp = l[i];
		++countmove;
		for (; j < i && l[j] <= tmp; ++j, ++countcmp);  //j为要插入的位置
		for (int k = i; k > j; --k) { l[k] = l[k - 1]; ++countmove; }
		l[j] = tmp;
		++countmove;
	}
	std::cout << "直接插入排序比较次数:\t" << countcmp << std::endl;
	std::cout << "直接插入排序移动次数:\t" << countmove << std::endl;
}

template <class T>
void bubbleSort(Array<T> & l) {
	int exchange = 1;
	long long countmove = 0, countcmp = 0;   //仅测试使用，统计交换次数与比较次数
	for (int i = 0; i < l.length() - 1 && exchange; ++i) {
		exchange = 0;   //假定未交换
		for (int j = l.length() - 1; j > i; --j) {
			++countcmp;
			if (l[j] < l[j - 1]) {
				swap(l[j], l[j - 1]);
				exchange = 1;
				countmove += 3;
			}
		}
	}
	std::cout << "冒泡排序比较次数:\t" << countcmp << std::endl;
	std::cout << "冒泡排序移动次数:\t" << countmove << std::endl;
}

template <class T>
void selectSort(Array<T> & l) {
	long long countmove = 0, countcmp = 0;   //仅测试使用，统计交换次数与比较次数
	for (int i = 0; i < l.length() - 1; ++i) {
		int min = l[i], minpos = i;
		for (int j = i; j < l.length(); ++j) {
			++countcmp;
			if (l[j] < min) {
				min = l[j];
				minpos = j;
			}
		}
		if (minpos != l[i]) {
			swap(l[i], l[minpos]);
			countmove += 3;
		}
	}
	std::cout << "选择排序比较次数:\t" << countcmp << std::endl;
	std::cout << "选择排序移动次数:\t" << countmove << std::endl;
}

template <class T>
void shellSort(Array<T> & l) {
	int countmove = 0, countcmp = 0;   //仅测试使用，统计交换次数与比较次数
	int gap = l.length() / 2;
	while (gap) {
		//做间隔为gap的希尔排序
		for (int i = gap; i < l.length(); ++i) {
			T tmp = l[i];
			++countmove;
			int j = i;
			for (; j >= gap && l[j - gap] > tmp; j -= gap, ++countcmp) {
				l[j] = l[j - gap];
				++countmove;
			}
			l[j] = tmp;
			++countmove;
		}
		gap = gap == 2 ? 1 : (int)(gap / 2.2);
	}
	std::cout << "希尔排序比较次数:\t" << countcmp << std::endl;
	std::cout << "希尔排序移动次数:\t" << countmove << std::endl;
}

template <class T>
void quickSort(Array<T> & l, int left, int right, int & countcmp, int & countmove) {
	//后2个参数仅测试使用
	if (left >= right) { return; }
	T pivot = l[left];
	int pivotpos = left;
	for (int i = left + 1; i <= right; ++i) {
		++countcmp;
		if (l[i] < pivot && ++pivotpos != i) {
			swap(l[pivotpos], l[i]);
			countmove += 3;
		}
	}
	swap(l[left], l[pivotpos]);
	countmove += 3;
	quickSort(l, left, pivotpos - 1, countcmp, countmove);
	quickSort(l, pivotpos + 1, right, countcmp, countmove);
}

template <class T>
void heapSort(Array<T> & l) {
	int countmove = 0, countcmp = 0;   //仅测试使用，统计交换次数与比较次数
	//建立堆
	for (int i = (l.length() - 2) / 2; i >= 0; --i) {
		//向下调整
		T tmp = l[i];
		++countmove;
		int p = i, q = 2 * i + 1;
		if (l[q] < l[q + 1]) { ++q; }  //q指向p较大的孩子
		++countcmp;
		while (q <= (l.length() - 2) / 2 && tmp < l[q]) {
			++countcmp;
			l[p] = l[q];
			++countmove;
			p = q;
			q = 2 * q + 1;
			if (q + 1 < l.length() && l[q] < l[q + 1]) { ++q; }
			++countcmp;
		}
		++countcmp;
		l[p] = tmp;
		++countmove;
	}
	for (int i = 1; i < l.length(); ++i) {
		swap(l[0], l[l.length() - i]);
		countmove += 3;
		//向下调整
		T tmp = l[0];
		++countmove;
		int p = 0, q = 1;
		if (l[q] < l[q + 1]) { ++q; }  //q指向p较大的孩子
		++countcmp;
		while (q <= (l.length() - 2) / 2 && tmp < l[q]) {
			++countcmp;
			l[p] = l[q];
			++countmove;
			p = q;
			q = 2 * q + 1;
			if (q + 1 < l.length() && l[q] < l[q + 1]) { ++q; }
			++countcmp;
		}
		++countcmp;
		l[p] = tmp;
		++countmove;
	}//循环length - 1次，每次取出最大元素置于数组后部
	std::cout << "堆排序比较次数:\t\t" << countcmp << std::endl;
	std::cout << "堆排序移动次数:\t\t" << countmove << std::endl;
}

template <class T>
void merge(Array<T> & l, int lo, int mi, int hi, int & countcmp, int & countmove) {   //后2个参数仅测试时使用
	//归并 l[lo, mi) 与 l[mi, hi)
	int len = mi - lo;
	T * tmp = new T[len];
	int p = 0, q = lo;
	while (p < len) { tmp[p++] = l[q++]; ++countmove; }  //将 l[lo, lo + len) 拷贝至 tmp
	int i = 0, j = mi, k = lo;
	while (i < len && j < hi) {
		if (tmp[i] <= l[j]) { l[k++] = tmp[i++]; }
		else { l[k++] = l[j++]; }
		++countcmp;
		++countmove;
	}
	if (j == hi) {
		while (k < hi) { l[k++] = tmp[i++]; ++countmove; }
	}
	delete[]tmp;
}

template <class T>
void mergeSort(Array<T> & l) {
	int countmove = 0, countcmp = 0;   //仅测试使用，统计交换次数与比较次数
	int len = 1;
	for (; 2 * len <= l.length(); len *= 2) {
		int lo = 0, mi = len, hi = 2 * len;
		while (hi <= l.length()) {
			merge(l, lo, mi, hi, countcmp, countmove);
			lo += 2 * len;
			mi += 2 * len;
			hi += 2 * len;
		}
		if (mi < l.length()) { merge(l, lo, mi, l.length(), countcmp, countmove); }
	}
	merge(l, 0, len, l.length(), countcmp, countmove);
	std::cout << "归并排序比较次数:\t" << countcmp << std::endl;
	std::cout << "归并排序移动次数:\t" << countmove << std::endl;
}

void radixSort(Array<unsigned int> & l) {
	int countmove = 0, countcmp = 0;   //仅测试使用，统计交换次数与比较次数
	unsigned int * bucket = new unsigned int[l.length()];
	int * count = new int[256];    //记录每个桶的元素个数
	int * start = new int[256];
	for (int k = 0; k < 8 * sizeof(unsigned int); k += 8) {
		//每次比较8位
		for (int i = 0; i < 256; ++i) {
			count[i] = 0;
		}
		for (int i = 0; i < l.length(); ++i) {
			++count[(l[i] >> k) & 0xFF];   //从右至左依次取8位
		}
		start[0] = 0;
		for (int i = 1; i < 256; ++i) {
			start[i] = start[i - 1] + count[i - 1];
		}
		for (int i = 0; i < l.length(); ++i) {
			bucket[start[(l[i] >> k) & 0xFF]++] = l[i];  //装入桶中
			++countmove;
		}
		for (int i = 0; i < l.length(); ++i) {
			l[i] = bucket[i];  //倒出桶中元素
			++countmove;
		}
	}
	delete[]bucket;
	delete[]count;
	delete[]start;
	std::cout << "基数排序比较次数:\t" << countcmp << std::endl;
	std::cout << "基数排序移动次数:\t" << countmove << std::endl;
}

int main() {
	srand((unsigned)time(NULL));
	std::cout << "\t排序算法比较" << std::endl;
	std::cout << "============================" << std::endl;
	std::cout << "\t1  冒泡排序" << std::endl;
	std::cout << "\t2  选择排序" << std::endl;
	std::cout << "\t3  直接插入排序" << std::endl;
	std::cout << "\t4  希尔排序" << std::endl;
	std::cout << "\t5  快速排序" << std::endl;
	std::cout << "\t6  堆排序" << std::endl;
	std::cout << "\t7  归并排序" << std::endl;
	std::cout << "\t8  基数排序" << std::endl;
	std::cout << "\t9  退出程序" << std::endl;
	std::cout << "============================" << std::endl;
	std::cout << "请输入要产生随机数的个数" << std::endl;
	int randnum;
	std::cin >> randnum;
	while (randnum <= 0) {
		std::cout << "输入必须为正整数, 请重新输入: ";
		std::cin >> randnum;
	}
	Array<unsigned int> randlist(randnum);
	for (int i = 0; i < randnum; ++i) {
		randlist.pushback(rand());
	}
	while (1) {
		int option;
		clock_t start, end;
		std::cout << "请选择排序算法:\t\t";
		std::cin >> option;
		Array<unsigned int> list = randlist;
		int countcmp = 0, countmove = 0;
		switch (option)
		{
		case 1:
			start = clock();
			bubbleSort(list);
			end = clock();
			std::cout << "冒泡排序所用时间:\t" << end - start << "ms" << std::endl << std::endl;
			break;
		case 2:
			start = clock();
			selectSort(list);
			end = clock();
			std::cout << "选择排序所用时间:\t" << end - start << "ms" << std::endl << std::endl;
			break;
		case 3:
			start = clock();
			insertionSort(list);
			end = clock();
			std::cout << "直接插入排序所用时间:\t" << end - start << "ms" << std::endl << std::endl;
			break;
		case 4:
			start = clock();
			shellSort(list);
			end = clock();
			std::cout << "希尔排序所用时间:\t" << end - start << "ms" << std::endl << std::endl;
			break;
		case 5:
			start = clock();
			quickSort(list, 0, randnum - 1, countcmp, countmove);
			end = clock();
			std::cout << "快速排序比较次数:\t" << countcmp << std::endl;
			std::cout << "快速排序移动次数:\t" << countmove << std::endl;
			std::cout << "快速排序所用时间:\t" << end - start << "ms" << std::endl << std::endl;
			break;
		case 6:
			start = clock();
			heapSort(list);
			end = clock();
			std::cout << "堆排序所用时间:\t\t" << end - start << "ms" << std::endl << std::endl;
			break;
		case 7:
			start = clock();
			mergeSort(list);
			end = clock();
			std::cout << "归并排序所用时间:\t" << end - start << "ms" << std::endl << std::endl;
			break;
		case 8:
			start = clock();
			radixSort(list);
			end = clock();
			std::cout << "基数排序所用时间:\t" << end - start << "ms" << std::endl << std::endl;
			break;
		case 9:
			return 0;
		default:
			std::cout << "输入有误, 请重新输入!" << std::endl;
			break;
		}
	}
}