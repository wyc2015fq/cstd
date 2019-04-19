# C++模板实现链式栈 - fanyun的博客 - CSDN博客
2017年08月31日 22:04:33[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：619
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
      链式栈是一种数据存储结构，可以通过单链表的方式来实现，使用链式栈的优点在于它能够克服用数组实现的顺序栈空间利用率不高的特点，但是需要为每个栈元素分配额外的指针空间用来存放指针域。
     下面我们使用模板实现一个简单链式栈：
**StackNode.h**
```cpp
template<typename Type> class LinkStack;
template<typename Type> class StackNode{
private:
	friend class LinkStack<Type>;
	StackNode(Type dt,StackNode<Type> *next=NULL):m_data(dt),m_pnext(next){} 
private:
	Type m_data;
	StackNode<Type> *m_pnext;
};
```
**LinkStack.h**
```cpp
#include "StackNode.h"
template<typename Type> class LinkStack{
public:
	LinkStack():m_ptop(NULL){}
	~LinkStack(){
		MakeEmpty();
	}
public:
	void MakeEmpty();           //make the stack empty
	void Push(const Type item); //push the data
	Type Pop();                 //pop the data
	Type GetTop() const;        //get the data
    void Print();               //print the stack
        
	bool IsEmpty() const{
		return m_ptop==NULL;
	}
	
private:
	StackNode<Type> *m_ptop;
};
template<typename Type> void LinkStack<Type>::MakeEmpty(){
	StackNode<Type> *pmove;
	while(m_ptop!=NULL){
		pmove=m_ptop;
		m_ptop=m_ptop->m_pnext;
		delete pmove;
	}
}
template<typename Type> void LinkStack<Type>::Push(const Type item){
	m_ptop=new StackNode<Type>(item,m_ptop);
}
template<typename Type> Type LinkStack<Type>::GetTop() const{
	if(IsEmpty()){
		cout<<"There is no elements!"<<endl;
		exit(1);
	}
	return m_ptop->m_data;
}
template<typename Type> Type LinkStack<Type>::Pop(){
	if(IsEmpty()){
		cout<<"There is no elements!"<<endl;
		exit(1);
	}
	StackNode<Type> *pdel=m_ptop;
	m_ptop=m_ptop->m_pnext;
	Type temp=pdel->m_data;
	delete pdel;
	return temp;
}
template<typename Type> void LinkStack<Type>::Print(){
	StackNode<Type> *pmove=m_ptop;
	cout<<"buttom";
	while(pmove!=NULL){
		cout<<"--->"<<pmove->m_data;
		pmove=pmove->m_pnext;
	}
	cout<<"--->top"<<endl<<endl<<endl;
}
```
Main.cpp
```cpp
#include <iostream>
using namespace std;
#include "LinkStack.h"
int main(){
	LinkStack<int> stack;
	int init[10]={1,3,5,7,4,2,8,0,6,9};
	for(int i=0;i<10;i++){
		stack.Push(init[i]);
	}
	stack.Print();
	cout<<stack.Pop()<<endl;
	stack.Print();
	
	cout<<stack.GetTop()<<endl;
	stack.Print();
	cout<<stack.Pop()<<endl;
	stack.Print();
	stack.MakeEmpty();
	stack.Print();
	
	stack.Pop();
	return 0;
}
```
