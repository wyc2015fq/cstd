# 【C语言】【编程练习】字符大小写问题 - csdn_baotai的博客 - CSDN博客

2018年04月21日 15:12:29[csdn-[baotai]](https://me.csdn.net/csdn_baotai)阅读数：181


# 编写一个程序，可以一直接收键盘字符，如果是小写字符就输出对应的大写字符，如果接收的是大写字符，就输出对应的小写字符，如果是数字不输出。

小写字母的ASCII值比相应小写字母的ASCII值大32

```cpp
#include<stdio.h>
int main()
{
	int ch;
	printf("请输入一个字符:");
	while ((ch = getchar()) != EOF)
	{
		if (ch >= 'a'&&ch <= 'z')
			printf("%c", ch - 32);
		else
		if (ch >= 'A'&&ch <= 'Z')
			printf("%c", ch + 32);
		else
		if (ch >= '0'&&ch <= '9')
			break;
	}
	printf("\n");
	system("pause");
	return 0;
}
```

	int ch;
	printf("请输入一个字符:");
	while ((ch = getchar()) != EOF)
	{
		if (ch >= 'a'&&ch <= 'z')
			printf("%c", ch - 32);
		else
		if (ch >= 'A'&&ch <= 'Z')
			printf("%c", ch + 32);
		else
		if (ch >= '0'&&ch <= '9')
			break;
	}
	printf("\n");
	system("pause");
	return 0;
}

