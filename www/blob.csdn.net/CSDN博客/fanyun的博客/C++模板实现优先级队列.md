# C++模板实现优先级队列 - fanyun的博客 - CSDN博客
2017年08月27日 10:06:42[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：258
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
      如果我们给每个元素都分配一个数字来标记其优先级，不妨设较小的数字具有较高的优先级，这样我们就可以在一个集合中访问优先级最高的元素并对其进行查找和删除操作了。这样，我们就引入了优先级队列 这种数据结构。 优先级队列（priority queue） 是0个或多个元素的集合，每个元素都有一个优先权。
对优先级队列执行的操作有：
（1）.查找
（2）.插入一个新元素 
（3）.删除 一般情况下，查找操作用来搜索优先权最大的元素，删除操作用来删除该元素 。
      对于优先权相同的元素，可按先进先出次序处理或按任意优先权进行。优先级队列 是不同于先进先出队列的另一种队列。每次从队列中取出的是具有最高优先权的元素。
下面我们使用模板实现一个简答优先级队列：
** QueueNode.h**
```cpp
template<typename Type,typename Cmp> class PriorityQueue;
template<typename Type,typename Cmp> class QueueNode{
private:
	friend class PriorityQueue<Type,Cmp>;
	QueueNode(const Type item,QueueNode<Type,Cmp> *next=NULL)
		:m_data(item),m_pnext(next){}
private:
	Type m_data;
	QueueNode<Type,Cmp> *m_pnext;
};
```
Compare.h
```cpp
template<typename Type> class Compare{	//处理一般比较大小
public:
	static bool lt(Type item1,Type item2);
};
template<typename Type> bool Compare<Type>::lt(Type item1, Type item2){
	return item1<item2;
}
struct SpecialData{
	friend ostream& operator<<(ostream& ,SpecialData &);
	int m_ntenor;
	int m_npir;
};
ostream& operator<<(ostream& os,SpecialData &out){
	os<<out.m_ntenor<<"   "<<out.m_npir;
	return os;
}
class SpecialCmp{		//处理特殊比较大小,用户可添加适当的类
public:
	static bool lt(SpecialData item1,SpecialData item2);
};
bool SpecialCmp::lt(SpecialData item1, SpecialData item2){
	return item1.m_npir<item2.m_npir;
}
```
**PriorityQueue.h**
```cpp
#include "QueueNode.h"
#include "Compare.h"
template<typename Type,typename Cmp> class PriorityQueue{	//Cmp is Designed for compare
public:
	PriorityQueue():m_prear(NULL),m_pfront(NULL){}
	~PriorityQueue(){
		MakeEmpty();
	}
	void MakeEmpty();               //make the queue empty
	void Append(const Type item);   //insert data
	Type Delete();                  //delete data
	Type GetFront();                //get data
    void Print();                   //print the queue
        
	bool IsEmpty() const{           
		return m_pfront==NULL;
	}
	
private:
	QueueNode<Type,Cmp> *m_prear,*m_pfront;
};
template<typename Type,typename Cmp> void PriorityQueue<Type,Cmp>::MakeEmpty(){
	QueueNode<Type,Cmp> *pdel;
	while(m_pfront){
		pdel=m_pfront;
		m_pfront=m_pfront->m_pnext;
		delete pdel;
	}
}
template<typename Type,typename Cmp> void PriorityQueue<Type,Cmp>::Append(const Type item){
	if(m_pfront==NULL){
		m_pfront=m_prear=new QueueNode<Type,Cmp>(item);
	}
	else{
		m_prear=m_prear->m_pnext=new QueueNode<Type,Cmp>(item);
	}
}
template<typename Type,typename Cmp> Type PriorityQueue<Type,Cmp>::Delete(){
	if(IsEmpty()){
		cout<<"There is no elements!"<<endl;
		exit(1);
	}
	QueueNode<Type,Cmp> *pdel=m_pfront,*pmove=m_pfront;
	while(pmove->m_pnext){  //get the minimize priority's data
        //cmp:: lt is used for compare the two data, if the front one 
        //      is less than the back, then return 1
		if(Cmp::lt(pmove->m_pnext->m_data,pdel->m_pnext->m_data)){
			pdel=pmove;
		}
		pmove=pmove->m_pnext;
	}
	pmove=pdel;
	pdel=pdel->m_pnext;
	pmove->m_pnext=pdel->m_pnext;
	Type temp=pdel->m_data;
	delete pdel;
	return temp;
}
template<typename Type,typename Cmp> Type PriorityQueue<Type,Cmp>::GetFront(){
	if(IsEmpty()){
		cout<<"There is no elements!"<<endl;
		exit(1);
	}
	QueueNode<Type,Cmp> *pdel=m_pfront,*pmove=m_pfront->m_pnext;
	while(pmove){   //get the minimize priority's data
		if(Cmp::lt(pmove->m_data,pdel->m_data)){
			pdel=pmove;
		}
		pmove=pmove->m_pnext;
	}
	return pdel->m_data;
}
template<typename Type,typename Cmp> void PriorityQueue<Type,Cmp>::Print(){
	QueueNode<Type,Cmp> *pmove=m_pfront;
	cout<<"front";
	while(pmove){
		cout<<"--->"<<pmove->m_data;
		pmove=pmove->m_pnext;
	}
	cout<<"--->rear"<<endl<<endl<<endl;
}
```
Main.cpp
```cpp
#include <iostream>
#include <cstdlib>
using namespace std;
#include "PriorityQueue.h"
int main(){
	PriorityQueue<int,Compare<int> > queue;
	int init[10]={1,9,3,5,0,8,2,4,6,7};
	for(int i=0;i<10;i++){
		queue.Append(init[i]);
	}
	queue.Print();
	queue.Delete();
	queue.Print();
	system("pause");
	system("cls");
	
	PriorityQueue<SpecialData,SpecialCmp> spe_queue;
	int init2[5][2]={{34,2},{64,1},{18,3},{24,2},{55,4}};
	SpecialData data[5];
	for(int i=0;i<5;i++){
		data[i].m_npir=init2[i][1];
		data[i].m_ntenor=init2[i][0];
	}
	for(int i=0;i<5;i++){
		spe_queue.Append(data[i]);
	}
	spe_queue.Print();
    cout<<spe_queue.GetFront()<<endl<<endl;
	spe_queue.Delete();
	spe_queue.Print();
	
	
	return 0;
}
```

