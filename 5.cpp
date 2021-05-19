#include <iostream>

template <class T>
class Queue;

template <class T>
class QueueNode {
	friend class Queue<T>;
private:
	T data;
	QueueNode<T> * link;
	QueueNode() :link(NULL) {};
	QueueNode(const T & x) :data(x), link(NULL) {};
};

template <class T>
class Queue {
private:
	QueueNode<T> * front;    //头指针
	QueueNode<T> * rear;     //尾指针
public:
	Queue();   //构造函数
	~Queue();  //析构函数
	void enQueue(const T & x);  //入队
	T deQueue();       //出队
	T getFront();      //访问队头元素
	bool isEmpty();    //判断是否为空
	void clear();      //清空队列
	int length();      //获取队列当前长度
};

template <class T>
Queue<T>::Queue() :front(NULL), rear(NULL) {}

template <class T>
Queue<T>::~Queue() {
	QueueNode<T> * p;
	while (front != NULL) {
		p = front;
		front = front->link;
		delete p;
	}
}

template <class T>
void Queue<T>::enQueue(const T & x) {
	if (isEmpty()) {
		front = rear = new QueueNode<T>(x);
	}
	else {
		rear->link = new QueueNode<T>(x);
		rear = rear->link;
	}
}

template <class T>
T Queue<T>::deQueue() {
	if (isEmpty()) return -1;
	QueueNode<T> * p = front;
	front = front->link;
	T tmp = p->data;
	delete p;
	return tmp;
}

template <class T>
T Queue<T>::getFront() {
	return front->data;
}

template <class T>
bool Queue<T>::isEmpty() {
	return front == NULL;
}

template <class T>
void Queue<T>::clear() {
	QueueNode<T> * p;
	while (front != NULL) {
		p = front;
		front = front->link;
		delete p;
	}
	rear = NULL;
}

template <class T>
int Queue<T>::length() {
	int len = 0;
	QueueNode<T> * p = front;
	while (p != NULL) {
		++len;
		p = p->link;
	}
	return len;
}

int main() {
	int n, number;
	Queue<int> q1, q2;
	std::cin >> n;
	for (int i = 0; i < n; ++i) {
		std::cin >> number;
		if (number % 2) {
			q1.enQueue(number);
		}
		else {
			q2.enQueue(number);
		}
	}
	int tmp = (q1.length() / 2 < q2.length()) ? q1.length() / 2 : q2.length();
	for (int i = 0; i < tmp; ++i) {
		std::cout << q1.deQueue() << ' ';
		std::cout << q1.deQueue() << ' ';
		std::cout << q2.deQueue() << ' ';
	}
	while (!q1.isEmpty()) {
		std::cout << q1.deQueue() << ' ';
	}
	while (!q2.isEmpty()) {
		std::cout << q2.deQueue() << ' ';
	}
	return 0;
}