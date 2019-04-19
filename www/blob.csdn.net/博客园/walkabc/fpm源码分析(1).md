# fpm源码分析(1) - walkabc - 博客园
# [fpm源码分析(1)](https://www.cnblogs.com/hitfire/articles/4574484.html)
　　源码分析一般都是从main函数开始的，但我还是觉得首先把程序关键的数据结构介绍下，这样看fpm源码会更容易理解。
　　废话不多说，直接上fpm中最关键的数据结构定义。
```
1     /* $Id: fpm.h,v 1.13 2008/05/24 17:38:47 anight Exp $ */
 2     /* (c) 2007,2008 Andrei Nigmatulin */
 3 
 4 #ifndef FPM_H
 5 #define FPM_H 1
 6 
 7 #include <unistd.h>
 8 
 9 #ifdef HAVE_SYSEXITS_H
10 #include <sysexits.h>
11 #endif
12 
13 #ifdef EX_OK
14 #define FPM_EXIT_OK EX_OK
15 #else
16 #define FPM_EXIT_OK 0
17 #endif
18 
19 #ifdef EX_USAGE
20 #define FPM_EXIT_USAGE EX_USAGE
21 #else
22 #define FPM_EXIT_USAGE 64
23 #endif
24 
25 #ifdef EX_SOFTWARE
26 #define FPM_EXIT_SOFTWARE EX_SOFTWARE
27 #else
28 #define FPM_EXIT_SOFTWARE 70
29 #endif
30 
31 #ifdef EX_CONFIG
32 #define FPM_EXIT_CONFIG EX_CONFIG
33 #else
34 #define FPM_EXIT_CONFIG 78
35 #endif
36 
37 
38 int fpm_run(int *max_requests);
39 int fpm_init(int argc, char **argv, char *config, char *prefix, char *pid, int test_conf, int run_as_root, int force_daemon, int force_stderr);
40 
41 struct fpm_globals_s {
42     pid_t parent_pid;
43     int argc;
44     char **argv;
45     char *config;
46     char *prefix;
47     char *pid;
48     int running_children;
49     int error_log_fd;
50     int log_level;
51     int listening_socket; /* for this child */
52     int max_requests; /* for this child */
53     int is_child;
54     int test_successful;
55     int heartbeat;
56     int run_as_root;
57     int force_stderr;
58     int send_config_pipe[2];
59 };
60 
61 extern struct fpm_globals_s fpm_globals;
62 
63 #endif
```
　　介绍下“fpm_globals”，从名字就可以看出来，这是fpm的全局定义，所有的代码都会使用到这个变量。
　　下面捡几个比较重要的结构体属性介绍下：
　　parent_pid 本进程如果是一个worker进程的话，parent_id是不为0的。保留parent_id是为了将来能和父进程进行沟通；当然也可以每次都通过getppid来获取parent_id，但是这样效率比较低，现在保存parent_id也是为了减少系统调用，提高程序整体性能。
　　config 表示的是fpm的配置文件，如果在启动程序时，不指定配置文件，程序将会自动查找这个配置文件。如一下代码：
```
if (fpm_globals.config == NULL) {
        char *tmp;
        if (fpm_globals.prefix == NULL) {
            spprintf(&tmp, 0, "%s/php-fpm.conf", PHP_SYSCONFDIR);
        } else {
            spprintf(&tmp, 0, "%s/etc/php-fpm.conf", fpm_globals.prefix);
        }
        if (!tmp) {
            zlog(ZLOG_SYSERROR, "spprintf() failed (tmp for fpm_globals.config)");
            return -1;
        }
        fpm_globals.config = strdup(tmp);
        efree(tmp);
        if (!fpm_globals.config) {
            zlog(ZLOG_SYSERROR, "spprintf() failed (fpm_globals.config)");
            return -1;
        }
    }
```
　　prefix这个php程序猿应该比较熟悉，这个是php在编译时设置的主目录。
　　pid这个文件就是/var/run/fpm.pid，至于为什么要保存这个属性，是为了防止程序的多启动，每次启动时都会锁定这个pid，锁定成功就执行，锁定不成功就退出，相当于程序在系统中单实例运行。

