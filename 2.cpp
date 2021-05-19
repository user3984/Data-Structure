#include <iostream>

using namespace std;

template <class T>
class List;

template <class T>
class Iterator;

template <class T>
class Node {
	friend class List<T>;
	friend class Iterator<T>;
private:
	T data;
	Node<T> * leftLink;
	Node<T> * rightLink;
	Node() :leftLink(NULL), rightLink(NULL) {}
	Node(const T & val) :data(val), leftLink(NULL), rightLink(NULL) {}
};

template <class T>
class List {
	friend class Iterator<T>;
private:
	Node<T> * first;     //指向头节点 
	Node<T> * last;      //指向尾节点 
public:
	List();              //构造函数 
	List(const List & l);//复制构造函数 
	~List();             //析构函数 
	void clear();        //清空链表 
	void insert(const T & val, Iterator<T> & iter);    //在迭代器iter指向的元素后插入值为val的元素 
	void pushback(const T & val);    //在链表末尾加入值为val的元素 
	T remove(Iterator<T> & iter);    //删除迭代器所指向的元素 
	Iterator<T> begin()const;        //返回指向第一个元素的迭代器 
	Iterator<T> end()const;          //返回指向最后一个元素后面的位置的迭代器 
	Iterator<T> find(const T & val)const;    //返回指向值为val元素迭代器
};

template <class T>
class Iterator {
	friend class List<T>;
private:
	Node<T> * ptr;
public:
	Iterator() :ptr(NULL) {}
	Iterator(Node<T> * nodeptr) :ptr(nodeptr) {}   //用Node指针初始化迭代器 
	Iterator(const Iterator<T> & iter) :ptr(iter.ptr) {}
	Iterator<T> & operator++();
	Iterator<T> operator++(int);
	Iterator<T> & operator--();
	Iterator<T> operator--(int);
	T & operator * ();
	Iterator<T> & operator = (const Iterator<T> & x);
	bool operator == (const Iterator<T> & x)const;
	bool operator != (const Iterator<T> & x)const;
};

template <class T>
List<T>::List() {
	first = new Node<T>;
	last = new Node<T>;
	first->rightLink = last;
	last->leftLink = first;
}

template <class T>
List<T>::List(const List & l) {
	Node<T> * p1 = l.first->rightLink;
	first = new Node<T>;
	last = new Node<T>;
	first->rightLink = last;
	last->leftLink = first;
	while (p1 != l.last) {
		Node<T> * q = new Node<T>;
		q->data = (*p1).data;
		q->leftLink = last->leftLink;
		q->leftLink->rightLink = q;
		q->rightLink = last;
		last->leftLink = q;
		p1 = p1->rightLink;
	}
}

template <class T>
List<T>::~List() {
	Node<T> * p = first;
	while (p != last) {
		Node<T> * tmp = p;
		p = p->rightLink;
		delete tmp;
	}
	delete p;
}

template <class T>
void List<T>::clear() {
	Node<T> * p = first->rightLink;
	while (p != last) {
		Node<T> * tmp = p;
		p = p->rightLink;
		delete tmp;
	}
}

template <class T>
void List<T>::insert(const T & val, Iterator<T> & iter) {
	Node<T> * p = iter.ptr;      //插入到p节点后 
	Node<T> * q = new Node<T>;
	q->data = val;
	q->rightLink = p->rightLink;
	q->leftLink = p;
	p->rightLink = q;
	q->rightLink->leftLink = p;
}

template <class T>
void List<T>::pushback(const T & val) {
	Node<T> * q = new Node<T>;     //将q添加到链表末尾 
	q->data = val;
	q->leftLink = last->leftLink;
	q->leftLink->rightLink = q;
	q->rightLink = last;
	last->leftLink = q;
}

template <class T>
T List<T>::remove(Iterator<T> & iter) {
	Node<T> * p = iter.ptr;
	p->leftLink->rightLink = p->rightLink;
	p->rightLink->leftLink = p->leftLink;
	T temp = p->data;
	delete p;
	return temp;
}

template <class T>
Iterator<T> List<T>::begin()const {
	Iterator<T> tempIter(first->rightLink);
	return tempIter;
}

template <class T>
Iterator<T> List<T>::end()const {
	Iterator<T> tempIter(last);
	return tempIter;
}

template <class T>
Iterator<T> List<T>::find(const T & val)const {
	Iterator<T> it;
	for (it = begin(); it != end() && *it != val; ++it);
	return it;
}

template <class T>
Iterator<T> & Iterator<T>::operator++() {
	ptr = ptr->rightLink;
	return *this;
}

template <class T>
Iterator<T> Iterator<T>::operator++(int) {
	Iterator<T> tempIter(ptr);
	ptr = ptr->rightLink;
	return tempIter;
}

template <class T>
Iterator<T> & Iterator<T>::operator--() {
	ptr = ptr->leftLink;
	return *this;
}

template <class T>
Iterator<T> Iterator<T>::operator--(int) {
	Iterator<T> tempIter(ptr);
	ptr = ptr->leftLink;
	return tempIter;
}

template <class T>
T & Iterator<T>::operator * () {
	return ptr->data;
}

template <class T>
Iterator<T> & Iterator<T>::operator = (const Iterator<T> & x) {
	ptr = x.ptr;
}

template <class T>
bool Iterator<T>::operator == (const Iterator<T> & x)const {
	return (ptr == x.ptr) ? true : false;
}

template <class T>
bool Iterator<T>::operator != (const Iterator<T> & x)const {
	return (ptr != x.ptr) ? true : false;
}

List<int> intersection(List<int> l1, List<int> l2) {
	List<int> result;
	Iterator<int> it1 = l1.begin(), it2 = l2.begin();
	while (it1 != l1.end() && it2 != l2.end()) {
		if (*it1 > *it2) {
			++it2;
		}
		else if (*it1 == *it2) {
			result.pushback(*it1);
			++it1;
			++it2;
		}
		else {
			++it1;
		}
	}
	return result;
}

int main() {
	List<int> l1, l2;
	int n, count = 0;
	cout << "输入第一个链表, 以 -1 结束:" << endl;
	cin >> n;
	while (n != -1) {
		l1.pushback(n);
		cin >> n;
	}
	cout << "输入第二个链表, 以 -1 结束:" << endl;
	cin >> n;
	while (n != -1) {
		l2.pushback(n);
		cin >> n;
	}
	List<int> result = intersection(l1, l2);
	Iterator<int> it = result.begin();
	cout << "交集:" << endl;
	for (; it != result.end(); ++it) {
		cout << *it << " ";
		++count;
	}
	if (count == 0) {
		cout << "NULL" << endl;
	}
	return 0;
}