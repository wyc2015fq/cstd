# Linux 下编译及调试 C 代码的简易指南 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [周兆熊](http://www.jobbole.com/members/zhouzxi) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
对于Linux下的C程序员来说，几乎天天都会和Linux打交道。但在很多人的眼中，Linux是一个易用性极差、靠命令驱动的操作系统，根本无法与有着友好用户界面的Windows相比。确实是这样的，即使大家的程序是运行在Linux下，基于以下种种原因，我们的大部分工作还是在Windows下完成的：
> 
第一，除了编译调试代码之外，每个程序员还有很多工作要做，像文档编写、邮件发送及回复、PPT制作等，这些工作在Windows下做要更方便快捷一些。
第二，公司及项目组的资源有限，一般不会为每个开发人员配备一台安装有Linux的机器，而是大家共用一台或少许几台Linux机器。在每台机器上建立多个用户，需要用来编译或调试程序的时候，大家用某个用户登录上去。
这样，问题就来了：自己平时是在Windows下面办公的，而自己编写的程序的运行环境又是Linux的，如何从Windows切换到Linux呢？是不是要到专门的Linux机器上去编写代码呢？我们如何在Linux下调试程序呢？本文将一一道来。
到Linux下去编译运行程序的步骤
只要在Windows下安装一个叫做SecureCRT的软件和一个叫做FileZilla的软件，便可轻松实现Windows到Linux的切换。
SecureCRT是一款支持SSH(SSH1和SSH2)的终端仿真程序，简单地说是Windows下登录Linux服务器主机的软件。FileZilla是一个免费开源的FTP软件，分为客户端版本和服务器版本两种，具备所有的FTP软件功能。
在使用SecureCRT和FileZilla之前，要确保有一台安装了Linux的机器处于运行状态(一般说来，每个开发小组都会有专门用于测试程序的机器，可以在此机器上安装Linux)。作者使用的Linux机器的IP地址为xx.xx.xx.xx，用户名为zxin10，密码为yyyy。
第一步：使用SecureCRT登录Linux。
打开SecureCRT软件，在界面上输入IP和用户名，如图1所示。
![](http://ww4.sinaimg.cn/mw690/0064cTs2jw1exwqxre7k5j30b008tt93.jpg)
图1 登录界面
然后，单击图1中的“Connect”，在出现的界面上输入密码，如图2所示。
![](http://ww2.sinaimg.cn/mw690/0064cTs2jw1exwqxs644qj30ci0370st.jpg)
图2 密码输入界面
密码输入正确之后，便登录到了Linux系统下，如图3所示。
![](http://ww2.sinaimg.cn/mw690/0064cTs2jw1exwqxso478j30ei0750sw.jpg)
图3 登录成功之后的界面
为了编译自己的程序，我们需要建立自己的文件存放目录，如图4所示。
![](http://ww2.sinaimg.cn/mw690/0064cTs2jw1exwqxtg9cxj30c8038mxb.jpg)
图4 新建个人目录
目录建立成功之后，我们便可以转到目录中去看一下，如图5所示。
![](http://ww4.sinaimg.cn/mw690/0064cTs2jw1exwqxu5zpzj30ci04dq38.jpg)
图5 转到新建目录
此时，“万事俱备，只欠东风”，我们接下来要做的工作是利用FileZilla软件将自己在Windows下编写的程序传上去。
示例程序如下：

C
```
/**********************************************************************
* 版权所有 (C)2015, Zhou Zhaoxiong。
*
* 文件名称：Hello.c
* 文件标识：无
* 内容摘要：演示Windows下编写的程序如何在Linux下执行
* 其它说明：无
* 当前版本：V1.0
* 作    者：Zhou Zhaoxiong
* 完成日期：201501028
*
**********************************************************************/
#include <stdio.h>
/**********************************************************************
* 功能描述：主函数
* 输入参数：无
* 输出参数：无
* 返 回 值：0-执行完毕
* 其它说明：无
* 修改日期            版本号     修改人                修改内容
* -------------------------------------------------------------------
* 201501028        V1.0     Zhou Zhaoxiong        创建
***********************************************************************/
int main()
{
    printf("Hello, world!\n");
    return 0;
}
```
第二步：使用FileZilla将代码上传到Linux。
将该“Hello.c”文件存放在D盘的“Test”文件夹下，并启动FileZilla，如图6所示。
![](http://ww2.sinaimg.cn/large/0064cTs2jw1exwqxv6i5ij30qb0b40tp.jpg)
图6 启动FileZilla之后的界面
在“主机(H)”中输入IP地址，在“用户名(U)”中输入“zxin10”用户名，在“密码(W)”中输入正确的密码，“端口(P)”可不填写而使用默认值，则可登录到Linux机器上去。登上去后，转到“zhouzx”目录下，并将“Hello.c”文件传上去，如图7所示。
![](http://ww1.sinaimg.cn/large/0064cTs2jw1exwr2di3iqj30q20alta2.jpg)
图7 上传文件之后的界面
此时，“Hello.c”文件已经传到了“zhouzx”目录下，现在可以对该文件进行编译了。
第三步：在Linux上编译和运行程序。
使用“gcc -g -o Hello Hello.c”命令对文件进行编译，如图8所示。
![](http://ww4.sinaimg.cn/mw690/0064cTs2jw1exwqxvjlugj30a2024wej.jpg)
图8 编译之后的结果
可以看到，编译成功之后，有“Hello”文件生成。紧接着，运行“Hello”命令，便可看到程序的输出结果，如图9所示。
![](http://ww4.sinaimg.cn/mw690/0064cTs2jw1exwqxw25qqj306q015q2s.jpg)
图9 程序的输出结果
以上便是将Windows下的程序放到Linux下去编译和运行的全过程。这里只是示例了简单的程序，实际软件开发项目中的程序要复杂很多，但基本操作流程都是类似的。当然，直接在Linux下编写程序也是可以的，如可以利用VI编辑器来写程序。但由于易用性的原因，我认为，在Windows下编写程序要更方便一点。大家要根据自己的习惯及项目组的要求来选择合理的代码编写的方式。
程序调试示例—用gdb分析core文件
在实际的软件开发项目中，程序出现问题是在所难免的。遥想本人参加工作之后首次遇到程序的情景，至今还历历在目。之前的经验告诉我，我们越是惊慌失措，问题就越是解决不了。我们要先让自己平静下来，然后再寻找解决程序问题的办法。
在Linux下做开发的朋友，想必都与core文件打过交道。当看到自己的程序运行之后出现core时，很多人都慌乱了，仿佛天快要塌下来一样。其实，我们大可不必如此，只要我们掌握了用gdb调试core文件的办法，依然可以很快定位程序问题，一举将bug消灭掉。有关Linux core文件的更多介绍，请阅读此文：[http://www.cnblogs.com/dongzhi … .html](http://www.cnblogs.com/dongzhiquan/archive/2012/01/20/2328355.html)。
这里以一个实际的程序为例，以用gdb分析core文件为例介绍了Linux下程序调试的方法，同时演示了常见gdb命令的操作方法。
在Linux下执行“ulimit –a”命令查看程序运行出错时是否会产生core文件，命令执行的结果中有“core file size = 0”表示不会产生core文件，此时要使用“ulimit -c 1000000”命令设置core文件的大小。
示例程序

C
```
/**********************************************************************
* 版权所有 (C)2015, Zhou Zhaoxiong。
*
* 文件名称：GdbDebug.c
* 文件标识：无
* 内容摘要：Gdb命令演示程序
* 其它说明：无
* 当前版本：V1.0
* 作    者：Zhou Zhaoxiong
* 完成日期：20151008
*
**********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// 数据类型重定义
typedef unsigned char       UINT8;
typedef signed   int        INT32;
typedef unsigned int        UINT32;
// 函数声明
void Sleep(UINT32 iCountMs);
void PrintInfo(void);
INT32 main();
/**********************************************************************
* 功能描述：主函数
* 输入参数：无
* 输出参数：无
* 返 回 值：无
* 其它说明：无
* 修改日期        版本号     修改人            修改内容
* -------------------------------------------------------------------
* 20151008       V1.0     Zhou Zhaoxiong      创建
***********************************************************************/
INT32 main()
{
    PrintInfo();   // 在屏幕上输出消息
    return 0;
}
/**********************************************************************
 * 功能描述： 在屏幕上输出消息
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 * 其它说明： 无
 * 修改日期            版本号            修改人           修改内容
 * ----------------------------------------------------------------------
 * 20151008            V1.0        Zhou Zhaoxiong        创建
 ************************************************************************/
void PrintInfo(void)
{
    UINT32 iLoopFlag = 0;
    UINT32 iSum      = 0;
    UINT32 iLen      = 0;
    UINT8 *pCtrStr   = NULL;
    iLen = strlen(pCtrStr);
    for (iLoopFlag = 0; iLoopFlag < iLen; iLoopFlag ++)      // 打印消息iLen次
    {
        printf("PrintInfo: hello, world!\n");
        iSum = iSum + iLoopFlag;
        Sleep(10 * 1000);   // 每10s打印一次
    }
    return;
}
/**********************************************************************
* 功能描述： 程序休眠
* 输入参数： iCountMs-休眠时间(单位:ms)
* 输出参数： 无
* 返 回 值： 无
* 其它说明： 无
* 修改日期          版本号       修改人              修改内容
* ------------------------------------------------------------------
* 20151008         V1.0     Zhou Zhaoxiong          创建
********************************************************************/
void Sleep(UINT32 iCountMs)
{
    struct timeval t_timeout = {0};
    if (iCountMs < 1000)
    {
        t_timeout.tv_sec = 0;
        t_timeout.tv_usec = iCountMs * 1000;
    }
    else
    {
        t_timeout.tv_sec = iCountMs / 1000;
        t_timeout.tv_usec = (iCountMs % 1000) * 1000;
    }
    select(0, NULL, NULL, NULL, &t_timeout);   // 调用select函数阻塞程序
}
```
用gdb分析core文件
在Linux上用“gcc -g -o GdbDebug GdbDebug.c”命令对程序进行编译之后，运行“GdbDebug”命令，发现在当前目录下出现了core文件。利用gdb命令对core文件进行分析的过程如下所示：

C
```
~/zhouzhaoxiong/zzx/GdbDebug> gdb GdbDebug core     -- 启动gdb对core文件的分析
GNU gdb (GDB) SUSE (7.3-0.6.1)
Copyright (C) 2011 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "x86_64-suse-linux".
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>...
Reading symbols from /home/zhou/zhouzhaoxiong/zzx/GdbDebug/GdbDebug...done.
Core was generated by `GdbDebug'.
Program terminated with signal 11, Segmentation fault.
#0  0x00007f4a736f9812 in __strlen_sse2 () from /lib64/libc.so.6
(gdb) where          -- 查看程序出问题的地方
#0  0x00007f4a736f9812 in __strlen_sse2 () from /lib64/libc.so.6
#1  0x000000000040061a in PrintInfo () at GdbDebug.c:64   -- 可以看到，在GdbDebug.c文件的第64行出的问题
#2  0x00000000004005e5 in main () at GdbDebug.c:41
(gdb) b 41           -- 在GdbDebug.c文件第41行设立断点
Breakpoint 1 at 0x4005e0: file GdbDebug.c, line 41.
(gdb) b 64           -- 在GdbDebug.c文件第64行设立断点
Breakpoint 2 at 0x400611: file GdbDebug.c, line 64.
(gdb) info b         -- 显示断点信息
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x00000000004005e0 in main at GdbDebug.c:41
2       breakpoint     keep y   0x0000000000400611 in PrintInfo at GdbDebug.c:64
(gdb) r              -- 运行GdbDebug
Starting program: /home/zhou/zhouzhaoxiong/zzx/GdbDebug/GdbDebug 
Breakpoint 1, main () at GdbDebug.c:41
41          PrintInfo();   // 在屏幕上输出消息
(gdb) n             -- 执行下一步
Breakpoint 2, PrintInfo () at GdbDebug.c:64
64              iLen = strlen(pCtrStr);
(gdb) p iLen        -- 打印(输出)iLen的值
$1 = 0
(gdb) p iLoopFlag   -- 打印(输出)iLoopFlag的值
$2 = 0
(gdb) c             -- 继续执行     
Continuing.
Program received signal SIGSEGV, Segmentation fault.    -- 程序core掉了
0x00007ffff7ae9812 in __strlen_sse2 () from /lib64/libc.so.6
(gdb) q             -- 退出gdb
A debugging session is active.
        Inferior 1 [process 26640] will be killed.
Quit anyway? (y or n) y
~/zhouzhaoxiong/zzx/GdbDebug>
```
从以上分析可知，执行GdbDebug.c文件的第64行时程序core掉了。此时仔细分析程序，发现pCtrStr指针为空。当对一个不存在的指针取长度时，由于找不到地址，程序便崩溃了。修改的办法也非常的简单，只需要让pCtrStr指针指向具体的地址即可。
常见gdb命令操作示例
修改之后的代码如下：

C
```
/**********************************************************************
* 版权所有 (C)2015, Zhou Zhaoxiong。
*
* 文件名称：GdbDebug.c
* 文件标识：无
* 内容摘要：Gdb命令演示程序
* 其它说明：无
* 当前版本：V1.0
* 作    者：Zhou Zhaoxiong
* 完成日期：20151008
*
**********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// 数据类型重定义
typedef unsigned char       UINT8;
typedef signed   int        INT32;
typedef unsigned int        UINT32;
// 函数声明
void Sleep(UINT32 iCountMs);
void PrintInfo(void);
INT32 main();
/**********************************************************************
* 功能描述：主函数
* 输入参数：无
* 输出参数：无
* 返 回 值：无
* 其它说明：无
* 修改日期        版本号     修改人            修改内容
* -------------------------------------------------------------------
* 20151008       V1.0    Zhou Zhaoxiong       创建
***********************************************************************/
INT32 main()
{
    PrintInfo();   // 在屏幕上输出消息
    return 0;
}
/**********************************************************************
 * 功能描述： 在屏幕上输出消息
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 * 其它说明： 无
 * 修改日期            版本号            修改人           修改内容
 * ----------------------------------------------------------------------
 * 20151008           V1.0         Zhou Zhaoxiong        创建
 ************************************************************************/
void PrintInfo(void)
{
    UINT32 iLoopFlag = 0;
    UINT32 iSum      = 0;
    UINT32 iLen      = 0;
    UINT8 *pCtrStr   = "hello, world!";  // 修改了这行代码
    iLen = strlen(pCtrStr);
    for (iLoopFlag = 0; iLoopFlag < iLen; iLoopFlag ++)      // 打印消息iLen次
    {
        printf("PrintInfo: hello, world!\n");
        iSum = iSum + iLoopFlag;
        Sleep(10 * 1000);   // 每10s打印一次
    }
    return;
}
/**********************************************************************
* 功能描述： 程序休眠
* 输入参数： iCountMs-休眠时间(单位:ms)
* 输出参数： 无
* 返 回 值： 无
* 其它说明： 无
* 修改日期          版本号       修改人              修改内容
* ------------------------------------------------------------------
* 20151008         V1.0     Zhou Zhaoxiong          创建
********************************************************************/
void Sleep(UINT32 iCountMs)
{
    struct timeval t_timeout = {0};
    if (iCountMs < 1000)
    {
        t_timeout.tv_sec = 0;
        t_timeout.tv_usec = iCountMs * 1000;
    }
    else
    {
        t_timeout.tv_sec = iCountMs / 1000;
        t_timeout.tv_usec = (iCountMs % 1000) * 1000;
    }
    select(0, NULL, NULL, NULL, &t_timeout);   // 调用select函数阻塞程序
}
```
编译并运行之后，程序正常，说明问题已被我们解决掉。下面是常见的gdb命令的操作示例：

C
```
~/zhouzhaoxiong/zzx/GdbDebug> gdb GdbDebug    -- 启动gdb调试
GNU gdb (GDB) SUSE (7.3-0.6.1)
Copyright (C) 2011 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "x86_64-suse-linux".
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>...
Reading symbols from /home/zhou/zhouzhaoxiong/zzx/GdbDebug/GdbDebug...done.
(gdb) b 64     -- 在GdbDebug.c文件第64行设立断点
Breakpoint 1 at 0x400611: file GdbDebug.c, line 64.
(gdb) b 72     -- 在GdbDebug.c文件第72行设立断点
Breakpoint 2 at 0x400637: file GdbDebug.c, line 72.
(gdb) info b   -- 显示断点信息
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x0000000000400611 in PrintInfo at GdbDebug.c:64
2       breakpoint     keep y   0x0000000000400637 in PrintInfo at GdbDebug.c:72
(gdb) r        -- 运行GdbDebug
Starting program: /home/zhou/zhouzhaoxiong/zzx/GdbDebug/GdbDebug 
Breakpoint 1, PrintInfo () at GdbDebug.c:64
64              iLen = strlen(pCtrStr);
(gdb) p iLen    -- 打印(输出)iLen的值
$1 = 0
(gdb) n         -- 执行下一步
66              for (iLoopFlag = 0; iLoopFlag < iLen; iLoopFlag ++)      // 打印消息iLen次
(gdb) n         -- 执行下一步
68              printf("PrintInfo: hello, world!\n");
(gdb) p iLoopFlag   -- 打印(输出)iLoopFlag的值
$2 = 0
(gdb) p iLen    -- 打印(输出)iLen的值
$3 = 13
(gdb) n         -- 执行下一步
PrintInfo: hello, world!    -- 程序的输出结果
70                      iSum = iSum + iLoopFlag;
(gdb) p iSum    -- 打印(输出)iSum的值
$4 = 0
(gdb) n        -- 执行下一步
Breakpoint 2, PrintInfo () at GdbDebug.c:72
72                      Sleep(10 * 1000);   // 每10s打印一次
(gdb) n      
66              for (iLoopFlag = 0; iLoopFlag < iLen; iLoopFlag ++)      // 打印消息iLen次
(gdb) p iLoopFlag
$5 = 0
(gdb) n
68              printf("PrintInfo: hello, world!\n");
(gdb) p iLoopFlag
$6 = 1
(gdb) n
PrintInfo: hello, world!
70                      iSum = iSum + iLoopFlag;
(gdb) p iSum
$7 = 0
(gdb) n
Breakpoint 2, PrintInfo () at GdbDebug.c:72
72                      Sleep(10 * 1000);   // 每10s打印一次
(gdb) p iSum
$8 = 1
(gdb) finish        -- 一直运行到函数返回
Run till exit from #0  PrintInfo () at GdbDebug.c:72
PrintInfo: hello, world!
Breakpoint 2, PrintInfo () at GdbDebug.c:72
72                      Sleep(10 * 1000);   // 每10s打印一次
(gdb) c           -- 继续执行 
Continuing.
PrintInfo: hello, world!
Breakpoint 2, PrintInfo () at GdbDebug.c:72
72                      Sleep(10 * 1000);   // 每10s打印一次
(gdb) bt            -- 打印当前的函数调用栈的所有信息
#0  PrintInfo () at GdbDebug.c:72
#1  0x00000000004005e5 in main () at GdbDebug.c:41
(gdb) q              -- 退出gdb
A debugging session is active.
        Inferior 1 [process 26685] will be killed.
Quit anyway? (y or n) y
~/zhouzhaoxiong/zzx/GdbDebug>
```
作为Linux下调试C/C++程序的工具，大家一定要熟练掌握gdb的用法。
总结
Linux具有免费、可靠、安全、稳定、多平台等特点，因此深受全球各大IT厂商的追捧。Linux操作系统的两大主要应用领域是服务器领域和嵌入式Linux系统。不管你从事的开发工作是否与Linux有关，掌握Linux下的软件开发方法总是有好处的。
