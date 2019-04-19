# C++实现简单回文算法 - fanyun的博客 - CSDN博客
2018年09月28日 20:23:16[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：603标签：[回文算法](https://so.csdn.net/so/search/s.do?q=回文算法&t=blog)
个人分类：[C++](https://blog.csdn.net/fanyun_01/article/category/6102444)
1.实现简答回文算法
编写一个程序，判断一个字符串是否为"回文"。回文串：字符串字符从前往后与从后往前一致（中心对称）。
2.回文算法思路
通过回文字符串的移位比较，检查是否为回文。
3.回文算法实现代码
```cpp
// PalindromeJudgment.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <cstring>
using namespace std;
bool  isHuiwen(char *array)
{
	int len = strlen(array);
	for (int i = 0, j = len - 1; i < j; i++, j--)
	{
		if (array[i] != array[j])
			return false;
	}
	return true;
}
int main()
{
	char input[1000];
	cin >> input;
	for (int i = 0; i < (int)strlen(input); i++)
	{
		cout << input[i];
	}
	if (isHuiwen(input))
	{
		cout << "是回文！" << endl;
	}
	else
		cout << "不是回文！" << endl;
	return 0;
}
```
程序输出：
![](https://img-blog.csdn.net/20180925163052131?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zhbnl1bl8wMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180925163154761?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zhbnl1bl8wMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
