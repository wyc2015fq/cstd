# 通信算法之八：通信系统算法设计、RM编码算法性能验证 - leegang12的专栏 - CSDN博客
2014年09月26日 14:50:02[leegang12](https://me.csdn.net/leegang12)阅读数：1529
1.RM编码选择
   信道环境：AWGN
   信噪比：SNR 
   编码 : RM(5,16)，RM(10,32)
MATALB仿真架构：
   源比特 +RM +QPSK调制+ 扩频+AWGN +解扩+QPSK解调+RM +BLER
  说明：两种RM编码方式。
  其中RM(10,32)的BLER = -21dB，rawBer = -8.5dB。
<详细资料，及相应MATLAB代码，相应C语言代码。咨询qq：1279682290 >
