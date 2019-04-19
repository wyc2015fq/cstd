# 《UNIX环境高级编程》笔记——进程（二） - yixianfeng41的专栏 - CSDN博客
2015年12月31日 21:32:18[第2梦](https://me.csdn.net/yixianfeng41)阅读数：975
所属专栏：[Linux/Unix 网络编程](https://blog.csdn.net/column/details/14513.html)
本篇主要是进程控制相关知识。
## 一、进程标示符
每个进程都有一个非负整数表示的唯一ID。虽然是唯一的，但是进程ID可以重用。当一个进程终止后，其进程ID就可以再次使用了。
ID为0的通常是调度进程，常被称为交换进程，也被称为系统进程；
ID为1的通常是init进程；
ID为2的通常是页守护进程。
```cpp
#include <sys/types.h>
#include <unistd.h>
pid_t getpid(void); return PID
pid_t getppid(void); return parent PID
uid_t getuid(void); return real UID
uid_t geteuid(void); return effective UID
gid_t getgid(void); return real GID
gid_t getegid(void); return effective GID
```
```cpp
```
### 二、Fork和vfork
**1. Fork**
   pid_t fork (void ); 
正确返回：父进程中返回子进程的进程号；子进程中返回0 ；
错误返回：-1；
子进程是父进程的一个拷贝。子进程从父进程那得到了数据段和堆栈段，但不是与父进程共享而是单独分配内存。fork函数返回后，子进程和父进程都是从fork函数的下一条语句开始执行。由于子进程与父进程的运行是无关的，父进程可先于子进程运行，子进程也可先于父进程运行。fork在linux下使用了写时复制技术，刚开始共享父进程的数据段，在写数据段的时候才进行复制。
**2. Vfork**
```cpp
pid_t vfork (void );
```
返回值同上。
vfork创建新进程的主要目的在于用exec函数执行另外的程序，实际上，在没调用exec或exit之前子进程与父进程共享数据段。在vfork调用中，子进程先运行，父进程挂起，直到子进程调用exec或exit，在这以后，父子进程的执行顺序不再有限制。
**3. 区别：**
1）fork()创建的子进程是父进程的副本。即子进程获取父进程数据空间，堆和 栈的副本。父子进程之间不共享这些存储空间。而vfork()创建的进程并不将父进程的地址空间完全复制到子进程中，因为子进程会立即调用exec(或exit)，于是也就不会存放该地址空间。在子进程调用exec或exit之前，它在父进程的空间进行。
2）vfork()与fork()另一个区别就是：vfork保证子进程先运行，在调用exec或exit之前与父进程数据是共享的,在它调用exec或exit之后父进程才可能被调度运行；而fork的父子进程是同级别的，没有前后限制。vfork保证子进程先运行，如果子进程依赖于父进程的进一步动作，则会导致死锁。
**4. 为什么会有vfork呢?**
因为以前的fork当它创建一个子进程时，将会创建一个新的地址空间，并且拷贝父进程的资源，然后将会有两种行为：
1）执行从父进程那里拷贝过来的代码段
2）调用一个exec执行一个新的代码段
当进程调用exec函数时，一个新程序替换了当前进程的正文，数据，堆和栈段。这样，前面的拷贝工作就是白费力气了，这种情况下，聪明的人就想出了vfork。vfork并不复制父进程的进程环境，子进程在父进程的地址空间中运行，所以子进程不能进行写操作，并且在儿子“霸占”着老子的房子时候，要委屈老子一下了，让他在外面歇着（阻塞），一旦儿子执行了exec或者exit后，相当于儿子买了自己的房子了，这时候就相当于分家了。
因此，如果创建子进程是为了调用exec执行一个新的程序的时候，就应该使用vfork。
### 三、Unix如何实现文件共享
多个进程打开同一个文件时，每个进程都获得各自的一个文件表项，每个进程都有它自己的对该文件的当前偏移量。这里，当不同进程对同一文件进行写操作时，通过一些原子操作和文件记录锁来避免产生冲突的。
### 四、wait和waitpid
当一个进程正常或者异常终止时，内核就向其父进程发送SIGCHID信号。但是，当父进程在子进程之前终止怎么办，也就是孤儿进程怎么办？孤儿进程将由init进程收养。这样保证了每一个进程都有一个父进程。
这里还会涉及到一个概念，僵死进程，一个已经终止，但是其父进程还没有对其进行善后处理（也就是获得子进程的有关信息，释放它占用的资源）的进程称为僵死进程。
那么父进程是如何获得子进程的终止状态得了？内核为每个终止子进程保存了一定量的信息，所以当终止进程的父进程调用wait或者waitpid时，可以得到这些信息。
那么进程调用wait或waitpid会发生什么情况了？
1）如果其所有子进程都还在运行，则阻塞；
2）如果一个子进程已终止，正等待父进程获取其终止状态，则取得该子进程的终止状态立即返回。
3）如果他没有任何子进程，则立即出错返回。
```cpp
pid_t wait(int *statloc);   //staloc指向存放终止状态的内存单元
pid_t waitpid( pid_t pid, int *statloc, int options);
```
这两个函数区别如下：
1）在一个子进程终止前，wait使其调用者阻塞，而waitpid有一个选项，可使调用者不阻塞；
2）waitpid并不等待在其调用之后的第一个终止子进程，它有若干个选项，可以控制它所等待的子进程。
如果不想让一个进程等待其子进程终止，也不希望子进程处于僵死状态直到父进程终止，有一个实现技巧：调用两次FORK;
### 五、竞争条件
![](https://img-blog.csdn.net/20151231213143797)
### 六、Exec函数
用fork可以创建新进程，用exec可以执行新程序。当进程调用一种exec函数时，该进程执行的程序完全替换为新程序，而新程序则从其main函数开始执行。因为调用exec并不创建新进程，所以前后的进程ID并不改变。Exec只是用一个全新的程序替换了当前的进程的正文、数据、堆和栈。
```cpp
#include <unistd.h>
int execl(const char *pathname, 
const char *arg0, … /* (char *)0 */);
int execv(const char *pathname, char *const argv[]);
int execle(const char *pathname, const char *arg0, … /* (char *)0, char *const envp[] */);
int execve(const char *pathname, 
char *const argv[], char *const envp[]);
int execlp(const char *filename, const char *arg0, … /* (char *)0 */);
int execvp(const char *filename, char *const argv[]);
Return -1 on error, no return on success.
```
![](https://img-blog.csdn.net/20170221110445359)
这几个函数功能都差不多，主要区别在于参数上:
1)待执行的程序文件是由文件名还是由路径名制定；
2）新程序的参数是一一列举还是由一个指针数组来引用；
3）把调用进程的环境传递给新程序还是新程序指定新的环境。
我的记法是：
l:表示列举参数；
v:表示*argv，参数数组由指针来指向；
e:表示envp，环境
p:有p的是file，无p的是path
### 七、更改用户ID和组ID
在unix系统中，特权是基于用户和组ID的，当程序需要增加特权，或需要访问当前并不允许访问的资源时，我们需要更换自己用户ID或组ID，使的新ID具有合适的特权或访问权限。与此类似，当程序需要降低其特权或阻止对某些资源的访问时，也需要更换用户ID或组ID，从而使新ID不具有相应特权或访问这些资源的能力。
可以用setuid函数设置实际用户ID和有效ID，与此类似，可以使用setgid函数设置实际组ID和有效组ID。
```cpp
int setuid(uid_t uid);
    int setgid(gid_t gid);
    Return: 0 if OK, -1 on error
```
关于改变用户ID的规则如下：（用户ID所说明的一切都适用于组ID）
1.若进程具有超集用户特权，那牛掰了，进程执行setuid函数可以将实际用户ID，有效用户ID，以及保存的设置用户ID都设置为uid。
2.若进程没有超级用户特权，只是普普通通的“老百姓”，那该进程只能改变自己的有效用户ID，而且改也只能改成实际ID或者设置ID。
3.如果上面两个条件都不满足，则将errno设置为EPERM，并返回-1.
关于内核所维护的三个用户ID，还要注意下列几点：
1.只有超级用户进程可以更改实际用户ID，通常实际用户ID是在用户登录时，由login程序设置的，而且永远不会改变它。
2.仅当对程序文件设置了设置用户ID位后，exec函数才会设置有效用户ID。任何时候都可以调用setuid，将有效用户ID设置为实际用户ID或保存的设置用户ID。自然，不能将有效用户ID设置为任何随机值。
3.保存的设置用户ID是由exec复制有效用户ID而来的，如果设置了文件的设置用户ID，则在exec根据文件的用户ID设置了进程的有效用户ID后，将这个副本保存起来。
### 八、System函数
```cpp
#include <stdlib.h>
int system ( const char *cmdstring );
```
功能：调用fork产生子进程，由子进程来调用；/bin/sh -c command来执行参数command所代表的命令，阻塞当前进程直到command命令执行完毕。
因为system在其实现中调用了fork、exec和waitpid，因此有三种返回值：
（1）如果fork失败或者waitpid返回除EINTR之外的出错，则system返回-1，而且errno中设置了错误类型值。
（2）如果exec失败（表示不能执行shell），则其返回值如同shell执行了exit(127)一样，即返回127。
（3）否则所有三个函数（fork、exec和waitpid）都执行成功，并且system的返回值是shell的终止状态。
