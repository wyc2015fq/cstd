# 区分execl与system——应用程序中执行命令 - DoubleLi - 博客园







**execl：相关函数：**fork, execle, execlp, execv, execve, execvp
**表头文件：#**include <unistd.h>
**函数定义：**int execl(const char *path, const char *arg, ...);
**函数说明：**execl()用来执行参数path字符串所代表的文件路径， 接下来的参数代表执行该文件时传递的argv[0],argv[1].....是后一个参数必须用空指针NULL作结束
**返回值    ：**成功则不返回值， 失败返回-1， 失败原因存于errno中
**错误代码：**参execve()
范例：
/*   执行 /bin/ls   -al   /ect/passwd */

#include <unistd.h>


/**

* File: execl.c

*

*/
main()
{
    execl("/bin/ls", "ls", "-al", "/etc/passwd", (char *) 0);
}






或

#include <unistd.h>

/**

* File: execl.c

*

*/


int main()
{
   char args[]=" -l";
   execl("/bin/ls","ls","-al","/etc/",NULL);
   return 0;
}





[cnscn@test c]$ make execl
cc     execl.c   -o execl
[cnscn@test c]$ ./execl   
-rw-r--r-- 1 root root 2218 Jan 13 11:36 /etc/passwd

system:
相关函数
fork，execve，waitpid，popen
表头文件
＃i nclude<stdlib.h>
定义函数
int system(const char * string);
函数说明
system()会调用fork()产生子进程，由子进程来调用/bin/sh-c string来执行参数string字符串所代表的命令，此命>令执行完后随即返回原调用的进程。在调用system()期间SIGCHLD 信号会被暂时搁置，SIGINT和SIGQUIT 信号则会被忽略。
返回值
=-1:出现错误   
=0:调用成功但是没有出现子进程   
>0:成功退出的子进程的id
如果system()在调用/bin/sh时失败则返回127，其他失败原因返回-1。若参数string为空指针(NULL)，则返回非零值>。 如果system()调用成功则最后会返回执行shell命令后的返回值，但是此返回值也有可能为 system()调用/bin/sh失败所返回的127，因此最好能再检查errno 来确认执行成功。
附加说明
在编写具有SUID/SGID权限的程序时请勿使用system()，system()会继承环境变量，通过环境变量可能会造成系统安全的问题。
范例
＃i nclude<stdlib.h>
main()
{
system(“ls -al /etc/passwd /etc/shadow”);
}
执行结果：

-rw-r--r-- 1 root root 705 Sep 3 13 :52 /etc/passwd
-r--------- 1 root root 572 Sep 2 15 :34 /etc/shado

例2：

char tmp[];
sprintf(tmp,"/bin/mount -t vfat %s /mnt/usb",dev);
system(tmp);
其中dev是/dev/sda1。



system源码

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
    else if(pid == 0){
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

当system接受的命令为NULL时直接返回，否则fork出一个子进程，因为fork在两个进程：父进程和子进程中都返回，这里要检查返回的pid，fork在子进程中返回0，在父进程中返回子进程的pid，父进程使用waitpid等待子进程结束，子进程则是调用execl来启动一个程序代替自己，execl("/bin/sh", "sh", "-c", cmdstring,(char*)0)是调用shell，这个shell的路径是/bin/sh，后面的字符串都是参数，然后子进程就变成了一个shell进程，这个shell的参数是cmdstring，就是system接受的参数。在windows中的shell是command，想必大家很熟悉shell接受命令之后做的事了。

如果上面的你没有看懂，那我再解释下fork的原理：当一个进程A调用fork时，系统内核创建一个新的进程B，并将A的内存映像复制到B的进程空间中，因为A和B是一样的，那么他们怎么知道自己是父进程还是子进程呢，看fork的返回值就知道，上面也说了fork在子进程中返回0，在父进程中返回子进程的pid。

execl是编译器的函数（在一定程度上隐藏具体系统实现），在linux中它会接着产生一个linux系统的调用execve, 原型见下：
    int execve(const char * file,const char **argv,const char **envp);

看到这里你就会明白为什么system（）会接受父进程的环境变量，但是用system改变环境变量后，system一返回主函数还是没变，原因从system的实现可以看到，它是通过产生新进程实现的，从我的分析中可以看到父进程和子进程间没有进程通信，子进程自然改变不了父进程的环境变量。




注意 ：execl与system的一个区别是：使用execl时最好在之前先调用fork，在子进程中执行execl，否则execl会返回。而system则不用，因为从它的实现源码来看，实际上已经调用了fork。









