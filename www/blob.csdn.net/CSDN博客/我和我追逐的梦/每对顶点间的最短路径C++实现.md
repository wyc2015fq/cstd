
# 每对顶点间的最短路径C++实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月11日 16:47:53[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：1062标签：[c++																](https://so.csdn.net/so/search/s.do?q=c++&t=blog)[system																](https://so.csdn.net/so/search/s.do?q=system&t=blog)[测试																](https://so.csdn.net/so/search/s.do?q=测试&t=blog)[存储																](https://so.csdn.net/so/search/s.do?q=存储&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=测试&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=system&t=blog)个人分类：[C、C++开发																](https://blog.csdn.net/heyongluoyao8/article/category/908444)[算法设计与分析																](https://blog.csdn.net/heyongluoyao8/article/category/961150)[
							](https://blog.csdn.net/heyongluoyao8/article/category/908444)
[
				](https://so.csdn.net/so/search/s.do?q=system&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=system&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=c++&t=blog)

**// 每对顶点间的最短路径.cpp : Defines the entry point for the console application.**
**//**
**\#include "stdafx.h"**
**\#include<iostream>**
**\#define MAX 100**
**\#define Infinity 65535**
**using namespace std;**
**//**
**int L1[MAX][MAX];**
**int L2[MAX][MAX];**
**//用来存储边的权值，即有向图的邻接矩阵**
**int w[MAX][MAX];**
**//初始化，把w[i][j]赋给L1[i][j]**
**void initialise(int n)**
**{**
**int i,j;**
**for(i=1;i<=n;i++)**
**for(j=1;j<=n;j++)**
**L1[i][j] = w[i][j];**
**}**
**//求每一对顶点间暂时最短距离**
**void extend_shortest_paths(int n)**
**{**
**int i,j,k,l;**
**for(i=1;i<=n;i++)**
**{**
**for(j=1;j<=n;j++)**
**{**
**L2[i][j] = Infinity;**
**for(k=1;k<=n;k++)**
**L2[i][j] = L2[i][j]<(L1[i][k]+w[k][j])?L2[i][j]:(L1[i][k]+w[k][j]);**
**}**
**}**
**for(i=1;i<=n;i++)**
**for(j=1;j<=n;j++)**
**L1[i][j] = L2[i][j];**
**}**
**//求所有对顶点之间的最短距离**
**void show_all_pairs_shortest_paths(int n)**
**{**
**initialise(n);**
**int m;**
**for(m=2;m<=n-1;m++)**
**extend_shortest_paths(n);**
**}**
**int _tmain(int argc, _TCHAR* argv[])**
**{**
**int cases;**
**cout<<"请输入案例的个数："<<endl;**
**cin>>cases;**
**while(cases--)**
**{**
**cout<<"请输入顶点个数："<<endl;**
**int n;**
**cin>>n;**
**cout<<"请输入邻接矩阵(n*n)(如果二点之间没有有向线段，输入65535)："<<endl;**
**int i,j;**
**//二点之间没有有向线段，输入65535**
**for(i=1;i<=n;i++)**
**for(j=1;j<=n;j++)**
**{**
**cin>>w[i][j];**
**}**
**show_all_pairs_shortest_paths(n);**
**cout<<"输出每一对顶点间的最短距离："<<endl;**
**for(i=1;i<=n;i++)**
**for(j=1;j<=n;j++)**
**{**
**cout<<"顶点"<<i<<"到顶点"<<j<<"的最短距离为："<<L1[i][j]<<endl;**
**}**
**}**
**system("pause");**
**return 0;**
**}**
**-----------------------------------------------------程序测试----------------------------------------------------------**
**请输入案例的个数：**
**1**
**请输入顶点个数：**
**5**
**请输入邻接矩阵(n*n)(如果二点之间没有有向线段，输入65535)：**
**0 3 8 65535 -4**
**65535 0 65535 1 7**
**65535 4 0 65535 65535**
**2 65535 -5 0 65535**
**65535 65535 65535 6 0**
**输出每一对顶点间的最短距离：**
**顶点1到顶点1的最短距离为：0**
**顶点1到顶点2的最短距离为：1**
**顶点1到顶点3的最短距离为：-3**
**顶点1到顶点4的最短距离为：2**
**顶点1到顶点5的最短距离为：-4**
**顶点2到顶点1的最短距离为：3**
**顶点2到顶点2的最短距离为：0**
**顶点2到顶点3的最短距离为：-4**
**顶点2到顶点4的最短距离为：1**
**顶点2到顶点5的最短距离为：-1**
**顶点3到顶点1的最短距离为：7**
**顶点3到顶点2的最短距离为：4**
**顶点3到顶点3的最短距离为：0**
**顶点3到顶点4的最短距离为：5**
**顶点3到顶点5的最短距离为：3**
**顶点4到顶点1的最短距离为：2**
**顶点4到顶点2的最短距离为：-1**
**顶点4到顶点3的最短距离为：-5**
**顶点4到顶点4的最短距离为：0**
**顶点4到顶点5的最短距离为：-2**
**顶点5到顶点1的最短距离为：8**
**顶点5到顶点2的最短距离为：5**
**顶点5到顶点3的最短距离为：1**
**顶点5到顶点4的最短距离为：6**
**顶点5到顶点5的最短距离为：0**
**请按任意键继续. . .**

