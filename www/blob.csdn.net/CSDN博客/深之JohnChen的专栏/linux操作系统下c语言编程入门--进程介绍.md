# linux操作系统下c语言编程入门--进程介绍   - 深之JohnChen的专栏 - CSDN博客

2007年02月28日 17:56:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2165标签：[linux																[编程																[语言																[c																[shell																[path](https://so.csdn.net/so/search/s.do?q=path&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[Linux & Unix](https://blog.csdn.net/byxdaz/article/category/152883)


linux操作系统下c语言编程入门--进程介绍  

Linux下进程的创建
前言:
这篇文章是用来介绍在Linux下和进程相关的各个概念.我们将会学到:
进程的概念
进程的身份
进程的创建
守护进程的创建
----------------------------------------------------------------------------
1。进程的概念
Linux[操作系统](http://os.rdxx.com/)是面向多用户的.在同一时间可以有许多用户向操作系统发出各种命令.那么[操作系统](http://os.rdxx.com/)是怎么实现多用户的环境呢? 在现代的操作系统里面,都有程序和进程
的概念.那么什么是程序,什么是进程呢? 通俗的讲程序是一个包含可以执行代码的文件
,是一个静态的文件.而进程是一个开始执行但是还没有结束的程序的实例.就是可执行文
件的具体实现. 一个程序可能有许多进程,而每一个进程又可以有许多子进程.依次循环
下去,而产生子孙进程. 当程序被系统调用到内存以后,系统会给程序分配一定的资源(内
存,设备等等)然后进行一系列的复杂操作,使程序变成进程以供系统调用.在系统里面只
有进程没有程序,为了区分各个不同的进程,系统给每一个进程分配了一个ID(就象我们的
身份证)以便识别. 为了充分的利用资源,系统还对进程区分了不同的状态.将进程分为新
建,运行,阻塞,就绪和完成五个状态. 新建表示进程正在被创建,运行是进程正在运行,阻
塞是进程正在等待某一个事件发生,就绪是表示系统正在等待CPU来执行命令,而完成表示
进程已经结束了系统正在回收资源. 关于进程五个状态的详细解说我们可以看《操作系
统》上面有详细的解说。
2。进程的标志
上面我们知道了进程都有一个ID,那么我们怎么得到进程的ID呢?系统调用getpid可
以得到进程的ID,而getppid可以得到父进程(创建调用该函数进程的进程)的ID.
#include <unistd>
pid_t getpid(void);
pid_t getppid(void);
进程是为程序服务的,而程序是为了用户服务的.系统为了找到进程的用户名,还为进程和
用户建立联系.这个用户称为进程的所有者.相应的每一个用户也有一个用户ID.通过系统
调用getuid可以得到进程的所有者的ID.由于进程要用到一些资源,而[Linux](http://os.rdxx.com/Linux/)对系统资源是
进行保护的,为了获取一定资源进程还有一个有效用户ID.这个ID和系统的资源使用有关
,涉及到进程的权限. 通过系统调用geteuid我们可以得到进程的有效用户ID. 和用户ID
相对应进程还有一个组ID和有效组ID系统调用getgid和getegid可以分别得到组ID和有效
组ID
#include <unistd>
#include <sys/types.h>

uid_t getuid(void);
uid_t geteuid(void);
gid_t getgid(void);
git_t getegid(void); 
有时候我们还会对用户的其他信息感兴趣(登录名等等),这个时候我们可以调用getpwui
d来得到.
struct passwd {
char *pw_name; /* 登录名称 */
char *pw_passwd; /* 登录口令 */
uid_t pw_uid; /* 用户ID */
gid_t pw_gid; /* 用户组ID */
char *pw_gecos; /* 用户的真名 */
char *pw_dir; /* 用户的目录 */
char *pw_shell; /* 用户的SHELL */
};
#include <pwd.h>
#include <sys/types.h>

struct passwd *getpwuid(uid_t uid);
下面我们学习一个实例来实践一下上面我们所学习的几个函数:
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <stdio.h>
int main(int argc,char **argv)
{
pid_t my_pid,parent_pid;
uid_t my_uid,my_euid;
gid_t my_gid,my_egid;
struct passwd *my_info;
my_pid=getpid();
parent_pid=getppid();
my_uid=getuid();
my_euid=geteuid();
my_gid=getgid();
my_egid=getegid();
my_info=getpwuid(my_uid);
printf("Process ID:%ld/n",my_pid);
printf("Parent ID:%ld/n",parent_pid);
printf("User ID:%ld/n",my_uid);
printf("Effective User ID:%ld/n",my_euid);
printf("Group ID:%ld/n",my_gid);
printf("Effective Group ID:%ld/n",my_egid):
if(my_info)
{
printf("My Login Name:%s/n" ,my_info->pw_name);
printf("My Password :%s/n" ,my_info->pw_passwd);
printf("My User ID :%ld/n",my_info->pw_uid);
printf("My Group ID :%ld/n",my_info->pw_gid);
printf("My Real Name:%s/n" ,my_info->pw_gecos);
printf("My Home Dir :%s/n", my_info->pw_dir);
printf("My Work Shell:%s/n", my_info->pw_shell);
}
}
3。进程的创建
创建一个进程的系统调用很简单.我们只要调用fork函数就可以了.
#include <unistd.h>

pid_t fork();
当一个进程调用了fork以后,系统会创建一个子进程.这个子进程和父进程不同的地方只
有他的进程ID和父进程ID,其他的都是一样.就象符进程克隆(clone)自己一样.当然创建
两个一模一样的进程是没有意义的.为了区分父进程和子进程,我们必须跟踪fork的返回
值. 当fork掉用失败的时候(内存不足或者是用户的最大进程数已到)fork返回-1,否则f
ork的返回值有重要的作用.对于父进程fork返回子进程的ID,而对于fork子进程返回0.我
们就是根据这个返回值来区分父子进程的. 父进程为什么要创建子进程呢?前面我们已经
说过了[Linux](http://os.rdxx.com/Linux/)是一个多用户[操作系统](http://os.rdxx.com/),在同一时间会有许多的用户在争夺系统的资源.有时
进程为了早一点完成任务就创建子进程来争夺资源. 一旦子进程被创建,父子进程一起从
fork处继续执行,相互竞争系统的资源.有时候我们希望子进程继续执行,而父进程阻塞直
到子进程完成任务.这个时候我们可以调用wait或者waitpid系统调用.
#include <sys/types.h>
#include <sys/wait.h>

pid_t wait(int *stat_loc);
pid_t waitpid(pid_t pid,int *stat_loc,int options);
wait系统调用会使父进程阻塞直到一个子进程结束或者是父进程接受到了一个信号.如果
没有父进程没有子进程或者他的子进程已经结束了wait回立即返回.成功时(因一个子进
程结束)wait将返回子进程的ID,否则返回-1,并设置全局变量errno.stat_loc是子进程的
退出状态.子进程调用exit,_exit 或者是return来设置这个值. 为了得到这个值Linux定
义了几个宏来测试这个返回值.
WIFEXITED:判断子进程退出值是非0
WEXITSTATUS:判断子进程的退出值(当子进程退出时非0).
WIFSIGNALED:子进程由于有没有获得的信号而退出.
WTERMSIG:子进程没有获得的信号号(在WIFSIGNALED为真时才有意义).
waitpid等待指定的子进程直到子进程返回.如果pid为正值则等待指定的进程(pid).如果
为0则等待任何一个组ID和调用者的组ID相同的进程.为-1时等同于wait调用.小于-1时等
待任何一个组ID等于pid绝对值的进程. stat_loc和wait的意义一样. options可以决定
父进程的状态.可以取两个值 WNOHANG:父进程立即返回当没有子进程存在时. WUNTACHE
D:当子进程结束时waitpid返回,但是子进程的退出状态不可得到.
父进程创建子进程后,子进程一般要执行不同的程序.为了调用系统程序,我们可以使用系
统调用exec族调用.exec族调用有着5个函数.
#include <unistd.h>
int execl(const char *path,const char *arg,...);
int execlp(const char *file,const char *arg,...);
int execle(const char *path,const char *arg,...);
int execv(const char *path,char *const argv[]);
int execvp(const char *file,char *const argv[]):
exec族调用可以执行给定程序.关于exec族调用的详细解说可以参考系统手册(man exec
l). 下面我们来学习一个实例.注意编译的时候要加 -lm以便连接数学函数库.
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
void main(void)
{
pid_t child;
int status;
printf("This will demostrate how to get child status/n");
if((child=fork())==-1)
{
printf("Fork Error :%s/n",strerror(errno));
exit(1);
}
else if(child==0)
{
int i;
printf("I am the child:%ld/n",getpid());
for(i=0;i<1000000;i++) sin(i);
i=5;
printf("I exit with %d/n",i);
exit(i);
}
while(((child=wait(&status))==-1)&(errno==EINTR));
if(child==-1)
printf("Wait Error:%s/n",strerror(errno));
else if(!status)
printf("Child %ld terminated normally return status is zero/n",
child);
else if(WIFEXITED(status))
printf("Child %ld terminated normally return status is %d/n",
child,WEXITSTATUS(status));
else if(WIFSIGNALED(status))
printf("Child %ld terminated due to signal %d znot caught/n",
child,WTERMSIG(status)); 
}
strerror函数会返回一个指定的错误号的错误信息的字符串.
4。守护进程的创建
如果你在[DOS](http://os.rdxx.com/DOS/)时代编写过程序,那么你也许知道在DOS下为了编写一个常驻内存的程序
我们要编写多少代码了.相反如果在[Linux](http://os.rdxx.com/Linux/)下编写一个"常驻内存"的程序却是很容易的.我
们只要几行代码就可以做到. 实际上由于Linux是多任务[操作系统](http://os.rdxx.com/),我们就是不编写代码
也可以把一个程序放到后台去执行的.我们只要在命令后面加上&符号SHELL就会把我们的
程序放到后台去运行的. 这里我们"开发"一个后台检查邮件的程序.这个程序每个一个指
定的时间回去检查我们的邮箱,如果发现我们有邮件了,会不断的报警(通过机箱上的小喇
叭来发出声音). 后面有这个函数的加强版本加强版本
后台进程的创建思想: 首先父进程创建一个子进程.然后子进程杀死父进程(是不是很无
情?). 信号处理所有的工作由子进程来处理.
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
/* Linux 的默任个人的邮箱地址是 /var/spool/mail/用户的登录名 */
#define MAIL "/var/spool/mail/hoyt"
/* 睡眠10秒钟 */

#define SLEEP_TIME 10
main(void)
{
pid_t child;
if((child=fork())==-1)
{
printf("Fork Error:%s/n",strerror(errno));
exit(1);
}
else if(child>0)
while(1);
if(kill(getppid(),SIGTERM)==-1)
{
printf("Kill Parent Error:%s/n",strerror(errno));
exit(1);
} 
{
int mailfd;
while(1)
{
if((mailfd=open(MAIL,O_RDONLY))!=-1)
{
fprintf(stderr,"%s","/007");
close(mailfd);
} 
sleep(SLEEP_TIME);
}
}
}
你可以在默认的路径下创建你的邮箱文件,然后测试一下这个程序.当然这个程序还有很
多地方要改善的.我们后面会对这个小程序改善的,再看我的改善之前你可以尝试自己改
善一下.比如让用户指定邮相的路径和睡眠时间等等.相信自己可以做到的.动手吧,勇敢的探险者.
好了进程一节的内容我们就先学到这里了.进程是一个非常重要的概念,许多的程序都会
用子进程.创建一个子进程是每一个程序员的基本要求!   


