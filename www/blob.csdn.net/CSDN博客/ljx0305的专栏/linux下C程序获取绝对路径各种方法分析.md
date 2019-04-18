# linux下C程序获取绝对路径各种方法分析 - ljx0305的专栏 - CSDN博客
2013年06月22日 21:10:52[ljx0305](https://me.csdn.net/ljx0305)阅读数：21650
个人分类：[C																[C++																[Linux](https://blog.csdn.net/ljx0305/article/category/364133)](https://blog.csdn.net/ljx0305/article/category/380566)](https://blog.csdn.net/ljx0305/article/category/394717)
昨天肚腩群里有人问LINUX下C如何获取程序绝对路径。初看这问题，肚腩觉得很简单啊，就用getcwd或者argv[0]就可以了。写了个程序试试，
> 
#include <unistd.h>
int main(int argc,char *argv[])
{
    char buffer[100];
    getcwd(buffer, sizeof(buffer));
    printf("The current directory is: %s\n", buffer);
    printf("prog name : %s \n" , argv[0]);
    return 0;
}
编译：
> 
gcc -Wall -pipe -g -static -o getpath getpath.c 
发现，getcwd不行，这个函数返回的是当时的执行路径，如果在其他目录执行该程序，如～/.这样调用就不行了。
argv[0]也不行，程序的第一个参数未必是绝对路径，比如使用/home/../../root/getpath这样调用，那么第一个参数就变成了这样的相对路径。
肚腩改进了这个程序，用realpath把相对路径转换成绝对路径：
> 
#include  <unistd.h>
main(int argc, char * argv[])
{
    char  resolved_path[80];
    realpath(argv[0],resolved_path);
    printf("resolved_path: %s\n", resolved_path);
    sleep(300);
}
OK.这个时候argv[0]用相对路径调用的问题解决了。
#### BUGS
但随后肚腩又发现存在另外一个问题，比如把程序放到/usr/bin这些存在于环境变量PATH的目录下，然后在/home目录下通过程序名直接执行这个程序，输出的程序绝对路径就变成了/home/getpath,实际上应该是/usr/bin/getpath。
#### ELF文件里是否有相关信息
还有没有其它办法，肚腩先试了下在这个ELF文件下是否有一些绝对路径的信息。
> 
执行：strings getpath| grep getpath
自然是没有找到了。从另一个方向想想，如果绝对路径的信息保存在ELF文件中，那么改变一个程序的路径，连程序的内容都要修改？所以是不可能滴。
#### 尝试从进程内存获取
那么这个绝对路径的信息，只能是程序运行的时候，加入到进程空间里面的。我们看看是否能否进程内存获取?
写一个代码看下系统的内存地址分布情况：
> 
#include <stdio.h>
#include <string.h>
#include<stdlib.h>
int a = 0; //全局初始化区
char *p1; //全局未初始化区
main(int argc,char **argv)
{
int b; //栈
char s[] = "abc"; //栈
char *p2; //栈
char *p3 = "123456";// 123456\0在常量区，p3在栈上。
static int c =0;// 全局（静态）初始化区
static int uc,uc1,uc2;// 全局（静态）未初始化区
p1 = (char *)malloc(10);
p2 = (char *)malloc(20);
//分配得来得10和20字节的区域就在堆区。
strcpy(p1, "123456");// 123456\0放在常量区，编译器可能会将它与p3所指向的"123456"优化成一个地方。
printf("堆p1 \t\t\t%p\n",p1);
printf("堆p2 \t\t\t%p\n",p2);
printf("栈&p3 \t\t\t%p\n",&p3);
printf("栈&p2 \t\t\t%p\n",&p2);
printf("栈s \t\t\t%p\n",s);
printf("栈&s[1] \t\t%p\n",&s[1]);
printf("栈&b \t\t\t%p\n",&b);
printf("main地址\t\t%p\n",main);
printf("文本常量区\t\t%p\n",p3);
printf("全局初始化区\t\t%p\n",&a);
printf("（静态）初始化区\t%p\n",&c);
printf("全局未初始化区\t\t%p\n",&p1);
printf("（静态）未初始化区\t%p\n",&uc);
printf("（静态）未初始化区\t%p\n",&uc1);
printf("（静态）未初始化区\t%p\n",&uc2);
char *p;
if(p=getenv("PATH")){
        printf("USER=%p\n",p);
}
printf("ARGC\t%p\n",&argc);
printf("ARGV\t%p\n",argv);
printf("ARGV\t%p\n",*argv);
sleep(300);
}
这里要指出一下，因为肚腩的系统是x86-64.64为系统，这里用gcc打印地址的是要注意是使用%p,不用去用%x.用uname -a可以查看系统版本。
> 
编译一下：gcc -Wall -pipe -g -static -o memoryshow memoryshow.c 
执行，结果是：
> 
[root@localhost ~]# ./memoryshow 
堆p1                    0xa3e8b0
堆p2                    0xa3e8d0
栈&p3                   0x7fff94fec4a0
栈&p2                   0x7fff94fec4a8
栈s                     0x7fff94fec4b0
栈&s[1]                 0x7fff94fec4b1
栈&b                    0x7fff94fec4b4
main地址                0×400494
文本常量区              0x47f3b0
全局初始化区            0x6a6e50
（静态）初始化区        0x6a6e54
全局未初始化区          0x6a9ba8
（静态）未初始化区      0x6a6e58
（静态）未初始化区      0x6a6e5c
（静态）未初始化区      0x6a6e60
USER=0x7fff94fece29
ARGC    0x7fff94fec49c
ARGV    0x7fff94fec598
ARGV    0x7fff94fec82b
查看下这个进程的map情况：
> 
[root@localhost ~]# ps -ef | grep showmemory
root      2715  2150  0 21:58 pts/1    00:00:00 grep showmemory
[root@localhost ~]# ps -ef | grep memory
root      2713  2087  0 21:58 pts/0    00:00:00 ./memoryshow
root      2717  2150  0 21:58 pts/1    00:00:00 grep memory
[root@localhost ~]# cat /proc/2713/maps 
00400000-004a6000 r-xp 00000000 fd:00 524196                             /root/memoryshow
006a6000-006a7000 rw-p 000a6000 fd:00 524196                             /root/memoryshow
006a7000-006aa000 rw-p 00000000 00:00 0 
00a3d000-00a60000 rw-p 00000000 00:00 0                                  [heap]
7f050a5c2000-7f050a5c3000 rw-p 00000000 00:00 0 
7fff94fd8000-7fff94fed000 rw-p 00000000 00:00 0                          [stack]
7fff94fff000-7fff95000000 r-xp 00000000 00:00 0                          [vdso]
ffffffffff600000-ffffffffff601000 r-xp 00000000 00:00 0                  [vsyscall]
再次重复一下操作，肚腩需要看下进程哪些地址会改变，那些不会改变。网上一般的32为线性分布，和肚腩的系统对不上号。所以自己写个程序一看就清晰明了了。
> 
[root@localhost ~]# ./memoryshow 
堆p1                    0x76b8b0
堆p2                    0x76b8d0
栈&p3                   0x7fff9987a9f0
栈&p2                   0x7fff9987a9f8
栈s                     0x7fff9987aa00
栈&s[1]                 0x7fff9987aa01
栈&b                    0x7fff9987aa04
main地址                0×400494
文本常量区              0x47f3b0
全局初始化区            0x6a6e50
（静态）初始化区        0x6a6e54
全局未初始化区          0x6a9ba8
（静态）未初始化区      0x6a6e58
（静态）未初始化区      0x6a6e5c
（静态）未初始化区      0x6a6e60
USER=0x7fff9987ce29
ARGC    0x7fff9987a9ec
ARGV    0x7fff9987aae8
ARGV    0x7fff9987c82b
其对应的MAP情况：
> 
[root@localhost ~]# cat /proc/2713/maps 
cat: /proc/2713/maps: No such file or directory
[root@localhost ~]# ps -ef | grep memory
root      2719  2087  0 21:59 pts/0    00:00:00 ./memoryshow
root      2722  2150  0 21:59 pts/1    00:00:00 grep memory
[root@localhost ~]# cat /proc/2719/maps 
00400000-004a6000 r-xp 00000000 fd:00 524196                             /root/memoryshow
006a6000-006a7000 rw-p 000a6000 fd:00 524196                             /root/memoryshow
006a7000-006aa000 rw-p 00000000 00:00 0 
0076a000-0078d000 rw-p 00000000 00:00 0                                  [heap]
7f9987494000-7f9987495000 rw-p 00000000 00:00 0 
7fff99868000-7fff9987d000 rw-p 00000000 00:00 0                          [stack]
7fff999ff000-7fff99a00000 r-xp 00000000 00:00 0                          [vdso]
ffffffffff600000-ffffffffff601000 r-xp 00000000 00:00 0                  [vsyscall]
这里可以看到，前面三个段的地址是不变的，但堆开始，后面的高位地址段每次运行都会改变。
如果包含文件绝对路径的数据是落在前面三个段，那么可以通过地址直接获取，如果是在后面，可能就要通过计算获取。
用GDB调试下，看看何种情况：
> 
[root@localhost ~]# gdb memoryshow
Reading symbols from /root/memoryshow…done.
(gdb) b main
Breakpoint 1 at 0x4004a3: file memoryshow.c, line 11.
(gdb) r
Starting program: /root/memoryshow 
Breakpoint 1, main (argc=1, argv=0x7fffffffe5c8) at memoryshow.c:11
11      char s[] = "abc"; //栈
(gdb) b 44
Breakpoint 2 at 0x4006db: file memoryshow.c, line 44.
(gdb) c
Continuing.
堆p1                    0x6ab8b0
堆p2                    0x6ab8d0
栈&p3                   0x7fffffffe4d0
栈&p2                   0x7fffffffe4d8
栈s                     0x7fffffffe4e0
栈&s[1]                 0x7fffffffe4e1
栈&b                    0x7fffffffe4e4
main地址                0×400494
文本常量区              0x47f3b0
全局初始化区            0x6a6e50
（静态）初始化区        0x6a6e54
全局未初始化区          0x6a9ba8
（静态）未初始化区      0x6a6e58
（静态）未初始化区      0x6a6e5c
（静态）未初始化区      0x6a6e60
USER=0x7fffffffedff
ARGC    0x7fffffffe4cc
ARGV    0x7fffffffe5c8
ARGV    0x7fffffffe81b
Breakpoint 2, main (argc=1, argv=0x7fffffffe5c8) at memoryshow.c:45
45      sleep(300);
(gdb) x/30s 0x7fffffffedff
0x7fffffffedff:  "/usr/local/maven/bin:/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin:/usr/local/jrockit/bin:/usr/local/redis/bin:/usr/local/mysql/bin:/root/bin"
0x7fffffffee9c:  "MAIL=/var/spool/mail/root"
0x7fffffffeeb6:  "nodeName=_0_218"
0x7fffffffeec6:  "_=/usr/bin/gdb"
0x7fffffffeed5:  "PWD=/root"
0x7fffffffeedf:  "JAVA_HOME=/usr/local/jrockit"
0x7fffffffeefc:  "LANG=en_US.UTF-8"
0x7fffffffef0d:  "configDir=/data/config"
0x7fffffffef24:  "LINES=43"
0x7fffffffef2d:  "HISTCONTROL=ignoredups"
0x7fffffffef44:  "HOME=/root"
0x7fffffffef4f:  "SHLVL=1"
0x7fffffffef57:  "M2_HOME=/usr/local/maven"
0x7fffffffef70:  "LOGNAME=root"
0x7fffffffef7d:  "SSH_CONNECTION=192.168.0.158 62555 192.168.0.98 22"
0x7fffffffefb0:  "LESSOPEN=|/usr/bin/lesspipe.sh %s"
0x7fffffffefd2:  "G_BROKEN_FILENAMES=1"
0x7fffffffefe7:  "/root/memoryshow"   <–注意这里
0x7fffffffeff8:  ""
0x7fffffffeff9:  ""
0x7fffffffeffa:  ""
0x7fffffffeffb:  ""
0x7fffffffeffc:  ""
0x7fffffffeffd:  ""
0x7fffffffeffe:  ""
0x7fffffffefff:  ""
0x7ffffffff000:  <Address 0x7ffffffff000 out of bounds>
0x7ffffffff000:  <Address 0x7ffffffff000 out of bounds>
0x7ffffffff000:  <Address 0x7ffffffff000 out of bounds>
0x7ffffffff000:  <Address 0x7ffffffff000 out of bounds>
这里可以看到程序运行的绝对路径在envstring中保存，而envstring却在栈中，而栈地址是每次运行都变化的。而且如果继续测试，还可以发现，调用的方式不同，这个绝对路径对应栈开始地址的位置也是不同。而且机器不同，算法也不同。所以直接从进程内存里面获取绝对路径也不太靠谱。
#### 从/proc/self/exe获取
肚腩在网上查了下，最常用的方法是利用进程映射文件,用readlink读取/proc/self/exe这个符号链接指向的文件.插一段，如何判断链接是符号链接或者是硬链接，主要是看文件的INODE，一样就是硬链接，不一样就是符号链接。用ls -li查看。
网上找到的一段代码：
> 
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h> 
#define MAXBUFSIZE 1024 
int main ( int argc, char * argv[] ) { 
    char buf[ MAXBUFSIZE ]; 
    int count; 
    count = readlink( "/proc/self/exe", buf, MAXBUFSIZE ); 
    if ( count < 0 || count >= MAXBUFSIZE ) { 
    printf( "Failed\n" ); 
    return( EXIT_FAILURE ); 
    } 
    buf[ count ] = '\0'; 
    printf( "/proc/self/exe -> [%s]\n", buf ); 
    return( EXIT_SUCCESS ); 
} /* end of main */ 
#### exe文件权限问题
可能有人会担心/proc/self/exe这个文件的权限问题，肚腩觉得不需要担心，该目录下的文件权限是由系统确认的，用户是不能修改的。因此读的是自己进程在PROC目录下的映射目录，用户权限肯定是有的。如果去读其它用户运行的程序在/proc下的映射目录，就不一定有权限了。
**备注：**文章内容均出自[肚腩照明月](http://wjthink.org/)个人的真实理解以及对网上一些知识的总结，而并非存心妄自揣测来故意愚人耳目。由于个人水平有限，虽力求内容正确无误，但仍然难免出错，请勿见怪，如果可以则请留言告之。
[http://www.wjthink.org/?p=522](http://www.wjthink.org/?p=522)
