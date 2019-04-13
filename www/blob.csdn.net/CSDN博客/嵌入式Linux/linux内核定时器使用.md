
# linux内核定时器使用 - 嵌入式Linux - CSDN博客

2015年02月05日 12:21:58[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：411



//加入头文件
\#include <linux/hrtimer.h>
//init owen.wei modify time

struct hrtimer timer_ptt;

//初始化定时器
hrtimer_init(&timer_ptt, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
timer_ptt.function = timer_ptt_interrupt;
//

//定义定时中断函数
static enum hrtimer_restart timer_ptt_interrupt(struct hrtimer *timer)
{
static unsigned int c=0;
c++;
printk("%s,%d,c=%d\n",__func__,__LINE__,c);
hrtimer_start(&timer_ptt, ktime_set(1, 0), HRTIMER_MODE_REL);
return HRTIMER_NORESTART;
}
//开始起动定时器
hrtimer_start(&timer_ptt, ktime_set(1, 0), HRTIMER_MODE_REL);




