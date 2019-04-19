# 算法笔记：简单的字符串模式匹配-BF算法 - Koma Hub - CSDN博客
2018年04月20日 20:44:02[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：195
字符串模式匹配是匹配字符串A中是否存在子串a，一般字符串的结尾为'\0'，可以以此作为字符串结束的判定标准。
其过程原理图如下所示：
![](https://img-blog.csdn.net/2018042020425596)
给出源代码：
```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define LENGTH 10
typedef unsigned char String[LENGTH + 1];//index = 0存放长度
int BF(char *A, char *a)
{
	int pos = -1;
	char *B, *b;
	int ib = 0, iB = 0;
	B = A;
	b = a;
	while (*B != '\0')
	{
		if(*b != '\0')
		{
			if(*B == *b)
			{
				b ++;ib ++;
				B ++;iB ++;
			}
			else 
			{
				B ++;iB ++;
			}
		}
		else 
		{
			return iB - ib;
		}
	}
	return -1;
}
int main(int argc, char **argv)
{
	char *A, *a;
	A = "abcdefg123hijk";
	a = "123";
	printf("A: %s\n",A);
	printf("a: %s\n",a);
	printf("pos: %d\n",BF(A,a));
	
	return 0;
}
```
运行结果：
```
D:\test>gcc BF.c
D:\test>a.exe
A: abcdefg123hijk
a: 123
pos: 7
```
然后大家有没有感觉有点奇怪？查找的子串没有回溯！没错，上面的程序是错误的，当我们输入如下：
```cpp
D:\test>gcc BF.c
D:\test>a.exe
A: abcdabddef
a: abcde
pos: 4
```
这样就错了，应该返回-1,。正确的算法是：
```cpp
int BF(char *A, char *a)
{
	int ia = 0, iA = 0;
	
	while(A[iA] != '\0')
	{
		if(a[ia] != '\0')
		{
			if(A[iA] == a[ia])
			{
				ia ++;
				iA ++;
			}
			else 
			{
				iA = iA - ia +1;
				ia = 0;
			}
		}
		else 
		{
			return iA - ia;
		}
			
	}
	return -1;
}
```
结果：
```
D:\test>gcc BF.c
D:\test>a.exe
A: abcdabddef
a: abcde
pos: -1
```
仔细看上面的代码还是有问题，当模式字符串在主串的结尾处时会出错：
```
abcdabddef
d
 d
  d
   dd
    d
     d
      ddef
-----------------
pos: -1
```
可以看到字符串匹配上了，但是返回位置不匹配，再次修改一下代码：
```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int BF(char *A, char *a)
{
	int ia = 0, iA = 0, ispace;
	printf("%s\n",A);
	while(A[iA] != '\0')
	{
		if(a[ia] != '\0')
		{
			if(A[iA] == a[ia])
			{
				printf("%c",a[ia]);
				ia ++;
				iA ++;
			}
			else 
			{
				printf("%c\n",a[ia]);
				iA = iA - ia +1;
				ia = 0;
				for(ispace = 0; ispace < iA; ispace++)
					printf("%c",' ');
			}
		}
		if(ia == strlen(a)) 
		{
			printf("\n-----------------\n");
			return iA - ia;
		}
	}
	printf("\n-----------------\n");
	return -1;
}
int main(int argc, char **argv)
{
	char *A, *a;
	A = "abcdabddef";
	a = "ddef";
	printf("A: %s\n",A);
	printf("a: %s\n",a);
	printf("pos: %d\n",BF(A,a));
	
	return 0;
}
```
测试结果：
```
D:\test>gcc BF.c
D:\test>a.exe
A: abcdabddef
a: ddef
abcdabddef
d
 d
  d
   dd
    d
     d
      ddef
-----------------
pos: 6
```
