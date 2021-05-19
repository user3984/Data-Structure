#include <iostream>
#include <string>
#include <fstream>

using namespace std;
const int DEFAULTSIZE = 2;

template <class T>
class Array {
private:
	T * ptr;       //数组指针
	int len;       //当前长度
	int maxsize;   //最大容量
	bool resize(); //扩容
public:
	Array();                         //构造函数
	Array(int sz);                   //构造函数, 指定容量
	Array(const Array<T> & ar);      //复制构造函数
	~Array();                        //析构函数
	int length() { return len; }     //获取当前长度
	T & operator[](int n);           //按下标访问
	void insert(const T & x, int n); //将x插入到n之后
	void pushback(const T & x);      //从尾部添加元素
	T remove(int n);                 //删除下标为n的元素
	void clear() { len = 0; }        //清空数组
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

//利用出边表存储数据
struct Edge {
	int dest;
	Edge * next;
};

//输入的课程数据用Course结构体数组存储
struct Course {
	string courseNum;  //课程代码
	string name;       //课程名称
	int time;          //学时数
	int semester;      //指定开课学习
	Array<string> pre; //先修课程
};

//先选固定开课学期的课程, 其余课程存在图中
class Vertex {       //顶点类(不指定开课学期的课程)
	friend class Graph;
private:
	int inDegree;
	int outDegree;
	string courseNum;
	string name;
	int time;
	Edge * firstNeighbor;
public:
	Vertex() :inDegree(0), outDegree(0), firstNeighbor(NULL) {}
	Vertex(int indeg, int outdeg, string cn, string cnum, int t, Edge * fn = NULL) :inDegree(indeg), courseNum(cnum), name(cn), time(t), firstNeighbor(fn) {}
};

struct English {  //指定开课学期的课程
	string name;
	int time;
};

class Graph {
private:
	int nSemesters;         //学期数
	Array<Vertex> vertices; //顶点(不指定开课学期的课)
	Array<English> * eng;   //存储每个学期的指定开课学期的课程
	Array<int> indeg0;      //入度为0的顶点的下标, 按出度递增排列
	int findCourse(string num);  //根据课程代码查找课程的下标
	int arrange(string cname, string ** l, int n, int before = -1); //处理学时数为1, 2, 3的排课
public:
	Graph(Course data[], int len, int nsem);  //构造函数
	~Graph();   //析构函数
	bool semesterArrange(int smt, int nCourse, string ** l);  //排一个学期的课程
};

int Graph::findCourse(string num) {   //查找给定课程代码的课程在vertices表中的位置
	for (int i = 0; i < vertices.length(); ++i) {
		if (vertices[i].courseNum == num) { return i; }
	}
	cout << "课程不存在!" << endl;
	return -1;
}

Graph::Graph(Course data[], int len, int nsem) :nSemesters(nsem) {  //构造函数
	eng = new Array<English>[nsem];   //存储每个学期的指定开课学期的课程
	vertices.clear();
	indeg0.clear();
	for (int i = 0; i < len; ++i) {     //第一次扫描课程数据数组data, 保存课程名称、课程代码及学时数
		if (data[i].semester == 0) {    //不指定开课学期, 存储在vertices数组中
			Course c = data[i];
			Vertex v(0, 0, c.name, c.courseNum, c.time);
			vertices.pushback(v);
		}
		else {   //指定开课学期, 存储在eng数组中
			English e;
			e.name = data[i].name;
			e.time = data[i].time;
			eng[data[i].semester - 1].pushback(e);
		}
	}
	for (int i = 0; i < len; ++i) {    //第二次扫描课程数据数组data, 创建邻接表
		if (data[i].semester == 0) {   //不指定学期
			int npre = data[i].pre.length(), pos;  //npre为先修课门数
			pos = findCourse(data[i].courseNum);   //pos为data数组第i门课在vertices数组中的下标
			vertices[pos].inDegree = npre;
			for (int k = 0; k < npre; ++k) {       //data[i]的每一门先修课添加后继data[i]
				int prepos = findCourse(data[i].pre[k]);  //寻找前驱在vertices数组中的下标
				++vertices[prepos].outDegree;
				Edge * p = vertices[prepos].firstNeighbor;
				if (p) {
					while (p->next) { p = p->next; }
					p->next = new Edge;
					p->next->dest = pos;
					p->next->next = NULL;
				}
				else {
					vertices[prepos].firstNeighbor = new Edge;
					vertices[prepos].firstNeighbor->dest = pos;
					vertices[prepos].firstNeighbor->next = NULL;
				}
			}
		}
	}
	for (int i = 0; i < vertices.length(); ++i) {  //存储入度为0的顶点
		if (vertices[i].inDegree == 0) {
			//二分插入, 按出度递增
			int lo = 0, hi = indeg0.length() - 1, mid;
			while (hi >= lo) {
				mid = (hi + lo) / 2;
				if (vertices[indeg0[mid]].outDegree <= vertices[i].outDegree) { lo = mid + 1; }
				else { hi = mid - 1; }
			}
			indeg0.insert(i, hi);
		}
	}
}

Graph::~Graph() {
	for (int i = 0; i < vertices.length(); ++i) {
		Edge * p = vertices[i].firstNeighbor, * q;
		while (p) {
			q = p;
			p = p->next;
			delete q;
		}
	}
	delete[]eng;
}

int Graph::arrange(string cname, string ** l, int n, int before) {
	//处理学时数为1, 2, 3的排课, 其他课时数可分解为1, 2, 3的组合
	//cname为要排课的课程名, l为存储课程表的二维数组, n为学时数, 只能取1, 2, 3
	//before参数为上次上课时间, 若本次为首次排课, before = -1, before的默认值为-1
	int temp = -1;
	if (before == -1) {
		switch (n) {
		case 1:    //优先排第3或第8节
			for (int d = 0; d < 5; ++d) {
				if (l[d][2] == "无") {
					l[d][2] = cname;
					return d;
				}
				if (l[d][7] == "无") {
					l[d][7] = cname;
					return d;
				}
			}
			for (int d = 0; d < 5; ++d) {  //无法安排在第3或第8节, 安排在其他时间
				for (int c = 0; c < 9; ++c) {
					if (l[d][c] == "无") {
						l[d][c] = cname;
						return d;
					}
				}
			}
			cout << "无法排课！" << endl;
			return -1;
			break;
		case 2:     //优先排第1或第6节
			for (int d = 0; d < 5; ++d) {
				if (l[d][0] == "无" && l[d][1] == "无") {
					l[d][0] = l[d][1] = cname;
					return d;
				}
				if (l[d][5] == "无" && l[d][6] == "无") {
					l[d][5] = l[d][6] = cname;
					return d;
				}
			}
			for (int d = 0; d < 5; ++d) {    //排第4或第9节
				if (l[d][3] == "无" && l[d][4] == "无") {
					l[d][3] = l[d][4] = cname;
					return d;
				}
				if (l[d][8] == "无" && l[d][9] == "无") {
					l[d][8] = l[d][9] = cname;
					return d;
				}
			}
			//没有连续2节, 拆分成1+1
			temp = arrange(cname, l, 1);
			if (temp != -1) { temp = arrange(cname, l, 1, temp); }
			return temp;
			break;
		case 3:   //优先排第3或第8节
			for (int d = 0; d < 5; ++d) {
				if (l[d][2] == "无" && l[d][3] == "无" && l[d][4] == "无") {
					l[d][2] = l[d][3] = l[d][4] = cname;
					return d;
				}
				if (l[d][7] == "无" && l[d][8] == "无" && l[d][9] == "无") {
					l[d][7] = l[d][8] = l[d][9] = cname;
					return d;
				}
			}
			//没有连续3节, 拆分成1+2
			temp = arrange(cname, l, 2);
			if (temp != -1) { temp = arrange(cname, l, 1, temp); }
			return temp;
		default:
			return -1;
		}
	}
	else {  //需要隔天开课
		switch (n) {
		case 1:    //优先排第3或第8节
			for (int i = 0, d = (before + 2) % 5; i < 2; ++i, d = (d + 1) % 5) {
				if (l[d][2] == "无") {
					l[d][2] = cname;
					return d;
				}
				if (l[d][7] == "无") {
					l[d][7] = cname;
					return d;
				}
			}
			for (int i = 0, d = (before + 2) % 5; i < 2; ++i, d = (d + 1) % 5) {
				for (int c = 0; c < 9; ++c) {
					if (l[d][c] == "无") {
						l[d][c] = cname;
						return d;
					}
				}
			}
			return arrange(cname, l, 1);  //无法满足隔天条件
			break;
		case 2:    //优先排第1或第6节
			for (int i = 0, d = (before + 2) % 5; i < 2; ++i, d = (d + 1) % 5) {
				if (l[d][0] == "无" && l[d][1] == "无") {
					l[d][0] = l[d][1] = cname;
					return d;
				}
				if (l[d][5] == "无" && l[d][6] == "无") {
					l[d][5] = l[d][6] = cname;
					return d;
				}
			}
			for (int i = 0, d = (before + 2) % 5; i < 2; ++i, d = (d + 1) % 5) {    //排第4或第9节
				if (l[d][3] == "无" && l[d][4] == "无") {
					l[d][3] = l[d][4] = cname;
					return d;
				}
				if (l[d][8] == "无" && l[d][9] == "无") {
					l[d][8] = l[d][9] = cname;
					return d;
				}
			}
			return arrange(cname, l, 2);  //无法满足隔天条件
			break;
		case 3:   //优先排第3或第8节
			for (int i = 0, d = (before + 2) % 5; i < 2; ++i, d = (d + 1) % 5) {
				if (l[d][2] == "无" && l[d][3] == "无" && l[d][4] == "无") {
					l[d][2] = l[d][3] = l[d][4] = cname;
					return d;
				}
				if (l[d][7] == "无" && l[d][8] == "无" && l[d][9] == "无") {
					l[d][7] = l[d][8] = l[d][9] = cname;
					return d;
				}
			}
			return arrange(cname, l, 3);  //无法满足隔天条件
		default:   //参数n错误
			return -1;
		}
	}
}

bool Graph::semesterArrange(int smt, int nCourse, string ** l) {   //排一个学期的课程
	//smt为要排课的学期, nCourse为该学期的开课门数, l为存储课程表的二维数组
	if (nCourse < eng[smt].length()) {
		cout << "无法排课：第" << smt + 1<< "学期开设课程过少！" << endl;
		return false;
	}
	Array<int> tmparr;
	for (int i = 0; i < nCourse - eng[smt].length(); ++i) {
		if (indeg0.length() != 0) {
			//取出出度最大的入度为0的课程
			int tmp = indeg0.remove(indeg0.length() - 1);
			Edge * p1 = vertices[tmp].firstNeighbor;
			while (p1 != NULL) {
				tmparr.pushback(p1->dest);
				p1 = p1->next;
			}
			//排课
			//二维数组l用于存储本学期课程表, 行下标0-4分别代表周一至周五, 列下标代表节次。
			int t = vertices[tmp].time;
			if (t < 4) {
				arrange(vertices[tmp].name, l, t);
			}
			else if (t % 3 == 1) {
				arrange(vertices[tmp].name, l, 2, arrange(vertices[tmp].name, l, 2));
				for (int i = 0; i < (t - 4) / 3; ++i) {
					arrange(vertices[tmp].name, l, 3);
				}
			}
			else if (t % 3 == 2) {
				arrange(vertices[tmp].name, l, 2, arrange(vertices[tmp].name, l, 3));
				for (int i = 0; i < (t - 5) / 3; ++i) {
					arrange(vertices[tmp].name, l, 3);
				}
			}
			else {
				arrange(vertices[tmp].name, l, 3, arrange(vertices[tmp].name, l, 3));
				for (int i = 0; i < t / 3 - 2; ++i) {
					arrange(vertices[tmp].name, l, 3);
				}
			}
		}
		else {
			cout << "无法排课：不满足先修课要求。第" << smt + 1 << "学期开设课程过多！" << endl;
			return false;
		}
	}
	for (int i = 0; i < tmparr.length(); ++i) {
		if (--vertices[tmparr[i]].inDegree == 0) {  //若出度为0, 压入栈中
			//二分插入
			int lo = 0, hi = indeg0.length() - 1, mid;
			while (hi >= lo) {
				mid = (hi + lo) / 2;
				if (vertices[indeg0[mid]].outDegree <= vertices[tmparr[i]].outDegree) { lo = mid + 1; }
				else { hi = mid - 1; }
			}
			indeg0.insert(tmparr[i], hi);
		}
	}
	for (int i = 0; i < eng[smt].length(); ++i) {
		int t = eng[smt][i].time;
		if (t < 4) {
			arrange(eng[smt][i].name, l, t);
		}
		else if (t % 3 == 1) {
			arrange(eng[smt][i].name, l, 2, arrange(eng[smt][i].name, l, 2));
			for (int j = 0; j < (t - 4) / 3; ++j) {
				arrange(eng[smt][i].name, l, 3);
			}
		}
		else if (t % 3 == 2) {
			arrange(eng[smt][i].name, l, 2, arrange(eng[smt][i].name, l, 3));
			for (int i = 0; i < (t - 5) / 3; ++i) {
				arrange(eng[smt][i].name, l, 3);
			}
		}
		else {
			arrange(eng[smt][i].name, l, 3, arrange(eng[smt][i].name, l, 3));
			for (int i = 0; i < t / 3 - 2; ++i) {
				arrange(eng[smt][i].name, l, 3);
			}
		}
	}
	return true;
}

int main() {
	int nTotal, nSem;  //课程总数, 学期数
	cout << "请输入课程总数: ";
	cin >> nTotal;
	cout << "请输入学期数: ";
	cin >> nSem;
	Course * data = new Course[nTotal];
	int * eachSem = new int[nSem];
	cout << "请输入每个学期的开课门数: ";
	int s = 0;
	for (int i = 0; i < nSem; ++i) {
		cin >> eachSem[i];
		s += eachSem[i];
	}
	if (s != nTotal) {
		cout << "开课总数不等于课程总数！" << endl;
		return 0;
	}
	cout << "请输入课程信息:" << endl;
	string p;
	ofstream courseout("Course.txt", ios::out);
	courseout << "课程编号\t课程名称\t\t学时数\t指定开课学期\t先修课程" << endl;
	for (int i = 0; i < nTotal; ++i) {    //输入每一门课程信息, 存储在data数组中, 同时写入文件中
		cin >> data[i].courseNum >> data[i].name >> data[i].time >> data[i].semester;
		courseout << data[i].courseNum << "\t";
		if (data[i].name.length() > 8) { courseout << data[i].name << "\t"; }
		else { courseout << data[i].name << "\t\t"; }
		courseout << data[i].time << "\t" << data[i].semester << "\t\t";
		cin >> p;
		while (p != "0") {
			courseout << p << "\t";
			data[i].pre.pushback(p);
			cin >> p;
		}
		courseout << endl;
	}
	Graph g(data, nTotal, nSem);
	ofstream out("CourseArrangement.txt", ios::out);
	string ** table = new string *[5];
	for (int i = 0; i < 5; ++i) { table[i] = new string[10]; }
	for (int i = 0; i < nSem; ++i) {
		for (int k = 0; k < 5; ++k) {
			for (int j = 0; j < 10; ++j) {
				table[k][j] = "无";
			}
		} //课程表初始化为空
		if (!g.semesterArrange(i, eachSem[i], table)) { break; }
		out << "第" << i + 1 << "学期课程表" << endl << endl << "\t周一\t\t周二\t\t周三\t\t周四\t\t周五" << endl;
		for (int r = 0; r < 10; ++r) {
			out << "第" << r + 1 << "节\t";
			for (int c = 0; c < 5; ++c) {
				if (table[c][r].length() > 8){ out << table[c][r] << "\t"; }
				else { out << table[c][r] << "\t\t"; }
			}
			out << endl;
		}
		out << endl;
	} //对每个学期进行排课, 并将排课结果写入文件中
	for (int j = 0; j < 5; ++j) { delete[]table[j]; }
	delete[]table;
	delete[]eachSem;
	delete[]data;
	return 0;
}