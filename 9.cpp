#include <iostream>
#include <string>
#include <fstream>

using namespace std;
const int DEFAULTSIZE = 2;

template <class T>
class Array {
private:
	T * ptr;       //����ָ��
	int len;       //��ǰ����
	int maxsize;   //�������
	bool resize(); //����
public:
	Array();                         //���캯��
	Array(int sz);                   //���캯��, ָ������
	Array(const Array<T> & ar);      //���ƹ��캯��
	~Array();                        //��������
	int length() { return len; }     //��ȡ��ǰ����
	T & operator[](int n);           //���±����
	void insert(const T & x, int n); //��x���뵽n֮��
	void pushback(const T & x);      //��β�����Ԫ��
	T remove(int n);                 //ɾ���±�Ϊn��Ԫ��
	void clear() { len = 0; }        //�������
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

//���ó��߱�洢����
struct Edge {
	int dest;
	Edge * next;
};

//����Ŀγ�������Course�ṹ������洢
struct Course {
	string courseNum;  //�γ̴���
	string name;       //�γ�����
	int time;          //ѧʱ��
	int semester;      //ָ������ѧϰ
	Array<string> pre; //���޿γ�
};

//��ѡ�̶�����ѧ�ڵĿγ�, ����γ̴���ͼ��
class Vertex {       //������(��ָ������ѧ�ڵĿγ�)
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

struct English {  //ָ������ѧ�ڵĿγ�
	string name;
	int time;
};

class Graph {
private:
	int nSemesters;         //ѧ����
	Array<Vertex> vertices; //����(��ָ������ѧ�ڵĿ�)
	Array<English> * eng;   //�洢ÿ��ѧ�ڵ�ָ������ѧ�ڵĿγ�
	Array<int> indeg0;      //���Ϊ0�Ķ�����±�, �����ȵ�������
	int findCourse(string num);  //���ݿγ̴�����ҿγ̵��±�
	int arrange(string cname, string ** l, int n, int before = -1); //����ѧʱ��Ϊ1, 2, 3���ſ�
public:
	Graph(Course data[], int len, int nsem);  //���캯��
	~Graph();   //��������
	bool semesterArrange(int smt, int nCourse, string ** l);  //��һ��ѧ�ڵĿγ�
};

int Graph::findCourse(string num) {   //���Ҹ����γ̴���Ŀγ���vertices���е�λ��
	for (int i = 0; i < vertices.length(); ++i) {
		if (vertices[i].courseNum == num) { return i; }
	}
	cout << "�γ̲�����!" << endl;
	return -1;
}

Graph::Graph(Course data[], int len, int nsem) :nSemesters(nsem) {  //���캯��
	eng = new Array<English>[nsem];   //�洢ÿ��ѧ�ڵ�ָ������ѧ�ڵĿγ�
	vertices.clear();
	indeg0.clear();
	for (int i = 0; i < len; ++i) {     //��һ��ɨ��γ���������data, ����γ����ơ��γ̴��뼰ѧʱ��
		if (data[i].semester == 0) {    //��ָ������ѧ��, �洢��vertices������
			Course c = data[i];
			Vertex v(0, 0, c.name, c.courseNum, c.time);
			vertices.pushback(v);
		}
		else {   //ָ������ѧ��, �洢��eng������
			English e;
			e.name = data[i].name;
			e.time = data[i].time;
			eng[data[i].semester - 1].pushback(e);
		}
	}
	for (int i = 0; i < len; ++i) {    //�ڶ���ɨ��γ���������data, �����ڽӱ�
		if (data[i].semester == 0) {   //��ָ��ѧ��
			int npre = data[i].pre.length(), pos;  //npreΪ���޿�����
			pos = findCourse(data[i].courseNum);   //posΪdata�����i�ſ���vertices�����е��±�
			vertices[pos].inDegree = npre;
			for (int k = 0; k < npre; ++k) {       //data[i]��ÿһ�����޿���Ӻ��data[i]
				int prepos = findCourse(data[i].pre[k]);  //Ѱ��ǰ����vertices�����е��±�
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
	for (int i = 0; i < vertices.length(); ++i) {  //�洢���Ϊ0�Ķ���
		if (vertices[i].inDegree == 0) {
			//���ֲ���, �����ȵ���
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
	//����ѧʱ��Ϊ1, 2, 3���ſ�, ������ʱ���ɷֽ�Ϊ1, 2, 3�����
	//cnameΪҪ�ſεĿγ���, lΪ�洢�γ̱�Ķ�ά����, nΪѧʱ��, ֻ��ȡ1, 2, 3
	//before����Ϊ�ϴ��Ͽ�ʱ��, ������Ϊ�״��ſ�, before = -1, before��Ĭ��ֵΪ-1
	int temp = -1;
	if (before == -1) {
		switch (n) {
		case 1:    //�����ŵ�3���8��
			for (int d = 0; d < 5; ++d) {
				if (l[d][2] == "��") {
					l[d][2] = cname;
					return d;
				}
				if (l[d][7] == "��") {
					l[d][7] = cname;
					return d;
				}
			}
			for (int d = 0; d < 5; ++d) {  //�޷������ڵ�3���8��, ����������ʱ��
				for (int c = 0; c < 9; ++c) {
					if (l[d][c] == "��") {
						l[d][c] = cname;
						return d;
					}
				}
			}
			cout << "�޷��ſΣ�" << endl;
			return -1;
			break;
		case 2:     //�����ŵ�1���6��
			for (int d = 0; d < 5; ++d) {
				if (l[d][0] == "��" && l[d][1] == "��") {
					l[d][0] = l[d][1] = cname;
					return d;
				}
				if (l[d][5] == "��" && l[d][6] == "��") {
					l[d][5] = l[d][6] = cname;
					return d;
				}
			}
			for (int d = 0; d < 5; ++d) {    //�ŵ�4���9��
				if (l[d][3] == "��" && l[d][4] == "��") {
					l[d][3] = l[d][4] = cname;
					return d;
				}
				if (l[d][8] == "��" && l[d][9] == "��") {
					l[d][8] = l[d][9] = cname;
					return d;
				}
			}
			//û������2��, ��ֳ�1+1
			temp = arrange(cname, l, 1);
			if (temp != -1) { temp = arrange(cname, l, 1, temp); }
			return temp;
			break;
		case 3:   //�����ŵ�3���8��
			for (int d = 0; d < 5; ++d) {
				if (l[d][2] == "��" && l[d][3] == "��" && l[d][4] == "��") {
					l[d][2] = l[d][3] = l[d][4] = cname;
					return d;
				}
				if (l[d][7] == "��" && l[d][8] == "��" && l[d][9] == "��") {
					l[d][7] = l[d][8] = l[d][9] = cname;
					return d;
				}
			}
			//û������3��, ��ֳ�1+2
			temp = arrange(cname, l, 2);
			if (temp != -1) { temp = arrange(cname, l, 1, temp); }
			return temp;
		default:
			return -1;
		}
	}
	else {  //��Ҫ���쿪��
		switch (n) {
		case 1:    //�����ŵ�3���8��
			for (int i = 0, d = (before + 2) % 5; i < 2; ++i, d = (d + 1) % 5) {
				if (l[d][2] == "��") {
					l[d][2] = cname;
					return d;
				}
				if (l[d][7] == "��") {
					l[d][7] = cname;
					return d;
				}
			}
			for (int i = 0, d = (before + 2) % 5; i < 2; ++i, d = (d + 1) % 5) {
				for (int c = 0; c < 9; ++c) {
					if (l[d][c] == "��") {
						l[d][c] = cname;
						return d;
					}
				}
			}
			return arrange(cname, l, 1);  //�޷������������
			break;
		case 2:    //�����ŵ�1���6��
			for (int i = 0, d = (before + 2) % 5; i < 2; ++i, d = (d + 1) % 5) {
				if (l[d][0] == "��" && l[d][1] == "��") {
					l[d][0] = l[d][1] = cname;
					return d;
				}
				if (l[d][5] == "��" && l[d][6] == "��") {
					l[d][5] = l[d][6] = cname;
					return d;
				}
			}
			for (int i = 0, d = (before + 2) % 5; i < 2; ++i, d = (d + 1) % 5) {    //�ŵ�4���9��
				if (l[d][3] == "��" && l[d][4] == "��") {
					l[d][3] = l[d][4] = cname;
					return d;
				}
				if (l[d][8] == "��" && l[d][9] == "��") {
					l[d][8] = l[d][9] = cname;
					return d;
				}
			}
			return arrange(cname, l, 2);  //�޷������������
			break;
		case 3:   //�����ŵ�3���8��
			for (int i = 0, d = (before + 2) % 5; i < 2; ++i, d = (d + 1) % 5) {
				if (l[d][2] == "��" && l[d][3] == "��" && l[d][4] == "��") {
					l[d][2] = l[d][3] = l[d][4] = cname;
					return d;
				}
				if (l[d][7] == "��" && l[d][8] == "��" && l[d][9] == "��") {
					l[d][7] = l[d][8] = l[d][9] = cname;
					return d;
				}
			}
			return arrange(cname, l, 3);  //�޷������������
		default:   //����n����
			return -1;
		}
	}
}

bool Graph::semesterArrange(int smt, int nCourse, string ** l) {   //��һ��ѧ�ڵĿγ�
	//smtΪҪ�ſε�ѧ��, nCourseΪ��ѧ�ڵĿ�������, lΪ�洢�γ̱�Ķ�ά����
	if (nCourse < eng[smt].length()) {
		cout << "�޷��ſΣ���" << smt + 1<< "ѧ�ڿ���γ̹��٣�" << endl;
		return false;
	}
	Array<int> tmparr;
	for (int i = 0; i < nCourse - eng[smt].length(); ++i) {
		if (indeg0.length() != 0) {
			//ȡ�������������Ϊ0�Ŀγ�
			int tmp = indeg0.remove(indeg0.length() - 1);
			Edge * p1 = vertices[tmp].firstNeighbor;
			while (p1 != NULL) {
				tmparr.pushback(p1->dest);
				p1 = p1->next;
			}
			//�ſ�
			//��ά����l���ڴ洢��ѧ�ڿγ̱�, ���±�0-4�ֱ������һ������, ���±����ڴΡ�
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
			cout << "�޷��ſΣ����������޿�Ҫ�󡣵�" << smt + 1 << "ѧ�ڿ���γ̹��࣡" << endl;
			return false;
		}
	}
	for (int i = 0; i < tmparr.length(); ++i) {
		if (--vertices[tmparr[i]].inDegree == 0) {  //������Ϊ0, ѹ��ջ��
			//���ֲ���
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
	int nTotal, nSem;  //�γ�����, ѧ����
	cout << "������γ�����: ";
	cin >> nTotal;
	cout << "������ѧ����: ";
	cin >> nSem;
	Course * data = new Course[nTotal];
	int * eachSem = new int[nSem];
	cout << "������ÿ��ѧ�ڵĿ�������: ";
	int s = 0;
	for (int i = 0; i < nSem; ++i) {
		cin >> eachSem[i];
		s += eachSem[i];
	}
	if (s != nTotal) {
		cout << "�������������ڿγ�������" << endl;
		return 0;
	}
	cout << "������γ���Ϣ:" << endl;
	string p;
	ofstream courseout("Course.txt", ios::out);
	courseout << "�γ̱��\t�γ�����\t\tѧʱ��\tָ������ѧ��\t���޿γ�" << endl;
	for (int i = 0; i < nTotal; ++i) {    //����ÿһ�ſγ���Ϣ, �洢��data������, ͬʱд���ļ���
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
				table[k][j] = "��";
			}
		} //�γ̱��ʼ��Ϊ��
		if (!g.semesterArrange(i, eachSem[i], table)) { break; }
		out << "��" << i + 1 << "ѧ�ڿγ̱�" << endl << endl << "\t��һ\t\t�ܶ�\t\t����\t\t����\t\t����" << endl;
		for (int r = 0; r < 10; ++r) {
			out << "��" << r + 1 << "��\t";
			for (int c = 0; c < 5; ++c) {
				if (table[c][r].length() > 8){ out << table[c][r] << "\t"; }
				else { out << table[c][r] << "\t\t"; }
			}
			out << endl;
		}
		out << endl;
	} //��ÿ��ѧ�ڽ����ſ�, �����ſν��д���ļ���
	for (int j = 0; j < 5; ++j) { delete[]table[j]; }
	delete[]table;
	delete[]eachSem;
	delete[]data;
	return 0;
}