# 《win32多线程程序设计》学习笔记五 - 一世豁然的专栏 - CSDN博客





2017年03月28日 20:52:19[一世豁然](https://me.csdn.net/Explorer_day)阅读数：362








一、干净的终止一个线程

1、利用TerminateThread()放弃一个线程




![](https://img-blog.csdn.net/20170327210458285?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


 注：尽量不要使用TerminateThread()函数。




2、几个可能的终止一个线程的方法

 1）、使用信号（但是win32中煤有真正的signals）

 2）、跨线程，丢出异常状况（win32 AOU中没有此方法）

 3）、设立一个标记







二、线程优先权

1、优先权

 1）、Win32有“优先权”概念，用以决定下一个获得CPU时间的线程是谁。较高优先权的线程必然获得更多的CPU时间。

 2）、Win32优先权是以数值表现的，并以进程的“优先权类别”、线程的“优先权”层级和操作系统当时采用的“动态提升”作为计算基准。




2、优先权类别

 1）、“优先权类别”是进程的属性之一。这个属性可以表现出这一进程和其他进程比较之下的重要性。

 2）、Win32提供四种优先权类别，每一个类别对应一个基本的优先权层级。四个优先权类别如下

![](https://img-blog.csdn.net/20170328201319181?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


        I、大部分程序使用NORMAL_PRIORITY_CLASS。少数情况下才会考虑使用其他类别。

 3）、优先权适用于进程而非线程。




3、优先权层级

 1）、线程的优先权层级是对进程的优先权类别的一种修改使你能够调整同一进程内的各线程的相对重要性。

 2）、一共有其中优先权层级：

![](https://img-blog.csdn.net/20170328202409218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


 3）、优先权层级可以利用以下函数改变

![](https://img-blog.csdn.net/20170328202603953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170328202612938?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


 4）、线程目前的优先权层级可以利用如下函数改变：

![](https://img-blog.csdn.net/20170328202730000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





4、KERNEL32.DLL中的优先权（略）




5、动态提升

 1）、决定线程真正优先权的最后一个因素是其目前的动态提升值。所谓动态提升是对优先权的一种调整，使系统能够机动对待线程，以强化程序的可用性。







三、初始化一个线程

1、使用线程的一个常见问题就是如何能够在一个线程开始运行之前适当的将它初始化。理由如下

 1）、调整优先权

 2）、为了在SMP系统中设定线程比较喜欢的CPU。




2、初始化线程的解决之道就是CreateThread()的第五个参数，它允许你指定线程诞生时的属性。




3、一旦线程设置妥当，调用以下函数开始执行

![](https://img-blog.csdn.net/20170328204602495?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





4、挂起一个线程

 1）、以下函数允许调用端指定一个线程睡眠（挂起）。直到有人调用ResumeThread()，线程才会醒来

![](https://img-blog.csdn.net/20170328205004261?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


 2）、SuspendThread()的最大用途就是用来协助撰写调试器。



