# 《Linux程序设计》——Linux环境 - 一世豁然的专栏 - CSDN博客





2014年09月21日 20:40:33[一世豁然](https://me.csdn.net/Explorer_day)阅读数：769








一、程序参数

 1、main函数参数

   1）、**int main(int arc, char *argv[ ]);**

       注： 其中argc是程序参数的个数，argv是一个代表参数自身的字符串数组。此函数一般是由另外的程序提供，这个程序一般是shell。

                Linux的shell一般会在设置argc和argv之前对文件参数进行通配符扩展，而MS-DOS的shell则期望程序接受带通配符的参数，并执行他们自己的通配符扩展。

   2）、main（）

       注：默认的返回值是int，而且函数中不用的形式参数不需要声明，argc和argv还在，如果不声明它们，就不能使用它们。

   3）、在应用程序中，所有的命令行开关都应以一个短横线开头，其后包含单个字母或数字。如果需要，不带后续参数的选项可以在一个短横线后归并到一起。如果某个选项需要 值，则该值应作为独立的参数紧跟在该选项后。




 2、getopt函数

   1）、**#include<unistd.h>**

**             int getopt(int argc, char *const  argv[ ],  const  char  *optstring);**

**             extern  char  *optarg;**

**             extern   int  optind,  opterr,  optopt;**

   2）、getopt函数将传递给程序的main函数的argc和argv作为参数，同时接受一个选项指定符字符串optstring，该字符串告诉getopt那些选项可用，以及他们是否有关联值。

   3）、optstring只是一个字符列表，每个列表代表一个单子符选项。如果一个字符后面紧跟一个冒号（：），则表明该该选项有一个关联值作为下一个参数。

   4）、getopt额返回值是argv数组中的下一个选项字符（如果有的话）。循环调用getopt就可以一次等到每个选项。

   5）、getopt的行为

           I、如果选项有一个关联值，则外部变量optarg指向这个值。

          II、如果选项处理完毕，getopt返回-1，特殊参数--将使getopt停止扫描选项。

         III、如果遇到一个无法识别的选项，getopt返回一个问号（？），并把它保存在外部变量optopt中。

         IV、如果一个选项要求有一个关联值，但用户并未提供这些值，getopt通常将返回一个问号（？）。如果我们将选项字符串的第一个字符设置为冒号（：），那么getopt将 


               在用户未提供值得情况下返回冒号而不是问号。

   6）、外部变量optind被设置为下一个待处理参数的索引。getopt利用它来记录自己的进度。有些getopt版本会在遇到未知选项时打印出错信息。




 3、getopt_long函数

   1）、getopt_long比getopt多两个参数。第一个附加选项是一个结构数组，它描述了每个长选项并告诉getopt_long如何处理它们。第二个附加参数是一个变量指针，它可以作

             为option的长选项版本使用。

   2）、struct  option结构成员（P120  表4-1）

            struct  option {

            const char *name;

            int has_arg;

            int *flag;

            int val;

         }；







二、环境变量

 1、通过getenv和putenv访问环境变量

**#include<stdlib.h>**

**    char  *getenv(const  char  *name);**

**    int  putenv(const  char  *string);**




 2、环境变量的用途

   1）、程序经常使用环境变量来改变它们的工作方式。

   2）、用户可以通过以下方式设置环境变量的值。

      I、在默认环境中设置。

      II、通过登录shell读取.profile文件来设置。

     III、使用shell专用的启动文件（rc）或在shell命令行上对环境进行设定。

   3）、环境变量就像全局变量一样，它们会改变程序的行为，产生不可预知的结果。




 3、environ变量

    #include<stdilb.h>

    extern  char  **environ；


   1）、程序可以通过**environ**变量直接访问程序环境由格式为”名字=值“的字符串。







三、时间和日期

 1、time函数

**#include<time.h>**

**    time_t   time(time_t *tloc);**


   1）、time函数可以得到底层的时间值，它返回的是从纪元开始至今的秒数。如果tloc不是一个空指针，time函数还会把返回值写入tloc指针指向额位置。




 2、difftime函数

**  #include<time.h>**

**    double  difftime(time_t  time1, time_t  time2);    **

    1）、计算两个time_t值之间的秒数并以double类型返回它。




 3、gmtime函数

**    #include<time.h>**

**   struct  tm  *gmtime(const  time_t  *timeval);**

   1）、gmtime函数把底层时间值分解为一个结构，这结构有供人类阅读的时间和日期格式。

    2）、tm成员（P125  表4-2）




 4、localtime函数

**   #include<time.h>**

**   struct  tm  *localtime(const  time_t  *timeval);**

   1）、localtime函数可以看当地时间。




 5、mktime函数

** #inlucde<time.h>**

**    time_t  mktime(struct  tm  *timeptr);**

    1）、把tm结构转换为岩原始的time_t时间值。




 6、asctime函数和ctime函数

**    #include<time.h>**

**   char  *asctime(const  tm  *timeptr);**

**   char  *ctime(const  time_t  *timeval);**

   1）、利用这两个函数可以得到更好的时间显示。

   2）、asctime函数返回一个字符串，它表示由tm结构timeptr所给出的时间和日期。




 7、strftime函数

**    #include<time.h>**

**   size_t  strftime(char  *s, size_t  maxsize,  const  char  *format,  struct tm *timeptr);**

   1）、此函数可以对时间和日期有更多的控制。

   2）、strftime函数格式化timeptr指针指向的tm结构所表示的时间和日期，并将结果放在字符串s中。字符串被指定（至少）maxsize个字符长。format字符串用于控制写入字

             符串的字符。

   3）、转换控制符（P127  表4-3）




 8、strptime函数

** #include<time.h>**

**    char  *strptime(const  char  *buf,  const  char  *format,  struct  tm  *timeptr);**

  1）、该函数以一个代表日期和时间的字符串为参数，并创建表示同一日期和时间的tm结构。

2）、该函数返回一个指针，指向转换过程处理的最后一个字符后面的那个字符。如果遇到不能转换的字符，转换过程就在该处停下来。







四、临时文件

 1、tmpnam函数

**   #include<stdio.h>**

**    char  *tmpnam(char  *s);**

    1）、此函数可以生成惟一的文件名，它的返回值是一个不与任何已存在文件同名的有效文件名。如果字符串不为空，文件名也会写入它。

     2）、  tmpnam函数可以被一个程序最多调用TMP_MAX次，每次都会返回不同的文件。




 2、tmpfile函数

** #include<stdio.h>**

**    FILE  *tmpfile(void);**

   1）、临时文件需要立即使用，可以用此函数，它可以同时创建并打开文件。

   2）、tmpfile函数返回一个文件流指针，它指向一个惟一的临时文件。该文件以读写方式打开（通过w+方式的fopen），当对它的所有引用全部关闭时，该文件会被自动删

             除。如果出错，该函数返回空指针并设置errno额值。




 3、mktemp函数和mkxstemp函数

**  #include<stdlib.h>**

**    char  *mktemp(char  *template);**

**    int  mkstemp(char  *template);**

    1）、mktemp函数以给定的模板为基础创造一个唯一的文件名。它返回一个指向生成字符串的指针，如果不能生成唯一的名字，它就返回一个空指针。

    2）、mkstemp函数同时创建并打开一个临时文件，文件名的生成方法和mktemp一样，但是它的返回值是一个打开的、底层的文件描述符。







五、用户信息

 1、getuid和getlogin函数

** #include<sys/types.h>**

**     #include<unistd.h> **

**     uid_t  getuid(void);**

**     char *getlogin(void); **

     1）、getuid函数返回程序关联的UID，它通常是启动程序的用户的UID。

     2）、getlogin函数返回与当前用户关联的登陆名。




 2、getpwuid和getpwnam函数

    #include<sys/types.h>

    #include<pwd.h>

    struct  passwd  *getpwuid(uid_t  uid);

    struct  passwd  *getpwnam(const  char  *name);

    1）、getpwuid和getpwnam函数都返回一个指针，该指针指向与某个用户对应的passwd结构。这个用户通过getpwuid的UID参数或通过getpwnam的用户名参数来确定。出


             错时，都返回空指针并设置errno。

    2）、passwd结构成员（P134  表4-4）




 3、endpwent、getpwent和setpwent函数

    #include<sys/types.h>

    #include<pwd.h>

    void  endpwent(uid_t  uid);

    struct  passwd  *getpwent(void);

    void  setpwent(void)；

    1）、getpwent函数依次返回每个用户的信息数据项。但到达文件尾时，它返回一个空指针。如果已经扫描了足够多的数据项，可以使用endpwent函数来终止处理过程。

    2）、setpwent函数重置读指针到密码文件的开始位置。




 4、相关函数

**#include<sys/types.h>**

**    #include<unistd.h>**
**    uid_t  geteuid(void);**
**    gid_t  getgid(void);**

**    gid_t  getegid(void);**

**    int  setuid(uid_t  uid);**

**    int  setgid(gid_t  gid);**







六、主机信息

 1、gethostname函数

**  #include<unistd.h>**

**     int  gethostname(char  *name, size_t  namelen);**

     1）、gethostname函数把机器的网络名写入name字符串。该字符串至少有namelen个字符长。成功时，gethostbyname返回0，否则返回-1。




 2、uname函数

**#include<sys/utsname.h>**

**     int  uname(struct  utsname  *name);**

     1）、uname函数把主机信息写入name参数指向的结构。

     3）、utsname成员（P134  表4-5）

     3）、uname在成功时返回一个负整数，否则返回-1并设置errno指出错误。




 3、gethostid函数

**#include<unistd.h>**

**    long  gethostid(void);**

   1）、此函数可以获得主机的惟一标识符。







七、日志

 1、syslog函数  

**     #include<syslog.h>**

**     void  syslog(int  priority,  const  char  *message,  arguments...);**

    1）、syslog函数向系统的日志设施发送一条日志信息。每条信息都有一个priority参数。该参数是一个严重级别与一个设施值的按位或。严重级别控制日志信息的处理方式，

              设施值记录日志信息的来源。

     2）、设施值包括LOG_USER（默认值）——它指出消息来自一个用户应用程序，以及LOG_LOCAL0、LOG_LOCAL1知道LOG_LOCAL7，他们的含义由本地管理员确定。

     3）、严重级别按优先级递减排列（P136  表4-6）。




 2、closelog、openlog和setogmask函数

** #include<syslog.h>**

**    void  closelog(void);**

**    void  openlog(const  char  *ident,  int  logopt,  int  facility);**

**    int  setlogmask(int  maskpri);**

    1）、可以通过调用openlog函数来改变日志信息的表达方式。它可以设置一个字符串ident，该字符串会添加在日志信息的前面。


     2）、facility调用默认设置值。logopt参数（P137  表4-7）。

     3）、closelog函数可以关闭openlog函数打开的文件描述符。

     4）、可以利用setlogmask函数来设置一个日志掩码，他的作用是创建一个只包含一个优先级的掩码。




 3、getpid和getppid函数

**  #include<sys/types.h>**

**    #include<unistd.h>**

**    pid_t  getpid(void);**

**    pid_t  getppid(void);**

    1）、两个函数分别返回调用进程和调用进程的父进程的进程标识符。







八、资源和限制

 1、资源限制：硬件方面的物理性限制、系统策略的限制、具体实现的限制。

 2、头文件limits.h中定义了许多代表操作系统方面的显性限制。（P138  表4-8）

 3、函数：头文件sys/resource.h提供了资源操作方面的定义，包括对程序的长度、执行优先级和文件资源等方面限制进行查询和设置额函数。

**  #include<sys/types.h>**

**    int  getpriority(int  which,  i_t  who);**

**    int  setpriority(int  which,  i_t  who, int  priority);**

**    int  getrlimit(int  resource,  struct  rlimit  *r_limit);**

**    int  setrlimit(int  resource,  const  struct  rlimit  *r_limit);**

**    int  getrusage(int  who,  struct  rusage  *r_usage);**

    1）、getrusage函数将CPU时间信息写入参数r_usage指向的rusage结构中。who参数（P140  表4-10），rusage结构（P1139  表4-9）。

     2）、应用程序可以用getpriority和setpriority函数确定和更改自身和其他程序的优先级。which参数（P140  表4-11）

     3）、系统资源方面的限制可以通过getrlimit和setrlimit来读取和设置。rlimit参数（P140  表4-12），resource参数（P141  表4-13）。




