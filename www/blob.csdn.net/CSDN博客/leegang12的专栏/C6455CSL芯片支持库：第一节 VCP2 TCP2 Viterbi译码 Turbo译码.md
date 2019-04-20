# C6455CSL芯片支持库：第一节 VCP2/TCP2 Viterbi译码 Turbo译码 - leegang12的专栏 - CSDN博客
2013年06月23日 00:44:16[leegang12](https://me.csdn.net/leegang12)阅读数：1745

 0. C6455CSL
DSP芯片：C6455、C6474、C6416等，具有VCP/TCP协处理器，VCP2/TCP2协处理器功能，来进行Viterbi译码，turbo译码。作者耗时三周，成功调试通了这些DSP芯片的协处理器功能，正确Viterbi译码，turbo译码。
1. C6455CSL之VCP2
       C6455的维特比译码协处理器VCP2和Turbo译码协处理器TCP2在C6416T的VCP和TCP的基础上，功能和性能上都有了较大的提升。它们不仅全面兼容VCP和TCP，而且无论在译码速度、动态范围、支持的帧长和编码方式、编程的灵活度以及功耗等方面都有了比较大的改进。 
       具体来说，VCP的运行时钟等于CPU内核时钟/4，而VCP2提升到了CPU内核时钟/3；分支度量（Branch Metrics）的位宽由VCP的7 bit提高到了VCP2的8 bit，VCP2支持的最大滑动窗口也比VCP的增加了30%左右。
针对卷积编码的viterbi译码，作者整理了：基于TI芯片C6455的DSP程序例程（vcp2_hard_decisions，和vcp2_soft_decisions），主要寄存器配置参考C语言代码，及VCP2/TCP2完整的用户手册资料。
VCP2用户手册资料：
       TMS320C645x DSP Viterbi-Decoder Coprocessor 2 (VCP2) User's Guide
信道编译码技术的研究与DSP实现之TCP2/VCP2(硕士论文)
DSP程序工程参考：
        a)csl_c6455\example\vcp2\vcp2_hard_decisions
        b)csl_c6455\example\vcp2\vcp2_soft_decisions
VCP2_soft_decisions程序主要寄存器配置如下：
       /*      Frame length...................................186                                
*      rate...........................................1/2
*      constraint length..............................9
*      Decision mode..................................Soft 
        *      Traceback Mode.................................Mixed
*      Index of the initial max state metric(IMAXI)...63 
*      output parameters read flag....................TRUE
        *      Yamamoto enable................................TRUE
        *      Yamamoto threshold.............................80
*      Traceback state enable.........................False 
*      Traceback state index..........................N/A  
*      DSP endian.....................................Little 
*      Endian Register................................bm_endian
*      Output order...................................1  
     ***********************************************************/
    /* VCP configuration */
    vcpBaseParams.rate      = VCP2_RATE_1_2;
    vcpBaseParams.constLen  = CONSTRAINT_LEN;
    vcpBaseParams.frameLen  = FRAME_LENGTH;
    vcpBaseParams.yamTh     = YAMAMOTO_THRESHOLD;
    vcpBaseParams.stateNum  = INITIAL_STATE;
    vcpBaseParams.tbConvrgMode = FALSE;
    vcpBaseParams.decision  = VCP2_DECISION_SOFT;
    vcpBaseParams.readFlag  = TRUE;
    vcpBaseParams.tailBitEnable = FALSE;
    vcpBaseParams.traceBackIndex = 0;
    vcpBaseParams.outOrder  = 1;
VCP2_hard_decisions程序主要寄存器配置如下：
/* ===============================================
       Frame length...................................300                                
       rate...........................................1/3
       constraint length..............................5
       Decision mode..................................Hard 
       Traceback Mode.................................Convergent
       Index of the initial max state metric(IMAXI)...0 
       output parameters read flag....................0
       Yamamoto bit enable............................False
       Yamamote threshold.............................N/A
       Traceback state enable.........................False 
       Traceback state index..........................N/A  
       DSP endian.....................................Little 
       Endian Register................................bm_endian
       Output order...................................0       
 ===============================================*/
    /* VCP configuration */
    vcpBaseParams.rate = VCP2_RATE_1_3;
    vcpBaseParams.constLen = CONSTRAINT_LEN;
    vcpBaseParams.frameLen = FRAME_LENGTH;
    vcpBaseParams.yamTh = 0;
    vcpBaseParams.stateNum = 0;
    vcpBaseParams.tbConvrgMode = TRUE;
    vcpBaseParams.decision = VCP2_DECISION_HARD;
    vcpBaseParams.readFlag = 1;
    vcpBaseParams.tailBitEnable = FALSE;
    vcpBaseParams.traceBackIndex = 0;
    vcpBaseParams.outOrder = 0;
2. C6455CSL之TCP2
        Turbo码编码器主要是由编码器，交织器以及删余矩阵和复接器组成。Turbo译码包括：基于最大后验概率(MAP)的译码算法和基于似然序列的软输出维特比(SOVA)译码算法，后来又出现了对MAP算法采取某种变换域，或采取某种数学近似以降低译码复杂度的算法，出现了MAX-Log-MAP算法等。
        TI公司C6455芯片提供了TCP2协处理器，可以灵活配置所要使用的Turbo译码算法。对于Turbo译码协处理器来说，TCP2的时钟由TCP的CPU内核时钟/2降到CPU内核时钟/3，但是支持一次性译码帧长由TCP的5114增大到20730，支持的编码率在TCP的1/2、1/3和1/4的基础上增加了3/4和1/5的编码率，支持重复译码和休眠模式等。 
    针对Turbo译码，作者整理了：基于TI芯片C6455的DSP程序例程（TCP2_standalone_mode，和TCP2_shared_mode），主要寄存器配置参考C语言代码，及TCP2完整的用户手册资料。其中TCP2两种工作模式区别及使用场景，见资料《基于TMS320C64xx协处理器TCP的Turbo译码实现》。
TCP2用户手册资料：
       TMS320C645x DSP Turbo-Decoder Coprocessor 2 (TCP2) Reference Guide
       基于TMS320C64xx协处理器TCP的Turbo译码实现
       Turbo码编译码技术的研究及DSP实现（硕士论文）
DSP程序工程参考：
        a)csl_c6455\example\tcp2\tcp2_standalone_mode
        b)csl_c6455\example\tcp2\tcp2_shared_mode
TCP2_shared_mode程序主要寄存器配置如下：
/* setup the base params */
    configBase.frameLen     = frameLen;
    configBase.inputSign    = TCP2_INPUT_SIGN_POSITIVE;
    configBase.intFlag      = INTERLEAVER_LOAD_FLAG;
    configBase.maxIter      = MAX_TCP_ITERATIONS;
    configBase.maxStarEn    = FALSE;
    configBase.standard     = TCP2_STANDARD_3GPP;
    configBase.crcLen       = 0;  /* Disable CRC */
    configBase.crcPoly      = 0;  /* not used */
    configBase.minIter      = MIN_TCP_ITERATIONS;
    configBase.numCrcPass   = 0; /* default value */
    configBase.outParmFlag  = OUT_PARAM_READ_FLAG;
    configBase.outputOrder  = TCP2_OUT_ORDER_0_31;
    configBase.prologRedEn  = FALSE;
    configBase.prologSize   = PROLOG_SIZE;
    configBase.rate         = TCP2_RATE_1_3;
    configBase.snr          = 0; /* disable SNR threshold checking */
    configBase.map          = TCP2_MAP_MAP1;
TCP2_standalone_mode程序主要寄存器配置如下：
 /* setup the base params */
    configBase.frameLen     = frameLen;
    configBase.inputSign    = TCP2_INPUT_SIGN_POSITIVE;
    configBase.intFlag      = INTERLEAVER_LOAD_FLAG;
    configBase.maxIter      = MAX_TCP_ITERATIONS;
    configBase.maxStarEn    = TRUE;
    configBase.standard     = TCP2_STANDARD_3GPP;
    configBase.crcLen       = 0;  /* Disable CRC */
    configBase.crcPoly      = 0;  /* not used */
    configBase.minIter      = MIN_TCP_ITERATIONS;
    configBase.numCrcPass   = 0; /* default value */
    configBase.outParmFlag  = OUT_PARAM_READ_FLAG;
    configBase.outputOrder  = TCP2_OUT_ORDER_0_31;
    configBase.prologRedEn  = FALSE;
    configBase.prologSize   = PROLOG_SIZE;
    configBase.rate         = TCP2_RATE_1_3;
    configBase.snr          = 0;
/*-----------------------------------------------------------------------------------------------------------------------------*/
如需详细资料，相关经验交流，请联系QQ：1279682290
资料清单：
<1>. TMS320C645x DSP Viterbi-Decoder Coprocessor 2 (VCP2) User's Guide
<2>. 信道编译码技术的研究与DSP实现之TCP2/VCP2(硕士论文)
<3>. VCP2-DSP程序工程参考：
        a)csl_c6455\example\vcp2\vcp2_hard_decisions
        b)csl_c6455\example\vcp2\vcp2_soft_decisions
<4>. TMS320C645x DSP Turbo-Decoder Coprocessor 2 (TCP2) Reference Guide
<5>. 基于TMS320C64xx协处理器TCP的Turbo译码实现（硕士论文）
<6>. Turbo码编译码技术的研究及DSP实现（硕士论文）
<7>. TCP2-DSP程序工程参考：
        a)csl_c6455\example\tcp2\tcp2_standalone_mode
        b)csl_c6455\example\tcp2\tcp2_shared_mode
<8>. C6455_CSL_API函数使用说明（用户手册）
<9>. TMS320C6455的硬件资源分析（硕士论文）
<10>. C6455概述（用户手册）
/*-------------------------------------------------------------------------------------------------------------------------------*/
