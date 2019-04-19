# STM32 USB 之从0开始移植笔记 - xqhrs232的专栏 - CSDN博客
2018年01月27日 18:27:44[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：215
原文地址::[http://blog.csdn.net/GZZXBCXM2005/article/details/51347624](http://blog.csdn.net/GZZXBCXM2005/article/details/51347624)
STM32 USB 之从0开始移植笔记
        -----------------------------------动机-----------------------------------
写在前面的话：最近逛淘宝无意间发现RC522居然只要10元左右就可以包邮买到，真是太便宜了，就忍不住买了个回来玩玩。到货移植到我的板子上OK 后突然发现我的USB口紧张了，一个用来给板子供电一个插jlink 一个插入usb转串口给RC522下命令。就想着将板子供电和RC522传输用一个USB接口来实现。这就是这次折腾USB的来由~-~
       ----------------------------------开始折腾USB-----------------------------
首先我想拿来主义，百度一个完整的USB范例就完事。以上就百度了下STM32
 USB HID
那就一个多啊，已是我就下一个看一个看是否适合我用。很快我就发现了两个问题不能说所有至少我百度到到都是这样：
1.这些范例大都是在官方USB范例上修改得来，工程名都没做修改。
2.这些范例大都是用的3.x的USB
 FW，官网上我搜到的已是4.0了
3.这些范例中STM中为了兼容他的N个板子做了很多通用性的处理。这些对于初学者来说不够清晰明了。
所以我决定从0开始构建一个方便移植的模块，移植他的JoyStickMouse。在我的项目中用到时加入这个模块即可。
首先从官网下载最新的USB库得到的是一个压缩包stsw-stm32081.zip解压后如图：
![](https://img-blog.csdn.net/20160508205318797?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Projects 是官方的范例，Libraries 是USB的4.0驱动
接下来用MDK建立一个工程我用的MDK是4.12 
 STM的固件库是3.5的 USB库就是4.0的了，芯片用的是STM32F103VC   建立以下文件夹
![](https://img-blog.csdn.net/20160508205341625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
在USBFW 下建立两个文件夹分别存放USB的底层驱动文件和中间层接口文件并把STM32_USB-FS-Device_Lib_V4.0.0\Libraries\STM32_USB-FS-Device_Driver中的inc和src复制到DRIVER文件夹内，把STM32_USB-FS-Device_Lib_V4.0.0\Projects\JoyStickMouse中的inc和src复制到MID文件夹内。一般我们只要修改MID层的文件而DRIVER是不用修改的。
建好后的工程截图：
![](https://img-blog.csdn.net/20160508205409097?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这个时候编译下可能出来一大堆的错误。这个USB的源码分析百度有很多很好的文章讲解的非常详细大家可以自行百度。
接下来说说我的修改----------------------------------------
首先在usb_conf中定义了Buffer
 Description Table 
![](https://img-blog.csdn.net/20160508205438628?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这个0x18意味着留24字节给table  只用到3个端点了，一般我们用端点0枚举，端点1输入，端点2输出这个定义是够用了，如果你的端点应用大于3 就要修改这个表的大小了。
-------------------------------------------修改硬件抽象---------------------------------------------------------
在有3个文件相关：
1.platform_config.h  STM官方板子的资源平台配置文件。
2.hw_config.c   hw_config.h  USB的硬件相关配置
首先我用的不是官方板子，把#include "platform_config.h"注释掉只保留其中的ID和USB接入检测机制的上拉控制IO宏定义（我的板子是PC6,这个根据实际情况修改即可）
![](https://img-blog.csdn.net/20160508205509269?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
官方范例中在main 用
Set_System();
 USB_Interrupts_Config();
 Set_USBClock();
USB_Init();
完成了USB的初始化工作，
Set_System();
这个名字太大了，很显然用在我的项目中太大好听，修改hw_config.c
    voidUSB_Configurage(void)
{
         GPIO_InitTypeDefGPIO_InitStructure;
         /*enable the PWR clock */
         RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
         //USB用的IO
         RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
         //USB时钟
         Set_USBClock();
         RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
         GPIO_InitStructure.GPIO_Pin= GPIO_Pin_6;
         GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
         GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_OD;
         GPIO_Init(GPIOC,&GPIO_InitStructure);
        /*C6-USB接入控制口 */
           /* Enable the USB disconnect GPIO clock */
           RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_DISCONNECT,ENABLE);
           /* USB_DISCONNECT used as USB pull-up */
           GPIO_InitStructure.GPIO_Pin =USB_DISCONNECT_PIN;
           GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
           GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
           GPIO_Init(USB_DISCONNECT,&GPIO_InitStructure); 
         USB_Cable_Config(DISABLE);
         USB_Cable_Config(ENABLE);
         USB_Interrupts_Config();
         USB_Init();
}
最后在usb_pwr.c 做些小改动，因为不知道他那个板子是怎么唤醒的，所以把那些都屏蔽掉了。具体见代码咯
这样在main.c中只要USB_Configurage(); 就搞定USB初始化了，当然还要在h文件中做些接口声明和去掉一些和我的板子硬件关系不大的函数，目的只有一个简单明了。
![](https://img-blog.csdn.net/20160508205549269?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
编译一下，通过。这样以后在任何一个项目中要用到USB时只要复制USBFW中的文件，然后小改下MID层的文件就可以用上USB了，在main.c中只要
#include"usb_lib.h"
#include"hw_config.h"
#include"usb_pwr.h"
在任何需要发送数据的地方将数据写入buff就完成了，当然不同的应用可能还要修改端点初始化和描述符等等，但这些应该都是大把的例子可以参照复制的了。具体看代码咯个人觉得st的c应用还是很好的特别是指针函数数组这块 ~-~
我的板子用了4个按键（PD3:6）来控制PC的光标4个方向的移动，将代码下到板子上的结果就是按下相应的按键光标就移动咯
至此就移植完了，接下来继续移植官方的另外几个例子
 代码下载     http://download.csdn.net/detail/gzzxbcxm2005/9513812

