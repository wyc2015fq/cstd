# FDAT设计滤波器方法 - 工作笔记 - CSDN博客





2012年07月25日 13:06:11[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：7110








 IIR滤波器的MATLAB设计

1  FDATool界面设计

1.1 FDATool的介绍

    FDATool(Fil[te](http://www.dzsc.com/product/searchfile/10366.html)r Design&Analysis Tool)是MATLAB信号处理[工具箱](http://www.dzsc.com/product/searchfile/4611.html)里专用的滤波器设计分析工具，MATLAB 6.O以上的版本还专门增加了滤波器设计工具箱(Filter
 Design Toolbox)。FDATool可以设计几乎所有的常规滤波器，包括FIR和IIR的各种设计方法。它操作简单，方便灵活。

    FDATool。界面总共分两大部分，一部分是Design Filter。在界面的下半部，用来设置滤波器的设计参数；另一部分则是特性区，在界面的上半部分，用来显示滤波器的各种特性。Design Filter部分主要分为：Filter Type(滤波器类型)选项，包括Lowpass(低通)、Highpass([高通](http://www.dzsc.com/data/iccompany/detail287.html))、Bandpass(带通)、Bandstop(带阻)和特殊的[FIR滤波器](http://wiki.dzsc.com/info/2528.html)。

   Design Method(设计方法)选项，包括IIR滤波器的Butterwotth(巴特沃思)法、Chebyshev  Type I(切比雪夫I型)法、Chebyshev Type II(切比雪夫II型)法、Elliptic(椭圆滤波器)法和FIR滤波器的Equiripple法、Least-Squares(最小乘方)法、Window(窗函数)法。

  Filter Order(滤波器阶数)选项，定义滤波器的阶数，包括Specify Order(指定阶数)和Minimum Order(最小阶数)。在Specify Order中填入所要设计的滤波器的阶数(N阶滤波器，Specify Order=N-1)。如果选择Minimum Order，则MATLAB根据所选择的滤波器类型自动使用最小阶数。

  Frequency Specificati[ON](http://www.dzsc.com/data/iccompany/detail12.html)s选项，可以详细定义频带的各参数，包括采样频率和频带的截止频率。它的具体选项由Filter Type选项和Design Method选项决定。例如Bandpass(带通)滤波器需要定义Fstop1(下阻带截止频率)、Fpass1(通带下限截止频率)、Fpass2(通带上限截止频率)、Fstop2(上阻带截止频率)，而Lowpass(低通)滤波器只需要定义Fstop1、Fpass1。采用窗函数设计滤波器时，由于过渡带是由窗函数的类型和阶数所决定，所以只需定义通带截止频率，而不必定义阻带参数。

    Magnitude Specifications选项，可以定义幅值衰减的情况。例如设计[带通滤波器](http://www.dzsc.com/product/searchfile/7331.html)时，可以定义Wstop1(频率Fstop1处的幅值衰减)、Wpass(通带范围内的幅值衰减)、Wstop2(频率Fstop2处的幅值衰减)。当采用窗函数设计时，通带截止频率处的幅值衰减固定为6db，所以不必定义。

  Window Specifications选项，当选取采用窗函数设计，该选项可定义，它包含了各种窗函数。


3.1.2  IIR滤波器设计实例

    本文以一个IIR滤波器的设计为例说明如何使用MATLAB设计数字滤波器。要求设计一个10阶的带通Chebyshev I滤波器，它的通带范围是100到200Hz，采样频率为1000Hz，Rp=0.5。

    本例中，首先在Filter Type中选择Bandpass([带通滤波器](http://wiki.dzsc.com/info/1054.html))；在Design Method选项中选择IIR，接着在相邻的右则选项中选择Chebyshev I(切比雪夫I型)；指定Filter Order项中的Specify Order=10；由于采用的是切比雪夫设计，不必在Options中选择；然后在Frequency Specifications中选择Unit为Hz，给出采样频率Fs=1000，通带Fpass1=100和Fpass2=200；最后在Magnitude
 Specifications中选择Unit为db，Apass=0.5。设置完成后

点击Design Filter即可得到所设计的IIR滤波器。通过菜单选项Analysis可以在特性区看到所设计的幅频响应、相频响应、冲击响应和零极点配置等特性，如图1所示。设计完成后将结果保存为filterl.fda文件。





![](http://www.dzsc.com/data/uploadfile/2008818152347849.gif)

![](http://www.dzsc.com/data/uploadfile/2008818152347662.gif)

![](http://www.dzsc.com/data/uploadfile/2008818152347520.gif)



图1滤波器的幅频、相频和冲激响应(特性区)



3.2  程序设计法

  在MATLAB中，对各种滤波器的设计都有相应的计算振幅、相位和冲激响应的函数，可以用来做滤波器程序设计。

  上例的IIR滤波器的冲激响应可用程序设计如下：

%ehebyshevl bpf

n=10;    %阶数为10

Rp=0.5;  %幅值衰减为0.5

Wn[100 200]/500；

[b,a]=chebyl(n,Rp,wn);

[y,t]impz(b,a,101);

stem(t,y,'.');

    在MATLAB环境下运行该程序即可得到滤波器的时域冲激响应。由于篇幅所限，这里不再详述源程序。


3.3设计实例分析

   由图1(a)可知，这种滤波器在100-200Hz的通带范围内是等波纹的，而在阻带中是单调的，这是Chebyshev I滤波器的幅频特性。由图1(b)可知，在100-200Hz的范围内相移较小，其曲线近似一条直线，失真较小；当频率超过这一范围时，相移较大，而且其曲线是非直线的，所以失真也较大。图1(c)得到了Chebyshev  I滤波器的时域冲激响应，在5ms-100ms有冲激响应，超过这一范围的冲激响应近似为零，进而实现了Chebyshev I带通

滤波器的设计。 

4 Simulink仿真

4.1 Simulink仿真实例

    通过调用Simulink中的功能模块，可以构成数字滤波器的仿真框图。在仿真过程中，双击各功能模块，随时改变参数，获得不同状态下的仿真结果。例如原始信号x(t)=2sin(0.05wt)+w(t)，w(t)为随机信号，幅值为0.2，通过一[传递函数](http://wiki.dzsc.com/info/5346.html)为H(z)=0.15/(1-0.8z-1)的滤波器可得到如图2的仿真结果。其中仿真过程中可导入FDATool所设计的滤波器文件。





![](http://www.dzsc.com/data/uploadfile/2008818152347763.gif)

![](http://www.dzsc.com/data/uploadfile/2008818152347484.gif)

![](http://www.dzsc.com/data/uploadfile/2008818152347153.gif)




4.2  仿真分析

    比较图2中(b)和(c)的波形可知，输入的原始信号经过滤波器滤波后，(b)中波形的毛刺部分(即干扰噪声)被滤除，输出的信号更接近正弦波，如(c)中所示波形。由此说明，传递函数为H(z)=0.15/(1-0.8z-1)的滤波器的设计是恰当的。


5 结论

    利用MATLAB的强大运算功能，基于MATLAB的信号处理工具箱(Signal Processing Toolbox)的数字滤波器设计法可以快速有效地设计由软件组成的常规数字滤波器，设计方便、快捷，大大减轻了工作量。在设计过程中可以对比滤波器特性。随时更改参数，以达到滤波器设计的最优化。利用MATLAB设计数字滤波器在数字[通信系统](http://wiki.dzsc.com/info/5062.html)和计算机领域信号处理中，有着广泛的应用前景,可关注。



