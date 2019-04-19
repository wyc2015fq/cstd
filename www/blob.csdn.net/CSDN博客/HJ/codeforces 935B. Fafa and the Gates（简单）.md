# codeforces 935B. Fafa and the Gates（简单） - HJ - CSDN博客
2018年03月13日 13:18:10[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：113
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
题目链接 [http://codeforces.com/problemset/problem/935/B](http://codeforces.com/problemset/problem/935/B)
解题思路：每次判断当前状态下的小三角形直角边的长度关系。
```cpp
#include<iostream>  
using namespace std;  
char s[100005];
int main()
{  
    int n;  
    while(cin>>n)
	{
		int num=0,cnt=0;
		cin>>s;
		for(int i=0;i<n;i++)
		{
			if(s[i]=='U')
			  num++;
			else
			  num--;
			if(!num&&s[i]==s[i+1])
			  cnt++;
		}
		cout<<cnt<<endl;
	}  
	return 0;
}
```
