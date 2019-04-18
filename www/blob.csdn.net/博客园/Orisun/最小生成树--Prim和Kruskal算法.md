# 最小生成树--Prim和Kruskal算法 - Orisun - 博客园







# [最小生成树--Prim和Kruskal算法](https://www.cnblogs.com/zhangchaoyang/articles/2017211.html)





代码依据书上的例子写出来的,<<数据结构与算法分析C++描述>>Mark Allen Weiss著,张怀勇译

Prim算法:

#include<iostream>
#include<vector>
using namespace std;
const int INF=0xfffffff;
struct mark{
		bool known;
		int dv;
		int pv;

		mark(){
			dv=INF;
			pv=INF;
			known=false;
		}
	};
int main(){
	int graph[7][7]={{0,2,4,1,0,0,0},
					{2,0,0,3,10,0,0},
					{4,0,0,2,0,5,0},
					{1,3,2,0,7,8,4},
					{0,10,0,7,0,0,6},
					{0,0,5,8,0,0,1},
					{0,0,0,4,6,1,0}};
	
	vector<mark> table(7);
	table[0].dv=0;		//随便从一个节点开始,这里从第01个节点开始
	table[0].pv=-1;	
	//v1声明为已知
	while(1){
		int i;
		int mindv=INF;
		int mindex=INF;
		//从表中找到dv最小的
		for(i=0;i<7;i++){
			if(table[i].known==false){
				if(table[i].dv<mindv){
					mindv=table[i].dv;
					mindex=i;
				}
			}
		}
		if(mindv==INF)		//已经发现全部节点,退出while
			break;
		table[mindex].known=true;		//将dv最小的声明为known
		//遍历与刚声明的节点相领的且未知的节点,修改其dv和pv
		for(i=0;i<7;i++){
			if(table[i].known==false){
				if(graph[mindex][i]>0 && graph[mindex][i]<table[i].dv){
					table[i].dv=graph[mindex][i];
					table[i].pv=mindex;
				}
			}
		}
	}
	cout<<"dv\tpv"<<endl;
	for(int i=0;i<7;i++){
		cout<<table[i].dv<<"\t"<<table[i].pv+1<<endl;
	}

	return 0;
}


外层while循环是7次,内层两个for循环,长度都是7.运行时间是O(V^2),这对于稠密图来说是最优的.使用二叉堆时运行时间是O(ElogV),对于稀疏图是一个好的界.

注意这个关系:E=O(V^2),对于稀疏图,情况不会这么差.

下面的

Kruskal算法是本次的重点,因为它采用了更优化的结构,用到了优先队列(也就是堆)和不相交集.

Kruskal是贪心地连续查找权重最小的边,只要不构成回路.

下面代码中用到的DisjSets类在我的这篇博客中已给出[http://www.cnblogs.com/zhangchaoyang/articles/1999458.html](http://www.cnblogs.com/zhangchaoyang/articles/1999458.html)


下面代码还会涉及到STL中的优先队列,如果你不清楚

priority_queue的用法,建议先看一下[http://hi.baidu.com/motioo/blog/item/217ec0253763a36334a80fb3.html](http://hi.baidu.com/motioo/blog/item/217ec0253763a36334a80fb3.html)

#include"DisjSets.h"
#include<iostream>
#include<functional>
#include<vector>
#include<string>
#include<queue>
using namespace std;
struct edge_weight{
	int id;
	int weight;
	edge_weight(){
	}
	edge_weight(int id,int weight){
		this->id=id;
		this->weight=weight;
	}
	bool operator< (const edge_weight& obj)const{
		return weight > obj.weight;
	}
};
class cmp{
	public:
	bool operator()(const edge_weight & n1,const edge_weight & n2)const{
		return n1<n2;
	}
};
struct edge_ends{
	int end1;
	int end2;
	edge_ends(){
	}
	edge_ends(int a,int b){
		end1=a;
		end2=b;
	}
};
int main(){
	/**录入初始条件**/
	//录入边的权重,同时把边放入二叉堆中
	//priority_queue<edge_weight,vector<edge_weight>,greater<vector<edge_weight>::value_type> > minQP;
	priority_queue<edge_weight,vector<edge_weight>,cmp> minQP;
	minQP.push(edge_weight(0,2));		minQP.push(edge_weight(1,4));		minQP.push(edge_weight(2,1));
	minQP.push(edge_weight(3,3));		minQP.push(edge_weight(4,10));		minQP.push(edge_weight(5,2));
	minQP.push(edge_weight(6,5));		minQP.push(edge_weight(7,7));		minQP.push(edge_weight(8,8));
	minQP.push(edge_weight(9,4));		minQP.push(edge_weight(10,6));		minQP.push(edge_weight(11,1));
	//录入每条边对应的两个端点
	vector<edge_ends> vec;
	vec.push_back(edge_ends(1,2));		vec.push_back(edge_ends(1,3));		vec.push_back(edge_ends(1,4));
	vec.push_back(edge_ends(2,4));		vec.push_back(edge_ends(2,5));		vec.push_back(edge_ends(3,4));
	vec.push_back(edge_ends(3,6));		vec.push_back(edge_ends(4,5));		vec.push_back(edge_ends(4,6));
	vec.push_back(edge_ends(4,7));		vec.push_back(edge_ends(5,7));		vec.push_back(edge_ends(6,7));
	
	/**算法的初始状态**/
	int edgeAccepted=0;		//已接受的边的个数为0
	vector<int> EAP;		//已接受的边的id放入EAP中
	DisjSets ds(7);			//最初7个顶点在7个不相交的集合中
	
	/**kruskal核心代码**/
	while(edgeAccepted<7-1){
		edge_weight minEdg=minQP.top();			//贪婪做法,找出剩余边中权重最小的
		minQP.pop();
		int index=minEdg.id;
		//最小边对应的两个顶点分别是u和v
		int u=vec.at(index).end1;
		int v=vec.at(index).end2;
		//顶点编号是从1开始的,而我们不相交集类中要求元素编号从0开始,所以这里顶点号要-1
		int root1=ds.find(u-1);
		int root2=ds.find(v-1);
		if(root1!=root2){			//两个顶点在不相交集中
			EAP.push_back(index);
			edgeAccepted++;
			ds.unionSets(root1,root2);
		}
	}
	
	/**打印结果**/
	cout<<"最小生成树由下列顶点和它们之间的边组成:"<<endl;
	cout<<"顶点1\t顶点2"<<endl;
	vector<int>::iterator itr=EAP.begin();
	while(itr!=EAP.end()){
		int i=*itr;
		cout<<vec[i].end1<<"\t"<<vec[i].end2<<endl;
		++itr;
	}
	
	return 0;
}













