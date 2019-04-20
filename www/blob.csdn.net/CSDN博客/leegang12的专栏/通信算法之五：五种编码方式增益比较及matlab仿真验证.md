# 通信算法之五：五种编码方式增益比较及matlab仿真验证 - leegang12的专栏 - CSDN博客
2014年08月31日 10:45:53[leegang12](https://me.csdn.net/leegang12)阅读数：5101
1. 卷积码增益性能、误码率
    信道环境：AWGN
    信噪比SNR ：0:0.1:6
    MATALB仿真架构：源比特 +卷积码 +BPSK +AWGN +Viterbi +BER
    说明：卷积编码，不同的R码率，不同的约束长度
2. Turbo增益性能、误码率
    信道环境：AWGN
    信噪比SNR ：-1:0.1:2
    MATALB仿真架构：源比特 +Turbo编码 +BPSK +AWGN +Turbo译码+BER
    说明：turbo译码，不同的译码算法那，不同的交织长度，不同的迭代次数。
              LTE 标准的turbo
      3GPP标准的turbo  （DSP6455，TCP2，VCP2，也验证过正确性）
3. LDPC增益性能、误码率
    信道环境：AWGN
    信噪比SNR ：-1:0.1:5
    MATALB仿真架构：源比特 +LDPC编码 +BPSK +AWGN +LDPC译码+BER
    说明：LDPC，在DVB-S2，CCSDS应用。
4. BCH/RS增益性能、误码率
    信道环境：AWGN
    信噪比SNR ：-1:0.1:5
    MATALB仿真架构：源比特 +BCH编码 +BPSK +AWGN +BCH译码+BER
<详细资料，及相应MATLAB代码，咨询qq：1279682290 >
