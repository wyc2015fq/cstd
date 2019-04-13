
# 堆排序C++实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年10月30日 16:47:32[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：1021


// 堆排序.cpp : Defines the entry point for the console application.
//时间复杂度为nlgn
//建立最大堆
\#include "stdafx.h"
\#include<iostream>
using namespace std;
int A[100];
//
//保持堆得性质
//a 为待排序数组，sum是待排序元素个数 i是父元素序号
void keep_heapify(int *a,int sum,int i)
{
int left = 2*i;
int right = 2*i+1;
int largest = i;
if(left<=sum&&a[left]>a[i])
largest = left;
if(right<=sum&&a[right]>a[largest])
largest = right;
if(largest!=i)
{
int temp = a[i];
a[i] = a[largest];
a[largest] = temp;
keep_heapify(a,sum,largest);
}
}
//创建大堆
void create_heap(int *a,int sum)
{
int i;
for(i=sum/2;i>=1;i--)
{
keep_heapify(a,sum,i);
}
}
//堆排序
//首先建立大堆，然后交换堆顶和堆底的元素，待排序元素总数减一
//调用keep_heapify()保持堆性质，直到排完
void heap_sort(int *a,int sum)
{
create_heap(a,sum);
int j;
int num = sum;
for(j=sum;j>=2;j--)
{
num--;
int temp = a[1];
a[1]=a[j];
a[j]=temp;
keep_heapify(a,num,1);
}
}
int _tmain(int argc, _TCHAR* argv[])
{
int cases;
cout<<"请输入案例个数："<<endl;
cin>>cases;
while(cases--)
{
cout<<"请输入需要排序元素的个数："<<endl;
int n;
cin>>n;
int k;
cout<<"请输入需要排序的元素："<<endl;
for(k=1;k<=n;k++)
cin>>A[k];
cout<<"排序前："<<endl;
for(k=1;k<=n;k++)
cout<<A[k]<<" ";
cout<<endl;
heap_sort(A,n);
cout<<"堆排序后："<<endl;
for(k=1;k<=n;k++)
cout<<A[k]<<" ";
cout<<endl;
}
system("pause");
return 0;
}


