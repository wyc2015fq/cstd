# fork与vfork的区别 - DoubleLi - 博客园






fork（）与vfock（）都是创建一个进程，那他们有什么区别呢？总结有以下三点区别： 
1.  fork  （）：子进程拷贝父进程的数据段，代码段 
    vfork （ ）：子进程与父进程共享数据段 
2.  fork （）父子进程的执行次序不确定 
    vfork 保证子进程先运行，在调用exec 或exit 之前与父进程数据是共享的,在它调用exec
     或exit 之后父进程才可能被调度运行。 
3.  vfork （）保证子进程先运行，在她调用exec 或exit 之后父进程才可能被调度运行。如果在
   调用这两个函数之前子进程依赖于父进程的进一步动作，则会导致死锁。 
下面通过几个例子加以说明： 
第一：子进程拷贝父进程的代码段的例子： 

- #include<sys/types.h>  
- #include<unistd.h>  
- #include<stdio.h>  
- 
- int main()  
- {  
-     pid_t pid;  
-     pid = fork();  
- if(pid<0)  
-         printf("error in fork!\n");  
- else if(pid == 0)  
-         printf("I am the child process,ID is %d\n",getpid());  
- else   
-         printf("I am the parent process,ID is %d\n",getpid());  
- return 0;  
- 
- }  



运行结果： 

- [root@localhost fork]# gcc -o fork fork.c   
- [root@localhost fork]# ./fork  


- I am the child process,ID is 4711  
- I am the parent process,ID is 4710  



为什么两条语 都会打印呢？这是因为fork（）函数用于从已存在的进程中创建一个新的进 
程，新的进程称为子进程，而原进程称为父进程，fork （）的返回值有两个，子进程返回0，
父进程返回子进程的进程号，进程号都是非零的正整数，所以父进程返回的值一定大于零，
在pid=fork();语句之前只有父进程在运行，而在pid=fork();之后，父进程和新创建的子进程 
都在运行，所以如果pid==0，那么肯定是子进程，若pid ！=0 （事实上肯定大于0），那么是 
父进程在运行。而我们知道fork()函数子进程是拷贝父进程的代码段的，所以子进程中同样 
有 
if(pid<0) 
         printf("error in fork!"); 
     else if(pid==0) 
         printf("I am the child process,ID is %d\n",getpid()); 
     else 
         printf("I am the parent process,ID is %d\n",getpid()); 
} 
这么一段代码，所以上面这段代码会被父进程和子进程各执行一次，最终由于子进程的pid= =0,

而打印出第一句话，父进程的pid>0，而打印出第二句话。于是得到了上面的运行结果。 
再来看一个拷贝数据段的例子： 

- #include<sys/types.h>  
- #include<unistd.h>  
- #include<stdio.h>  
- 
- int main()  
- {  
-     pid_t pid;  
- int cnt = 0;  
-     pid = fork();  
- if(pid<0)  
-         printf("error in fork!\n");  
- else if(pid == 0)  
-     {  
-         cnt++;  
-         printf("cnt=%d\n",cnt);  
-         printf("I am the child process,ID is %d\n",getpid());  
-     }  
- else  
-     {  
-         cnt++;  
-         printf("cnt=%d\n",cnt);  
-         printf("I am the parent process,ID is %d\n",getpid());  
-     }  
- return 0;  
- }  




大家觉着打印出的值应该是多少呢？是不是2 呢？先来看下运行结果吧 

- [root@localhost fork]# ./fork2  
- cnt=1  
- I am the child process,ID is 5077  
- cnt=1  
- I am the parent process,ID is 5076  




为什么不是2 呢？因为我们一次强调fork （）函数子进程拷贝父进程的数据段代码段，所以 
cnt++; 
    printf("cnt= %d\n",cnt);

    return 0 
将被父子进程各执行一次，但是子进程执行时使自己的数据段里面的（这个数据段是从父进 
程那copy 过来的一模一样）count+1，同样父进程执行时使自己的数据段里面的count+1， 
他们互不影响，与是便出现了如上的结果。


那么再来看看vfork （）吧。如果将上面程序中的fork （）改成vfork（），运行结果是什么 
样子的呢？ 

- [root@localhost fork]# gcc -o fork3 fork3.c   
- [root@localhost fork]# ./fork3  
- cnt=1  
- I am the child process,ID is 4711  
- cnt=1  
- I am the parent process,ID is 4710  
- 段错误  




本来vfock（）是共享数据段的，结果应该是2，为什么不是预想的2 呢？先看一个知识点： 
vfork 和fork 之间的另一个区别是：vfork 保证子进程先运行，在她调用exec 或exit 之 
后父进程才可能被调度运行。如果在调用这两个函数之前子进程依赖于父进程的进一步动 
作，则会导致死锁。 
这样上面程序中的fork （）改成vfork（）后，vfork （）创建子进程并没有调用exec 或exit，
所以最终将导致死锁。 
怎么改呢？看下面程序： 

- #include<sys/types.h>  
- #include<unistd.h>  
- #include<stdio.h>  
- 
- int main()  
- {  
-     pid_t pid;  
- int cnt = 0;  
-     pid = vfork();  
- if(pid<0)  
-         printf("error in fork!\n");  
- else if(pid == 0)  
-     {  
-         cnt++;  
-         printf("cnt=%d\n",cnt);  
-         printf("I am the child process,ID is %d\n",getpid());  
-        _exit(0);  
-     }  
- else  
-     {  
-         cnt++;  
-         printf("cnt=%d\n",cnt);  
-         printf("I am the parent process,ID is %d\n",getpid());  
-     }  
- return 0;  
- 
- }  





如果没有_exit（0）的话，子进程没有调用exec 或exit，所以父进程是不可能执行的，在子 
进程调用exec 或exit 之后父进程才可能被调度运行。 
所以我们加上_exit(0);使得子进程退出，父进程执行，这样else 后的语句就会被父进程执行， 
又因在子进程调用exec 或exit之前与父进程数据是共享的,所以子进程退出后把父进程的数 
据段count改成1 了，子进程退出后，父进程又执行，最终就将count变成了2，看下实际 
运行结果： 

- [root@localhost fork]# gcc -o fork3 fork3.c   
- [root@localhost fork]# ./fork3  
- cnt=1  
- I am the child process,ID is 4711  
- cnt=2  
- I am the parent process,ID is 4710  




网上抄的一段，可以再理解理解： 
为什么会有vfork，因为以前的fork 很傻， 它创建一个子进程时，将会创建一个新的地址 
空间，并且拷贝父进程的资源，而往往在子进程中会执行exec 调用，这样，前面的拷贝工 
作就是白费力气了，这种情况下，聪明的人就想出了vfork，它产生的子进程刚开始暂时与 
父进程共享地址空间（其实就是线程的概念了），因为这时候子进程在父进程的地址空间中 
运行，所以子进程不能进行写操作，并且在儿子 霸占”着老子的房子时候，要委屈老子一 
下了，让他在外面歇着（阻塞），一旦儿子执行了exec 或者exit 后，相 于儿子买了自己的 
房子了，这时候就相 于分家了。









