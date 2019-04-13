
# Linux多线程——使用互斥量同步线程 - 嵌入式Linux - CSDN博客

2013年09月06日 16:53:10[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：792个人分类：[Linux																](https://blog.csdn.net/weiqifa0/article/category/1388863)


前文再续，书接上一回，在上一篇文章：Linux多线程——使用信号量同步线程中，我们留下了一个如何使用互斥量来进行线程同步的问题，本文将会给出互斥量的详细解说，并用一个互斥量解决上一篇文章中，要使用两个信号量才能解决的只有子线程结束了对输入的处理和统计后，主线程才能继续执行的问题。

一、什么是互斥量

互斥量是另一种用于多线程中的同步访问方法，它允许程序锁住某个对象，使得每次只能有一个线程访问它。为了控制对关键代码的访问，必须在进入这段代码之前锁住一个互斥量，然后在完成操作之后解锁。

二、互斥量的函数的使用

它们的定义与使用信号量的函数非常相似，它们的定义如下：
**[cpp]**[view
 plain](http://blog.csdn.net/ljianhui/article/details/10875883#)[copy](http://blog.csdn.net/ljianhui/article/details/10875883#)[print](http://blog.csdn.net/ljianhui/article/details/10875883#)[?](http://blog.csdn.net/ljianhui/article/details/10875883#)

\#include <pthread.h>
intpthread_mutex_init(pthread_mutex_t *mutex,constpthread_mutexattr_t *mutexattr);
intpthread_mutex_lock(pthread_mutex_t *mutex);
intpthread_mutex_unlock(pthread_mutex_t *mutex);
intpthread_mutex_destroy(pthread_mutex_t *mutex);
它们的意义就如它们的名字所示的那样，成功时返回0，失败时返回错误代码，它们并不设置errno。

pthread_mutex_init函数中的参数mutexattr指定互斥量的属性，在这里我们并不关心互斥量的属性，所以把它设置为NULL，使用默认属性即可。同样的，pthread_mutex_lock和pthread_mutex_unlock都是原子操作，如果一个线程调用pthread_mutex_lock试图锁住互斥量，而该互斥量，又被其他线程锁住（占用），则该线程的pthread_mutex_lock调用就会阻塞，直到其他线程对该互斥量进行解锁，该线程才能获得该互斥量，pthread_mutex_lock调用才会返回。

注意，使用互斥量的默认属性，如果程序试图对一个已经加锁的互斥量调用pthread_mutex_lock，程序就会阻塞，而又因为拥有互斥量的这个线程正是现在被阻塞的线程，所以这个互斥量就永远不会被解锁，也就是说，程序就会进入死锁的状态。在使用时要多加注意，确保在同一个线程中，对加锁的互斥再次进行加锁前要对其进行解锁。

三、使用互斥量进行线程同步

下面以一个简单的多线程程序来演示如何使用互斥量来进行线程同步。在主线程中，我们创建子线程，并把数组msg作为参数传递给子线程，然后主线程调用函数pthread_mutex_lock对互斥量加锁，等待输入，输入完成后，调用函数pthread_mutex_unlock对互斥量解锁，从而使线程函数中的对互斥量加锁的pthread_mutex_lock函数返回并执行子线程中的代码。线程函数在把字符串的小写字母变成大写并统计输入的字符数量之后，它调用pthread_mutex_unlock对互斥量解锁，使主线程能够继续获得互斥量（即对其加锁函数返回），再次执行输入功能直到主线程再次调用pthread_mutex_unlock对其解锁，一直如此重复，直到输入end。

源文件为lockthread.c，源代码如下：
**[cpp]**[view
 plain](http://blog.csdn.net/ljianhui/article/details/10875883#)[copy](http://blog.csdn.net/ljianhui/article/details/10875883#)[print](http://blog.csdn.net/ljianhui/article/details/10875883#)[?](http://blog.csdn.net/ljianhui/article/details/10875883#)

\#include <unistd.h>
\#include <pthread.h>
\#include <stdlib.h>
\#include <stdio.h>
\#include <string.h>

//声明线程函数和互斥量
void* thread_func(void*msg);
pthread_mutex_t mutex;

\#define MSG_SIZE 512

intmain()
{
intres = -1;
pthread_tthread;
void*thread_result = NULL;
charmsg[MSG_SIZE] = {'\0'};
//初始化互斥量，使用默认的互斥量属性
res = pthread_mutex_init(&mutex, NULL);
if(res != 0)
{
perror("pthread_mutex_init failed\n");
exit(EXIT_FAILURE);
}
//创建子线程，并把msg作为线程函数的参数传递给thread_func
res = pthread_create(&thread, NULL, thread_func, msg);
if(res != 0)
{
perror("pthread_create failed\n");
exit(EXIT_FAILURE);
}
//输入字符串，以串‘end’结束
printf("Input some test. Enter 'end' to finish\n");
//把互斥量mutex加锁，以确保同一时间只有该线程可以访问msg中的数据
pthread_mutex_lock(&mutex);
while(strcmp("end\n", msg) != 0)
{
if(strncmp("TEST", msg, 4) == 0)
{
strcpy(msg,"copy_data\n");
}
else
{
fgets(msg, MSG_SIZE, stdin);
}
//把互斥量mutex解锁，让其他的线程可以访问msg中的数据
pthread_mutex_unlock(&mutex);
sleep(1);//休眠1秒再继续循环，让其他线程有执行的机会
pthread_mutex_lock(&mutex);
}
pthread_mutex_unlock(&mutex);
printf("\nWaiting for thread finish...\n");
//等待子线程结束
res = pthread_join(thread, &thread_result);
if(res != 0)
{
perror("pthread_join failed\n");
exit(EXIT_FAILURE);
}
printf("Thread joined\n");
//清理互斥量
pthread_mutex_destroy(&mutex);
exit(EXIT_SUCCESS);
}
void* thread_func(void*msg)
{
inti = 0;
char*ptr = msg;
sleep(1);
//把互斥量mutex加锁，以确保同一时间只有该线程可以访问msg中的数据
pthread_mutex_lock(&mutex);
while(strcmp("end\n", msg) != 0)
{
//把小写字母变成大写
for(i = 0; ptr[i] !='\0'; ++i)
{
if(ptr[i] >='a'&& ptr[i] <='z')
{
ptr[i] -='a'-'A';
}
}
printf("You input %d characters\n", i-1);
printf("To uppercase: %s\n", ptr);
//把互斥量mutex解锁，让其他的线程可以访问msg中的数据
pthread_mutex_unlock(&mutex);
sleep(1);//休眠1秒再继续循环，让其他线程有执行的机会
pthread_mutex_lock(&mutex);
}
pthread_mutex_unlock(&mutex);
//退出线程
pthread_exit(NULL);
}
运行结果如下：
![](https://img-blog.csdn.net/20130902000834828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGppYW5odWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

程序分析：

这个程序的工作流程已经说得非常清楚了，这里先来说说在main函数和线程函数thread_func中while循环中的sleep(1)语句的作用。可能很多人会认为这个sleep(1)是为了让子线程完成其处理和统计功能，所以要让主线程休眠1秒钟来等待子线程的处理统计工作的完成。的确在这里子线程进行的工作十分简单，1秒钟内的确可以处理统计完毕。但是这里的sleep(1)并不是为了实现这个功能，这两个循环中的sleep(1)是为了让其他的线程有机会被执行到，如果在一次的加锁和解锁之间没有这条语句的话，则当前的线程将会一直在循环中获得互斥量，因为其他的线程没有执行它的代码的时间，所以就要用这样的一条语句来给其他的线程一个运行的机会。如果子线程的执行时间超过1秒，这个程序还是会正常运行。

以这个例子来说，在主线程中，当输入数据完毕并对互斥量解锁之后，并不马上循环对其加锁，此时子线程就有了执行的机会，它会对互斥量进行加锁，同样地，当它处理统计完输入的数据后，它在进入下一次循环前，也休眠1秒，让主线程有机会再次运行。而主线程什么时候能够执行，取决于子线程何时对互斥量进行解锁。因为如果子线程拥有（锁住）互斥量，则主线程中函数pthread_mutex_lock就不会返回，使主线程处于阻塞状态。

换句话来说，就是只有子线程结束了对输入的处理和统计后，主线程才能继续执行，向msg中写入数据。看到这里，你应该知道之前在使用信号量时，我们多用一个信号量也是为了达到这个目的。所以当我们输入TEST时，程序有两个输入，但还是能正常运行，同样解决了之前使用一个信号量时所带来的问题。

信号量和互斥量的作用都是保护代码段的互斥设备，它们也非常相似。但在本例中，与使用信号量相比，实现同样的功能，如果使用信号量的话，则需要两个信号量，而使用互斥量的话，只需要一个。可以说在本例中，使用互斥量更简单。但是我觉得使用互斥量更容易犯错，我们可以看到在这个例子中，我们需要使用sleep语句来让其他线程获得执行的机会，但是在使用信号量的程序，它并不需要使用sleep，相对来说比较直观。我知道可能是我的实现方法不好，但是对于使用互斥量来说，我想了很久也想不到不使用sleep的方法。




undefined reference to 'pthread_create'
undefined reference to 'pthread_join'
问题解决：
在编译中要加 -lpthread参数
gcc thread.c -o thread -lpthread
thread.c为你些的源文件，不要忘了加上头文件\#include<pthread.h>


我自己的试验例子
```python
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//声明线程函数和互斥量
void* thread_func(void *msg);
pthread_mutex_t mutex;

#define MSG_SIZE 512

int main()
{
	int res = -1;
	pthread_t thread;
	void *thread_result = NULL;
	char msg[MSG_SIZE] = {'\0'};
	//初始化互斥量，使用默认的互斥量属性
	res = pthread_mutex_init(&mutex, NULL);
	if(res != 0)
	{
		perror("pthread_mutex_init failed\n");
		exit(EXIT_FAILURE);
	}
	//创建子线程，并把msg作为线程函数的参数传递给thread_func
	res = pthread_create(&thread, NULL, thread_func, msg);
	if(res != 0)
	{
		perror("pthread_create failed\n");
		exit(EXIT_FAILURE);
	}
	//输入字符串，以串‘end’结束
	printf("Input some test. Enter 'end' to finish\n");
	//把互斥量mutex加锁，以确保同一时间只有该线程可以访问msg中的数据
	pthread_mutex_lock(&mutex);
	while(strcmp("end\n", msg) != 0)
	{
		if(strncmp("TEST", msg, 4) == 0)/*输入的msg是TEST*/
		{
			printf("www=============================test1\n");
			strcpy(msg, "copy_data\n");
		}
		else/*输入的msg不是TEST*/
		{
			printf("www=============================test2\n");
			fgets(msg, MSG_SIZE, stdin);/*等待用户输入*/
		}
		//把互斥量mutex解锁，让其他的线程可以访问msg中的数据
		pthread_mutex_unlock(&mutex);
		sleep(1);//休眠1秒再继续循环，让其他线程有执行的机会
		pthread_mutex_lock(&mutex);
	}
	pthread_mutex_unlock(&mutex);
	printf("\nWaiting for thread finish...\n");
	//等待子线程结束
	res = pthread_join(thread, &thread_result);
	if(res != 0)
	{
		perror("pthread_join failed\n");
		exit(EXIT_FAILURE);
	}
	printf("Thread joined\n");
	//清理互斥量
	pthread_mutex_destroy(&mutex);
	exit(EXIT_SUCCESS);
}
void* thread_func(void *msg)/*子线程的功能是把主线程的传下来的字母变成大写字母*/
{
	int i = 0;
	char *ptr = msg;
	sleep(1);
	//把互斥量mutex加锁，以确保同一时间只有该线程可以访问msg中的数据
	pthread_mutex_lock(&mutex);
	while(strcmp("end\n", msg) != 0)
	{
		//把小写字母变成大写
		for(i = 0; ptr[i] != '\0'; ++i)
		{
			if(ptr[i] >= 'a' && ptr[i] <='z')
			{
				ptr[i] -= 'a' - 'A';
			}
		}
		printf("You input %d characters\n", i-1);
		printf("To uppercase: %s\n", ptr);
		//把互斥量mutex解锁，让其他的线程可以访问msg中的数据
		pthread_mutex_unlock(&mutex);
		sleep(1);//休眠1秒再继续循环，让其他线程有执行的机会
		pthread_mutex_lock(&mutex);
	}
	pthread_mutex_unlock(&mutex);
	//退出线程
	pthread_exit(NULL);
}
```

```python
wqf@wqf-System-Product-Name:/worksen/lddexamples/muax$ gcc  muax.c -o muax -lpthread
wqf@wqf-System-Product-Name:/worksen/lddexamples/muax$ ./muax 
Input some test. Enter 'end' to finish
www=============================test2
qqq
You input 3 characters
To uppercase: QQQ
www=============================test2
TEST
You input 4 characters
To uppercase: TEST
www=============================test1
You input 9 characters
To uppercase: COPY_DATA
www=============================test2
eee
You input 3 characters
To uppercase: EEE
www=============================test2
end
Waiting for thread finish...
Thread joined
wqf@wqf-System-Product-Name:/worksen/lddexamples/muax$
```




