# uCOS II多任务调度的启动流程 - xiahouzuoxin - CSDN博客





2013年08月15日 14:35:12[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：3539
个人分类：[uCOS-II](https://blog.csdn.net/xiahouzuoxin/article/category/1172812)









创建了一个主任务MainTask，在MainTask中又创建了2个任务LED1Task和LED2Task。

移植uCOS II的内核为为Cortex-M3，芯片为STM32F103CB。




启动多任务调度的流程如下图。虚线流程表示函数内执行的操作。




![](https://img-blog.csdn.net/20130815143255187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



