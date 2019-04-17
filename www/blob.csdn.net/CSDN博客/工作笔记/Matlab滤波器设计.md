# Matlab滤波器设计 - 工作笔记 - CSDN博客





2012年07月26日 15:53:36[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：13491








                
滤波器设计是一个创建满足指定滤波要求的滤波器参数的过程。滤波器的实现包括滤波器结构的选择和滤波器参数的计算。只有完成了滤波器的设计和实现，才能最终完成数据的滤波。

    滤波器设计的目标是实现数据序列的频率成分变更。严格的设计规格需要指定通带波纹数、阻带衰减、过渡带宽度等。更准确的指定可能需要实现最小阶数的滤波器、需要实现任意形状的滤波器形状或者需要用fir滤波器实现。指定的要求不同，滤波器的设计也不同。

    Matlab的信号处理工具箱软件提供了两种方式设计滤波器：面向对象的和非面向对象的。面向对象的方法首先创建一个滤波器对象fdesign，然后调用合适的design参数设计。如实现一个5阶的低通巴特沃斯滤波器，3dB截止频率为200Hz，采样频率1000Hz，代码如下

Fs=1000; %Sampling Frequency

time = 0:(1/Fs):1; %time vector

% Data vector

x = cos(2*pi*60*time)+sin(2*pi*120*time)+randn(size(time));

d=fdesign.lowpass('N,F3dB',5,200,Fs); %lowpass filter specification object

% Invoke Butterworth design method

Hd=design(d,'butter');

y=filter(Hd,x);

    非面向对象的方法则适用函数实现滤波器设计，如butter、firpm。所有非面向对象的滤波器设计函数使用的是归一化频率，归一化频率[0, 1]之间，1表示πrad。将Hz频率转化为归一化频率的方法为乘以2除以采样频率。设计上面同样的滤波器，使用非面向对象的方法如下

Wn = (2*200)/1000; %Convert 3-dB frequency

% to normalized frequency: 0.4*pi rad/sample

[B,A] = butter(5,Wn,'low');

y = filter(B,A,x);



#### 滤波函数

* filter：利用递归滤波器(IIR)或非递归滤波器(FIR)对数据进行数字滤波；

* fftfilt：利用基于FFT的重叠相加法对数据进行滤波，只适用于非递归滤波器(FIR)；

* filter2：二维FIR数字滤波；

* filtfilt：零相位滤波（IIR与FIR均可）。



#### 滤波器特性分析

* 脉冲响应Impz

    等价于使用函数filter输入一个脉冲信号x=[1;zero(N-1,1)]。

* 频率响应freqz与freqs

    Freqz：求解数字滤波器的频率响应

    Freqs：求解模拟滤波器的频率响应

* 幅频和相频abs与angle、unwrap

    Unwrap：解卷绕

* 群延迟grpdelay

    群延迟即为滤波器相位响应的负一阶导数，是滤波器平均延迟的度量。

* 零极点分析zplane



#### IIR数字滤波器设计

##### 模拟低通滤波器设计

* 巴特沃斯低通滤波器设计buttap

* 切比雪夫低通滤波器设计cheb1ap与cheb2ap

* 椭圆低通滤波器设计ellipap

##### 模拟滤波器最小阶数的选择

* 巴特沃斯滤波器Buttord

* 切比雪夫1型滤波器Cheb1ord

* 切比雪夫2型滤波器Cheb2ord

* 椭圆滤波器Ellipord

##### 模拟高通、带通、带阻滤波器设计

* 模拟低通到模拟低通lp2lp

* 模拟低通到模拟高通lp2hp

* 模拟低通到模拟带通lp2bp

* 模拟低通到模拟带阻lp2bs

##### IIR实频变换

* IIR实频率移位变换iirshift

* 实低通到实低通的频率移位变换iirlp2lp

* 实低通到实高通的频率移位变换iirlp2hp

* 实低通到实带通的频率移位变换iirlp2bp

* 实低通到实带阻的频率移位变换iirlp2bs

* 实低通到实多带的频率移位变换iirlp2mb

* 实低通到实多点的频率移位变换iirlp2xn

##### IIR复频变换

* IIR复频率移位变换iirshiftc

* 实低通到复带通的频率移位变换iirlp2bpc

* 实低通到复带阻的频率移位变换iirlp2bsc

* 实低通到复多带的频率移位变换iirlp2mbc

* 实低通到复多点的频率移位变换iirlp2xnc

* 复带通到复带通的频率移位变换iirbpc2bpc

##### 模拟滤波器的离散化

* 脉冲响应不变法impinvar

* 双线性变换法bilinear

##### IIR滤波器的直接设计

* 贝塞尔模拟滤波器Besself

* 巴特沃斯滤波器Butter

* 切比雪夫1型滤波器Cheby1

* 切比雪夫2型滤波器Cheby2

* 椭圆型滤波器Ellip

* 递归数字滤波器Yulewalk

    使用最小二乘法拟合频率响应函数。

* 一般数字滤波器Maxplat

##### 小结

    相对于FIR滤波器，IIR滤波器的主要优点在于它以比FIR更小的阶数满足指定的滤波要求。虽然IIR滤波器有非线性的相位，但matlab软件中的数据处理方式是离散的，即全部数据序列被用于滤波。这允许了零相位滤波方法存在，可以使用函数filtfilt函数实现，它可以消除IIR滤波器的非线性相位偏移。

IIR滤波方法小结
|滤波方法|描述|滤波器函数|
|----|----|----|
|模拟原型|使用连续域的经典低通原型滤波器模型零极点，再通过频率变换和滤波器离散化得到数字滤波器|直接滤波器设计函数：besself,butter,cheby1, cheby2, ellip阶数预测函数：buttord, cheb1ord, cheb2ord, ellipord低通模拟原型函数：besselap, buttap, cheb1ap, cheb2ap, ellipap频率变换函数：lp2bp, lp2bs, lp2hp, lp2lp滤波器离散函数：bilinear, impinvar|
|直接设计|通过逼近线性幅值响应直接在离散域设计数字滤波器|yulewalk|
|广义巴特沃斯滤波器设计|设计零点多于极点的低通巴特沃斯滤波器|maxflat|
|参数建模|通过逼近给定的时域或频域响应得到数字滤波器|时域建模函数：lpc, prony, stmcb频域建模函数：invfreqs, invfreqz|

#### FIR数字滤波器设计

##### 窗函数法

* 设计具有标准频率响应的FIR滤波器Fir1

* 设计具有任意频率响应的FIR滤波器Fir2（如多带通滤波器）

     使用凯塞窗时可以使用kaiserord函数设计FIR参数

##### 切比雪夫逼近法

* 最佳一致逼近法设计firpm

     估计最佳一致逼近法滤波器的阶次firpmord

* 任意响应法cfirpm

##### 约束最小二乘法

* 设计线性相位滤波器firls

* 设计多带线性相位滤波器fircls

* 设计低通或高通线性相位滤波器fircls1

##### 设计Savitzky-Golay平滑滤波器sgolay

##### 小结

    FIR滤波器有如下主要优点：

* 准确线性相位

* 总是稳定的

* 设计方法是线性的

* 可以有效的在硬件上实现

* 滤波初始过渡阶段持续时间有限

    它的主要缺点是它需要比IIR更高的阶数来实现给定的响应，相应的，它的滤波器延迟也比IIR的大。

    FIR滤波方法小结：
|滤波器设计方法|描述|滤波器函数|
|----|----|----|
|窗函数设计法|对傅里叶逆变换加窗|fir1, fir2, kaiserord|
|多带逼近|最小二乘法一致逼近多带频率响应|firls, firpm, firpmord|
|约束最小二乘法|在最大误差的约束下最小化误差平方和|fircls, fircls1|
|任意响应|任意响应设计，包括非线性相位、复频域滤波器|cfirpm|
|Raised Cosine|平滑正弦过渡的低通响应|firrcos|

#### 工具箱GUI

Sptool信号分析工具箱GUI

Wintool窗函数查看工具箱GUI



