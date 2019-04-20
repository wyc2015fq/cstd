# 通信算法之十六：卷积编码与Viterbi软译码仿真链路 - leegang12的专栏 - CSDN博客
2017年02月25日 10:32:45[leegang12](https://me.csdn.net/leegang12)阅读数：832
一. 卷积编码与Viterbi软译码原理
       内容见网络资源。
二. 仿真链路：
信道环境：高斯白噪声，瑞丽信道
仿真参数：R =1/2 ,  约束长度=7。
仿真结果：仿真增益为：8dB左右。（12.6 - （4.1 ）= 8.5dB ）
仿真平台：matlab
信道环境：高斯白噪声
仿真链路：源比特+卷积编码+QPSK调制+AWGN+QPSK解调+Viterbi软译码+BER
%****************************************************************
% 内容概述：卷积编码与Viterbi软译码，QPSK调制与QPSK解调链路
% 仿真链路：源比特-->卷积编码-->QPSK调制-->高斯白噪声信道-->
%           QPSK解调-->卷积译码-->BER
% 创 建 人：Lee Gang
% 创建时间：2017年2月21日
% 参考文献：网络资料
% 版权声明：不可随意复制、不可随意传播。
%****************************************************************
clc;
clear all;
close all;
%系统参数设计------------------------------
SourceBitlen = 100;%输入源比特长度
SNR = 2:1:6;%信噪比序列
BerEbNo = zeros(1,length(SNR));
for nEN=1:length(SNR)
    Err_counter = 0;
    for nframe=1:1000%5000     
        %输入源比特序列基带信号--------------------
        SourceBit = round(rand(1,SourceBitlen));
        %卷积编码--------------------------------
        Conv_out = convEncodeLjt(SourceBit,tblen);      
        %QPSK调制---------------------------------
        Conv_out1 = reshape(Conv_out,1,[]);
        Conv_out2 = QpskModulation(Conv_out1);       
        %高斯白噪声信道----------------------
        soft_in = awgn(Conv_out2,SNR(nEN));
        %QPSK解调---------------------------
        soft_in1 = QpskDemodulation(-soft_in);         
        %Viterbi译码--------------------------
        Dec_out = convDecodeLjt(soft_in1,tblen);     
        %误码率-----------------------------
        。。。。。。。
。。。。。。。
。。。。。。。
【详细资料，及相应MATLAB代码，[C语言](http://lib.csdn.net/base/c)代码。索要请咨询qq：1279682290】
