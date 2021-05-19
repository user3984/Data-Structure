#include <iostream>
#include <string>

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
	Iterator<T> find(const T & val)const;    //返回指向值为val元素迭代器，搜索不成功时返回end 
};

template <class T>
class Iterator {
	friend class List<T>;
private:
	Node<T> * ptr;
public:
	Iterator() :ptr(NULL) {}
	Iterator(Node<T> * nodeptr) :ptr(nodeptr) {}   //用Node指针初始化迭代器 
	Iterator(const Iterator<T> & iter) :ptr(iter.ptr) {}  //复制构造函数
	Iterator<T> & operator++();    //前缀自增，指向后一个元素
	Iterator<T> operator++(int);   //后缀自增，指向后一个元素
	Iterator<T> & operator--();    //前缀自减，指向前一个元素
	Iterator<T> operator--(int);   //后缀自减，指向前一个元素
	T & operator * ();   //取指向的元素
	Iterator<T> & operator = (const Iterator<T> & x); //赋值
	bool operator == (const Iterator<T> & x)const;    //判断相等
	bool operator != (const Iterator<T> & x)const;    //判断不等
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
		q->data = *p1.data;
		q->leftLink = last->leftLink;
		q->leftLink->rightLink = q;
		q->rightLink = last;
		last->leftLink = q;
		++p1;
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

class Student {
	friend ostream & operator << (ostream & o, const Student & stu);
private:
	int number;
	string name;
	string gender;
	int age;
	string type;
public:
	Student(int number_, string & name_, string gender_, int age_, string type_) :
		number(number_), name(name_), gender(gender_), age(age_), type(type_) {}
	Student() {}
	int getNumber()const {
		return number;
	}
	string getType() {
		return type;
	}
};

struct TypeData {  //用于统计时储存类型信息
	string type;
	int number;
};

ostream & operator << (ostream & o, const Student & stu) {
	o << stu.number << "\t" << stu.name << "\t" << stu.gender << "\t" << stu.age << "\t" << stu.type << endl;
	return o;
}

List<Student> studentList;
void setup() {
	cout << "请输入考生人数: ";
	int length;
	cin >> length;
	while (!cin.good()) {
		cin.clear();
		cin.ignore(1024, '\n');
		cout << "输入有误，请重新输入" << endl;
		cin >> length;
	}
	while (length <= 0) {
		cout << "考生人数必须为正整数" << endl;
		cout << "请输入考生人数: " << endl;
		cin >> length;
	}
	cout << "请依次输入考生的考号、姓名、性别、年龄及报考类别" << endl;
	for (int i = 0; i < length; ++i) {
		int number_;
		string name_;
		string gender_;
		int age_;
		string type_;
		cin >> number_ >> name_ >> gender_ >> age_ >> type_;
		if (!cin.good()) {
			cin.clear();
			cin.ignore(1024, '\n');
			cout << "输入有误，请重新输入" << endl;
			--i;
		}
		else {
			Student stu(number_, name_, gender_, age_, type_);
			studentList.pushback(stu);
		}
	}
}

void insertStu() {
	cout << "请输入你要插入考生的位置" << endl;
	int position;
	cin >> position;
	while (!cin.good()) {  //输入类型不匹配
		cin.clear();
		cin.ignore(1024, '\n');
		cout << "输入有误，请重新输入" << endl;
		cin >> position;
	}
	Iterator<Student> it = studentList.begin();
	if (position <= 0) {
		cout << "无效的插入位置！" << endl;
		return;
	}
	if (position == 1) {
		--it;
	}
	else {
		for (int i = 0; i < position - 2; ++i) {
			++it;
			if (it == studentList.end()) {
				cout << "无效的插入位置！" << endl;
				return;
			}
		}
	}  //将迭代器指向要插入的位置
	cout << "请依次输入要插入的考生的考号、姓名、性别、年龄及报考类别" << endl;
	int number_;
	string name_;
	string gender_;
	int age_;
	string type_;
	cin >> number_ >> name_ >> gender_ >> age_ >> type_;
	Student stu(number_, name_, gender_, age_, type_);
	studentList.insert(stu, it); //将stu对象插入到it指向的位置之后
}

void removeStu() {
	cout << "请输入要删除的考生的考号: ";
	int num;
	cin >> num;
	while (!cin.good()) {
		cin.clear();
		cin.ignore(1024, '\n');
		cout << "输入有误，请重新输入" << endl;
		cin >> num;
	}
	Iterator<Student> it = studentList.begin();
	for (; it != studentList.end() && (*it).getNumber() != num; ++it); //搜索
	if (it == studentList.end()) {   //搜索失败
		cout << "考号不存在!" << endl;
	}
	else {  //搜索成功
		cout << "你删除的考生信息是：" << studentList.remove(it);
	}
}

void findStu() {
	cout << "请输入要查找的考生的考号：";
	int num;
	cin >> num;
	while (!cin.good()) {   //输入类型不匹配
		cin.clear();
		cin.ignore(1024, '\n');
		cout << "输入有误，请重新输入" << endl;
		cin >> num;
	}
	Iterator<Student> it = studentList.begin();
	for (; it != studentList.end() && (*it).getNumber() != num; ++it);
	if (it == studentList.end()) {  //搜索失败
		cout << "考号不存在!" << endl;
	}
	else {
		cout << *it;  //输出考生信息
	}
}

void display() {
	cout << endl << "考号\t姓名\t性别\t年龄\t报考类型" << endl;
	Iterator<Student> it = studentList.begin();
	for (; it != studentList.end(); ++it) {
		cout << *it;
	}
	cout << endl;
}

void modify() {
	cout << "请输入要修改的考生的考号：";
	int num;
	cin >> num;
	while (!cin.good()) {   //输入类型不匹配
		cin.clear();
		cin.ignore(1024, '\n');
		cout << "输入有误，请重新输入" << endl;
		cin >> num;
	}
	Iterator<Student> it = studentList.begin();
	for (; it != studentList.end() && (*it).getNumber() != num; ++it);
	if (it == studentList.end()) {
		cout << "考号不存在!" << endl;
		return;
	}   //搜索
	cout << "请依次输入要插入的考生的考号、姓名、性别、年龄及报考类别" << endl;
	int number_;
	string name_;
	string gender_;
	int age_;
	string type_;
	cin >> number_ >> name_ >> gender_ >> age_ >> type_;
	while (!cin.good()) {   //输入类型不匹配
		cin.clear();
		cin.ignore(1024, '\n');
		cout << "输入有误，请重新输入" << endl;
		cin >> number_ >> name_ >> gender_ >> age_ >> type_;
	}
	Student stu(number_, name_, gender_, age_, type_);
	*it = stu;
	cout << "修改成功！" << endl;
}

Iterator<struct TypeData> findType(List<struct TypeData> & typeList, string type0) {
	//在链表中查找给定报考类型对应的结点，并返回指向该结点的迭代器
	Iterator<struct TypeData> it = typeList.begin();
	for (; it != typeList.end(); ++it) {
		if ((*it).type == type0) {
			return it;
		}
	}
	return NULL;    //查找失败，返回空迭代器
}
void statistic() {
	int totalNum = 0;   //总人数
	List<struct TypeData> typeList;  //用于储存各报考类型的人数
	Iterator<Student> iter = studentList.begin();
	for (; iter != studentList.end(); ++iter) {  //遍历所有考生
		++totalNum;
		string currentType = (*iter).getType();
		Iterator<struct TypeData> tmpIter = findType(typeList, currentType);
		if (tmpIter == NULL) {    //结果链表中不存在此报考类型，新建结点
			struct TypeData tmpData;
			tmpData.type = currentType;
			tmpData.number = 1;
			typeList.pushback(tmpData);
		}
		else {   //结果链表中已存在此报考类型，人数加一
			(*tmpIter).number++;
		}
	}
	Iterator<struct TypeData> it = typeList.begin();
	for (; it != typeList.end(); ++it) {    //输出所有类型的报考人数
		cout << (*it).type << "\t" << (*it).number << "人\t" << 100 * (double((*it).number) / totalNum) << "%" << endl;
	}
	cout << "报考总人数: " << totalNum << "人" << endl;
}

int main() {
	setup();
	display();
	while (1) {
		int operation;
		cout << "请选择您要进行的操作（1为插入，2为删除，3为查找，4为修改，5为统计，6为退出，0为取消操作）" << endl;
		cin >> operation;
		while (!cin.good()) {    //输入类型不匹配
			cin.clear();
			cin.ignore(1024, '\n');
			cout << "输入有误，请重新输入" << endl;
			cin >> operation;
		}
		switch (operation) {
		case 1:
			insertStu();
			display();
			break;
		case 2:
			removeStu();
			display();
			break;
		case 3:
			findStu();
			break;
		case 4:
			modify();
			display();
			break;
		case 5:
			statistic();
			break;
		case 6:
			return 0;
		case 0:
			display();
			break;
		default:
			cout << "输入有误，请重新输入！" << endl;
		}
	}
}
