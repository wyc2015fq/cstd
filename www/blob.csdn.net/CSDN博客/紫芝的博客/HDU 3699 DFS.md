# HDU 3699 DFS - 紫芝的博客 - CSDN博客





2018年04月06日 13:00:42[紫芝](https://me.csdn.net/qq_40507857)阅读数：66
个人分类：[DFS](https://blog.csdn.net/qq_40507857/article/category/7566193)











# A hard Aoshu Problem
**Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 62768/32768 K (Java/Others)Total Submission(s): 1620    Accepted Submission(s): 848**


Problem Description

Math Olympiad is called “Aoshu” in China. Aoshu is very popular in elementary schools. Nowadays, Aoshu is getting more and more difficult. Here is a classic Aoshu problem:


ABBDE __ ABCCC = BDBDE

In the equation above, a letter stands for a digit(0 – 9), and different letters stands for different digits. You can fill the blank with ‘+’, ‘-‘ , ‘×’ or ‘÷’. 

How to make the equation right? Here is a solution:


12245 + 12000 = 24245

In that solution, A = 1, B = 2, C = 0, D = 4, E = 5, and ‘+’ is filled in the blank.

When I was a kid, finding a solution is OK. But now, my daughter’s teacher tells her to find all solutions. That’s terrible. I doubt whether her teacher really knows how many solutions are there. So please write a program for me to solve this kind of problems.






Input

The first line of the input is an integer T( T <= 20) indicating the number of test cases.

Each test case is a line which is in the format below:


s1 s2 s3 

s1, s2 and s3 are all strings which are made up of capital letters. Those capital letters only include ‘A’,’B’,’C’,’D’ and ‘E’, so forget about ‘F’ to ‘Z’. The length of s1,s2 or s3 is no more than 8.

When you put a ‘=’ between s2 and s3, and put a operator( ‘+’,’-‘, ‘×’ or ‘÷’.) between s1 and s2, and replace every capital letter with a digit, you get a equation. 

You should figure out the number of solutions making the equation right.

Please note that same letters must be replaced by same digits, and different letters must be replaced by different digits. If a number in the equation is more than one digit, it must not have leading zero.






Output

For each test case, print an integer in a line. It represents the number of solutions.





Sample Input


2
A A A
BCD BCD B





Sample Output


5
72



题意：

给三个字符串，只包含A,B,C,D,E, 长度不超过8，对三个字符串中的字符赋值，即相同的字符用0~9之间的字符替换，

s1 ( ‘+’,’-‘, ‘×’ or ‘÷’.)  s2=s3，替换后的三个数进行四则运算，要满足左边等于右边，求有几种解法。

注意：没有前导 0

```cpp
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<string>
#include<queue>
#include<map>
using namespace std;

int vis[10],flag[10],l,ans;
char s1[10],s2[10],s3[10],s[10];
map<char,int>mp,dl;
void Cal()
{
	int a=0,b=0,c=0;
	for(int i=0;i<strlen(s1);i++)
		a=a*10+mp[s1[i]];
	for(int i=0;i<strlen(s2);i++)
		b=b*10+mp[s2[i]];
	for(int i=0;i<strlen(s3);i++)
		c=c*10+mp[s3[i]];
	if(a+b==c) ans++;
	if(a-b==c) ans++;
	if(a*b==c) ans++;
	if(b&&a==b*c) ans++;
}
void dfs(int num)
{
	if(num>=l) 
	{
		Cal();
		return;
	}
	for(int i=0;i<10;i++)
	{
		if(i==0&&dl[s[num]]) continue;
		if(!flag[i])
		{
			flag[i]=1;
			mp[s[num]]=i;
			dfs(num+1);
			flag[i]=0;
		}
	}
}
int main()
{
	int t;
	scanf("%d",&t);
	while(t--)
	{
		memset(vis,0,sizeof(vis));
		mp.clear();dl.clear();
		ans=0;
		scanf("%s%s%s",s1,s2,s3);
		if(strlen(s1)>1) dl[s1[0]]=1;
		if(strlen(s2)>1) dl[s2[0]]=1;
		if(strlen(s3)>1) dl[s3[0]]=1;
		int i,k,j;
		l=0;
		for(i=0;i<strlen(s1);i++)
		{
			if(vis[s1[i]-'A']) continue;
			vis[s1[i]-'A']++;
			s[l++]=s1[i];
		}
		for(i=0;i<strlen(s2);i++)
		{
			if(vis[s2[i]-'A']) continue;
			vis[s2[i]-'A']++;
			s[l++]=s2[i];
		}
		for(i=0;i<strlen(s3);i++)
		{
			if(vis[s3[i]-'A']) continue;
			vis[s3[i]-'A']++;
			s[l++]=s3[i];
		}
		dfs(0);
		printf("%d\n",ans);
	}
	return 0;
}
```




