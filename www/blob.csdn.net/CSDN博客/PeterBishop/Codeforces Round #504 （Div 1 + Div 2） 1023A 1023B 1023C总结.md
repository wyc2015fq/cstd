# Codeforces Round #504 （Div. 1 + Div. 2）  1023A 1023B 1023C总结 - PeterBishop - CSDN博客





2018年08月18日 10:32:02[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：45
个人分类：[CodeForces](https://blog.csdn.net/qq_40061421/article/category/7796219)









A题卡了一个小时，真实。。。。

看了wa6，相同长度的字符串，坑在代码里面写了，凉凉难受。题目其实很清楚，就是从两头去比较是否相同。

```cpp
#include<stdio.h>
#include<string.h>

int main()
{
	int n, m;
	while (~scanf("%d %d", &n, &m))
	{
		char s1[300005], s2[300005];
		scanf("%s", s1);
		getchar();
		scanf("%s", s2);
		if (n == m)//这里其实是最坑的，因为我只考虑了相等没有*，但是有的话如果其他的一样也是可以替换的，所以一开始直接判断一直被坑
		{
			
			for (int i = 0; i < n; i++)
			{
				if (s1[i] == '*')
				{
					s1[i] = s2[i];
					break;
				}
			}
			
			if (strcmp(s1, s2) == 0)
				printf("YES\n");
			else
				printf("NO\n");
		}
		else if (n > m + 1)
		{
			printf("NO\n");
		}
		else if (n == m + 1)
		{
			for (int i = 0; i < n; i++)
			{
				if (s1[i] == '*')
				{
					for (int j = i; j < n - 1; j++)
					{
						s1[j] = s1[j + 1];
					}
					s1[n - 1] = '\0';
					break;
				}
			}
			if (strcmp(s1, s2) == 0)
				printf("YES\n");
			else
				printf("NO\n");
		}
		else
		{
			int flag = 1;
			int i;
			for (i = 0; i < n; i++)
			{
				if (s1[i] == '*')
					break;
			}
			if (i == n)
				flag = 0;
			else
			{
				for (int k = 0; k < i; k++)
				{
					if (s2[k] != s1[k])
					{
						flag = 0;
						break;
					}
				}
				for (int k = i + 1; k <n; k++)
				{
					if (s2[m - (n - k)] != s1[k])
					{
						flag = 0;
						break;
					}
				}

			}
			if (flag)
				printf("YES\n");
			else
				printf("NO\n");
		}
	}

	return 0;
}
```

 B题秒出，就是一个数学题目，找个规律嘛（然而A卡了一个小时没用了……）

```cpp
#include<iostream>
using namespace std;
int main()
{
	long long n, k;
	cin >> n >> k;
	long long ans=0;
	if (k <= n)
	{
		if (k % 2 == 1)
			ans = k / 2;
		else
			ans = k / 2 - 1;
	}
	else
	{
		long long s = k / 2;
		if (s >= n)
			ans = 0;
		else
		{
			ans = n - s;
		}
	}
	cout << ans << endl;
	return 0;
}
```

C题贪心，我是没看出来，就是从头开始成对的记录，count就是保证了成对输出，open则是满足子串

```cpp
#include<iostream>
using namespace std;
int main()
{
	long n,k,i,count=0,open=0;
	cin>>n>>k;
	k=k/2;
	string p;
	cin>>p;
	for(i=0;i<n;i++)
	{
		if(p[i]=='(' && open<k)
		{
			cout<<"(";
			open++;
			count++;
		}
		else if(p[i]==')' && count>0)
		{
			cout<<")";
			count--;
		}
	}
}
```

这次比赛真的是被A题坑惨了……血的教训啊！！！



