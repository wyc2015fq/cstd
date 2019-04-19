# 【苦逼bug系列】之重构播放器篇 - 三少GG - CSDN博客
2011年09月08日 15:44:45[三少GG](https://me.csdn.net/scut1135)阅读数：3490
bug啊。。。要命啊。。。
两种ndk-build方式
a.libffmpeg什么都不修改，
b. 修改，按我的博文   或者我本次采用的 [http://abitno.me.sixxs.org/compile-ffmpeg-android-ndk](http://abitno.me.sixxs.org/compile-ffmpeg-android-ndk)
（又或者参考baicheng推荐。。[將 FFmpeg整合至Android平台](http://bbs.chinavideo.org/viewthread.php?tid=10849&extra=page%3D1%26amp%3Bfilter%3Dtype%26amp%3Btypeid%3D5)）        进行尝试。
本次首先使用第一种方式：
**问题1. make: *** empty variable name.  Stop (已解决)**
原因：原来是ndk路径对空格‘ ‘ 和小数点’ .'均不支持。myelipse8.6 默认workspace 的位置有个/myeclipse 8.6/ 这个路径，要删除！！！
            之后即可。但是紧接着报出第二个错误（之前sz机器上并没有这个问题，产生的原因估计跟新机器指令集有关系）
**问题2.  Compile thumb  : avcodec <= 4xm.c****（未解决）**
/home/grant/Workspaces/ffmpeg/jni/libffmpeg/libavcodec/4xm.c: In function 'decode_frame':
/home/grant/Workspaces/ffmpeg/jni/libffmpeg**/libavcodec/x86****/mathops.h:93: error: can't find a register in class 'CC_REG' while reloading 'asm'**
为什么会出现这个呢？定位到具体位置内容为
```cpp
#define NEG_USR32 NEG_USR32
static inline uint32_t NEG_USR32(uint32_t a, int8_t s){
    __asm__ ("shrl %1, %0\n\t"
         : "+r" (a)
         : "ic" ((uint8_t)(-s))
    );
    return a;
}
```
**难道是不支持 汇编语句 —asm—？ 是gcc的问题还是**ndk的问题？
原因：
参考文章：1. [http://www.diybl.com/course/6_system/linux/Linuxjs/20090309/160214.html](http://www.diybl.com/course/6_system/linux/Linuxjs/20090309/160214.html)
之前因為想找出 ass 字幕爛掉的原因，得編譯一個 debug 版本的 mplayer。但只要一加入這個選項後編譯就會爛掉，會跑出如下的錯誤訊息： 
can’t find a register in class ‘GENERAL_REGS’ while reloading ‘asm’ 
當時也沒仔細去找解法，直接用插入 printf 的方法來觀察一些變數，問題也順利解決了。不過昨天把 svn head 抓下來編譯後也是一樣，看來應該是老問題了，查了一下 maillist 發現有人問過，可以參考這幾封信： 
http://lists.mplayerhq.hu/pipermail/mplayer-dev-eng/2006-November/047222.html 
http://lists.mplayerhq.hu/pipermail/mplayer-dev-eng/2006-November/047221.html 
解決方法是在 libavcodec/Makefile 的 CFLAGS 加入 -fomit-frame-pointer 參數就好。編譯完果然變得超級肥大…看起來是可以用 gdb 來跑了： 
88M mplayer
6.9M mplayer-strip 
這問題好像留了好幾年還找不到解決方法，不過在 FAQ 中是把問題推給 gcc，所以在 gcc 的 ml 上也發現有人問過類似的 問 題….
2. 但是上面的方法也有人不成功：
```
46 for (i = 0; i < 0x20 / 2; i += 4)
  47 {
  48 asm(
  49 "movaps %2, %%xmm3\n\t"
  50 "shufps $27, %%xmm3, %%xmm3\n\t"
  51 "movups %3, %%xmm1\n\t"
我查了网上有如下说的：
export CFLAGS=-fomit-frame-pointer
但依然没有解决，Google也不好搜，这个问题难道过于容易了？请大侠指点一下，搞了好久了，无从下手啊。
多谢。
```
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
**鉴于错误太多，不得不采用第二种方法。**
**Android的GCC是不支持restrict关键字的，所以把ffmpeg源码下configure生成的config.h文件中的这一行：#define restrict restrict   改成 #define restrict    /*意思就是把后面的restrict去掉*/如果重新Configure的话记得要把这个关键字去掉。**
**完全修改后，**
**需要着重留意config.sh文件（生成config.h及config.mak），比如曾由于**
**#define HAVE_SYS_RESOURCE_H 1  （在config.h里面搜索）**
**HAVE_SYS_RESOURCE_H=yes           （在config.mak里面搜索）**
**未设置成1或yes，**
**导致**
**#if HAVE_SYS_RESOURCE_H#include <sys/types.h>#include <sys/time.h>#include <sys/resource.h>**
**未能include进来，从而缺少头文件引发 undefined reference to "rusage "or undeclared 等。**
**又或者**
**jni/com_media_ffmpeg_FFMpeg.c:642: undefined reference to `avfilter_uninit'/home/grant/Workspaces/AVSffmpeg/obj/local/armeabi/objs/ffmpeg_jni/com_media_ffmpeg_FFMpeg.o: In function `output_query_formats':/home/grant/Workspaces/AVSffmpeg/jni/jni/com_media_ffmpeg_FFMpeg.c:361: undefined reference to `avfilter_make_format_list'**
**办法：原以为跟上面一样，直接注释掉com_media_ffmpeg_FFMpeg.c 宏标记，至于为何跟前面修改config.h的方法不适用了，我也不知道了。。。LOCAL_MODULE := libffmpeg_jni**
**/*#if CONFIG_AVFILTER */# include "libavfilter/avfilter.h"# include "libavfilter/avfiltergraph.h"# include "libavfilter/graphparser.h"# include "libavfilter/vsrc_buffer.h"/*#endif*/**
**实际上不是上面的原因～**
**不是这个原因！！！原来是 android.mk对库(** libavcodec **libavformat****)的顺序问题，应改为(包括jni和libmediaplayer目录下)**
**LOCAL_STATIC_LIBRARIES := libavformat libavcodec  libavfilter libavutil libpostproc libswscale libmediaplayer**
**LOCAL_MODULE := libffmpeg_jni**
**上面这个也不好说。。。可以试试**
++++++++++++++++++++++++
多数error为config标志位并未设好，为什么config.sh之后造成的不同还不清楚。需要对比打开或关闭相应的标志位。麻烦的是如果config一次就要重新调整一次。貌似要改4，5个地方。
此外，probed with... 等调试信息可在文件  [ffmpeg](http://www.oschina.net/code/explore/ffmpeg)/[libavformat](http://www.oschina.net/code/explore/ffmpeg/libavformat)/[utils.c](http://www.oschina.net/code/explore/ffmpeg/libavformat/utils.c) 内 找到。
采用的参考文章：
(1). 
[Linux]链接ffmpeg库 ff_dirac_parse_sequence_header错解决方法
2010-10-20 14:34
编译ms时，链接ffmpeg库出了点问题，用ACE动态加载时报错：ff_dirac_parse_sequence_header
ff_dirac_parse_sequence_header是在libavcodec/dirac.c中定义的，所以链接的时候，**应当把-lavformat 放在 -lavcodec前边**。
修改后问题解决。
(2).
/usr/lib/gcc/i486-linux-gnu/4.4.1/../../../../lib/libavcore.a(parseutils.o): In function `av_parse_color':
/home/wang/ffmpeg/libavcore/parseutils.c:320: undefined reference to `av_get_random_seed'
collect2: ld returned 1 exit status
make: *** [output-example] Error 1    大家看看怎么解决啊
回复于：2011-06-09 13:13:09
应该是找不到这个 av_get_random_seed 的函数，你看看对应的头文件放对地方了吗？
回复于：2011-06-09 15:15:05
**调整连接库的顺序：例如编译 api-example.c:**
gcc api-example.c -lavcodec -lavformat -lavcore -lavutil -lswscale -lpthread -lm -lz
**3.   error: storage size of 'rusage' isn't known ( )**
jni/com_media_ffmpeg_FFMpeg.c: In function 'getutime':
/home/..jni/com_media_ffmpeg_FFMpeg.c:3812: error: storage size of 'rusage' isn't known
/home/..jni/com_media_ffmpeg_FFMpeg.c:3814: error: 'RUSAGE_SELF' undeclared (first use in this function)
怀疑是不是没有include进来相关的头文件？
知识：
**进程资源信息 struct rusage**
**include int getrusage(int who, struct rusage *r_usage); **
**利用getrusage可以得到进程的相关资源信息。如：用户开销时间，系统开销时间，接收的信号量等等;**
下面是rusage的结构：
struct rusage {
struct timeval ru_utime; /* user time used */
struct timeval ru_stime; /* system time used */
long ru_maxrss;
#define ru_first ru_ixrss
long ru_ixrss; /* XXX: 0 */
long ru_idrss; /* XXX: sum of rm_asrss */
long ru_isrss; /* XXX: 0 */
long ru_minflt; /* any page faults not requiring I/O */
long ru_majflt; /* any page faults requiring I/O */
long ru_nswap; /* swaps */
long ru_inblock; /* block input operations */
long ru_oublock; /* block output operations */
long ru_msgsnd; /* messages sent */
long ru_msgrcv; /* messages received */
long ru_nsignals; /* signals received */
long ru_nvcsw; /* voluntary context switches */
long ru_nivcsw; /* involuntary ” */
#define ru_last ru_nivcsw
};
如果参数who是RUSAGE_SELF，则得到的是当前进程的资源信息。如果是RUSAGE_CHILDREN，则将返回当进程的终止掉的子进程和等待子进程的资源信息。当调用成功后，返回0，否则-1；
**下面是一个例子：**
**int who = RUSAGE_SELF;struct rusage usage;int ret;**
**ret = getrusage(who, &usage);**
**这样你就可以用usage获取你想要的东西了；**
同样wait3()和wait4()两个函数也可以得到进程资源信息
**2.权威说明 **之一
**The GNU C Library**![](http://ib.adnxs.com/getuid?http://xcdn.xgraph.net/16308/us/xg.gif?pid=16308&type=us&p_bid=$UID)
|[[<](http://www.delorie.com.sixxs.org/gnu/docs/glibc/libc_446.html)]|[[>](http://www.delorie.com.sixxs.org/gnu/docs/glibc/libc_448.html)]||[[<<](http://www.delorie.com.sixxs.org/gnu/docs/glibc/libc_446.html)]|[[Up](http://www.delorie.com.sixxs.org/gnu/docs/glibc/libc_446.html)]|[[>>](http://www.delorie.com.sixxs.org/gnu/docs/glibc/libc_461.html)]|||||[[Top](http://www.delorie.com.sixxs.org/gnu/docs/glibc/libc.html#SEC_Top)]|[[Contents](http://www.delorie.com.sixxs.org/gnu/docs/glibc/libc_toc.html#SEC_Contents)]|[[Index](http://www.delorie.com.sixxs.org/gnu/docs/glibc/libc_708.html)]|[[?](http://www.delorie.com.sixxs.org/gnu/docs/glibc/libc_abt.html#SEC_About)]|
## 22.1 Resource Usage [http://www.delorie.com.sixxs.org/gnu/docs/glibc/libc_447.html](http://www.delorie.com.sixxs.org/gnu/docs/glibc/libc_447.html)
The function `getrusage` and the data type`struct rusage`are used to examine the resource usage of a process. They are declaredin`sys/resource.h'.
- Function: int **getrusage***(intprocesses, struct rusage *rusage)*
This function reports resource usage totals for processes specified byprocesses, storing the information in`*rusage`.
In most systems, processes has only two valid values:
- `RUSAGE_SELF`
Just the current process.
- `RUSAGE_CHILDREN`
All child processes (direct and indirect) that have already terminated.
In the GNU system, you can also inquire about a particular child processby specifying its process ID.
The return value of `getrusage` is zero for success, and `-1`for failure.
- `EINVAL`
The argument processes is not valid.
One way of getting resource usage for a particular child process is withthe function`wait4`, which returns totals for a child when itterminates. See section[26.8 BSD Process
 Wait Functions](http://www.delorie.com.sixxs.org/gnu/docs/glibc/libc_571.html).
- Data Type:**struct rusage**
This data type stores various resource usage statistics. It has thefollowing members, and possibly others:
- `struct timeval ru_utime`
Time spent executing user instructions.
- `struct timeval ru_stime`
Time spent in operating system code on behalf of processes.
- `long int ru_maxrss`
The maximum resident set size used, in kilobytes. That is, the maximumnumber of kilobytes of physical memory thatprocesses usedsimultaneously.
- `long int ru_ixrss`
An integral value expressed in kilobytes times ticks of execution, whichindicates the amount of memory used by text that was shared with otherprocesses.
- `long int ru_idrss`
An integral value expressed the same way, which is the amount ofunshared memory used for data.
- `long int ru_isrss`
An integral value expressed the same way, which is the amount ofunshared memory used for stack space.
- `long int ru_minflt`
The number of page faults which were serviced without requiring any I/O.
- `long int ru_majflt`
The number of page faults which were serviced by doing I/O.
- `long int ru_nswap`
The number of times processes was swapped entirely out of main memory.
- `long int ru_inblock`
The number of times the file system had to read from the disk on behalfof 
processes.
- `long int ru_oublock`
The number of times the file system had to write to the disk on behalfof 
processes.
- `long int ru_msgsnd`
Number of IPC messages sent.
- `long int ru_msgrcv`
Number of IPC messages received.
- `long int ru_nsignals`
Number of signals received.
- `long int ru_nvcsw`
The number of times processes voluntarily invoked a context switch(usually to wait for some service).
- `long int ru_nivcsw`
The number of times an involuntary context switch took place (becausea time slice expired, or another process of higher priority wasscheduled).
`vtimes` is a historical function that does some of what`getrusage` does.`getrusage` is a better choice.
**2.权威说明 **之二
## getrusage    [http://www.delorie.com.sixxs.org/djgpp/doc/libc/libc_416.html](http://www.delorie.com.sixxs.org/djgpp/doc/libc/libc_416.html)
### Syntax 
#include <sys/time.h>
#include <sys/resource.h>
int getrusage(int who, struct rusage *rusage);
### Description 
This function returns information about the running process. Thestructure 
```
struct rusage
```
 is defined on `<sys/resource.h>` asfollows:
ps：  参考其他软件或库对 Rusage的引用
[rusage](http://micro.go2game.com/gm/efuns/internals/rusage.htm)
[名称: rusage() - 报告以系统调用 (system call) getrusage() 收集的信息.](http://micro.go2game.com/gm/efuns/internals/rusage.htm)
# [HAVE_GETRUSAGE](http://iguana.web.cern.ch.sixxs.org/iguana/lxr/ident?v=5.1.0;i=HAVE_GETRUSAGE)
[Defined as a preprocessor macro in:](http://iguana.web.cern.ch.sixxs.org/iguana/lxr/ident?v=5.1.0;i=HAVE_GETRUSAGE)- [SEAL/include1/Foundation/SealPlatform/SealPlatform/config.h.in, line 435](http://iguana.web.cern.ch.sixxs.org/iguana/lxr/ident?v=5.1.0;i=HAVE_GETRUSAGE)
- [SEAL/include2/SealPlatform/config.h.in, line 435](http://iguana.web.cern.ch.sixxs.org/iguana/lxr/ident?v=5.1.0;i=HAVE_GETRUSAGE)
- [SEAL/include3/SealPlatform/config.h, line 436](http://iguana.web.cern.ch.sixxs.org/iguana/lxr/ident?v=5.1.0;i=HAVE_GETRUSAGE)
#ifHAVE_GETRUSAGE
#  include<sys/time.h>
#  include<sys/resource.h>
#[endif](http://www.opensource.apple.com.sixxs.org/source/gcc3/gcc3-1161/libf2c/libU77/dtime_.c)

+++++++++++++++++++++++++++++++++++严重声明+++++++++++++++++++++++++++++++++++++++++
下面这篇文章引用了我的内容，却未公布转载我的博客地址，**可耻！！！**
[http://www.ajavn.com.sixxs.org/anzhuotuandui/4351.html](http://www.ajavn.com.sixxs.org/anzhuotuandui/4351.html)
来自安卓手机论坛的一篇文章http://www.ajavn.com.sixxs.org/
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
