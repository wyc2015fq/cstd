# wait && waitpid - ljx0305的专栏 - CSDN博客
2008年09月09日 14:57:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1110标签：[null																[linux																[工作																[编程](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=工作&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)
个人分类：[Linux程序及使用](https://blog.csdn.net/ljx0305/article/category/394700)
1 wait
**1.1 简介**
wait的函数原型是： 
#include <sys/types.h> /* 提供类型pid_t的定义 */#include <sys/wait.h>pid_t wait(int *status) 
进程一旦调用了wait，就立即阻塞自己，由wait自动分析是否当前进程的某个子进程已经退出，如果让它找到了这样一个已经变成僵尸的子进程，wait就会收集这个子进程的信息，并把它彻底销毁后返回；如果没有找到这样一个子进程，wait就会一直阻塞在这里，直到有一个出现为止。 
参数status用来保存被收集进程退出时的一些状态，它是一个指向int类型的指针。但如果我们对这个子进程是如何死掉的毫不在意，只想把这个僵尸进程消灭掉，（事实上绝大多数情况下，我们都会这样想），我们就可以设定这个参数为NULL，就象下面这样： 
pid = wait(NULL); 
如果成功，wait会返回被收集的子进程的进程ID，如果调用进程没有子进程，调用就会失败，此时wait返回-1，同时errno被置为ECHILD。 
**1.2 实战**
下面就让我们用一个例子来实战应用一下wait调用，程序中用到了系统调用fork，如果你对此不大熟悉或已经忘记了，请参考上一篇文章《进程管理相关的系统调用（一）》。 
/* wait1.c */#include <sys/types.h>#include <sys/wait.h>#include <unistd.h>#include <stdlib.h>main(){pid_t pc,pr;pc=fork();if(pc<0) /* 如果出错 */printf("error ocurred!/n");else if(pc==0){/* 如果是子进程 */ printf("This is child process with pid of %d/n",getpid());sleep(10);/* 睡眠10秒钟 */}else{/* 如果是父进程 */pr=wait(NULL);/* 在这里等待 */printf("I catched a child process with pid of %d/n"),pr);}exit(0);} 
编译并运行: 
$ cc wait1.c -o wait1$ ./wait1This is child process with pid of 1508I catched a child process with pid of 1508 
可以明显注意到，在第2行结果打印出来前有10秒钟的等待时间，这就是我们设定的让子进程睡眠的时间，只有子进程从睡眠中苏醒过来，它才能正常退出，也就才能被父进程捕捉到。其实这里我们不管设定子进程睡眠的时间有多长，父进程都会一直等待下去，读者如果有兴趣的话，可以试着自己修改一下这个数值，看看会出现怎样的结果。 
**1.3 参数status**
如果参数status的值不是NULL，wait就会把子进程退出时的状态取出并存入其中，这是一个整数值（int），指出了子进程是正常退出还是被非正常结束的（一个进程也可以被其他进程用信号结束，我们将在以后的文章中介绍），以及正常结束时的返回值，或被哪一个信号结束的等信息。由于这些信息被存放在一个整数的不同二进制位中，所以用常规的方法读取会非常麻烦，人们就设计了一套专门的宏（macro）来完成这项工作，下面我们来学习一下其中最常用的两个： 
1，WIFEXITED(status) 这个宏用来指出子进程是否为正常退出的，如果是，它会返回一个非零值。 
（请注意，虽然名字一样，这里的参数status并不同于wait唯一的参数--指向整数的指针status，而是那个指针所指向的整数，切记不要搞混了。） 
2，WEXITSTATUS(status) 当WIFEXITED返回非零值时，我们可以用这个宏来提取子进程的返回值，如果子进程调用exit(5)退出，WEXITSTATUS(status)就会返回5；如果子进程调用exit(7)，WEXITSTATUS(status)就会返回7。请注意，如果进程不是正常退出的，也就是说，WIFEXITED返回0，这个值就毫无意义。 
下面通过例子来实战一下我们刚刚学到的内容： 
/* wait2.c */#include <sys/types.h>#include <sys/wait.h>#include <unistd.h>main(){int status;pid_t pc,pr;pc=fork();if(pc<0)/* 如果出错 */printf("error ocurred!/n");else if(pc==0){/* 子进程 */printf("This is child process with pid of %d./n",getpid());exit(3);/* 子进程返回3 */}else{/* 父进程 */pr=wait(&status);if(WIFEXITED(status)){/* 如果WIFEXITED返回非零值 */printf("the child process %d exit normally./n",pr);printf("the return code is %d./n",WEXITSTATUS(status));}else/* 如果WIFEXITED返回零 */printf("the child process %d exit abnormally./n",pr);}} 
编译并运行: 
$ cc wait2.c -o wait2$ ./wait2This is child process with pid of 1538.the child process 1538 exit normally.the return code is 3. 
父进程准确捕捉到了子进程的返回值3，并把它打印了出来。 
当然，处理进程退出状态的宏并不止这两个，但它们当中的绝大部分在平时的编程中很少用到，就也不在这里浪费篇幅介绍了，有兴趣的读者可以自己参阅Linux man pages去了解它们的用法。 
2 waitpid
**2.1 简介**
waitpid系统调用在Linux函数库中的原型是： 
#include <sys/types.h> /* 提供类型pid_t的定义 */#include <sys/wait.h>pid_t waitpid(pid_t pid,int *status,int options) 
从本质上讲，系统调用waitpid和wait的作用是完全相同的，但waitpid多出了两个可由用户控制的参数pid和options，从而为我们编程提供了另一种更灵活的方式。下面我们就来详细介绍一下这两个参数： 
**pid**
从参数的名字pid和类型pid_t中就可以看出，这里需要的是一个进程ID。但当pid取不同的值时，在这里有不同的意义。 
pid>0时，只等待进程ID等于pid的子进程，不管其它已经有多少子进程运行结束退出了，只要指定的子进程还没有结束，waitpid就会一直等下去。 pid=-1时，等待任何一个子进程退出，没有任何限制，此时waitpid和wait的作用一模一样。 pid=0时，等待同一个进程组中的任何子进程，如果子进程已经加入了别的进程组，waitpid不会对它做任何理睬。 pid<-1时，等待一个指定进程组中的任何子进程，这个进程组的ID等于pid的绝对值。 **options**
options提供了一些额外的选项来控制waitpid，目前在Linux中只支持WNOHANG和WUNTRACED两个选项，这是两个常数，可以用"|"运算符把它们连接起来使用，比如： 
ret=waitpid(-1,NULL,WNOHANG | WUNTRACED); 
如果我们不想使用它们，也可以把options设为0，如： 
ret=waitpid(-1,NULL,0); 
如果使用了WNOHANG参数调用waitpid，即使没有子进程退出，它也会立即返回，不会像wait那样永远等下去。 
而WUNTRACED参数，由于涉及到一些跟踪调试方面的知识，加之极少用到，这里就不多费笔墨了，有兴趣的读者可以自行查阅相关材料。 
看到这里，聪明的读者可能已经看出端倪了--wait不就是经过包装的waitpid吗？没错，察看<内核源码目录>/include/unistd.h文件349-352行就会发现以下程序段： 
static inline pid_t wait(int * wait_stat){return waitpid(-1,wait_stat,0);} 
**2.2 返回值和错误**
waitpid的返回值比wait稍微复杂一些，一共有3种情况： 
当正常返回的时候，waitpid返回收集到的子进程的进程ID； 如果设置了选项WNOHANG，而调用中waitpid发现没有已退出的子进程可收集，则返回0； 如果调用中出错，则返回-1，这时errno会被设置成相应的值以指示错误所在； 当pid所指示的子进程不存在，或此进程存在，但不是调用进程的子进程，waitpid就会出错返回，这时errno被设置为ECHILD； 
/* waitpid.c */#include <sys/types.h>#include <sys/wait.h>#include <unistd.h>main(){pid_t pc, pr;pc=fork();if(pc<0)/* 如果fork出错 */printf("Error occured on forking./n");else if(pc==0){/* 如果是子进程 */sleep(10);/* 睡眠10秒 */exit(0);}/* 如果是父进程 */do{pr=waitpid(pc, NULL, WNOHANG);/* 使用了WNOHANG参数，waitpid不会在这里等待 */if(pr==0){/* 如果没有收集到子进程 */printf("No child exited/n");sleep(1);}}while(pr==0);/* 没有收集到子进程，就回去继续尝试 */if(pr==pc)printf("successfully get child %d/n", pr);elseprintf("some error occured/n");} 
编译并运行： 
$ cc waitpid.c -o waitpid$ ./waitpidNo child exitedNo child exitedNo child exitedNo child exitedNo child exitedNo child exitedNo child exitedNo child exitedNo child exitedNo child exitedsuccessfully get child 1526 
父进程经过10次失败的尝试之后，终于收集到了退出的子进程。 
因为这只是一个例子程序，不便写得太复杂，所以我们就让父进程和子进程分别睡眠了10秒钟和1秒钟，代表它们分别作了10秒钟和1秒钟的工作。父子进程都有工作要做，父进程利用工作的简短间歇察看子进程的是否退出，如退出就收集它。            
