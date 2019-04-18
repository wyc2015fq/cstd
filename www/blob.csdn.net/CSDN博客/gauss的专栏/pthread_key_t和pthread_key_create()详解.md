# pthread_key_t和pthread_key_create()详解 - gauss的专栏 - CSDN博客
2015年04月23日 21:20:05[gauss](https://me.csdn.net/mathlmx)阅读数：303
个人分类：[服务器架构与网络编程](https://blog.csdn.net/mathlmx/article/category/944322)
pthread_key_t和pthread_key_create()详解
下面说一下线程中特有的线程存储， Thread Specific Data 。线程存储有什么用了？他是什么意思了？大家都知道，在多线程程序中，所有线程共享程序中的变量。现在有一全局变量，所有线程都可以使用它，改变它的值。而如果每个线程希望能单独拥有它，那么就需要使用线程存储了。表面上看起来这是一个全局变量，所有线程都可以使用它，而它的值在每一个线程中又是单独存储的。这就是线程存储的意义。
下面说一下线程存储的具体用法。
1. 创建一个类型为 pthread_key_t 类型的变量。
2. 调用 pthread_key_create() 来创建该变量。该函数有两个参数，第一个参数就是上面声明的 pthread_key_t 变量，第二个参数是一个清理函数，用来在线程释放该线程存储的时候被调用。该函数指针可以设成 NULL ，这样系统将调用默认的清理函数。
3. 当线程中需要存储特殊值的时候，可以调用 pthread_setspcific() 。该函数有两个参数，第一个为前面声明的 pthread_key_t 变量，第二个为 void* 变量，这样你可以存储任何类型的值。
4. 如果需要取出所存储的值，调用 pthread_getspecific() 。该函数的参数为前面提到的 pthread_key_t 变量，该函数返回 void * 类型的值。
下面是前面提到的函数的原型：
int pthread_setspecific(pthread_key_t key, const void *value);
void *pthread_getspecific(pthread_key_t key);
int pthread_key_create(pthread_key_t *key, void (*destructor)(void*));
下面是一个如何使用线程存储的例子：
**[cpp]**[view
 plain](http://blog.csdn.net/lmh12506/article/details/8452700#)[copy](http://blog.csdn.net/lmh12506/article/details/8452700#)[print](http://blog.csdn.net/lmh12506/article/details/8452700#)[?](http://blog.csdn.net/lmh12506/article/details/8452700#)
- #include <malloc.h>
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
- FILE* thread_log = (FILE*) pthread_getspecific (thread_log_key);  
- 
-     fprintf (thread_log, “%s\n”, message);  
- }  
- 
- /* Close the log file pointer THREAD_LOG. */
- 
- void close_thread_log (void* thread_log)  
- 
- {  
-     fclose ((FILE*) thread_log);      
- }  
- 
- void* thread_function (void* args)  
- 
- {  
- char thread_log_filename[20];  
- 
- FILE* thread_log;  
- 
- /* Generate the filename for this thread’s log file. */
- 
-     sprintf (thread_log_filename, “thread%d.log”, (int) pthread_self ());  
- 
- /* Open the log file. */
- 
-     thread_log = fopen (thread_log_filename, “w”);  
- 
- /* Store the file pointer in thread-specific data under thread_log_key. */
- 
-     pthread_setspecific (thread_log_key, thread_log);  
- 
-     write_to_thread_log (“Thread starting.”);  
- 
- /* Do work here... */
- 
- return NULL;  
- }  
- 
- int main ()  
- 
- {  
- 
- int i;  
- 
-     pthread_t threads[5];  
- 
- /* Create a key to associate thread log file pointers in
- 
-     thread-specific data. Use close_thread_log to clean up the file
- 
-     pointers. */
- 
-     pthread_key_create (&thread_log_key, close_thread_log);  
- 
- /* Create threads to do the work. */
- 
- for (i = 0; i < 5; ++i)  
- 
-         pthread_create (&(threads[i]), NULL, thread_function, NULL);  
- 
- /* Wait for all threads to finish. */
- 
- for (i = 0; i < 5; ++i)  
- 
-         pthread_join (threads[i], NULL);  
- 
- return 0;  
- 
- }    
最后说一下线程的本质。
其实在Linux 中，新建的线程并不是在原先的进程中，而是系统通过一个系统调用clone() 。该系统copy 了一个和原先进程完全一样的进程，并在这个进程中执行线程函数。不过这个copy 过程和fork 不一样。copy 后的进程和原先的进程共享了所有的变量，运行环境（clone的实现是可以指定新进程与老进程之间的共享关系，100%共享就表示创建了一个线程）。这样，原先进程中的变量变动在copy
 后的进程中便能体现出来。
