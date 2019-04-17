# IAR FOR STM8 学习笔记 固件库 GPIO - DoubleLi - 博客园






经过一番挣扎，还是决定使用官方的固件库了。。

从网上下一个STM8S的固件库，记得是FOR IAR的。

![](http://upload.semidata.info/www.eefocus.com/blog/media/201111/188837.png)

找到里面的IAR模板就可以开始用了。



![](http://upload.semidata.info/www.eefocus.com/blog/media/201111/188838.png)



这些都是直接写好的库函数，可以直接调用，但首先得先读懂，先了解些必备知识。

STM8的寄存器的地址

在STM8中，与某个外围设备有关的寄存器在地址上都是顺序排列的；比如与GPIO有关的寄存器有ODR、IDR、DDR、CR1、CR2，与PA口有关的这五个寄存器就被安排在了0x00 5000~0x00 5004这5个地址空间中，它们有一个0x005000的基地址，分别偏移0，1，2，3，4。 



”stm8s.h"中与GPIO有关的定义，以及固件库中访问外围设备寄存器的方法 

typedef struct GPIO_struct 

{ 

  vu8 ODR; /*!< Output Data Register */ 

  vu8 IDR; /*!< Input Data Register */ 

  vu8 DDR; /*!< Data Direction Register */ 

  vu8 CR1; /*!< Configuration Register 1 */ 

  vu8 CR2; /*!< Configuration Register 2 */ 

} 

GPIO_TypeDef; 



#define GPIOA_BaseAddress       0x5000 

#define GPIOB_BaseAddress       0x5005 

#define GPIOC_BaseAddress       0x500A 

#define GPIOD_BaseAddress       0x500F 

#define GPIOE_BaseAddress       0x5014 

#define GPIOF_BaseAddress       0x5019 



#define GPIOA ((GPIO_TypeDef *) GPIOA_BaseAddress) 

#define GPIOB ((GPIO_TypeDef *) GPIOB_BaseAddress) 

#define GPIOC ((GPIO_TypeDef *) GPIOC_BaseAddress) 

#define GPIOD ((GPIO_TypeDef *) GPIOD_BaseAddress) 

#define GPIOE ((GPIO_TypeDef *) GPIOE_BaseAddress) 

#define GPIOF ((GPIO_TypeDef *) GPIOF_BaseAddress) 



在结构体GPIO_TypeDef中，ODR,IDR,DDR,CR1,CR2的偏移分别是0，1，2，3，4正好与STM8S208RB对这几个安排一致，当我们将0x5000这个地址转换为指向GPIO_TypeDef的指针后， 

我们就可以用类似GPIOA->ODR的方法访问寄存器了。











另外固件库在函数的参数入口都有断言，



![](http://upload.semidata.info/www.eefocus.com/blog/media/201111/188839.png)

判断参数设置是否符合要求，方便调试程序。如果在调试时程序跑到void assert_failed(u8* file, u32 line){}。里面去，可以看看是否是参数设置出现问题









先从对STM8的GPIO操作开始。

 以前没有过，只用过51的。操作管脚，给1就是1给0就是0.虽然简单但是功能不够给力啊。。

先了解下GPIO吧：

General Purpose Input Output (通用输入/输出)简称为GPIO

STM8的每一个GPIO引脚都可以独立的作为输入/输出IO引脚使用，

作为输入IO引脚使用时，每一个IO引脚都可以作为外部中断的触发输入端使用，输入有上拉和悬浮，

输出有模拟开漏和推挽模式。 

DDR用于设置方向，ODR用于输出，IDR用于输入，CR1、CR2用于控制。

具体可参考数据手册。



固件库为我们提供了GPIO_Init这个函数，用于GPIO工作模式的初始化设置，它的原型为 

void GPIO_Init(GPIO_TypeDef* GPIOx,  GPIO_Pin_TypeDef GPIO_Pin, GPIO_Mode_TypeDef GPIO_Mode)； 

第一个参数是上面提到的GPIOA这样的指针，第二、三个参数都是枚举型变量（本质就是一些常数），用来制定要初始化的引脚和工作模式，具体设置可在GPIO.h中找到。 



typedef enum 

{ 

  GPIO_PIN_0    = ((u8)0x01),  /*!< Pin 0 selected */ 

  GPIO_PIN_1    = ((u8)0x02),  /*!< Pin 1 selected */ 

  GPIO_PIN_2    = ((u8)0x04),  /*!< Pin 2 selected */ 

  GPIO_PIN_3    = ((u8)0x08),   /*!< Pin 3 selected */ 

  GPIO_PIN_4    = ((u8)0x10),  /*!< Pin 4 selected */ 

  GPIO_PIN_5    = ((u8)0x20),  /*!< Pin 5 selected */ 

  GPIO_PIN_6    = ((u8)0x40),  /*!< Pin 6 selected */ 

  GPIO_PIN_7    = ((u8)0x80),  /*!< Pin 7 selected */ 

  GPIO_PIN_LNIB = ((u8)0x0F),  /*!< Low nibble pins selected */ 

  GPIO_PIN_HNIB = ((u8)0xF0),  /*!< High nibble pins selected */ 

  GPIO_PIN_ALL  = ((u8)0xFF)   /*!< All pins selected */ 

}GPIO_Pin_TypeDef; 



我们可以如此使用这个函数： 



#define LEDS_PORT (GPIOH) 

#define LED1_PIN  (GPIO_PIN_3) 

#define LED2_PIN  (GPIO_PIN_2) 

#define LED3_PIN  (GPIO_PIN_1) 

#define LED4_PIN  (GPIO_PIN_0) 



#define BUTTON_PORT (GPIOC) 

#define BUTTON_PIN  (GPIO_PIN_0) 



/* Initialize I/Os in Output Mode */ 

GPIO_Init(LEDS_PORT, (LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN), GPIO_MODE_OUT_PP_LOW_FAST); 

/* Initialize I/O in Input Mode with Interrupt */ 

GPIO_Init(BUTTON_PORT, BUTTON_PIN, GPIO_MODE_IN_FL_IT); 



GPIO的操作函数 

void GPIO_Write(GPIO_TypeDef* GPIOx, u8 PortVal);             //写端口，8个脚一起设置 

void GPIO_WriteHigh(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPins); // 将指定脚设置为高 

void GPIO_WriteLow(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPins);  // 讲指定脚设置为低 

void GPIO_WriteReverse(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPins);  // 指定脚取反 

u8 GPIO_ReadInputData(GPIO_TypeDef* GPIOx);  // 读引脚 

u8 GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);  // 读端口，上次锁存到ODR中的数据 

BitStatus GPIO_ReadInputPin(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef GPIO_Pin); // 读制定脚的状态，高电平返回非0，低返回0 



写一个点亮LED的程序：

#include "stm8s.h"

#define LED_PORT (GPIOD) 

#define LED1_PIN  (GPIO_PIN_3) 

#define LED2_PIN  (GPIO_PIN_2) 

#define LED3_PIN  (GPIO_PIN_0) 





#define BUTTON_PORT (GPIOD) 

#define BUTTON_PIN  (GPIO_PIN_7) 

/* Private defines -----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/



void main(void)

{

  /* Infinite loop */

   GPIO_Init(LED_PORT, LED1_PIN,GPIO_MODE_OUT_PP_LOW_SLOW);

   GPIO_WriteHigh(LED_PORT, LED1_PIN);



}

下载测试，没有问题。

from:http://www.eefocus.com/w7838207178/blog/11-11/235052_ff6b7.html









