# 关于STM32F0管脚定义安排的两三个话题----本人备注 - xqhrs232的专栏 - CSDN博客
2019年01月10日 15:14:33[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：108
原文地址::[http://www.51hei.com/bbs/dpj-40985-1.html](http://www.51hei.com/bbs/dpj-40985-1.html)
对于STM32系列芯片的管脚定义，因为绝大部分管脚除了默认的GPIO功能外，还可以有复用或额外功能，搞得很复杂。初次使用可能让人犯晕。这里以STM32F0系列为例，来聊聊管脚相关的话题。
STM32F0芯片管脚除了个别电源或特定功能脚【比如复位脚、引导控制脚】外，芯片复位后，其它管脚的默认功能就是GPIO，同时各GPIO口往往具有其它复用功能或者额外功能。关于芯片管脚的说明在芯片的数据手册里有详细介绍。
**一、**芯片管脚默认功能、复用功能、额外功能界定话题
在芯片的数据手册里都有关于各系列芯片的管脚描述表。如下图表格可以看出，各个管脚有分3个功能块，一个复位后默认功能【function after reset】，一块复用功能【alternate functions】，还一块就是额外功能【additional functions】。额外功能主要集中在做AD输入脚、外部晶体脚及其它个别额外功能脚。![](http://c.51hei.com/a/a/c/51271948070747.jpg)
FIG1. 管脚描述表
默认功能没啥好说的，就是芯片复位后的管脚功能或状态。复用功能是通过配置GPIOx_AFR寄存器后选择的功能。额外功能就是配置相关外设寄存器来选择的功能，比方配置ADC使能某些通道、开启USB应用等来使能相应管脚的额外功能。 ![](http://c.51hei.com/a/a/c/51271948053904.jpg)
FIG2 管脚复用功能对应表
**二、**有些管脚的定义安排貌似有冲突或重复的话题。以下图中的I2C1管脚说明为例。
![](http://c.51hei.com/a/a/c/5127194803040.jpg)
从上图可以看出，对于48脚或64脚的32F030芯片，I2C1复用功能脚似乎可位于两个位置，咋看上去好像标错了。让人疑惑到底使用哪对脚。
那我们再看看有关复用功能寄存器对应表，结合上面FIG2 管脚复用功能对应表的PB6、PB7、PB8、PB9所对应的AF1那一栏，其实那4个脚可以有4种组合来实现I2C1的复用功能。可以分别是PB6\PB7,PB8\PB9,PB6\PB9,PB7\PB8四组中其中之一来做I2C1的通信功能脚。
我们可以用STM32CUBEMX工具来验证测试下上面的理解。下面只列出了PB6\PB9,PB8\PB7的配置使用过程。
![](http://c.51hei.com/a/a/c/51271948018824.jpg)
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
GPIO_InitTypeDef GPIO_InitStruct;
if(hi2c->Instance==I2C1)
{
/* USER CODE BEGIN I2C1_MspInit 0 */
/* USER CODE END I2C1_MspInit 0 */
/**I2C1 GPIO Configuration
PB6 ------> I2C1_SCL
PB9 ------> I2C1_SDA
*/
GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_9;
GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
GPIO_InitStruct.Pull = GPIO_PULLUP;
GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
GPIO_InitStruct.Alternate = GPIO_AF1_I2C1;
HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
![](http://c.51hei.com/a/a/c/51271948042871.jpg)
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
GPIO_InitTypeDef GPIO_InitStruct;
if(hi2c->Instance==I2C1)
{
/* USER CODE BEGIN I2C1_MspInit 0 */
/* USER CODE END I2C1_MspInit 0 */
/**I2C1 GPIO Configuration
PB7 ------> I2C1_SDA
PB8 ------> I2C1_SCL
*/
GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8;
GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
GPIO_InitStruct.Pull = GPIO_PULLUP;
GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
GPIO_InitStruct.Alternate = GPIO_AF1_I2C1;
HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
像上面类似管脚安排的不只I2C1，比方还有SPI等。总之，参照那个复用功能表结合你芯片管脚合理安排就好。当然有STM32CUBEMX配合也就更方便了。
**三、**一个跟管脚安排理解有关的应用异常话题
不只一个工程师反映把基于STM32F072R6主芯片学习板上的调试OK的USB 应用项目，移植到基于STM32F042F4的目标系统时，发现PC机无法发现设备。
STM32F072R6和STM32F042F4隶属于STM32F0同一系列，二者主要差别在于有无DAC。其它外设基本一样，具体到这2个型号，就是GPIO数目上及管脚安排上的细微差异。二者的数据手册也明明白白的写了都支持USB FS DEVICE功能。对于USB应用，二者除了可能管脚安排的差异外，内部驱动程序应该是一模一样。
从手册上看，64脚STM32F072R的USB外设功能管脚为PA11,PA12,属于额外功能，即只需使能相关usb外设应用即可。![](http://c.51hei.com/a/a/c/51271948026025.jpg)
再看看下面20脚 STM32F042F4的相关管脚定义和安排。![](http://c.51hei.com/a/a/c/51271948074624.jpg)
显然，管脚编号为17/18的功能描述有点另类，图下方紧跟一句说明。大致意思PA11/PA12可以通过配置SYSCFG_CFGR1来进行重映射来取代PA9/PA10的功能。是个重要提醒，也不是特别明了，再细看芯片如下的管脚功能描述表。![](http://c.51hei.com/a/a/c/51271948031277.jpg)
我们上面第一个话题就提到32f0芯片管脚往往可能有三种功能，默认功能，复用功能及额外功能。结合上面表格，同样的17、18脚，可能作为PA9 \PA10或后面一大堆的复用功能选择之一，也可能作为PA11\PA12或者与之对应的复用功能及额外功能的选择之一。
不过，要做PA11\PA12或相关复用功能或额外功能，必须首先使能SYSCFG外设并配置SYSCFG_CFGR1寄存器才行，否则基于PA11\PA12的默认功能及复用和额外功能都不会生效。
上面工程师碰到的问题刚好就出在这里，他们只是简单的连接了芯片的第17脚18脚，此时USB_DM/USB_DP功能没生效，即使你代码写得再漂亮，PC机还是无法识别USB设备。
我们不妨通过STM32CUBE MX配置工具来看看STM32F042F4芯片的管脚配置情况。不打开还好，咋一打开似乎让你发现新大陆。
当打开基于STM32F042F4的STM32CUBE的配置界面时，发现跟USB有关的外设配置都是无效状态，第一感觉就是它不支持USB功能。
![](http://c.51hei.com/a/a/c/51271948042542.jpg)
![](http://c.51hei.com/a/a/c/51271948035627.jpg)
再次打开STM32F042F4的数据手册，上面分明写了它是支持USB device的。结合前面的描述，感觉是不是CUBEMX里没有打开相关外设或选型的配置。其实，STM32F0系列芯片里有个外设就是系统配置控制器【SYSCFG】，只有开启了它，上面提到的SYSCFG_CFGR1寄存器配置才有意义。
具体到32f0的CUBEMX界面里就有个SYS的外设可以配置。当你点开那里时你会发现另一番天地。上面提到的一些东西都会赫然而现。
当在左边外设栏的SYS和USB的适当地方勾选，右边管脚配置图立即出现USB通信脚位安排位置。![](http://c.51hei.com/a/a/c/51271948036917.jpg)
小结：
1、STM32芯片的管脚可能呈现三种功能状态之一，默认功能、复用功能、额外功能。
2、stm32芯片外设往往很丰富，管脚复用安排往往很庞大复杂。数据手册里有管脚说明及各类表格，查看时耐心、细心点。
3、STM32CUBEMX工具为我们的芯片管脚安排提供了巨大的方便，但再方便也不能完全不看数据手册，比方上面提到第3个话题如果不看数据手册很可能会把自己带进误区。
//=======================================================================
本人备注::
1>STM32F0XX系列也有一份参考手册------这个跟STM32F10XX系列是不同的！！！
2>STM32F0XX系列管脚的AF定义请参考STM32F031_datasheet_DM00104043.pdf 
3>STM32F0XX系列的PA/PB管脚复用定义如下
![](https://img-blog.csdnimg.cn/20190110155248732.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hxaHJzMjMy,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190110155317950.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hxaHJzMjMy,size_16,color_FFFFFF,t_70)
