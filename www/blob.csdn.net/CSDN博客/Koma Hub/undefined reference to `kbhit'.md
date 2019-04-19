# undefined reference to `kbhit' - Koma Hub - CSDN博客
2019年03月13日 22:02:54[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：19
个人分类：[C/C++](https://blog.csdn.net/Rong_Toa/article/category/7156199)
kbhit()接口在头文件conio.h（控制台输入输出console input/output）中。
> 
**百度百科：**
- conio.h不是C标准库中的头文件，在C standard library，ISO C 和[POSIX](https://baike.baidu.com/item/POSIX)标准中均没有定义。
- conio是Console Input/Output（控制台输入输出）的简写，其中定义了通过控制台进行数据输入和[数据输出](https://baike.baidu.com/item/%E6%95%B0%E6%8D%AE%E8%BE%93%E5%87%BA/5885826)的函数，主要是一些用户通过按键盘产生的对应操作，比如getch()函数等等。
- 大部分[DOS](https://baike.baidu.com/item/DOS)，[Windows 3.x](https://baike.baidu.com/item/Windows%203.x)，Phar Lap，DOSX，[OS/2](https://baike.baidu.com/item/OS%2F2) or [Win32](https://baike.baidu.com/item/Win32)平台上的C[编译器](https://baike.baidu.com/item/%E7%BC%96%E8%AF%91%E5%99%A8/8853067)提供此文件，[UNIX](https://baike.baidu.com/item/UNIX) 和[Linux](https://baike.baidu.com/item/Linux)平台的c编译器通常不包含此头文件。
- 如果需要使用此头文件，可以从互联网下载。
# conio.h
```cpp
cgets(char*);;
cputs(constchar*);
cscanf(constchar*,...);
inpw(unsignedshort);
getch(void);
getche(void);
kbhit(void);
outp(unsignedshort,int);
outpw(unsignedshort,unsignedshort);
putch(int);
ungetch(int);
void_Cdeclclreol(void);
void_Cdeclclrscr(void);
void_Cdecldelline(void);
int_Cdeclgettext(intleft,inttop,intright,intbottom,void*destin);
void_Cdeclgettextinfo(structtext_info*r);
void_Cdeclgotoxy(intx,inty);
void_Cdeclhighvideo(void);
void_Cdeclinsline(void);
void_Cdecllowvideo(void);
int_Cdeclmovetext(intleft,inttop,intright,intbottom,intdestleft,intdesttop);
void_Cdeclnormvideo(void);
int_Cdeclputtext(intleft,inttop,intright,intbottom,void*source);
void_Cdecltextattr(intnewattr);
void_Cdecltextbackground(intnewcolor);
void_Cdecltextcolor(intnewcolor);
void_Cdecltextmode(intnewmode);
int_Cdeclwherex(void);
int_Cdeclwherey(void);
void_Cdeclwindow(intleft,inttop,intright,intbottom);
char*_Cdeclcgets(char*str);
int_Cdeclcprintf(constchar*format,...);
int_Cdeclcputs(constchar*str);
int_Cdeclcscanf(constchar*format,...);
int_Cdeclgetch(void);
int_Cdeclgetche(void);
char*_Cdeclgetpass(constchar*prompt);
int_Cdeclkbhit(void);
int_Cdeclputch(intc);
int_Cdeclungetch(intch);
```
