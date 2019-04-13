
# 归并排序 C++实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月05日 03:08:45[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：2965标签：[c++																](https://so.csdn.net/so/search/s.do?q=c++&t=blog)[merge																](https://so.csdn.net/so/search/s.do?q=merge&t=blog)[system																](https://so.csdn.net/so/search/s.do?q=system&t=blog)[测试																](https://so.csdn.net/so/search/s.do?q=测试&t=blog)[存储																](https://so.csdn.net/so/search/s.do?q=存储&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=测试&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=system&t=blog)个人分类：[C、C++开发																](https://blog.csdn.net/heyongluoyao8/article/category/908444)[算法设计与分析																](https://blog.csdn.net/heyongluoyao8/article/category/961150)[
							](https://blog.csdn.net/heyongluoyao8/article/category/908444)
[
				](https://so.csdn.net/so/search/s.do?q=system&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=system&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=merge&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
**// 归并排序.cpp : Defines the entry point for the console application.**
**//**
**\#include "stdafx.h"**
**\#include<iostream>**
**\#define N 100**
**using namespace std;**
**typedef int DataType;**
**//合并data1[begin...mid]和data1[mid+1.end]到data2[begin,end]中,然后复制到data1[begin,end]中**
**void merge(DataType *data1,DataType *data2,int begin,int mid,int end)**
**{**
**int i,j,k;**
**for(i = begin,j=mid+1,k = begin;i<=mid&&j<=end;k++)**
**{**
**if(data1[i]<=data1[j])**
**data2[k] = data1[i++];**
**else**
**data2[k] = data1[j++];**
**}**
**if(i<=mid)**
**while(i<=mid)**
**data2[k++] = data1[i++];**
**else**
**while(j<=end)**
**data2[k++] = data1[j++];**
**for(i=begin;i<=end;i++)**
**data1[i] = data2[i];**
**}**
**//归并排序**
**void Msort(DataType *data1,DataType *data2,int begin,int end)**
**{**
**if(begin==end)**
**data1[begin] = data1[begin];**
**else**
**{**
**int mid = (begin+end)/2;**
**Msort(data1,data2,begin,mid);**
**Msort(data1,data2,mid+1,end);**
**merge(data1,data2,begin,mid,end);**
**}**
**}**
**int _tmain(int argc, _TCHAR* argv[])**
**{**
**//data1存储原数据和排序后的数据，data2为辅助数据**
**DataType data1[N],data2[N];**
**int cases;**
**cout<<"请输入案例个数：";**
**cin>>cases;**
**while(cases--)**
**{**
**int n;**
**cout<<"请输入元素个数：";**
**cin>>n;**
**int i;**
**cout<<"请输入待排序元素："<<endl;**
**for(i=1;i<=n;i++)**
**cin>>data1[i];**
**cout<<"归并排序前："<<endl;**
**for(i=1;i<=n;i++)**
**cout<<data1[i]<<" ";**
**cout<<endl;**
**Msort(data1,data2,1,n);**
**cout<<"归并排序后："<<endl;**
**for(i=1;i<=n;i++)**
**cout<<data1[i]<<" ";**
**cout<<endl;**
**}**
**system("pause");**
**return 0;**
**}**
**--------------------------------------------------测试程序-----------------------------------------------------****请输入案例个数：2**
**请输入元素个数：9**
**请输入待排序元素：**
**87 65 2 77 88 54 2 7 8**
**归并排序前：**
**87 65 2 77 88 54 2 7 8**
**归并排序后：**
**2 2 7 8 54 65 77 87 88**
**请输入元素个数：10**
**请输入待排序元素：**
**10 9 8 7 6 5 4 3 2 1**
**归并排序前：**
**10 9 8 7 6 5 4 3 2 1**
**归并排序后：**
**1 2 3 4 5 6 7 8 9 10**
**请按任意键继续. . .**

