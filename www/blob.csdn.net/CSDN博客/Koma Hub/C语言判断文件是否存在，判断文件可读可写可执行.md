# C语言判断文件是否存在，判断文件可读可写可执行 - Koma Hub - CSDN博客
2018年09月15日 09:43:58[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：383
```cpp
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <io.h>
#define F_OK 0
#define R_OK 2
#define W_OK 4
#define X_OK 6
int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		printf("Usage: ./a.out <filename>\n");
		return -1;
	}
	if(0 == access(argv[1], F_OK))
	{
		printf("%s exist.\n", argv[1]);
	}
	if(0 == access(argv[1], R_OK))
	{
		printf("%s readable.\n", argv[1]);
	}
	if(0 == access(argv[1], W_OK))
	{
		printf("%s writeable.\n", argv[1]);
	}
	if(0 == access(argv[1], X_OK))
	{
		printf("%s executable.\n", argv[1]);
	}
}
```
运行：
```
$ gcc a.c
$ ./a.exe a.exe
a.exe exist.
a.exe readable.
a.exe writeable.
a.exe executable.
```
