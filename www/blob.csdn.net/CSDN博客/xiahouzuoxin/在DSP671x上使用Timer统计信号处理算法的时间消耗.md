# 在DSP671x上使用Timer统计信号处理算法的时间消耗 - xiahouzuoxin - CSDN博客





2014年03月30日 23:57:17[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：2688








## 代码实例

hTimer=TIMER_open(TIMER_DEVANY,);/* open a timer *//* Configure the timer. 1 count corresponds to 4 CPU cycles in C67 *//* control period initial value */TIMER_configArgs(hTimer,0x000002C0,0xFFFFFFFF,0x00000000);/* Compute the overhead of calling the timer. */start=TIMER_getCount(hTimer);/* to remove L1P miss overhead */start=TIMER_getCount(hTimer);/* get count */stop=TIMER_getCount(hTimer);/* get count */overhead=stop-start;start=TIMER_getCount(hTimer);/* get count *//* Call a function here. *//* get count */diff=(TIMER_getCount(hTimer)–start)?overhead;TIMER_close(hTimer);printf(”%dcycles\n”,diff*);


注意，C6713上定时器的一个计数值对应4个时钟周期。


overhead表示Timer本身配置过程的时钟消耗，所以有diff计算，

diff=(TIMER_getCount(hTimer)–start)?overhead;/* get count */



