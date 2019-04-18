# 模拟和数字低通滤波器的MATLAB实现 - fighting！！！ - CSDN博客
2019年01月08日 11:47:17[dujiahei](https://me.csdn.net/dujiahei)阅读数：109
个人分类：[matlab](https://blog.csdn.net/dujiahei/article/category/7619771)
转自：[http://blog.sina.com.cn/s/blog_79ecf6980100vcrf.html](http://blog.sina.com.cn/s/blog_79ecf6980100vcrf.html)
低通滤波器参数：Fs=8000，fp=2500，fs=3500，Rp=1dB，As=30dB，其他滤波器可以通过与低通之间的映射关系实现。
%%模拟滤波器
%巴特沃斯——滤波器设计
wp=2*pi*2500;ws=2*pi*3500;Rp=1;As=30;
[N,wc]=buttord(wp,ws,Rp,As,'s')%计算率波器的阶数和3dB截止频率
[B,A]=butter(N,wc,'s');%计算滤波器系统函数分子分母多项式
fk=0:800/512:8000;wk=2*pi*fk;
Hk=freqs(B,A,wk);
figure
plot(fk/1000,20*log10(abs(Hk)));
grid on,xlabel('频率（kHz）'),ylabel('幅度（dB）')
title('巴特沃斯模拟滤波器')
axis([0,4,-35,5])
![模拟和数字低通滤波器的MATLAB实现](http://s12.sinaimg.cn/middle/79ecf698gb218e93e8bbb&690)
%%
%切比雪夫I——滤波器设计
wp=2*pi*2500;ws=2*pi*3500;Rp=1;As=30;
[N1,wp1]=cheb1ord(wp,ws,Rp,As,'s')%计算切比雪夫滤波器的阶数和通带边界频率
[B1,A1]=cheby1(N1,Rp,wp1,'s');%计算滤波器系统函数分子分母多项式
fk=0:800/512:8000;wk=2*pi*fk;
Hk=freqs(B1,A1,wk);figure,
plot(fk/1000,20*log10(abs(Hk)));
grid on,xlabel('频率（kHz）'),ylabel('幅度（dB）')
title('切比雪夫I模拟滤波器')
axis([0,4,-35,5])
%%
![模拟和数字低通滤波器的MATLAB实现](http://s14.sinaimg.cn/bmiddle/79ecf698gb218ebf02ffd&690)
%切比雪夫II——滤波器设计
wp=2*pi*2500;ws=2*pi*3500;Rp=1;As=30;
[N2,wso]=cheb2ord(wp,ws,Rp,As,'s')%计算切比雪夫滤波器的阶数和通带边界频率
[B2,A2]=cheby2(N1,Rp,wso,'s');%计算滤波器系统函数分子分母多项式
fk=0:800/512:8000;wk=2*pi*fk;
Hk=freqs(B1,A1,wk);figure,
plot(fk/1000,20*log10(abs(Hk)));
grid on,xlabel('频率（kHz）'),ylabel('幅度（dB）')
title('切比雪夫II模拟滤波器')
axis([0,4,-35,5])
![模拟和数字低通滤波器的MATLAB实现](http://s1.sinaimg.cn/bmiddle/79ecf698g7835b143fa30&690)
 %%
%椭圆——滤波器设计
wp=2*pi*2500;ws=2*pi*3500;Rp=1;As=30;
[N,wpo]=ellipord(wp,ws,Rp,As,'s')%计算滤波器的阶数和通带边界频率
[B,A]=ellip(N,Rp,As,wpo,'s');%计算滤波器系统函数分子分母多项式
fk=0:800/512:8000;wk=2*pi*fk;
Hk=freqs(B1,A1,wk);figure,
plot(fk/1000,20*log10(abs(Hk)));
grid on,xlabel('频率（kHz）'),ylabel('幅度（dB）')
axis([0,4,-35,5]),title('椭圆模拟滤波器')
![模拟和数字低通滤波器的MATLAB实现](http://s16.sinaimg.cn/bmiddle/79ecf698gb218ed79b95f&690)
 %%
%数字滤波器
%脉冲响应法滤波器设计
fp=2500;fs=3500;Fs=8000;
wp=2*fp/Fs;ws=2*fs/Fs;%求归一化数字通带截止频率,求归一化数字阻带起始频率 
deltaw=ws-wp;%求过渡带宽
N0=ceil(6.6/deltaw);%求窗口长度
N=N0+mod(N0+1,2); %确保窗口长度 N为奇数 
n=N-1;%求出滤波器的阶数 n 
wn=(ws+wp)/2; %求滤波器的截止频率 
b=fir1(n,wn)%利用 fir1 函数求出滤波器的系数
[Hk,w] = freqz(b,1);                     %  计算频率响应 
mag = abs(Hk);                         %  求幅频特性
db = 20*log10(mag/max(mag));           %  化为分贝值 
dw =pi/512; %关于pi归一化
Rp = -(min(db(1:wp*pi/dw+1)))            %  检验通带波动 
As = -(max(db(ws*pi/dw+1:512)))         %  检验最小阻带衰减 
figure,plot(0:pi/511:pi,db),grid on
axis([0,4.0,-80,5]),title('数字滤波器——脉冲响应法')
%%
![模拟和数字低通滤波器的MATLAB实现](http://s7.sinaimg.cn/bmiddle/79ecf698gb218eecf3176&690)
%fir1窗函数法
fp=2500;fs=3500;Fs=8000;rs=30;
wp=2*fp*pi/Fs;ws=2*fs*pi/Fs;%求归一化数字通带截止频率,求归一化数字阻带起始频率 
Bt=ws-wp;%求过渡带宽
alpha=0.5842*(rs-21)^0.4+0.07886*(rs-21);%计算kaiser窗的控制参数
M=ceil((rs-8)/2.285/Bt);%求出滤波器的阶数
wc=(ws+wp)/2/pi; %求滤波器的截止频率并关于pi归一化 
hk=fir1(M,wc,kaiser(M+1,alpha))%利用 fir1 函数求出滤波器的系数
[Hk,w] = freqz(hk,1);                     %  计算频率响应 
mag = abs(Hk);                         %  求幅频特性
db = 20*log10(mag/max(mag));           %  化为分贝值 
db1=db';
figure,plot(0:pi/511:pi,db1),grid on
axis([0,4.0,-80,5]),title('数字滤波器——fir1窗函数法')
%%
![模拟和数字低通滤波器的MATLAB实现](http://s8.sinaimg.cn/bmiddle/79ecf698gb218ef9c2d87&690)
%频率采样法
fp=2500;fs=3500;Fs=8000;rs=30;
wp=2*fp*pi/Fs;ws=2*fs*pi/Fs;%求归一化数字通带截止频率,求归一化数字阻带起始频率 
Bt=ws-wp;%求过渡带宽
m=1;alpha=0.5842*(rs-21)^0.4+0.07886*(rs-21);%计算kaiser窗的控制参数
N=ceil(m+1)*2*pi/Bt;%求出滤波器的阶数
N=N+mod(N+1,2);
Np=fix(wp/(2*pi/N));
Ns=N-2*Np-1;
Hk=[ones(1,Np+1),zeros(1,Ns),ones(1,Np)];
wc=(ws+wp)/2/pi; %求滤波器的截止频率并关于pi归一化 
hk=fir1(M,wc,kaiser(M+1,alpha))%利用 fir1 函数求出滤波器的系数
[Hk,w] = freqz(hk,1);                     %  计算频率响应 
mag = abs(Hk);                         %  求幅频特性
db = 20*log10(mag/max(mag));           %  化为分贝值 
db1=db';
figure,plot(0:pi/511:pi,db1),grid on
axis([0,4.0,-80,5]),title('数字滤波器——频率采样法')
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
![模拟和数字低通滤波器的MATLAB实现](http://s3.sinaimg.cn/bmiddle/79ecf698gb218f0c13862&690)
%%
%利用等波纹最佳逼近法设计FIR数字滤波器
Fs=8000;f=[2500,3500];m=[1,0];
rp=1;rs=30;
delta1=(10^(rp/20)-1)/(10^(rp/20)+1);delta2=10^(-rs/20);
rip=[delta1,delta2];
[M,fo,mo,w]=remezord(f,m,rip,Fs);%边界频率为模拟频率时必须加入采样频率
M=M+1;%估算的M直达不到要求，家1后满足要求
hn=remez(M,fo,mo,w);
[Hk,w] = freqz(hn,1);                     %  计算频率响应 
mag = abs(Hk);                         %  求幅频特性
db = 20*log10(mag/max(mag));           %  化为分贝值 
db1=db';
figure,plot(0:pi/511:pi,db1),grid on
axis([0,4.0,-80,5]),title('数字滤波器——等波纹最佳逼近法')
![模拟和数字低通滤波器的MATLAB实现](http://s13.sinaimg.cn/bmiddle/79ecf698gb218f1c84adc&690)
