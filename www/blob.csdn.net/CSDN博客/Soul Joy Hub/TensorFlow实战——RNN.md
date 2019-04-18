# TensorFlow实战——RNN - Soul Joy Hub - CSDN博客

2017年06月12日 22:30:16[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：1043


[http://blog.csdn.net/u011239443/article/details/73136866](http://blog.csdn.net/u011239443/article/details/73136866)

# RNN

循环神经网络（RNN）的特殊的地方在于它保存了自己的状态，每次数据输入都会更新状态，输出预测值，并输出更新后的状态，和批数据一起作为输入：

![这里写图片描述](https://img-blog.csdn.net/20170612215524845?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

如上图，U为数据输入，V为预测值输出，W为状态值输出并输入下一轮。 

由于现实中不能无限的迭代下去，我们将其展开，并在有限次迭代后结束：

![这里写图片描述](https://img-blog.csdn.net/20170612215857913?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

# 例子

RNN的状态是通过一个向量来表示的，设该向量的维度为`n`，输入数据`x`的维度为`m`，则参数个数为$(n+m)*n+n+n*m+m$。我们来举个例子,$n=2,m=1$。

## 第1轮

设一开始的状态为$w_0 = (0,0)$ , $x_1=(1)$ , RNN 和将其合并成一个向量$y_1 = (0,0,1) $，则参数矩阵$A_1$ 为一个$(n+m)*n$即$3*2$的矩阵。设：

$      \begin{equation}    A_1=\left[    \begin{matrix}    0.1&0.2\\    0.3&0.4\\    0.5&0.6    \end{matrix}    \right]    \end{equation}   $

设，维度为n偏置向量：$b_1=(0.1,-0.1)$

$\large \color{blue}{w_1 = y_1*A_1+b_1 = (0.537,0.462)}$

即，输出到下一轮的状态向量为(0.537,0.462)，而在这轮该状态还会与参数$n*m$参数矩阵$A_2$相乘，再加上偏置向量m的偏置向量$b_2$。设：

$      \begin{equation}    A_2=\left[    \begin{matrix}    1\\    2    \end{matrix}    \right]    \end{equation}   $

$b_2=(0.1)$

$\large \color{blue}{o_1 = w_1*A_2+b_2 = (1.56)}$

## 第2轮

设$x_2 = 2$， $y_2$为$ w_1$和$x_2$合并，即 (0.54,0.46,2)

$\large \color{blue}{w_2 = y_2*A_1+b_1 = (0.860,0.884)}$

$\large \color{blue}{o_2 = w_2*A_2+b_2 = (2.73)}$

# RNN扩展

## 双向RNN

顾名思义，双向的状态传播：

![这里写图片描述](https://img-blog.csdn.net/20170612234430158?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 深度RNN

![这里写图片描述](https://img-blog.csdn.net/20170612234526065?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

要注意的是，RNN的`dropout`不会在深度方向上使用，只会在状态输出到下一轮的时候使用。

![这里写图片描述](https://img-blog.csdn.net/20170613001147639?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

