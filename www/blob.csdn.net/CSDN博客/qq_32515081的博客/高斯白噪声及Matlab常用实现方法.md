# 高斯白噪声及Matlab常用实现方法 - qq_32515081的博客 - CSDN博客
2018年03月09日 17:22:19[TengAdventure](https://me.csdn.net/qq_32515081)阅读数：27278
http://blog.sina.com.cn/s/blog_4d7c97a00101cv5h.html
一、概念
英文名称：white Gaussian noise; WGN
定义：均匀分布于给定频带上的高斯噪声；
 所谓高斯白噪声中的高斯是指概率分布是正态函数，而白噪声是指它的二阶矩不相关，一阶矩为常数，是指先后信号在时间上的相关性。这是考察一个信号的两个不同方面的问题。
 高斯白噪声：如果一个噪声，它的幅度服从高斯分布，而它的功率谱密度又是均匀分布的，则称它为高斯白噪声。
 热噪声和散粒噪声是高斯白噪声。
 二、matlab举例
Matlab有两个函数可以产生高斯白噪声，wgn( )和awgn( )。
1. WGN：产生高斯白噪声 
y = wgn(m,n,p)
 y = wgn(m,n,p) %产生一个m行n列的高斯白噪声的矩阵，p以dBW为单位指定输出噪声的强度。 
y = wgn(m,n,p,imp)
 y = wgn(m,n,p,imp) %以欧姆(Ohm)为单位指定负载阻抗。 
y = wgn(m,n,p,imp,state)
 y = wgn(m,n,p,imp,state) %重置RANDN的状态。
在数值变量后还可附加一些标志性参数： 
y = wgn(…,POWERTYPE) 指定p的单位。POWERTYPE可以是'dBW', 'dBm'或'linear'。线性强度(linear power)以瓦特(Watt)为单位。 
y = wgn(…,OUTPUTTYPE) 指定输出类型。OUTPUTTYPE可以是'real'或'complex'
2. AWGN：在某一信号中加入高斯白噪声 
y = awgn(x,SNR)
 y = awgn(x,SNR) %在信号x中加入高斯白噪声。信噪比SNR以dB为单位。x的强度假定为0dBW。如果x是 复数，就加入复噪声。
y = awgn(x,SNR,SIGPOWER) 如果SIGPOWER是数值，则其代表以dBW为单位的信号强度；如果SIGPOWER为'measured'，则函数将在加入噪声之前测定信号强度。 
y = awgn(x,SNR,SIGPOWER,STATE) 重置RANDN的状态。 
y = awgn(…,POWERTYPE) 指定SNR和SIGPOWER的单位。POWERTYPE可以是'dB'或'linear'。如果POWERTYPE是'dB'，那么SNR以dB为单位，而SIGPOWER以dBW为单位。如果POWERTYPE是'linear'，那么SNR作为比值来度量，而SIGPOWER以瓦特为单位
clear,clc;
 N=0:1000;
 fs=1024;
 t=N./fs; 
 y=3*sin(2*pi*t);
 x=wgn(1,1001,2);
 i=y+x;
 % i=awgn(y,2);
 subplot(3,1,1),plot(x);
 subplot(3,1,2),plot(y);
 subplot(3,1,3),plot(i);
![](https://img-blog.csdn.net/20150311110420511?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3psY3cx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
补充：
一阶矩就是随机变量的期望，二阶矩就是随机变量平方的期望，以此可以类推高阶的矩。
﻿﻿
