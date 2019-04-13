
# pthread 的坑 - 嵌入式Linux - CSDN博客

2019年01月05日 00:42:30[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：46个人分类：[Linux																](https://blog.csdn.net/weiqifa0/article/category/1388863)



## 为什么写这篇文章？
[嵌入式Linux：pthread_create 记录线程使用​zhuanlan.zhihu.com](https://zhuanlan.zhihu.com/p/53149003)
这是上篇文章使用了pthread_create来实现闪烁led灯，因为代码写的有偏差导致了一个问题，**就是不能进入深度休眠**

## 问题产生原因
**先了解下互斥锁**
线程之间可以共享内存空间,这意味着不同线程可以访问内存中的同一个变量。然而不同线程在同一时间修改一个内存对象会造成一些不可预知的结果。
为了避免意外,我们需要用到pthread里一个非常重要的数据结构 —— 互斥对象(mutex)。互斥对象在使用时结合互斥锁使用,pthread_mutex_lock 和 pthread_mutex_unlock。
互斥锁是这样工作的:线程B锁定了一个互斥对象(mymutex),如果线程A也试图锁定该互斥对象(mymutex)时,线程A就进入休眠状态。一旦线程B释放了互斥对象（调用 pthread_mutex_unlock()) ,线程A 就能够锁定这个互斥对象（换句话说,线程A就将从 pthread_mutex_lock() 函数调用中返回,同时互斥对象被重新锁定）。同样地,当线程A正锁定互斥对象时,如果线程C试图锁定互斥对象的话,线程C也将临时进入睡眠状态。
`pthread_mutex_lock(&mymutex);
 /*
   直至解锁后,mymutex会阻止另一个试图访问此区域的线程
 */
pthread_mutex_unlock(&mymutex);`对已锁定的互斥对象上调用 pthread_mutex_lock() 的所有线程都将进入睡眠状态,这些睡眠的线程将“排队”访问这个互斥对象。
**从上述可知,mutex会帮助我们锁定一段逻辑区域的访问。但是,如果一个数据对象有多处调用的情况,我们需要根据实际情况,设计统一的接口。**
**上诉问题出现的原因**
![](https://pic4.zhimg.com/80/v2-4d4135f050b8efcbb6896411c3b49aa7_hd.jpg)
图片显示两个线程
左边是控制背光的程序，右边是低电的时候控制led灯闪烁的程序，当右边的程序持有互斥锁的时候，因为里面有休眠函数，导致左边pthreadmutex_lock获取互斥锁的时候发生阻塞，但是因为led线程里面有msleep函数，导致系统进行调度，调度之后有可能还是闪烁led灯的线程持有锁，导致控制背光的线程没有获取锁🔒，背光没有被正常关闭。所以在按下power键休眠后，lcd的背光还是亮着。
我把代码上传到了github上，感兴趣的可以看看
[https://github.com/weiqifa0/linux-light.cpp/blob/master/lights.cpp​github.com](http://link.zhihu.com/?target=https%3A//github.com/weiqifa0/linux-light.cpp/blob/master/lights.cpp)
`pthread_mutex_lock(&g_lock);
if(st_blink)
{
    if (red)
    {
        err |= write_int(RED_LED_PATH, 1);
        msleep(onMS);
        err |= write_int(RED_LED_PATH, 0);
        msleep(offMS);
    }
    if (green)
    {
        err = write_int(GREEN_LED_PATH, 1);
        msleep(onMS);
        err = write_int(GREEN_LED_PATH, 0);
        msleep(offMS);
    }
    if (blue)
    {
        err = write_int(BLUE_LED_PATH, 1);
        msleep(onMS);
        err = write_int(BLUE_LED_PATH, 0);
        msleep(offMS);
    }
    
}
pthread_mutex_unlock(&g_lock);`我们要理解加锁的意义是什么？我刚开始总是担心代码被重入导致问题，但是对于
`static int write_int(char const *path, int value)
{
    int fd;
    static int already_warned;
    already_warned = 0;
    LOGI("write_int: path %s, value %d", path, value);
    fd = open(path, O_RDWR);
    if (fd >= 0) {
        char buffer[20];
        int bytes = sprintf(buffer, "%d\n", value);
        int amt = write(fd, buffer, bytes);
            if(amt < 0)
                ALOGE(">>> write_int failed to write %s %d\n", path,bytes);
        close(fd);
        return amt == -1 ? -errno : 0;
    } else {
        if (already_warned == 0) {
            LOGE(">>> write_int failed to open %s\n", path);
            already_warned = 1;
        }
        return -errno;
    }
}`这句代码而言，如果控制的是红灯,那么如果代码被重入，下次又调用了一次控制红灯的函数，但是因为可能上一次的代码没有走到close(fd),这时候再去open的时候就会导致出错，我们加锁的作用就是保证上次执行完了再去执行下一次。
不过，如果这次是控制红灯，下次重入的时候控制的是绿灯，就没有因为重入导致的问题，因为fd是局部变量，这个局部变量在每一次调用都会分配新的内存空间，因为open的是绿灯，之前没有被open过，也不会出现open失败的情况。
所以对于不同的PATH，我们可以用不同的lock来加锁和解锁，不要用全局锁来操作，引发这样的问题。
## 用实例测试
`#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock;
int sum = 0;
#define msleep(ms)  usleep((ms) * 1000)
void * add1(void * cnt)
{     
	int i = 0;
	char * name = (char*)cnt;
	while(1)
	{
		pthread_mutex_lock(&lock);
		i++;
		printf("%s i=%d sum=%d\n",name,i,sum++);
		msleep(1000);		
		pthread_mutex_unlock(&lock);
	}
	pthread_exit(NULL);
	return 0;
}
void * add2(void *cnt)
{     
	int i = 0;
	char * name = (char*)cnt;
	while(1)
	{
		pthread_mutex_lock(&lock);
		i++;
		printf("%s i=%d sum=%d\n",name,i,sum++);
		msleep(1000);
		pthread_mutex_unlock(&lock);
	}
	pthread_exit(NULL);
	return 0;
}
int main(void)
{ 
	int i;
	pthread_t ptid1,ptid2;
	pthread_mutex_init(&lock,NULL);
	pthread_create(&ptid1,NULL,add1,"Job01");
	pthread_create(&ptid2,NULL,add2,"Job02");
	pthread_mutex_lock(&lock);
	pthread_mutex_unlock(&lock);
	pthread_join(ptid1,NULL);
	pthread_join(ptid2,NULL);
	pthread_mutex_destroy(&lock);
	return 0;
}`**编译命令**
> gcc pth.c -pthread && ./a.out
**如果保留msleep的话，输出如下**
`Job02 i=1 sum=0
Job02 i=2 sum=1
Job02 i=3 sum=2
Job02 i=4 sum=3
Job02 i=5 sum=4
Job02 i=6 sum=5
Job02 i=7 sum=6
Job02 i=8 sum=7
Job02 i=9 sum=8
Job02 i=10 sum=9
Job02 i=11 sum=10
Job02 i=12 sum=11
Job02 i=13 sum=12
Job02 i=14 sum=13
Job02 i=15 sum=14
Job02 i=16 sum=15
Job02 i=17 sum=16
Job02 i=18 sum=17
Job02 i=19 sum=18
Job02 i=20 sum=19
Job02 i=21 sum=20
Job02 i=22 sum=21
Job02 i=23 sum=22
Job01 i=1 sum=23
Job01 i=2 sum=24
Job01 i=3 sum=25
Job01 i=4 sum=26
Job01 i=5 sum=27
Job01 i=6 sum=28
Job01 i=7 sum=29
Job01 i=8 sum=30
Job01 i=9 sum=31
Job01 i=10 sum=32
Job01 i=11 sum=33
Job01 i=12 sum=34
Job01 i=13 sum=35
Job01 i=14 sum=36
Job01 i=15 sum=37
Job01 i=16 sum=38
Job01 i=17 sum=39
Job01 i=18 sum=40
Job01 i=19 sum=41
Job01 i=20 sum=42
Job01 i=21 sum=43
Job01 i=22 sum=44
Job01 i=23 sum=45
Job01 i=24 sum=46
Job02 i=24 sum=47
Job02 i=25 sum=48
Job02 i=26 sum=49
Job02 i=27 sum=50
Job02 i=28 sum=51
Job02 i=29 sum=52
Job02 i=30 sum=53
Job02 i=31 sum=54
Job02 i=32 sum=55
Job02 i=33 sum=56
Job02 i=34 sum=57
Job02 i=35 sum=58
Job02 i=36 sum=59
Job02 i=37 sum=60
Job02 i=38 sum=61
Job02 i=39 sum=62`
看到job2一直在执行很久后才轮到job1执行
**如果把msleep去掉后，执行输出如下**
`Job02 i=17143 sum=17142
Job02 i=17144 sum=17143
Job02 i=17145 sum=17144
Job02 i=17146 sum=17145
Job02 i=17147 sum=17146
Job02 i=17148 sum=17147
Job02 i=17149 sum=17148
Job02 i=17150 sum=17149
Job02 i=17151 sum=17150
Job02 i=17152 sum=17151
Job02 i=17153 sum=17152
Job02 i=17154 sum=17153
Job02 i=17155 sum=17154
Job02 i=17156 sum=17155
Job02 i=17157 sum=17156
Job02 i=17158 sum=17157
Job02 i=17159 sum=17158
Job02 i=17160 sum=17159
Job01 i=1 sum=17160
Job01 i=2 sum=17161
Job01 i=3 sum=17162
Job01 i=4 sum=17163
Job01 i=5 sum=17164
Job01 i=6 sum=17165
Job01 i=7 sum=17166
Job01 i=8 sum=17167
Job01 i=9 sum=17168
Job01 i=10 sum=17169
Job01 i=11 sum=17170
Job01 i=12 sum=17171
Job01 i=13 sum=17172
Job01 i=14 sum=17173
Job01 i=15 sum=17174
Job01 i=16 sum=17175
Job01 i=17 sum=17176
Job01 i=18 sum=17177
Job01 i=19 sum=17178`有延迟和没有延迟，调度的情况完全不一样
**把上面的代码修改成下面的**
`#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock;
int sum = 0;
#define msleep(ms)  usleep((ms) * 1000)
void * add1(void * cnt)
{     
	int i = 0;
	char * name = (char*)cnt;
	while(1)
	{
		i++;
		pthread_mutex_lock(&lock);
		printf("%s i=%d sum=%d\n",name,i,sum++);
		pthread_mutex_unlock(&lock);
		msleep(1000);		
	}
	pthread_exit(NULL);
	return 0;
}
void * add2(void *cnt)
{     
	int i = 0;
	char * name = (char*)cnt;
	while(1)
	{
		i++;
		pthread_mutex_lock(&lock);
		printf("%s i=%d sum=%d\n",name,i,sum++);
		pthread_mutex_unlock(&lock);
		msleep(1000);
	}
	pthread_exit(NULL);
	return 0;
}
int main(void)
{ 
	int i;
	pthread_t ptid1,ptid2;
	pthread_mutex_init(&lock,NULL);
	pthread_create(&ptid1,NULL,add1,"Job01");
	pthread_create(&ptid2,NULL,add2,"Job02");
	pthread_mutex_lock(&lock);
	pthread_mutex_unlock(&lock);
	pthread_join(ptid1,NULL);
	pthread_join(ptid2,NULL);
	pthread_mutex_destroy(&lock);
	return 0;
}`**执行输出如下**
`root@ubuntu:~/project# gcc pth.c -pthread && ./a.out
Job02 i=1 sum=0
Job01 i=1 sum=1
Job02 i=2 sum=2
Job01 i=2 sum=3
Job02 i=3 sum=4
Job01 i=3 sum=5
Job02 i=4 sum=6
Job01 i=4 sum=7
Job02 i=5 sum=8
Job01 i=5 sum=9
Job02 i=6 sum=10
Job01 i=6 sum=11
Job02 i=7 sum=12
Job01 i=7 sum=13
Job02 i=8 sum=14
Job01 i=8 sum=15
Job02 i=9 sum=16
Job01 i=9 sum=17
Job02 i=10 sum=18
Job01 i=10 sum=19`
## 总结
加锁的位置一定要确定好，不能把休眠也加锁了，导致调度变慢，一定要理解了再去做修改，所以我们上面有问题的可以这样修改
`if(st_blink)
{
    if (red)
    {
    	
		pthread_mutex_lock(&g_lock);
        err |= write_int(RED_LED_PATH, 1);
		pthread_mutex_unlock(&g_lock);
		
        msleep(onMS);
		
		pthread_mutex_lock(&g_lock);
        err |= write_int(RED_LED_PATH, 0);
		pthread_mutex_unlock(&g_lock);
        msleep(offMS);
    }
    if (green)
    {
    	
		pthread_mutex_lock(&g_lock);
        err = write_int(GREEN_LED_PATH, 1);
		pthread_mutex_unlock(&g_lock);
        msleep(onMS);
		
		pthread_mutex_lock(&g_lock);
        err = write_int(GREEN_LED_PATH, 0);
		pthread_mutex_unlock(&g_lock);
        msleep(offMS);
    }
    if (blue)
    {
    	
		pthread_mutex_lock(&g_lock);
        err = write_int(BLUE_LED_PATH, 1);
		pthread_mutex_unlock(&g_lock);
        msleep(onMS);
		
		pthread_mutex_lock(&g_lock);
        err = write_int(BLUE_LED_PATH, 0);
		pthread_mutex_unlock(&g_lock);
        msleep(offMS);
    }
    
}`
## 参考
[pthread_mutex_lock等待时间的问题-CSDN论坛​bbs.csdn.net](http://link.zhihu.com/?target=https%3A//bbs.csdn.net/topics/90104019)


