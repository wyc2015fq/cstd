# C语言使用函数参数传递中的省略号：va_list, va_start, va_arg, va_end - Koma Hub - CSDN博客
2018年08月26日 10:40:44[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：152
首先要处理这种省略号的参数的话，需要包含头文件#include <stdarg.h>，然后利用下面的函数对“...”省略号变量进行处理。
```cpp
va_list arg;
type va_arg( va_list arg, type );
void va_end( va_list arg );
void va_start( va_list arg, prev_param );
```
下面实现一个自己的print()，它的参数的传递是根据具体输入的个数决定的。如下：
```cpp
void print(char *msg, ...)
{
	char *p = msg;
	
	va_list arg;
	
	va_start(arg, msg);
	
	int _i;
	char _c;
	char * _s;
	
	while(*p != '\0')
	{
		switch(*p)
		{
			case '%':
				p++;
				switch(*p)
				{
					case 'd':
						_i = va_arg(arg, int);
						printf("%d", _i);
						break;
					case 'c':
						_c = va_arg(arg, int);
						printf("%c", _c);
						break;
					case 's':
						_s = va_arg(arg, char*);
						printf("%s", _s);
						break;
					default:
						break;
				}
				break;
			default:
				printf("%c", *p);
				break;
		}
		p++;
	}
	
	va_end(arg);
}
```
使用过程：
```cpp
print("Hello %d, %s\n", 12, "World\n");
```
给出我的输出：
```
$ gcc main.c
$ ./a
Hello 12, World
```
当然这只是最简单的实现，实际的处理过程要比这复杂得多。仅供学习参考！！
给出完整的代码：
```cpp
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
void print(char *msg, ...)
{
	char *p = msg;
	
	va_list arg;
	
	va_start(arg, msg);
	
	int _i;
	char _c;
	char * _s;
	
	while(*p != '\0')
	{
		switch(*p)
		{
			case '%':
				p++;
				switch(*p)
				{
					case 'd':
						_i = va_arg(arg, int);
						printf("%d", _i);
						break;
					case 'c':
						_c = va_arg(arg, int);
						printf("%c", _c);
						break;
					case 's':
						_s = va_arg(arg, char*);
						printf("%s", _s);
						break;
					default:
						break;
				}
				break;
			default:
				printf("%c", *p);
				break;
		}
		p++;
	}
	
	va_end(arg);
}
int main(int argc, char **argv)
{
	print("Hello %d, %s\n", 12, "World\n");
	
	return 0;
}
```
