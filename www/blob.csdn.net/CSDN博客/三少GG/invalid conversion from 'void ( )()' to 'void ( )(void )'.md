# invalid conversion from 'void* (*)()' to 'void* (*)(void*)' - 三少GG - CSDN博客
2013年11月01日 08:36:32[三少GG](https://me.csdn.net/scut1135)阅读数：2584
void *thread1()
 if((temp = pthread_create(&thread[0], NULL, thread1, NULL)) != 0)  
提示：invalid conversion from 'void* (*)()' to 'void* (*)(void*)'
=============
改成： void *thread1(void *)
void* (*)(void*)：  （*）表示这里需要一个函数指针， (void*)表示参数类型需要为void*
参考：
http://groups.google.com/group/comp.programming.threads/browse_thread/thread/8fddb216a36daa87/b03dda5b143e8260?#b03dda5b143e8260
The pthread_* functions are C language.  The C++ prototype for a thread function is
extern "C" void *thread_function(void *);
===============================================================================================
void main_thread ( void *ptr )
{
 char *message1 = "Thread 1";
 char *message2 = "Thread 2";
 pthread_t thread3, thread4;
 int iret3, iret4;
 iret3 = pthread_create(&thread3, NULL, (void *)&print_message_function, (void *) message1);
 iret4 = pthread_create(&thread4, NULL, (void *)&print_message_function, (void *) message2);
}
void print_message_function( void *ptr )
{
 char *message;
 message = (char *) ptr;
 printf("%s \n", message);
}
以上这段代码在用g++编译是会有如下错误：
invalid conversion from `void*' to `void*(*)(void*)
我们注意在Posix定义建立线程函数的原型：
extern int pthread_create (pthread_t *__restrict __threadp,
 __const
 pthread_attr_t *__restrict __attr,
 void
 *(*__start_routine) (void *),
 void
 *__restrict __arg) __THROW;
这个**call中的第三个参数是载入一个****函数，这个函数有一个参数可以传入，返回一个通用指针。**
我们再来看看原来函数中是怎样调用这一原型的，基本上就是类似一下的调用方式：
(void *)&main_thread
这个表达式的含义：取一个指向函数main_thread的指针，然后将其转换为一个通用指针。
这就是说显然上述两个事情并非同一个事情，故而正确的调用方式是
 iret3 = pthread_create(&thread3, NULL, print_message_function, (void *) message1);
处理函数的定义如下：
void* print_message_function( void *ptr )
值得注意的是**在gcc编译时不会出错，但是用g++就会有问题**，究其原因就是C语言编译器允许隐含性的将一个通用指针转换为任意类型的指针，而C＋＋不允许。
