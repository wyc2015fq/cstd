# 《离散时间信号处理学习笔记》—z变换（二） - 一世豁然的专栏 - CSDN博客





2018年09月09日 11:40:04[一世豁然](https://me.csdn.net/Explorer_day)阅读数：413








注：本博客是基于奥本海姆《离散时间信号处理》第三版编写，主要是为了自己学习的复习与加深。





一、z变换性质

1、以下讨论中，将X(z)记为x[n]的z变换，X(z)的收敛域用Rx表示，即

![](https://img-blog.csdn.net/2018090910160657?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

Rx代表一个满足![](https://img-blog.csdn.net/20180909101659184?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)的z值得集合。对于设计两个序列及其相关z变换的性质，这些变换对将记为

![](https://img-blog.csdn.net/20180909101754596?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



1、线性性质表明

![](https://img-blog.csdn.net/20180909101852132?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

正如所指出的，为了将和的z变换分解为z变换的和，z必须位于两个收敛域中。因此，收敛域至少是两个单一收敛域的交际。



2、时移

![](https://img-blog.csdn.net/20180909102145883?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其中，n0为以整数。若n0为正，原序列x[n]被右移；若n0为负，x[n]则向左移。与线性性质一样，收敛域可能由于z-n0因子改变在z=0或z=∞处极点的数目而发生变化。



3、用指数序列相乘

指数相乘性质在数学上的表示为

![](https://img-blog.csdn.net/20180909102553169?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

符号收敛域=![](https://img-blog.csdn.net/20180909102710512?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)表示收敛域Rx，但用|z0|改变了尺度；也就是说，如果Rx满足rR<|z|<rL的z值集合，那么|z0|Rx就是满足|![](https://img-blog.csdn.net/20180909102909730?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)的z值集合。



4、X(z)的微分

微分性质表示为

![](https://img-blog.csdn.net/20180909103035607?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



5、复数序列的共轭

共轭性质表示为

![](https://img-blog.csdn.net/2018090910314844?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



6、时间倒置

时间倒置性质是指

![](https://img-blog.csdn.net/20180909103234325?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其中，收敛域=1/Rx意指Rx的颠倒；即如果Rx是在![](https://img-blog.csdn.net/20180909103433926?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)内z值得集合，那么X*(1/z*)的收敛域就是在![](https://img-blog.csdn.net/20180909103536621?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)内z值得集合。



7、序列卷积

卷积性质有

![](https://img-blog.csdn.net/20180909103650517?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



8、z变换性质表

![](https://img-blog.csdn.net/20180909103754760?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



二、z变换与LTI系统

1、一个LTI系统可以表征为输入信号x[n]和h[n]的卷积y[n]=x[n]*h[n]，其中h[n]是系统对单位脉冲序列的响应。根据卷及性质，得到y[n]的z变换为

![](https://img-blog.csdn.net/20180909105107408?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)式(3.1)

式中，H(z)和X(z)分别为h[n]和x[n]的z变换，z变换H[z]称为脉冲响应为h[n]得LTI系统的系统函数。



2、对于用差分方程描述得LTI系统而言，z变换将十分有用。具有如下形式的差分方程;

![](https://img-blog.csdn.net/201809091054574?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)式(3.2)

所描述的系统，当输入在n=0时刻以前为零且在输入变为非零之前满足初始松弛状态条件时，系统为因果LTI系统，所谓初始松弛·条件，也就是

![](https://img-blog.csdn.net/20180909105718268?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

都假设为零。虽然带有初始松弛条件的差分方程可以用来定义LTI系统，但仍希望知道其对应的系统函数。对式（3.2）应用线性性质和时移性质，可以得到

![](https://img-blog.csdn.net/20180909110213355?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)式(3.3)

求解Y(z)，并用X(z)和差分方程的系数进行比较表示，可以得到

![](https://img-blog.csdn.net/20180909110441367?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)式(3.4)

通过对式(3.1)和式(3.4)进行比较可以得到，对于式(3.2)描述的LTI系统来说，其系统函数为

![](https://img-blog.csdn.net/2018090911072084?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)式（3.5）

因为由式(3.2)差分方程定义的系统是因果的，由以前套路你可知，式(3.5)中的H(z)必须具有一个形式为|z|>rR的收敛域，且由于收敛域不能包含极点，因此rR必须等于H(z)举例原点最远的那个极点的幅度。如果rR<1，即所有的极点都位于单位圆内部，那么系统是稳定的，且系统的频率响应可通过将式（3.5）中的z设置为z=ehw来获得。



3、注意到，如果将式(3.2)用下面的等效形式来表示；

![](https://img-blog.csdn.net/20180909111425500?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)式(3.6)

那么式(3.5)，即将系统函数(和稳定系统的评率响应)表示为两个z-1为变量的多项式之比的形式，可以直接进行简化，可以看出分子是有关输入的系数和延迟项的z变化表达式，而分母表示了有关输出的系数和延迟项。类似的，当给出了形如式(3.5)的z-1多项式之比的系统函数后，可以直接写出具有式(3.6)形式的差分方程，再将其斜为式(3.2)的形式从而得到递归实现结构。





三、单边z变换

1、单边z变换的定义如下；

![](https://img-blog.csdn.net/2018090911291817?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

单边z变换与双边z变换的不同之处在于，求和下界总是固定为0，不考虑n<0所对应的x[n]值。如果n<0时x[n]=0，那么单边z变换和双边z变换是一致的，否则，如果当n<0时x[n]部位0，则两者不同。



2、所有单边z变换的收敛域都有具有|z|>rR的形式，且对于有理单边z变换，收敛域的边界可以由z平面上举例原点最远的极点来定义。在数字信号处理应用中，形如式(3.2)的差分方程通常都是在初始松弛条件下使用的。然而，在某些情况下，非初始松弛条件可能会发生。此时，单边z变换的线性性质则称为十分有用的工具。对于单边z变换，线性性质与双边z变换的线性性质相同，但是以性质与双边z变换不同。



