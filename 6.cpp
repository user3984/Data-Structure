#include <iostream>
#include <string>
using namespace std;

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
	QueueNode<T> * front;
	QueueNode<T> * rear;
public:
	Queue();
	~Queue();
	void enQueue(const T & x);
	T deQueue();
	T getFront();
	bool isEmpty();
	void clear();
	int length();
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

template <class T>
class Tree;

template <class T>
class TreeNode {
	friend class Tree<T>;
private:
	T data;
	TreeNode<T> * firstChild;
	TreeNode<T> * nextSibling;
	TreeNode() :firstChild(NULL), nextSibling(NULL) {};
	TreeNode(const T & x) :data(x), firstChild(NULL), nextSibling(NULL) {};
};

template <class T>
class Tree {
private:
	TreeNode<T> * root;     //指向根结点
	TreeNode<T> * current;  //指向当前结点
	void RemoveSubtree(TreeNode<T> * p);  //删除结点p的子树
public:
	Tree(const T & rootVal);   //构造函数
	~Tree();                   //析构函数
	void Root();               //使根结点成为当前结点
	void InsertChild(const T & x);      //在当前结点插入孩子
	void InsertSibling(const T & x);    //在当前结点插入兄弟
	bool Parent();             //使双亲结点成为当前结点
	bool FirstChild();         //使长子结点成为当前结点
	bool NextSibling();        //使下一兄弟成为当前结点
	bool Find(const T & x);    //查找值为x的结点，并使其成为当前结点
	void RemoveSubtree();      //删除当前结点的子树
	T & Current();             //获取当前结点的值
};

template <class T>
Tree<T>::Tree(const T & rootVal) {
	root = new TreeNode<T>(rootVal);
	current = root;
}

template <class T>
Tree<T>::~Tree() {
	RemoveSubtree(root);
	delete root;
}

template <class T>
void Tree<T>::RemoveSubtree(TreeNode<T> * p) {
	TreeNode<T> * q = p;
	if (p == NULL || p->firstChild == NULL) return;
	p = p->firstChild;
	while (p != NULL) {
		RemoveSubtree(p);
		TreeNode<T> * tmp = p;
		p = p->nextSibling;
		delete tmp;
	}  //逐个删除孩子的子树, 并释放孩子结点
	q->firstChild = NULL; //孩子已被全部删除, 长子置为空
}

template <class T>
void Tree<T>::InsertSibling(const T & x) {
	if (current == NULL || current == root) {
		cout << "插入失败" << endl;
		return;
	}
	while (current->nextSibling != NULL) {
		current = current->nextSibling;
	}
	current->nextSibling = new TreeNode<T>(x);
	current = current->nextSibling;
}


template <class T>
void Tree<T>::Root() {
	current = root;
}

template <class T>
void Tree<T>::InsertChild(const T & x) {
	if (current == NULL) {
		cout << "插入失败" << endl;
		return;
	}
	if (current->firstChild == NULL) {
		current->firstChild = new TreeNode<T>(x);
	}
	else {
		TreeNode<T> * p = current->firstChild;
		while (p->nextSibling != NULL) {
			p = p->nextSibling;
		}
		p->nextSibling = new TreeNode<T>(x);
	}
}

template <class T>
bool Tree<T>::Parent() {
	if (current == NULL || current == root) {
		current = NULL;
		return 0;
	}
	Queue<TreeNode<T> *> q;
	q.enQue(root);
	while (!q.isEmpty) {
		TreeNode<T> * p = q.deQueue(), x;
		x = p->firstChild;
		while (x) {
			if (x == current) {
				current = p;
				return true;
			}
			q.enQueue(x);
			x = x->nextSibling;
		}
	}
	return false;
}

template <class T>
bool Tree<T>::FirstChild() {
	if (current == NULL || current->firstChild == NULL) return false;
	else {
		current = current->firstChild;
		return true;
	}
}

template <class T>
bool Tree<T>::NextSibling() {
	if (current == NULL || current->nextSibling == NULL) return false;
	else {
		current = current->nextSibling;
		return true;
	}
}

template <class T>
bool Tree<T>::Find(const T & x) {
	if (root->data == x) {
		current = root;
		return true;
	}
	Queue<TreeNode<T> *> q;
	q.enQueue(root);
	while (!q.isEmpty()) {
		TreeNode<T> * p = q.deQueue();
		p = p->firstChild;
		while (p) {
			if (p->data == x) {
				current = p;
				return true;
			}
			q.enQueue(p);
			p = p->nextSibling;
		}
	}
	return false;
}

template <class T>
void Tree<T>::RemoveSubtree() {
	TreeNode<T> * p = current;
	RemoveSubtree(p);
}

template <class T>
T & Tree<T>::Current() {
	return current->data;
}

int main() {
	cout << "请输入要执行的操作：" << endl;
	cout << "A 完善家谱" << endl;
	cout << "B 添加家庭成员" << endl;
	cout << "C 解散局部家庭" << endl;
	cout << "D 更改家庭成员姓名" << endl;
	cout << "E 退出程序" << endl;
	cout << "=====================" << endl;
	cout << "首先建立一个家谱：" << endl;
	cout << "请输入祖先的姓名：";
	string name;
	cin >> name;
	Tree<string> family(name);
	cout << "此家谱的祖先为：" << name << endl;
	while (1) {
		char option;
		string name, name1;
		cout << endl << "请输入要执行的操作：";
		cin >> option;
		switch (option) {
		case 'A':
			cout << "请输入要建立家庭的人的姓名：";
			cin >> name;
			if (!family.Find(name)) {
				cout << "此人不存在" << endl;
				break;
			}
			cout << "请输入" << name << "的子女人数：";
			int n;
			cin >> n;
			if (n <= 0) {
				cout << "输入有误：输入的人数必须为正数" << endl;
				break;
			}
			cout << "请依次输入" << name << "的子女的姓名：";
			for (int i = 0; i < n; ++i) {
				cin >> name1;
				family.InsertChild(name1);
			}
			cout << name << "的第一代子孙为：";
			family.FirstChild();
			do {
				cout << family.Current() << " ";
			} while (family.NextSibling());
			cout << endl;
			break;
		case 'B':
			cout << "请输入要添加子女的人的姓名：";
			cin >> name;
			if (!family.Find(name)) {
				cout << "此人不存在" << endl;
				break;
			}
			cout << "请输入" << name << "新添加的子女的姓名：";
			cin >> name1;
			family.InsertChild(name1);
			cout << name << "的第一代子孙为：";
			family.FirstChild();
			do {
				cout << family.Current() << " ";
			} while (family.NextSibling());
			cout << endl;
			break;
		case 'C':
			cout << "请输入要解散家庭的人的姓名：";
			cin >> name;
			cout << "要解散家庭的人是：" << name << endl;
			if (!family.Find(name)) {
				cout << "此人不存在" << endl;
				break;
			}
			if (family.FirstChild()) {
				cout << name << "的第一代子孙为：";
				do {
					cout << family.Current() << " ";
				} while (family.NextSibling());
				cout << endl;
				family.Find(name);
				family.RemoveSubtree();
				cout << name << "家庭已解散" << endl;
			}
			else {
				cout << "该家庭无子孙" << endl;
			}
			break;
		case 'D':
			cout << "请输入要更改姓名的人的目前姓名：";
			cin >> name;
			if (!family.Find(name)) {
				cout << "此人不存在" << endl;
				break;
			}
			cout << "请输入更改后的姓名：";
			cin >> name1;
			family.Current() = name1;
			cout << name << "已更名为" << name1 << endl;
			break;
		case 'E':
			return 0;
		default:
			cout << "输入有误，请重新输入" << endl;
		}
	}
}
