
# Dijkstra算法  学习笔记 - cocoonyang的专栏 - CSDN博客


2017年11月09日 21:12:37[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：322


Dijkstra算法最初用于搜索一个正加权连通图中两顶点(起始点, 终点)之间最短路径。 它由Edsger W. Dijkstra 在1956设计,并于三年后发表[wiki].
![Dijkstra[3]](https://img-blog.csdn.net/20171121201654212?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY29jb29ueWFuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)**Dijkstra**[[3]](https://img-blog.csdn.net/20171121201654212?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY29jb29ueWFuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 算法核心思想
将图中顶点分为两个集合，一个集合$S$包含已搜索过的顶点; 另一个集合$S^{*}$是$S$的补集, 包含未搜索过的顶点.
以顶点到起始点之间最短路径为度量，为$S$中各顶点赋值.
基于$S$边界逐步向$S^{*}$扩展, 直到搜索到终点为止.
## Dijkstra算法适用范围
连通图。
图中边的加权值应为正数。
## 算法实现
Dijkstra算法的实现需要两种容器类数据结构: 一个是维护图信息,另一个保存路径信息.   Dijkstra算法所需的图操作有:
添加顶点,添加边,
设置边的权值,
提取顶点,
遍历与某个顶点相连的边和顶点.
## 代码
Rosetta提供了一个C语言版的Dijkstra算法代码[Rosetta].  现将其改写成C++版, 代码如下:
dijkstra.h
`#ifndef YANG_A_GRAPH_DIJK_H_
#define YANG_A_GRAPH_DIJK_H_
/*
 * dijkstra.h
 *
 *  Created on:
 *      Author:
 *  Based on:
 *  https://rosettacode.org/wiki/Dijkstra%27s_algorithm
 *
 *  Copyright @ 2017. CHUNFENG YANG. All Rights Reserved.
 *  Permission to use, copy, modify, and distribute this software
 *  and its documentation for educational, research, and
 *  not-for-profit purposes, without fee and without a signed
 *  above copyright notice, this paragraph and the following two
 *  distributions.
 *  IN NO EVENT SHALL CHUNFENG YANG BE LIABLE TO ANY PARTY FOR
 *  DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
 *  DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF THIS
 *  SOFTWARE AND ITS DOCUMENTATION, EVEN IF CHUNFENG YANG HAS BEEN
 *  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *  CHUNFENG YANG SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 *  BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 *  AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE AND
 *  ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS
 *  PROVIDED "AS IS". CHUNFENG YANG HAS NO OBLIGATION TO PROVIDE
 *  MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
class Edge
{
public:
	Edge();
	virtual ~Edge();
public:
	void show();
public:
	int _vertex;
	double _weight;
};
class Vertex
{
public:
	Vertex();
	virtual ~Vertex();
public:
	void addEdge(Edge*);
	void setDist(double);
	double getDist();
	void setPrev(int);
	int getPrev();
	void setVisited(int);
	int getVisited();
	int edgeLength();
	Edge* getEdge(int);
	void show();
public:
	Edge **_edges;
	int _edges_len;
	int _edges_size;
	double _dist;
	int _prev;
	int _visited;
};

class Heap
{
public:
	Heap();
	Heap(int n);
	virtual ~Heap();
public:
	void push(int v, double p);
	int pop();
	int minimize(int i, int j, int k);
	int length();
	void show();
	void clean();
public:
	int *_data;
	double *_prio;
	int *_index;
	int _len;
	int _size;
};
class Graph
{
public:
	Graph();
	virtual ~Graph();
	Graph(int);
public:
	void addVertex(int i);
	void addEdge(int a, int b, double w);
	void dijkstra(int a, int b);
	void print_path(int);
	void show();
	double distance(int i, int j);
	int length();
public:
	Vertex **_vertices;
	int _vertices_len;
	int _vertices_size;
};
#endif // YANG_A_GRAPH_DIJK_H_`[dijkstra.cc](http://dijkstra.cc)
`/*
 * dijkstra.cc
 *
 *  Created on:
 *      Author:
 *  Based on:
 *  https://rosettacode.org/wiki/Dijkstra%27s_algorithm
 *
 *  Copyright @ 2017. CHUNFENG YANG. All Rights Reserved.
 *  Permission to use, copy, modify, and distribute this software
 *  and its documentation for educational, research, and
 *  not-for-profit purposes, without fee and without a signed
 *  above copyright notice, this paragraph and the following two
 *  distributions.
 *  IN NO EVENT SHALL CHUNFENG YANG BE LIABLE TO ANY PARTY FOR
 *  DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
 *  DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF THIS
 *  SOFTWARE AND ITS DOCUMENTATION, EVEN IF CHUNFENG YANG HAS BEEN
 *  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *  CHUNFENG YANG SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 *  BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 *  AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE AND
 *  ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS
 *  PROVIDED "AS IS". CHUNFENG YANG HAS NO OBLIGATION TO PROVIDE
 *  MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 */
#include "dijkstra.h"
#include <float.h>
//
//  Edge
//
Edge::Edge()
{
	_vertex = 0;
	_weight = 0;
}
Edge::~Edge()
{
}
void Edge::show()
{
	printf("  -- vertex: %-8d  weight: %-3f  \n", _vertex, _weight);
}
//
// Vertex
//
Vertex::Vertex()
{
	_edges = NULL;
	_edges_len = 0;
	_edges_size = 0;
	_dist = 0;
	_prev = 0;
	_visited = 0;
}
Vertex::~Vertex()
{
	if (NULL != _edges)
	{
		for (int i = 0; i < _edges_len; i++)
		{
			Edge* pEdge = _edges[i];
			free(pEdge);
		}
		free(_edges);
		_edges = NULL;
		return;
	}
}
void Vertex::addEdge(Edge *e)
{
	if (NULL == e)
	{
		return;
	}
	if (_edges_len >= _edges_size)
	{
		_edges_size = _edges_size ? _edges_size * 2 : 4;
		_edges = (Edge **) realloc(_edges, _edges_size * sizeof(Edge *));
		if (NULL == _edges)
		{
			_edges_size = 0;
			_edges_len = 0;
			_dist = 0;
			_prev = 0;
			_visited = 0;
			return;
		}
	}
	_edges[_edges_len++] = e;
}
Edge* Vertex::getEdge(int index)
{
	if ((index < 0) || (index >= _edges_len))
	{
		return NULL;
	}
	if (NULL == _edges)
	{
		return NULL;
	}
	return _edges[index];
}
int Vertex::edgeLength()
{
	return _edges_len;
}
void Vertex::Vertex::setDist(double value)
{
	_dist = value;
}
double Vertex::getDist()
{
	return _dist;
}
void Vertex::setPrev(int value)
{
	_prev = value;
}
int Vertex::getPrev()
{
	return _prev;
}
void Vertex::setVisited(int value)
{
	_visited = value;
}
int Vertex::getVisited()
{
	return _visited;
}
void Vertex::show()
{
	printf("  dist = %f\t ", _dist);
	printf("prev = %d\n", _prev);
	if (NULL == _edges)
	{
		printf("  no edges \n");
		return;
	}
	for (int i = 0; i < _edges_len; i++)
	{
		Edge* pEdge = _edges[i];
		if(NULL != pEdge )
		{
			pEdge->show();
		}
	}
}
//
// Heap
//
Heap::Heap()
{
	_len = 0;
	_size = 0;
	_data = NULL;
	_prio = NULL;
	_index = NULL;
}
Heap::Heap(int n)
{
	_data = NULL;
	_prio = NULL;
	_index = NULL;
	_len = 0;
	_size = 0;
	if (n >= 0)
	{
		_data = (int*) calloc(n + 1, sizeof(int));
		_prio = (double*) calloc(n + 1, sizeof(double));
		_index = (int*) calloc(n, sizeof(int));
		if ((NULL == _data) || (NULL == _prio) || (NULL == _index))
		{
			clean();
			return;
		}
		for (int i = 0; i < n; i++)
		{
			_data[i] = 0;
			_prio[i] = 0;
			_index[i] = 0;
		}
		_data[n] = 0;
		_prio[n] = 0;
	}
	_size = n;
}
Heap::~Heap()
{
	if (NULL != _data)
	{
		free(_data);
		_data = 0;
	}
	if (NULL != _prio)
	{
		free(_prio);
		_prio = 0;
	}
	if (NULL != _index)
	{
		free(_index);
		_index = 0;
	}
}
int Heap::length()
{
	return _len;
}
void Heap::push(int v, double p)
{
	if (v < 0)
	{
		return;
	}
	if ((NULL == _data) || (NULL == _prio) || (NULL == _index))
	{
		printf("ERROR: Heap::push: _data is NULL. \n");
		return;
	}
	int i = _index[v] == 0 ? ++_len : _index[v];
	int j = i / 2;
	while (i > 1)
	{
		if (_prio[j] < p)
			break;
		_data[i] = _data[j];
		_prio[i] = _prio[j];
		_index[_data[i]] = i;
		i = j;
		j = j / 2;
	}
	_data[i] = v;
	_prio[i] = p;
	_index[v] = i;
	return;
}
int Heap::pop()
{
	if ((NULL == _data) || (NULL == _prio) || (NULL == _index))
	{
		printf("Heap::pop: data pointers invalid \n");
		return 0;
	}
	int v = _data[1];
	int i = 1;
	while (1)
	{
		int j = minimize(_len, 2 * i, 2 * i + 1);
		if (j == _len)
			break;
		_data[i] = _data[j];
		_prio[i] = _prio[j];
		_index[_data[i]] = i;
		i = j;
	}
	_data[i] = _data[_len];
	_prio[i] = _prio[_len];
	_index[_data[i]] = i;
	_len--;
	return v;
}
int Heap::minimize(int i, int j, int k)
{
	int m = i;
	if (j <= _len && _prio[j] < _prio[m])
		m = j;
	if (k <= _len && _prio[k] < _prio[m])
		m = k;
	return m;
}
void Heap::clean()
{
	free(_data);
	free(_prio);
	free(_index);
	_data = NULL;
	_prio = NULL;
	_index = NULL;
	_len = 0;
	_size = 0;
}
void Heap::show()
{
	printf("Heap::show() \n");
	printf("    len = %d\n", _len);
	for (int i = 0; i < _len; i++)
	{
		printf("data[%d] = %d\n", i, _data[i]);
		printf("dist[%d] = %f\n", i, _prio[i]);
	}
}
//
// Graph
//
Graph::Graph()
{
	_vertices = NULL;
	_vertices_len = 0;
	_vertices_size = 0;
}
Graph::Graph(int size)
{
	_vertices = NULL;
	_vertices_len = 0;
	if (size <= 0)
	{
		_vertices_size = 0;
	}
	else
	{
		_vertices_size = size;
		_vertices = (Vertex **) realloc(_vertices,
				_vertices_size * sizeof(Vertex *));
		for (int i = 0; i < _vertices_size; i++)
		{
			_vertices[i] = NULL;
		}
	}
}
Graph::~Graph()
{
	if (NULL != _vertices)
	{
		for (int i = 0; i < _vertices_len; i++)
		{
			delete _vertices[i];
		}
		free(_vertices);
		_vertices = 0;
	}
}
void Graph::addVertex(int i)
{
	if (i < 0)
	{
		printf("ERROR: Graph::addVertex: vertices id < 0. \n");
		return;
	}
	if (NULL == _vertices)
	{
		printf("WARN: Graph::addVertex: vertices pointer is NULL. \n");
		//return;
	}
	if (_vertices_size < i + 1)
	{
		int size = _vertices_size * 2 > i ? _vertices_size * 2 : i + 4;
		_vertices = (Vertex **) realloc(_vertices, size * sizeof(Vertex *));
		if (NULL == _vertices)
		{
			_vertices_len = 0;
			_vertices_size = 0;
			return;
		}
		for (int j = _vertices_size; j < size; j++)
		{
			_vertices[j] = NULL;
		}
		_vertices_size = size;
	}
	if (!_vertices[i])
	{
	_vertices[i] = new Vertex();
	_vertices_len++;
	}
	return;
}
void Graph::addEdge(int a, int b, double w)
{
	if (w < 0)
	{
		printf("ERROR: Graph::addEdge: edge weight < 0. \n");
		return;
	}
	a = a - 'a';
	b = b - 'a';
	addVertex(a);
	addVertex(b);
	Vertex *v = _vertices[a];
	Edge *e = (Edge *) calloc(1, sizeof(Edge));
	if (NULL == e)
	{
		_vertices[a] = NULL;
		return;
	}
	e->_vertex = b;
	e->_weight = w;
	v->addEdge( e );
}
void Graph::dijkstra(int a, int b)
{
	if (_vertices_len <= 0)
	{
		printf("WARN: Graph::dijkstra: vertices pointer is NULL. \n");
		return;
	}
	int i, j;
	a = a - 'a';
	b = b - 'a';
	for (i = 0; i < _vertices_len; i++)
	{
		Vertex *v = _vertices[i];
		v->setDist(FLT_MAX);
		v->setPrev(0);
		v->setVisited(0);
	}
	Vertex *v = _vertices[a];
	v->setDist(0);
	Heap *h = new Heap(_vertices_len);
	if (NULL == h)
	{
		printf("WARN: Graph::dijkstra: create heap of vertex failed. \n");
		return;
	}
	h->push(a, v->getDist());
	int len = h->length();
	while (len)
	{
		i = h->pop();
		if (i == b)
			break;
		v = _vertices[i];
		v->setVisited(1);
		for (j = 0; j < v->edgeLength(); j++)
		{
			Edge *e = v->getEdge(j);
			Vertex *u = _vertices[e->_vertex];
			if (!u->getVisited() && v->getDist() + e->_weight <= u->getDist())
			{
				u->setPrev(i);
				u->setDist(v->getDist() + e->_weight);
				h->push(e->_vertex, u->getDist());
			}
		}
	}
}
int Graph::length()
{
	return _vertices_len;
}
double Graph::distance(int i, int j)
{
	dijkstra(i, j);
	Vertex *v = _vertices[j];
	if (v->_dist == FLT_MAX)
	{
		printf("Vertex reached Maxium. \n no path\n");
		return 0.0;
	}
	double len = v->_dist;
	return len;
}
void Graph::show()
{
	printf("Graph::show() \n");
	printf(" vertices length = %d\n", _vertices_len);
	if (NULL == _vertices)
	{
		printf("ERROR: Graph::show: vertices pointer is NULL. \n");
		return;
	}
	for (int i = 0; i < _vertices_len; i++)
	{
		Vertex* pVertex = _vertices[i];
		if( NULL != pVertex )
		{
			printf("  Vertex -- %d \n", i);
			pVertex->show();
		}
		printf("\n");
	}
}
void Graph::print_path(int i)
{
	printf("Path:  \n");
	int n, j;
	Vertex *v, *u;
	i = i - 'a';
	v = _vertices[i];
	if (v->_dist == FLT_MAX)
	{
		printf("Vertex reached Maxium. \n no path\n");
		return;
	}
	double len = v->_dist;
	for (n = 1, u = v; u->_dist; u = _vertices[u->_prev], n++)
		;
	char *path = (char*) malloc(n + 1);
	if (NULL == path)
	{
		return;
	}
	path[n] = '\0';
	path[n - 1] = 'a' + i;
	for (j = 0, u = v; u->_dist; u = _vertices[u->_prev], j++)
	{
		if (NULL == u)
		{
			break;
		}
		path[n - j - 2] = 'a' + u->_prev;
	}
	printf("Length: %f  \n Route: %s \n", len, path);
	printf("%d", i);
	for (j = 0, u = v; u->_dist; u = _vertices[u->_prev], j++)
	{
		if (NULL == u)
		{
			break;
		}
		path[n - j - 2] = 0 + u->_prev;
		printf(" => %d", u->_prev );
	}
	printf(" \n", i);
	free(path);
}`
## 使用说明
为了提高程序的用户友好性和运行效率，简化代码，以a为基数设置顶点标号. 顶点标号是一个整型数，并不限于英文字母. 程序所能处理顶点数量主要受限于编译器整型数上限值和机器运算能力。
一般情况下，所有节点需要按顺序分别按顺序取一个英文字母做标号。
使用addEdge() 添加边，并设置边的加权值
使用dijkstra()搜索两顶点之间最短路径
使用print_path()显示路径
## 使用算例
[main.cc](http://main.cc)
`#include "dijkstra.h"
int main()
{
	Graph g(2);
	g.addEdge('a', 'b', 7);
	g.addEdge('a', 'c', 9);
	g.addEdge('a', 'f', 14);
	g.addEdge('b', 'c', 10);
	g.addEdge('b', 'd', 15);
	g.addEdge('c', 'd', 11);
	g.addEdge('c', 'f', 2);
	g.addEdge('d', 'e', 6);
	g.addEdge('e', 'f', 9);
	g.show();
	g.dijkstra('a', 'e');
	g.print_path('e');
	printf("Graph length = %d \n", g.length());
	return 0;
}`makefile
`# Target   
PROGRAM = test.exe  
  
inc = "../inc/"  
src = "../src/"    
INCLUDEDIRS =  /I $(inc)    
 
# Flags  
CPPOPT = $(INCLUDEDIRS) /w /EHsc /D_CRT_SECURE_NO_DEPRECATE  
  
# Compiler   
cc = cl   
link = link.exe
   
# list of source files   
CPPSOURCES =  main.cc  dijkstra.cc      
  
# expands to list of object files          
CPPOBJECTS = $(CPPSOURCES:.cc=.obj)   
    
all: $(PROGRAM)  
  
$(PROGRAM): $(CPPOBJECTS)  
    $(link) /out:$(PROGRAM)  $(CPPOBJECTS)    
      
main.obj:   
    $(cc) $(CPPOPT) /c ../src/main.cc       
 
dijkstra.obj:   
    $(cc) $(CPPOPT) /c ../src/dijkstra.cc        
          
clean:    
    del $(CPPOBJECTS) $(PROGRAM)`
## 算例运行结果
`vertices length = 6
  Vertex -- 0
  dist = 0.000000        prev = 0
  -- vertex: 1         weight: 7.000000
  -- vertex: 2         weight: 9.000000
  -- vertex: 5         weight: 14.000000
  Vertex -- 1
  dist = 0.000000        prev = 0
  -- vertex: 2         weight: 10.000000
  -- vertex: 3         weight: 15.000000
  Vertex -- 2
  dist = 0.000000        prev = 0
  -- vertex: 3         weight: 11.000000
  -- vertex: 5         weight: 2.000000
  Vertex -- 3
  dist = 0.000000        prev = 0
  -- vertex: 4         weight: 6.000000
  Vertex -- 4
  dist = 0.000000        prev = 0
  -- vertex: 5         weight: 9.000000
  Vertex -- 5
  dist = 0.000000        prev = 0
  no edges
Path:
Length: 26.000000
 Route: acde`
## 算例详解
算例使用的加权有向图如下图所示
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181031100520760.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NvY29vbnlhbmc=,size_16,color_FFFFFF,t_70)
从上图中可以看出，顶点a 与顶点e之间的最短路径是a -> c -> d -> e, 路径长度为26.
与算例程序运行结果一致。
Graph
`a  b  c  d  e  f  
a  0  7  9       14
b     0 10 15
c        0 11     2
d           0  6
e              0  9
f                 0`Dijkstra算法搜索流程图
Created with Raphaël 2.2.0a(起始点)遍历与a 关联的顶点得到ab(7), ac(9), af(14)遍历与d,f 关联的顶点得到abde(28), acde(26), af(14)遇到e？e(终点)yes
`vertices length = 6
  Vertex -- a
  dist = 0.000000        prev = 0
   --> b  weight: 7.000000
   --> c  weight: 9.000000
   --> f  weight: 14.000000
  Vertex -- b
  dist = 340282346638528859811704183484516925440.000000  prev = 0
   --> c  weight: 10.000000
   --> d  weight: 15.000000
  Vertex -- c
  dist = 340282346638528859811704183484516925440.000000  prev = 0
   --> d  weight: 11.000000
   --> f  weight: 2.000000
  Vertex -- d
  dist = 340282346638528859811704183484516925440.000000  prev = 0
   --> e  weight: 6.000000
  Vertex -- e
  dist = 340282346638528859811704183484516925440.000000  prev = 0
   --> f  weight: 9.000000
  Vertex -- f
  dist = 340282346638528859811704183484516925440.000000  prev = 0
  no edges``heap1: {a}
heap2: {ab(7), ac(9), d(~), e(~), af{14}} 
Graph::show()
vertices length = 6
 Vertex -- a
 dist = 0.000000        prev = 0
  --> b  weight: 7.000000
  --> c  weight: 9.000000
  --> f  weight: 14.000000
 Vertex -- b
 dist = 7.000000        prev = 0
  --> c  weight: 10.000000
  --> d  weight: 15.000000
 Vertex -- c
 dist = 9.000000        prev = 0
  --> d  weight: 11.000000
  --> f  weight: 2.000000
 Vertex -- d
 dist = 340282346638528859811704183484516925440.000000  prev = 0
  --> e  weight: 6.000000
 Vertex -- e
 dist = 340282346638528859811704183484516925440.000000  prev = 0
  --> f  weight: 9.000000
 Vertex -- f
 dist = 14.000000       prev = 0
 no edges``heap1: {ab(7), ac(9), af(14)}
heap2: {bd{15}, cd(11)} 
Graph::show()
 vertices length = 6
  Vertex -- a
  dist = 0.000000        prev = 0
   --> b  weight: 7.000000
   --> c  weight: 9.000000
   --> f  weight: 14.000000
  Vertex -- b
  dist = 7.000000        prev = 0
   --> c  weight: 10.000000
   --> d  weight: 15.000000
  Vertex -- c
  dist = 9.000000        prev = 0
   --> d  weight: 11.000000
   --> f  weight: 2.000000
  Vertex -- d
  dist = 22.000000       prev = 1
   --> e  weight: 6.000000
  Vertex -- e
  dist = 340282346638528859811704183484516925440.000000  prev = 0
   --> f  weight: 9.000000
  Vertex -- f
  dist = 14.000000       prev = 0
  no edges``heap1:{abd(22),acd(20),af(14)}
heap2:{de(6)}
Graph::show()
 vertices length = 6
  Vertex -- a
  dist = 0.000000        prev = 0
   --> b  weight: 7.000000
   --> c  weight: 9.000000
   --> f  weight: 14.000000
  Vertex -- b
  dist = 7.000000        prev = 0
   --> c  weight: 10.000000
   --> d  weight: 15.000000
  Vertex -- c
  dist = 9.000000        prev = 0
   --> d  weight: 11.000000
   --> f  weight: 2.000000
  Vertex -- d
  dist = 20.000000       prev = 2
   --> e  weight: 6.000000
  Vertex -- e
  dist = 340282346638528859811704183484516925440.000000  prev = 0
   --> f  weight: 9.000000
  Vertex -- f
  dist = 11.000000       prev = 2
  no edges``heap1:{abde(28), acde(26), af(14)}
heap2:{ef(9)}
Graph::show()
 vertices length = 6
  Vertex -- a
  dist = 0.000000        prev = 0
   --> b  weight: 7.000000
   --> c  weight: 9.000000
   --> f  weight: 14.000000
  Vertex -- b
  dist = 7.000000        prev = 0
   --> c  weight: 10.000000
   --> d  weight: 15.000000
  Vertex -- c
  dist = 9.000000        prev = 0
   --> d  weight: 11.000000
   --> f  weight: 2.000000
  Vertex -- d
  dist = 20.000000       prev = 2
   --> e  weight: 6.000000
  Vertex -- e
  dist = 26.000000       prev = 3
   --> f  weight: 9.000000
  Vertex -- f
  dist = 11.000000       prev = 2
  no edges`Testing Result
`Path:
Length: 26
 Route: acde`
> [wiki]
> [https://en.wikipedia.org/wiki/Dijkstra's_algorithm](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm)

> [Rosetta]
> [https://rosettacode.org/wiki/Dijkstra's_algorithm](https://rosettacode.org/wiki/Dijkstra%27s_algorithm)

> [3]
> [http://math.mit.edu/~rothvoss/18.304.3PM/Presentations/1-Melissa.pdf](http://math.mit.edu/~rothvoss/18.304.3PM/Presentations/1-Melissa.pdf)

> [4] 三角网格上的寻路算法Part.1—Dijkstra算法.
> [http://www.cnblogs.com/chnhideyoshi/p/Dijkstra.html](http://www.cnblogs.com/chnhideyoshi/p/Dijkstra.html)


