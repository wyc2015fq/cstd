# Linux 下控制蜂鸣器发声的程序 - xqhrs232的专栏 - CSDN博客
2017年02月09日 16:01:19[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1900
原文地址::[http://blog.csdn.net/midion9/article/details/49862991](http://blog.csdn.net/midion9/article/details/49862991)
```
```cpp
/*===========================================
```
```cpp
*《Linux声音设备编程实例》
```
```cpp
* 1. 对内部扬声器编程
```
```cpp
*　　内部扬声器是控制台的一部分，所以它对应的设备文件为/dev/console。
```
```cpp
*    变量KIOCSOUND在头文件 /usr /include /linux /kd.h中声明，
```
```cpp
*    ioctl函数使用它可以来控制扬声器的发声，使用规则为：
```
```cpp
*　　    ioctl ( fd, KIOCSOUND, (int) tone);
```
```cpp
*　　    fd为文件设备号，tone 是音频值。当tone为0时，终止发声。
```
```cpp
*    必须一提的是它所理解的音频和我们平常以为的音频是不同的，
```
```cpp
*    由于计算机主板定时器的时钟频率为1.19MHZ，所以要进行正确的发声，
```
```cpp
*    必须进行如下的转换：
```
```cpp
*　　扬声器音频值=1190000/我们期望的音频值。
```
```cpp
*　　扬声器发声时间的长短我们通过函数usleep（unsigned long usec）来控制。
```
```cpp
*    它是在头文件/usr /include /unistd.h中定义的，让程序睡眠usec微秒。
```
```cpp
*    下面即是让扬声器按指定的长度和音频发声的程序的完整清单：
```
```cpp
*========================================================*/
```
```cpp
#include <fcntl.h>
```
```cpp
#include <stdio.h>
```
```cpp
#include <stdlib.h>
```
```cpp
#include <string.h>
```
```cpp
#include <unistd.h>
```
```cpp
#include <sys/ioctl.h>
```
```cpp
#include <sys/types.h>
```
```cpp
#include <linux/kd.h>
```
```cpp
/* 设定默认值 */
```
```cpp
#define DEFAULT_FREQ 440   /* 设定一个合适的频率 */
```
```cpp
#define DEFAULT_LENGTH 200 /* 200 微秒，发声的长度是以微秒为单位的*/
```
```cpp
#define DEFAULT_REPS 1     /* 默认不重复发声 */
```
```cpp
#define DEFAULT_DELAY 100  /* 同样以微秒为单位*/
```
```cpp
/* 定义一个结构，存储所需的数据*/
```
```cpp
typedef
```
```cpp
struct
```
```cpp
{
```
```cpp
```
```cpp
int
```
```cpp
freq;
```
```cpp
/* 我们期望输出的频率，单位为Hz */
```
```cpp
```
```cpp
int
```
```cpp
length;
```
```cpp
/* 发声长度，以微秒为单位*/
```
```cpp
```
```cpp
int
```
```cpp
reps;
```
```cpp
/* 重复的次数*/
```
```cpp
```
```cpp
int
```
```cpp
delay;
```
```cpp
/* 两次发声间隔，以微秒为单位*/
```
```cpp
} beep_parms_t;
```
```cpp
/* 打印帮助信息并退出*/
```
```cpp
void
```
```cpp
usage_bail (
```
```cpp
const
```
```cpp
char
```
```cpp
*executable_name ) {
```
```cpp
```
```cpp
printf
```
```cpp
(
```
```cpp
"Usage: \n \t%s [-f frequency] [-l length] [-r reps] [-d delay] \n "
```
```cpp
,
```
```cpp
```
```cpp
executable_name );
```
```cpp
```
```cpp
exit
```
```cpp
(1);
```
```cpp
}
```
```cpp
/*===================================
```
```cpp
* 分析运行参数，各项意义如下：
```
```cpp
* "-f <以HZ为单位的频率值 >"
```
```cpp
* "-l <以毫秒为单位的发声时长 >"
```
```cpp
* "-r <重复次数 >"
```
```cpp
* "-d <以毫秒为单位的间歇时长 >"
```
```cpp
* =====================================*/
```
```cpp
void
```
```cpp
parse_command_line(
```
```cpp
char
```
```cpp
**argv, beep_parms_t *result) {
```
```cpp
```
```cpp
char
```
```cpp
*arg0 = *(argv++);
```
```cpp
```
```cpp
while
```
```cpp
( *argv ) {
```
```cpp
```
```cpp
if
```
```cpp
( !
```
```cpp
strcmp
```
```cpp
( *argv,
```
```cpp
"-f"
```
```cpp
))
 {
```
```cpp
```
```cpp
/*频率*/
```
```cpp
```
```cpp
int
```
```cpp
freq =
```
```cpp
atoi
```
```cpp
( *( ++argv ) );
```
```cpp
```
```cpp
if
```
```cpp
( ( freq <= 0 ) || ( freq > 10000 ) ) {
```
```cpp
```
```cpp
fprintf
```
```cpp
( stderr,
```
```cpp
"Bad parameter: frequency must be from 1..10000\n"
```
```cpp
);
```
```cpp
```
```cpp
exit
```
```cpp
(1) ;
```
```cpp
```
```cpp
}
```
```cpp
else
```
```cpp
{
```
```cpp
```
```cpp
result->freq = freq;
```
```cpp
```
```cpp
argv++;
```
```cpp
```
```cpp
}
```
```cpp
```
```cpp
}
```
```cpp
else
```
```cpp
if
```
```cpp
( !
```
```cpp
strcmp
```
```cpp
(
 *argv,
```
```cpp
"-l"
```
```cpp
) ) {
```
```cpp
```
```cpp
/*时长*/
```
```cpp
```
```cpp
int
```
```cpp
length =
```
```cpp
atoi
```
```cpp
( *(++argv ) );
```
```cpp
```
```cpp
if
```
```cpp
(length < 0) {
```
```cpp
```
```cpp
fprintf
```
```cpp
(stderr,
```
```cpp
"Bad parameter: length must be >= 0\n"
```
```cpp
);
```
```cpp
```
```cpp
exit
```
```cpp
(1);
```
```cpp
```
```cpp
}
```
```cpp
else
```
```cpp
{
```
```cpp
```
```cpp
result->length = length;
```
```cpp
```
```cpp
argv++;
```
```cpp
```
```cpp
}
```
```cpp
```
```cpp
}
```
```cpp
else
```
```cpp
if
```
```cpp
(!
```
```cpp
strcmp
```
```cpp
(*argv,
```
```cpp
"-r"
```
```cpp
))
 {
```
```cpp
```
```cpp
/*重复次数*/
```
```cpp
```
```cpp
int
```
```cpp
reps =
```
```cpp
atoi
```
```cpp
(*(++argv));
```
```cpp
```
```cpp
if
```
```cpp
(reps < 0) {
```
```cpp
```
```cpp
fprintf
```
```cpp
(stderr,
```
```cpp
"Bad parameter: reps must be >= 0\n"
```
```cpp
);
```
```cpp
```
```cpp
exit
```
```cpp
(1);
```
```cpp
```
```cpp
}
```
```cpp
else
```
```cpp
{
```
```cpp
```
```cpp
result->reps = reps;
```
```cpp
```
```cpp
argv++;
```
```cpp
```
```cpp
}
```
```cpp
```
```cpp
}
```
```cpp
else
```
```cpp
if
```
```cpp
(!
```
```cpp
strcmp
```
```cpp
(*argv,
```
```cpp
"-d"
```
```cpp
))
 {
```
```cpp
```
```cpp
/* 延时 */
```
```cpp
```
```cpp
int
```
```cpp
delay =
```
```cpp
atoi
```
```cpp
(*(++argv));
```
```cpp
```
```cpp
if
```
```cpp
(delay < 0) {
```
```cpp
```
```cpp
fprintf
```
```cpp
(stderr,
```
```cpp
"Bad parameter: delay must be >= 0\n"
```
```cpp
);
```
```cpp
```
```cpp
exit
```
```cpp
(1);
```
```cpp
```
```cpp
}
```
```cpp
else
```
```cpp
{
```
```cpp
```
```cpp
result->delay = delay;
```
```cpp
```
```cpp
argv++;
```
```cpp
```
```cpp
}
```
```cpp
```
```cpp
}
```
```cpp
else
```
```cpp
{
```
```cpp
```
```cpp
fprintf
```
```cpp
(stderr,
```
```cpp
"Bad parameter: %s\n"
```
```cpp
, *argv);
```
```cpp
```
```cpp
usage_bail(arg0);
```
```cpp
```
```cpp
}
```
```cpp
```
```cpp
}
```
```cpp
}
```
```cpp
int
```
```cpp
main(
```
```cpp
int
```
```cpp
argc,
```
```cpp
char
```
```cpp
**argv)
 {
```
```cpp
```
```cpp
int
```
```cpp
console_fd;
```
```cpp
```
```cpp
int
```
```cpp
i;
```
```cpp
/* 循环计数器 */
```
```cpp
```
```cpp
/* 设发声参数为默认值*/
```
```cpp
```
```cpp
beep_parms_t parms = {
```
```cpp
DEFAULT_FREQ, DEFAULT_LENGTH, DEFAULT_REPS,
```
```cpp
```
```cpp
DEFAULT_DELAY
```
```cpp
};
```
```cpp
```
```cpp
/* 分析参数，可能的话更新发声参数*/
```
```cpp
```
```cpp
parse_command_line(argv, &parms);
```
```cpp
```
```cpp
/* 打开控制台，失败则结束程序*/
```
```cpp
```
```cpp
if
```
```cpp
( ( console_fd = open (
```
```cpp
"/dev/console"
```
```cpp
, O_WRONLY )
 ) == -1 ) {
```
```cpp
```
```cpp
fprintf
```
```cpp
(stderr,
```
```cpp
"Failed to open console.\n"
```
```cpp
);
```
```cpp
```
```cpp
perror
```
```cpp
(
```
```cpp
"open"
```
```cpp
);
```
```cpp
```
```cpp
exit
```
```cpp
(1);
```
```cpp
```
```cpp
}
```
```cpp
```
```cpp
/* 真正开始让扬声器发声*/
```
```cpp
```
```cpp
for
```
```cpp
(i = 0; i < parms.reps; i++) {
```
```cpp
```
```cpp
/* 数字1190000从何而来，不得而知*/
```
```cpp
```
```cpp
int
```
```cpp
magical_fairy_number = 1190000/parms.freq;
```
```cpp
```
```cpp
ioctl(console_fd, KIOCSOUND, magical_fairy_number);
```
```cpp
/* 开始发声 */
```
```cpp
```
```cpp
usleep(1000*parms.length);
```
```cpp
/*等待... */
```
```cpp
```
```cpp
ioctl(console_fd, KIOCSOUND, 0);
```
```cpp
/* 停止发声*/
```
```cpp
```
```cpp
usleep(1000*parms.delay);
```
```cpp
/* 等待... */
```
```cpp
```
```cpp
}
```
```cpp
/* 重复播放*/
```
```cpp
```
```cpp
return
```
```cpp
EXIT_SUCCESS;
```
```cpp
}
```
```
[](http://blog.csdn.net/midion9/article/details/49862991#)[](http://blog.csdn.net/midion9/article/details/49862991#)[](http://blog.csdn.net/midion9/article/details/49862991#)[](http://blog.csdn.net/midion9/article/details/49862991#)[](http://blog.csdn.net/midion9/article/details/49862991#)[](http://blog.csdn.net/midion9/article/details/49862991#)
