# 【水枚举】#83 A. Palindromic Times - CD's Coding - CSDN博客





2014年11月29日 21:18:02[糖果天王](https://me.csdn.net/okcd00)阅读数：617标签：[Codeforces																[Beta Round #83																[Div. 2 Only](https://so.csdn.net/so/search/s.do?q=Div. 2 Only&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)](https://so.csdn.net/so/search/s.do?q=Beta Round #83&t=blog)












Tattah is asleep if and only if Tattah is attending a lecture. This is a well-known formula among Tattah's colleagues.


On a Wednesday afternoon, Tattah was attending Professor HH's lecture. At 12:21, right before falling asleep, he was staring at the digital watch around Saher's
 wrist. He noticed that the digits on the clock were the same when read from both directions i.e. a palindrome.


In his sleep, he started dreaming about such rare moments of the day when the time displayed on a digital clock is a palindrome. As soon as he woke up, he felt destined to write a program that finds the next such moment.


However, he still hasn't mastered the skill of programming while sleeping, so your task is to help him.




Input


The first and only line of the input starts with a string with the format "HH:MM" where "HH"
 is from "00" to "23" and "MM"
 is from "00" to "59". Both "HH"
 and "MM" have exactly two digits.




Output


Print the palindromic time of day that comes soonest after the time given in the input. If the input time is palindromic, output the soonest palindromic time after the input time.




Sample test(s)




input
12:21




output
13:31




input
23:59




output
00:00












题意是说，电子钟的小时和分钟有时会形成回文序列，比如1221，0550之类的，给定一个现在的时间，问下一个回文时间是什么时候。



反正没多少，直接枚举出来呗~ （需要注意的是，23：32之后的时间会推到下一天的00：00去）

Code:



```cpp
#include <cstdio>
#include <string>
#include <cstring> 
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;
// http://codeforces.com/contest/108
// Palindromic Times
int hor[]={0, 1, 2, 3, 4, 5,10,11,12,13,14,15,20,21,22,23};
int mie[]={0,10,20,30,40,50,1 ,11,21,31,41,51, 2,12,22,32};

int main()
{
	char maohao;
	int h,m;
	cin>>h>>maohao>>m;
	for(int i=0;i<16;i++)
	{
		if((hor[i]==h && mie[i]>m) || hor[i]>h)
		{
			if(hor[i]<10)cout<<"0";
			cout<<hor[i]<<":";
			if(mie[i]<10)cout<<"0";
			cout<<mie[i]<<endl;
			return 0;
		}
	}
	cout<<"00:00"<<endl;
	return 0;
}
```](https://so.csdn.net/so/search/s.do?q=Codeforces&t=blog)




