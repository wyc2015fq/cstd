
# 活动选择问题（活动安排问题）（最大数目活动选择问题）贪心算法C++实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月03日 22:44:57[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：3269


// 活动选择问题（活动安排问题）（最大数目活动选择问题）.cpp : Defines the entry point for the console application.
//贪心算法
\#include "stdafx.h"
\#include<iostream>
\#define N 100
using namespace std;
struct Activity
{
int number;    //活动编号
int begin;     //活动开始时间
int end;      //活动结束时间
bool flag;    //此活动是否被选择
};
//对于活动集，按照结束时间递增排序,使用快速排序
void fast_sort(Activity *act,int f,int t)
{
if(f<t)
{
int i = f-1,j = f;
Activity a = act[t];
while(j<t)
{
if(act[j].end<=a.end)
{
i++;
Activity temp1 = act[i];
act[i] = act[j];
act[j] = temp1;
}
j++;
}
Activity temp2 = act[t];
act[t] = act[i+1];
act[i+1] = temp2;
fast_sort(act,f,i);
fast_sort(act,i+2,t);
}
}
//选择最大活动数目
void select_activity(Activity *act,int n)
{
int i = 0;
act[0].flag = true;
Activity a = act[0];
for(i=1;i<n;i++)
{
if(act[i].begin>=a.end)
{
act[i].flag = true;
a = act[i];
}
}
}
int _tmain(int argc, _TCHAR* argv[])
{
int cases;
Activity act[N];
cout<<"请输入案例的个数："<<endl;
cin>>cases;
while(cases--)
{
int n;
cout<<"请输入活动的数目："<<endl;
cin>>n;
int i;
for(i=0;i<n;i++)
{
act[i].number = i+1;
act[i].flag = false;
cout<<"活动"<<i+1<<"开始时间：";
cin>>act[i].begin;
cout<<"活动"<<i+1<<"结束时间：";
cin>>act[i].end;
}
//快速排序
fast_sort(act,0,n-1);
//选择最大数目活动集
select_activity(act,n);
int sum = 0;
cout<<"最终选择的活动集合为："<<endl;
cout<<"< ";
for(i=0;i<n;i++)
if(act[i].flag)
{
cout<<act[i].number<<" ";
sum ++;
}
cout<<">"<<endl;
cout<<"最大活动集合的数目为："<<sum<<endl;
}
system("pause");
return 0;
}
----------------------------------------------程序测试----------------------------------------------------
请输入案例的个数：
1
请输入活动的数目：
11
活动1开始时间：1
活动1结束时间：4
活动2开始时间：3
活动2结束时间：5
活动3开始时间：0
活动3结束时间：6
活动4开始时间：5
活动4结束时间：7
活动5开始时间：3
活动5结束时间：8
活动6开始时间：5
活动6结束时间：9
活动7开始时间：6
活动7结束时间：10
活动8开始时间：8
活动8结束时间：11
活动9开始时间：8
活动9结束时间：12
活动10开始时间：2
活动10结束时间：13
活动11开始时间：12
活动11结束时间：14
最终选择的活动集合为：
<1 4 8 11 >
最大活动集合的数目为：4
请按任意键继续. . .

