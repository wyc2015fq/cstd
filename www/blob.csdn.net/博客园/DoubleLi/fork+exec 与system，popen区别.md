# fork+exec 与system，popen区别 - DoubleLi - 博客园






**1、fork + exec**

fork用来创建一个子进程。一个程序一调用fork函数，系统就为一个新的进程准备了前述三个段，首先，系统让新的进程与旧的进程使用同一个代码段，因为它们的程序还是相同的，对于数据段和堆栈段，系统则复制一份给新的进程，这样，父进程的所有数据都可以留给子进程，但是，子进程一旦开始运行，虽然它继承了父进程的一切数据，但实际上数据却已经分开，相互之间不再有影响了，也就是说，它们之间不再共享任何数据了。而如果两个进程要共享什么数据的话，就要使用另一套函数（shmget，shmat，shmdt等）来操作。现在，已经是两个进程了，对于父进程，fork函数返回了子程序的进程号，而对于子程序，fork函数则返回零，这样，对于程序，只要判断fork函数的返回值，就知道自己是处于父进程还是子进程中。

事实上，目前大多数的unix系统在实现上并没有作真正的copy。一般的，CPU都是以“页”为单位分配空间的，象INTEL的CPU，其一页在通常情况下是4K字节大小，而无论是数据段还是堆栈段都是由许多“页”构成的，fork函数复制这两个段，只是“逻辑”上的，并非“物理”上的，也就是说，实际执行fork时，物理空间上两个进程的数据段和堆栈段都还是共享着的，当有一个进程写了某个数据时，这时两个进程之间的数据才有了区别，系统就将有区别的“页”从物理上也分开。系统在空间上的开销就可以达到最小。

vfork和fork一样，也是创建一个子进程，但是它并不将父进程的地址空间完全复制到子进程中，不会复制页表。因为子进程会立即调用exec，于是也就不会存放该地址空间。不过在子进程中调用exec或exit之前，他在父进程的空间中运行。

为什么会有vfork，因为以前的fork当它创建一个子进程时，将会创建一个新的地址空间，并且拷贝父进程的资源，而往往在子进程中会执行exec调用，这样，前面的拷贝工作就是白费力气了，这种情况下，聪明的人就想出了vfork，它产生的子进程刚开始暂时与父进程共享地址空间（其实就是线程的概念了），因为这时候子进程在父进程的地址空间中运行，所以子进程不能进行写操作，并且在儿子“霸占”着老子的房子时候，要委屈老子一下了，让他在外面歇着（阻塞），一旦儿子执行了exec或者exit后，相当于儿子买了自己的房子了，这时候就相当于分家了。

vfork和fork之间的另一个区别是： vfork保证子进程先运行，在她调用exec或exit之后父进程才可能被调度运行。如果在调用这两个函数之前子进程依赖于父进程的进一步动作，则会导致死锁。

由此可见，这个系统调用是用来启动一个新的应用程序。其次，子进程在vfork()返回后直接运行在父进程的栈空间，并使用父进程的内存和数据。这意味着子进程可能破坏父进程的数据结构或栈，造成失败。

为了避免这些问题，需要确保一旦调用vfork()，子进程就不从当前的栈框架中返回，并且如果子进程改变了父进程的数据结构就不能调用exit函数。子进程还必须避免改变全局数据结构或全局变量中的任何信息，因为这些改变都有可能使父进程不能继续。

通常，如果应用程序不是在fork()之后立即调用exec()，就有必要在fork()被替换成vfork()之前做仔细的检查。

用fork函数创建子进程后，子进程往往要调用一种exec函数以执行另一个程序，当进程调用一种exec函数时，该进程完全由新程序代换，而新程序则从其main函数开始执行，因为调用exec并不创建新进程，所以前后的进程id 并未改变，exec只是用另一个新程序替换了当前进程的正文，数据，堆和栈段。

一个进程一旦调用exec类函数，它本身就“死亡”了，系统把代码段替换成新的程序的代码，废弃原有的数据段和堆栈段，并为新程序分配新的数据段与堆栈段，唯一留下的，就是进程号，也就是说，对系统而言，还是同一个进程，不过已经是另一个程序了。不过exec类函数中有的还允许继承环境变量之类的信息，这个通过exec系列函数中的一部分函数的参数可以得到。



**2、system**

system 可以看做是fork + execl + waitpid。system()函数功能强大，很多人用却对它的原理知之甚少先看linux版system函数的源码：


#include 
#include 
#include 
#include

int system(const char * cmdstring)

{
    pid_t pid;
    int status;

    if(cmdstring == NULL){

         return (1);
    }


    if((pid = fork())<0){

            status = -1;
    }


    else if(pid = 0){

        execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);

        -exit(127); //子进程正常执行则不会执行此语句

        }

    else{

            while(waitpid(pid, &status, 0) < 0){

                if(errno != EINTER){

                    status = -1;

                    break;

                }

            }

        }

        return status;

}


先分析一下原理，然后再看上面的代码大家估计就能看懂了：  
    当system接受的命令为NULL时直接返回，否则fork出一个子进程，因为fork在两个进程：父进程和子进程中都返回，这里要检查返回的pid，fork在子进程中返回0，在父进程中返回子进程的pid，父进程使用waitpid等待子进程结束，子进程则是调用execl来启动一个程序代替自己，execl("/bin/sh", "sh", "-c", cmdstring, (char*)0)是调用shell，这个shell的路径是/bin/sh，后面的字符串都是参数，然后子进程就变成了一个shell进程，这个shell的参数是cmdstring，就是system接受的参数。在windows中的shell是command，想必大家很熟悉shell接受命令之后做的事了。

如果上面的你没有看懂，那我再解释下fork的原理：当一个进程A调用fork时，系统内核创建一个新的进程B，并将A的内存映像复制到B的进程空间中，因为A和B是一样的，那么他们怎么知道自己是父进程还是子进程呢，看fork的返回值就知道，上面也说了fork在子进程中返回0，在父进程中返回子进程的pid。

    windows中的情况也类似，就是execl换了个又臭又长的名字，参数名也换的看了让人发晕的，我在MSDN中找到了原型，给大家看看：

HINSTANCE   ShellExecute(
               HWND   hwnd,
               LPCTSTR   lpVerb,
               LPCTSTR   lpFile,
               LPCTSTR   lpParameters,
               LPCTSTR   lpDirectory,
               INT   nShowCmd
   );  

      用法见下：

     ShellExecute(NULL,   "open",   "c://a.reg",   NULL,   NULL,   SW_SHOWNORMAL);  


    你也许会奇怪 ShellExecute中有个用来传递父进程环境变量的参数 lpDirectory，linux中的 execl却没有，这是因为execl是编译器的函数（在一定程度上隐藏具体系统实现），在linux中它会接着产生一个linux系统的调用 execve, 原型见下：
    int execve(const char * file,const char **argv,const char **envp);

    看到这里你就会明白为什么system（）会接受父进程的环境变量，但是用system改变环境变量后，system一返回主函数还是没变。原因从system的实现可以看到，它是通过产生新进程实现的，从我的分析中可以看到父进程和子进程间没有进程通信，子进程自然改变不了父进程的环境变量。



 关于返回值，如果system()在调用/bin/sh时失败则返回127，其他失败原因返回-1。如果返回值为0，表示调用成功但是没有出现子进程。若参数string为空指针(NULL)，则返回非零值。如果system()调用成功则最后会返回执行shell命令后的返回值，但是此返回值也有可能为 system()调用/bin/sh失败所返回的127，因此最好能再检查errno 来确认执行成功。

 shell命令的返回值可以通过WEXITSTATUS(stat)得到。处理返回值的宏定义在 中，包括（stat是waitpid()的返回值）：
         WIFEXITED(stat)Non zero if child exited normally.
         非零 如果子程序正常退出
         WEXITSTATUS(stat)exit code returned by child
         子程序返回exit 值
         WIFSIGNALED(stat)Non-zero if child was terminated by a signal
         非零 如果子进程被一个信号结束
         WTERMSIG(stat)signal number that terminated child
         结束子进程的signal number
         WIFSTOPPED(stat)non-zero if child is stopped
         非零 如果子进程被停止
         WSTOPSIG(stat)number of signal that stopped child
         停止子进程的signal number.
         WIFCONTINUED(stat)non-zero if status was for continued child
         非零 如果状态是继续运行的子进程
         WCOREDUMP(stat)If WIFSIGNALED(stat) is non-zero, this is non-zero if the process leftbehind a core dump.
         如果WIFSIGNALED(stat)非零，而且进程产生了一个core dump,那么这个也是非零。



**3、popen**

popen()也常常被用来执行一个程序。

FILE *popen(const char *command, const char *type);
int pclose(FILE *stream);

popen() 函数用创建管道的方式启动一个 进程, 并调用 shell. 因为管道是被定义成单向的, 所以 type 参数只能定义成只读或者只写, 不能是两者同时, 结果流也相应的是只读或者只写. command 参数是一个字符串指针, 指向的是一个以 null 结束符结尾的字符串, 这个字符串包含一个 shell 命令. 这个命令被送到 /bin/sh 以 -c 参数执行, 即由 shell 来执行. type 参数也是一个指向以 null 结束符结尾的字符串的指针, 这个字符串必须是 'r' 或者 'w’ 来指明是读还是写.

popen() 函数的返回值是一个普通的标准I/O流, 它只能用 pclose() 函数来关闭, 而不是 fclose() 函数. 向这个流的写入被转化为对 command 命令的标准输入; 而 command 命令的标准输出则是和调用 popen(), 函数的进程相同，除非这个被command命令自己改变. 相反的, 读取一个 “被popen了的” 流, 就相当于读取 command 命令的标准输出, 而 command 的标准输入则是和调用 popen, 函数的进程相同.

注意, popen 函数的 输出流默认是被全缓冲的.

pclose 函数等待相关的进程结束并返回一个 command 命令的退出状态, 就像 wait4 函数 一样

#include

int main(int argc, char *argv[])
{
         char buf[128];
         FILE *pp;

         if( (pp = popen("ls -l", "r")) == NULL )
         {
                 printf("popen() error!/n");
                 exit(1);
         }

         while(fgets(buf, sizeof buf, pp))
         {
                 printf("%s", buf);
         }
         pclose(pp);
         return 0;
}









