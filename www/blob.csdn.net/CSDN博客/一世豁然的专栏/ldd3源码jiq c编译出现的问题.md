# ldd3源码jiq.c编译出现的问题 - 一世豁然的专栏 - CSDN博客





2014年12月15日 21:29:27[一世豁然](https://me.csdn.net/Explorer_day)阅读数：565标签：[Linux驱动																[kernel](https://so.csdn.net/so/search/s.do?q=kernel&t=blog)](https://so.csdn.net/so/search/s.do?q=Linux驱动&t=blog)
个人分类：[linux驱动](https://blog.csdn.net/Explorer_day/article/category/2652125)








此博客转载于：http://blog.csdn.net/makethyme/article/details/6940203





本人使用的环境是uname -r 为2.6.35.6-45.fc14.i686

编译中，首先碰到的就是现在头文件<linux/config.h>已经不存在，搜索看看就知道这么解决了。

另外一个就是<linux/workqueue.h>中一些函数和结构体已经改变了：

extern int schedule_delayed_work(struct delayed_work *work, unsigned long delay);

extern int schedule_work(struct work_struct *work);

struct delayed_work {

     struct work_struct work;

     struct timer_list timer;

};




为了尽可能少修改源代码，修改如下：

1）定义一个新的结构体

static struct delayed_work_struct_ldd3_t {

    struct delayed_work delayed_work;

    void *privatedata;

} delayed_work_struct_ldd3;

在该结构体中，privatedata指向以前的jiq_data结构。而struct delayed_work delayed_work则包含

结构work_struction work.



2）由于使用jiq_data的地方很多，保留该变量，但是在jiq_init()函数中，将该变量已delayed_work_struct_ldd3关联起来。

    delayed_work_struct_ldd3.privatedata = (void*)&jiq_data;



3）源代码中好像还以一个宏定义为

#define SCHEDULE_QUEUE ((task_queue *) 1)

在此文件中应该没用，可以注释掉。



4）在函数jiq_init中，将以前的INIT_WORK(…)替换为

INIT_DELAYED_WORK(&(delayed_work_struct_ldd3.delayed_work),jiq_print_wq);

如果仍然使用INIT_WORK，则/proc/jiqwq能够正常工作。但是/proc/jiqwqdelay不能正常工作。即

schedule_delayed_work需要INIT_DELAYED_WORK而不是INIT_WORK配合。



5）此情况下jiq_print_wq变化较大：

static void jiq_print_wq(struct work_struct *ptr)    //传入的参数类型不再是void *，而是struct work_struct *了

{

     //通过work_struct得到delayed_work，再得到delayed_work_struct_ldd3_t，从而得到jiq_data数据结构。

    struct delayed_work *p_delayed_work = to_delayed_work(ptr);

    struct delayed_work_struct_ldd3_t *p_delayed_work_struct_ldd3 = 

            container_of(p_delayed_work, struct delayed_work_struct_ldd3_t,delayed_work);

    struct clientdata *data = (struct clientdata *) (p_delayed_work_struct_ldd3->privatedata);

    if (! jiq_print (data))

          return;


    if (data->delay)

         schedule_delayed_work(p_delayed_work, data->delay);

    else

         schedule_work(ptr);

}




6）函数jiq_read_wq中

   schedule_work(&jiq_work)修改为schedule_work(&(delayed_work_struct_ldd3.delayed_work.work));



7）函数jiq_read_wq_delayed中

  schedule_delayed_work(&jiq_work,delay)修改为schedule_delayed_work(&(delayed_work_struct_ldd3.delayed_work),delay);






编译运行可得到：

cat /proc/jiqwqdelay

    time  delta preempt   pid cpu command

  3385115     1       0     6   0 events/0

  3385116     1       0     6   0 events/0

  3385117     1       0     6   0 events/0

  3385118     1       0     6   0 events/0

  3385119     1       0     6   0 events/0

cat /proc/jiqwq

    time  delta preempt   pid cpu command

  3403777     1       0     6   0 events/0

  3403777     0       0     6   0 events/0

  3403777     0       0     6   0 events/0

  3403777     0       0     6   0 events/0

  3403777     0       0     6   0 events/0

  3403777     0       0     6   0 events/0





