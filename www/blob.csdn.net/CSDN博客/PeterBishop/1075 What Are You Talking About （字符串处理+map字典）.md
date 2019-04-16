# 1075 What Are You Talking About （字符串处理+map字典） - PeterBishop - CSDN博客





2018年07月12日 10:14:37[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：27
个人分类：[HDOJ](https://blog.csdn.net/qq_40061421/article/category/7502192)









```cpp
#include <map>
#include <string.h>
#include <iostream>
#include<string>
using namespace std;
#include<stdio.h>//存字典+读取翻译（字符串操作的经典）
int main()
{
	char s1[20], s2[20], s[3005], s3[20], c, a[100];
	int k = 0, i, j;
	map<string, string>v;//字典
	cin >> s;
	while (cin >> s1)
	{
		if (strcmp(s1, "END") == 0)break;
		cin >> s2;
		v[s2] = s1;
	}
	getchar();
	while (gets(s))//每次读取一行
	{
		if (strcmp(s, "START") == 0)continue;
		if (strcmp(s, "END") == 0)break;
		for (int i = 0,k=0; i < strlen(s); i++)
		{
			if (isalpha(s[i]))
				a[k++] = s[i];
			else//一个单词结束了
			{
				a[k] = '\0';//加上结束符合
				if (v.count(a))
					cout << v[a];//找到了对应的单词
				else
					cout << a;//没找到直接输出a
				cout << s[i];//输出符号
				for (int j = 0; j < k; j++)
					a[j] = '\0';//将a重置
				k = 0;
			}
		}
		cout << endl;
	}
	
	return 0;
}
```




