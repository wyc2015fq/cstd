# 关于STM32 ADC DMA 使用心得（2） - xqhrs232的专栏 - CSDN博客
2013年04月27日 15:38:27[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1287
个人分类：[ARM/STM32/LPC17/单片机技术](https://blog.csdn.net/xqhrs232/article/category/939440)
原文地址::[http://home.eeworld.com.cn/my/space.php?uid=310305&do=blog&id=63373](http://home.eeworld.com.cn/my/space.php?uid=310305&do=blog&id=63373)
（二）ADC循环采集六路电压，使用DMA.
     这次实验真的很郁闷，对DMA的不了解让我深陷误区，明白之后，让我更加佩服DMA的强大。
     误区就是：从实验的目标我们知道这次是用DMA把ADC转换的数据传送到内存中的一个数组里存起来，因为是采集6个通道，这里使能了ADC的扫描模式。一旦启动ADC，就会按顺序转换SQRX里选中的通道，问题就是我一开始以为ADC与DMA并不会协调工做，也就是ADC自己转自己的，DMA自己传自己的，这样的话内存里的数组就不是我想要的了，后来着实的研究了很长时间，在群里的一位兄弟的提醒下，我才知道，可能我想的复杂了，也许就可以在ADC转一次，然后DMA把数据传一次，Ok,经过实验得知，这个想法是正确的。
    好了，说了这么多废话，开始进入正题。
    这里使用了ADC1的六个规则通道分别是：CH0、CH1、CH2、CH3、CH14、CH15，
                    分别对应的引脚为PA0、PA1、PA2、PA3、PC4、PC5。
    关于ADC的配置：
    启动了ADC1的扫描模式，还有连续转换模式，独立工作模式（只用1个ADC）,因为用的了DMA，所以也要使能DMA位，使用外部触发(SWSTART)，数据为右对齐。还有SQRX等等就不说了，这里不需要ADC中断的。中断在DMA里。
    关于DMA的配置：
    因为ADC请求规定在DMA1的第一个通道，所以这里使用DMA_CH1,外设地址为ADC唯一的数据寄存器(u32)&ADC1->DR,存储器地址为(u32)SendBuff数组，这个数组可以存放6个元素。这里还有使能传输完中断（TCIF）,选择从外设读取，循环模式，外设地址非增量模式，存储器地址增量模式，外设数据宽度16位，存储器地址16位，非存储器到存储器模式。
    关于DMA中断函数：
   当DMA传输完6次数据时，TCIF位自动置位，程序进入中断服务函数，首先先关闭ADC的连续转换，我们把数组的处理都放在了这里，处理完发送到串口，通过电脑的超级终端可以看到不停变化的6个引脚电压的数据。别忘了一定要清楚中断标志位并置位ADC的连续转换，然后再次启动转换。
    主程序中只要初始化系统函数，还有串口，然后配置好DMA，启动规则转换通道，并启动DMA,然后等在死循环即可。
    下面附上部分代码
------------------------------------------------------------------------------------------- 
 void  Adc_Init(void)
{    
 //先初始化IO口
  RCC->APB2ENR|=1<<2;     //使能PORTA口时钟 
  RCC->APB2ENR|=1<<4;     //使能PORTC口时钟 
 GPIOA->CRL&=0XFFFF0000; //PA0 1 2 3 anolog输入
 GPIOC->CRL&=0XFF00FFFF; //PC4,5 anolog输入
 //通道10/11设置    
 RCC->APB2ENR|=1<<9;     //ADC1时钟使能   
 RCC->APB2RSTR|=1<<9;    //ADC1复位
 RCC->APB2RSTR&=~(1<<9); //复位结束     
 RCC->CFGR&=~(3<<14);    //分频因子清零 
 //SYSCLK/DIV2=12M ADC时钟设置为12M,ADC最大时钟不能超过14M!
 //否则将导致ADC准确度下降! 
 RCC->CFGR|=2<<14;        
 ADC1->CR1&=0XF0FFFF;    //工作模式清零
 ADC1->CR1|=0<<16;       //独立工作模式  
 ADC1->CR1|=1<<8;     //扫描模式
 ADC1->CR2|=1<<1;        //启用连续转换            
 ADC1->CR2|=1<<8;        //启用DMA      
// ADC1->CR2&=~(1<<1);     //单次转换模式
 ADC1->CR2&=~(7<<17);    
 ADC1->CR2|=7<<17;     //软件控制转换  
 ADC1->CR2|=1<<20;       //使用用外部触发(SWSTART)!!! 必须使用一个事件来触发
 ADC1->CR2&=~(1<<11);    //右对齐
 ADC1->SQR1&=~(0XF<<20);
 ADC1->SQR1|=5<<20;      //6个转换在规则序列
 ADC1->SQR3 = 0X00000000;
 ADC1->SQR3|= 0X1EE18820;       
 //设置通道0~3,14,15的采样时间
 ADC1->SMPR1&=0XFFFC0FFF;//通道14，15采样时间清空
 ADC1->SMPR2&=0XFFFFF000;//通道0,1,2,3采样时间清空  
 ADC1->SMPR1|=7<<15;     //通道15  239.5周期,提高采样时间可以提高精确度  
 ADC1->SMPR1|=7<<12;     //通道14  239.5周期,提高采样时间可以提高精确度   
 ADC1->SMPR2|=7<<9;      //通道3  239.5周期,提高采样时间可以提高精确度  
 ADC1->SMPR2|=7<<6;      //通道2  239.5周期,提高采样时间可以提高精确度  
 ADC1->SMPR2|=7<<3;      //通道1  239.5周期,提高采样时间可以提高精确度  
 ADC1->SMPR2|=7<<0;      //通道0  239.5周期,提高采样时间可以提高精确度 
 ADC1->CR2|=1<<0;     //开启AD转换器  
 ADC1->CR2|=1<<3;        //使能复位校准  
 while(ADC1->CR2&1<<3);  //等待校准结束     
    //该位由软件设置并由硬件清除。在校准寄存器被初始化后该位将被清除。    
 ADC1->CR2|=1<<2;        //开启AD校准    
 while(ADC1->CR2&1<<2);  //等待校准结束
 //该位由软件设置以开始校准，并在校准结束时由硬件清除  
}     
 void MYDMA_Config(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
 u32 DR_Base;      //做缓冲用,不知道为什么.非要不可
 RCC->AHBENR|=1<<0;   //开启DMA1时钟
 DR_Base=cpar;
 DMA_CHx->CPAR=DR_Base;    //DMA1 外设地址 
 DMA_CHx->CMAR=(u32)cmar;  //DMA1,存储器地址
 DMA1_MEM_LEN=cndtr;       //保存DMA传输数据量
 DMA_CHx->CNDTR=cndtr;     //DMA1,传输数据量
 DMA_CHx->CCR=0X00000000; //复位
 DMA_CHx->CCR|=1<<1;    //允许传输完中断
 DMA_CHx->CCR|=0<<4;    //从外设读
 DMA_CHx->CCR|=1<<5;    //循环模式
 DMA_CHx->CCR|=0<<6;    //外设地址非增量模式
 DMA_CHx->CCR|=1<<7;    //存储器增量模式
 DMA_CHx->CCR|=1<<8;    //外设数据宽度为16位
 DMA_CHx->CCR|=1<<10;   //存储器数据宽度16位
 DMA_CHx->CCR|=1<<12;   //中等优先级
 DMA_CHx->CCR|=0<<14;   //非存储器到存储器模式
 MY_NVIC_Init(1,3,DMA1_Channel1_IRQChannel ,2);     
} 
//开启一次DMA传输
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{
 DMA_CHx->CCR&=~(1<<0);       //关闭DMA传输 
 DMA_CHx->CNDTR=DMA1_MEM_LEN; //DMA1,传输数据量 
 DMA_CHx->CCR|=1<<0;          //开启DMA传输
}
u16 ADC1_DR, adcx;
void DMAChannel1_IRQHandler(void)
{       
 u16 i;
 u32 sum[6]={0},val=0;  
 LED0 =!LED0;
 ADC1->CR2&=~(1<<1);        //关闭连续转换            
 for( i = 0; i<768 ;i+= 6)
 {
  sum[0] += SendBuff[i];
  sum[1] += SendBuff[i+1];
  sum[2] += SendBuff[i+2];
  sum[3] += SendBuff[i+3];
  sum[4] += SendBuff[i+4];
  sum[5] += SendBuff[i+5];
 }
 for(i = 0;i <6 ;i++)
 {
  val = sum[i]/DMA_COUNT;
  ADC1_DR = sum[i]/DMA_COUNT;
  TEMP=(float)ADC1_DR*(3.3/4096); 
  adcx=TEMP;
  LCD_ShowNum(149,70+i*20,adcx,1,16);//显示电压值
  TEMP-=adcx;
  TEMP*=1000;
  LCD_ShowNum(165,70+i*20,TEMP,3,16);
 }
 delay_ms(200); 
   DMA1->IFCR |= 1<<1;        //清零通道完成中断标志位
 ADC1->CR2|=1<<1;           //启用连续转换            
 ADC1->CR2|=1<<22;          //启动规则转换通道          
} 
--------------------------------------------------------------------------------------
 接下来就要干个大的了~~~ 
