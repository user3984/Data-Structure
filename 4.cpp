#include <iostream>
#include <string>
using namespace std;

template <class T>
class Stack;

template <class T>
class Node {
	friend class Stack<T>;
private:
	T data;
	Node<T> * link;
	Node() :link(NULL) {};
	Node(T x, Node<T> * l = NULL) :data(x), link(l) {};
};

template <class T>
class Stack {
private:
	Node <T> * top;
public:
	Stack();
	~Stack();
	void push(const T & x);
	T pop();
	T getTop();
	void clear();
	bool isEmpty();
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
	//if (top == NULL) { throw 0; }
	Node<T> * p = top;
	T tmp = p->data;
	top = top->link;
	delete p;
	return tmp;
}

template <class T>
T Stack<T>::getTop() {
	//if (top == NULL) { throw 0; }
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

bool isNumber(char c) {
	return (c >= 48 && c <= 57);
}

bool isOperator(char c) {
	return (c == '(' || c == ')' || c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^');
}

int OpPriority(char c) {   //运算符的优先级
	switch (c) {
	case '(':
	case ')':
		return 0;
	case '+':
	case '-':
		return 1;
	case '*':
	case '/':
	case '%':
		return 2;
	case '^':
		return 3;
	default:
		return 0;
	}
}

int biOperation(int a, int b, char c) {
	//根据操作数与运算符进行双目运算, 返回运算结果
	int tmp = 1;
	switch (c) {
	case '+':
		return a + b;
	case '-':
		return a - b;
	case '*':
		return a * b;
	case '/':
		return a / b;
	case '%':
		return a % b;
	case '^':
		if (b < 0) { return 0; }
		for (int i = 0; i < b; ++i) {
			tmp *= a;
		}
		return tmp;
	default:
		return 0;
	}
}

bool check(char pre, char cur) {  //检查两个相邻字符组合是否合法
	if (pre == '+' || pre == '-' || pre == '*' || pre == '/' || pre == '%' || pre == '^') {
		if (!isNumber(cur) && cur != '(' && cur != '+' && cur != '-') { return false; }
	}
	else if (isNumber(pre)) {
		if (!isNumber(cur) && cur != '=' && cur != ')' && cur != '+' && cur != '-' && cur != '*' && cur != '/' && cur != '%' && cur != '^') { return false; }
	}
	else if (pre == ')') {
		if (cur != '=' && cur != ')' && cur != '+' && cur != '-' && cur != '*' && cur != '/' && cur != '%' && cur != '^') { return false; }
	}
	else if (pre == '(') {
		if (!isNumber(cur) && cur != '(' && cur != '+' && cur != '-') { return false; }
	}
	else {
		return false;
	}
	return true;
}

bool bracketMatch(string s) {   //检查括号是否匹配
	Stack<char> st;
	for (int i = 0; i < (int)s.length(); ++i) {
		if (s[i] == '(') {
			st.push(s[i]);
		}
		else if (s[i] == ')') {
			if (!st.isEmpty()) {
				st.pop();
			}
			else {   //右括号多于左括号
				return false;
			}
		}
	}
	return st.isEmpty() ? true : false;
}

bool syntaxCheck(string s) {    //对表达式进行语法检查
	if (!isNumber(s[0]) && s[0] != '(' && s[0] != '+' && s[0] != '-') {
		cout << "表达式有误: 语法错误!" << endl;
		return false;
	}
	if (s[s.length() - 1] != '=') {
		cout << "表达式有误: 必须以 '=' 结尾!" << endl;
		return false;
	}
	for (int i = 0; i < s.length() - 1; ++i) {
		if (!check(s[i], s[i + 1])) {
			cout << "表达式有误: 语法错误!" << endl;
			return false;
		}
	}
	if (!bracketMatch(s)) {
		cout << "表达式有误: 括号不匹配!" << endl;
		return false;
	}
	return true;
}

int operation(string s) {
	Stack<int> numSt;
	Stack<char> opSt;
	int tag = 0;      //用于判断 '+', '-' 类型，tag = 0 表示正负，tag = 1 表示加减
	int numTag = 1;   //表示数字的符号，1为正，-1为负
	for (int i = 0; i < int(s.length()) - 1; ++i) {
		//每次取出一个字符
		if (isNumber(s[i])) {  //当前字符为数字
			int num = 0;
			while (isNumber(s[i])) {
				num = num * 10 + s[i] - 48;
				++i;
			} //取出后续数字, 转换为十进制数
			numSt.push(numTag * num);
			--i;
			tag = 1;      //数字后的 '+', '-' 表示加减
			numTag = 1;   //重置符号标识
		}
		else if (isOperator(s[i])) {
			if (tag == 0 && (s[i] == '+' || s[i] == '-')) {  //表示正负
				if (s[i] == '-') {
					numTag = -numTag;
				}
			}
			else if (s[i] == ')') {
				int num2 = numSt.pop(), num1 = numSt.pop();
				int tmp = biOperation(num1, num2, opSt.pop());
				while (opSt.getTop() != '(') {
					tmp = biOperation(numSt.pop(), tmp, opSt.pop());
				}  //将左括号后的运算符全部计算完
				opSt.pop();       //弹出对应的左括号
				numSt.push(tmp);  //括号内表达式的计算结果入栈
			}
			else if (s[i] == '(' || opSt.isEmpty() || opSt.getTop() == '(' || OpPriority(s[i]) > OpPriority(opSt.getTop())) {
				opSt.push(s[i]);
			}
			else {
				while (!opSt.isEmpty() && OpPriority(s[i]) <= OpPriority(opSt.getTop())) {
					int num2 = numSt.pop();
					int num1 = numSt.pop();
					int tmp = biOperation(num1, num2, opSt.pop());
					numSt.push(tmp);
				}
				opSt.push(s[i]);
			}
			if (s[i] == '(' || s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/' || s[i] == '%' || s[i] == '^') {
				tag = 0;   //以上符号后的 '+', '-' 表示正负
			}
			else {
				tag = 1;   //其他符号后的 '+', '-' 表示加减
			}
		}
		else {
			cout << "表达式有误: 输入了非法字符!" << endl;
		}
	}
	int tmp = numSt.pop();
	while (!numSt.isEmpty() && !opSt.isEmpty()) {
		tmp = biOperation(numSt.pop(), tmp, opSt.pop());
	}
	return tmp;
}


int main() {
	int option = 1;
	while (option) {
		cout << "请输入表达式:" << endl;
		string s;
		cin >> s;
		if (syntaxCheck(s)) {   //语法检查
			int result = operation(s);
			cout << "运算结果: " << result << endl;
		}
		cout << "是否继续? (y,n) ";
		char c;
		do {
			cin >> c;
			switch (c) {
			case 'y':
				option = 1;
				break;
			case 'n':
				option = 0;
				break;
			default:
				cout << "输入有误，请重新输入" << endl;
			}
		} while (c != 'y' && c != 'n');
	}
	return 0;
}