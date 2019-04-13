
# BM模式匹配算法I C++实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月16日 16:48:55[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：1993


**// BM模式匹配算法I.cpp : Defines the entry point for the console application.**
**//**
**\#include "stdafx.h"**
**\#include<iostream>**
**\#define MAX 200**
**using namespace std;**
**void get_dist(int *dist,char *t,const int lenT)**
**{**
**int i;**
**for(i=0;i<=MAX;i++)**
**dist[i] = lenT;**
**for(i=0;i<lenT;i++)**
**dist[(int)t[i]] = lenT-i-1;**
**}**
**//**
**int BM(char *s,char *t,int *dist,const int lenS,const int lenT)**
**{**
**int i,j,k;**
**i = lenT-1;**
**while(i<lenS)**
**{**
**j = lenT-1;**
**k = i;**
**while(j>=0&&s[k]==t[j])**
**{**
**j--;**
**k--;**
**}**
**if(j<0)**
**return i+2-lenT;**
**else**
**i = i + dist[s[k]];**
**}**
**if(i>=lenS)**
**return 0;**
**}**
**int _tmain(int argc, _TCHAR* argv[])**
**{**
**int cases;**
**char s[MAX],t[MAX];**
**int dist[MAX];**
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
**get_dist(dist,t,lenT);**
**int pos = BM(s,t,dist,lenS,lenT);**
**if(pos==0)**
**cout<<"没有匹配项!"<<endl;**
**else**
**cout<<"匹配的开始位置为:"<<pos<<endl;**
**}**
**}**
**system("pause");**
**return 0;**
**}**
**----------------------------------------------------程序测试-------------------------------------------------------------**
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

