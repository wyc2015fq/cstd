# Linux 多线程1 - writeeee的专栏 - CSDN博客
2016年03月26日 12:57:32[writeeee](https://me.csdn.net/writeeee)阅读数：168
个人分类：[c++](https://blog.csdn.net/writeeee/article/category/5634855)
主要用到的几个函数 
//作用创建线程 
pthread_create() //pthread_t类型实例（这里是个引用）, 在linux中pthread_t被定义为 “unsigned long int”，要创建线程属性，调用的线程函数，线程函数需要的参数 
//等待线程返回，阻塞式等待 
pthread_join() //
//子线程退出函数 
pthread_exit()
编译时命令 
g++ -D_REENTRANT -I /usr/include/nptl thread1.cc -o thread1 -L /usr/lib/nptl -lpthread
```cpp
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
void *thread_func(void *arg);
char message[] ="hello world";
pthread_t a_thread;
void *thread_result;
int main()
{
  int res=0;
  printf("main start running");
  res=pthread_create(&a_thread,NULL,thread_func,(void *)message);
  if(res !=0){
    printf("pthread_create failed res=%d",res);
    exit(EXIT_FAILURE);
  }
  res =pthread_join(a_thread,&thread_result);
  if(res !=0){
    printf("pthread_join failed");
    exit(EXIT_FAILURE);
  }
  printf("Thread joined success");
  printf("message now is %s",message);
  exit(EXIT_SUCCESS);
}
void *thread_func(void *args){
  printf("new thread starting..\n");
  strcpy(message,"aaaa");
  pthread_exit((void *)"the end\n");
}
```
