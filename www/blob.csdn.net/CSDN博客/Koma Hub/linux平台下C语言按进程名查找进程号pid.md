# linux平台下C语言按进程名查找进程号pid - Koma Hub - CSDN博客
2018年09月15日 10:02:39[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：387
首先建立一个while循环可执行文件
```cpp
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
        while(1);
}
```
编译运行：
```
$ gcc forever.c -o forever
$ ./forever.exe &
```
查看进程编号：
```
$top
top - 09:59:45 up 21 days, 11:02,  0 users,  load average: 2.94, 2.40, 1.79
任务:   5 total,   2 running,   3 sleeping,   0 stopped,   0 zombie
%Cpu0  :  13.9/3.0    17[|||||||||                                            ]
%Cpu1  :  33.7/1.0    35[|||||||||||||||||||                                  ]
%Cpu2  :  26.7/2.0    29[|||||||||||||||                                      ]
%Cpu3  :  29.7/1.0    31[|||||||||||||||||                                    ]
GiB Mem : 54.7/3.945    [                                                     ]
GiB Swap:  2.1/1.375    [                                                     ]
进▒ USER      PR  NI    VIRT    RES  %CPU %MEM     TIME+ ▒ COMMAND
  996 toa      0   0    3.4m   4.0m  99.9  0.1   0:26.04 R forever
```
是996没错了！
然后编写一个可以运行命令行的C语言函数
```cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int main(int argc, char *argv[])
{
	char cmd[256];
	memset(cmd, 0, 256);
	
	pid_t pid = getpid();
	
	if(argc < 2)
	{
		printf("Usage: ./a.exe <pid name>\n");
		return -1;
	}
	
	sprintf(cmd, "ps -e | grep %s | awk '{print $1}'", argv[1]);
	
	system(cmd);
	
	return 0;
}
```
编译运行：
```
$ gcc a.c
$ ./a.exe forever
996
```
