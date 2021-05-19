#include <iostream>
using namespace std;
const int MAXNUM = 10000;

class UFSet {
private:
	int currentSize; //当前元素个数
	int maxSize;     //最大元素个数
	int * parent;    //双亲数组
public:
	UFSet(int sz);  //构造函数
	~UFSet();       //析构函数
	int findRoot(int n);   //寻找n所在树的根结点
	void Union(int root1, int root2);  //合并
	bool addElement();   //添加元素
};

UFSet::UFSet(int sz) :maxSize(sz), currentSize(0) {
	parent = new int[maxSize];
	for (int i = 0; i < maxSize; ++i) {
		parent[i] = -1;
	}
}

UFSet::~UFSet() {
	delete[]parent;
}

int UFSet::findRoot(int n) {
	int i = n;
	for (; parent[i] >= 0; i = parent[i]);
	return i;
}

void UFSet::Union(int root1, int root2) {
	if (parent[root1] < 0 && parent[root2] < 0 && root1 != root2) {
		int tmp = parent[root1] + parent[root2];
		if (parent[root1] < parent[root2]) {
			parent[root2] = root1;
			parent[root1] = tmp;
		}
		else {
			parent[root1] = root2;
			parent[root2] = tmp;
		}
	}
}

bool UFSet::addElement() {
	if (currentSize >= maxSize) { return false; }
	else {
		++currentSize;
		return true;
	}
}

template <class T1, class T2>
class Graph;

template <class T1, class T2>
class MinSpanTree {
	friend class Graph<T1, T2>;
private:
	Graph<T1, T2> * graph;  //指向对应图的指针
	int * head;   //起点数组
	int * tail;   //终点数组
public:
	MinSpanTree(Graph<T1, T2> * g);  //构造函数
	~MinSpanTree();   //析构函数
	void print();     //输出最小生成树的边
};

template <class T1, class T2>
class Graph {    //T1为顶点数据类型，T2为边权值数据类型
	friend class MinSpanTree<T1, T2>;
private:
	int maxSize;     //顶点的最大数量
	int nVertices;   //当前顶点数量
	T1 * vertices;   //顶点数组
	T2 ** edges;     //邻接矩阵
	UFSet ufset;     //并查集, 用于检查连通性
public:
	Graph(int sz);   //构造函数
	~Graph();        //析构函数
	bool insertVertex(T1 v);                       //添加顶点
	bool insertEdge(T1 head, T1 tail, T2 cost);    //添加边
	bool isConnected();                            //判断图是否连通
	bool prim(MinSpanTree<T1, T2> & mst, T1 start);//构造Prim最小生成树
};

template <class T1, class T2>
Graph<T1, T2>::Graph(int sz) :maxSize(sz), nVertices(0), ufset(sz) {
	vertices = new T1[maxSize];
	edges = new T2*[maxSize];
	for (int i = 0; i < maxSize; ++i) {
		edges[i] = new T2[maxSize];
	}
	for (int i = 0; i < maxSize; ++i) {
		for (int j = 0; j < i; ++j) { edges[i][j] = MAXNUM; }
		edges[i][i] = 0;
		for (int j = i + 1; j < maxSize; ++j) { edges[i][j] = MAXNUM; }
	}
}

template <class T1, class T2>
Graph<T1, T2>::~Graph() {
	delete[]vertices;
	for (int i = 0; i < maxSize; ++i) {
		delete[]edges[i];
	}
	delete[]edges;
}

template <class T1, class T2>
bool Graph<T1, T2>::insertVertex(T1 v) {
	if (nVertices >= maxSize) { return false; }
	vertices[nVertices] = v;
	++nVertices;
	ufset.addElement();
	return true;
}

template <class T1, class T2>
bool Graph<T1, T2>::insertEdge(T1 head, T1 tail, T2 cost) {
	int nHead = -1, nTail = -1;
	for (int i = 0; i < maxSize; ++i) {
		if (vertices[i] == head) {
			nHead = i;
			break;
		}
	}  //查找起点坐标
	for (int i = 0; i < maxSize; ++i) {
		if (vertices[i] == tail) {
			nTail = i;
			break;
		}
	}  //查找终点坐标
	if (nHead < 0 || nTail < 0 || nHead == nTail) { return false; } //找不到起点或终点, 或起点与终点为同一点
	edges[nHead][nTail] = edges[nTail][nHead] = cost;             //修改邻接矩阵
	ufset.Union(ufset.findRoot(nHead), ufset.findRoot(nTail));    //并查集合并
	return true;
}

template <class T1, class T2>
bool Graph<T1, T2>::isConnected() {
	int r0 = ufset.findRoot(0);
	for (int i = 1; i < nVertices; ++i) {
		if (ufset.findRoot(i) != r0) { return false; }
	}
	return true;
}

template <class T1, class T2>
bool Graph<T1, T2>::prim(MinSpanTree<T1, T2> & mst, T1 start) {
	int s = -1;
	for (int i = 0; i < nVertices; ++i) {
		if (vertices[i] == start) {
			s = i;
			break;
		}
	}
	if (s == -1) { return false; }   //找不到起点
	int * nearvex = new int[nVertices];   //辅助数组, 记录最近的树内顶点, 设树内顶点nearvex = -1
	T2 * lowcost = new T2[nVertices];     //辅助数组, 记录离树的最短距离
	for (int i = 0; i < nVertices; ++i) {
		lowcost[i] = edges[i][s];     //初始化为到起点的边的长度
		nearvex[i] = s;               //初始化为起点
	}
	nearvex[s] = -1;
	for (int i = 0; i < nVertices - 1; ++i) {
		int nearest = 0;
		T2 min = MAXNUM;
		for (int j = 0; j < nVertices; ++j) {
			if (nearvex[j] != -1 && lowcost[j] < min) {
				nearest = j;
				min = lowcost[j];
			}
		}    //找离树最近的点
		mst.head[i] = nearvex[nearest];
		mst.tail[i] = nearest;
		nearvex[nearest] = -1;
		for (int k = 0; k < nVertices; ++k) {
			if (nearvex[k] != -1 && edges[nearest][k] < lowcost[k]) {
				lowcost[k] = edges[nearest][k];
				nearvex[k] = nearest;
			}
		}    //更新nearvex和lowcost
	}
	delete[]nearvex;
	delete[]lowcost;
	return true;
}

template <class T1, class T2>
MinSpanTree<T1, T2>::MinSpanTree(Graph<T1, T2> * g) :graph(g) {
	head = new int[g->nVertices - 1];
	tail = new int[g->nVertices - 1];
}

template <class T1, class T2>
MinSpanTree<T1, T2>::~MinSpanTree() {
	delete[]head;
	delete[]tail;
}

template <class T1, class T2>
void MinSpanTree<T1, T2>::print() {
	for (int i = 0; i < graph->nVertices - 1; ++i) {
		std::cout << graph->vertices[head[i]] << "-(" << graph->edges[head[i]][tail[i]] << ")->" << graph->vertices[tail[i]] << "\t\t";
	}
	std::cout << std::endl;
}

int main() {
	cout << "电网造价模拟系统" << endl;
	cout << "=====================" << endl;
	cout << "A 创建电网顶点" << endl;
	cout << "B 添加电网的边" << endl;
	cout << "C 构造最小生成树" << endl;
	cout << "D 显示最小生成树" << endl;
	cout << "E 退出程序" << endl;
	cout << "=====================" << endl << endl;
	Graph<char, int> * net = NULL;
	MinSpanTree<char, int> * tree = NULL;
	while (1) {
		cout << endl << "请选择操作: ";
		char option, c;
		cin >> option;
		switch (option) {
		case'A':
			if (net) {
				cout << "此操作将清除已有的电网，是否继续？" << endl;
				cout << "A 确定" << endl;
				cout << "B 取消" << endl;
				cin >> option;
				if (option != 'A') { break; }
				else {
					if (tree) { delete tree; tree = NULL; }
					delete net;
				}
			}
			int n;
			cout << "请输入顶点的个数: ";
			cin >> n;
			while (n < 2) {
				cout << "顶点个数最小为2，请重新输入: ";
				cin >> n;
			}
			net = new Graph<char, int>(n);
			cout << "请依次输入各顶点的名称:" << endl;
			for (int i = 0; i < n; ++i) {
				cin >> c;
				net->insertVertex(c);
			}
			break;
		case 'B':
			if (net == NULL) {
				cout << "请先创建电网!" << endl;
			}
			else {
				char head, tail;
				int cost = 1;
				while (cost != 0) {
					cout << "请输入两个顶点及边: ";
					cin >> head >> tail >> cost;
					if (cost > 0 && !net->insertEdge(head, tail, cost)) {
						cout << "起点或终点输入有误, 请重新输入!" << endl;
					}
					else if (cost < 0) {
						cout << "边的权值必须为正数!" << endl;
					}
				}
			}
			break;
		case 'C':
			if (net == NULL) {
				cout << "请先创建电网!" << endl;
			}
			else if (!net->isConnected()) {
				cout << "电网未连通!" << endl;
			}
			else {
				if (tree) { delete tree; }
				cout << "请输入起始顶点: ";
				cin >> c;
				tree = new MinSpanTree<char, int>(net);
				if (net->prim(*tree, c)) {
					cout << "生成Prim最小生成树!" << endl;
				}
				else {
					cout << "输入的顶点不存在!" << endl;
				}
			}
			break;
		case 'D':
			if (tree) { tree->print(); }
			else { cout << "请先构造最小生成树!" << endl; }
			break;
		case 'E':
			if (tree) { delete tree; }
			if (net) { delete net; }
			return 0;
		default:
			cout << "输入有误，请重新输入!" << endl;
			break;
		}
	}
	return 0;
}