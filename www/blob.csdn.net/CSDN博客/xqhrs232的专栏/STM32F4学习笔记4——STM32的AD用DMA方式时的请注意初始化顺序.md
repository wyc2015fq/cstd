# STM32F4学习笔记4——STM32的AD用DMA方式时的请注意初始化顺序 - xqhrs232的专栏 - CSDN博客
2018年09月19日 09:38:23[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：91
原文地址::[https://blog.csdn.net/hnxyxiaomeng/article/details/53122616](https://blog.csdn.net/hnxyxiaomeng/article/details/53122616)
相关文章
1、stm32 adc多通道采集 使用dma 为什么只显示一个通道的值啊？----[http://www.openedv.com/posts/list/8407.htm](http://www.openedv.com/posts/list/8407.htm)
今天无意之间看到了这篇帖子，是斑竹好久前发的~~~
我突然意识到我的数据好像一直没有错位，看到这个我又试验了一下，发现把DMA_Cmd(DMA1_Channel1, ENABLE);放置到哪儿都是一样的啊~~~ 
这个是原帖部分： 
STM32的AD用DMA方式时的请注意初始化顺序 
珍情岁月(84848098) 17:19:23
```
DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_Result;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 6;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  //DMA_Cmd(DMA1_Channel1, ENABLE);//以前在这里启用DMA，出现通道错乱
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 6;
  ADC_Init(ADC1, &ADC_InitStructure);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_55Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_55Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 4, ADC_SampleTime_55Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 6, ADC_SampleTime_55Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 5, ADC_SampleTime_55Cycles5);
  ADC_DMACmd(ADC1, ENABLE);
  ADC_Cmd(ADC1, ENABLE);
  ADC_ResetCalibration(ADC1);
  while(ADC_GetResetCalibrationStatus(ADC1));
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1));
  DMA_Cmd(DMA1_Channel1, ENABLE);
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
```
- 
以上代码转自STM32交流群的“情岁月”(84848098) 17:27:48的发言，在此对他表示感谢。
下面的经验也是“珍情岁月”提供：
用DMA连续采样多路，自动触发，如果在初始化AD前使能了DMA就会出现数据错位现象。 
要注意在初始化时将DMA_Cmd(DMA1_Channel1, ENABLE);放置到ADC_SoftwareStartConvCmd(ADC1, ENABLE);前面。
原因是：如果在初始化AD的时候DMA被触发了一次，但是此时并没有采样，但是DMA目的地址已经发生了自加， 
当你采样第一路的时候，数据却填充到了第二路。
校准AD的时候会触发DMA导致通道错位，因此校准AD基准前不要启用DMA。
