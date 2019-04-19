# STM32M CUBE实现printf打印调试信息以及实现单字节接收 - xqhrs232的专栏 - CSDN博客
2018年02月09日 17:31:29[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：628
原文地址::[https://www.2cto.com/kf/201506/412341.html](https://www.2cto.com/kf/201506/412341.html)
相关文章
1、使用stm32cubemx开发四：串口标准化输出----[http://blog.csdn.net/u014256685/article/details/45893097](http://blog.csdn.net/u014256685/article/details/45893097)
2、STM32Cube Printf uart重定向----[http://blog.csdn.net/u014298427/article/details/50493905](http://blog.csdn.net/u014298427/article/details/50493905)
在写单片机程序时我们一般喜欢使用printf来通过串口打印调试信息，但这个函数是不可以直接使用的，必须做点对库函数的改动。
STM32M CUBE是ST官方提供的库以及初始化工具，很好很强大，但是在UART方面值提供了如下函数：
[?](https://www.2cto.com/kf/201506/412341.html#)
```
```java
HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);
```
```java
HAL_StatusTypeDef HAL_UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);
```
```java
HAL_StatusTypeDef HAL_UART_Transmit_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
```
```java
HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
```
```java
HAL_StatusTypeDef HAL_UART_Transmit_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
```
```java
HAL_StatusTypeDef HAL_UART_Receive_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
```
```
分别实现普通收发，中断收发，DMA收发，问题是所有函数要求发送和接收的buf必须要事先知道长度，也没有提供对单字节的收发，无法直接实现printf以及单字节接收。
其实要实现这些还是很简单的，首先是实现printf
在main.c 添加如下信息
[?](https://www.2cto.com/kf/201506/412341.html#)
```
```java
#include <stdio.h>
```
```java
#ifdef __GNUC__
```
```java
```
```java
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
```
```java
```
```java
set to 'Yes') calls __io_putchar() */
```
```java
```
```java
#define PUTCHAR_PROTOTYPE
```
```java
int
```
```java
__io_putchar(
```
```java
int
```
```java
ch)
```
```java
#
```
```java
else
```
```java
```
```java
#define PUTCHAR_PROTOTYPE
```
```java
int
```
```java
fputc(
```
```java
int
```
```java
ch, FILE *f)
```
```java
#endif
```
```java
/* __GNUC__ */
```
```java
/**
```
```java
```
```java
* @brief  Retargets the C library printf function to the USART.
```
```java
```
```java
* @param  None
```
```java
```
```java
* @retval None
```
```java
```
```java
*/
```
```java
PUTCHAR_PROTOTYPE
```
```java
{
```
```java
```
```java
/* Place your implementation of fputc here */
```
```java
```
```java
/* e.g. write a character to the USART */
```
```java
```
```java
huart1.Instance->DR = (uint8_t) ch;
```
```java
```
```java
/* Loop until the end of transmission */
```
```java
```
```java
while
```
```java
(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET){}
```
```java
```
```java
return
```
```java
ch;
```
```java
}</stdio.h>
```
```
在这里我们实现了单字节发送函数，注意实现这种发送方式的前提是单字节发送的相关中断不能打开，否则会进入无限等待，做好之后就可以使用printf了。
[?](https://www.2cto.com/kf/201506/412341.html#)
```
```java
void
```
```java
LED_Task2(
```
```java
void
```
```java
const
```
```java
* argument)
```
```java
{
```
```java
```
```java
while
```
```java
(
```
```java
1
```
```java
)
```
```java
```
```java
{
```
```java
```
```java
HAL_GPIO_TogglePin(GPIOG,GPIO_PIN_14);
```
```java
```
```java
printf(LED_Task2
```
```java
);
```
```java
```
```java
osDelay(
```
```java
2000
```
```java
);
```
```java
```
```java
}
```
```java
}
```
```
然后是中断单字节接收，修改中断接收函数如下：
[?](https://www.2cto.com/kf/201506/412341.html#)
```
```java
void
```
```java
USART1_IRQHandler(
```
```java
void
```
```java
)
```
```java
{
```
```java
```
```java
/* USER CODE BEGIN USART1_IRQn 0 */
```
```java
```
```java
static
```
```java
int
```
```java
count=
```
```java
0
```
```java
;
```
```java
```
```java
/* USER CODE END USART1_IRQn 0 */
```
```java
//  HAL_UART_IRQHandler(&huart1);
```
```java
```
```java
/* USER CODE BEGIN USART1_IRQn 1 */
```
```java
```
```java
if
```
```java
(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE) == SET)
```
```java
//有接受到字符串
```
```java
```
```java
{
```
```java
```
```java
uart_recbuf[count++] = (uint8_t)(huart1.Instance->DR & (uint8_t)
```
```java
0x00FF
```
```java
);
```
```java
//接收
```
```java
```
```java
huart1.Instance->DR = uart_recbuf[count-
```
```java
1
```
```java
];
```
```java
//发送接收的数据
```
```java
```
```java
if
```
```java
(count ==
```
```java
100
```
```java
) count =
```
```java
0
```
```java
;
```
```java
```
```java
}
```
```java
```
```java
/* USER CODE END USART1_IRQn 1 */
```
```java
}
```
```
注意使用cube生成的代码默认是没有打开接收中断使能的，要在这里打开：
[?](https://www.2cto.com/kf/201506/412341.html#)
```
```java
void
```
```java
HAL_UART_MspInit(UART_HandleTypeDef* huart)
```
```java
{
```
```java
```
```java
GPIO_InitTypeDef GPIO_InitStruct;
```
```java
```
```java
if
```
```java
(huart->Instance==USART1)
```
```java
```
```java
{
```
```java
```
```java
/* USER CODE BEGIN USART1_MspInit 0 */
```
```java
```
```java
/* USER CODE END USART1_MspInit 0 */
```
```java
```
```java
/* Peripheral clock enable */
```
```java
```
```java
__USART1_CLK_ENABLE();
```
```java
```
```java
```
```java
/**USART1 GPIO Configuration
```
```java
```
```java
PA9     ------> USART1_TX
```
```java
```
```java
PA10     ------> USART1_RX
```
```java
```
```java
*/
```
```java
```
```java
GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
```
```java
```
```java
GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
```
```java
```
```java
GPIO_InitStruct.Pull = GPIO_PULLUP;
```
```java
```
```java
GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
```
```java
```
```java
GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
```
```java
```
```java
HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
```
```java
```
```java
/* Peripheral interrupt init*/
```
```java
```
```java
HAL_NVIC_SetPriority(USART1_IRQn,
```
```java
5
```
```java
,
```
```java
0
```
```java
);
```
```java
```
```java
HAL_NVIC_EnableIRQ(USART1_IRQn);
```
```java
```
```java
/* USER CODE BEGIN USART1_MspInit 1 */
```
```java
```
```java
huart->Instance->CR1 |= USART_CR1_RXNEIE;
```
```java
//使能接收中断
```
```java
```
```java
/* USER CODE END USART1_MspInit 1 */
```
```java
```
```java
}
```
```java
}
```
```
这样就实现了这些功能，但是之前cube的默认功能，中断收发已经不能用了。
