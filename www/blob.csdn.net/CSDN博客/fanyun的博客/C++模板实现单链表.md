# C++模板实现单链表 - fanyun的博客 - CSDN博客
2017年08月27日 22:04:05[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：724标签：[C++](https://so.csdn.net/so/search/s.do?q=C++&t=blog)
个人分类：[C++																[数据结构](https://blog.csdn.net/fanyun_01/article/category/7129881)](https://blog.csdn.net/fanyun_01/article/category/6102444)
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
##           单链表是一种链式存取的[数据](https://baike.so.com/doc/5387430.html)结构，用一组地址任意的存储单元存放线性表中的数据[元素](https://baike.so.com/doc/5398837.html)。表中的数据是以节点来表示的，每个结点的构成:元素(数据元素的映象) + 指针(指示后继元素存储位置)，元素就是存储数据的存储单元，指针就是连接每个结点的地址数据。以"结点的序列"表示线性表称作线性链表(单链表)
## 单链表是链式存取的结构，为找第 i 个数据元素，必须先找到第 i-1 个数据元素。
       了解了单链表结构，我们来用模板实现一个简单的单链表：
//ListNode.h
```cpp
template<typename Type> class SingleList;
template<typename Type> class ListNode{
private:
	friend typename SingleList<Type>;
	ListNode():m_pnext(NULL){}
	ListNode(const Type item,ListNode<Type> *next=NULL):m_data(item),m_pnext(next){}
	~ListNode(){
		m_pnext=NULL;
	}
public:
	Type GetData();
	friend ostream& operator<< <Type>(ostream& ,ListNode<Type>&);
private:
	Type m_data;
	ListNode *m_pnext;
};
template<typename Type> Type ListNode<Type>::GetData(){
	return this->m_data;
}
template<typename Type> ostream& operator<<(ostream& os,ListNode<Type>& out){
	os<<out.m_data;
	return os;
}
```
##### SingleList.h
```cpp
#include "ListNode.h"
template<typename Type> class SingleList{
public:
	SingleList():head(new ListNode<Type>()){}
	~SingleList(){
		MakeEmpty();
		delete head;
	}
public:
	void MakeEmpty();                       //make the list empty
	int Length();                           //get the length
	ListNode<Type> *Find(Type value,int n); //find thd nth data which is equal to value
	ListNode<Type> *Find(int n);            //find the nth data
	bool Insert(Type item,int n=0);         //insert the data in the nth position
	Type Remove(int n=0);                   //remove the nth data
	bool RemoveAll(Type item);              //remove all the data which is equal to item
	Type Get(int n);                        //get the nth data
	void Print();                           //print the list
private:
	ListNode<Type> *head;
};
template<typename Type> void SingleList<Type>::MakeEmpty(){
	ListNode<Type> *pdel;
	while(head->m_pnext!=NULL){
		pdel=head->m_pnext;
		head->m_pnext=pdel->m_pnext;
		delete pdel;
	}
}
template<typename Type> int SingleList<Type>::Length(){
	ListNode<Type> *pmove=head->m_pnext;
	int count=0;
	while(pmove!=NULL){
		pmove=pmove->m_pnext;
		count++;
	}
	return count;
}
template<typename Type> ListNode<Type>* SingleList<Type>::Find(int n){
	if(n<0){
		cout<<"The n is out of boundary"<<endl;
		return NULL;
	}
	ListNode<Type> *pmove=head->m_pnext;
	for(int i=0;i<n&&pmove;i++){
		pmove=pmove->m_pnext;
	}
	if(pmove==NULL){
		cout<<"The n is out of boundary"<<endl;
		return NULL;
	}
	return pmove;
}
template<typename Type> ListNode<Type>* SingleList<Type>::Find(Type value,int n){
	if(n<1){
		cout<<"The n is illegal"<<endl;
		return NULL;
	}
	ListNode<Type> *pmove=head;
	int count=0;
	while(count!=n&&pmove){
		pmove=pmove->m_pnext;
		if(pmove->m_data==value){
			count++;
		}
	}
	if(pmove==NULL){
		cout<<"can't find the element"<<endl;
		return NULL;
	}
	return pmove;
}
template<typename Type> bool SingleList<Type>::Insert(Type item, int n){
	if(n<0){
		cout<<"The n is illegal"<<endl;
		return 0;
	}
	ListNode<Type> *pmove=head;
	ListNode<Type> *pnode=new ListNode<Type>(item);
	if(pnode==NULL){
		cout<<"Application error!"<<endl;
		return 0;
	}
	for(int i=0;i<n&&pmove;i++){
		pmove=pmove->m_pnext;
	}
	if(pmove==NULL){
		cout<<"the n is illegal"<<endl;
		return 0;
	}
	pnode->m_pnext=pmove->m_pnext;
	pmove->m_pnext=pnode;
	return 1;
}
template<typename Type> bool SingleList<Type>::RemoveAll(Type item){
	ListNode<Type> *pmove=head;
	ListNode<Type> *pdel=head->m_pnext;
	while(pdel!=NULL){
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
template<typename Type> Type SingleList<Type>::Remove(int n){
	if(n<0){
		cout<<"can't find the element"<<endl;
		exit(1);
	}
	ListNode<Type> *pmove=head,*pdel;
	for(int i=0;i<n&&pmove->m_pnext;i++){
		pmove=pmove->m_pnext;
	}
	if(pmove->m_pnext==NULL){
		cout<<"can't find the element"<<endl;
		exit(1);
	}
	pdel=pmove->m_pnext;
	pmove->m_pnext=pdel->m_pnext;
	Type temp=pdel->m_data;
	delete pdel;
	return temp;
}
template<typename Type> Type SingleList<Type>::Get(int n){
	if(n<0){
		cout<<"The n is out of boundary"<<endl;
		exit(1);
	}
	ListNode<Type> *pmove=head->m_pnext;
	for(int i=0;i<n;i++){
		pmove=pmove->m_pnext;
		if(NULL==pmove){
			cout<<"The n is out of boundary"<<endl;
			exit(1);
		}
	}
	return pmove->m_data;
}
template<typename Type> void SingleList<Type>::Print(){
	ListNode<Type> *pmove=head->m_pnext;
	cout<<"head";
	while(pmove){
		cout<<"--->"<<pmove->m_data;
		pmove=pmove->m_pnext;
	}
	cout<<"--->over"<<endl<<endl<<endl;
}
```
Main.cpp
```cpp
#include <iostream>
using namespace std;
#include "SingleList.h"
int main()
{
	SingleList<int> list;
	for(int i=0;i<20;i++){
		list.Insert(i*3,i);
	}
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
	cout<<*list.Find(18,1)<<endl;
	list.Find(100);
	list.MakeEmpty();
	cout<<"the Length of the list is "<<list.Length()<<endl;
	list.Print();
	return 0;
}
```
