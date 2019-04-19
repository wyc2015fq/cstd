# linux进程函数 - youfangyuan - CSDN博客
2012年05月11日 23:50:20[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：357标签：[linux																[path																[file																[function																[数据结构																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=path&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[linux编程](https://blog.csdn.net/youfangyuan/article/category/1144942)
linux进程函数大全
atexit（设置程序正常结束前调用的函数）
相关函数 _exit,exit,on_exit
表头文件 #include<stdlib.h>
定义函数 int atexit (void (*function)(void));
函数说明 atexit()用来设置一个程序正常结束前调用的函数。当程序通过调用exit()
或从main中返回时，参数function所指定的函数会先被调用，然后才真正由exit()结束
程序。
返回值如果执行成功则返回0，否则返回-1，失败原因存于errno中。
范例 #include<stdlib.h>
void my_exit(void)
{
printf(“before exit () !\n”);
}
main()
{
atexit (my_exit);
exit(0);
}
执行 before exit()!
execl（执行文件）
相关函数 fork，execle，execlp，execv，execve，execvp
表头文件 #include<unistd.h>
定义函数 int execl(const char * path,const char * arg,....);
函数说明 execl()用来执行参数path字符串所代表的文件路径，接下来的参数代表执行
该文件时传递过去的argv(0)、argv[1]……，最后一个参数必须用空指针(NULL)作结
束。
返回值如果执行成功则函数不会返回，执行失败则直接返回-1，失败原因存于errno
中。
范例 #include<unistd.h>
main()
{
execl(“/bin/ls”,”ls”,”-al”,”/etc/passwd”,(char * )0);
}
执行 /*执行/bin/ls -al /etc/passwd */
-rw-r--r-- 1 root root 705 Sep 3 13 :52 /etc/passwd
execlp（从PATH环境变量中查找文件并执行）
相关函数 fork，execl，execle，execv，execve，execvp
表头文件 #include<unistd.h>
定义函数 int execlp(const char * file,const char * arg,……)；
函数说明 execlp()会从PATH环境变量所指的目录中查找符合参数file的文件名，找到
后便执行该文件，然后将第二个以后的参数当做该文件的argv[0]、argv[1]……，最后
一个参数必须用空指针(NULL)作结束。
返回值如果执行成功则函数不会返回，执行失败则直接返回-1，失败原因存于errno
中。
错误代码参考execve()。
范例 /* 执行ls -al /etc/passwd execlp()会依PATH变量中的/bin找到/bin/ls
 */
#include<unistd.h>
main()
{
execlp(“ls”,”ls”,”-al”,”/etc/passwd”,(char *)0);
}
执行 -rw-r--r-- 1 root root 705 Sep 3 13 :52 /etc/passwd
execv（执行文件）
相关函数 fork，execl，execle，execlp，execve，execvp
表头文件 #include<unistd.h>
定义函数 int execv (const char * path, char * const argv[ ]);
函数说明 execv()用来执行参数path字符串所代表的文件路径，与execl()不同的地方
在于execve()只需两个参数，第二个参数利用数组指针来传递给执行文件。
返回值如果执行成功则函数不会返回，执行失败则直接返回-1，失败原因存于errno
中。
错误代码请参考execve（）。
范例 /* 执行/bin/ls -al /etc/passwd */
#include<unistd.h>
main()
{
char * argv[ ]={“ls”,”-al”,”/etc/passwd”,(char*) }};
execv(“/bin/ls”,argv);
}
执行 -rw-r--r-- 1 root root 705 Sep 3 13 :52 /etc/passwd
execve（执行文件）
相关函数 fork，execl，execle，execlp，execv，execvp
表头文件 #include<unistd.h>
定义函数 int execve(const char * filename,char * const argv[ ],char * const
envp[ ]);
函数说明 execve()用来执行参数filename字符串所代表的文件路径，第二个参数系利
用数组指针来传递给执行文件，最后一个参数则为传递给执行文件的新环境变量数组。
返回值如果执行成功则函数不会返回，执行失败则直接返回-1，失败原因存于errno
中。
错误代码 EACCES
1. 欲执行的文件不具有用户可执行的权限。
2. 欲执行的文件所属的文件系统是以noexec方式挂上。
3.欲执行的文件或script翻译器非一般文件。
EPERM
1.进程处于被追踪模式，执行者并不具有root权限，欲执行的文件具有SUID或SGID
位。
2.欲执行的文件所属的文件系统是以nosuid方式挂上，欲执行的文件具有SUID或SGID
位元，但执行者并不具有root权限。
E2BIG 参数数组过大
ENOEXEC 无法判断欲执行文件的执行文件格式，有可能是格式错误或无法在此平台执
行。
EFAULT 参数filename所指的字符串地址超出可存取空间范围。
ENAMETOOLONG 参数filename所指的字符串太长。
ENOENT 参数filename字符串所指定的文件不存在。
ENOMEM 核心内存不足
ENOTDIR 参数filename字符串所包含的目录路径并非有效目录
EACCES 参数filename字符串所包含的目录路径无法存取，权限不足
ELOOP 过多的符号连接
ETXTBUSY 欲执行的文件已被其他进程打开而且正把数据写入该文件中
EIO I/O 存取错误
ENFILE 已达到系统所允许的打开文件总数。
EMFILE 已达到系统所允许单一进程所能打开的文件总数。
EINVAL 欲执行文件的ELF执行格式不只一个PT_INTERP节区
EISDIR ELF翻译器为一目录
ELIBBAD ELF翻译器有问题。
范例 #include<unistd.h>
main()
{
char * argv[ ]={“ls”,”-al”,”/etc/passwd”,(char *)0};
char * envp[ ]={“PATH=/bin”,0}
execve(“/bin/ls”,argv,envp);
}
执行 -rw-r--r-- 1 root root 705 Sep 3 13 :52 /etc/passwd
execvp（执行文件）
相关函数 fork，execl，execle，execlp，execv，execve
表头文件 #include<unistd.h>
定义函数 int execvp(const char *file ,char * const argv []);
函数说明 execvp()会从PATH环境变量所指的目录中查找符合参数file
的文件名，找
到后便执行该文件，然后将第二个参数argv传给该欲执行的文件。
返回值如果执行成功则函数不会返回，执行失败则直接返回-1，失败原因存于errno
中。
错误代码请参考execve（）。
范例 /*请与execlp（）范例对照*/
#include<unistd.h>
main()
{
char * argv[ ] ={ “ls”,”-al”,”/etc/passwd”,0};
execvp(“ls”,argv);
}
执行 -rw-r--r-- 1 root root 705 Sep 3 13 :52 /etc/passwd
exit（正常结束进程）
相关函数 _exit，atexit，on_exit
表头文件 #include<stdlib.h>
定义函数 void exit(int status);
函数说明 exit()用来正常终结目前进程的执行，并把参数status返回给父进程，而进
程所有的缓冲区数据会自动写回并关闭未关闭的文件。
返回值
范例参考wait（）
_ 
exit（结束进程执行）
相关函数 exit，wait，abort
表头文件 #include<unistd.h>
定义函数 void _exit(int status);
函数说明 _exit()用来立刻结束目前进程的执行，并把参数status返回给父进程，并关
闭未关闭的文件。此函数调用后不会返回，并且会传递SIGCHLD信号给父进程，父进程可
以由wait函数取得子进程结束状态。
返回值
附加说明 _exit（）不会处理标准I/O缓冲区，如要更新缓冲区请使用exit（）。
vfork（建立一个新的进程）
相关函数 wait，execve
表头文件 #include<unistd.h>
定义函数 pid_t vfork(void);
函数说明 vfork()会产生一个新的子进程，其子进程会复制父进程的数据与堆栈空间，
并继承父进程的用户代码，组代码，环境变量、已打开的文件代码、工作目录和资源限
制等。Linux 
使用copy-on-write(COW)技术，只有当其中一进程试图修改欲复制的空间
时才会做真正的复制动作，由于这些继承的信息是复制而来，并非指相同的内存空间，
因此子进程对这些变量的修改和父进程并不会同步。此外，子进程不会继承父进程的文
件锁定和未处理的信号。注意，Linux不保证子进程会比父进程先执行或晚执行，因此编
写程序时要留意
死锁或竞争条件的发生。
返回值如果vfork()成功则在父进程会返回新建立的子进程代码(PID)，而在新建立的
子进程中则返回0。如果vfork失败则直接返回-1，失败原因存于errno中。
错误代码 EAGAIN 
内存不足。ENOMEM 内存不足，无法配置核心所需的数据结构空间。
范例 #include<unistd.h>
main()
{
if(vfork() = =0)
{
printf(“This is the child process\n”);
}else{
printf(“This is the parent process\n”);
}
}
执行 this is the parent process
this is the child process
getpgid（取得进程组识别码）
相关函数 setpgid，setpgrp，getpgrp
表头文件 #include<unistd.h>
定义函数 pid_t getpgid( pid_t pid);
函数说明 getpgid()用来取得参数pid指定进程所属的组识别码。如果参数pid为0，则
会取得目前进程的组识别码。
返回值执行成功则返回组识别码，如果有错误则返回-1，错误原因存于errno中。
错误代码 ESRCH 
找不到符合参数pid 指定的进程。
范例 /*取得init进程（pid＝1）的组识别码*/
#include<unistd.h>
mian()
{
printf(“init gid = %d\n”,getpgid(1));
}
执行 init gid = 0
心态影响行为，行为量定成败。
[url=http://life4j.javaeye.com][/url]
[广告]
