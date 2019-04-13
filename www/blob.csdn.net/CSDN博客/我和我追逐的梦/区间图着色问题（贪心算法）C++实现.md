
# 区间图着色问题（贪心算法）C++实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月04日 02:34:29[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：6191


区间图着色问题
问题描述：假设要用很多个教室对一组活动进行调度。我们希望使用尽可能少的教室来调度所有活动。请给出一个算法，来确定哪一个活动使用哪一间教室。
这个问题也被称为区间图着色问题，即相容的活动着同色，不相容的着不同颜色，使得所用颜色数最少。
//贪心算法
\#include "stdafx.h"
\#include<iostream>
\#define N 100
using namespace std;
struct Activity
{
int number; //活动编号
int begin; //活动开始时间
int end; //活动结束时间
bool flag;//此活动是否被选择
int  roomNum; //此活动在哪间教室举行
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
//把每一个相容的活动集添加到一个教室，使得教室数目最少
int  select_room(Activity *act,int *time,int n)
{
int i = 1;
int j = 1;
int sumRoom;
//目前所用的教室数目
sumRoom = 1;
int sumAct;
//目前有多少活动被选择了
sumAct = 1;
//教室1目前最晚时间为排在最前面的活动的结束时间
time[1] = act[0].end;
//最先结束的活动放在教室1中
act[0].roomNum = 1;
for(i=1;i<n;i++)
{
for(j=1;j<=sumRoom;j++)
{
//如果活动act[i]的开始时间大于等于j教室目前的最晚结束时间且此活动还没有被选择，
//则此活动与目前这间教室里面的活动是兼容的，可以加入进去
if((act[i].begin>=time[j])&&(!act[i].flag))
{
//此活动的教室号码
act[i].roomNum = j;
//此活动被选择
act[i].flag = true;
//更新此教室的最晚时间
time[j] = act[i].end;
//被选择的活动数目加1
sumAct ++;
}
}
//说明活动没有全部被选择，而所有活动都遍历一遍
//所以需要再加一个教室，从头再遍历
if(sumAct<n&&i==n-1)
{
//从头开始遍历
i = 0;
//教室数目加1
sumRoom = sumRoom+1;
}
}
return sumRoom;
}
int _tmain(int argc, _TCHAR* argv[])
{
int cases;
Activity act[N];
//用来记录每个教室目前最晚完成的活动的结束时间
int time[N];
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
time[i+1] = 0; //初始化每个教室目前最晚的时间为0
act[i].number = i+1;
act[i].flag = false;   //初始化每个活动都未被选择
act[i].roomNum = 0;  //初始化每个活动都占用教室
cout<<"活动"<<i+1<<"开始时间：";
cin>>act[i].begin;
cout<<"活动"<<i+1<<"结束时间：";
cin>>act[i].end;
}
fast_sort(act,0,n-1);
int roomNum =select_room(act,time,n);
cout<<"所用教室总数为："<<roomNum<<endl;
cout<<"每个活动在哪一个教室中："<<endl;
for(i=0;i<n;i++)
{
cout<<"活动"<<act[i].number<<"在教室"<<act[i].roomNum<<"中"<<endl;
}
}
system("pause");
return 0;
}

-------------------------------------------------程序测试----------------------------------------------------
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
所用教室总数为：5
每个活动在哪一个教室中：
活动1在教室1中
活动2在教室2中
活动3在教室3中
活动4在教室1中
活动5在教室4中
活动6在教室2中
活动7在教室3中
活动8在教室1中
活动9在教室4中
活动10在教室5中
活动11在教室1中
请按任意键继续. . .

