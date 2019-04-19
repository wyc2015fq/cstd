# C++模板实现顺序栈 - fanyun的博客 - CSDN博客
2017年08月27日 22:05:37[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：432标签：[C++](https://so.csdn.net/so/search/s.do?q=C++&t=blog)
个人分类：[C++																[数据结构](https://blog.csdn.net/fanyun_01/article/category/7129881)](https://blog.csdn.net/fanyun_01/article/category/6102444)
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
                
       顺序栈：利用一组连续的存储单元依次存放自栈底到栈顶的数据元素；由于栈顶元素是经常变动的，所以附设top指示栈顶元素在顺序表中的位置，同时也需要知道顺序栈存储空间的起始位置，因此还需设定一个base指针用来指示栈空间的起始位置。
      一般约定top指针指向栈顶元素的下一个位置，即新数据元素将要插入得位置。
      下面我们使用模板简单实现一个顺序栈：
**SeqStack.h**
```cpp
template<typename Type> class SeqStack{
public:
	SeqStack(int sz):m_ntop(-1),m_nMaxSize(sz){
		m_pelements=new Type[sz];
		if(m_pelements==NULL){
			cout<<"Application Error!"<<endl;
			exit(1);
		}
	}
	~SeqStack(){
		delete[] m_pelements;
	}
public:
	void Push(const Type item); //push data
	Type Pop();                 //pop data
	Type GetTop() const;        //get data
    void Print();               //print the stack
	void MakeEmpty(){           //make the stack empty
		m_ntop=-1;
	}
	bool IsEmpty() const{
		return m_ntop==-1;
	}
	bool IsFull() const{
		return m_ntop==m_nMaxSize-1;
	}
	
private:
	int m_ntop;
	Type *m_pelements;
	int m_nMaxSize;
};
template<typename Type> void SeqStack<Type>::Push(const Type item){
	if(IsFull()){
		cout<<"The stack is full!"<<endl;
		return;
	}
	m_pelements[++m_ntop]=item;
}
template<typename Type> Type SeqStack<Type>::Pop(){
	if(IsEmpty()){
		cout<<"There is no element!"<<endl;
		exit(1);
	}
	return m_pelements[m_ntop--];
}
template<typename Type> Type SeqStack<Type>::GetTop() const{
	if(IsEmpty()){
		cout<<"There is no element!"<<endl;
		exit(1);
	}
	return m_pelements[m_ntop];
}
template<typename Type> void SeqStack<Type>::Print(){
	cout<<"bottom";
	for(int i=0;i<=m_ntop;i++){
		cout<<"--->"<<m_pelements[i];
	}
	cout<<"--->top"<<endl<<endl<<endl;
}
```
**Main.cpp**
```cpp
#include<iostream>
using namespace std;
#include "SeqStack.h"
int main(){
	SeqStack<int> stack(10);
	int init[10]={1,2,6,9,0,3,8,7,5,4};
	for(int i=0;i<10;i++){
		stack.Push(init[i]);
	}
	stack.Print();
	stack.Push(88);
	cout<<stack.Pop()<<endl;
	stack.Print();
	
	stack.MakeEmpty();
	stack.Print();
	stack.Pop();
	return 0;
}
```
