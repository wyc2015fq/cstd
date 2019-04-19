# stm32芯片smartcard功能开发（接触式IC卡） - xqhrs232的专栏 - CSDN博客
2018年07月03日 21:25:56[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：780
原文地址::[https://blog.csdn.net/weixin_37302264/article/details/71598805](https://blog.csdn.net/weixin_37302264/article/details/71598805)
相关文章
1、STM32_7816智能卡接口（源程序和中文文档）----[https://download.csdn.net/download/wolfliuming/2668000](https://download.csdn.net/download/wolfliuming/2668000)
```
本文基于stm32l0c8t6芯片和cubemx，利用芯片USART1和USART2的smartcard功能，在不经过ST8024的情况下直接与IC卡和ESAM进行通讯处理。不使用ST8024的主要原因是ST8024功耗比较大，在低功耗应用场景下，性价比并不高。
 IC卡卡座与IC卡触点如下所示：
```
- 1
- 2
- 3
![这里写图片描述](https://img-blog.csdn.net/20170510233531274?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzM3MzAyMjY0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
C1：电源电压（VCC）C2：复位RST;C3：时钟CLK;C4未用；C5:GND;C6:编程电压VPP(未用）;C7:输入输出I/O;C8：未用。 
芯片引脚设置及硬件电路连接为：选用PA9和PA3作为IC和ESAM数据传输口，这两个引脚为USART_TX引脚，模式为smartcard模式，按字节实现双向数据传输。RST选用任意GPIO引脚，推挽输出。CLK选用USART1和USART2smartcard模式下的clk引脚，时钟来源是PCLK1和PCLK2,时钟频率需要正确设置，引脚为PA8和PA4。此外，电源和地均需正确连接。 
硬件连接无误后，程序设计如下： 
1、时钟设置： 
IC卡时钟频率需设置为3.2MHz或3.6MHz，4MHz下通讯异常。波特率为：时钟频率/3720。 
以下为USART1和USART2初始化代码，其中，字节长度为9bits，8位数据位，1位偶校验位，1.5位停止位。主频率是32MHz，为了得到3.2MHz的clk，进行10分频，hsmartcard1.Init.Prescaler = 0x05。其他设置如下：
```
static void MX_USART1_SMARTCARD_Init(void)
{
  hsmartcard1.Instance = USART1;
  hsmartcard1.Init.BaudRate = HAL_RCC_GetPCLK2Freq()/3720;
  hsmartcard1.Init.WordLength = SMARTCARD_WORDLENGTH_9B;
  hsmartcard1.Init.StopBits = SMARTCARD_STOPBITS_1_5;
  hsmartcard1.Init.Mode = SMARTCARD_MODE_TX_RX;
    hsmartcard1.Init.Parity = UART_PARITY_EVEN;
  hsmartcard1.Init.OneBitSampling = SMARTCARD_ONE_BIT_SAMPLE_ENABLE;
  hsmartcard1.Init.Prescaler = 0x05;
  hsmartcard1.Init.GuardTime = 16;
  hsmartcard1.Init.NACKEnable = SMARTCARD_NACK_ENABLE;
  hsmartcard1.Init.TimeOutEnable = SMARTCARD_TIMEOUT_ENABLE;
  //hsmartcard1.Init.BlockLength = 0;
  hsmartcard1.Init.AutoRetryCount = 3;
  hsmartcard1.AdvancedInit.AdvFeatureInit = SMARTCARD_ADVFEATURE_NO_INIT;
  if (HAL_SMARTCARD_Init(&hsmartcard1) != HAL_OK)
  {
    Error_Handler();
  }
   HAL_SMARTCARD_DeInit(&hsmartcard1);
}
/* USART2 init function */
static void MX_USART2_SMARTCARD_Init(void)
{
  hsmartcard2.Instance = USART2;
  hsmartcard2.Init.BaudRate = HAL_RCC_GetPCLK1Freq()/3720;
  hsmartcard2.Init.WordLength = SMARTCARD_WORDLENGTH_9B;
  hsmartcard2.Init.StopBits = SMARTCARD_STOPBITS_1_5;
  hsmartcard2.Init.Mode = SMARTCARD_MODE_TX_RX;
    hsmartcard2.Init.Parity = UART_PARITY_EVEN;
  hsmartcard2.Init.OneBitSampling = SMARTCARD_ONE_BIT_SAMPLE_DISABLE;
  hsmartcard2.Init.Prescaler = 0x05;
  hsmartcard2.Init.GuardTime = 16;
  hsmartcard2.Init.NACKEnable = SMARTCARD_NACK_ENABLE;
  hsmartcard2.Init.TimeOutEnable = SMARTCARD_TIMEOUT_ENABLE;
  //hsmartcard2.Init.BlockLength = 1;
  hsmartcard2.Init.AutoRetryCount = 3;
  hsmartcard2.AdvancedInit.AdvFeatureInit = SMARTCARD_ADVFEATURE_NO_INIT;
  if (HAL_SMARTCARD_Init(&hsmartcard2) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_SMARTCARD_DeInit(&hsmartcard2);
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
其中，HAL_SMARTCARD_Init函数中初始化了smartcard模式下寄存器等的操作，cubemx会自动生成相关代码。 
2、GPIO相关设置如下：
```
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
   /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_5|GPIO_PIN_12|GPIO_PIN_10, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11|GPIO_PIN_3, GPIO_PIN_SET);
  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
  /*Configure GPIO pins : PA3 PA5 PA11 PA12 PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  /*Configure GPIO pins : PB12 PB4 PB5 PB6 
                           PB7 PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6 
                          |GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    __HAL_RCC_USART1_CLK_ENABLE();
    /**USART1 GPIO Configuration    
    PA8     ------> USART1_CK
    PA9     ------> USART1_TX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    __HAL_RCC_USART2_CLK_ENABLE();
    /**USART2 GPIO Configuration    
    PA2     ------> USART2_TX
    PA4     ------> USART2_CK 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
3、插卡检测 
笔者在程序中设置的IC卡检测模式为每分钟执行一次IC卡检测函数，检测是否有卡插入，也可以设置为有卡插入则进入中断模式来完成IC卡的检测，功耗更低。定时器函数在这里不再列出，将定时器设为一分钟进入一次卡检测函数，有卡插入则开始执行卡的相关操作。 
4、IC卡的复位应答 
IC卡插入后，需要先对卡进行复位操作，接收到正确的复位应答信号，则卡复位成功。复位应答信号错误或未收到复位应答信号，则关闭IC卡电源。 
卡复位函数及接收复位应答函数：
```php
BOOLEAN Card_Reset()
{
  if(gIC_flg&IC_Choose)
  {  
    if( Recv_ATR())         //RECEIVE ANSWER TO RESET
        {
        return TRUE;
        }
    else
        {
      return FALSE;
        }
  }
  else
  {                        //RESET ESAM
    if( Recv_ATR())         //RECEIVE ANSWER TO RESET
        {
        return TRUE;
        }
    else
        {
      return FALSE;
        }
  }
}
BOOLEAN Recv_ATR()
{
      INT8U j;
    for(j=0; j<13; j++)
    {
     RecvBuf[j] = 0;
    }
    if(gIC_flg&IC_Choose)
  { 
      IC_Poweron();
     HAL_SMARTCARD_Receive(&hsmartcard1,RecvBuf,13,1500);
    if(RecvBuf[0] == 0x3B) // TS=3B  T0=0x69  1(T0)+2(TB1,TC1)+9(T1--T9)
    {   
             Card_errInfo = 0;
    }
    else 
    {
      Card_errInfo = 0xff;
    }
    }
    else
    { 
        ESAM_Poweron();
        if( RecvBuf[0] == 0x3B)  // TS=3B T0=0x69 
    {
     Card_errInfo = 0;
    }
    else 
    {
      Card_errInfo = 0xff;
    }
    }
  if(Card_errInfo == 0)
    return TRUE;
  else
    return FALSE;
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
卡复位操作函数：
```
void IC_Poweron()
{
  OPEN_IC_SET();
  RST_IC_CLR();   //rst引脚清零
  Delay10etu();
  OPEN_IC_CLR();  //power on  
    Delay10etu();
  HAL_SMARTCARD_Init(&hsmartcard1);   //开启时钟
  Delay10etu();    //delay 3 etu
  Delay10etu();
  Delay10etu();
  RST_IC_SET();    //rst引脚置1
  Delay10etu();   
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
卡复位应答正确返回数字第一个字节为3B，则复位正确。 
复位成功后，即可根据通讯协议进行卡的操作。
