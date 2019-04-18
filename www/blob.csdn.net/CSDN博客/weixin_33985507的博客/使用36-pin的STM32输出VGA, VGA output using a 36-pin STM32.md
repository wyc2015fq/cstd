# 使用36-pin的STM32输出VGA, VGA output using a 36-pin STM32 - weixin_33985507的博客 - CSDN博客
2015年08月26日 17:46:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：12
# [使用36-pin的STM32输出VGA](http://jeremybai.github.io/blog/2014/04/20/VGA-output-using-arm/)
手头上有个项目需要通过单片机来控制将图像显示在LCD上，在网上搜了一阵子，发现都是使用的FPGA做的，
开始自己对FPGA不是很熟，一直在用的也是ARM系列的，终于让我找到一份至少现在看起来还是含金量蛮高的资料，
因为是英文的，这边先将它翻译一下（[**原文链接**](http://www.artekit.eu/vga-output-using-a-36-pin-stm32/)）。
　　想到之前玩的一些老的视频游戏和街机游戏（很早之前，大概70/80年代左右），脑子里浮现出一个想法：
如果在今天，我们是不是可以使用成本比较低的微控制器来实现之前玩玩的那些游戏呢？
这些微控制器设计的初衷并不是用来干这些事情的，所以问题也就产生了：
如何在使用很少或者不使用外部组件的情况下向显示器输出视频信号呢？
　　我们选择了36-pin, 72 MHz的STM32 (STM32F103T8U6),
足够用于产生黑白视频信号和点信号，同时还使用了一些定时器和SPI（在这种方式下更新帧缓冲是自动完成的），
在400*200分辨率的显示器上VGA输出视频信号看起来还是比较可观的。
### 使用的材料：
　　1 STM32F103T8U6开发板一块（或者同类型的开发板）。我们使用的是AK-STM32-LKIT。
　　2 VGA母口一个（DB15）
![](https://images0.cnblogs.com/blog2015/268182/201508/261658112034654.gif)
虽然帧缓冲区是400*200的，但是输出的分辨率却是800*600（56hz刷新频率），
我们采用把横着点绘制两次，竖着的点绘制三次的方法来达到扩展分辨率的目的。
我们选择800×600 @ 56Hz的原因是因为像素时钟；
输出分辨率使用36MHz像素时钟，周期是72MHz的倍数（STM32的频率），
因为我们需要使用SPI产生像素信号，可以把STM32的频率经过SPI预分频得到18MHz的像素时钟，
然后将每一个像素点绘制两次，具体方法是当在水平方向800像素点时输出一个信号像素，
SPI 的 MOSI信号保持低电平或者高电平两倍的时间（相比于之前绘制一个点的时间）。
帧缓冲区是一个52×200字节的数组。每一行有50*8=400个像素（每一个bit是一个像素），
剩下的两个字节（52-50）模拟每一行的消隐间隔。
```
#define VID_VSIZE 200
#define VID_HSIZE 50
 
__align(4) u8 fb[VID_VSIZE][VID_HSIZE+2];
```
在这一块ram中写入的数据都会被输出到屏幕，DMA被设置为自动从数据缓冲区读取数据并且输出到SPI的MOSI引脚。
### 水平同步 　　
水平同步信号（ horizontal synchronism signal）和后延时间（back porch time）由TIM1定时器产生的通道1和2产生，TIM1定时器产生的通道1连接到PA8。
![](https://images0.cnblogs.com/blog2015/268182/201508/261653020008923.png)
H-SYNC也就是TIM1定时器的通道1将会产生水平同步信号给显示器。
H-BACKPORCH也就是TIM1定时器的通道2,计算水平同步时间的和以及后延时间，
这个定时器产生一个中断用于触发DMA开始通过SPI发送像素的请求。
帧缓冲里面的每一行都会重复这样的过程，
## 垂直同步
TIM2定时器用于产生垂直同步信号，但是实在从机模式下。
TIM2计算主机（TIM1）产生的H-SYNC脉冲数。
![](https://images0.cnblogs.com/blog2015/268182/201508/261653186725752.png)
TIM2的通道2通过PA1输出V-SYNC信号。
TIM2的通道3将会触发一个中断当定时器的计数器达到V-SYNC的和垂直后沿时间。
这个中断会设置一个变量表明正在扫描一个有效帧并且DMA可以开始发送像素到屏幕了。
![](https://images0.cnblogs.com/blog2015/268182/201508/261653404374767.png)
## 像素发生器
像素由SPI的MOSI（PA7）产生。
定时器TIM1的通道2产生一个中断用于使能DMA TX请求向SPI发送数据。
DMA将会从帧缓冲区读取一行并且将数据放到SPI的DR寄存器。
DMA被设置用来在一行信号被发送之后产生一个中断，行号是递增的。
因为我们将每一行发送了三次，我们在中断中将计数加1。
当三行数据被发送出去，我们将DMA指针指向下一行的帧缓冲。
当所有的行被发送出去，DMA被禁止直到下一个有效单的帧中断发生（TIM2通道3）。
## 连接
你只需要几根杜邦线和一个母口的VGA接口就可以完成这项工作了。
VGA标准说输出信号应该在0.7V到1V之间，所以你需要在线上进行分压（串联68欧姆和33欧姆的电阻要比47pF和68欧姆的并联），
我们已经测试了一系列的LCD寄存器在没有分压的情况下，工作起来还行。
引脚发参考AK-STM32-LKIT扩展板接插件，引脚命名对于所有的STM32都有效。
根据你自己所用的STM32的手册确定使用的引脚是否一致。
`我们使用绿色（VGA的引脚2）来模拟旧的那种显示效果，你可以使用其他的色彩方案使用`
`RED/GREEN/BLUE DB15引脚，可以创建8种颜色组合。`
![](https://images0.cnblogs.com/blog2015/268182/201508/261654055625343.png)
![](https://images0.cnblogs.com/blog2015/268182/201508/261654190153654.png)
## 结论
　　我们使用了一个低成本微处理器作为VGA控制器，实现这个目的的方法很多，但是这种方法不需要额外的组件除了一个VGA接口。
　　如果你使用的是更高级的STM32，你可以试着使用扩大缓冲区而且在DMA被禁止的时候写帧缓冲区以避免数据被割裂。
　　你可以下载 [源码](http://www.artekit.eu/resources/blog/artekit_vga.zip)，在里面你可以找到画线描点等等的工具库。
　　下一篇日志中会通过VGA样例来实现视频游戏。 
## [Space Invaders for STM32](http://www.artekit.eu/space-invaders-for-stm32/)
Using the previous [VGA output project](http://www.artekit.eu/vga-output-using-a-36-pin-stm32/), we have created this Space Invaders version for STM32.
The source code is kept as a separated project, but uses the same engine of the VGA output project.
[Click here to download the source code.](http://www.artekit.eu/resources/blog/artekit_space_invaders.zip)
### Connections
We have added three push-buttons to the original connections.
These push-buttons serve as a joystick.
Here are the connections for the push-buttons and the VGA connector:
![](https://images0.cnblogs.com/blog2015/268182/201508/261711072342503.png)
THANK YOU for sharing you project with us.
Your project was a starting point to our project.
I used your logic, rewritten whole code, expanded to 2 output SPI, 
sinhronized them in slave mode using two more timers.
One to be same as TIM1, to trigger 10Mhz timer for SPI CLK.
I did additional logic with input part and viola an adapter on a MCU.
[https://www.youtube.com/watch?v=HGje7a6_1Jk](https://www.youtube.com/watch?v=HGje7a6_1Jk)
Best regards from Slovenia
```
1 #ifndef    __VIDEO_H
 2 #define    __VIDEO_H
 3 
 4 #include "gdptypes.h"
 5 
 6 #define    VID_HSIZE        50        // Horizontal resolution (in bytes)
 7 #define    VID_VSIZE        200        // Vertical resolution (in lines)
 8 
 9 #define    VID_PIXELS_X    (VID_HSIZE * 8)
10 #define    VID_PIXELS_Y    VID_VSIZE
11 #define    VID_PIXELS_XR   (VID_PIXELS_X + 16)
12 #define    VID_HSIZE_R     (VID_HSIZE + 2)
13 
14 //    Function definitions
15 
16 void    vidInit(void);
17 void    vidClearScreen(void);
18 
19 #endif    // __VIDEO_H
```
```
1 /***************************************************************************
  2  * STM32 VGA demo
  3  * Copyright (C) 2012 Artekit Italy
  4  * http://www.artekit.eu
  5  * Written by Ruben H. Meleca
  6  
  7 ### video.c
  8  
  9 #   This program is free software; you can redistribute it and/or modify
 10 #   it under the terms of the GNU General Public License as published by
 11 #   the Free Software Foundation; either version 2 of the License, or
 12 #   (at your option) any later version.
 13 #
 14 #   This program is distributed in the hope that it will be useful,
 15 #   but WITHOUT ANY WARRANTY; without even the implied warranty of
 16 #   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 17 #   GNU General Public License for more details.
 18 #
 19 #   You should have received a copy of the GNU General Public License
 20 #   along with this program; if not, write to the Free Software
 21 #   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 22 
 23 ***************************************************************************/
 24 
 25 #include "stm32f10x.h"
 26 #include "video.h"
 27 
 28 #define VTOTAL    52                            /* Total bytes to send through SPI */
 29 __align(4) u8 fb[VID_VSIZE][VID_HSIZE+2];    /* Frame buffer */
 30 static volatile u16 vline = 0;                /* The current line being drawn */
 31 static volatile u32 vflag = 0;                /* When 1, the SPI DMA request can draw on the screen */
 32 static volatile u32 vdraw = 0;                /* Used to increment vline every 3 drawn lines */ 
 33 
 34 void TIMER_Configuration(void)
 35 {
 36     GPIO_InitTypeDef GPIO_InitStructure;
 37     NVIC_InitTypeDef nvic;
 38     TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
 39     TIM_OCInitTypeDef TIM_OCInitStructure;
 40     u32 TimerPeriod = 0;
 41     u16 Channel1Pulse = 0, Channel2Pulse = 0, Channel3Pulse = 0;
 42     
 43     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_8;
 44     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
 45     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
 46     GPIO_Init(GPIOA, &GPIO_InitStructure);
 47     
 48     /*
 49         SVGA 800x600 @ 56 Hz
 50         Vertical refresh    35.15625 kHz
 51         Pixel freq.            36.0 MHz
 52         
 53         1 system tick @ **72Mhz** = 0,0138 us
 54     */
 55     
 56     /*
 57         Horizontal timing
 58         -----------------
 59         
 60         Timer 1 period = 35156 Hz
 61         
 62         Timer 1 channel 1 generates a 2 us pulse for HSYNC each 28.4 us. : D = ( 2 / 28.4 )
 63         28.4 us    = Visible area + Front porch + Sync pulse + Back porch.
 64         HSYNC is 2 us long, so the math to do is:
 65         2us / 0,0138us = 144 system ticks.
 66         
 67         Timer 1 channel 2 generates a pulse equal to HSYNC + back porch = 2 + 3.55 = 5.55
 68         This interrupt will fire the DMA request to draw on the screen if vflag == 1.
 69         Since firing the DMA takes more or less 800ns, we'll add some extra time.
 70         The math for HSYNC + back porch is:
 71         (2us + 3,55us - dma) / 0,0138us = +-350 system ticks
 72     
 73         Horizontal timing info 800 + 24+72+128 = 1024
 74         --------------------------------------------
 75 
 76                         Dots       us
 77         --------------------------------------------        
 78         Visible area    800        22.222222222222
 79         Front porch      24        0.66666666666667
 80         Sync pulse       72        2
 81         Back porch      128        3.5555555555556
 82         Whole line     1024        28.444444444444
 83     
 84     */
 85 
 86     TimerPeriod = 2048;
 87     Channel1Pulse = 144;        /* HSYNC */
 88     Channel2Pulse = 352;        /* HSYNC + BACK PORCH */
 89     
 90     TIM_TimeBaseStructure.TIM_Prescaler = 0;
 91     TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
 92     TIM_TimeBaseStructure.TIM_Period = TimerPeriod; // 2048
 93     TIM_TimeBaseStructure.TIM_ClockDivision = 0;
 94     TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
 95     TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
 96 
 97     TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
 98     TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
 99     TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
100     TIM_OCInitStructure.TIM_Pulse = Channel1Pulse; // 144
101     TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
102     TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
103     TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
104     TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Set;
105 
106     TIM_OC1Init(TIM1, &TIM_OCInitStructure); **// PA8**107     
108     TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Inactive; **// No Output**
109     TIM_OCInitStructure.TIM_Pulse = Channel2Pulse; // 352
110     TIM_OC2Init(TIM1, &TIM_OCInitStructure);
111 
112     /* TIM1 counter enable and output enable */
113     TIM_CtrlPWMOutputs(TIM1, ENABLE);
114 
**115     /* Select TIM1 as Master */**
116     TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable);
117     TIM_SelectOutputTrigger(TIM1, **TIM_TRGOSource_Update**);
118     
119     /*
120         Vertical timing
121         ---------------
122         
123         Polarity of vertical sync pulse is positive.
124 
125                         Lines = 600 + 1+2+22 = 625
126         ------------------------------
127         Visible area    600
128         Front porch     1
129         Sync pulse      2
130         Back porch      22
131         Whole frame     625
132         
133     */
134 
135     /* VSYNC (TIM2_CH2) and VSYNC_BACKPORCH (TIM2_CH3) */
136     /* Channel 2 and 3 Configuration in PWM mode */
137     TIM_SelectSlaveMode(TIM2, **TIM_SlaveMode_Gated**);
138     TIM_SelectInputTrigger(TIM2, **TIM_TS_ITR0**); **// TIM1**139     
140     TimerPeriod = 625;        /* Vertical lines */
141     Channel2Pulse = 2;        /* Sync pulse */
142     Channel3Pulse = 24;        /* Sync pulse + Back porch = 2 lines + 24 lines */
143     TIM_TimeBaseStructure.TIM_Prescaler = 0;
144     TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
145     TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
146     TIM_TimeBaseStructure.TIM_ClockDivision = 0;
147     TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
148 
149     TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
150 
151     TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
152     TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
153     TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
154     TIM_OCInitStructure.TIM_Pulse = Channel2Pulse;
155     TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
156     TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
157     TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
158     TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Set;
159     TIM_OC2Init(TIM2, &TIM_OCInitStructure);
160     
161     TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Inactive;
162     TIM_OCInitStructure.TIM_Pulse = Channel3Pulse;
163     TIM_OC3Init(TIM2, &TIM_OCInitStructure);
164 
165     /*    TIM2 counter enable and output enable */
166     TIM_CtrlPWMOutputs(TIM2, ENABLE);
167 
**168     /* Interrupt TIM2 --  Generate IRQ to restart the Frame Buffer Count */**
169     nvic.NVIC_IRQChannel = TIM2_IRQn;
170     nvic.NVIC_IRQChannelPreemptionPriority = 1;
171     nvic.NVIC_IRQChannelSubPriority = 0;
172     nvic.NVIC_IRQChannelCmd = ENABLE;
173 
174     NVIC_Init(&nvic);
175     TIM_ITConfig(TIM2, TIM_IT_CC3, ENABLE);
176 
**177     /* Interrupt TIM1 -- Generate IRQ to start DMA1 Channel 3 */**
178     nvic.NVIC_IRQChannel = TIM1_CC_IRQn;
179     nvic.NVIC_IRQChannelPreemptionPriority = 1;
180     nvic.NVIC_IRQChannelSubPriority = 0;
181     nvic.NVIC_IRQChannelCmd = ENABLE;
182 
183     NVIC_Init(&nvic);
184     TIM_ITConfig(TIM1, TIM_IT_CC2, ENABLE);
185     
186     TIM_Cmd(TIM2, ENABLE);
187     TIM_Cmd(TIM1, ENABLE);
188 }
189 
190 void SPI_Configuration(void)
191 {
192     NVIC_InitTypeDef nvic;
193     SPI_InitTypeDef SPI_InitStructure;
194     DMA_InitTypeDef    DMA_InitStructure;
195     GPIO_InitTypeDef GPIO_InitStructure;
196     
197     GPIO_InitStructure.GPIO_Pin =**  GPIO_Pin_7**;
198     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
199     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
200     GPIO_Init(GPIOA, &GPIO_InitStructure);
201 
202     SPI_Cmd(SPI1, DISABLE);
203     DMA_DeInit(DMA1_Channel3);
204 
205     DMA_InitStructure.DMA_PeripheralBaseAddr = **(uint32_t)&SPI1->DR;**206     DMA_InitStructure.DMA_MemoryBaseAddr = **(u32) &fb[0][0];**207     DMA_InitStructure.DMA_DIR =** DMA_DIR_PeripheralDST**;
208     DMA_InitStructure.DMA_BufferSize = VTOTAL;
209     DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
210     DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
211     DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
212     DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
213     DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
214     DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
215     DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
216     DMA_Init(DMA1_Channel3, &DMA_InitStructure);
217 
218     SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
219     SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
220     SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
221     SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;  **// Mode : (0,1)**222     SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
223     SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
224     SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;  **// 72/4=18MHz**225     SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
226     SPI_InitStructure.SPI_CRCPolynomial = 7;
227     SPI_Init(SPI1, &SPI_InitStructure);
228     
229     SPI_CalculateCRC(SPI1, DISABLE);
230     SPI_Cmd(SPI1, ENABLE);
231     
**232     SPI1->CR2 |= SPI_I2S_DMAReq_Tx;**233     
234     nvic.NVIC_IRQChannel = DMA1_Channel3_IRQn;
235     nvic.NVIC_IRQChannelPreemptionPriority = 0;
236     nvic.NVIC_IRQChannelSubPriority = 0;
237     nvic.NVIC_IRQChannelCmd = ENABLE;
238     NVIC_Init(&nvic);
239     
240     DMA1_Channel3->CCR &= ~1;
241     DMA1_Channel3->CNDTR = VTOTAL;
242     DMA1_Channel3->CMAR = (u32) &fb[0][0];
243     
244     DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);
245 }
246 
247 //*****************************************************************************
248 //    This irq is generated at the end of the horizontal back porch.
249 //    Test if inside a valid vertical start frame (vflag variable), 
250 //**    and start the DMA to output a single frame buffer line through the SPI device.**251 //*****************************************************************************
252 __irq void TIM1_CC_IRQHandler(void)
253 {
254     if (vflag)
255     {
256         DMA1_Channel3->CCR = 0x93;
257     }
258     TIM1->SR = 0xFFFB; //~TIM_IT_CC2;
259 }
260 
261 //*****************************************************************************
262 //    This irq is generated at the end of the vertical back porch.
263 //**    Sets the 'vflag' variable to 1 (valid vertical frame).**264 //*****************************************************************************
265 __irq void TIM2_IRQHandler(void)
266 {
267     vflag = 1;
268     TIM2->SR = 0xFFF7; //~TIM_IT_CC3;
269 }
270 
271 //*****************************************************************************
272 //    This interrupt is generated at the end of every line.
273 //    It will increment the line number and set the corresponding line pointer
274 //    in the DMA register.
275 //*****************************************************************************
276 __irq void DMA1_Channel3_IRQHandler(void)
277 {    
278     DMA1->IFCR = DMA1_IT_TC3;
279     DMA1_Channel3->CCR = 0x92;
280     DMA1_Channel3->CNDTR = VTOTAL;
281     
282     vdraw++;
283     
284     if (vdraw == 3)
285     {
286         vdraw = 0;
287 
288         vline++;
289         
290         if (vline == VID_VSIZE)
291         {
292             vdraw = vline = vflag = 0;
293             DMA1_Channel3->CMAR = (u32) &fb[0][0];
294         } else {
295             DMA1_Channel3->CMAR += VTOTAL;
296         }
297     }
298 }
299 
300 void vidClearScreen(void)
301 {
302     u16 x, y;
303 
304     for (y = 0; y < VID_VSIZE; y++)
305     {
306         for (x = 0; x < VTOTAL; x++)
307         {
308             fb[y][x] = 0;
309         }
310     }
311 }
312 
313 void vidInit(void)
314 {
315     SPI_Configuration();
316     TIMER_Configuration();
317     vidClearScreen();
318 }
```
