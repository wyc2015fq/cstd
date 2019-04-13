
# Array类模板实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月18日 19:51:11[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：2158


**Array类模板实现**
**---------------------------------------------Array.h------------------------------------------------------**
**\#include<string>**
**using namespace std;**
**template<class DataType>**
**class Array**
**{**
**public:**
**/***
**Array(int szie);**
**inline DataType &operator[](int index);**
**void changeSize(int newSize);**
**inline int length() const;**
**string err() const;**
***/**
**Array(int size)**
**{**
**if(size<1)**
**{**
**capacity = 1;**
**errorCode = 1;**
**}**
**else**
**{**
**capacity = size;**
**errorCode = 0;**
**}**
**elements = new DataType[capacity];**
**}**
**inline DataType &operator [](int index)**
**{**
**\#ifdef DEBUG_ARRAY**
**if(index<0||index>=capacity)**
**{**
**errorCode |=2;**
**return dud;**
**}**
**\#endif**
**return elements[index];**
**}**
**void changeSize(int newSize)**
**{**
**if(newSize<1)**
**{**
**errorCode |=4;**
**return;**
**}**
**DataType *newArray = new DataType[newSize];**
**int limit = (newSize>capacity)?capacity:newSize;**
**for(int i = 0;i < limit;i++)**
**newArray[i] = elements[i];**
**delete[] elements;**
**elements = newArray;**
**capacity = newSize;**
**}**
**inline int length() const**
**{**
**return capacity;**
**}**
**string err() const**
**{**
**if(errorCode==0)**
**return "No error.\n";**
**string errorMessage = "";**
**if(errorCode&1)**
**{**
**errorMessage +="Nonpositive size passed into constructor,so\n";**
**errorMessage +="the capacity was set to 1 by default.\n";**
**}**
**if(errorCode&2)**
**{**
**errorMessage +="Index out of range.\n";**
**}**
**if(errorCode&4)**
**{**
**errorMessage += "Nonpositive size passed into changeSize,so\n";**
**errorMessage += "the size of the array was not change.\n";**
**}**
**return errorMessage;**
**}**
**private:**
**DataType *elements;**
**int capacity;**
**DataType dud;**
**int errorCode;**
**};**
**---------------------------------- 主函数（使用Array）------------------------------------------**
**// 各种类模板实现.cpp : Defines the entry point for the console application.**
**//**
**\#include "stdafx.h"**
**\#include<iostream>**
**\#include "Array.h"**
**\#define DEBUG_ARRAY**
**using namespace std;**
**void getElements( Array<int> &numbers);**
**float calcAverage( Array<int> avnums);**
**int _tmain(int argc, _TCHAR* argv[])**
**{**
**Array<int> nums(2);**
**getElements(nums);**
**float average = calcAverage(nums);**
**cout<<"The average is:"<<average<<endl;**
**cout<<endl;**
**system("pause");**
**return 0;**
**}**
**void getElements(Array<int> &numbers)**
**{**
**int i = 0;**
**cout<<"Enter a positive integer:";**
**cin >> numbers[i];**
**while(numbers[i] !=-1)**
**{**
**i++;**
**if(i == numbers.length())**
**numbers.changeSize(2*i);**
**cout<<"Enter a positive integer(enter -1 to end):";**
**cin>>numbers[i];**
**}**
**numbers.changeSize(i);**
**cout<<"getElemens:"<<numbers.err()<<endl;**
**}**
**float calcAverage(Array<int> avnums)**
**{**
**int sum = 0;**
**for(int i=0;i<avnums.length();i++)**
**sum += avnums[i];**
**cout<<"calcAverage:"<<avnums.err()<<endl;**
**return sum/(float)(avnums.length());**
**}**


