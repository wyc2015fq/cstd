# 编写who命令--从Linux中学习Linux - Orisun - 博客园







# [编写who命令--从Linux中学习Linux](https://www.cnblogs.com/zhangchaoyang/articles/2293657.html)





执行who命令查看已登录的用户信息

$ who
orisun tty7 2011-12-19 14:12 (:0)
orisun pts/0 2011-12-19 14:12 (:0.0)

显示出了：用户名、终端名、登录时间、登录地址

现在我们要自己写一个程序来实现who。是不是要用到一些特殊的系统调用，或者需要root权限呢？实际上都不用，我们要学会使用Linux的联机帮助文档，从Linux中学习Linux。

要学习linux系统给我们提供的编程接口，很多时间查看相应的头文件就可以了。

$ man who

WHO(1) User Commands WHO(1)

NAME
 who - show who is logged on

SYNOPSIS
 who [OPTION]... [ FILE | ARG1 ARG2 ]

DESCRIPTION
 Print information about users who are currently logged in.

-a, --all
 same as -b -d --login -p -r -t -T -u

-b, --boot
 time of last system boot

-d, --dead

……

注意我红色标出的那个1是小节编号。使用who命令时可以指定FILE，在who的DESCRIPTION中有这么几行字：If FILE is not specified, use /var/run/utmp. /var/log/wtmp as FILE iscommon.实际上who是通过读取/var/run/utmp文件来获取已登录的用户信息的。

带有选项-k的man命令可以根据关键字来搜索联机帮助。

$ man -k utmp
endutent (3) - access utmp file entries
endutxent (3) - access utmp file entries
getutent (3) - access utmp file entries
getutent_r (3) - access utmp file entries
getutid (3) - access utmp file entries
getutid_r (3) - access utmp file entries
getutline (3) - access utmp file entries
getutline_r (3) - access utmp file entries
getutmp (3) - copy utmp structure to utmpx, and vice versa
getutmpx (3) - copy utmp structure to utmpx, and vice versa
getutxent (3) - access utmp file entries
getutxid (3) - access utmp file entries
getutxline (3) - access utmp file entries
login (3) - write utmp and wtmp entries
logout (3) - write utmp and wtmp entries
pututline (3) - access utmp file entries
pututxline (3) - access utmp file entries
sessreg (1) - manage utmp/wtmp entries for non-init clients
setutent (3) - access utmp file entries
setutxent (3) - access utmp file entries
utmp (5) - login records
utmpname (3) - access utmp file entries
utmpx (5) - login records
utmpxname (3) - access utmp file entries

这里的“5”是小节编号。

$ man 5 utmp　　

UTMP(5) Linux Programmer's Manual UTMP(5)　　

NAME

utmp, wtmp - login records

SYNOPSIS
 #include <utmp.h>

DESCRIPTION
 The utmp file allows one to discover information about who is currently using the system. There may be more users currently using the system, because not all programs use utmp logging.

……

/* Values for ut_type field, below */

#define EMPTY 0 /* Record does not contain valid info (formerly known as UT_UNKNOWN on Linux) */
 #define RUN_LVL 1 /* Change in system run-level (see init(8)) */
 #define BOOT_TIME 2 /* Time of system boot (in ut_tv) */
 #define NEW_TIME 3 /* Time after system clock change (in ut_tv) */
 #define OLD_TIME 4 /* Time before system clock change (in ut_tv) */
 #define INIT_PROCESS 5 /* Process spawned by init(8) */
 #define LOGIN_PROCESS 6 /* Session leader process for user login */
 #define USER_PROCESS 7 /* Normal process */
 #define DEAD_PROCESS 8 /* Terminated process */
 #define ACCOUNTING 9 /* Not implemented */

#define UT_LINESIZE 32
 #define UT_NAMESIZE 32
 #define UT_HOSTSIZE 256

struct exit_status { /* Type for ut_exit, below */
 short int e_termination; /* Process termination status */
 short int e_exit; /* Process exit status */
 };

struct utmp {
 short ut_type; /* Type of record */
 pid_t ut_pid; /* PID of login process */
 char ut_line[UT_LINESIZE]; /* Device name of tty - "/dev/" */
 char ut_id[4]; /* Terminal name suffix, or inittab(5) ID */
 char ut_user[UT_NAMESIZE]; /* Username */
 char ut_host[UT_HOSTSIZE]; /* Hostname for remote login, or kernel version for run-level messages */
 struct exit_status ut_exit; /* Exit status of a process marked as DEAD_PROCESS; not used by Linux init(8) */
 /* The ut_session and ut_tv fields must be the same size when compiled 32- and 64-bit. This allows data files and shared memory to be shared between 32- and 64-bit applications. */
 #if __WORDSIZE == 64 && defined __WORDSIZE_COMPAT32
 int32_t ut_session; /* Session ID (getsid(2)), used for windowing */

struct {
 int32_t tv_sec; /* Seconds */
 int32_t tv_usec; /* Microseconds */
 } ut_tv; /* Time entry was made */
 #else
 long ut_session; /* Session ID */
 struct timeval ut_tv; /* Time entry was made */
 #endif

int32_t ut_addr_v6[4]; /* Internet address of remote host; IPv4 address uses just ut_addr_v6[0] */
 char __unused[20]; /* Reserved for future use */
 };

/* Backwards compatibility hacks */
 #define ut_name ut_user
 #ifndef _NO_UT_TIME
 #define ut_time ut_tv.tv_sec
 #endif
 #define ut_xtime ut_tv.tv_sec
 #define ut_addr ut_addr_v6[0]

……

```
#include<stdio.h>
#include<utmp.h>
#include<string.h>
#include<stdlib.h>

#define SHOWHOST

void show_info(struct utmp*);

int main(){
    struct utmp record;
    FILE *fp;
    int strulen=sizeof(record);
    if((fp=fopen("/var/run/utmp","r"))==0){     //打开文件
        perror("fopen");
        exit(0);
    }
    memset(&record,0x00,strulen);
    while(fread(&record,strulen,1,fp)){      //循环读取记录（结构体）
        show_info(&record);
    }
    fclose(fp);     //关闭文件
    return 0;
}
void show_info(struct utmp *buf){
    printf("%-8s  ",buf->ut_name);      //输出用户名
    printf("%-8s  ",buf->ut_line);      //终端设备名
    printf("%10ld  ",buf->ut_time);      //登录时间
#ifdef SHOWHOST
    printf("(%s)",buf->ut_host);
#endif
    printf("\n");
}
```


运行结果：

$ ./who1
reboot ~ 1324275122 (2.6.38-13-generic)
runlevel ~ 1324275122 (2.6.38-13-generic)
LOGIN tty4 1324275122 ()
LOGIN tty5 1324275122 ()
LOGIN tty2 1324275122 ()
LOGIN tty3 1324275122 ()
LOGIN tty6 1324275122 ()
LOGIN tty1 1324275128 ()
orisun tty7 1324275138 (:0)
orisun pts/0 1324275175 (:0.0)
orisun pts/1 1324285820 (:0.0)

上面的程序有两个问题：
- 时间显示的是time_t类型的，不便于阅读
- utmp文件不仅包含活动用户的信息，连那些尚未用到的终端信息也保存在utmp文件。

解决问题1。让我们找一下和时间格式转换相关的函数。

$ man -k time | grep -i transform
asctime (3) - transform date and time to broken-down time or ASCII
asctime_r (3) - transform date and time to broken-down time or ASCII
ctime (3) - transform date and time to broken-down time or ASCII
ctime_r (3) - transform date and time to broken-down time or ASCII
gmtime (3) - transform date and time to broken-down time or ASCII
gmtime_r (3) - transform date and time to broken-down time or ASCII
localtime (3) - transform date and time to broken-down time or ASCII
localtime_r (3) - transform date and time to broken-down time or ASCII
mktime (3) - transform date and time to broken-down time or ASCII

好，我们仔细阅读一下localtime

$ man localtime

CTIME(3) Linux Programmer's Manual CTIME(3)

NAME
 asctime, ctime, gmtime, localtime, mktime, asctime_r, ctime_r, gmtime_r, localtime_r - transform date and time to
 broken-down time or ASCII

SYNOPSIS
 #include <time.h>

char *asctime(const struct tm *tm);
 char *asctime_r(const struct tm *tm, char *buf);

char *ctime(const time_t *timep);
 char *ctime_r(const time_t *timep, char *buf);

struct tm *gmtime(const time_t *timep);
 struct tm *gmtime_r(const time_t *timep, struct tm *result);

struct tm *localtime(const time_t *timep);
 struct tm *localtime_r(const time_t *timep, struct tm *result);

time_t mktime(struct tm *tm);

Feature Test Macro Requirements for glibc (see feature_test_macros(7)):

asctime_r(), ctime_r(), gmtime_r(), localtime_r():
 _POSIX_C_SOURCE >= 1 || _XOPEN_SOURCE || _BSD_SOURCE || _SVID_SOURCE || _POSIX_SOURCE

DESCRIPTION
 The ctime(), gmtime() and localtime() functions all take an argument of data type time_t which represents calendar
 time. When interpreted as an absolute time value, it represents the number of seconds elapsed since the Epoch,
 1970-01-01 00:00:00 +0000 (UTC).

The asctime() and mktime() functions both take an argument representing broken-down time which is a representation
 separated into year, month, day, etc.

Broken-down time is stored in the structure tm which is defined in <time.h> as follows:

struct tm {
 int tm_sec; /* seconds */
 int tm_min; /* minutes */
 int tm_hour; /* hours */
 int tm_mday; /* day of the month */
 int tm_mon; /* month */
 int tm_year; /* year */
 int tm_wday; /* day of the week */
 int tm_yday; /* day in the year */

解决问题2。通过阅读utmp.h我们发现，utmp结构中有一个成员ut_type，当它的值为7（USER_PROCESS）时，表示这个用户已经登录。

综上所述，我们给出改进后的代码。

#include<stdio.h>

#include<utmp.h>

#include<string.h>

#include<stdlib.h>

#include<time.h>




#define SHOWHOST



void show_info(struct utmp*);



int main(){

struct utmp record;

FILE *fp;

int strulen=sizeof(record);

if((fp=fopen("/var/run/utmp","r"))==0){ //打开文件

perror("fopen");

exit(0);

}

memset(&record,0x00,strulen);

while(fread(&record,strulen,1,fp)){ //循环读取记录（结构体）

show_info(&record);

}

fclose(fp); //关闭文件

return 0;

}

void show_info(struct utmp *buf){

if(buf->ut_type!=USER_PROCESS)
	//非登录用户

	return;


printf("%-8s ",buf->ut_name); //输出用户名

printf("%-12s ",buf->ut_line); //终端设备名

time_t timelong=buf->ut_time;

struct tm
*localnow=localtime(&timelong);


   
printf("%d-%d-%d %d:%d
",localnow->tm_year+1990,localnow->tm_mon+1,localnow->tm_mday,localnow->tm_hour,localnow->tm_min); //登录时间

#ifdef SHOWHOST

printf("(%s)",buf->ut_host);

#endif

printf("\n");

}

![](https://pic002.cnblogs.com/images/2011/103496/2011121918304063.png)












