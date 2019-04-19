# C语言在linux终端下实现2048小游戏：第二版 - Koma Hub - CSDN博客
2018年09月15日 18:25:28[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：352
原来我转载过一个机遇ncurses的2048，今天无聊自己手写了一个，看下我的目录结构：
```
$ tree ../2048/
../2048/
├── 2048.c
├── 2048.h
└── main.c
0 directories, 3 files
```
# 2048.h
```cpp
#ifndef _2048_H
#define _2048_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <curses.h>
#include <time.h>
#include <math.h>
#include <termios.h>
#define VK_LEFT 	(97)
#define VK_UP 		(119)
#define VK_RIGHT 	(100)
#define	VK_DOWN 	(115)
#define VK_QUIT		(113)
int table[16];
int init();
void show();
int random_position();
void handle(char VK);
int check_zero();
int check_alive();
char getch_from_terminal();
#endif /*<_2048_H>*/
```
# 2048.c
```cpp
#include "2048.h"
int init()
{
	int id;
	for(id=0; id<16; id++)
	{
		table[id]   = 0;
	}
	table[random_position()] = 1;
	
	return 0;
}
void show()
{
	system("clear");
	printf("\n\n");
	int ix, iz, id;
	char ch;
	
	for(ix=0; ix<4; ix++)
	{
		for(iz=0; iz<4; iz++)
		{
			id = iz*4 + ix;
			
			printf("%7d", table[id]);
		}
		printf("\n\n");
	}
	if(-1 == check_alive())
	{
		printf("You die!\n");
		printf("press <y> to play, <n> to quit\n");
		
switch_ch:
		ch = getchar();
		switch(ch)
		{
			case 'y':
				init();
				show();
				break;
			case 'n':
				printf("Goodbye! Have a good day!\n");
				exit(1);
				break;
			default:
				goto switch_ch;
				break;
		}
		
		
	}
}
int random_position()
{
	srand( (unsigned)time( NULL ) ); 
	int id;
	if(0 == check_zero())
	{
		while(1)
		{
			id = rand()%16;
			if(table[id] == 0)
			{
				return id;
			}
		}
	}
}
int check_zero()
{
	int id, i, j, ij;
	for(id=0; id<16; id++)
	{
		if(table[id] != 0)
		{
			if(id == 15)
			{
				return -1;
			}
			continue;
		}
		else
		{
			return 0;
		}
	}
}
int check_alive()
{
	int id, ix, iz;
	
	
	if(table[0] == table[1] ||
	   table[0] == table[4] ||
	   table[3] == table[2] ||
	   table[3] == table[7] ||
	   table[12] == table[13] ||
	   table[12] == table[8]  ||
	   table[15] == table[14] ||
	   table[15] == table[11] ||
	   table[2] == table[3] ||
	   table[4] == table[8] ||
	   table[7] == table[11] ||
	   table[13] == table[14])
	{
		return 0;
	}
	
	for(ix=0; ix<4; ix++)
	{
		for(iz=0; iz<4; iz++)
		{
			id = ix*4+iz;
			if(table[id] == 0)
			{
				return 0;
			}
			if(ix >= 1 && ix <= 2 && iz >=1 && iz <= 2)
			{
				if(table[id] == table[id-1])
				{
					return 0;
				}
				if(table[id] == table[id+1])
				{
					return 0;
				}
				if(table[id] == table[id-4])
				{
					return 0;
				}
				if(table[id] == table[id+4])
				{
					return 0;
				}
			}
			
		}
	}
	return -1;
}
void handle(char VK)
{
	int ix, iz, i, id;
	
	switch(VK)
	{
		case VK_UP:
			for(ix=0; ix<4; ix++)
			{
loop_VK_UP_1:
				for(iz=0; iz<4; iz++)
				{
					if(table[ix*4+iz] == 0)
					{
						for(i=iz; i<4; i++)
						{
							if(i==3)
							{
								table[ix*4+i] = 0;
							}
							else
							{
								table[ix*4+i] = table[ix*4+i+1];
							}
						}
					}
				}
				for(iz=0; iz<4; iz++)
				{
					if(table[ix*4+iz] != 0)
					{
						for(i=0; i<iz; i++)
						{
							if(table[ix*4+i] == 0)
							{
								goto loop_VK_UP_1;
							}
						}
					}
				}
loop_VK_UP_2:
				for(iz=0; iz<3; iz++)
				{
					if(table[ix*4+iz] == table[ix*4+iz+1])
					{
						table[ix*4+iz] 	= table[ix*4+iz]	+ table[ix*4+iz+1];
						for(i=iz+1; i<4; i++)
						{
							if(i==3)
							{
								table[ix*4+i] = 0;
							}
							else
							{
								table[ix*4+i] = table[ix*4+i+1];
							}
						}
					}
				}
				for(iz=0; iz<3; iz++)
				{
					if(table[ix*4+iz] != 0 && table[ix*4+iz] == table[ix*4+iz+1])
					{
						goto loop_VK_UP_2;
					}
				}
			}
			table[random_position()] = 1;
			break;
			
		case VK_DOWN:
			for(ix=0; ix<4; ix++)
			{
loop_VK_DOWN_1:
				for(iz=3; iz>=0; iz--)
				{
					if(table[ix*4+iz] == 0)
					{
						for(i=iz; i>=0; i--)
						{
							if(i==0)
							{
								table[ix*4+i] = 0;
							}
							else
							{
								table[ix*4+i] = table[ix*4+i-1];
							}
						}
					}
				}
				for(iz=3; iz>=0; iz--)
				{
					if(table[ix*4+iz] != 0)
					{
						for(i=3; i>iz; i--)
						{
							if(table[ix*4+i] == 0)
							{
								goto loop_VK_DOWN_1;
							}
						}
					}
				}
loop_VK_DOWN_2:
				for(iz=3; iz>=1; iz--)
				{
					if(table[ix*4+iz] == table[ix*4+iz-1])
					{
						table[ix*4+iz] 	= table[ix*4+iz]	+ table[ix*4+iz-1];
						for(i=iz-1; i>=0; i--)
						{
							if(i==0)
							{
								table[ix*4+i] = 0;
							}
							else
							{
								table[ix*4+i] = table[ix*4+i-1];
							}
						}
					}
				}
				for(iz=3; iz>=1; iz--)
				{
					if(table[ix*4+iz] != 0 && table[ix*4+iz] == table[ix*4+iz-1])
					{
						goto loop_VK_DOWN_2;
					}
				}
			}
			table[random_position()] = 1;
			break;
			
		case VK_LEFT:
			for(iz=0; iz<4; iz++)
			{
loop_VK_LEFT_1:
				for(ix=0; ix<4; ix++)
				{
					if(table[ix*4+iz] == 0)
					{
						for(i=ix; i<4; i++)
						{
							if(i==3)
							{
								table[i*4+iz] = 0;
							}
							else
							{
								table[i*4+iz] = table[(i+1)*4+iz];
							}
						}
					}
				}
				for(ix=0; ix<4; ix++)
				{
					if(table[ix*4+iz] != 0)
					{
						for(i=0; i<ix; i++)
						{
							if(table[i*4+iz] == 0)
							{
								goto loop_VK_LEFT_1;
							}
						}
					}
				}
loop_VK_LEFT_2:
				for(ix=0; ix<3; ix++)
				{
					if(table[ix*4+iz] == table[(ix+1)*4+iz])
					{
						table[ix*4+iz] 	= table[ix*4+iz]	+ table[(ix+1)*4+iz];
						for(i=ix+1; i<4; i++)
						{
							if(i==3)
							{
								table[i*4+iz] = 0;
							}
							else
							{
								table[i*4+iz] = table[(i+1)*4+iz];
							}
						}
					}
				}
				for(ix=0; ix<3; ix++)
				{
					if(table[ix*4+iz] != 0 && table[ix*4+iz] == table[(ix+1)*4+iz])
					{
						goto loop_VK_LEFT_2;
					}
				}
			}
			table[random_position()] = 1;
			break;
			
		case VK_RIGHT:
			for(iz=0; iz<4; iz++)
			{
loop_VK_RIGHT_1:
				for(ix=3; ix>=0; ix--)
				{
					if(table[iz+ix*4] == 0)
					{
						for(i=ix; i>=0; i--)
						{
							if(i==0)
							{
								table[iz+i*4] = 0;
							}
							else
							{
								table[iz+i*4] = table[iz+(i-1)*4];
							}
						}
					}
				}
				for(ix=3; ix>=0; ix--)
				{
					if(table[iz+ix*4] != 0)
					{
						for(i=3; i>ix; i--)
						{
							if(table[iz+i*4] == 0)
							{
								goto loop_VK_RIGHT_1;
							}
						}
					}
				}
loop_VK_RIGHT_2:
				for(ix=3; ix>=1; ix--)
				{
					if(table[iz+ix*4] == table[iz+(ix-1)*4])
					{
						table[iz+ix*4] 	= table[iz+ix*4]	+ table[iz+(ix-1)*4];
						for(i=ix-1; i>=0; i--)
						{
							if(i==0)
							{
								table[iz+i*4] = 0;
							}
							else
							{
								table[iz+i*4] = table[iz+(i-1)*4];
							}
						}
					}
				}
				for(ix=3; ix>=1; ix--)
				{
					if(table[iz+ix*4] != 0 && table[iz+ix*4] == table[iz+(ix-1)*4])
					{
						goto loop_VK_RIGHT_2;
					}
				}
			}
			table[random_position()] = 1;
			break;
			
		default:
			break;
	}
}
char getch_from_terminal()
/*https://blog.csdn.net/Timsley/article/details/51424068*/
{
	FILE *input;
    FILE *output;
    struct termios initial_settings, new_settings;
    int inputString;
    input = fopen("/dev/tty", "r");
    output = fopen("/dev/tty", "w");
    if(!input || !output)
    {
        fprintf(stderr, "Unable to open /dev/tty\n");
        exit(1);
    }
    tcgetattr(fileno(input), &initial_settings);
    new_settings = initial_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;
    new_settings.c_lflag &= ~ISIG;
    if(tcsetattr(fileno(input), TCSANOW, &new_settings) != 0)
    {
        fprintf(stderr, "Could not set attributes\n");
    }
    //while(inputString != 'q')
    //{
        //do
        //{
            inputString = fgetc(input);
        //}while(inputString == '\n' || inputString == '\r');
        //return (char )inputString;
    //}
    tcsetattr(fileno(input), TCSANOW, &initial_settings);
	
	return (char)inputString;
}
```
# main.c
```cpp
#include "2048.h"
/*
     64     32     16      8
     32     16      8      4
     16      8      4      2
      8      4      2      1
You die!
press <y> to play, <n> to quit
*/
int main()
{
	system("clear");
	init();
	
	show();
	
	char ch;
	
	while((ch = getch_from_terminal()) != VK_QUIT)
	//while((ch = getchar()) != VK_QUIT)
	{
		switch(ch)
		{
			case VK_UP:
				handle(VK_UP);
				show();
				break;
				
			case VK_DOWN:
				handle(VK_DOWN);
				show();
				break;
				
			case VK_LEFT:
				handle(VK_LEFT);
				show();
				break;
				
			case VK_RIGHT:
				handle(VK_RIGHT);
				show();
				break;
				
			case 'q':
				exit(1);
				break;
				
			default:
				printf("<w><s><a><d>\n");
				break;
		}
	}
	
	
	return 0;
}
```
# 编译运行：
```
$ gcc main.c -I . 2048.c
$ ./a.exe
      1      2      8      1
     32     16      4      2
      1     64     32      8
      2      1      4      2
You die!
press <y> to play, <n> to quit
Goodbye! Have a good day!
rongtao@DESKTOP-OG70MJ9 ~/2048
```
完整代码在：[https://github.com/Rtoax/2048](https://github.com/Rtoax/2048)
