# fork两次如何避免僵尸进程 - ljx0305的专栏 - CSDN博客
2012年11月08日 13:49:21[ljx0305](https://me.csdn.net/ljx0305)阅读数：716
在fork()/execve()过程中，假设子进程结束时父进程仍存在，而父进程fork()之前既没安装SIGCHLD 信号处理函数调用waitpid()等待子进程结束，又没有显式忽略该信号，则子进程成为僵尸进程，无法正常结束，此时即使是root身份kill-9也 不能杀死僵尸进程。补救办法是杀死僵尸进程的父进程(僵尸进程的父进程必然存在)，僵尸进程成为"孤儿进程"，过继给1号进程init，init始终会负 责清理僵尸进程。
怎样来清除僵尸进程：
1.改写父进程，在子进程死后要为它收尸。具体做法是接管SIGCHLD信号。子进程死后，会发送SIGCHLD信号 给父进程，父进程收到此信号后，执行 waitpid()函数为子进程收尸。这是基于这样的原理：就算父进程没有调用wait，内核也会向它发送SIGCHLD消息，尽管对的默认处理是忽略， 如果想响应这个消息，可以设置一个处理函数。
2.把父进程杀掉。父进程死后，僵尸进程成为"孤儿进程"，过继给1号进程init，init始终会负责清理僵尸进程．它产生的所有僵尸进程也跟着消失。
3.调用fork两次。
如何产生僵死进程：
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv)
{
if (0 == fork()) {
printf("In the child process:%d\n", getpid());
} else {
printf("In the parent process:%d\n", getpid());
sleep(10);
exit(0);
}
printf("Pid:%d\n", getpid());
return 0;
}
如何避免：
若调用成功则返回清理掉的子进程id，若调用出错则返回-1。父进程调用`wait`或`waitpid`时可能会：
- 
阻塞（如果它的所有子进程都还在运行）。
- 
带子进程的终止信息立即返回（如果一个子进程已终止，正等待父进程读取其终止信息）。
- 
出错立即返回（如果它没有任何子进程）。
这两个函数的区别是：
- 
如果父进程的所有子进程都还在运行，调用`wait`将使父进程阻塞，而调用`waitpid`时如果在`options`参数中指定`WNOHANG`可以使父进程不阻塞而立即返回0。
- 
`wait`等待第一个终止的子进程，而`waitpid`可以通过`pid`参数指定等待哪一个子进程。
可见，调用`wait`和`waitpid`不仅可以获得子进程的终止信息，还可以使父进程阻塞等待子进程终止，起到进
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main(void)
{
 pid_t pid;
 pid = fork();
 if (pid < 0) {
  perror("fork failed");
  exit(1);
 }
 if (pid == 0) {
  int i;
  for (i = 3; i > 0; i--) {
   printf("This is the child\n");
   sleep(1);
  }
  exit(3);
 } else {
  int stat_val;
  waitpid(pid, &stat_val, 0);
  if (WIFEXITED(stat_val))
   printf("Child exited with code %d\n", WEXITSTATUS(stat_val));
  else if (WIFSIGNALED(stat_val))
   printf("Child terminated abnormally, signal %d\n", WTERMSIG(stat_val));
 }
 return 0;
}
2 处理信号：
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include<sys/wait.h>
void proc_child(int SIGNO)
{
int pid = -1;
int stat;
pid = waitpid(-1, &stat, WNOHANG);
}
int main(int argc, char **argv)
{
signal(SIGCHLD, proc_child);
if (0 == fork()) {
printf("In the child process:%d\n", getpid());
} else {
printf("In the parent process:%d\n", getpid());
sleep(10);
exit(0);
}
printf("Pid:%d\n", getpid());
return 0;
}
3 忽略信号
int main(int argc, char **argv)
{
signal(SIGCHLD, SIG_IGN); //加入此句即可，忽略SIGCHLD信号
if (0 == fork()) {
printf("In the child process:%d\n", getpid());
} else {
printf("In the parent process:%d\n", getpid());
sleep(10);
exit(0);
}
printf("Pid:%d\n", getpid());
return 0;
}
4 apue上的fork两次：
fork两次在防止僵死方面来说，就是因为儿子进程先退出，孙子进程就被init接管了，实际上与最初的父进程脱离了关系，就不会僵死了。
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
int main(void)   
{   
   pid_t pid;   
    if ((pid = fork()) < 0)   
    {   
        fprintf(stderr,"Fork error!\n");   
        exit(-1);   
    }   
    else if (pid == 0) /* first child */  
    {    
        if ((pid = fork()) < 0)   
        {    
            fprintf(stderr,"Fork error!\n");   
            exit(-1);   
        }   
        else if (pid > 0)   
            exit(0); /* parent from second fork == first child */  
        /*  
         * We're the second child; our parent becomes init as soon  
         * as our real parent calls exit() in the statement above.  
         * Here's where we'd continue executing, knowing that when  
         * we're done, init will reap our status.  
         */  
        sleep(2);   
        printf("Second child, parent pid = %d\n", getppid());   
        exit(0);   
    }   
    if (waitpid(pid, NULL, 0) != pid) /* wait for first child */  
    {   
        fprintf(stderr,"Waitpid error!\n");   
        exit(-1);   
    }   
    /*  
     * We're the parent (the original process); we continue executing,  
     * knowing that we're not the parent of the second child.  
     */  
    exit(0);   
}   
5 
static void
sig_child()
{
    waitpid(-1, NULL, 0);
}
[...]
signal(SIGCHLD, sig_child)
sig_child这个函数有问题。这个函数的作用是每当有一个进程退出时，便会调用一次sig_child，sig_child便会调用
waitpid处理一个已退出的子进程。但是假如程序正在sig_child里面，这时又有另外一个子进程退出了，这时该怎么处理呢？处理的方法应该是不
会有另外一个sig_child响应，于是这时便有2个子进程退出了，但是sig_child只能处理一个，于是便有一个退出的子进程成了僵死进程了。
于是把sig_child改成：
static void
sig_child()
{
    while (waitpid(-1, NULL, 0) > 0);
}
这样假如程序正在sig_child里面，这时又有另外n个子进程退出了，由于用了while，于是sig_child便会将所以退出的子进程“一网打尽”。
这是一种处理僵死进程的方法，另外还有2种：
signal(SIGCHLD, SIG_IGN);
忽略SIGCHLD信号，父进程不需要处理，直接把退出的子进程推给init进程处理。
struct sigaction    sa;
sa.sa_handler = SIG_IGN;
sa.sa_flags = SA_NOCLDWAIT;
sigemptyset(&sa.sa_mask);
if (sigaction(SIGCHLD, &sa, NULL) < 0) {
    exit(-1);
}
