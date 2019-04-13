
# 石油主管道最优位置问题（平均时间为线性时间）C++实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年10月29日 16:02:26[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：1880


// 石油主管道最优位置问题.cpp : Defines the entry point for the console application.
//公司计划建设一条从西到东石油主管道，它穿过一个有n口油井的油田，
//每口油井都有一个或向北或向南的沿最短管道与主管道相连，给定每口油井的坐标（x，y）
//选择主管道的最优y值，使得每口油井的管道之和最小。
/////////////////////解决方法///////////////////////
//每口油井的y坐标按非降序排序（实际的解决方法不需要排序）（y1,y2,y3,y4,...yn）
//假设主管道的纵坐标为y，可知对于y1yn，|y-y1|+|y-yn|最小，则y要处于y1与yn之间，
//同理把y2y(n-1)、y3y(n-2)...为一组，y也要处于它们之间。于是如果n为奇数，就找出中位数，
//如果n为偶数，则找出它们的中间值的一个即可（不唯一，在这里取它们的平均值）。
//实际上不需要排序，利用快速排序的一趟快速排序找出的值的位置是其排序后最终的位置的特点，
//只需要比较每一趟找出的是否为中位数即可。
//在每一趟一趟快速排序时，加入随机化选择主元，经分析可得平均时间为线性时间。
\#include "stdafx.h"
\#include<iostream>
\#include<ctime>
\#define N 100
using namespace std;
double pos[N];
//产生数组的一个随机下标，以此下标对应的数组元素
//作为每趟快速选择（即为每趟快速排序）的主元,以系统时间作为随机种子
int random(int begin,int end)
{
srand((unsigned)time(NULL));
return begin + rand()%(end-begin+1);
}
//一趟快速选择（即为一趟快速排序）一旦找到了中位数，就停止快速选择
int qartition(double *a,int begin,int end)
{
double x = a[end];
int i = begin-1,j=begin;
while(j<end)
{
if(a[j]<=x)
{
i++;
double temp = a[i];
a[i]=a[j];
a[j]=temp;
}
j++;
}
double temp = a[i+1];
a[i+1]=a[end];
a[end]=temp;
cout<<"qartition"<<endl;
for(int l=0;l<end+1;l++)
cout<<a[l]<< " ";
cout<< endl;
return i+1;
}
//随机化一趟快速选择,产生一个随机下标，
//然后把其对应的数组元素与最后一个数组元素交换
int random_qartition(double *a,int begin,int end)
{
int q = random(begin,end);
cout<<"random q = " << q <<endl;
double temp = a[q];
a[q]=a[end];
a[end]=temp;
return qartition(a,begin,end);
}
//随机化快速选择，一旦找到中位数就停止，返回中位数，
//否则继续调用自己寻找中位数
double random_middle_select(double *a,int begin,int end,int position)
{
if(begin>end)
return -1;
else if(begin==end)
return a[begin];
else
{
int k = random_qartition(a,begin,end);
cout<<"1 random sort k = " << k<<endl;
if(position==k)
return a[k];
else if(position<k)
return random_middle_select(a,begin,k-1,position);
else
return random_middle_select(a,k+1,end,position);
}
}
int _tmain(int argc, _TCHAR* argv[])
{
int cases;
cout<<"请输入案例的个数："<<endl;
cin>>cases;
while(cases--)
{
int  n;
cout<<"请输入油井的个数："<<endl;
cin>>n;
cout<<"请输入每个油井的Y坐标："<<endl;
int i;
for(i=0;i<n;i++)
cin>>pos[i];
double  position=0.0;
if(n%2)   //油井个数为奇数，中位数即为最中间的位置对应的元素
position = random_middle_select(pos,0,n-1,n/2);
else      //油井个数为偶数，则选择中间2个元素的平均值作为中位数
{
position = random_middle_select(pos,0,n-1,n/2-1);
position += random_middle_select(pos,0,n-1,n/2);
position/=2;
}
cout<<"主管道的Y坐标为："<<endl;
cout<<position<<endl;
}
system("pause");
return 0;
}


