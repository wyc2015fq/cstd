# C++模板实现链式队列 - fanyun的博客 - CSDN博客
2017年08月27日 09:35:00[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：228
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
      队列 是一种特殊的线性表，它只允许在表的前端（front）进行删除操作，而在表的后端（rear）进行插入操作。进行插入操作的端称为队尾，进行删除操作的端称为队头。队列中没有元素时，称为空队列。一般队列的存储结构是顺序存储，当队列的存储结构是链式存储结构时（即队列中每个元素都包含一个指向其后继的指针，最后一个元素指针为null)，就是链式队列。
     下面我们简单实现一个链式队列：
**QueueNode.h**
```cpp
template<typename Type> class LinkQueue;
template<typename Type> class QueueNode{
private:
	friend class LinkQueue<Type>;
	QueueNode(const Type item,QueueNode<Type> *next=NULL)
		:m_data(item),m_pnext(next){}
private:
	Type m_data;
	QueueNode<Type> *m_pnext;
};
```
**LinkQueue.h**
```cpp
#include "QueueNode.h"
template<typename Type> class LinkQueue{
public:
	LinkQueue():m_prear(NULL),m_pfront(NULL){}
	~LinkQueue(){
		MakeEmpty();
	}
	void Append(const Type item);   //insert data
	Type Delete();                  //delete data
	Type GetFront();                //get data
	void MakeEmpty();               //make the queue empty
    void Print();                   //print the queue
	bool IsEmpty() const{
		return m_pfront==NULL;
	}
private:
	QueueNode<Type> *m_prear,*m_pfront;
};
template<typename Type> void LinkQueue<Type>::MakeEmpty(){
	QueueNode<Type> *pdel;
	while(m_pfront){
		pdel=m_pfront;
		m_pfront=m_pfront->m_pnext;
		delete pdel;
	}
}
template<typename Type> void LinkQueue<Type>::Append(const Type item){
	if(m_pfront==NULL){
		m_pfront=m_prear=new QueueNode<Type>(item);
	}
	else{
		m_prear=m_prear->m_pnext=new QueueNode<Type>(item);
	}
}
template<typename Type> Type LinkQueue<Type>::Delete(){
	if(IsEmpty()){
		cout<<"There is no element!"<<endl;
		exit(1);
	}
	QueueNode<Type> *pdel=m_pfront;
	Type temp=m_pfront->m_data;
	m_pfront=m_pfront->m_pnext;
	delete pdel;
	return temp;
}
template<typename Type> Type LinkQueue<Type>::GetFront(){
	if(IsEmpty()){
		cout<<"There is no element!"<<endl;
		exit(1);
	}
	return m_pfront->m_data;
}
template<typename Type> void LinkQueue<Type>::Print(){
	QueueNode<Type> *pmove=m_pfront;
	cout<<"front";
	while(pmove){
		cout<<"--->"<<pmove->m_data;
		pmove=pmove->m_pnext;
	}
	cout<<"--->rear"<<endl<<endl<<endl;
}
```
**Main.cpp**
```cpp
#include <iostream>
using namespace std;
#include "LinkQueue.h"
int main(){
	LinkQueue<int> queue;
	int init[10]={1,3,6,8,9,2,0,5,4,7};
	for(int i=0;i<10;i++){
		queue.Append(init[i]);
	}
	queue.Print();
	queue.Delete();
	queue.Print();
	cout<<queue.GetFront()<<endl;
	queue.Print();
	queue.MakeEmpty();
	queue.Print();
	queue.Delete();
	return 0;
}
```
