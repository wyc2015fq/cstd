
# C++桶排序 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年10月28日 14:53:16[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：3124


C++桶排序
\#include "stdafx.h"
\#include<cmath>
\#include<iostream>
using namespace std;
struct Node                          //结构体
{
double value;
Node *next;
};
int _tmain(int argc, _TCHAR* argv[])
{
Node key[10];
int i;
for(i=0;i<10;i++)
{
key[i].value = 0;
key[i].next = NULL;
}
cout<<"请输入排序案例个数："<<endl;
int cases;
cin>>cases;
Node *p,*q;
while(cases--)
{
cout<<"请输入排序的元素（大于等于0小于1）（以-1结束）："<<endl;
double number;
cin>>number;
while(abs(number-(-1))>0.0000001)                    //输入需排序的元素，并建立对应的桶的链表，每个桶内采用插入排序
{
Node *nodes = new Node() ;
nodes->value = number;
nodes->next = NULL;
int n = (int)(number*10);
if(key[n].next==NULL)
{
key[n].next = nodes;
nodes->next = NULL;
}
else
{
p = &key[n];
q =  key[n].next;
while(q!=NULL&&q->value<=number)
{
q=q->next;
p=p->next;
}
nodes->next = q;
p->next = nodes;
}
cin>>number;
}
cout<<"排序之后："<<endl;                    // 输出排序后的元素
for(i=0;i<10;i++)
{
p = key[i].next;
if(p==NULL)
continue;
while(p!=NULL)
{
cout<<p->value<< " ";
p=p->next;
}
}
cout<<endl;
for(i=0;i<10;i++)                                       //用delete回收new创建的空间
{
p = key[i].next;
if(p==NULL)
continue;
while(p!=NULL)
{
q = p->next;
delete p;
p=q;
}
}
}
system("pause");
return 0;
}

