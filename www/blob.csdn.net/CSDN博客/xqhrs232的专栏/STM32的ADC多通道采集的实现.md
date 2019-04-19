# STM32的ADC多通道采集的实现 - xqhrs232的专栏 - CSDN博客
2013年04月14日 11:33:33[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3582
原文地址::[http://www.9mcu.com/9mcubbs/forum.php?mod=viewthread&tid=961171](http://www.9mcu.com/9mcubbs/forum.php?mod=viewthread&tid=961171)
相关网帖----[http://d1.ourdev.cn/bbs_upload782111/files_37/ourdev_624088CA1C2E.pdf](http://d1.ourdev.cn/bbs_upload782111/files_37/ourdev_624088CA1C2E.pdf)
在经过半个月的不懈努力下，终于将32的ADC多通道的信号采集完成，在网上能找到一些对于多通道信号采集的范例，要么就是用多个ADC，要么就是指贴出部分关键代码，也不对其中的配置及实现做一些细致的说明，这让人会走很多弯路，但也正是这样才能不断提升，才能认识自己的不足。好了，下面就先对STM的ADC做个简单的介绍；
         ADC又称'模拟/数字转换器'，在[STM32](http://www.9mcu.com/9mcubbs/forum.php?mod=viewthread&tid=1005681)[单片机](http://www.9mcu.com/9mcubbs/forum.php?mod=viewthread&tid=1005681)中模拟/数字转换器（ADC）是一种提供可选择多通道输入，逐次逼近型的模数转换器。分辨率为12位，在一般的信号采集应用中已经算是很高的分辨率了。
        它有18个通道，可测量16个外部和2个内部信号源。各通道的A/D转换可以单次、连续、扫描或间断模式执行。ADC的结果可以左对齐或右对齐方式存储在16位数据寄存器中。
        下面就是对ADC多通道采集是如何实现做详细说明：
        第一：先讲讲大家在调试这功能可能遇到的一些困惑，我也是这样过来的，不知道你们是不是这样.
        1、范例是用DMA的方式对所采集到的数据进行的读取，这是我首先困惑的，存储到哪个地方去了，为此我还专门看了DMA（直接记忆存取/直接内存存取）到底是什么东西；
        2、我们就认为采集的数据已经被DMA提取并存储了，怎么把这么多通道的数据分离！！！ 真滴是伤神~~~
        把上面这两个主要的大问题解决了，那么就能将这个模块调通，并能在实际中得到应用。16路信号的采集还不是小小的应用哟~~~
        下面上代码
//多通道转换函数
vu32 ADC_MultiChannelConvertedValue;
void ADC1_MultiChannel_Configuration(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;
        DMA_InitTypeDef DMA_InitStruct;
        ADC_InitTypeDef ADC_InitStruct;
        //ADC input init PA0-7 AD0-7
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
        GPIO_InitStructure.GPIO_Pin = GPIO_PinSource0|GPIO_PinSource1|GPIO_PinSource2|GPIO_PinSource3|GPIO_PinSource4|GPIO_PinSource5|GPIO_PinSource6|GPIO_PinSource7;
        GPIO_Init(GPIOA,&GPIO_InitStructure);
        //ADC input init PB0-1 AD8-9
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
        GPIO_InitStructure.GPIO_Pin = GPIO_PinSource0|GPIO_PinSource1;
        GPIO_Init(GPIOB,&GPIO_InitStructure);
        //ADC input init Pc0-5 AD10-15
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
        GPIO_InitStructure.GPIO_Pin = GPIO_PinSource0|GPIO_PinSource1|GPIO_PinSource2|GPIO_PinSource3|GPIO_PinSource4|GPIO_PinSource5;
        GPIO_Init(GPIOC,&GPIO_InitStructure);
        DMA_DeInit(DMA1_Channel1);//开启DMA1的第一通道
        DMA_InitStruct.DMA_PeripheralBaseAddr = ADC1_DR_Address; //DMA对应的外设基地址
        DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
        DMA_InitStruct.DMA_MemoryBaseAddr = (unsigned long)&ADC_MultiChannelConvertedValue; 
       这就是单片机采集后存储的值的地址
        DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
        DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
        DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;//DMA_MemoryDataSize_HalfWord  DMA搬运的数据尺寸，注意ADC是12位的，HalfWord就是16位。这个地方只有用word方式才能对位转换搬移，具体原因不详wxm
        DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Disable;
        DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable; //接收一次数据后，目标内存地址是否后移--重要概念，用来采集多个数据的，多通道的时候需要使能它
        DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//接收一次数据后，设备地址是否后移
        DMA_InitStruct.DMA_Mode  = DMA_Mode_Circular;//转换模式，循环缓存模式，常用
        DMA_InitStruct.DMA_Priority = DMA_Priority_High;//DMA优先级，高
        DMA_InitStruct.DMA_BufferSize = 16;//DMA缓存大小，1*16个
        DMA_Init(DMA1_Channel1,&DMA_InitStruct);
        ADC_InitStruct.ADC_Mode = ADC_Mode_Independent; //转换模式，为独立转换
        ADC_InitStruct.ADC_ScanConvMode = ENABLE; 
        ADC_InitStruct.ADC_ContinuousConvMode = ENABLE; //连续转换模式开启
        ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//ADC外部触发开关，关闭 
        ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right; //对齐方式，ADC结果是12位的，显然有个对齐左边还是右边的问题。一般是右对齐
        ADC_InitStruct.ADC_NbrOfChannel = 16; //开启通道数，16个
        ADC_Init(ADC1, &ADC_InitStruct); 
        /* Enable ADC1 */ 
        ADC_Cmd(ADC1, ENABLE); 
        /* Enable ADC1 reset calibaration register */    
        ADC_ResetCalibration(ADC1); 
        /* Check the end of ADC1 reset calibration register */ 
        while(ADC_GetResetCalibrationStatus(ADC1)); 
        /* Start ADC1 calibaration */ 
        ADC_StartCalibration(ADC1); 
        /* Check the end of ADC1 calibration */ 
        while(ADC_GetCalibrationStatus(ADC1));  
}
//多通道转换函数
u8 MultiChannel_AsciiBuff[5];
void Read_ADC1_MultiChannel(unsigned char u8_channNo)
{  
          unsigned short  ADC_data = 0;
        unsigned char i,id;
        ADC_SoftwareStartConvCmd(ADC1, DISABLE);
        for(i=16;i>0;i--)
        {        
                //ADC1,ADC 通道 x,规则采样顺序值为 1,采样时间为 7.5 周期,大于此周期除了通道 0 能识别外，其他不能识别
                ADC_RegularChannelConfig(ADC1, u8_channNo, 1, ADC_SampleTime_7Cycles5 );
                ADC_SoftwareStartConvCmd(ADC1, ENABLE);                         /* 软件启动 ADC1 */                
                do
                {
                        id = ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC);                         /* 读取转换标志 */
                }        while(!id);                                                                                 /* 等待，直到转换完成
 */
ADC_data += ADC_GetConversionValue(ADC1);                   
             这里将值读出，后面就是解析的过程咯
                ADC_SoftwareStartConvCmd(ADC1, DISABLE);                                 /* 软件停止 ADC1 */                      
                                                    
        }
        ADC_data = ADC_data >> 4;                                                                         /* 取 16 次采样平均值 */ 
       
        ADC_data = (unsigned int)(((unsigned int)ADC_data)*3300>>12);/* 把采集到的数值转换成电压数据 */
        MultiChannel_AsciiBuff[0] = ADC_data/1000%10 + 0x30;
        MultiChannel_AsciiBuff[1] = ADC_data/100%10 + 0x30;
        MultiChannel_AsciiBuff[2] = ADC_data/10%10 + 0x30;                                                 /* 数据分离送 USART1 显示
 */
        MultiChannel_AsciiBuff[3] = ADC_data%10 + 0x30;
        MultiChannel_AsciiBuff[4] = 0;
        USART1Write((u8*)MultiChannel_AsciiBuff,4);
        USART1Write((u8*)"\r\n",sizeof("\r\n"));
        //return (unsigned int)(((unsigned long)(ADC_data>>4))*3300>>12);
}  
        对于那两个问题的答案也在这里面。首先是DMA的存储地址，用横着的红线做了标记；对于采集数据的分离在最后一个红色竖线部分。呵呵，看着是不是就很简单咯~   
