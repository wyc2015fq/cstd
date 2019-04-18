# du_Welch_cov_burg 功率谱估计 matlab - fighting！！！ - CSDN博客
2018年05月08日 06:32:34[dujiahei](https://me.csdn.net/dujiahei)阅读数：212
```
% clc,
% clear all;
fs=1000;  %采样率
df=0.1;    %频率分辨率
N=floor(fs/df)+1;%计算的序列点数
nfft=2*N; %FFT变换为复序列
t=0:1/fs:(N-1)/fs;%截取信号的时间段
f=0:df:fs;%功率谱估计的频率分辨率和范围
f=roundn(f,-2);%%find 函数依靠精度识别
% xn=2*cos(2*pi*40*t)+3*cos(2*pi*100*t)+randn(size(t));
xt_0=Feat_test{1,1}(:,1);
%                 xt_0=feat_test(:,Ch_i);
                xt_1=repmat(xt_0,1,11);
                xt_1=reshape(xt_1,1,11000);
                xn=xt_1(1,1:size(f,2));
%%
% %真实功率谱syms ff;
% % a=[zeros(1,N),1,0.98,1.92,0.94,0.92]; %模型系数
% a=xn;
% b=xcorr(a,length(a)-1);  %自相关
% c=(abs(fft(b))).^2;
% Px=c';
% figure;
% plot(f(1:300),Px(1:300));
% title('真实功率谱');xlabel('f(未归一化)');ylabel('P(dB)')
%% FFT
a=xn;
Px=abs(fft(a)).^2/(N^2);%功率谱估计
plot(f(1:300),Px(1:300));
 title('FFT功率谱');xlabel('f');ylabel('P')
%%
%周期图法功率谱估计
figure;
sum_Pxx=0;
window=hamming(N);%汉明窗
[Pxx,f]=periodogram(xn,window,2*N,fs); 
sum_Pxx=sum_Pxx+abs(Pxx);
plot(f(1:300),sum_Pxx(1:300));title('周期图法');xlabel('f(Hz)');ylabel('P');
%%
%WELCH法功率谱估计
figure;
L=length(xn)*0.5;   %数据段长度取值
nfft=2*N; %FFT变换为复序列
sum_Pxx=0;
for j=1:1:N/L
    window=hamming(L);%汉明窗
    noverlap=length(window)*0.5;%重叠数
    [Pxx,f]=pwelch(xn(1,:),window,noverlap,nfft,fs);
    sum_Pxx=sum_Pxx+abs(Pxx);
end    
plot(f(1:2000),sum_Pxx(1:2000));title('WELCH法');xlabel('f(Hz)');ylabel('P');
% %%
% %自相关法功率谱估计
% figure;
% % p=[2 4 8 16 32];%阶数的取值
% p=30;%阶数的取值
% for i=1:1:length(p)
% sum_Pxx=0;
% 
%     [Pxx,f]=pcov(xn,p(i),nfft,fs);
%     sum_Pxx=sum_Pxx+abs(Pxx);
% 
% subplot(length(p),1,i); plot(f,(sum_Pxx));
% s=['协方差法(p=',int2str(p(i)),')'];
% title(s);xlabel('f(Hz)');ylabel('P');
% end
%%
%Burg法功率谱估计
figure;
% p=[300 350 400 450 500 ];%阶数的取值
% p=[440 450 470];%阶数的取值
p=450;%阶数的取值
% p=32;%阶数的取值
for i=1:1:length(p)
sum_Pxx=0;
    [Pxx,f]=pburg(xn,p(i),nfft,fs);
    sum_Pxx=sum_Pxx+abs(Pxx);
subplot(length(p),1,i); plot(f,(sum_Pxx));
s=['Burg法(p=',int2str(p(i)),')'];
title(s);xlabel('f(Hz)');ylabel('P');
end
```
