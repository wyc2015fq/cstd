# C++模板实现顺序队列 - fanyun的博客 - CSDN博客
2017年08月31日 22:04:58[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：625
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
       顺序队列是队列的顺序存储结构，顺序队列实际上是运算受限的顺序表。和顺序表一样，顺序队列用一个向量空间来存放当前队列中的元素。由于队列的队头和队尾的位置是变化的，设置两个指针front和rear分别指示队头元素和队尾元素在向量空间中的位置，它们的初值在队列初始化时均应设置为0。
       现在我们简单实现一个顺序队列：
**SeqQueue.h**
```cpp
template<typename Type> class SeqQueue{
public:
	SeqQueue(int sz):m_nrear(0),m_nfront(0),m_ncount(0),m_nMaxSize(sz){
		m_pelements=new Type[sz];
		if(m_pelements==NULL){
			cout<<"Application Error!"<<endl;
			exit(1);
		}
	}
	~SeqQueue(){
		delete[] m_pelements;
	}
	void MakeEmpty();               //make the queue empty
	bool IsEmpty();
	bool IsFull();
	bool Append(const Type item);   //insert data
	Type Delete();                  //delete data
	Type Get();                     //get data
	void Print();                   //print the queue
private:
	int m_nrear;
	int m_nfront;
	int m_ncount;
	int m_nMaxSize;
	Type *m_pelements;
	
};
template<typename Type> void SeqQueue<Type>::MakeEmpty(){
	this->m_ncount=0;
	this->m_nfront=0;
	this->m_nrear=0;
}
template<typename Type> bool SeqQueue<Type>::IsEmpty(){
	return m_ncount==0;
}
template<typename Type> bool SeqQueue<Type>::IsFull(){
	return m_ncount==m_nMaxSize;
}
template<typename Type> bool SeqQueue<Type>::Append(const Type item){
	if(IsFull()){
		cout<<"The queue is full!"<<endl;
		return 0;
	}
	m_pelements[m_nrear]=item;
	m_nrear=(m_nrear+1)%m_nMaxSize;
	m_ncount++;
	return 1;
}
template<typename Type> Type SeqQueue<Type>::Delete(){
	if(IsEmpty()){
		cout<<"There is no element!"<<endl;
		exit(1);
	}
	Type temp=m_pelements[m_nfront];
	m_nfront=(m_nfront+1)%m_nMaxSize;
	m_ncount--;
	return temp;
}
template<typename Type> Type SeqQueue<Type>::Get(){
	if(IsEmpty()){
		cout<<"There is no element!"<<endl;
		exit(1);
	}
	return m_pelements[m_nfront];
}
template<typename Type> void SeqQueue<Type>::Print(){
	cout<<"front";
	for(int i=0;i<m_ncount;i++){
		cout<<"--->"<<m_pelements[(m_nfront+i+m_nMaxSize)%m_nMaxSize];
	}
	cout<<"--->rear"<<endl<<endl<<endl;
}
```
**Main.cpp**
```cpp
#include <iostream>
using namespace std;
#include "SeqQueue.h"
int main(){
	SeqQueue<int> queue(10);
	int init[10]={1,6,9,0,2,5,8,3,7,4};
	for(int i=0;i<5;i++){
		queue.Append(init[i]);
	}
	queue.Print();
	cout<<queue.Delete()<<endl;
	queue.Print();
	for(int i=5;i<10;i++){
		queue.Append(init[i]);
	}
	queue.Print();
	cout<<queue.Get()<<endl;
	queue.MakeEmpty();
	queue.Print();
	queue.Append(1);
	queue.Print();
	return 0;
}
```
