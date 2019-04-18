# getrlimit和setrlimit - ljx0305的专栏 - CSDN博客
2008年05月15日 22:19:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：2360标签：[struct																[shell																[存储																[rss																[文档																[file](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=文档&t=blog)](https://so.csdn.net/so/search/s.do?q=rss&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)
个人分类：[Linux程序及使用](https://blog.csdn.net/ljx0305/article/category/394700)
getrlimit和setrlimit函数 
　　每个进程都有一组资源限制，其中某一些可以用getrlimit和setrlimit函数查询和更改。
#include 
#include 
int getrlimit(int resource,struct rlimit *rlptr);
int setrlimit(int resource,const struct rlimit rlptr);
Both return: 0 if OK,nonzero on error两个函数
　　返回：若成功为0，出错为非0
　　对这两个函数的每一次调用都指定一个资源以及一个指向下列结构的指针。
struct rlimit{
rlim rlim ur;/* 软限制：当前限制 */
rlim rlim ax;／* 硬限制：rlimcur的最大值 */
}；
　　这两个函数不属于POSIX.1，但SVR4和4.3+BSD提供它们。SVR4在上面的结构中使用基本系统数据类型rlim。其它系统则将这
两个成员定义为整型或长整型。
　　进程的资源限制通常是在系统初启时由0#进程建立的，然后由后续进程继承。在SVR4中，系统默认值可以查看文
件/etc/conf/cfd/mtune在4.3+BSD中，系统默认值分散在多个头文件中。
　　在更改资源限制时，须遵循下列三条规则：
　　1.任何一个进程都可将一个软限制更改为小于或等于其硬限制。
　　2.任何一个进程都可降低其硬限制值，但它必须大于或等于其软限制值。这种降低，对普通用户而言是不可逆反的。
　　3.只有超级用户可以提高硬限制。
　　一个无限量的限制由常数RLIM NFINITY指定。
　　这两个函数的resource参数取下列值之一。注意并非所有资源限制都受到SVR4和4.3+BSD的支持。
　　RLMITCORE（SVR4及4.3+BSD)core文件的最大字节数，若其值为0则阻止创建core文件。
　　RLIMIT PU(SVR4及4.3+BSD)CPU时间的最大量值(秒)，当超过此软限止时，向该进程发送SIGXCPU信号。
　　RLIMIT ATA(SVR4及4.3+BSD)数据段的最大字节长度。这是图7.3中初始化数据、非初始化数据以及堆的总和。
　　RLIMIT SIZE(SVR4及4.3+BSD)可以创建的一个文件的最大字节长度。当超过此软限制时，则向该进程发送SIGFSZ信号。
　　RLIMIT EMLOCK（4.3+BSD)锁定在存储器地址空间(尚末实现)。
　　RLIMIT OFILE(SVR4)每个进程最多打开的文件数。更改此限制将影响到sysconf函数在参数-sc-OPEN-MAX中返回的值(2.5.4节
)。见程序2.3。
　　RLIMIT PROC(4.3+BSD)每个实际用户ID所拥有的最大子进程数。更改此限制将影响到sysconf函数在参数 CHILDMAX中返回的
值(2.5.4节)。
　　RLIMIT FILE(4.3+BSD)与SVR4的RLIMIT OFILE相同。
　　RLIMIT SS（4.3+BSD)最大驻内存集字节长度(RSS)。如果物理存储器供子应求，则系统核将从进程处取回超过RSS的部分。
　　RLIMIT TACK(SVR4及4.3+BSD)栈的最大字节长度。见图7.3。
　　RLIMIT MEM（SVR4)可映照地址空间的最大字节长度。这影响到mmap函数(12.9节)。
　　资源限制影响到调用进程并由其子进程继承。这就意味着为了影响一个用户的所有后续进程，需将资源限制设置构造在shell
之中。确实，Bourne Shell和Kornshell具有内部ulimit命令，CShell具有内部limit命令。(umask和chdir也必须是shell内部的)
。
　　较早的BourmeShell，例如由贝克莱提供的一种，不支持ulimit命令。较新的KornShell的ulimit命令具有-H和-s选择项，以
分别检查和修改硬和软的限制，但它们尚末编写入文档。
?
?简单的用例：
#include<sys/time.h>
#include<sys/resource.h>
#include<unistd.h>
int main()
{
  struct rlimit limit;
 char p = '1';
 limit.rlim_cur = RLIM_INFINITY;
 limit.rlim_max = RLIM_INFINITY;
 if(setrlimit(RLIMIT_CORE, &limit))
 {
  printf("set limit failed/n");
 }
 printf("p = %s/n",p);
} 
---------------------------------------------------------------------------------------------------------------------------------------------
对每个进程都有一组资源限制，其中一些可以用getrlimit和setrlimit函数查询和更改
getrlimit和setrlimit：资源限制 (getrlimit and setrlimit: Resource Limits)
- 这两个系统函数是和resource limit相关的。你用过ulimit命令吗？（反正我没用过） 
- 对于每个资源来说有两个limit：一个是hard limit，一个是soft limit，其中后者永远不能超过前者，并且只有拥有superuser权限的进程可以更改前者。 
- getrlimit和setrlimit的参数相同：第一个参数是资源的类型，第二个参数是rlimit结构的指针，这个结构里面就只有两个成员：hard limit和soft limit 
- 几个重要的资源类型
    - RLIMIT_CPU：程序执行的最大CPU时间，单位是秒。超过之后程序中止，中止的信号是SIGXCPU 
- RLIMIT_DATA：程序执行的最大内存 
- RLIMIT_NPROC：程序孵出的最大子进程数量 
- RLIMIT_NOFILE：程序打开的文件的最大数量 
引用：[http://blog.csdn.net/zhoujunyi/archive/2007/04/14/1564579.aspx](http://blog.csdn.net/zhoujunyi/archive/2007/04/14/1564579.aspx)
