# C++模板实现循环链表 - fanyun的博客 - CSDN博客
2017年08月27日 22:04:57[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：783标签：[C++](https://so.csdn.net/so/search/s.do?q=C++&t=blog)
个人分类：[C++																[数据结构](https://blog.csdn.net/fanyun_01/article/category/7129881)](https://blog.csdn.net/fanyun_01/article/category/6102444)
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
        循环链表是另一种形式的链式存贮结构。它的特点是表中最后一个结点的指针域指向头结点，整个链表形成一个环。
循环链表分类:
（1）单循环链表——在单链表中，将终端结点的指针域NULL改为指向表头结点或开始结点即可。
（2）多重链的循环链表——将表中结点链在多个环上。
带头结点:
       判断空链表的条件是head==head->next;
尾指针:
       用尾指针rear表示的单循环链表对开始结点a1和终端结点an查找时间都是O(1)。而表的操作常常是在表的首尾位置上进行，因此，实用中多采用尾指针表示单循环链表。带尾指针的单循环链表可见下图。
注意：判断空链表的条件为rear==rear->next;
特点:
循环链表的特点是无须增加存储量，仅对表的链接方式稍作改变，即可使得表处理更加方便灵活。
使用C++模板实现简单的循环链表：
**ListNode.h**
```cpp
template<typename Type> class CircularList;
template<typename Type> class ListNode{
private:
	friend class CircularList<Type>;
	ListNode():m_pnext(NULL){}
	ListNode(const Type item,ListNode<Type> *next=NULL):m_data(item),m_pnext(next){}
	~ListNode(){
		m_pnext=NULL;
	}
	
private:
	Type m_data;
	ListNode *m_pnext;
};
```
**CircularList.h**
```cpp
#include "ListNode.h"
template<typename Type> class CircularList{
public:
	CircularList():head(new ListNode<Type>()){
		head->m_pnext=head;	
	}
	~CircularList(){
		MakeEmpty();
		delete head;
	}
public:
	void MakeEmpty();	//clear the list
	int Length();		//get the length
	ListNode<Type> *Find(Type value,int n);	//find the nth data which is equal to value
	ListNode<Type> *Find(int n);			//find the nth data
	bool Insert(Type item,int n=0);			//insert the data into the nth data of the list
	Type Remove(int n=0);					//delete the nth data
	bool RemoveAll(Type item);				//delete all the datas which are equal to value
	Type Get(int n);	//get the nth data
	void Print();		//print the list
private:
	ListNode<Type> *head;
};
template<typename Type> void CircularList<Type>::MakeEmpty(){
	ListNode<Type> *pdel,*pmove=head;
	while(pmove->m_pnext!=head){
		pdel=pmove->m_pnext;
		pmove->m_pnext=pdel->m_pnext;
		delete pdel;
	}
}
template<typename Type> int CircularList<Type>::Length(){
	ListNode<Type> *pmove=head;
	int count=0;
	while(pmove->m_pnext!=head){
		pmove=pmove->m_pnext;
		count++;
	}
	return count;
}
template<typename Type> ListNode<Type>* CircularList<Type>::Find(int n){
	if(n<0){
		cout<<"The n is out of boundary"<<endl;
		return NULL;
	}
	ListNode<Type> *pmove=head->m_pnext;
	for(int i=0;i<n&&pmove!=head;i++){
		pmove=pmove->m_pnext;
	}
	if(pmove==head){
		cout<<"The n is out of boundary"<<endl;
		return NULL;
	}
	return pmove;
}
template<typename Type> ListNode<Type>* CircularList<Type>::Find(Type value,int n){
	if(n<1){
		cout<<"The n is illegal"<<endl;
		return NULL;
	}
	ListNode<Type> *pmove=head;
	int count=0;
	while(count!=n){
		pmove=pmove->m_pnext;
		if(pmove->m_data==value){
			count++;
		}
		if(pmove==head){
			cout<<"can't find the element"<<endl;
			return NULL;
		}
	}
	return pmove;
}
template<typename Type> bool CircularList<Type>::Insert(Type item, int n){
	if(n<0){
		cout<<"The n is out of boundary"<<endl;
		return 0;
	}
	ListNode<Type> *pmove=head;
	ListNode<Type> *pnode=new ListNode<Type>(item);
	if(pnode==NULL){
		cout<<"Application error!"<<endl;
		exit(1);
	}
	for(int i=0;i<n;i++){
		pmove=pmove->m_pnext;
		if(pmove==head){
			cout<<"The n is out of boundary"<<endl;
			return 0;
		}
	}
	pnode->m_pnext=pmove->m_pnext;
	pmove->m_pnext=pnode;
	return 1;
}
template<typename Type> bool CircularList<Type>::RemoveAll(Type item){
	ListNode<Type> *pmove=head;
	ListNode<Type> *pdel=head->m_pnext;
	while(pdel!=head){
		if(pdel->m_data==item){
			pmove->m_pnext=pdel->m_pnext;
			delete pdel;
			pdel=pmove->m_pnext;
			continue;
		}
		pmove=pmove->m_pnext;
		pdel=pdel->m_pnext;
	}
	return 1;
}
template<typename Type> Type CircularList<Type>::Remove(int n){
	if(n<0){
		cout<<"can't find the element"<<endl;
		exit(1);
	}
	ListNode<Type> *pmove=head,*pdel;
	for(int i=0;i<n&&pmove->m_pnext!=head;i++){
		pmove=pmove->m_pnext;
	}
	if(pmove->m_pnext==head){
		cout<<"can't find the element"<<endl;
		exit(1);
	}
	pdel=pmove->m_pnext;
	pmove->m_pnext=pdel->m_pnext;
	Type temp=pdel->m_data;
	delete pdel;
	return temp;
}
template<typename Type> Type CircularList<Type>::Get(int n){
	if(n<0){
		cout<<"The n is out of boundary"<<endl;
		exit(1);
	}
	ListNode<Type> *pmove=head->m_pnext;
	for(int i=0;i<n;i++){
		pmove=pmove->m_pnext;
		if(pmove==head){
			cout<<"The n is out of boundary"<<endl;
			exit(1);
		}
	}
	return pmove->m_data;
}
template<typename Type> void CircularList<Type>::Print(){
	ListNode<Type> *pmove=head->m_pnext;
	cout<<"head";
	while(pmove!=head){
		cout<<"--->"<<pmove->m_data;
		pmove=pmove->m_pnext;
	}
	cout<<"--->over"<<endl<<endl<<endl;
}
```
Main.cpp
```cpp
#include <iostream>
#include "CircularList.h"
using namespace std;
int main()
{
	CircularList<int> list;
	for(int i=0;i<20;i++){
		list.Insert(i*3,i);
	}
	cout<<"the Length of the list is "<<list.Length()<<endl;
	list.Print();
	for(int i=0;i<5;i++){
		list.Insert(3,i*3);
	}
	cout<<"the Length of the list is "<<list.Length()<<endl;
	list.Print();
	list.Remove(5);
	cout<<"the Length of the list is "<<list.Length()<<endl;
	list.Print();
	list.RemoveAll(3);
	cout<<"the Length of the list is "<<list.Length()<<endl;
	list.Print();
	cout<<"The third element is "<<list.Get(3)<<endl;
	list.MakeEmpty();
	cout<<"the Length of the list is "<<list.Length()<<endl;
	list.Print();
	return 0;
}
```

