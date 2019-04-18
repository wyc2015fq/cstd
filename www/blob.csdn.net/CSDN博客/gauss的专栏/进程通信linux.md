# 进程通信linux - gauss的专栏 - CSDN博客
2014年10月19日 12:44:30[gauss](https://me.csdn.net/mathlmx)阅读数：474
个人分类：[linux](https://blog.csdn.net/mathlmx/article/category/2303095)
# 1、进程间通信介绍
linux下的进程通信手段基本上是从Unix平台上的进程通信手段继承而来的。
对Unix发展做出重大贡献的两大主力AT&T的贝尔实验室及BSD（加州大学伯克利分校的伯克利软件发布中心）在进程间通信方面的侧重点有所不同。
1)前者对Unix早期的进程间通信手段进行了系统的改进和扩充，形成了“system V IPC”，通信进程局限在单个计算机内；
2)后者则跳过了该限制，形成了基于套接口（socket）的进程间通信机制。
Linux则把两者继承了下来，如图示：
![](https://img-blog.csdn.net/20140827135924239)
图一给出了linux 所支持的各种IPC手段，在本文接下来的讨论中，为了避免概念上的混淆，在尽可能少提及Unix的各个版本的情况下，所有问题的讨论最终都会归结到Linux环境下的进程间通信上来。并且，对于Linux所支持通信手段的不同实现版本（如对于共享内存来说，有Posix共享内存区以及System V共享内存区两个实现版本），将主要介绍Posix API。
进程通信有如下一些目的：
1）数据传输：一个进程需要将它的数据发送给另一个进程，发送的数据量在一个字节到几M字节之间
2）共享数据：多个进程想要操作共享数据，一个进程对共享数据的修改，别的进程应该立刻看到。
3）通知事件：一个进程需要向另一个或一组进程发送消息，通知它（它们）发生了某种事件（如进程终止时要通知父进程）。
4）资源共享：多个进程之间共享同样的资源。为了作到这一点，需要内核提供锁和同步机制。
5）进程控制：有些进程希望完全控制另一个进程的执行（如Debug进程），此时控制进程希望能够拦截另一个进程的所有陷入和异常，并能够及时知道它的状态改变。
进程间通信方式类型：
1）UNIX进程间通信方式：
管道、FIFO、信号。
早期UNIX进程间通信、基于System V进程间通信、基于Socket进程间通信和POSIX进程间通信。
2）System V进程间通信方式：
System V消息队列、System V信号灯、System V共享内存
3）POSIX进程间通信：
posix消息队列、posix信号灯、posix共享内存。
linux进程间通信方式(是以上的方式发展来的)：
1）管道（pipe）和有名管道（FIFO）
管道可用于具有亲缘关系进程间的通信，有名管道克服了管道没有名字的限制，因此，除具有管道所具有的功能外，它还允许无亲缘关系进程间的通信
2）信号（signal）
信号是比较复杂的通信方式，用于通知接受进程有某种事件发生，除了用于进程间通信外，进程还可以发送信号给进程本身；linux除了支持Unix早期信号语义函数sigal外，还支持语义符合Posix.1标准的信号函数sigaction（实际上，该函数是基于BSD的，BSD为了实现可靠信号机制，又能够统一对外接口，用sigaction函数重新实现了signal函数）
3）报文（Message）队列（消息队列）
消息队列是消息的链接表，包括Posix消息队列和system V消息队列。有足够权限的进程可以向队列中添加消息，被赋予读权限的进程则可以读走队列中的消息。
消息队列克服了信号承载信息量少，管道只能承载无格式字节流以及缓冲区大小受限等缺点。
4）共享内存
使得多个进程可以访问同一块内存空间，是最快的可用IPC形式。是针对其他通信机制运行效率较低而设计的。往往与其它通信机制，如信号量结合使用，来达到进程间的同步及互斥。
5）信号量
主要作为进程间以及同一进程不同线程之间的同步手段
6）套接字（socket)
更为一般的进程间通信机制，可用于不同机器之间的进程间通信。起初是由Unix系统的BSD分支开发出来的，但现在一般可以移植到其它类Unix系统上：Linux和System V的变种都支持套接字
ipc提供方：
|方法|提供方（操作系统或其他环境）|
|----|----|
|文件|多数操作系统|
|信号|多数操作系统|
|Berkeley套接字|多数操作系统|
|消息队列|多数操作系统|
|管道|所有的 POSIX 系统, Windows.|
|命名管道|所有的 POSIX 系统, Windows.|
|信号量|所有的 POSIX 系统, Windows.|
|共享内存|所有的 POSIX 系统, Windows.|
|Message passing(不共享)|用于 MPI规范，Java RMI, CORBA, MSMQ, MailSlot 以及其他.|
|Memory-mapped file|所有的 POSIX 系统, Windows.|
与直接共享内存地址空间的多线程编程相比，IPC的缺点：
1)采用了某种形式的内核开销，降低了性能;
2)几乎大部分IPC都不是程序设计的自然扩展，往往戏剧性增加了程序复杂度。
# 2、管道通信
普通的Linux shell都允许重定向，而重定向使用的就是管道。
例如：
ps | grep vsftpd .管道是单向的、先进先出的、无结构的、固定大小的字节流，它把一个进程的标准输出和另一个进程的标准输入连接在一起。写进程在管道的尾端写入数据，读进程在管道的道端读出数据。数据读出后将从管道中移走，其它读进程都不能再读到这些数据。管道提供了简单的流控制机制。进程试图读空管道时，在有数据写入管道前，进程将一直阻塞。同样，管道已经满时，进程再试图写管道，在其它进程从管道中移走数据之前，写进程将一直阻塞。管道主要用于不同进程间通信。
## （1）管道创建与关闭
创建一个简单的管道，可以使用系统调用pipe()。它接受一个参数，也就是一个包括两个整数的数组。如果系统调用成功，此数组将包括管道使用的两个文件描述符。创建一个管道之后，一般情况下进程将产生一个新的进程。
系统调用：pipe();
原型：int pipe(int fd[2]);
返回值：如果系统调用成功，返回0。如果系统调用失败返回-1：
errno=EMFILE(没有空亲的文件描述符）
      EMFILE（系统文件表已满）
      EFAULT（fd数组无效）
注意：fd[0]用于读取管道，fd[1]用于写入管道。
图见附件
## （2）管道的创建
**[cpp]**[view plain](http://blog.csdn.net/chenjiayi_yun/article/details/38867989#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/38867989#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/459283/fork)
- #include<unistd.h>
- #include<errno.h>
- #include<stdio.h>
- #include<stdlib.h>
- int main()  
- {  
- int pipe_fd[2];  
- if(pipe(pipe_fd)<0){  
-         printf("pipe create error\n");  
- return -1;  
-     }  
- else
-     printf("pipe create success\n");  
-     close(pipe_fd[0]);  
-     close(pipe_fd[1]);  
- return 0;  
- }  
## （3）管道的读写
管道主要用于不同进程间通信。实际上，通常先创建一个管道，再通过fork函数创建一个子进程。
管道读写注意事项：
可以通过打开两个管道来创建一个双向的管道。但需要在子理程中正确地设置文件描述符。必须在系统调用fork()中调用pipe()，否则子进程将不会继承文件描述符。当使用半双工管道时，任何关联的进程都必须共享一个相关的祖先进程。因为管道存在于系统内核之中，所以任何不在创建管道的进程的祖先进程之中的进程都将无法寻址它。而在命名管道中却不是这样。
管道实例见：
pipe_rw.c
**[cpp]**[view plain](http://blog.csdn.net/chenjiayi_yun/article/details/38867989#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/38867989#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/459283/fork)
- #include<unistd.h>
- #include<memory.h>
- #include<errno.h>
- #include<stdio.h>
- #include<stdlib.h>
- int main()  
- {  
- int pipe_fd[2];  
-     pid_t pid;  
- char buf_r[100];  
- char* p_wbuf;  
- int r_num;  
-     memset(buf_r,0,sizeof(buf_r));数组中的数据清0；  
- if(pipe(pipe_fd)<0){//创建管道
-         printf("pipe create error\n");  
- return -1;  
-     }  
- if((pid=fork())==0){//子进程（读者）
-         printf("\n");  
-         close(pipe_fd[1]);//子进程不需要管道写端
-         sleep(2);  
- if((r_num=read(pipe_fd[0],buf_r,100))>0){//从管道读取数据到缓冲区
-             printf("%d numbers read from be pipe is %s\n",r_num,buf_r);  
-         }  
-         close(pipe_fd[0]);//关闭管道读端
-         exit(0);  
-     }elseif(pid>0){//父进程(写者)
-         close(pipe_fd[0]);//父进程不需要管道读端
- 
- if(write(pipe_fd[1],"Hello",5)!=-1)  
-             printf("parent write success!\n");  
- 
- if(write(pipe_fd[1]," Pipe",5)!=-1)  
-             printf("parent wirte2 succes!\n");  
- 
-         close(pipe_fd[1]);//关闭管道写端
-         sleep(3);  
-         waitpid(pid,NULL,0);//暂时停止目前进程的执行，直到有信号来到或子进程结束
-         exit(0);  
-     }  
- return 0;  
- }  
# 3、标准流管道
与linux中文件操作有文件流的标准I/O一样，管道的操作也支持基于文件流的模式。
接口函数如下：
库函数：popen();
原型：FILE *open (char *command,char *type);
返回值：如果成功，返回一个新的文件流。如果无法创建进程或者管道，返回NULL。管道中数据流的方向是由第二个参数type控制的。此参数可以是r或者w，分别代表读或写。但不能同时为读和写。在Linux 系统下，管道将会以参数type中第一个字符代表的方式打开。所以，如果你在参数type中写入rw，管道将会以读的方式打开。
使用popen()创建的管道必须使用pclose()关闭。其实，popen/pclose和标准文件输入/输出流中的fopen()/fclose()十分相似。
库函数：pclose();
原型：int pclose(FILE *stream);
返回值：返回系统调用wait4()的状态。
如果stream无效，或者系统调用wait4()失败，则返回-1。注意此库函数等待管道进程运行结束，然后关闭文件流。库函数pclose()在使用popen（）创建的进程上执行wait4()函数，它将破坏管道和文件系统。
流管道的例子：
**[cpp]**[view plain](http://blog.csdn.net/chenjiayi_yun/article/details/38867989#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/38867989#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/459283/fork)
- #include<stdio.h>
- #include<unistd.h>
- #include<stdlib.h>
- #include<fcntl.h>
- #define BUFSIZE 1024
- 
- int main(){  
- FILE *fp;  
- char *cmd="ps -ef";  
- char buf[BUFSIZE];  
-     buf[BUFSIZE]='\0';  
- if((fp=popen(cmd,"r"))==NULL)  
-      perror("popen");  
- 
- while((fgets(buf,BUFSIZE,fp))!=NULL)  
-      printf("%s",buf);  
- 
-     pclose(fp);  
-     exit(0);  
- 
-     retutn 0;  
- }  
# 4、命名管道（FIFO）
基本概念
命名管道和一般的管道基本相同，但也有一些显著的不同：
1）命名管道是在文件系统中作为一个特殊的设备文件而存在的。
2）不同祖先的进程之间可以通过管道共享数据。
3）当共享管道的进程执行完所有的I/O操作以后，命名管道将继续保存在文件系统中以便以后使用。
管道只能由相关进程使用，它们共同的祖先进程创建了管道。但是，通过FIFO，不相关的进程也能交换数据。
命名管道创建与操作
命名管道创建
#include<sys/types.h>
#include<sys/stat.h>
int mkfifo(const char *pathname,mode_t mode);
返回：若成功则为0，若出错返回-1
一旦已经用mkfifo创建了一个FIFO，就可用open打开它。确实，一般的文件I/O函数(close,read,write,unlink等）都可用于FIFO。当打开一个FIFO时，非阻塞标（O_NONBLOCK）产生下列影响：
1）在一般情况中（没有说明O_NONBLOCK),只读打开要阻塞到某个其他进程为写打开此FIFO。类似，为写而打开一个FIFO要阻塞到某个其他进程为读而打开它。
2）如果指一了O_NONBLOCK，则只读打开立即返回。但是，如果没有进程已经为读而打开一个FIFO，那么只写打开将出错返回，其errno是ENXIO。类似于管道，若写一个尚无进程为读而打开的FIFO，则产生信号SIGPIPE。若某个FIFO的最后一个写进程关闭了该FIFO，则将为该FIFO的读进程产生一个文件结束标志。
FIFO相关出错信息：
EACCES（无存取权限）
EEXIST（指定文件不存在）
ENAMETOOLONG（路径名太长）
ENOENT（包含的目录不存在）
ENOSPC（文件系统余空间不足）
ENOTDIR（文件路径无效）
EROFS（指定的文件存在于只读文件系统中）
fifo_read.c //写者
**[cpp]**[view plain](http://blog.csdn.net/chenjiayi_yun/article/details/38867989#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/38867989#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/459283/fork)
- #include<sys/types.h>
- #include<sys/stat.h>
- #include<errno.h>
- #include<fcntl.h>
- #include<stdio.h>
- #include<stdlib.h>
- #include<string.h>
- 
- #define FIFO "/tmp/myfifo"
- 
- int main(int argc,char** argv)  
- {  
- char buf_r[100];  
- int fd;  
- int nread;  
- if((mkfifo(FIFO,O_CREAT|O_EXCL)<0)&&(errno!=EEXIST))//创建命名管道
-     {  
-         printf("cannot create fifoserver\n");  
- return -1;  
-     }  
- 
-     printf("Preparing for reading bytes....\n");  
-     memset(buf_r,0,sizeof(buf_r));//初始化缓冲区
-     fd=open(FIFO,O_RDONLY|O_NONBLOCK,0);//打开命名管道的读端
- if(fd==-1)  
-     {  
-         perror("open");  
-         exit(1);  
-     }  
- while(1){//一直从命名管道读数据
-         memset(buf_r,0,sizeof(buf_r));  
- if((nread=read(fd,buf_r,100))==-1){  
- if(errno==EAGAIN)  
-                 printf("no data yet\n");  
-         }  
-         printf("read %s from FIFO\n",buf_r);  
-         sleep(1);  
-     }  
-     pause();  
-     unlink(FIFO);//删除命名管道的文件
- return 0;  
- }  
fifo_write.c
**[cpp]**[view plain](http://blog.csdn.net/chenjiayi_yun/article/details/38867989#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/38867989#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/459283/fork)
- #include<sys/types.h>
- #include<sys/stat.h>
- #include<errno.h>
- #include<fcntl.h>
- #include<stdio.h>
- #include<stdlib.h>
- #include<string.h>
- 
- #define FIFO_SERVER "/tmp/myfifo"
- 
- main(int argc,char** argv)  
- {  
- if(argc==1)  
-     {  
-         printf("Please send something\n");  
- return -1;  
-     }  
- int fd;  
- char w_buf[100];  
- int nwrite;  
- 
-     fd=open(FIFO_SERVER,O_WRONLY|O_NONBLOCK,0);  
- if(fd==-1 )  
-     {  
- if(errno==ENXIO)printf("open error;no reading process\n");  
- return -1;  
-     }  
- 
-     strcpy(w_buf,argv[1]);//需要写的数据，拷贝到写缓冲区
- 
- if((nwrite=write(fd,w_buf,100))==-1)  
-     {//写有误
- if(errno==EAGAIN)//写缓存已满
-             printf("The FIFO has not been read yet. Please try later\n");  
-     }  
- else
-         printf("write %s to the FIFO\n",w_buf);  
- 
-     reutrn 0;  
- }  
# 5、信号
信号是软件中断。信号（signal）机制是Unix系统中最为古老的进程之间的能信机制。它用于在一个或多个进程之间传递异步信号。很多条件可以产生一个信号。
1）当用户按某些终端键时，产生信号。在终端上按DELETE键通常产生中断信号（SIGINT）。这是停止一个已失去控制程序的方法。
2）硬件异常产生信号：除数为0、无效的存储访问等等。这些条件通常由硬件检测到，并将其通知内核。然后内核为该条件发生时正在运行的进程产生适当的信号。例如，对于执行一个无效存储访问的进程产生一个SIGSEGV。
3）进程用kill(2)函数可将信号发送给另一个进程或进程组。自然，有些限制：接收信号进和发送信号进程的所有都必须相同，或发送信号进程的的所有者必须是超级用户。
4）用户可用Kill（ID 值）命令将信号发送给其它进程。此程序是Kill函数的界面。常用此命令终止一个失控的后台进程。
5）当检测到某种软件条件已经发生，并将其通知有关进程时也产生信号。这里并不是指硬件产生条件（如被0除），而是软件条件。例如SIGURG（在网络连接上传来非规定波特率的数据）、SIGPIPE（在管道的读进程已终止后一个进程写此管道），以及SIGALRM（进程所设置的闹钟时间已经超时）。
内核为进程生产信号，来响应不同的事件，这些事件就是信号源。
主要信号源如下：
1）异常：进程运行过程中出现异常
2）其它进程：一个进程可以向另一个或一组进程发送信号
3）终端中断：Ctrl-c,Ctro-\等
4）作业控制：前台、后台进程的管理
5）分配额：CPU超时或文件大小突破限制
6）通知：通知进程某事件发生，如I/O就绪等
7）报警：计时器到期
Linux中的信号
1、SIGHUP 2、SIGINT（终止） 3、SIGQUIT（退出） 4、SIGILL 5、SIGTRAP 6、SIGIOT  7、SIGBUS   8、SIGFPE   9、SIGKILL 10、SIGUSER 11、 SIGSEGV SIGUSER 12、 SIGPIPE 13、SIGALRM 14、SIGTERM 15、SIGCHLD 16、SIGCONT 17、SIGSTOP 18、SIGTSTP 19、SIGTTIN 20、SIGTTOU 21、SIGURG 22、SIGXCPU 23、SIGXFSZ
 24、SIGVTALRM 25、SIGPROF 26、SIGWINCH 27、SIGIO 28、SIGPWR
常用的信号：
SIGHUP：从终端上发出的结束信号
SIGINT：来自键盘的中断信号（Ctrl+c）
SIGQUIT：来自键盘的退出信号
SIGFPE：浮点异常信号（例如浮点运算溢出）
SIGKILL：该信号结束接收信号的进程
SIGALRM：进程的定时器到期时，发送该信号
SIGTERM：kill命令生出的信号
SIGCHLD：标识子进程停止或结束的信号
SIGSTOP：来自键盘（Ctrl-Z)或调试程序的停止扫行信号
可以要求系统在某个信号出现时按照下列三种方式中的一种进行操作。
1）忽略此信号：大多数信号都可使用这种方式进行处理，但有两种信号却决不能被忽略。它们是：SIGKILL和SIGSTOP。这两种信号不能被忽略的，原因是：它们向超级用户提供一种使进程终止或停止的可靠方法。另外，如果忽略某些由硬件异常产生的信号（例如非法存储访问或除以0），则进程的行为是示定义的。
2）捕捉信号：为了做到这一点要通知内核在某种信号发生时，调用一个用户函数。在用户函数中，可执行用户希望对这种事件进行的处理。如果捕捉到SIGCHLD信号，则表示子进程已经终止，所以此信号的捕捉函数可以调用waitpid以取得该子进程的进程ID以及它的终止状态。
3）执行系统默认动作：对大多数信号的系统默认动作是终止该进程。每一个信号都有一个缺省动作，它是当进程没有给这个信号指定处理程序时，内核对信号的处理。
有5种缺省的动作：
1）异常终止（abort)：在进程的当前目录下，把进程的地址空间内容、寄存器内容保存到一个叫做core的文件中，而后终止进程。
2）退出（exit）：不产生core文件，直接终止进程。
3）忽略（ignore）：忽略该信号。
4）停止（stop）：挂起该进程。
5）继续（contiune）：如果进程被挂起，刚恢复进程的动行。否则，忽略信号。
## （1）信号的发送与捕捉
kill()和raise()
kill()不仅可以中止进程，也可以向进程发送其他信号。
与kill函数不同的是，raise()函数运行向进程自身发送信号
#include<sys/types.h>
#include<signal.h>
int kill(pid_t pid,int signo);
int raise(int signo);
两个函数返回：若成功则为0，若出错则为-1。
kill的pid参数有四种不同的情况：
1）pid>0将信号发送给进程ID为pid的进程。
2）pid==0将信号发送给其进程组ID等于发送进程的进程组ID，而且发送进程有许可权向其发送信号的所有进程。
3）pid<0将信号发送给其进程组ID等于pid绝对值，而且发送进程有许可权向其发送信号的所有进程。如上所述一样，“所有进程”并不包括系统进程集中的进程。
4）pid==-1 POSIX.1未定义种情况
kill.c 
**[cpp]**[view plain](http://blog.csdn.net/chenjiayi_yun/article/details/38867989#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/38867989#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/459283/fork)
- #include<stdio.h>
- #include<stdlib.h>
- #include<signal.h>
- #include<sys/types.h>
- #include<sys/wait.h>
- 
- int main()  
- {  
-     pid_t pid;  
- int ret;  
- if((pid==fork())<0)//生成子进程
-     {  
-         perro("fork");  
-         exit(1);  
-     }  
- if(pid==0)//子进程
-     {  
-         raise(SIGSTOP);  
-         exit(0);  
-     }  
- else
-     {//父进程
-         printf("pid=%d\n",pid);  
- if((waitpid(pid,NULL,WNOHANG))==0)//等待子进程
-         {  
- if((ret=kill(pid,SIGKILL))==0) //发送杀死信号到子进程
-                 printf("kill %d\n",pid);  
- else
-             {  
-                 perror("kill");  
-             }  
-         }  
-     }  
- return 0;  
- }  
alarm和pause函数
使用alarm函数可以设置一个时间值（闹钟时间），在将来的某个时刻时间值会被超过。当所设置的时间被超过后，产生SIGALRM信号。如果不忽略或不捕捉引信号，则其默认动作是终止该进程。
#include<unistd.h>
unsigned int alarm(unsigned int secondss);
返回：0或以前设置的闹钟时间的余留秒数。
参数seconds的值是秒数，经过了指定的seconds秒后产生信号SIGALRM。每个进程只能有一个闹钟时间。如果在调用alarm时，以前已为该进程设置过闹钟时间，而且它还没有超时，则该闹钟时间的余留值作为本次alarm函数调用的值返回。以前登记的闹钟时间则被新值代换。
如果有以前登记的尚未超过的闹钟时间，而且seconds值是0，则取消以前的闹钟时间，其余留值仍作为函数的返回值。
pause函数使用调用进程挂起直至捕捉到一个信号
#include<unistd.h>
int pause(void);
返回：-1，errno设置为EINTR
只有执行了一信号处理程序并从其返回时，pause才返回。
alarm.c
**[cpp]**[view plain](http://blog.csdn.net/chenjiayi_yun/article/details/38867989#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/38867989#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/459283/fork)
- #include<unistd.h>
- #include<stdio.h>
- #include<stdlib.h>
- int main()  
- {  
- int ret;  
-     ret=alarm(5);  
-     pause();  
-     printf("I have been waken up.\n",ret);  
- }  
## （2）信号的处理
当系统捕捉到某个信号时，可以忽略谁信号或是使用指定的处理函数来处理该信号，或者使用系统默认的方式。信号处理的主要方式有两种，一种是使用简单的signal函数，别一种是使用信号集函数组。
signal()
#include<signal.h>
void (*signal (int signo,void (*func)(int)))(int)
返回：成功则为以前的信号处理配置，若出错则为SIG_ERR
func的值是：（a）常数SIGIGN，或（b）常数SIGDFL，或（c）当接到此信号后要调用的的函数的地址。如果指定SIGIGN，则向内核表示忽略此信号（有两个信号SIGKILL和SIGSTOP不能忽略）。如果指定SIGDFL，则表示接到此信号后的动作是系统默认动作。当指定函数地址时，我们称此为捕捉此信号。我们称此函数为信号处理程序(signal handler)或信号捕捉函数(signal-catching funcgion).signal函数原型太复杂了，如果使用下面的typedef,则可以使其简化。
type void sign(int);
sign *signal(int,handler *);
实例见:mysignal.c
**[cpp]**[view plain](http://blog.csdn.net/chenjiayi_yun/article/details/38867989#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/38867989#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/459283/fork)
- #include<signal.h>
- #include<stdio.h>
- #include<stdlib.h>
- 
- void my_func(int sign_no)  
- {  
- if(sign_no==SIGINT)//接收到的信号是SIGINT
-      printf("I have get SIGINT\n");  
- elseif(sign_no==SIGQUIT)//接收到的信号是SIGQUIT
-      printf("I have get SIGQUIT\n");  
- }  
- 
- int main()  
- {  
-  printf("Waiting for signal SIGINT or SIGQUTI\n");  
-  signal(SIGINT,my_func);//注册信号处理函数
-  signal(SIGQUIT,my_func);  
-  pasue();  
-  exit(0);  
- return 0;  
- }  
## （3）信号集函数组
我们需要有一个能表示多个信号——信号集（signal set）的数据类型。将在sigprocmask()这样的函数中使用这种数据类型，以告诉内核不允许发生该信号集中的信号。信号集函数组包含水量几大模块：创建函数集、登记信号集、检测信号集。
创建函数集
#include<signal.h>
int sigemptyset(sigset_t* set);
int sigfillset(sigset_t* set);
int sigaddset(sigset_t* set,int signo );
int sigdelset(sigset_t* set,int signo);
四个函数返回：若成功则为0，若出错则为-1
int sigismember(const sigset_t* set,int signo);
返回：若真则为1，若假则为0；
signemptyset:初始化信号集合为空。
sigfillset:初始化信号集合为所有的信号集合。
sigaddset:将指定信号添加到现存集中。
sigdelset:从信号集中删除指定信号。
sigismember:查询指定信号是否在信号集中。
登记信号集
登记信号处理机主要用于决定进程如何处理信号。首先要判断出当前进程阻塞能不能传递给该信号的信号集。这首先使用sigprocmask函数判断检测或更改信号屏蔽字，然后使用sigaction函数改变进程接受到特定信号之后的行为。
一个进程的信号屏蔽字可以规定当前阻塞而不能递送给该进程的信号集。调用函数sigprocmask可以检测或更改（或两者）进程的信号屏蔽字。
#include<signal.h>
int sigprocmask(int how,const sigset_t* set,sigset_t* oset);
返回：若成功则为0，若出错则为-1
oset是非空指针，进程是当前信号屏蔽字通过oset返回。其次，若set是一个非空指针，则参数how指示如何修改当前信号屏蔽字。
用sigprocmask更改当前信号屏蔽字的方法。how参数设定：
SIG_BLOCK该进程新的信号屏蔽字是其当前信号屏蔽字和set指向信号集的并集。set包含了我们希望阻塞的附加信号。
SIG_NUBLOCK该进程新的信号屏蔽字是其当前信号屏蔽字和set所指向信号集的交集。set包含了我们希望解除阻塞的信号。
SIG_SETMASK该进程新的信号屏蔽是set指向的值。如果set是个空指针，则不改变该进程的信号屏蔽字，how的值也无意义。
sigaction函数的功能是检查或修改（或两者）与指定信号相关联的处理动作。此函数取代了UNIX早期版本使用的signal函数。
#include<signal.h>
int sigaction(int signo,const struct sigaction* act,struct sigaction* oact);
返回：若成功则为0，若出错则为-1
参数signo是要检测或修改具体动作的信号的编号数。若act指针非空，则要修改其动作。如果oact指针为空，则系统返回该信号的原先动作。此函数使用下列结构：
**[cpp]**[view plain](http://blog.csdn.net/chenjiayi_yun/article/details/38867989#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/38867989#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/459283/fork)
- struct sigaction{//信号动作
- void (*sa_handler)(int signo);  
-     sigset_t sa_mask;  
- int sa_flags;  
- void (*sa_restore);  
- };  
sa_handler是一个函数指针，指定信号关联函数，可以是自定义处理函数，还可以SIG_DEF或SIG_IGN;
sa_mask是一个信号集，它可以指定在信号处理程序执行过程中哪些信号应当被阻塞。
sa_flags中包含许多标志位，是对信号进行处理的各种选项。具体如下：
SA_NODEFER\SA_NOMASK:当捕捉到此信号时，在执行其信号捕捉函数时，系统不会自动阻塞此信号。
SA_NOCLDSTOP:进程忽略子进程产生的任何SIGSTOP、SIGTSTP、SIGTTIN和SIGTOU信号
SA_RESTART:可让重启的系统调用重新起作用。
SA_ONESHOT\SA_RESETHAND:自定义信号只执行一次，在执行完毕后恢复信号的系统默认动作。
检测信号是信号处理的后续步骤，但不是必须的。sigpending函数运行进程检测“未决“信号（进程不清楚他的存在），并进一步决定对他们做何处理。
sigpending返回对于调用进程被阻塞不能递送和当前未决的信号集。
#include<signal.h>
int sigpending(sigset_t * set);
返回：若成功则为0，若出错则为-1
信号集实例见：sigaction.c
**[cpp]**[view plain](http://blog.csdn.net/chenjiayi_yun/article/details/38867989#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/38867989#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/459283/fork)
- #include<sys/types.h>
- #include<unistd.h>
- #include<signal.h>
- #include<stdio.h>
- #include<stdlib.h>
- 
- void my_func(int signum){  
-     printf("If you want to quit,please try SIGQUIT\n");  
- }  
- 
- int main()  
- {  
-     sigset_t set,pendset;//信号集
- struct sigaction action1,action2;  
- 
- if(sigemptyse(&set)<0)  
-         perror("sigemptyset");  
- if(sigaddset(&set,SIGQUIT)<0)  
-         perror("sigaddset");  
- if(sigaddset(&set,SIGINT)<0)  
-         perror("sigaddset");  
- if(sigprocmask(SIG_BLOCK,&set,NULL)<0)  
-         perror("sigprcmask");  
- else{  
-         printf("blocked\n");  
-         sleep(5);  
-     }  
- 
- if(sigprocmask(SIG_UNBLOCK,&set,NULL)  
-         perror("sigprocmask");  
- else
-         printf("unblock\n");  
- 
- while(1){  
- if(sigismember(&set,SIGINT))  
-         {  
-             sigemptyset(&action1.sa_mask);  
-             action1.sa_handler=my_func;  
-             sigaction(SIGINT,&action1,NULL);//注册信号动作（信号处理函数）
-         }  
- elseif(sigismember(&set,SIGQUIT))  
-         {  
-             sigemptyset(&action2.sa_mask);  
-             action2.sa_handler=SIG_DEL;  
-             sigaction(SIGTERM,&action2,NULL);  
-         }  
-     }  
- return 0;  
- }  
# 6、信号量    
信号量（英语：Semaphore）又称为信号量、旗语，它以一个整数变量，提供信号，以确保在并行计算环境中，不同进程在访问共享资源时，不会发生冲突。是一种不需要使用忙碌等待（busy waiting）的一种方法。
信号量的概念是由荷兰计算机科学家艾兹格·迪杰斯特拉（Edsger W. Dijkstra）发明的，广泛的应用于不同的操作系统中。在系统中，给予每一个进程一个信号量，代表每个进程目前的状态，未得到控制权的进程会在特定地方被强迫停下来，等待可以继续进行的信号到来。如果信号量是一个任意的整数，通常被称为计数信号量（Counting semaphore），或一般信号量（general
 semaphore）；如果信号量只有二进制的0或1，称为二进制信号量（binary semaphore）。在linux系中，二进制信号量（binary semaphore）又称Mutex。
计数信号量具备两种操作动作，之前称为 V（又称signal()）与 P（wait()）。 V操作会增加信号量 S的数值，P操作会减少它。
运作方式：
1）初始化，给与它一个非负数的整数值。
2）运行 P（wait()），信号量S的值将被减少。企图进入临界区段的进程，需要先运行 P（wait()）。当信号量S减为负值时，进程会被挡住，不能继续；当信号量S不为负值时，进程可以获准进入临界区段。
3）运行 V（又称signal()），信号量S的值会被增加。退出离开临界区段的进程，将会运行 V（又称signal()）。当信号量S不为负值时，先前被挡住的其他进程，将可获准进入临界区段。
## (1)信号量api
　　信号量本质上是一个非负的整数计数器，它被用来控制对公共资源的访问。当公共资源增加时，调用函数sem_post（）增加信号量。只有当信号量值大于０时，才能使用公共资源，使用后，函数sem_wait（）减少信号量。函数sem_trywait（）和函数pthread_ mutex_trylock（）起同样的作用，它是函数sem_wait（）的非阻塞版本。它们都在头文件/usr/include/semaphore.h中定义。
　　信号量的数据类型为结构sem_t，它本质上是一个长整型的数。函数sem_init（）用来初始化一个信号量。它的原型为：
　　extern int sem_init __P ((sem_t *__sem, int __pshared,unsigned int __value));
　　sem为指向信号量结构的一个指针；pshared不为０时此信号量在进程间共享，否则只能为当前进程的所有线程共享；value给出了信号量的初始值。
　　函数sem_post( sem_t *sem )用来增加信号量的值。当有线程阻塞在这个信号量上时，调用这个函数会使其中的一个线程不在阻塞，选择机制同样是由线程的调度策略决定的。
函数sem_wait( sem_t *sem )被用来阻塞当前线程直到信号量sem的值大于0，解除阻塞后将sem的值减一，表明公共资源经使用后减少。
函数sem_trywait ( sem_t *sem )是函数sem_wait（）的非阻塞版本，它直接将信号量sem的值减一。
　　函数sem_destroy(sem_t *sem)用来释放信号量sem。
## (2)信号量使用
例子中一共有4个线程，其中两个线程负责从文件读取数据到公共的缓冲区，另两个线程从缓冲区读取数据作不同的处理（加和乘运算）。
**[cpp]**[view plain](http://blog.csdn.net/chenjiayi_yun/article/details/38867989#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/38867989#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/459283/fork)
- /* File sem.c */
- #include <stdio.h>
- #include <pthread.h>
- #include <semaphore.h>
- 
- #define MAXSTACK 100
- int stack[MAXSTACK][2];//共享缓存
- int size=0;  
- sem_t sem;  
- 
- /* 从文件1.dat读取数据*/
- void ReadData1(void)  
- {  
- FILE*fp=fopen("1.dat","r");  
- while(!feof(fp))  
-     {  
-         fscanf(fp,"%d%d",&stack[size][0],&stack[size][1]);  
-         sem_post(&sem);//信号量加1
-         __sync_fetch_and_add(&size,1);  
-     }  
-     fclose(fp);  
- }  
- 
- /*从文件2.dat读取数据*/
- void ReadData2(void)  
- {  
- FILE*fp=fopen("2.dat","r");  
- while(!feof(fp))  
-     {  
-         fscanf(fp,"%d%d",&stack[size][0],&stack[size][1]);  
-         sem_post(&sem);//信号量加1
-      __sync_fetch_and_add(&size,1);  
- }fclose(fp);}  
- 
- 
- /*阻塞等待缓冲区的数据*/
- void HandleData1(void){while(1)  
- {  
-     sem_wait(&sem);//信号量减1
-     printf("Plus:%d+%d=%d\n",stack[size][0],stack[size][1],stack[size][0]+stack[size][1]);//读取缓冲区数据
-     __sync_fetch_and_sub(&size,1);}  
- }  
- 
- /*阻塞等待缓冲区的数据*/void HandleData2(void)  
- {  
- while(1)  
-     {  
-         sem_wait(&sem);//信号量减1
-         printf("Multiply:%d*%d=%d\n",  
-         stack[size][0],stack[size][1],stack[size][0]*stack[size][1]);//读取缓冲区数据
-       __sync_fetch_and_sub(&size,1);  
-   }  
- }  
- 
- int main(int argc,char** argc)  
- {  
-     pthread_t t1,t2,t3,t4;  
-     sem_init(&sem,0,0);//初始化信号量，初始值是0
-     pthread_create(&t1,NULL,(void*)HandleData1,NULL);  
-     pthread_create(&t2,NULL,(void*)HandleData2,NULL);  
-     pthread_create(&t3,NULL,(void*)ReadData1,NULL);  
-     pthread_create(&t4,NULL,(void*)ReadData2,NULL);  
- /* 连接程序*/
-     pthread_join(t1,NULL);  
-     pthread_join(t2,NULL);  
-     pthread_join(t3,NULL);  
-     pthread_join(t4,NULL);  
- 
- return 0;  
- }  
在Linux下，用命令gcc -lpthread sem.c -o sem生成可执行文件sem。 
编辑数据文件1.dat和2.dat，内容分别为1 2 3 4 5 6 7 8 9 10和 -1 -2 -3 -4 -5 -6 -7 -8 -9 -10 
运行./sem，结果如下：
Multiply:-1*-2=2
Plus:-1+-2=-3
Multiply:9*10=90
Plus:-9+-10=-19
Multiply:-7*-8=56
Plus:-5+-6=-11
Multiply:-3*-4=12
Plus:9+10=19
Plus:7+8=15
Plus:5+6=11
可以看出各个线程间的竞争关系。
数值并未按我们原先的顺序显示出来这是由于size这个数值被各个线程修改的缘故。
# 7、共享内存
共享内存是被多个进程共享的一部分物理内存。共享内存是进程间共享数据的一种最快的方法，一个进程向共享内存区域写入了数据，共享这个内存区域的所有进程就可以立刻看到其中的内容。
## （1）共享内存使用步骤
共享内存的实现分为两个步骤：
1） 创建共享内存，使用shmget函数。
2） 映射共享内存，将这段创建的共享内存映射到具体的进程空间去，使用shmat函数。
共享内存
int shmget(key_t key ,int size,int shmflg)
key标识共享内存的键值：0/IPC_PRIVATE 。
当key的取值为IPC_PRIVATE,则函数shmget将创建一块新的共享内存；如果key的取值为0，而参数中又设置了IPC_PRIVATE这个标志，则同样会创建一块新的共享内存。
size ：是这块内存的大小
shmflg： 是这块内存的模式(mode)以及权限标识:      
IPC_CREAT 新建（如果已创建则返回目前共享内存的id）
IPC_EXCL   与IPC_CREAT结合使用，如果已创建则则返回错误
然后将“模式” 和“权限标识”进行“或”运算，做为第三个参数。
如：    IPC_CREAT | IPC_EXCL | 0640   
例子中的0666为权限标识,4/2/1 分别表示读/写/执行3种权限，第一个0是UID,第一个6（4+2）表示拥有者的权限，第二个4表示同组权限，第3个0表示他人的权限。
：如0666为权限标识,4/2/1 分别表示读/写/执行3种权限，第一个0是UID,第一个6（4+2）表示拥有者的权限，第二个4表示同组权限，第3个0表示他人的权限。
返回值：如果成功，返回共享内存表示符，如果失败，返回-1。
映射共享内存
int shmat(int shmid,char *shmaddr，int flag)
参数：
shmid:shmget函数返回的共享存储标识符
flag：决定以什么样的方式来确定映射的地址(通常为0)
返回值：如果成功，则返回共享内存映射到进程中的地址；如果失败，则返回-1。
共享内存解除映射
当一个进程不再需要共享内存时，需要把它从进程地址空间中移除。
int shmdt(char *shmaddr)
实例如下：
创建两个进程，在A进程中创建一个共享内存，并向其写入数据，通过B进程从共享内存中读取数据。
chm_com.h函数
**[cpp]**[view plain](http://blog.csdn.net/chenjiayi_yun/article/details/38867989#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/38867989#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/459283/fork)
- #define TEXT_SZ 2048  
- 
- struct shared_use_st    
- {    
- int written_by_you;    
- char some_text[TEXT_SZ];    
- };    
- 
- #define  SHARE_KEY (1234)
读进程：read.c
**[cpp]**[view plain](http://blog.csdn.net/chenjiayi_yun/article/details/38867989#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/38867989#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/459283/fork)
- /********************************************************** 
-    本程序申请和分配共享内存，然后轮训并读取共享中的数据，直至 
- *           读到“end”。 
- **********************************************************/
- #include <unistd.h>  
- #include <stdlib.h>  
- #include <stdio.h>  
- #include <string.h>  
- #include <sys/types.h>  
- #include <sys/ipc.h>  
- #include <sys/shm.h>  
- #include "shm_com.h"  
- 
- /* 
-  * 程序入口 
-  * */
- int main(void)    
- {    
- int running=1;    
- void *shared_memory=(void *)0;    
- struct shared_use_st *shared_stuff;    
- int shmid;    
- /*创建共享内存*/
-     shmid=shmget((key_t)SHARE_KEY ,sizeof(struct shared_use_st),0666|IPC_CREAT);    
- if(shmid==-1)    
-     {    
-         fprintf(stderr,"shmget failed\n");    
-         exit(EXIT_FAILURE);    
-     }    
- 
- /*映射共享内存*/
-     shared_memory=shmat(shmid,(void *)0,0);    
- if(shared_memory==(void *)-1)    
-     {    
-         fprintf(stderr,"shmat failed\n");    
-         exit(EXIT_FAILURE);    
-     }    
-     printf("Memory attached at %X\n",(int)shared_memory);    
- 
- /*让结构体指针指向这块共享内存*/
-     shared_stuff=(struct shared_use_st *)shared_memory;    
- 
- /*控制读写顺序*/
-     shared_stuff->written_by_you=0;    
- /*循环的从共享内存中读数据，直到读到“end”为止*/
- while(running)    
-     {    
- if(shared_stuff->written_by_you)    
-        {    
-            printf("You wrote:%s",shared_stuff->some_text);    
-            sleep(1);  //读进程睡一秒，同时会导致写进程睡一秒，这样做到读了之后再写  
-            shared_stuff->written_by_you=0;    
- if(strncmp(shared_stuff->some_text,"end",3)==0)    
-            {    
-                running=0; //结束循环  
-            }    
-        }    
-     }    
- /*删除共享内存*/
- if(shmdt(shared_memory)==-1)    
-     {    
-         fprintf(stderr,"shmdt failed\n");    
-         exit(EXIT_FAILURE);    
-     }    
-        exit(EXIT_SUCCESS);    
- }    
写进程：write.c
**[cpp]**[view plain](http://blog.csdn.net/chenjiayi_yun/article/details/38867989#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/38867989#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/459283/fork)
- /********************************************************** 
- 本程序申请了上一段程序相同的共享内存块，然后循环向共享中 
- *           写数据，直至写入“end”。 
- **********************************************************/
- #include <unistd.h>  
- #include <stdlib.h>  
- #include <stdio.h>  
- #include <string.h>  
- #include <sys/types.h>  
- #include <sys/ipc.h>  
- #include <sys/shm.h>  
- #include "shm_com.h"  
- 
- /* 
-  * 程序入口 
-  * */
- int main(void)    
- {    
- int running=1;    
- void *shared_memory=(void *)0;    
- struct shared_use_st *shared_stuff;    
- char buffer[BUFSIZ];    
- int shmid;    
- /*创建共享内存*/
-     shmid=shmget((key_t)SHARE_KEY ,sizeof(struct shared_use_st),0666|IPC_CREAT);    
- if(shmid==-1)    
-     {    
-         fprintf(stderr,"shmget failed\n");    
-         exit(EXIT_FAILURE);    
-     }    
- 
- /*映射共享内存*/
-     shared_memory=shmat(shmid,(void *)0,0);    
- if(shared_memory==(void *)-1)    
-     {    
-         fprintf(stderr,"shmat failed\n");    
-         exit(EXIT_FAILURE);    
-     }    
-     printf("Memory attached at %X\n",(int)shared_memory);    
- 
- /*让结构体指针指向这块共享内存*/
-     shared_stuff=(struct shared_use_st *)shared_memory;    
- /*循环的向共享内存中写数据，直到写入的为“end”为止*/
- while(running)    
-     {    
- while(shared_stuff->written_by_you==1)    
-         {    
-             sleep(1);//等到读进程读完之后再写  
-             printf("waiting for client...\n");    
-         }    
-         printf("Ener some text:");    
-         fgets(buffer,BUFSIZ,stdin);    
-         strncpy(shared_stuff->some_text,buffer,TEXT_SZ);    
-         shared_stuff->written_by_you=1;    
- if(strncmp(buffer,"end",3)==0)    
-         {    
-             running=0;  //结束循环  
-         }    
-     }    
- /*删除共享内存*/
- if(shmdt(shared_memory)==-1)    
-     {    
-         fprintf(stderr,"shmdt failed\n");    
-         exit(EXIT_FAILURE);    
-     }    
-     exit(EXIT_SUCCESS);    
- }    
在一个终端中运行read，在另一个终端中运行write.当read运行起来之后，由于共享内存中没有数据可读，会处于等待状态
[root@localhost]# ./read
Memory attached at B7F9A000
向shm2运行的终端输入字符串
[root@localhost 2-4-4]# ./write
Memory attached at B7FD8000
Enter some text：Impossible is nothing
waiting for client。。。
waiting for client。。。
Enter some text：Anything is possible
waiting for client。。。
Ener some text：end
read能够逐个从共享内存中巴他们读出来，知道双方晕倒字符串"end"后，两个程序都退出。
[root@localhost]# ./read
Memory attached at B7F9A000
You write：Impossible is nothing
You write：Anything is possible
You write：end
## （2）查看共享内存
程序中在调用shmget函数时指定key参数值为IPC_PRIVATE，这个参数的意义是创建一个新的共享内存区，
创建成功后使用shell命令ipcs来显示系统下共享内存的状态。命令参数-m为只显示共享内存的状态。
**[cpp]**[view plain](http://blog.csdn.net/chenjiayi_yun/article/details/38867989#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/38867989#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/459283/fork)
- #include <sys/types.h>
- #include <sys/ipc.h>
- #include <sys/shm.h>
- #include <stdlib.h>
- #include <stdio.h>
- #define BUFSZ 4096
- int main ( void )  
- {  
- int shm_id; /*共享内存标识符*/
-     shm_id=shmget(IPC_PRIVATE, BUFSZ,0666);//使用IPC_PRIVATE会创建新的共享内存块
- //shm_id=shmget(999,BUFSZ,0666|O_CREAT);
- if (shm_id < 0 )   
-     { /*创建共享内存*/
-         perror( "shmget" ) ;  
-         exit ( 1 );  
-     }  
-     printf ( "successfully created segment : %d \n", shm_id ) ;  
-     system( "ipcs -m"); /*调用ipcs命令查看IPC*/
-     exit( 0 );  
- }  
调用了系统shell命令ipcs –m来查看当前系统IPC状态
运行结果：
$./ create_shm
successfully created segment : 2752516
------ Shared Memory Segments --------
key shmid owner perms bytes nattch status
0x0000000 66root 600 393216 2 dest
0x00000 5209root 666 4096 0
0x0056a4d5 2686978 root 600 488 1
0x0056a4d6 2719747 root 600 131072 1
root 666 4096 0 
