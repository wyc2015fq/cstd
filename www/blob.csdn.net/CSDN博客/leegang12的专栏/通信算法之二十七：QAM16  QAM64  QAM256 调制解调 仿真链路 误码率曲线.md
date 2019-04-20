# 通信算法之二十七：QAM16 / QAM64 / QAM256 调制解调 仿真链路 误码率曲线 - leegang12的专栏 - CSDN博客
2019年03月09日 16:15:22[leegang12](https://me.csdn.net/leegang12)阅读数：130

一.  QAM16 / QAM64 / QAM256调制与解调 原理：
     原理内容见网络资源及相关课本。
二. 仿真链路：
信道环境：高斯白噪声信道
仿真参数：略
仿真平台：matlab
仿真链路：源比特+QAM16 / QAM64 / QAM256调制+AWGN+QAM16 / QAM64 / QAM256解调+BER
%****************************************************************
% 内容概述：QAM(N)调制与QAM(N)解调链路
% 仿真链路：源比特-->QAM调制-->高斯白噪声信道-->
%           QAM解调-->BER
% 创 建 人：LEE GANG
% 创建时间：2019年 3月9日
% 参考文献：网络资料
% 版权声明：不可随意复制、不可随意传播。
%****************************************************************
clc;
clear all;
close all;
%系统参数设计------------------------------
SourceBitlen = 16*6*1000;%输入源比特长度
SNR = 6:2:18;%%信噪比序列
BerEbNo = zeros(1,length(SNR));
for nEN=1:length(SNR)   
    Err_counter = 0;
    for nframe=1:20        
        %输入源比特序列基带信号-------------------------------     
        SourceBit = gensrcbit(SourceBitlen);  
        % QAM 8/16/64/256调制---------------------------------
        MM = 16;
        x = qammodLJ(SourceBit,MM);
        %scatterplot(x)
        %高斯白噪声信道----------------------
        soft_in = awgnChannel(x,SNR(nEN));
%     scatterplot(soft_in)
        % QAM解调---------------------------
       [hard_bit soft_bit] = qamdemodLJ(soft_in,MM);
        %误码率-----------------------------
        errs = symerr(hard_bit,SourceBit);
        Err_counter = Err_counter + errs;
        Ber = Err_counter/nframe/SourceBitlen;
        %disp(['-----SNR=',num2str(SNR(nEN)),',Frame=',num2str(nframe),',Errorbit=',num2str(errs)']);
    end
    BerEbNo(nEN) = Ber;
    fprintf('EcNo:%1.2fdB，误码率：%8.4e,\n',SNR(nEN),Ber);
end
semilogy(SNR,BerEbNo,'b-v');
grid on;
xlabel('Ec/No[dB]');
ylabel('Bit Error Rate');
title('QAM(16/8/64/256)性能仿真误码率曲线');
[ 相应MATLAB仿真代码，C语言代码。咨询qq：1279682290 ]
