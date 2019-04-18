# system阻塞SIGCHLD信号原因 - tifentan的专栏 - CSDN博客

2014年11月08日 11:58:53[露蛇](https://me.csdn.net/tifentan)阅读数：666



**代码1：APUE10.18节的system函数源代码**
int system(const char *cmdstring) /* with appropriate signal handling */
{
    pid_t               pid;
    int                 status;
    struct sigaction    ignore, saveintr, savequit;
    sigset_t            chldmask, savemask;
    ……
    sigemptyset(&chldmask);         /* now block SIGCHLD */
    sigaddset(&chldmask, SIGCHLD);
    if (sigprocmask(SIG_BLOCK, &chldmask, &savemask) 
        return(-1);

    if ((pid = fork()) 
        status = -1;    /* probably out of processes */
    } else if (pid == 0) {          /* child */
        ……
        sigprocmask(SIG_SETMASK, &savemask, NULL);

        execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);
        _exit(127);     /* exec error */
    } else {                        /* parent */
       while (waitpid(pid, &status, 0) 
           if (errno != EINTR) {
               status = -1; /* error other than EINTR from waitpid() */
               break;
           }
    }
    ……
    if (sigprocmask(SIG_SETMASK, &savemask, NULL) 
        return(-1);

    return(status);}
如果该函数的调用者在其SIGCHLD信号处理函数中调用了waitpid函数来获得任意子进程的终止状态，且在fork之前没有阻塞SIGCHLD信号,则子进程结束，父进程执行SIGCHLD信号处理函数且得到了子进程的终止状态。而该函数中的waitpid函数由于没有得到其创建的子进程的终止状态而出错。

定义了一个函数，函数中fork了一个子进程，并且定义的函数要得到关于子进程的一些信息，例如子进程ID、子进程终止状态等，而该函数的调用者所注册的SIGCHLD信号处理函数会影响定义的这个函数获取这些信息，因此为了避免该函数在获取这些信息之前，由于子进程终止触发SIGCHLD信号处理函数先执行，在fork之前都将SIGCHLD信号阻塞了，在函数获取了相关信息后，才对SIGCHLD信号接触阻塞。

**接下来我们以代码1为例完整的解释一下阻塞SIGCHLD信号的原因：**

在一个进程终止或停止时，SIGCHLD信号被送给其父进程。按**系统默认，将忽略此信号**。如果父进程希望了解其子进程的这种状态改变，则应捕捉此信号。信号捕捉函数中通常要调用wait函数以取得子进程ID和其终止状态。

一般的，父进程在生成子进程之后会有两种情况：一是父进程继续去做别的事情；另一是父进程什么都不做，一直在wait子进程退出。

SIGCHLD信号就是为第一种情况准备的，它让父进程去做别的事情，而只要父进程注册了处理该信号的函数，在子进程退出时就会调用该函数，在函数中wait子进程得到终止状态之后再继续做父进程的事情。

我们先来写一个function_model函数模型，如下所示：
int function_model(…) {    pid_t               pid;    int                 status;    ……    if ((pid = fork())         status = -1;        } else if (pid == 0)
 {          /* child */       ……    } else {                        /* parent */       while (waitpid(pid, &status, 0)            if (errno != EINTR) {               status = -1;                break;           }    }        ……    return(status);}

当我们定义了一个具有function_model函数模型结构的函数时，应该在fork之前将SIGCHLD信号阻塞，fork之后，父子进程的SIGCHLD信号都是被阻塞的。子进程在进行任何的操作之前，应该将SIGCHLD恢复至原来的设置。父进程则应该在waitpid函数之后，将SIGCHLD恢复至原来的设置。

即这种类型的函数的正确的格式为：

int function_model(…) 
{
    pid_t               pid;
    int                 status;
    sigset_t            chldmask, savemask;
    ……
    sigemptyset(&chldmask);         /* now block SIGCHLD */
    sigaddset(&chldmask, SIGCHLD);
    if(sigprocmask(SIG_BLOCK, &chldmask, &savemask) 
        return(-1);

    if ((pid = fork()) 
        status = -1;    
} else if (pid == 0) {          /* child */

sigprocmask(SIG_SETMASK, &savemask, NULL);
       ……
    } else {                        /* parent */
       while (waitpid(pid, &status, 0) 
           if (errno != EINTR) {
               status = -1; 
               break;
           }
    }
    ……
if(sigprocmask(SIG_SETMASK, &savemask, NULL) 
    return(-1);
    return(status);
}

父进程阻塞SIGCHLD信号的原因在于：如果function_model函数的调用者caller在调用function_model之前，注册了SIGCHLD信号的处理函数sigchld_handler，且sigchld_handler函数中调用了waitpid函数等待任意子进程结束，如下所示：

void sigchld_handler(int signo)
{
    pid_t pid;
    int status;
    while((pid=waitpid(-1,&status,WNOHANG))>0)
    {
        ……
}
return ;
}

    若我们没有阻塞SIGCHLD信号，则当function_model函数中fork的子进程结束时，会向父进程发送SIGCHLD信号，则其信号处理函数sigchld_handler被调用，并在sigchld_handler函数中调用了waitpid得到了这个子进程的退出状态，然后释放掉其占用的进程表等其它资源。返回到function_model函数中继续执行，由于其创建的子进程的退出状态已经被获取，内核不再保存其退出状态，所以其中调用的waitpid函数由于无法得到其退出状态而出错，并将status设置为-1。
    若我们阻塞了SIGCHLD信号，则当function_model函数中fork的子进程结束时，内核会向父进程发送SIGCHLD信号，但SIGCHLD信号被阻塞而不递送给父进程，即处于未决状态，因此function_model函数中的waitpid就能正确得到这个子进程的退出状态。当解除对SIGCHLD信号的阻塞时，SIGCHLD信号会被递交给父进程，引发父进程调用sigchld_handler函数，由于在function_model函数中以调用waitpid获取了function_model函数创建的子进程的退出状态，内核不再保存其终止信息，所以sigchld_handler中的waitpid也不会的到其退出状态。

    下面是***Advanced Programming in the UNIX® Environment: Second Edition*（APUE）**中的解释：
POSIX.1 states that if wait or waitpid returns the status of a child process while SIGCHLD is pending, then SIGCHLD should not be delivered to the process unless
 the status of another child process is also available. None of the four implementations discussed in this book implements this semantic. Instead, SIGCHLD remains pending after the system function calls waitpid; when the signal is unblocked, it is delivered
 to the caller. If we called wait in the sig_chld function in 
[Figure
 10.26](http://bbs.chinaunix.net/mk:@MSITStore:C:/Documents%20and%20Settings/hackbutter/%E6%A1%8C%E9%9D%A2/LINUX/linux/linux%E5%BA%94%E7%94%A8/linux%E7%B3%BB%E7%BB%9F%E7%BC%96%E7%A8%8B/%E8%8B%B1%E6%96%87%E7%AC%AC%E4%BA%8C%E7%89%88-Advanced%20Programming%20in%20the%20UNIX%20Environment%20-%202nd%20Edition.chm::/0201433079/ch10lev1sec18.html#ch10fig26%23ch10fig26)
, it would return 1 with errno set to ECHILD, since the system function already retrieved the termination status of the child.

[http://blog.chinaunix.net/u1/59740/showart_486730.html](http://blog.chinaunix.net/u1/59740/showart_486730.html)

