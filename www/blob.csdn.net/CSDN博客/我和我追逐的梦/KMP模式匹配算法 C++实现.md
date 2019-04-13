
# KMP模式匹配算法 C++实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月16日 16:46:42[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：1382标签：[算法																](https://so.csdn.net/so/search/s.do?q=算法&t=blog)[c++																](https://so.csdn.net/so/search/s.do?q=c++&t=blog)[system																](https://so.csdn.net/so/search/s.do?q=system&t=blog)[测试																](https://so.csdn.net/so/search/s.do?q=测试&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=system&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=c++&t=blog)个人分类：[C、C++开发																](https://blog.csdn.net/heyongluoyao8/article/category/908444)[算法设计与分析																](https://blog.csdn.net/heyongluoyao8/article/category/961150)[
							](https://blog.csdn.net/heyongluoyao8/article/category/908444)
[
				](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=算法&t=blog)

**KMP模式比配算法**
**// KMP模式比配算法.cpp : Defines the entry point for the console application.**
**//**
**\#include "stdafx.h"**
**\#include<iostream>**
**\#define MAX 1000**
**using namespace std;**
**char s[MAX],t[MAX];**
**int next[MAX];**
**//**
**void ComputeNext(char *t,int *next,const int& lenT)**
**{**
**int i,j;**
**next[0] = -1;**
**i = 0;**
**j = -1;**
**while(i<lenT)**
**{**
**while(j>=0&&t[i] !=t[j] )**
**j = next[j];**
**j++;**
**i++;**
**//next[i] = j;**
**//改进**
**if(t[i]==t[j])**
**next[i] = next[j];**
**else**
**next[i] = j;**
**}**
**}**
**//**
**int KMP(char *s,char *t,int *next,const int lenS,const int lenT)**
**{**
**int i,j;**
**i=j=-1;**
**while(i<lenS&&j<lenT)**
**{**
**if((j==-1)||(s[i]==t[j]))**
**{**
**i++;**
**j++;**
**}**
**else**
**j = next[j];**
**}**
**if(j>=lenT)**
**return i-lenT+1;**
**else**
**return 0;**
**}**
**int _tmain(int argc, _TCHAR* argv[])**
**{**
**int cases;**
**cout<<"请输入案例的个数：";**
**cin>>cases;**
**while(cases--)**
**{**
**cout<<"请输入主串:"<<endl;**
**cin>>s;**
**int lenS = strlen(s);**
**while(1)**
**{**
**cout<<"请输入需要匹配的模式串(以0结束):"<<endl;**
**cin>>t;**
**if(!strcmp(t,"0"))**
**break;**
**int lenT = strlen(t);**
**for(int i=0;i<lenT;i++)**
**next[i] = -1;**
**ComputeNext(t,next,lenT);**
**int pos = KMP(s,t,next,lenS,lenT);**
**if(pos==0)**
**cout<<"没有匹配项!"<<endl;**
**else**
**cout<<"匹配的开始位置为:"<<pos<<endl;**
**}**
**}**
**system("pause");**
**return 0;**
**}**
**---------------------------------------------------程序测试---------------------------------------------------**
**请输入案例的个数：1**
**请输入主串:**
**heyongluoyao8**
**请输入需要匹配的模式串(以0结束):**
**he**
**匹配的开始位置为:1**
**请输入需要匹配的模式串(以0结束):**
**yong**
**匹配的开始位置为:3**
**请输入需要匹配的模式串(以0结束):**
**luo**
**匹配的开始位置为:7**
**请输入需要匹配的模式串(以0结束):**
**luoyao**
**匹配的开始位置为:7**
**请输入需要匹配的模式串(以0结束):**
**lao**
**没有匹配项!**
**请输入需要匹配的模式串(以0结束):**
**0**
**请按任意键继续. . .**

