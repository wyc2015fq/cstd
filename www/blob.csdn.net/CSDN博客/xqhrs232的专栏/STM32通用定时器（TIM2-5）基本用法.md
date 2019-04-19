# STM32通用定时器（TIM2-5）基本用法 - xqhrs232的专栏 - CSDN博客
2013年04月26日 15:25:04[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1166
原文地址::[http://blog.sina.com.cn/s/blog_4a3946360100wfjf.html](http://blog.sina.com.cn/s/blog_4a3946360100wfjf.html)
  STM32的定时器是个强大的模块，定时器使用的频率也是很高的，定时器可以做一些基本的定时，还可以做PWM输出或者输入捕获功能。从系统框架图下看，名为TIMx的有八个，其中TIM1和TIM8挂在APB2总线上，而TIM2-TIM7则挂在APB1总线上。其中TIM1&TIM8称为高级控制定时器（advanced control timer).他们所在的APB2总线也比APB1总线要好。APB2可以工作在72MHz下，而APB1最大是36MHz。
![STM32通用定时器（TIM2-5）基本用法](http://s8.sinaimg.cn/middle/4a394636gaac7728ad637&690)
   由上图可知，当APB1 的预分频系数为1 时，这个倍频器不起作用，定时器的时钟频率等于APB1 的频率；当APB1的预分频系数为其它数值(即预分频系数为2、4、8 或16)时，这个倍频器起作用，定时器的时钟频率等于APB1 的频率两倍。也就是，当APB1不分频，TIM3的时钟速度为36MHz，当2分频是，APB1变成18MHz，但是TIM又会倍频，即TIM时钟等于18*2=36MHz。这里我们用向上计数的方式，即TIMx_CNT中的计数值达到TIMx_ARR中的值时，产生中断，TIMx_CNT又从0开始计。
按以下步骤编程：
1.系统初始化，主要初始化时钟等。
2.GPIO初始化，用于LED，有了灯就便于观察了。
3.TIM3的配置。
4.NVIC的配置。
5.编写中断服务函数。
void GPIO_PA_Init()
{//PA8管脚配置
 GPIO_InitTypeDef GPIO_InitStructure;
 GPIO_DeInit(GPIOA);
 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP ;//推挽输出
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);//使能端口时钟A
 GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void TIMER3_Init()
{
 TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
 TIM_DeInit(TIM3);
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
 TIM_TimeBaseStructure.TIM_Period=10000;//ARR的值
 TIM_TimeBaseStructure.TIM_Prescaler=7200-1;
 TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
 TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
 TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
 TIM_Cmd(TIM3, ENABLE); //开启时钟
}
void NVIC_Configuration()
{  
 NVIC_InitTypeDef NVIC_InitStructure;
 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);   // 抢占式优先级别 
 NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQChannel;//指定中断源
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;// 指定响应优先级别1
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 NVIC_Init(&NVIC_InitStructure);
}
int main(void)
{     
 Stm32_Clock_Init(9); //系统时钟设置
 delay_init(72);      //延时初始化
 GPIO_PA_Init();
 TIMER3_Init();
 NVIC_Configuration();
 while(1); 
}
void TIM3_IRQHandler()
{
 if(TIM_GetITStatus(TIM3 , TIM_IT_Update) == SET)
 {
  TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);
  if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_8)) GPIO_ResetBits(GPIOA, GPIO_Pin_8);
  else GPIO_SetBits(GPIOA, GPIO_Pin_8);
 }
}
