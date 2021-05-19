#include <iostream>
using namespace std;

template <class T>
class Stack;

template <class T>
class Node {
	friend class Stack<T>;
private:
	T data;
	Node<T> * link;   //指向下一结点
	Node() :link(NULL) {};   //构造函数
	Node(T x, Node<T> * l = NULL) :data(x), link(l) {};  //构造函数
};

template <class T>
class Stack {
private:
	Node <T> * top;  //栈顶指针
public:
	Stack();    //构造函数
	~Stack();   //析构函数
	void push(const T & x);  //压栈
	T pop();     //弹出
	T getTop();  //取栈顶元素
	void clear();   //清空栈
	bool isEmpty(); //判断栈是否为空
};

template <class T>
Stack<T>::Stack() :top(NULL) {}

template <class T>
Stack<T>::~Stack() {
	while (top != NULL) {
		Node<T> * p = top;
		top = top->link;
		delete p;
	}
}

template <class T>
void Stack<T>::push(const T & x) {
	Node<T> * p = new Node<T>;
	p->data = x;
	p->link = top;
	top = p;
}

template <class T>
T Stack<T>::pop() {
	Node<T> * p = top;
	T tmp = p->data;
	top = top->link;
	delete p;
	return tmp;
}

template <class T>
T Stack<T>::getTop() {
	return top->data;
}

template <class T>
void Stack<T>::clear() {
	while (top != NULL) {
		Node<T> * p = top;
		top = top->link;
		delete p;
	}
}

template <class T>
bool Stack<T>::isEmpty() {
	return (top == NULL);
}

struct Point {
	int row;
	int col;
};

class Maze {
private:
	int size;   //迷宫大小，不包括边缘围墙
	int entranceRow, entranceCol, exitRow, exitCol;  //出入口坐标
	char ** data;     //二维数组，储存迷宫地图
	bool ** visited;  //二维数组，标记已访问的点
public:
	Maze();          //构造函数, 使用默认地图
	Maze(int size);  //构造函数, 用户自定义地图 
	~Maze();         //析构函数
	void findPath(); //寻找路径
};

Maze::Maze() :size(5) {
	data = new char*[size + 2];
	data[0] = new char[size + 2];
	for (int i = 0; i < size + 2; ++i) {
		data[0][i] = '#';
	}//上侧围墙
	for (int i = 1; i < size + 1; ++i) {
		data[i] = new char[size + 2];
		data[i][0] = '#';
		data[i][size + 1] = '#';
	}
	//地图初始化为默认值
	data[1][1] = '0'; data[1][2] = '#'; data[1][3] = '0'; data[1][4] = '0'; data[1][5] = '0';
	data[2][1] = '0'; data[2][2] = '#'; data[2][3] = '0'; data[2][4] = '#'; data[2][5] = '#';
	data[3][1] = '0'; data[3][2] = '0'; data[3][3] = '0'; data[3][4] = '#'; data[3][5] = '0';
	data[4][1] = '0'; data[4][2] = '#'; data[4][3] = '0'; data[4][4] = '0'; data[4][5] = '0';
	data[5][1] = '0'; data[5][2] = '#'; data[5][3] = '0'; data[5][4] = '#'; data[5][5] = '0';
	data[size + 1] = new char[size + 2];
	for (int i = 0; i < size + 2; ++i) {
		data[size + 1][i] = '#';
	}//下侧围墙
	visited = new bool*[size + 2];
	for (int i = 0; i < size + 2; ++i) {
		visited[i] = new bool[size + 2];
		for (int j = 0; j < size + 2; ++j) {
			visited[i][j] = 0;
		}
	}//visited数组初始化为0
	std::cout << "输入入口与出口" << endl;
	std::cin >> entranceRow >> entranceCol >> exitRow >> exitCol;
}

Maze::Maze(int sz) :size(sz) {
	data = new char*[size + 2];
	data[0] = new char[size + 2];
	std::cout << "输入迷宫地图" << endl;
	for (int i = 0; i < size + 2; ++i) {
		data[0][i] = '#';
	}//上侧围墙
	for (int i = 1; i < size + 1; ++i) {
		data[i] = new char[size + 2];
		data[i][0] = '#';
		for (int j = 1; j < size + 1; ++j) {
			std::cin >> data[i][j];
		}
		data[i][size + 1] = '#';
	}
	data[size + 1] = new char[size + 2];
	for (int i = 0; i < size + 2; ++i) {
		data[size + 1][i] = '#';
	}//下侧围墙
	visited = new bool*[size + 2];
	for (int i = 0; i < size + 2; ++i) {
		visited[i] = new bool[size + 2];
		for (int j = 0; j < size + 2; ++j) {
			visited[i][j] = 0;
		}
	}//visited数组初始化为0
	std::cout << "输入入口与出口" << endl;
	std::cin >> entranceRow >> entranceCol >> exitRow >> exitCol;
}

Maze::~Maze() {
	for (int i = 0; i < size + 2; ++i) {
		delete[]data[i];
		delete[]visited[i];
	}
	delete[]data;
	delete[]visited;
}

void Maze::findPath() {
	while (entranceRow <= 0 || entranceCol <= 0 || entranceRow > size || entranceCol > size) {
		std::cout << "入口坐标不合法, 请重新输入:" << std::endl;
		std::cin >> entranceRow >> entranceCol;
	}
	while (exitRow <= 0 || exitCol <= 0 || exitRow > size || exitCol > size) {
		std::cout << "出口坐标不合法, 请重新输入:" << std::endl;
		std::cin >> exitRow >> exitCol;
	}
	int currentRow = entranceRow, currentCol = entranceCol;
	Stack<Point> st;
	st.push(Point{ currentRow, currentCol });
	visited[currentRow][currentCol] = 1;
	do {
		if (currentRow == exitRow && currentCol == exitCol) {
			//已经到达出口, 输出找到的路径
			Stack<Point> tmp;   //用于暂存要输出的路径点
			while (!st.isEmpty()) {
				Point x = st.pop();
				data[x.row][x.col] = 'x';  //在地图上标记路径点
				tmp.push(x);
			}
			Point point = tmp.pop();
			std::cout << "迷宫地图:" << endl << endl << "\t";
			for (int i = 0; i < size + 2; ++i) {
				std::cout << i << "列\t";
			}
			std::cout << endl << endl;
			for (int i = 0; i < size + 2; ++i) {
				std::cout << i << "行\t";
				for (int j = 0; j < size + 2; ++j) {
					std::cout << data[i][j] << "\t";
				}
				std::cout << endl << endl;
			}
			std::cout << "迷宫路径:" << endl << endl << "<" << point.row << "," << point.col << "> ";
			while (!tmp.isEmpty()) {
				Point point = tmp.pop();
				std::cout << "--> <" << point.row << "," << point.col << "> ";
			}
			return;
		}
		if (data[currentRow + 1][currentCol] == '0' && !visited[currentRow + 1][currentCol]) {
			++currentRow;
			st.push(Point{ currentRow, currentCol });
			visited[currentRow][currentCol] = 1;
		}//向下
		else if (data[currentRow][currentCol + 1] == '0' && !visited[currentRow][currentCol + 1]) {
			++currentCol;
			st.push(Point{ currentRow, currentCol });
			visited[currentRow][currentCol] = 1;
		}//向右
		else if (data[currentRow][currentCol - 1] == '0' && !visited[currentRow][currentCol - 1]) {
			--currentCol;
			st.push(Point{ currentRow, currentCol });
			visited[currentRow][currentCol] = 1;
		}//向左
		else if (data[currentRow - 1][currentCol] == '0' && !visited[currentRow - 1][currentCol]) {
			--currentRow;
			st.push(Point{ currentRow, currentCol });
			visited[currentRow][currentCol] = 1;
		}//向上
		else {//所有方向都走不通, 回溯
			st.pop();
			if (!st.isEmpty()) {
				Point p = st.getTop();
				currentRow = p.row;
				currentCol = p.col;
			}
		}
	} while (!st.isEmpty());
	std::cout << "起点与终点间不存在路径!" << endl;
}

int main() {
	std::cout << "使用默认地图? (y/n)" << std::endl;
	char option;
	std::cin >> option;
	if (option == 'y') {
		Maze maze;
		maze.findPath();
	}
	else {
		int n;
		do {
			std::cout << "输入迷宫大小: ";
			std::cin >> n;
			if (n <= 0) {
				std::cout << "必须输入正整数!" << std::endl;
			}
		} while (n <= 0);
		Maze maze(n);
		maze.findPath();
	}
	return 0;
}

