# STM32 DMA 多通道 ADC 采集 - xqhrs232的专栏 - CSDN博客
2018年09月19日 09:44:31[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：139
原文地址::[https://blog.csdn.net/dearwind153/article/details/77621351](https://blog.csdn.net/dearwind153/article/details/77621351)
相关文章
1、STM32——多路ADC通道+DMA+定时器循环采集传感器数据（一）----[https://blog.csdn.net/IL_LBJ/article/details/81017012](https://blog.csdn.net/IL_LBJ/article/details/81017012)
2、STM32——多路ADC通道+DMA+定时器循环采集传感器数据（二）----[https://blog.csdn.net/IL_LBJ/article/details/81018521](https://blog.csdn.net/IL_LBJ/article/details/81018521)
**1. 本次实践目标**
    a. 完成外部 ADC 的采集, 连接引脚为 PC2 
    b. 完成 STM32 内置温度传感器 ADC 的采集
    c. ADC 的采集必须以 DMA 的方式工作，通过 FreeRTOS 系统中创建的一个进程读取采集结果，并打包通过网络发送至上位机
**2. 查看 datasheet **
**   2.1 确认 STM32 内部温度传感器 使用哪个 ADCx , 使用哪个 channel **
         经确认，STM32 内部温度传感器使用 ADC1 的 channel_16。
![](https://img-blog.csdn.net/20170827165314944?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZGVhcndpbmQxNTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20170827165319724?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZGVhcndpbmQxNTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**  2.2 查看 PC2 引脚作为 ADC 输入时，使用哪个 ADCx, 使用哪个 channel**
         经确认，STM32 的 PC2 引脚可以使用 ADC[1,2,3] 的 channel_12。
![](https://img-blog.csdn.net/20170827170131825?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZGVhcndpbmQxNTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**    2.3 通过上面的分析，可以选择 ADC1，进行 DMA 多通道采集 ADC**
         确认，使用 STM32 的 ADC1，后需要确认使用哪个 DMAx
         经过确认，ADC1 使用 DMA1
![](https://img-blog.csdn.net/20170827170742159?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZGVhcndpbmQxNTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
** 2.4 确认外设的时钟源**
          确认 ADC1 的时钟源为 APB2 时钟分枝，并且 GPIOC 的时钟源也为 APB2 时钟分枝 
![](https://img-blog.csdn.net/20170827171400794?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZGVhcndpbmQxNTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**2.5 研究 STM32 DMA 多通道采集的方法**
   2.5.1 位分辨率
   在STM32所有系列芯片中只有少部分是16位的，如：F373芯片。
   12位分辨率意味着我们采集电压的精度可以达到：Vref /4096。
   采集电压 = Vref * ADC_DR / 4096;
   Vref：参考电压
   ADC_DR：读取到ADC数据寄存器的值
   由于寄存器是32位的，在配置的时候分左对齐和右对齐，一般我们使用右对齐，也就是对低12位数据为有效数据。
   2.5.2 转换模式
   A.单次和连续转换
   单次：单通道单次转换、多通道单次（分多次）转换；
   连续：单通道连续转换、多通道连续（循环）转换； 
   B.双ADC模式
   也就是使用到了两个ADC，比如：ADC1和ADC2同时使用也就是双ADC模式。在该模式下可以配置为如下一些模式：同步规则模式、同步注入模式、独立模式等。
     2.5.3 引脚配置
![](https://img-blog.csdn.net/20170827172512722?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZGVhcndpbmQxNTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
     2.5.4 ADC 配置
![](https://img-blog.csdn.net/20170827172911002?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZGVhcndpbmQxNTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
    A.初始化基本参数：
    DMA_PeripheralBaseAddr = ADC1_DR_Address;
        #define ADC1_DR_Address    ((u32)0x40012400+0x4c)
        #define ADC1_BUF_SIZE       2
        #define TOTAL_CONVERT_CH_NUM  2
        vu16 ADC_Buf[2];
    其中 ADC1_DR_Address 可从数据手册中查得
![](https://img-blog.csdn.net/20170827175202411?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZGVhcndpbmQxNTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20170827175222394?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZGVhcndpbmQxNTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
    第一个参数 DMA_PeripheralBaseAddr 用来设置 DMA 传输的外设基地址，比如要进行ADC采集，那么外设基地址 ADC1 数据存储器 ADC1->DR 的地址，表示方法为 (u32) & ADC1->DR，当然也可以通过数据手册直接算出外设的地址。 
    第二个参数 DMA_MemoryBaseAddr 为内存基地址，也就是我们存放DMA传输数据的内存地址 = (u32)ADC_Buf。 
    第三个参数 DMA_DIR 设置数据传输方向，决定是从外设读取数据到内存还送从内存读取数据发送到外设，也就是外设是源地还是目的地，这里我们设置为从外设地址读取数据，所以外设是源地了，所以选择值为DMA_DIR_PeripheralSRC。 
    第四个参数 DMA_BufferSize 设置一次传输数据量的大小，我们需要采集两个通道的数据 所以设置为 ADC1_BUF_SIZE
    第五个参数 DMA_PeripheralInc 设置传输数据的时候外设地址是不变还是递增。设置为DMA_PeripheralInc_Disable； 
    第六个参数 DMA_MemoryInc 设置传输数据时候内存地址是否递增，由于我们是多通道采集，因此需要递增 
    第七个参数 DMA_PeripheralDataSize 用来设置外设的的数据长度是为字节传输（8bits），半字传输(16bits)还是字传输(32bits)，这里我们是 16 位半字传输，所以值设置为 DMA_PeripheralDataSize_HalfWord 
    第八个参数 DMA_MemoryDataSize 是用来设置内存的数据长度，同样设置为半字传输 DMA_PeripheralDataSize_HalfWord。
    第九个参数 DMA_Mode用来设置DMA模式是否循环采集。 
    第十个参数是设置 DMA 通道的优先级，有低，中，高，超高三种模式，这里我们设置优先级别为高级，所以值为DMA_Priority_High。因为开启多个通道，这个值非常重要。 
    第十一个参数 DMA_M2M 设置是否存储到存储器模式传输设置为 DMA_M2M_Disable。
    工作模式：ADC_Mode = ADC_Mode_Independent;
    总共有10种，主要都是针对双ADC下使用。针对初学者这里不多描述，感兴趣的朋友可以先自行研究一下各个模式的使用。
    浏览模式：ADC_ScanConvMode = ENABLE;
    主要是针对多条通道而言，也就是说你是否有多条通道。
    多通道：ENABLE;
    单通道：DISABLE;
    转换模式：ADC_ContinuousConvMode =ENABLE;
    这里是配置是否需要连续转换。
    连续转换ENABLE：也就是只需要启动（触发）转换一次，后面就不用再次启动（触发）就可以连续工作了。
    单次转换DISABLE：也就是根据一次转换完后需要再次启动（触发）才能工作。
    触发方式：ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    触发方式也就是使用什么方法触发ADC转换。哟定时器、外部触发、软件触发，一般常用软件触发。这里有很多种触发方式，详情可以参考其参数。
    对其方式：ADC_DataAlign = ADC_DataAlign_Right;
    右对齐：低12位数据为有效位（常用）；
    左对齐：高12为数据为有效位；
    通道数：ADC_NbrOfChannel =TOTAL_CONVERT_CH_NUM;
    这个参数比较简单，我们定义工作的通道数量。
    B.设置规则组通道：
    ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 1, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 2, ADC_SampleTime_239Cycles5);
    我们定义通道12的转换顺序为第1、通道16的转换顺序为第2；
    至于为什么 ADC_Channel_16 的采集周期要设置为 ADC_SampleTime_239Cycles5，是因为 STM32 的内部温度传感器要求的，现在 ADC1 的时钟源为 APB2 时钟分枝 8 分频，即 72MHz/8 = 9MHz。而要求采样周期 ≥ 17.1us， 所以 17.1 * 9 = 153.9，而最接近的也就是 ADC_SampleTime_239Cycles5
![](https://img-blog.csdn.net/20170827173635398?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZGVhcndpbmQxNTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
     2.5.4 读取采集结果
![](https://img-blog.csdn.net/20170827181215133?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZGVhcndpbmQxNTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**到此，本次实践总结完毕。**
**       其实，其它的  外设 DMA 控制方式与些类似，本次总结比较详细，也算是一个笔记。**
