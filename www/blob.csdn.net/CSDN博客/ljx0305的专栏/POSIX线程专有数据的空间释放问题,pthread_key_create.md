# POSIX线程专有数据的空间释放问题,pthread_key_create - ljx0305的专栏 - CSDN博客
2012年12月28日 17:47:58[ljx0305](https://me.csdn.net/ljx0305)阅读数：748
个人分类：[Linux程序及使用](https://blog.csdn.net/ljx0305/article/category/394700)
下面说一下线程中特有的线程存储， Thread Specific Data 。线程存储有什么用了？他是什么意思了？大家都知道，在多线程程序中，所有线程共享程序中的变量。现在有一全局变量，所有线程都可以使用它，改变它的值。而如果每个线程希望能单独拥有它，那么就需要使用线程存储了。表面上看起来这是一个全局变量，所有线程都可以使用它，而它的值在每一个线程中又是单独存储的。这就是线程存储的意义。
下面说一下线程存储的具体用法。
l          创建一个类型为 pthread_key_t 类型的变量。
l          调用 pthread_key_create() 来创建该变量。该函数有两个参数，第一个参数就是上面声明的 pthread_key_t 变量，第二个参数是一个清理函数，用来在线程释放该线程存储的时候被调用。该函数指针可以设成 NULL ，这样系统将调用默认的清理函数。
l          当线程中需要存储特殊值的时候，可以调用 pthread_setspcific() 。该函数有两个参数，第一个为前面声明的 pthread_key_t 变量，第二个为 void* 变量，这样你可以存储任何类型的值。
l          如果需要取出所存储的值，调用 pthread_getspecific() 。该函数的参数为前面提到的 pthread_key_t 变量，该函数返回 void * 类型的值。
下面是前面提到的函数的原型：
int pthread_setspecific(pthread_key_t key, const void *value);
void *pthread_getspecific(pthread_key_t key);
int pthread_key_create(pthread_key_t *key, void (*destructor)(void*));
下面是一个如何使用线程存储的例子：
**[cpp]**[view plain](http://blog.csdn.net/denny_233/article/details/7221831#)[copy](http://blog.csdn.net/denny_233/article/details/7221831#)
- <pre name="code"class="cpp">#include <malloc.h>  
- 
- #include <pthread.h>
- 
- #include <stdio.h>
- 
- /* The key used to associate a log file pointer with each thread. */
- 
- static pthread_key_t thread_log_key;  
- 
- /* Write MESSAGE to the log file for the current thread. */
- 
- void write_to_thread_log (constchar* message)  
- 
- {  
- 
- FILE* thread_log = (FILE*) pthread_getspecific (thread_log_key);  
- 
- fprintf (thread_log, “%s\n”, message);  
- 
- }  
- 
- /* Close the log file pointer THREAD_LOG. */
- 
- void close_thread_log (void* thread_log)  
- 
- {  
- 
- fclose ((FILE*) thread_log);  
- 
- }  
- 
- void* thread_function (void* args)  
- 
- {  
- 
- char thread_log_filename[20];  
- 
- FILE* thread_log;  
- 
- /* Generate the filename for this thread’s log file. */
- 
- sprintf (thread_log_filename, “thread%d.log”, (int) pthread_self ());  
- 
- /* Open the log file. */
- 
- thread_log = fopen (thread_log_filename, “w”);  
- 
- /* Store the file pointer in thread-specific data under thread_log_key. */
- 
- pthread_setspecific (thread_log_key, thread_log);  
- 
- write_to_thread_log (“Thread starting.”);  
- 
- /* Do work here... */
- 
- return NULL;  
- 
- }  
- 
- int main ()  
- 
- {  
- 
- int i;  
- 
- pthread_t threads[5];  
- 
- /* Create a key to associate thread log file pointers in
- 
- thread-specific data. Use close_thread_log to clean up the file
- 
- pointers. */
- 
- pthread_key_create (&thread_log_key, close_thread_log);  
- 
- /* Create threads to do the work. */
- 
- for (i = 0; i < 5; ++i)  
- 
- pthread_create (&(threads[i]), NULL, thread_function, NULL);  
- 
- /* Wait for all threads to finish. */
- 
- for (i = 0; i < 5; ++i)  
- 
- pthread_join (threads[i], NULL);  
- 
- return 0;  
- 
- }  </pre><br><br>  
最后说一下线程的本质。其实在Linux 中，新建的线程并不是在原先的进程中，而是系统通过一个系统调用clone() 。该系统copy 了一个和原先进程完全一样的进程，并在这个进程中执行线程函数。不过这个copy 过程和fork 不一样。copy 后的进程和原先的进程共享了所有的变量，运行环境。这样，原先进程中的变量变动在copy 后的进程中便能体现出来
//////--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------/////先记下来，以后有机会请教高手
最近学习通过pthread_key_create创建的线程专有数据时发现如果不对线程使用pthread_join，则不会调用pthread_key_create所指定的资源释放函数。而并没有像书上所说的在线程pthread_exit()后就会调用释放函数。这是为什么？究竟什么情况下会调用？
我使用的测试程序：
**[cpp]**[view
 plain](http://blog.csdn.net/denny_233/article/details/7221831#)[copy](http://blog.csdn.net/denny_233/article/details/7221831#)
- #include <pthread.h > 
- #include <stdio.h > 
- #include <unistd.h > 
- 
- usingnamespace std;   
- 
- pthread_key_t key;   
- void echomsg(void* p)   
- {   
- int t=*(int*)p;   
- printf( "destructor excuted in thread %d, param=%d\n ",pthread_self(),t);   
- }   
- 
- void* child1(void* arg)   
- {   
- int*ptid= newint;   
- *ptid=pthread_self();   
- printf( "thread %d enter\n ",*ptid);   
- pthread_setspecific(key,(void*)ptid);   
- sleep(2);   
- printf( "thread %d returns %d\n ",*ptid,*((int*)pthread_getspecific(key)));   
- sleep(5);   
- pthread_exit(NULL);   
- return NULL;   
- }   
- 
- void* child2(void* arg)   
- {   
- int*ptid= newint;   
- *ptid=pthread_self();   
- printf( "thread %d enter\n ",*ptid);   
- pthread_setspecific(key,(void*)ptid);   
- sleep(1);   
- printf( "thread %d returns %d\n ",*ptid,*((int*)pthread_getspecific(key)));   
- sleep(5);   
- pthread_exit(NULL);   
- return NULL;   
- }   
- 
- int main()   
- {   
- pthread_t tid1,tid2;   
- printf( "hello\n ");   
- pthread_key_create(&key,echomsg);   
- pthread_create(&tid1,NULL,child1,NULL);   
- pthread_create(&tid2,NULL,child2,NULL);   
- //pthread_join(tid1,NULL); 
- pthread_join(tid2,NULL);   
- sleep(3);   
- pthread_key_delete(key);   
- printf( "main thread %d exit\n ",pthread_self());   
- 
- return 0;   
- }   
这是程序输出 
---------------------------------------------- 
hello 
thread 1083395264 enter 
thread 1091783744 enter 
thread 1091783744 returns 1091783744 
thread 1083395264 returns 1083395264 
destructor excuted in thread 1075005312, param=1091783744
main thread 1075005312 exit 
----------------------------------------------
         这是在网上看到的一篇文章，个人认为并不是pthread_join接受线程时才调用每个线程的key的echomsg函数。而是由于pthread_join阻塞等待特定的线程结束，以至于被等待的线程能够全部处理完（当然包括每个线程key的特定清理函数echomsg），所以当pthread_join尤其用在main线程中时，能够确保特定的子线程能处理完。
       在以上程序中，稍加修改：child2（）函数中的sleep睡眠时间都改为sleep（1），并且把主函数的pthread_join(tid2,NULL);也注释掉，重新编译执行也会得到上面类似的结果。
       同时也发现child1的线程也退出了，并没有sleep（5）足够的时间，我认为是child2线程结束时要发送信号，而sleep是可被信号中断的（这个陈述稍有欠当，姑且是那个意思），所以child1的线程在child2结束后也结束了。但是没来及执行child1的echomsg，main线程就结束了，随之整个进程也结束了
个人认为上面的程序有点隐形的bug，自己改进的代码如下：
**[cpp]**[view
 plain](http://blog.csdn.net/denny_233/article/details/7221831#)[copy](http://blog.csdn.net/denny_233/article/details/7221831#)
- #include    <pthread.h>  
- #include    <stdio.h>  
- #include    <unistd.h>  
- #include    <stdlib.h> //atexit
- 
- usingnamespace  std;    
- 
- pthread_key_t  key;    
- pthread_once_t thread_once = PTHREAD_ONCE_INIT;  
- 
- void echomsg(void *);  
- void once_run(void)  
- {  
-     printf("pthread_key_t init in the once_run\n  ");  
-     pthread_key_create(&key,echomsg);  
- 
- }  
- void  echomsg(void*  p)    
- {    
- int  t=*(int*)p;    
-     printf(  "destructor  excuted  in  thread  %d,  param=%d\n  ",pthread_self(),t);    
- delete (int *)p;  
- }    
- 
- void*  child1(void*  arg)    
- {    
- int*ptid=  newint;    
- 
-     pthread_once(&thread_once, once_run);//测试pthread_once是否还会在此执行不，因为在main线程已经执行了.
-     *ptid=pthread_self();    
-     printf(  "thread1  %d  enter\n  ",*ptid);    
-     pthread_setspecific(key,(void*)ptid);    
-     sleep(2);    
-     printf(  "thread1  %d  returns  %d\n  ",*ptid,*((int*)pthread_getspecific(key)));    
-     sleep(5);    
-     pthread_exit(NULL);    
- return  NULL;    
- }    
- 
- void*  child2(void*  arg)    
- {    
- int*ptid=  newint;    
-     *ptid=pthread_self();    
-     printf(  "thread2  %d  enter\n  ",*ptid);    
-     pthread_setspecific(key,(void*)ptid);    
-     sleep(1);    
-     printf(  "thread2  %d  returns  %d\n  ",*ptid,*((int*)pthread_getspecific(key)));    
-     sleep(1);    
-     pthread_exit(NULL);    
- return  NULL;    
- }    
- 
- void main_exit()  
- {  
-     pthread_key_delete(key);  
- 
- }  
- int  main()    
- {    
-     pthread_t  tid1,tid2;    
-     printf(  "hello\n  ");    
-     atexit(main_exit);//为了防止sleep(4)放到pthread_key_delete(key)后面就会出现段错误了。但是个人也不很提倡用atexit这个函数。
-     pthread_once(&thread_once, once_run);  
- //pthread_key_create(&key,echomsg);  //保证一次性运行把其放到了pthread_once了
-     pthread_create(&tid1,NULL,child1,NULL);    
-     pthread_create(&tid2,NULL,child2,NULL);    
- //pthread_join(tid1,NULL);  
- //pthread_join(tid2,NULL);  
-     sleep(3);    
- //pthread_key_delete(key);  
-     printf(  "main  thread  %d  exit\n  ",pthread_self());    
- 
- return  0;    
- }   

程序输出：
hello
  pthread_key_t init in the once_run
  thread1  -1208583280  enter
  thread2  -1219073136  enter
  thread2  -1219073136  returns  -1219073136
  thread1  -1208583280  returns  -1208583280
  destructor  excuted  in  thread  -1219073136,  param=-1219073136
  main  thread  -1208580400  exit
-到此结束-![睡觉](http://static.blog.csdn.net/xheditor/xheditor_emot/default/sleep.gif)
、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、
<pthread_once and pthread_key_create的交叉应用说明>
转自：http://blog.csdn.net/yuyin86/article/details/6735245
一次性初始化
    有时候我们需要对一些posix变量只进行一次初始化，如线程键（我下面会讲到）。如果我们进行多次初始化程序就会出现错误。
    在传统的顺序编程中，一次性初始化经常通过使用布尔变量来管理。控制变量被静态初始化为0，而任何依赖于初始化的代码都能测试该变量。如果变量值仍然为0，则它能实行初始化，然后将变量置为1。以后检查的代码将跳过初始化。
    但是在多线程程序设计中，事情就变的复杂的多。如果多个线程并发地执行初始化序列代码，可能有2个线程发现控制变量为0，并且都实行初始化，而该过程本该仅仅执行一次。
如果我们需要对一个posix变量静态的初始化，可使用的方法是用一个互斥量对该变量的初始话进行控制。但有时候我们需要对该变量进行动态初始化，pthread_once就会方便的多。 
函数原形：
pthread_once_t once_control=PTHREAD_ONCE_INIT;
int pthread_once(pthread_once_t *once_control,void(*init_routine)(void));
参数：
once_control         控制变量
init_routine         初始化函数
返回值：
若成功返回0，若失败返回错误编号。
 类型为pthread_once_t的变量是一个控制变量。控制变量必须使用PTHREAD_ONCE_INIT宏静态地初始化。
pthread_once函数首先检查控制变量，判断是否已经完成初始化，如果完成就简单地返回；否则，pthread_once调用初始化函数，并且记录下初始化被完成。如果在一个线程初始时，另外的线程调用pthread_once，则调用线程等待，直到那个现成完成初始话返回。
下面就是该函数的程序例子： 
#include <pthread.h>
pthread_once_t once=PTHREAD_ONCE_INIT;
pthread_mutex_t mutex;　
void once_init_routine(void)　
{
    int status;
    status=pthread_mutex_init(&mutex,NULL);
    if(status==0)
    printf(“Init success!,My id is %u”,pthread_self());
}
void *child_thread(void *arg)
{
    printf(“I’m child ,My id is %u”,pthread_self());
    pthread_once(&once,once_init_routine);
}
    int main(int argc,char *argv[ ])
{
    pthread_t child_thread_id;
    pthread_create(&child_thread_id,NULL,child_thread,NULL);
    printf(“I’m father,my id is %u”,pthread_self());
    pthread_once(&once_block,once_init_routine);
    pthread_join(child_thread_id,NULL);
}
线程的私有数据
    在进程内的所有线程共享相同的地址空间，任何声明为静态或外部的变量，或在进程堆声明的变量，都可以被进程所有的线程读写。那怎样才能使线程序拥有自己的私有数据呢。
posix提供了一种方法，创建线程键。
函数原形：
int pthread_key_create(pthread_key *key,void(*destructor)(void *));
参数：
key           私有数据键
destructor    清理函数
返回值：
若成功返回0，若失败返回错误编号
   第一个参数为指向一个键值的指针，第二个参数指明了一个destructor函数（清理函数），如果这个参数不为空，那么当每个线程结束时，系统将调用这个函数来释放绑定在这个键上的内存块。这个函数常和函数pthread_once一起使用，为了让这个键只被创建一次。函数pthread_once声明一个初始化函数，第一次调用pthread_once时它执行这个函数，以后的调用将被它忽略。
下面是程序例子： 
#include <pthread.h>
pthread_key_t tsd_key;
pthread_once_t key_once=PTHREAD_ONCE_INIT;
void once_routine(void)
{
int status;
status=pthread_key_create(&tsd_key,NULL);
if(status=0)
    printf(“Key create success! My id is %u/n”,pthread_self());
}
void *child_thread(void *arg)
{
printf(“I’m child,My id is %u/n”,pthread_self());
pthread_once(&key_once,once_routine);
}
int main(int argc,char *argv[ ])
{
pthread_t child_thread_id;
pthread_create(&child_thread_id,NULL,child_thread,NULL);
printf(“I’m father,my id is%u/n”,pthread_self());
pthread_once(&key_once,once_routine);
}
引用：http://blog.csdn.net/denny_233/article/details/7221831
