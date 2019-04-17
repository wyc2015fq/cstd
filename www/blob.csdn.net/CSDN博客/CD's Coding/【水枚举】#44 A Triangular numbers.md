# 【水枚举】#44 A. Triangular numbers - CD's Coding - CSDN博客





2014年07月30日 13:35:39[糖果天王](https://me.csdn.net/okcd00)阅读数：606标签：[水题																[codeforces](https://so.csdn.net/so/search/s.do?q=codeforces&t=blog)](https://so.csdn.net/so/search/s.do?q=水题&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)

















A triangular number is the number of dots in an equilateral triangle uniformly filled with dots. For example, three dots can be arranged in a triangle; thus three is a triangular number. The *n*-th
 triangular number is the number of dots in a triangle with *n* dots on a side. ![](http://espresso.codeforces.com/d3b79c3a1beb823c96a128431cc056ce870b2780.png).
 You can learn more about these numbers from Wikipedia (http://en.wikipedia.org/wiki/Triangular_number).


Your task is to find out if a given integer is a triangular number.




Input


The first line contains the single number *n* (1 ≤ *n* ≤ 500)
 — the given integer.




Output


If the given integer is a triangular number output YES, otherwise output NO.




Sample test(s)




input
1




output
YES




input
2




output
NO




input
3




output
YES












问给定一个数n，是否存在一个数i，满足i*(i+1)/2=n。



反正n不大，就从1开始枚举咯~

code:






```cpp
#include <map> 
#include <queue>
#include <cmath> 
#include <memory> 
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

bool judge(int n)
{
	for(int i=1;;i++)
	{
		if(i*(i+1)/2==n)return true;
		if(i*(i+1)/2 >n)return false;
	}
	return false;
}

int main()
{
	int n;	cin>>n;
	cout<< (judge(n)?"YES":"NO")<<endl;
	return 0;
}
```












