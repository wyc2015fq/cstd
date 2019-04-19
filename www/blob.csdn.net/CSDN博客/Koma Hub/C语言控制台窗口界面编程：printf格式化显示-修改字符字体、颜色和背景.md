# C语言控制台窗口界面编程：printf格式化显示-修改字符字体、颜色和背景 - Koma Hub - CSDN博客
2018年04月02日 19:56:43[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：1416
环境：cygwin-Linux+GCC
PS：windows的cmd下不能实现。
在linux终端中一个ls指令得出的显示结果文件夹、文件以及不同操作权限的文件的显示颜色是不同的，那么这个是如何实现的呢？
![](https://img-blog.csdn.net/20180402194457276)
从上图中看到的，不同的颜色信息有助于判断不同的文件类型。以下面的printf格式化输出为例：
```cpp
#include<stdio.h>
#include<stdlib.h>
int main(int argc, char **argv)
{
        printf("\033[1m加粗\033[m\n");
	printf("\033[1;30m加粗黑色字体\033[m\n");
	printf("\033[1;30;41m加粗黑色字体红色背景\033[m\n");
	
        printf("\033[3m斜体\033[m\n");
	printf("\033[3;34m斜体深蓝色字体\033[m\n");
	printf("\033[3;34;47m斜体深蓝色字体白色背景\033[m\n");
	
        printf("\033[9m删除线\033[m\n");
	printf("\033[9;35m删除线紫色字体\033[m\n");
	printf("\033[9;35;43m删除线紫色字体黄色背景\033[m\n");
	
}
```
得到的输出结果为：
![](https://img-blog.csdn.net/20180402194656465)
这是一个格式化输出的结果，其格式化为（[原文链接](https://blog.csdn.net/talk_8/article/details/51764795)）：
```cpp
/* \033[字符的显示方式;字符的颜色;字符的背景颜色m 需要显示的字符 \033[m */
	/* 字符的显示方式:
	0（默认值）、1（加粗）、2（灰显）、3（斜体）、4（下划线）、7（反显）、9（删除线）
       字符的颜色:
	30（黑色）、31（红色）、32（绿色）、 33（黄色）、34（深蓝色）、
	35（紫色）、36（浅蓝色）、37（白色）
       字符的背景颜色:
	40（黑色）、41（红色）、42（绿色）、 43（黄色）、44（蓝色）、
	45（紫色）、46（浅蓝色）、47（白色） */
```
当然也可以这么做：
```cpp
#include<stdio.h>
#include<stdlib.h>
int main(int argc, char **argv)
{
	int fontShowWay=0, fontColor, backgroundColor, count = 0;
	
	for ( fontShowWay = 0; fontShowWay <= 9; fontShowWay ++)
		for( fontColor = 30; fontColor <= 37; fontColor ++)
			for ( backgroundColor = 40; backgroundColor <= 47; backgroundColor ++) {
				count ++;
	printf("\033[%d;%d;%dm Hello World ! \033[m",fontShowWay, fontColor, backgroundColor);
				if(count%10 == 0)
					printf("\n");
			}
	
}
```
![](https://img-blog.csdn.net/20180402195607114)
![](https://img-blog.csdn.net/20180402195617611)
**此外，这里有一篇文章**：[C语言控制台窗口图形界面编程（三）](https://www.cnblogs.com/lxl0928/p/3792445.html)
在windows系统下进行的字体变换，但是不太好理解。
```cpp
#include <stdio.h>  
#include <stdlib.h>  
#include <windows.h>  
#include <conio.h>  
/* 
基本文本属性 
FOREGROUND_BLUE 蓝色 
FOREGROUND_GREEN 绿色 
FOREGROUND_RED 红色 
FOREGROUND_INTENSITY 加强 
BACKGROUND_BLUE 蓝色背景 
BACKGROUND_GREEN 绿色背景 
BACKGROUND_RED 红色背景 
BACKGROUND_INTENSITY 背景色加强 
COMMON_LVB_REVERSE_VIDEO 反色 
*/  
  
const WORD FORE_BLUE   = FOREGROUND_BLUE;           //蓝色文本属性  
const WORD FORE_GREEN  = FOREGROUND_GREEN;          //绿色文本属性  
const WORD FORE_RED    = FOREGROUND_RED;            //红色文本属性  
const WORD FORE_PURPLE = FORE_BLUE | FORE_RED;      //紫色文本属性  
const WORD FORE_CYAN   = FORE_BLUE | FORE_GREEN;    //青色文本属性  
const WORD FORE_YELLOW = FORE_RED | FORE_GREEN;     //黄色文本属性  
const WORD FORE_GRAY   = FOREGROUND_INTENSITY;      //灰色文本属性  
const WORD BACK_BLUE   = BACKGROUND_BLUE;           //蓝色背景属性  
const WORD BACK_GREEN  = BACKGROUND_GREEN;          //绿色背景属性  
const WORD BACK_RED    = BACKGROUND_RED;            //绿色背景属性  
const WORD BACK_PURPLE = BACK_BLUE | BACK_RED;      //紫色背景属性  
const WORD BACK_CYAN   = BACK_BLUE | BACK_GREEN;    //青色背景属性  
const WORD BACK_YELLOW = BACK_RED | BACK_GREEN;     //黄色背景属性  
const WORD BACK_GRAY   = BACKGROUND_INTENSITY;      //灰色背景属性  
  
int main()  
{  
    HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);    //获得标准输出设备句柄  
    CONSOLE_SCREEN_BUFFER_INFO csbi;                        //定义窗口缓冲区信息结构体  
    GetConsoleScreenBufferInfo(handle_out, &csbi);          //获得窗口缓冲区信息  
    SetConsoleTextAttribute(handle_out, FORE_BLUE);  
    printf("蓝色\n");  
    SetConsoleTextAttribute(handle_out, FORE_RED);  
    printf("红色字符\n");  
    SetConsoleTextAttribute(handle_out, FORE_GREEN);  
    printf("绿色字符\n");  
    SetConsoleTextAttribute(handle_out, FORE_PURPLE);  
    printf("紫色字符\n");  
    SetConsoleTextAttribute(handle_out, FORE_CYAN);  
    printf("青色字符\n");  
    SetConsoleTextAttribute(handle_out, FORE_YELLOW);  
    printf("黄色字符\n");  
    SetConsoleTextAttribute(handle_out, FORE_GRAY);  
    printf("灰色字符\n");  
    SetConsoleTextAttribute(handle_out, FORE_GREEN | FORE_BLUE | FORE_RED);  
    printf("白色字符\n");  
    SetConsoleTextAttribute(handle_out, BACK_BLUE);  
    printf("蓝色背景\n");  
    SetConsoleTextAttribute(handle_out, BACK_RED);  
    printf("红色背景\n");  
    SetConsoleTextAttribute(handle_out, BACK_GREEN);  
    printf("绿色背景\n");  
    SetConsoleTextAttribute(handle_out, BACK_PURPLE);  
    printf("紫色背景\n");  
    SetConsoleTextAttribute(handle_out, BACK_CYAN);  
    printf("青色背景\n");  
    SetConsoleTextAttribute(handle_out, BACK_YELLOW);  
    printf("黄色背景\n");  
    SetConsoleTextAttribute(handle_out, BACK_GRAY);  
    printf("灰色背景\n");  
    SetConsoleTextAttribute(handle_out, BACK_BLUE | BACK_RED | BACK_GREEN);  
    printf("白色背景\n");  
    SetConsoleTextAttribute(handle_out, BACK_GREEN | FORE_RED); //示例：绿色背景红色字符  
    printf("绿色背景与红色字符的混合\n");  
    SetConsoleTextAttribute(handle_out, FOREGROUND_INTENSITY | FORE_RED);   //示例：亮红色字符  
    printf("亮色的生成，与加强色融合\n");  
    return 0;  
}
```
编译与运行：
![](https://img-blog.csdn.net/20180404092610884)
