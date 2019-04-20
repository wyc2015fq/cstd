# 通信算法之十一：QPSK/DQPSK/OQPSK/BPSK/DBPSK/16QAM调制解调仿真链路 - leegang12的专栏 - CSDN博客
2014年11月05日 20:56:00[leegang12](https://me.csdn.net/leegang12)阅读数：7971
个人分类：[无线通信基带处理算法](https://blog.csdn.net/leegang12/article/category/896922)
一. QPSK
        正交[相移键控](http://baike.baidu.com/view/525287.htm) (Quadrature
 Phase Shift Keying)简称“QPSK”，是一种数字调制方式。它分为绝对相移和相对相移两种。由于绝对相移方式存在相位模糊问题，所以在实际中主要采用相对移相方式DQPSK。目前已经广泛应用于无线通信中，成为现代通信中一种十分重要的调制解调方式。
二. QPSK: 
        EbNo=9.6dB
        EcNo=12.6dB
![](https://img-blog.csdn.net/20141105204551644?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVlZ2FuZzEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
四. QPSK仿真链路
%****************************************************************
% 内容概述：QPSK调制与QPSK解调 仿真链路
% 仿真链路：
% 创 建 人：Lee gang
% 创建时间：2014年10月20日
% 参考文献：
% 版权声明：不可随意复制、不可随意传播。
%****************************************************************
clc
close all
clear all
%系统参数---------------------------------------
        %QPSK调制---------------------------------------
        out = LGQpskMod(SourceBit);
        %高斯白噪声信道---------------------------------
        outch = ChannelAwgn(out,SNR(nEN));
        %QPSK解调-------------------------------------
        [BerDateOut Soft] = LGQpskDemod(III,QQQ);
五.仿真图形
调制后星座图：
![](https://img-blog.csdn.net/20141105204918109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVlZ2FuZzEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
解调前：
![](https://img-blog.csdn.net/20141105204841451?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVlZ2FuZzEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
六. BPSK
(1)
EbNo = 9.6dB
(2) 仿真链路略
(3) 误码率曲线
![](https://img-blog.csdn.net/20141105205102881?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVlZ2FuZzEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
七. 
DBPSK误码率曲线
![](https://img-blog.csdn.net/20141105205143503?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVlZ2FuZzEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
DQPSK误码率曲线
![](https://img-blog.csdn.net/20141105205310281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVlZ2FuZzEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
OQPSK误码率曲线
![](https://img-blog.csdn.net/20141105205315933?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVlZ2FuZzEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
16QAM误码率曲线
![](https://img-blog.csdn.net/20141105205416321?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVlZ2FuZzEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
【详细资料，及相应MATLAB代码，C语言代码。咨询qq：1279682290】
