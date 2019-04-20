# 通信算法之十八：BCH码+4FSK仿真链路 - leegang12的专栏 - CSDN博客
2018年04月25日 22:17:08[leegang12](https://me.csdn.net/leegang12)阅读数：155

一. BCH码编码与译码、4FSK调制与解调 原理：
     原理内容见网络资源及相关课本。
二. 仿真链路：
信道环境：高斯白噪声信道
仿真参数：略
仿真平台：matlab
仿真链路：源比特+BCH编码+4FSK调制+AWGN+4FSK解调+BCH软译码+BER
%****************************************************************
% 内容概述：BCH编码与BCH译码，4FSK调制与4FSK解调链路
% 仿真链路：源比特-->BCH编码-->4FSK调制-->高斯白噪声信道-->
%           
4FSK解调-->BCH译码-->BER
% 创 建 人：
% 创建时间：2018年2月21日
% 参考文献：网络资料
% 版权声明：不可随意复制、不可随意传播。
%****************************************************************
clc;
clear all;
close all;
%系统参数设计------------------------------
SourceBitlen = 1000;%输入源比特长度
SNR = ;%信噪比序列
BerEbNo = zeros(1,length(SNR));
for nEN=1:length(SNR)
    Err_counter = 0;
    for nframe=1:1000%5000     
        %输入源比特序列基带信号--------------------
        SourceBit = round(rand(1,SourceBitlen));
        %BCH编码--------------------------------
        Conv_out = 
BCHmod(SourceBit,tblen);      
        %4FSK调制---------------------------------
        Conv_out1 = reshape(Conv_out,1,[]);
        Conv_out2 = 4FSKModulation(Conv_out1);       
        %高斯白噪声信道----------------------
        soft_in = awgn(Conv_out2,SNR(nEN));
        %4FSK解调---------------------------
        soft_in1 = 4FSKDemodulation(-soft_in);         
        %BCH译码--------------------------
        Dec_out = BCHDecodeLjt(soft_in1,tblen);     
        %误码率-----------------------------
        。。。。。。。
。。。。。。。
。。。。。。。
【详细资料及MATLAB代码，[C语言](http://lib.csdn.net/base/c)代码。索要请咨询qq：1279682290】
