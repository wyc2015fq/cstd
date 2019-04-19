# C++模板实现顺序表 - fanyun的博客 - CSDN博客
2017年08月27日 22:05:20[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：932标签：[C++](https://so.csdn.net/so/search/s.do?q=C++&t=blog)
个人分类：[C++																[数据结构](https://blog.csdn.net/fanyun_01/article/category/7129881)](https://blog.csdn.net/fanyun_01/article/category/6102444)
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
顺序表是在计算机内存中以数组的形式保存的线性表，是指用一组地址连续的存储单元依次存储数据元素的线性结构。线性表采用顺序存储的方式存储就称之为顺序表。顺序表是将表中的结点依次存放在计算机内存中一组地址连续的存储单元中。
将表中元素一个接一个的存入一组连续的存储单元中，这种存储结构是顺序结构。
采用顺序存储结构的线性表简称为" 顺序表"。顺序表的存储特点是:只要确定了起始位置，表中任一元素的地址都通过下列公式得到:LOC(ai)=LOC(a1)+(i-1)*L 1≤i≤n 其中，L是元素占用存储单元的长度。
顺序表的结构定义:
#define maxlen 50 //定义顺序表中元素个数最多有几个
typedef struct
{
elementtype data[maxlen]; //elementtype是元素的类型 依具体情况而定
int listlen; //便于时刻了解顺序表里元素的个数
}seqlist; //顺序表的名称 不妨为seqlist
声明顺序表类型变量:
seqlist L,L1;
如顺序表的每个结点占用len个内存单元，用location (ki)表示顺序表中第i个结点ki所占内存空间的第1个单元的地址。则有如下的关系:location (ki+1) = location (ki) +len
location (ki) = location(k1) + (i-1)len
存储结构要体现数据的逻辑结构，顺序表的存储结构中，内存中物理地址相邻的结点一定具有顺序表中的逻辑关系。
下面我们用模板实现一个简单的顺序表：
**Seqlist.h**
```cpp
const int DefaultSize=100;
template <typename Type> class SeqList{
public:
	SeqList(int sz=DefaultSize)
		:m_nmaxsize(sz),m_ncurrentsize(-1){
		if(sz>0){
			m_elements=new Type[m_nmaxsize];
		}
	}
	~SeqList(){
		delete[] m_elements;
	}
	int Length() const{					//get the length
		return m_ncurrentsize+1;
	}
	int Find(Type x) const;				//find the position of x
	int IsElement(Type x) const;		//is it in the list
	int Insert(Type x,int i);			//insert data
	int Remove(Type x);					//delete data
	int IsEmpty(){
		return m_ncurrentsize==-1;
	}
	int IsFull(){
		return m_ncurrentsize==m_nmaxsize-1;
	}
	Type Get(int i){					//get the ith data
		return i<0||i>m_ncurrentsize?(cout<<"can't find the element"<<endl,0):m_elements[i];
	}
	void Print();
private:
	Type *m_elements;
	const int m_nmaxsize;
	int m_ncurrentsize;
};
template <typename Type> int SeqList<Type>::Find(Type x) const{
	for(int i=0;i<m_ncurrentsize;i++)
		if(m_elements[i]==x)
			return i;
	cout<<"can't find the element you want to find"<<endl;
	return -1;
}
template <typename Type> int SeqList<Type>::IsElement(Type x) const{
	if(Find(x)==-1)
		return 0;
	return 1;
}
template <typename Type> int SeqList<Type>::Insert(Type x, int i){
	if(i<0||i>m_ncurrentsize+1||m_ncurrentsize==m_nmaxsize-1){
		cout<<"the operate is illegal"<<endl;
		return 0;
	}
	m_ncurrentsize++;
	for(int j=m_ncurrentsize;j>i;j--){
		m_elements[j]=m_elements[j-1];
	}
	m_elements[i]=x;
	return 1;
}
template <typename Type> int SeqList<Type>::Remove(Type x){
	int size=m_ncurrentsize;
	for(int i=0;i<m_ncurrentsize;){
		if(m_elements[i]==x){
			for(int j=i;j<m_ncurrentsize;j++){
				m_elements[j]=m_elements[j+1];
			}
			m_ncurrentsize--;
			continue;
		}
		i++;
	}
	if(size==m_ncurrentsize){
		cout<<"can't find the element you want to remove"<<endl;
		return 0;
	}
	return 1;
}
template <typename Type> void SeqList<Type>::Print(){
	for(int i=0;i<=m_ncurrentsize;i++)
		cout<<i+1<<":\t"<<m_elements[i]<<endl;
	cout<<endl<<endl;
}
```
**Main.cpp**
```cpp
#include <iostream>
#include "SeqList.h"
using namespace std;
int main()
{
	SeqList<int> test(15);
	int array[15]={2,5,8,1,9,9,7,6,4,3,2,9,7,7,9};
	for(int i=0;i<15;i++){
		test.Insert(array[i],0);
}
	test.Insert(1,0);
	cout<<(test.Find(0)?"can't be found ":"Be found ")<< 0 << endl<<endl;
	test.Remove(7);
	test.Print();
	test.Remove(9);
	test.Print();
	test.Remove(0);
	test.Print();
	return 0;
}
```

