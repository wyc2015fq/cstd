# Windows和linux的多线程编程 c语言 - sxf_123456的博客 - CSDN博客
2017年02月26日 21:33:18[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：302
在Windows的多线程编程中，创建线程的函数主要有CreateThread和_beginthread（及_beginthreadex）。
CreateThread 和 ExitThread
  使用API函数CreateThread创建线程时，其中的线程函数原型：
  DWORD WINAPI ThreadProc(LPVOID lpParameter);
在线程函数返回后，其返回值用作调用ExitThread函数的参数（由系统隐式调用）。可以使用GetExitCodeThread函数获得该线程函数的返回值。
  当线程函数的起始地址无效（或者不可访问）时，CreateThread函数仍可能成功返回。如果该起始地址无效，则当线程运行时，异常将发生，线程终止。并返回一个错误代码。
  使用CreateThread创建的线程具有THREAD_PRIORITY_NORMAL的线程优先级。可以使用GetThreadPriority和SetThreadPriority函数获取和设置线程优先级值。
  系统中的线程对象一直存活到线程结束，并且所有指向它的句柄都通过调用CloseHandle关闭后。
_beginthread 和 _endthread (_beginthread & _endthread)
  对于使用C运行时库里的函数的线程应该使用_beginthread和_endthread这些C运行时函数来管理线程，而不是使用CreateThread和ExitThread。否则，当调用ExitThread后，可能引发内存泄露。
  在使用_beginthread或者_beginthreadex创建线程时，应该包含头文件<process.h>，并且需要设置多线程版 本的运行时库。「Project Settings」--> 「C/C++」-->「Category」-->「Code Generation」-->「Use Run-Time Library」-->「Multithreaded」和「Debug Multithreaded」。这相当于给编译添加了一个编译选项/MT，使编译器在编译时在.obj文件中使用libcmt.lib文件名而不是
 libc.lib。连接器使用这个名字与运行时库函数连接。
  可以调用_endthread和_endthreadex显示式结束一个线程。然而，当线程函数返回时，_endthread和_endthreadex 被自动调用。endthread和_endthreadex的调用有助于确保分配给线程的资源的合理回收。_endthread自动地关闭线程句柄，然而 _endthreadex却不会。因此，当使用_beginthread和_endthread时，不需要显示调用API函数CloseHandle式关 闭线程句柄。该行为有别于API函数ExitThread的调用。_endthread和_endthreadex回收分配的线程资源后，调用
 ExitThread。
   当_beginthread和_beginthreadex被调用时，操作系统自己处理线程栈的分配。如果在调用这些函数时，指定栈大小为0，则操作系统 为该线程创建和主线程大小一样的栈。如果任何一个线程调用了abort、exit或者ExitProcess，则所有线程都将被终止。
    线程是操作系统管理的一种资源，不同操作系统差异很大，有的支持，有的不支持，实现的方式也不同，下面是引用的LINUX下多线程例子，使用pthread库，简单说明下：
/*thread_example.c : c multiple thread programming in linux
  *author : falcon
  *date : 2006.8.15
  *e-mail : [email]tunzhj03@st.lzu.edu.cn[/email]
  */
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#define MAX 10
pthread_t thread[2]; //创建线程函数返回类型
pthread_mutex_t mut; //互斥锁类型
int number=0, i;
void *thread1() //线程函数
{
  printf ("thread1 : I'm thread 1\n");
  for (i = 0; i < MAX; i++)
  {   
  printf("thread1 : number = %d\n",number);   
  pthread_mutex_lock(&mut); //加锁，用于对共享变量操作
  number++;
  pthread_mutex_unlock(&mut); //解锁
  sleep(2);
  }   
  printf("thread1 :主函数在等我完成任务吗？\n");
  pthread_exit(NULL);
}
void *thread2()
{
  printf("thread2 : I'm thread 2\n");
  for (i = 0; i < MAX; i++)
  {   
  printf("thread2 : number = %d\n",number);
  pthread_mutex_lock(&mut);
  number++;
  pthread_mutex_unlock(&mut);
  sleep(3);
  }   
  printf("thread2 :主函数在等我完成任务吗？\n");
  pthread_exit(NULL);
}
void thread_create(void)
{
  /*创建线程*/
  pthread_create(&thread[0], NULL, thread1, NULL);
  printf("线程1被创建\n");
  pthread_create(&thread[1], NULL, thread2, NULL);
  printf("线程2被创建\n");
}
void thread_wait(void)
{
  /*等待线程结束*/
  pthread_join(thread[0],NULL);
  printf("线程1已经结束\n");
  pthread_join(thread[1],NULL);
  printf("线程2已经结束\n");
}
int main()
{
  /*用默认属性初始化互斥锁*/
  pthread_mutex_init(&mut,NULL);  
  printf("我是主函数哦，我正在创建线程，呵呵\n");
  thread_create();
  printf("我是主函数哦，我正在等待线程完成任务阿，呵呵\n");
  thread_wait();
  return 0;
}
pthread相关：
**运行之前需要做一些配置：**
1.下载PTHREAD的WINDOWS开发包 pthreads-w32-2-4-0-release.exe（任何一个版本均可） 
   http://sourceware.org/pthreads-win32/ ，解压到一个目录。 
2.找到include和lib文件夹，下面分别把它们添加到VC++6.0的头文件路径和静态链接库路径下面： 
   a).Tools->Options，选择Directory页面，然后在Show directories for:中选择Include files(默认)     在Directories中添加include的路径。在Show directories for:中选择Library files， 
    在Directories中添加lib的路径。 
   b).Project->Settings,选择Link页面，然后将lib下的*.lib文件添加到Object/library Modules, 
     各lib文件以空格隔开。 
   c).将lib下的*.dll文件复制到工程目录下，即根目录。 
3.代码
```
1.#include <stdio.h>   
2.#include <stdlib.h>   
3.#include <pthread.h>   
4.#include <windows.h>   
5.  
6.int  piao = 100;   
7.  
8.pthread_mutex_t mut;   
9.    
10.void* tprocess1(void* args){   
11.   int a = 0;   
12.       while(true){   
13.           pthread_mutex_lock(&mut);   
14.           if(piao>0){   
15.            Sleep(1);   
16.            piao--;   
17.            printf("窗口1----------------还剩%d张票\n",piao);   
18.           }else{   
19.            a = 1;   
20.           }   
21.          pthread_mutex_unlock(&mut);   
22.           if(a == 1) {   
23.            break;   
24.           }   
25.        }     
26.           
27.     
28.       return NULL;   
29.}   
30.  
31.void* tprocess2(void* args){   
32.     int a = 0;   
33.       while(true){   
34.           pthread_mutex_lock(&mut);   
35.           if(piao>0){   
36.            Sleep(1);   
37.           piao--;   
38.        printf("窗口2----------------还剩%d张票\n",piao);   
39.           }else{   
40.            a = 1;   
41.           }   
42.          pthread_mutex_unlock(&mut);   
43.           if(a == 1) {   
44.            break;   
45.           }   
46.        }     
47.           
48.     
49.       return NULL;   
50.}   
51.  
52.void* tprocess3(void* args){   
53.   int a = 0;   
54.       while(true){   
55.           pthread_mutex_lock(&mut);   
56.           if(piao>0){   
57.       Sleep(1);   
58.             piao--;   
59.          
60.        printf("窗口3----------------还剩%d张票\n",piao);   
61.           }else{   
62.            a = 1;   
63.           }   
64.          pthread_mutex_unlock(&mut);   
65.           if(a == 1) {   
66.            break;   
67.           }   
68.        }     
69.           
70.     
71.       return NULL;   
72.}   
73.  
74.void* tprocess4(void* args){   
75.       int a = 0;   
76.       while(true){   
77.           pthread_mutex_lock(&mut);   
78.           if(piao>0){   
79.       Sleep(1);   
80.         
81.                   piao--;   
82.          
83.        printf("窗口4----------------还剩%d张票\n",piao);   
84.           }else{   
85.            a = 1;   
86.           }   
87.          pthread_mutex_unlock(&mut);   
88.           if(a == 1) {   
89.            break;   
90.           }   
91.        }     
92.           
93.     
94.       return NULL;   
95.}   
96.  
97.int main(){   
98.  pthread_mutex_init(&mut,NULL);   
99.  pthread_t t1;   
100.  pthread_t t2;   
101.  pthread_t t3;   
102.  pthread_t t4;   
103.  pthread_create(&t4,NULL,tprocess4,NULL);   
104.  pthread_create(&t1,NULL,tprocess1,NULL);   
105.  pthread_create(&t2,NULL,tprocess2,NULL);   
106.  pthread_create(&t3,NULL,tprocess3,NULL);   
107.  Sleep(5000);   
108.  return 0;   
109.}  
转载网址：http://www.cnblogs.com/lixiaohui-ambition/archive/2012/07/26/2610336.html
```
