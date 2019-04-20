# 通信算法之十三：RS编码与译码仿真链路 - leegang12的专栏 - CSDN博客
2014年11月16日 21:22:45[leegang12](https://me.csdn.net/leegang12)阅读数：1909
一. RS编码与译码原理
       RS（Reed-Solomon）码，即里德索罗蒙码，它既能纠正随机误码和突发误码。广泛用于DVB,DVD,数字电视，深空通信，CCSDS。RS码是一种多进制、具有强纠错能力的码，特别适用于数字电视的信道纠错。在DVB的系统中采用的是RS(204，188)，在ATSC的系统中是RS(207，187)，深空通信系统中使用RS(255，235)。
RS编码包括系统编码和非系统编码。RS译码主要包括，时域译码和频域译码。
二. 仿真链路：
仿真平台：matlab
信道环境：高斯白噪声
信噪比SNR
仿真链路：源比特+RS编码+QPSK调制+AWGN+QPSK解调+RS译码+BER
![](https://img-blog.csdn.net/20141116212209156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVlZ2FuZzEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

