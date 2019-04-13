
# printf 宏 调试技巧 - 嵌入式Linux - CSDN博客

2018年10月10日 08:43:10[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：101



# 1 前言
printf调试是嵌入式调试的基本手段，而且是非常重要的手段，我认为相比单步调试更加有用有效，特别是单片机之后跑系统，单步调试效率更加低下了，我们在工作遇到bug的时候，我们第一时间就想知道那些该死的日志有没有保存下来，这样好让我们程序员装逼一波把问题解决。
printf宏定义调试非常重要，有些日志在开发的时候才需要打开，发布的时候需要关闭，但是在代码上又需要保留下次调试，所以我们在调试的时候才打开调试宏定义，而且printf会占用空间，很多芯片的空间非常有限，更应该关闭调试宏。
下面就直接进入正题，说一下调试的技巧

# 2. 正文

## 1 编译器内置宏
先介绍几个编译器内置的宏定义，这些宏定义不仅可以帮助我们完成跨平台的源码编写，灵活使用也可以巧妙地帮我们输出非常有用的调试信息。
> ANSI C标准中有几个标准预定义宏（也是常用的）：

> __LINE__：在源代码中插入当前源代码行号；

> __FILE__：在源文件中插入当前源文件名；

> __DATE__：在源文件中插入当前的编译日期

> __TIME__：在源文件中插入当前编译时间；

> __STDC__：当要求程序严格遵循ANSI C标准时该标识被赋值为1；

> __cplusplus：当编写C++程序时该标识符被定义。
编译器在进行源码编译的时候，会自动将这些宏替换为相应内容。

## 2 最基本的用法
**打开宏的时候输出**

**关闭宏的时候输出**

## 3 换个高级的用法
## 代码如下
`#include <stdio.h>
#define __DEBUG__
#ifdef __DEBUG__
#define DEBUG(format,...) printf("Date: "__DATE__",File: "__FILE__", Line: %05d: "format"\n", __LINE__, ##__VA_ARGS__)
#else
#define DEBUG(format,...)
#endif
int main(int argc, char **argv) {
    char str[]="Hello World";
    DEBUG("%s",str);
    return 0;
}`
**输出如下**
`Date: Oct  5 2018,File: /code/main.c, Line: 00013: Hello World
sandbox> exited with status 0`
在线编译器网址：https://tool.lu/coderunner/

## 4 \#\# __VA_ARGS__ ... 宏和可变参数
在GNU C中，宏可以接受可变数目的参数，就象函数一样
**例如:**
`#define pr_debug(fmt,arg...) \ 
printk(KERN_DEBUG fmt, ##arg)`用可变参数宏(variadic macros)传递可变参数表
你可能很熟悉在函数中使用可变参数表，如:
`void printf(const char* format, ...);`直到最近，可变参数表还是只能应用在真正的函数中，不能使用在宏中。
C99编译器标准允许你可以定义可变参数宏(variadic macros)，这样你就可以使用拥有可以变化的参数表的宏。可变参数宏就像下面这个样子:
`#define debug(...) printf(__VA_ARGS__)`**缺省号代表一个可以变化的参数表。使用保留名 __VA_ARGS__ 把参数传递给宏。当宏的调用展开时，实际的参数就传递给 printf()了**
**例如:**
`debug("Y = %d\n", y);`而处理器会把宏的调用替换成:
`printf("Y = %d\n", y);`因为debug()是一个可变参数宏，你能在每一次调用中传递不同数目的参数:
`debug("test");// 一个参数`用GCC和C99的可变参数宏， 更方便地打印调试信息
可变参数宏不被ANSI/ISO C++ 所正式支持。因此，你应当检查你的编译器，看它是否支持这项技术。
**可变参数的宏里的'\#\#'操作说明带有可变参数的宏(Macros with a Variable Number of Arguments)**
**更详细请查看如下链接**
http://www.cnblogs.com/alexshi/archive/2012/03/09/2388453.html

## 5 举个栗子-Linux内核调试宏
下面是Android touchscreen驱动的调试宏用法，看这样的写法就是一个大神了，给大家借鉴。
`// Log define
#define GTP_ERROR(fmt,arg...)          printk("<<-GTP-ERROR->> "fmt"\n",##arg)
#if DEBUG_SWITCH
#define GTP_INFO(fmt,arg...)           printk("<<-GTP-INFO->> "fmt"\n",##arg)
#define GTP_DEBUG(fmt,arg...)          do{\
                                         if(GTP_DEBUG_ON)\
                                         printk("<<-GTP-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                       }while(0)
#define GTP_DEBUG_ARRAY(array, num)    do{\
                                         s32 i;\
                                         u8* a = array;\
                                         if(GTP_DEBUG_ARRAY_ON)\
                                         {\
                                            printk("<<-GTP-DEBUG-ARRAY->>\n");\
                                            for (i = 0; i < (num); i++)\
                                            {\
                                                printk("%02x   ", (a)[i]);\
                                                if ((i + 1 ) %10 == 0)\
                                                {\
                                                    printk("\n");\
                                                }\
                                            }\
                                            printk("\n");\
                                        }\
                                       }while(0)
#define GTP_DEBUG_FUNC()               do{\
                                         if(GTP_DEBUG_FUNC_ON)\
                                         printk("     <<-GTP-FUNC->>       Func:%s@Line:%d\n",__func__,__LINE__);\
                                       }while(0)
#else
#define GTP_INFO(fmt,arg...)
#define GTP_DEBUG(fmt,arg...)
#define GTP_DEBUG_ARRAY(array, num)
#define GTP_DEBUG_FUNC()
#endif`

---

**欢迎加我微信（weiqifa0）拉大家进微信技术讨论群**

**欢迎关注微信公众号-嵌入式Linux**


