# 用STM32内置的高速ADC实现简易示波器 - xqhrs232的专栏 - CSDN博客
2013年04月14日 11:13:53[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3141
原文地址::[http://www.360doc.com/content/10/0912/09/1188581_53024390.shtml](http://www.360doc.com/content/10/0912/09/1188581_53024390.shtml)
做一个数字采样示波器一直是我长久以来的愿望，不过毕竟这个目标难度比较大，涉及的方面实在太多，模拟前端电路、高速ADC、单片机、CPLD/FPGA、通讯、上位机程序、数据处理等等，不是一下子就能成的，慢慢一步步来呗，呵呵，好歹有个目标，一直在学习各方面的知识，也有动力：）由于高速ADC涉及到采样后的数据存储问题，大量的数据涌入使得单片机无法承受，因此通常需要用外部高速RAM加CPLD配合，或者干脆用大容量的 FPGA做数据存储处理等，然后通知单片机将数据发送出去。这部分实在是难度比较大，电路非常复杂，自己是有心无力啊，还得慢慢地技术积累。。。
正好ST新推出市场的以CORTEX-M3为核心的STM32，内部集成了2个1Msps 12bit的独立ADC，并且内部高达72MHZ的主频，高达1.25DMIPS/MHZ的处理速度，高速的DMA传输功能，灵活强大的4个TIMER等等，这些真是非常有吸引力，何不用它来实现一个低频的数字示波器功能呢，我的目标是暂时只要定量定性地分析20KHZ以下的低频信号就行了，目标不高吧，用STM32可以方便地实现，等有了一定经验之后慢慢再用FPGA和高速ADC搞个100Msps采样的示波器！
说来也真是幸运，得到了GRANT兄相赠的STM32F103VB以及评估版的电路板，这些日子一直在学习STM32，不断地做实验，也算是稍微有点入门了，真是了解越多越喜欢这个芯片，呵呵。
想来这个论坛上对数据采样以及数字示波器感兴趣的朋友很多，下面我简单描述下实现方式，发帖也跟大家分享下我的喜悦：）
1、 ADC转换：STM32增强型芯片内置的2个独立ADC，可以有16个通道，并且2个通道可以并行的同步采样，触发方式很灵活，可以通过TIMER以及外部电平等方式触发，并行方式下ADC2自动同步于ADC1；ADC在最高速采样的时候需要1.5+12.5个ADC周期，在14M的ADC时钟下达到 1Msps的速度，因为我主频是72M所以4分频后稍微高了点，18MHZ的ADC时钟，采样速度应该高于1M了。ADC 采样2路同时采样方式，用TIM2 CC2来生成时钟信号触发ADC来实现指定频率的采样。ADC1/ADC2采样的结果是一个word
2、采样频率控制：由于STM32内部的4个TIMER非常强大，每个TIMER又有4个通道，再加上独立的预分配器，实际上可以实现任意分频，因此用TIM2 CC2来产生指定频率的时钟，用来触发ADC1连续采样。
3、采样数据传输及每次采样深度控制：ADC产生的转换数据通过高速DMA 通道1来传输置指定的内部RAM中，并且将DMA通道一设置成最高优先级，以保证数据准确，并且用DMA每次传输的个数来控制采样的深度，例如我要采集 100个点那么就设置DMA传输100个次，每次从32位ADC转换寄存器传输一个word到RAM中，等完成了100次传输后，DMA通道自动停止（实际上ADC是一直按照要求的采样频率连续在后台采样，只是我去取数据而已），下次采集的时候我只要再设置下采样的个数使能DMA
 CHANNEL1就行了。
4、与上位机通讯：通讯也是个难题，要达到快速地将大量数据发给上位机的目的，传输的速率肯定低不了，开始我想先用串口，不过很快就放弃了，一则即使我用外部USB转串口的芯片最高也只能达到1M的速度，并且数据会丢失；后来还是采用了网络传输的方式，用SPI 接口的ENC28J60芯片，这个芯片我在MEGA32和AT91SAM7S64上都用过，接口简单挺方便的，速度还可以，在SAM7S64上DMA凡是用UDP协议单向发送的速度可以达到400KB/S以上，这次用了STM32发现速度大增，经过我用STM32的DMA传输后，同样UDP协议单向发速度竟然达到了500KB/S以上，甚至最高可以达到600KB/S，这个真是意外的收获。
5、上位机程序：还是用VS2005，我还是喜欢用C#，主要是微软的C#做得是在太舒服了，编辑器智能化程度真高，我只要刚刚输个开头的字母，马上就感知出来一堆让你选择，连挨个敲字符的功夫都省了，还不用担心拼写出错到时候找原因的麻烦，呵呵，缺点就是程序执行时候CPU利用率要高点，什么时候它的C++ 编辑器也到这个程度我就换回C++，哈哈。波形显示还是用NI的measurementStudio8来实现，一个是漂亮方便，另外最要紧的就是
 MeasurementStudio8里面有一大堆数据处理的库，从简单的波形有效值计算，频率计算，到各种各样的函数滤波器功能，还有FFT频域分析，时域分析等等，但凡要用到的仪器相关处理里面都有，另外本来我打算要在模拟前端里面加一个相位锁定的电路，以固定显示的波形起点，后来发现 MeasurementStudio8里面有个PeakDetector的类，用这个来实现波形的锁定连这个电路都可以省了。用 MeasurementStudio8来实现实在是非常方便，并且准确。只是我没啥资料，还在探索当中
显示的界面及部分照片：
数据采样后输出到PC上显示的图形很精确，包括MAX038产生的正弦波上部的小尖峰也很清楚，STM32的ADC精度很稳定性相当好，对于音频范围的低频信号来说，1Msps的采样也基本够用了。只要采集足够的点送给measurementsudio提供的函数来分析，可以达到非常精确的程度，12BIT 的分辨率相当于数字表的3位半的效果，用来测试信号的频率、真有效值、峰值、峰峰值等等非常方便和精确，和我用硬件实现的频率计和真有效值的读数相同（这也说明了我做的信号发生器的硬件是准确的，哈哈，之前跟数字表总对不上，看来是数字表准确度差），实现完全可以当作低频示波器来用，再加上个模拟前端电路，完全可以实用化了
正弦波：
点击查看图片
上位机的程序：
上位机的程序还处在对于measuremenStudio的摸索当中，只是初步了解到了几个函数，用它来实现数据处理实在是方便，look public void DataReceived_Proc() //UDP数据接收、数据处理、数据显示函数
{
try
{
while (bStates)
{
myudpcomm.Receive(ref CommReceiveBuffer);
Received_Command = Bytes2Struct(ref CommReceiveBuffer);
//textBox3.Text = Received_Command.SampleRate.ToString() + (acEstimate++).ToString();
dADC1_Result = new double[Received_Command.SampleDepth];
dADC2_Result = new double[Received_Command.SampleDepth];
//数据处理，将通讯接收区中的ADC数据传入绘图用数组中
for (int i = 0; i < (int)(Received_Command.SampleDepth); i++)
{
dADC1_Result = (BitConverter.ToUInt16(CommReceiveBuffer, 40 + 4 * (i + 0))) * (3.3 / 4096.0);
dADC2_Result = (BitConverter.ToUInt16(CommReceiveBuffer, 40 + 4 * (i + 0) + 2)) * (3.3 / 4096.0);
}
str = "通道A(绿色)\r\n";
//测试真有效值
Measurements.ACDCEstimator(dADC1_Result, out acEstimate, out dcEstimate);//交流(AC方式相当于信号通过一个电容隔直后进行测量)和直流(DC直通方式进行测量)真有效值测量
str += "AC方式有效值：" + ((int)(acEstimate * 1000)).ToString() + "mV" + "DC方式有效值" + ((int)(dcEstimate * 1000)).ToString() + "mV\r\n";
//测试信号频率、振幅Vp
mySingleToneInformationADC1 = new SingleToneInformation(dADC1_Result, Received_Command.SampleRate);
str += "频率：" + ((int)(acEstimate * 1000)==0 ? 0int )mySingleToneInformationADC1.Frequency).ToString() + "Hz" + "振幅Vp：" + ((int )mySingleToneInformationADC1.Amplitude*1000).ToString() + "mV\r\n";
str += "\r\n通道B(红色)\r\n";
//测试真有效值
Measurements.ACDCEstimator(dADC2_Result, out acEstimate, out dcEstimate);//交流(AC方式相当于信号通过一个电容隔直后进行测量)和直流(DC直通方式进行测量)真有效值测量
str += "AC方式有效值：" + ((int)(acEstimate * 1000)).ToString() + "mV" + "DC方式有效值" + ((int)(dcEstimate * 1000)).ToString() + "mV\r\n";
//测试信号频率、振幅Vp
mySingleToneInformationADC2 = new SingleToneInformation(dADC2_Result, Received_Command.SampleRate);
str += "频率：" + ((int)(acEstimate * 1000) == 0 ? 0 : (int)mySingleToneInformationADC1.Frequency).ToString() + "Hz" + "振幅Vp：" + ((int)mySingleToneInformationADC1.Amplitude * 1000).ToString() + "mV\r\n";
textBox3.Text = str;
//ThresholdPeakDetector.Analyze用来找出从波谷到波峰上升沿顶点的数组序号
//可以用于固定显示波形从上升沿的某固定点开始，相当与硬件的同步触发电路功能
//b = ThresholdPeakDetector.Analyze(dADC2_Result, 2, 10);
//foreach (int k in b)
//{
//textBox3.Text += k.ToString() + " ";
//}
//for (int i = 0; i < Received_Command.SampleDepth - b[1]; i++)
{
//dADC1_Result = dADC2_Result[i + b[1]];
}
//textBox3.Text += b[b.Length - 1].ToString();
//bIsUdpDataReceived = true;//表示接收到了UDP数据，允许进行再次发送
bIsDataReadyForPlot = true;
myGraphPlotProc();//绘图输出*/
//myD1 = new myMethodDelegate(h);
//myD1(1);
}
}
catch (Exception e1)
{
timer1.Enabled = false;
MessageBox.Show(e1.ToString());
}
finally
{
timer1.Enabled = false;
}
}
/************************************************************************************
* 绘图输出过程函数供，mygGraphPlotThread进程调用
* 始终循环检测bIsDataReadyForPlot,一旦为真则进行绘图，绘图完成后置标志为false
* **********************************************************************************/
public void myGraphPlotProc()//绘图输出函数
{
//while (true )
{
if(bIsDataReadyForPlot)
{
waveformPlot1.PlotY(dADC1_Result);
waveformPlot2.PlotY(dADC2_Result);
bIsDataReadyForPlot = false;
}
}
}
下位机的程序：
下位机的程序，也还在完善，现在只做到了基本的功能，还不稳定，主要问题还是在传输上的，这次为了一次传输比较多的数据，要将UDP数据包分解，分成多个小于1518字节的帧发送，因此发现当数据发送快的时候很容易导致数据停止发送，以前用MEGA32和SAM7的时候没注意过，当时的处理速度也慢，没暴露出来，想来想去可能是由于连续发送的时候速度太快导致的冲突，ENC28J60出错挂起了，还是ENC28J60没有吃透，对于里面的流控、以太网冲突检测这些还需要进一步研究。
/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
*STM32F10*** 双通道ADC数据采集并通过ENC28J60实现UDP通讯传输
*作者:alien2006
*环境:keil for arm mdk 3.15b
*版本:V0.2
*时间:20071202
*说明:V0.2
*一、网络通讯部分
*1、先采用STM32 SPI轮询方式进行传输试验，ping 192.168.1.100 -l 1400 -n 10
*在轮询方式下未改进SPI1_SendByte()函数(内部直接用ST提供的函数语句)需 avg=9ms时间
*轮询方式下将SPI1_SendByte()函数中的4条语句修改为直接寄存器存取后avg提高到7ms
*轮询方式下取消SPI1_SendByte()直接代之以函数中四语句avg提高到6ms
*经过上述的逐步修改，传输UDP1400个字符时双向传输(接收1400个字节再发送这1400个字节)间隔4MS可达210KB/S
*2、enc28j60.c修改增加STM32 SPI传输DMA和非DMA编译选项，DMA方式下网络最大传输速度测试达到350KB/S
*3、改进了ZYP_UDP.C实现了当要发送的UDP数据长度超过单帧所能容纳时，将UDP数据
*自动进行分组，并可在编译时自定义每个分组长度;
*改进了ENC28J60.C加入了ENC28J60DMA空闲和网络发送完毕的判断，解决了当发送速度过快时导
*致传输出错问题。测试单向发送速度超过500KB/S;
*二、STM32数据采集部分
*1、ADC1/ADC2实现并行同时数据采集，12BIT最高可达1MSPS采样速度并通过STM32的DMA传输放入内存中
*2、TIM2 CC2实现对ADC采样的触发，ADC_Sample_Frequency_Set函数实现自定义TIM2 OC2频率输出，
*3、采样的频率和采样个数通过接收到的UDP控制命令来指定
*采样的频率为20HZ~1MHZ;
*采样深度为1~4000个数据(受限于STM32内存20KB容量,一个数据为2个12bitADC通道读数,需一个word)
*4、定义了简单的UDP控制命令结构，用于实现与PC通讯和控制采样频率和采样深度
*三、其他
*1、程序待解决问题：UDP分组发送出错问题未完全解决，有待进一步解决
*2、期待增加模拟前端电路，并实现放大倍数程控，通过上位机程序可以设置
*
* V0.1：最初程序，实现简单固定频率和深度的并行ADC采样和UDP通讯，并编制了简单的上位机程序，
*可以进行采样波形的显示
*******************************************************************************/
贴下ADC和时钟部分的程序，这里都是高手，大家不要笑我啊，呵呵
/*******************************************************************************
* Function Name: DMA_ADC_Transfer_Reset
* Description: ADC1/ADC2 DMA传输通道复位，准备下一次DMA传输
* Input: None
* Output: None
* Return: None
*******************************************************************************/
void DMA_ADC_Transfer_Reset(void)
{
//开始DMA传输
//以下5句是采用函数方式共耗时多达百多个周期
//DMA_Cmd(DMA_Channel1, DISABLE);//先要禁止DMA_ChannelX,才能修改DMA通道X传输数量寄存器DMA_CNDTRx
//DMA_ClearFlag(DMA_FLAG_GL1|DMA_FLAG_TC1|DMA_FLAG_HT1|DMA_FLAG_TE1);
//DMA_InitStructure.DMA_BufferSize = Transfer_ReceiveData_Buffer.InWord.SampleDepth;//重新设置要通过DMA传输数据量
//DMA_Init(DMA_Channel1, &DMA_InitStructure);
//DMA_Cmd(DMA_Channel1, ENABLE);// Enable DMA channel1
//以下4句是采用直接写寄存器方式一共耗时24个周期
DMA_Channel1->CCR &= ~(1<<0); //禁用DMA_Channel,EN是CCR1寄存器的0位
DMA->IFCR |= 0x0000000F;//清除CHANNEL1的4个标志
DMA_Channel1->CNDTR = (u16)Transfer_ReceiveData_Buffer.InWord.SampleDepth;//重新设置要设置的DMA传输数据量
DMA_Channel1->CCR |= (1<<0);//重新使能DMA_channel1
while(!(DMA->ISR & DMA_FLAG_TC1));
DMA_Channel1->CCR &= ~(1<<0); //禁用DMA_Channel,EN是CCR1寄存器的0位
}
/*******************************************************************************
* Function Name: DMA_ADC_Transfer_Init
* Description: ADC1/ADC2 DMA传输通道初始化
* Input: None
* Output: None
* Return: None
*******************************************************************************/
void DMA_ADC_Transfer_Init(void)
{
/* DMA channel1 configuration ----------------------------------------------*/
DMA_DeInit(DMA_Channel1);
DMA_InitStructure.DMA_PeripheralBaseAddr = ADC_DR_Address;//ADC数据寄存器地址
DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&Transfer_SendData_Buffer.InWord.data[0];//目标缓冲区地址
DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//外设是源
DMA_InitStructure.DMA_BufferSize = 0;//设置DMA读取长度为
DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址不递增
DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//目标缓冲区地址递增
DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;//外设数据宽度
DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;//目标缓冲区数据宽度
DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//DMA模式：Circular循环模式/Normal普通模式
DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;//优先级
DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//内存到内存模式不使能
DMA_Init(DMA_Channel1, &DMA_InitStructure);
/* Enable DMA channel1 */
//DMA_Cmd(DMA_Channel1, ENABLE);
}
/*******************************************************************************
* Function Name: ADC_Initial
* Description: ADC1/ADC2初始化
* Input: None
* Output: None
* Return: None
*******************************************************************************/
void ADC_Initial(void)
{
/* ADC1 configuration ------------------------------------------------------*/
ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;//ADC1/ADC2同时并行采样模式
ADC_InitStructure.ADC_ScanConvMode = DISABLE;//多通道扫描模式
ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//单次转换模式(转换后即停止)
ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2;//触发模式
ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//数据右对齐
ADC_InitStructure.ADC_NbrOfChannel = 1;
ADC_Init(ADC1 , &ADC_InitStructure);
/* ADC1 regular channel14 configuration */
ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_1Cycles5);
/* Enable ADC1 DMA */
ADC_DMACmd(ADC1, ENABLE);
/* ADC2 configuration ---配置同ADC1--------------------------------------------*/
ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;
ADC_InitStructure.ADC_ScanConvMode = DISABLE;
ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2;
ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
ADC_InitStructure.ADC_NbrOfChannel = 1;
ADC_Init(ADC2, &ADC_InitStructure);
/* ADC2 regular channels configuration */
ADC_RegularChannelConfig(ADC2, ADC_Channel_11, 1, ADC_SampleTime_1Cycles5);
/* Enable ADC2 external trigger conversion */
ADC_ExternalTrigConvCmd(ADC2, ENABLE);
/* Enable ADC1 */
ADC_Cmd(ADC1, ENABLE);
/* Enable ADC1 reset calibaration register 校准ADC1*/
ADC_ResetCalibration(ADC1);
/* Check the end of ADC1 reset calibration register */
while(ADC_GetResetCalibrationStatus(ADC1));
/* Start ADC1 calibaration */
ADC_StartCalibration(ADC1);
/* Check the end of ADC1 calibration */
while(ADC_GetCalibrationStatus(ADC1));
/* Enable ADC2 */
ADC_Cmd(ADC2, ENABLE);
/* Enable ADC2 reset calibaration register 校准ADC2*/
ADC_ResetCalibration(ADC2);
/* Check the end of ADC2 reset calibration register */
while(ADC_GetResetCalibrationStatus(ADC2));
/* Start ADC2 calibaration */
ADC_StartCalibration(ADC2);
/* Check the end of ADC2 calibration */
while(ADC_GetCalibrationStatus(ADC2));
/* Test on channel1 transfer complete flag */
//while(!DMA_GetFlagStatus(DMA_FLAG_TC1));
/* Clear channel1 transfer complete flag */
//DMA_ClearFlag(DMA_FLAG_TC1);
ADC_ExternalTrigConvCmd( ADC1, ENABLE);
}
/*******************************************************************************
* Function Name: ADC_Sample_Frequency_Set
* Description: 根据输入的频率设置，TIM2_CC2产生相应的频率
*用来控制ADC的采样，Frequency=1000000/(Prescaler+1)来产生
*因此有些频率计算不准确，一般频率为2或5的倍数才准确
*频率范围为16Hz~1000,000Hz
* Input: u16 Frequency:输入所需要的采样频率
*
* Output: None
* Return: None
*******************************************************************************/
void ADC_Sample_Frequency_Set(u32 Frequency)
{
TIM_Cmd(TIM2, DISABLE);//先停止TIM2时钟，以准备下面的设置
/* -----------------------------------------------------------------------
TIM2 配置: 产生TIM2_CC2时钟控制信号用于控制ADC采样
TIM2CLK = 72 MHz
TIM2 ARR Register = 35 => TIM3 Frequency = (TIM3 counter clock/(ARR + 1))/2
TIM2 Frequency = 1000 KHz.
----------------------------------------------------------------------- */
/* Time base configuration */
TIM_TimeBaseStructure.TIM_Period = 35; //APR寄存器
TIM_TimeBaseStructure.TIM_Prescaler = 1000000/Frequency-1; //预分频值，用来调整频率，分频系数=1000khz/(prescaler+1)
TIM_TimeBaseStructure.TIM_ClockDivision = 0;
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
/* TIM_OCMode_Toggle Mode configuration: Channel2 */
TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
TIM_OCInitStructure.TIM_Channel = TIM_Channel_2;
TIM_OCInitStructure.TIM_Pulse = 35;
TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
TIM_OCInit(TIM2, &TIM_OCInitStructure);
//TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
/*---------------------------------------*/
//TIM_ARRPreloadConfig(TIM2, ENABLE);
/* TIM2 enable counter */
TIM_Cmd(TIM2, ENABLE);
转自Tony嵌入式论坛,地址:http://www.cevx.com/Bbs/thread-24545-1-1.html
