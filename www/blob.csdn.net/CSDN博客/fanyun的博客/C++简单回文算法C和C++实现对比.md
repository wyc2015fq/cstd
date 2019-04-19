# C++简单回文算法C和C++实现对比 - fanyun的博客 - CSDN博客
2018年09月28日 20:22:49[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：306
1.实现简答回文算法
编写一个程序，判断一个字符串是否为"回文"。回文串：字符串字符从前往后与从后往前一致（中心对称）。
2.回文算法思路
通过回文字符串的移位比较，检查是否为回文。
3.回文算法的代码实现
(1).C语言实现回文算法
```cpp
#include <stdio.h>
#include <string.h>
int main()
{
      char a[105];
      int i,l;
      while(scanf("%s",a)!=EOF)//多组输入
      {
          l=strlen(a);//取字符串长度
          for(i=0;i<l/2;i++)
          {
              if(a[i]!=a[l-i-1]) {printf("no\n");break;}//对称位不等，不是回文，退出循环
          }
          if(i>=l/2) printf("yes\n");//对比次数大于字符串长一半，是回文
      }
      return 0;
}
```
程序测试结果如下：
![](https://img-blog.csdn.net/20180925172532567?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zhbnl1bl8wMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
(2).C++实现回文算法
```cpp
#include<iostream>
#include<string>
#include<cstdlib>
#include<stack>
int main()
{
	string a;
	while (1)//多组输入
	{
		cin >> a;
		int i, flag = 1;//标志位为1
		char b;
		stack<char> q;
		for (i = 0; i < a.length(); i++)
		{
			q.push(a[i]);//依次入栈
		}
		for (i = 0; i < a.length(); i++)
		{
			b = q.top();//取栈顶
			if (b != a[i]) { flag = 0; break; }//栈顶与相应位字符不同，非回文，标志位改为0，退出循环
			q.pop();//出栈
		}
		if (flag) cout << "Yes" << endl;//标志位为1，是回文
		else     cout << "No" << endl;//标志位改变，非回文
	}
	return 0;
}
```
程序测试结果如下：
![](https://img-blog.csdn.net/20180925172938757?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zhbnl1bl8wMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
