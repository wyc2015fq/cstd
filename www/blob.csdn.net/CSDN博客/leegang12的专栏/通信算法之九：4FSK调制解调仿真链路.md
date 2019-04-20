# 通信算法之九：4FSK调制解调仿真链路 - leegang12的专栏 - CSDN博客
2014年11月04日 23:25:56[leegang12](https://me.csdn.net/leegang12)阅读数：6584
一.  4FSK原理
         DMR系统，数字集群通信系统。DMR协议采用的调制方式4FSK，是一种恒包络调制，调制时每秒发送2400个符号，每个符号携带两比特的信息。最大频偏D定义如下：D = 3h/2T，h代表每个调制的频偏系数，0.6。T标示符号周期，为1/2400。D=2160。
       4FSK调制器由一个平方根升余弦滤波器级联一个频率调制器组成。第一部分是成型滤波器模块，产生四电平的基带带限信号作为调制信号。第二部分是跳频部分FM。
      调频是角度调制的一种，角度调制一般表示为：Sm(t) = Acos(Wc(t)+ )。频率调制是利用基带信号载波频率得到的，即瞬时频偏随调制信号m(t)线性变化。满足： =Kf.m(t) 。其中，Kf为频偏常数。
二. 4FSK调制信号产生
       产生调制信号主要有两种，直接法和间接法。间接法实现成本和复杂度高。直接法调频是调制信号m(t)直接控制振荡器的频率使其按照调制信号的规律线性变化。调频信号的正交调制是间接法调频的一种实现方式.
三。4FSK调制解调算法设计流程
发射机设计：
![](https://img-blog.csdn.net/20141104230430465?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVlZ2FuZzEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20141104230538703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVlZ2FuZzEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
接收机设计：
![](https://img-blog.csdn.net/20141104231112010?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVlZ2FuZzEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20141104231127438?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVlZ2FuZzEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
四。MATLAB仿真链路
%****************************************************************
% 内容概述：4FSK调制与4FSK解调 仿真链路
% 仿真链路：
% 创 建 人：
% 创建时间：2014年10月23日
% 修改时间：2014年10月28日 理想环境下调试通过
% 修改时间：2014年10月31日 高斯白噪声信道下调试通过
% 修改时间：2014年11月2日  高斯白噪声信道、有相差、有频偏直流分量环境下调试通过
% 参考文献：DMR通信系统各类工程实现文献
% 版权声明：不可随意复制、不可随意传播。
%****************************************************************
clc;
clear all;
close all;
        %----------------发送端物理层基带算法---------------------------------------
        %-1.输入基带信号-源比特序列--串并转换-映射-----------------------------------
        %-2.成型滤波器-升余弦滤波器-------------------------------------------------
        %-包含：八倍内插和平方根升余弦成型滤波器，减小基带信号带宽占用
        Fsksignal = Squrootcosfilter(SourceData,Fd,Fs,Type,Rolloff,Delay);
        %-----------------------以上是发送端物理层基带算法--------------------------
        %--------------------发送端物理层中频算法数字上变频-------------------------
        %-3.相位累加求和-----------------------------------------------------------
        phase = Phasesum(Fsksignal);
        %-符号速率2.4kbps，符号周期T ，调制系数H ，最大频偏D：D = 3h/2T
        [II QQ] = phaseSinCos(phase,factor);
        %-5.数字中频-数字上变频-内插滤波-CIC-HB-FIR---------------------------------
        % 根据奈奎斯特抽样定理，采样频率FS应该大于2*FC+Bm.在实际应用中，一般要
        % 取采样频率FS大于或等于四倍的模拟信号频率。FS >= 4*FC
        % 【待开发数字中频算法：CIC-HB-FIR，符号速率：4*FC】
        %-6.数字中频-数字上变频-载波体制-sin(Wc*t)--cos(Wc*t)------------------------
        % 【待开发数字中频算法：正交，载波调制】
        % s(t) = A*cos(2*pi*FC*t)*I(t) - A*sin(2*pi*FC*t)*Q(t)
        %--------------------以上发送端物理层中频算法数字上变频----------------------
        %-7.空间信道传输-高斯白噪声信道---------------------------------------------
        title('过高斯白噪声信道之后的信号');
        %-8.数字中频-数字下变频-正交解调算法----------------------------------------
        %-9.数字中频-数字下变频-抽取滤波-------------------------------------------
        % 【待开发，数字下变频算法抽取滤波】
        %-10.空间信道传输-加相位差值--加频偏值----------------------------------------
        PPHH = (........................);
        NewSig = SigComplex.*PPHH;
        II = real(NewSig);
        QQ = imag(NewSig);
        figure()
        plot(II(500:800),'m-');
        grid on; 
        title('过高斯白噪声信道/相位差/频率差之后的信号');
        %-11.相位估计、相差，差分算法--消除相干解调中出现的本地载波与信号相差的问题
        %-13.信号标准门限-动态范围确定------------------------
        %-确定动态范围---计算标准门限---------------------------
        [up down mid mean]=SignalDynamicRange(mx);
        [up_level_std mid_level_std down_level_std]=SignalBiaozhunRange(up,down,mean);
        %-16.门限判决，逆映射--------------------------------------
title('--4FSK调制解调仿真误码率曲线--');
disp('--4FSK调制，4FSK解调仿真结束--');
%---------------------------------------------
五。仿真图形
![](https://img-blog.csdn.net/20141104232251234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVlZ2FuZzEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20141104232315992?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVlZ2FuZzEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20141104232327473?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVlZ2FuZzEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20141104232337270?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVlZ2FuZzEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20141104232432656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVlZ2FuZzEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
【详细资料，及相应MATLAB代码，C语言代码。咨询qq：1279682290】
