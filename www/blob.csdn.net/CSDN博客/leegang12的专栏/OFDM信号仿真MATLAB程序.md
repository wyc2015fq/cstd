# OFDM信号仿真MATLAB程序 - leegang12的专栏 - CSDN博客
2013年03月04日 00:23:09[leegang12](https://me.csdn.net/leegang12)阅读数：5647
个人分类：[MATLAB仿真](https://blog.csdn.net/leegang12/article/category/816263)

%-- OFDM信号产生仿真程序 --
clear all;clc;close all;
Fv=32; %原始输入数据流速率（Mb/s）
M=2;   %子载波调制进制=2M，QPSK时M=2，,16QAM,M=4
N=8;   %子信道数
I=50;  %50倍内插
Fs=Fv/N/M; %调制符号速率（2Mb/s）
fs=I*Fs;   %输出采样率（100MHz）
f1=25;     %数字上变频中心频率（MHz）
d(1)=1+j;  % 数据符号
d(2)=-1+j;
d(3)=-1-j;
d(4)=1-j;
d(5)=1+j;
d(6)=-1+j;
d(7)=-1-j;
d(8)=1-j;
s=ifft(d); %离散傅里叶反变换
for k=1:N
    s(k)=s(k)*complex(cos(pi/N*(k-1)),sin(pi/N*(k-1)));%子载波搬移半个信道
end
[n0,f,m,w]=firpmord([0.9 1.1],[1 0],[0.001 0.001],fs);
b=firpm(n0,f,m,w);%内插滤波器
for n=1:N*I
    if mod((n-1),I)==0
        dd(n)=s((n-1)/I+1);
    else
        dd(n)=0.0; %内插（I-1）个0点。
    end
end
y=conv(dd,b);%内插滤波
m=(N*I+n0);
for k=1:m
    y1(k)=real(y(k))*cos(2*pi*f1*(k-1)/fs)+imag(y(k))*sin(2*pi*f1*(k-1)/fs);%数字上变频
    l(k)=fs/m*(k-1);
end
yy=20*log10(abs(fft(y1)));
figure
plot(l(1:m/2),yy(1:m/2));
grid on;
