# 通信算法之十二：Turbo编码与译码仿真链路 - leegang12的专栏 - CSDN博客
2014年11月05日 21:22:05[leegang12](https://me.csdn.net/leegang12)阅读数：2453
一. Turbo编码与译码原理
       Turbo编码器是：并行级联卷积编码(PCCC，Parallel Concatenated ConvolutionalCode),它使用了两个8状态子编码器和一个Turbo码内交织器。Turbo编码器的码率为1/3。
二. 仿真链路：
信道环境：高斯白噪声
仿真参数：R =1/3 ,  约束长度=9，MAP交织器为：LTE交织器
仿真结果：LTE标准Turbo编码译码，仿真增益为：13dB。（12.6 - （-0.9 ）= 13.5dB ）
仿真平台：matlab
信道环境：高斯白噪声
信噪比SNR : -1.4:0.1:-0.6
仿真链路：源比特+Turbo编码+QPSK调制+AWGN+QPSK解调+Turbo译码+BER
仿真验证架构：
![](https://img-blog.csdn.net/20141105211139250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVlZ2FuZzEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
误码率曲线：
![](https://img-blog.csdn.net/20141105211258384?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVlZ2FuZzEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
三.  MATLAB仿真代码
%****************************************************************
% 内容概述：Turbo编码与Turbo译码链路，符合LTE标准,仿真增益：13dB,R=1/3
% 仿真链路：源比特-->Turbo编码-->QPSK调制-->高斯白噪声信道-->
%           QPSK解调-->Turbo译码-->BER
% 创 建 人：
% 创建时间：2013年11月21日
% 参考文献：网络资料
% 版权声明：不可随意复制、不可随意传播。
%****************************************************************
clc;
clear all;
close all;
%系统参数设计------------------------------
        %Turbo编码-
        Turbo_out = TurboEncode(SourceBit,SourceBitlen,RefLen,F1,F2);
        %QPSK调制---------------------------------
        Turbo_out2 = QpskMod(Turbo_out);
        %高斯白噪声信道----------------------
        soft_in = Channel(Turbo_out2,SNR(nEN));
        %QPSK解调---------------------------
        soft_in1 = QpskDemod(soft_in);    
        %Turbo译码--LTE----------------
        [hard_out,soft_out] = TurboDecode(soft_in1,TurboIter,RefLen,F1,F2);
        %误码率-----------------------------
        errs = length(find(hard_out(1:SourceBitlen) ~= SourceBit));
【详细资料，及相应MATLAB代码，C语言代码。咨询qq：1279682290】
