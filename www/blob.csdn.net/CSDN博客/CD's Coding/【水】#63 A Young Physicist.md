# 【水】#63 A. Young Physicist - CD's Coding - CSDN博客





2014年10月10日 10:35:54[糖果天王](https://me.csdn.net/okcd00)阅读数：804
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















A guy named Vasya attends the final grade of a high school. One day Vasya decided to watch a match of his favorite hockey team. And, as the boy loves hockey very much, even more than physics, he forgot to do the homework. Specifically, he forgot to complete
 his physics tasks. Next day the teacher got very angry at Vasya and decided to teach him a lesson. He gave the lazy student a seemingly easy task: You are given an idle body in space and the forces that affect it. The body can be considered as a material point
 with coordinates (0; 0; 0). Vasya had only to answer whether it is in equilibrium. "Piece of cake" — thought Vasya, we need only to check if the sum of all vectors is equal to 0. So, Vasya began to solve the problem. But later it turned out that there can
 be lots and lots of these forces, and Vasya can not cope without your help. Help him. Write a program that determines whether a body is idle or is moving by the given vectors of forces.




Input


The first line contains a positive integer *n* (1 ≤ *n* ≤ 100),
 then follow *n* lines containing three integers each: the *x**i* coordinate,
 the*y**i* coordinate
 and the *z**i* coordinate
 of the force vector, applied to the body ( - 100 ≤ *x**i*, *y**i*, *z**i* ≤ 100).




Output


Print the word "YES" if the body is in equilibrium, or the word "NO"
 if it is not.




Sample test(s)




input
3
4 1 7
-2 4 -1
1 -5 -3




output
NO



input
3
3 -1 7
-5 2 -4
2 -1 -3




output
YES











这道题并没有什么可以说的呢，那么就当给大家讲讲题意吧^_^



有n个向量，每行的三个数字构成了向量(a,b,c)，问这么多向量和是否为0向量，那么不就是三维分别求和都为0即可吗？

x、y、z向量上的分别加一加，都为0就是yes啦~



```cpp
#include <cstdio>
#include <iostream>
using namespace std;

int main()
{
	int n;	cin>>n;
	int x=0,y=0,z=0;
	for(int i=0;i<n;i++)
	{
		int xx,yy,zz;
		scanf("%d%d%d",&xx,&yy,&zz);
		x+=xx,y+=yy,z+=zz;
	} 
	cout<<(x==0&&y==0&&z==0?"YES":"NO");
	return 0;
}
```













