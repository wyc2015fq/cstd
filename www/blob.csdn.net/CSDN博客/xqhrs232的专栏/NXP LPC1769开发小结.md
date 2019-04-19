# NXP LPC1769开发小结 - xqhrs232的专栏 - CSDN博客
2016年12月05日 23:04:25[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1904
原文地址::[http://jingyan.eeboard.com/article/65129](http://jingyan.eeboard.com/article/65129)
相关文章
1、LPC1769 LPCXpresso board----[http://www.lpctools.com/lpc1768.lpcxpresso.aspx](http://www.lpctools.com/lpc1768.lpcxpresso.aspx)
2、OM25180FDK: PN5180 NFC Frontend Development Kit for POS Terminal Applications----[http://www.nxp.com/products/identification-and-security/nfc-and-reader-ics/nfc-frontend-solutions/pn5180-nfc-frontend-development-kit-for-pos-terminal-applications:OM25180FDK](http://www.nxp.com/products/identification-and-security/nfc-and-reader-ics/nfc-frontend-solutions/pn5180-nfc-frontend-development-kit-for-pos-terminal-applications:OM25180FDK)
3、LPC17xx用户手册----[http://wenku.baidu.com/link?url=q5x3qi2_aSjjn5HNsFalZfRvWFq2iAKIQFpUUJvpkLY_hLdzhjh4zgl2Rk13D3fA118IFyVx-4ESnLZkfjGEhA15a3mj2vq2qr4aENHurfi](http://wenku.baidu.com/link?url=q5x3qi2_aSjjn5HNsFalZfRvWFq2iAKIQFpUUJvpkLY_hLdzhjh4zgl2Rk13D3fA118IFyVx-4ESnLZkfjGEhA15a3mj2vq2qr4aENHurfi)
4、LPCXpresso IDE for Windows v8.x.x----[https://nxp.flexnetoperations.com/control/frse/download?agree=Accept&element=7632767](https://nxp.flexnetoperations.com/control/frse/download?agree=Accept&element=7632767)
5、LPCXpresso__用户手册----[http://wenku.baidu.com/link?url=fSilF1C3hndhTdU1daxDPjDHtj54yL3flf5fnT2ev0vlMGH84e50NTI_LmGvNMCKtWO9Av8CFZ8uqmQg0-fX3HTiCPVq0UcwTh1Wfxn3KNO](http://wenku.baidu.com/link?url=fSilF1C3hndhTdU1daxDPjDHtj54yL3flf5fnT2ev0vlMGH84e50NTI_LmGvNMCKtWO9Av8CFZ8uqmQg0-fX3HTiCPVq0UcwTh1Wfxn3KNO)

我们这次使用的芯片型号是LPC1769，关于它的使用手册可以参考周立功的LPC17xx用户手册。使用的是NXP官方开发板LPCxpresso LPC1769 Rev B，关于它的详细说明可参考另一个文件LPCXpressoLPC1769revB。在我们这篇文档中，尽可能用最简洁的语言告诉大家如何进行开发。我们这篇文档中的字体有两种，宋体字是关于操作的说明，楷体字是关于原理的说明，可以根据你的需要来选择看或不看。
一、关于LPCxpresso编译环境的简单说明
我们在开发时使用的编译器是NXP官方提供的LPCxpresso，我们不再详细地说明它的使用方法，在这里只对我们用到的功能作一些简要描述。
1. 载入例程。在下载此编译器时，同时下载了关于LPC1769所有模块的使用例程，在我们进行开发时可以对其进行适当的修改，达到事半功倍的效果。在启动LPCxpresso的开发环境时，会让你首先选择工作区（Workspace）你的所有操作只能在某一个工作区中进行，每个工作区有自己的文件夹。当然，点击File，在下拉菜单中你可以进行工作区的切换。下面我们载入项目，如图：
![载入项目](http://www.eeboard.com/wp-content/uploads/2014/07/%E8%BD%BD%E5%85%A5%E9%A1%B9%E7%9B%AE-500x342.jpg)
可以进入载入项目的对话框中：
![载入项目对话框](http://www.eeboard.com/wp-content/uploads/2014/07/image005-500x234.png)     
 从官方下载的例程都是zip压缩包，所以点击第一个编辑框后面的Browse把，找到LPC1769的例程库，然后选中所有例程，点击Finish完成。
![导入完成](http://www.eeboard.com/wp-content/uploads/2014/07/%E5%AF%BC%E5%85%A5%E5%AE%8C%E6%88%90-500x168.jpg)
2. 编译，链接，下载，调试
![编译、调试](http://www.eeboard.com/wp-content/uploads/2014/07/image011-500x248.png)
首先我们先说说菜单Project下的Open Project和Close Project。这两个选项不是载入项目或者把项目从工作区中删除，而是将已经载入工作区的项目关闭或者将已经关闭的项目重新打开，这样的目的是尽量减少打开的项目，减少内存的占用。当我们选中某已打开的项目时，左下角锤子图片的字符会变为黑色，单引号中显示的是项目的名称，点击就会进行编译与链接。编译链接结束后，我们可以点击锤子下面绿色的小虫子进行下载和调试，或者点击右上角的小虫子。如果是第一次调试，我们需要建立调试对象，即点击右上角小虫子后面的绿色三角键。![调试](http://www.eeboard.com/wp-content/uploads/2014/07/%E8%B0%83%E8%AF%951-500x191.jpg)
其中，AImove Debug和AIScreen Debug是已经建好的调试对象，点击会进行下载和调试，而我们新建对象要点击Debug Configureations…。我们要在C/C++MCU Application下创建对象，点击新建后进入新建调试对象界面，当前的项目会默认为新建的对象。
![新建对象](http://www.eeboard.com/wp-content/uploads/2014/07/image025-500x330.png)
点击Debug即可进入调试。
注：LPCxpresso暂时不支持仿真调试。
3、新建项目
我们利用官方的例程来建立我们自己的项目，下面来说明如何建立项目。
![新建项目](http://www.eeboard.com/wp-content/uploads/2014/07/image027-500x267.png)
得到新建项目的对话框：
![新建项目对话框](http://www.eeboard.com/wp-content/uploads/2014/07/image029-500x475.png)
选中LPCXpresso C Project，点击Next>
![新建项目对话框2](http://www.eeboard.com/wp-content/uploads/2014/07/image031-500x455.png)
选中相应的芯片型号，并选中其目录下的C Progect
![新建项目对话框3](http://www.eeboard.com/wp-content/uploads/2014/07/image033-500x475.png)
选择具体型号：
![新建项目对话框 选择型号](http://www.eeboard.com/wp-content/uploads/2014/07/image035-500x467.png)
我们用的是LPC1769，所以选择LPC1769![新建项目 选择内核](http://www.eeboard.com/wp-content/uploads/2014/07/image037-500x472.png)
有时候，在我们的工作区中使用的内核不是LPCXpresso当前版本默认的，所以直接选None，不添加内核。在建立之后我们自己添加。选完None就可以按Finish了。
接着我们建立我们的程序，将我们需要的源文件和对应的头文件直接放到项目的目录下，不用像Keil MDK那样添加，LPCXpresso默认在项目目录中的src文件夹下的所有文件都是项目文件。
然后我们加入内核。首先，右键点击我们建立的项目，会出现下拉菜单。
![载入内核后 下拉菜单](http://www.eeboard.com/wp-content/uploads/2014/07/image039-500x439.png)
点击Properties，进入项目设置对话框。
![项目设置对话框](http://www.eeboard.com/wp-content/uploads/2014/07/image041-500x330.png)
在Project References栏中选择内核项，我们用的内核是CMSISv2_LPC17xx。
在C/C++ Build下的Settings项中，Tool Settings 页面中MCU C Compiler下Symbols栏中添加
“__USE_CMSIS=CMSISv2_LPC17xx”
在Includes栏中添加文件夹CMSISv2_LPC17xx中的inc目录![项目设置2](http://www.eeboard.com/wp-content/uploads/2014/07/image043-500x153.png)
在C/C++ Build下的Settings项Tool Settings 页面中MCU Linker下的Libraries中添加库名称CMSISv2_LPC17xx和库的路径CMSISv2_LPC17xx/Debug，记住，内核库在Debug路径中。![项目设置 内核调用](http://www.eeboard.com/wp-content/uploads/2014/07/image045-500x295.png)
接下来，就可以开始在新建的项目中编程了。
二、GPIO端口的使用
我们已经为GPIO的使用建立了一个比较完整的函数库，因此我们的工作是建立在我们的函数库之上。
1. 我们的GPIO函数库
我们建立的函数库由源文件lpc_GPIO.c和它的头文件lpc_GPIO.h组成。使用时，只需要看懂lpc_GPIO.h即可。
GPIO函数库共有四个函数：![GPIO函数库](http://www.eeboard.com/wp-content/uploads/2014/07/image047-500x57.png)
其中，LPC_GPIO_Init()是GPIO端口的初始化程序，LPC_GPIO_Read()是利用已经初始化好的端口实现读数据，LPC_GPIO_Write()是利用已经初始化好的端口来写数据，LPC_GPIO_SetBit()和LPC_GPIO_ClrBit()是对已经初始化好的端口的某一位或者某几位来置0或者置1，它们的具体操作与它们的变量有关。
首先我们来看LPC_GPIO_TypeDef类型。此类型是在LPC17xx.h中定义的结构体，它的结构与GPIO寄存器的地址分配一致，有兴趣可以看LPC17xx.h文件。我们只需记住，如果你用0端口，这个变量写为LPC_GPIO0，用1端口些为LPC_GPIO1，以此类推，当然你要查看你的芯片中究竟有没有这个端口。如果你不大明白“*”和“结构体”的含义，建议你查询C语言的基本书籍。
下面我们来说在lpc_GPIO.h中我们定义的结构体和它的使用。在lpc_GPIO.h文件中，我们定义了两个结构体，定义好这两个结构体，就能使用这个GPIO输入输出的程序库了。
LPC_GPIO_Def结构体，这个结构体是用来对端口进行初始化定义，里面共有六个成员，它们的可用值在#define中都有定义。这六个成员分别是：
PIN_SEL，用来设置端口的工作模式，有4个可用值，分别是GPIO_FUN_IO和GPIO_FUN_2~GPIO_FUN_4，其中GPIO_FUN_IO是一般的IO端口的输入和输出，其它的功能可参见周立功《LPC17xx用户手册》第七、八章。
PIN_Mode，用来设置输入输出的电气模式，有四个可选项，分别是上拉、下拉、浮空和中继模式，在我们的#define中有定义。
PIN_DO，用来设置是否此引脚需要使用漏极输入输出（如果引脚使用I2C功能必需使用），有enable和disable两种模式。
PINDIR，设置此端口是输入还是输出，有Input和Output两种模式。
PINMASK，定义端口是否被mask，如果定义“是（true）”则此引脚不能使用（当然MASK的意义并不在于此，关于进一步的了解可以参考周立功《LPC17xx用户手册》第九章）
Pin，定义使用的引脚，是一个32位2进制数，来设置某端口引脚是否使用。
下面是一个使用的实例，我们定义GPIO0端口的7、24引脚为一般引脚输入，上拉模式，不用MASK，非漏极：
![使用实例](http://www.eeboard.com/wp-content/uploads/2014/07/image050.png)     
 LPC_GPIO_Dat结构体，用来对端口进行操作，有三个成员变量：
联合体PORT(x)，其中PORT是对32位端口进行操作，PORTL和PORTH分别对低、高16位端口进行操作，PORT0~3将整个32位端口分为4个8位端口进行操作。关于联合体（也叫共用体）不会的话请查看任何一本C语言的教科书。
GPIO_Bit，来设置如何使用端口，是作为32位的端口，还是使用作为8位端口的某个，其设置可参考#define中的GPIO_32Bit到GPIO_8Bit_3。
GPIO_Pin，来设置用整个端口中的哪几个，其定义模式与LPC_GPIO_Def中的Pin成员相同，下面是我们使用的例子：
将GPIO0.17和GPIO0.18置低电平：
![GPIO0.17和GPIO0.18置低电平](http://www.eeboard.com/wp-content/uploads/2014/07/image052.png)    
 用LPC_GPIO_ClrBit()与LPC_GPIO_SetBit()函数是不用对成员PORT(x)进行设置的，只有用函数LPC_GPIO_Read()和LPC_GPIO_Write()时才用到PORT(x)，下面是一个实例：
LPC_GPIO_Read(&LPC_Key,LPC_GPIO0);
将LPC_Key中成员GPIO_Pin中定义的引脚中的数据放入成员PORT(x)中。
可以参考工程AImove中main.c中的子函数void ShowLEDInit(void)和void KeyControl(uint32_t PinX)，在这里，我们定义GPIO0的9端口接模式指示灯，8端口为采集状态指示灯，GPIO0.25、26与GPIO1.30、31为采集数据指示灯，GPIO0.7和24端口分别为模式切换和数据采集开始/停止切换键，GPIO0.17、18来给这两个键提供低电平输入源。
按住模式切换键，模式指示灯（红）开始闪烁，当它一直保持亮时松开，进入数据采集状态，这时按模式切换键，会接换采集数据的种类（采集数据指示灯会显示出这4位2进制数）。按采集切换键，会开始或停止采集（开始时采集状态指示灯（蓝）亮，结束时灭）相应的数据。采集好相应的数据后，再按住模式切换键，模式指示灯开始闪烁，当它保持熄灭时松开，推出采集模式。
三、用GPIO端口来驱动液晶屏
四、外部中断的使用
LPC1769的外部中断端口共有4个，它们分别对应不同的中断函数，所有的中断函数名称在LPC17xx.h中列出，在这一节我们只考虑外部中断，所以只用到4个外部中断函数![4个外部中断函数](http://www.eeboard.com/wp-content/uploads/2014/07/image054-500x66.png)
这4个函数已经写到我们的建立项目的函数库：lpc_EXTI.c中，它的头文件是lpc_EXTI.h。中断的原理其实很简单，当我们触发中断时，会自动调用对应的中断函数，其名字就是在中断名字后面的n改为Handler，四个中断函数名如下（这段程序在lpc_EXTI.h中）：![四个中断函数名](http://www.eeboard.com/wp-content/uploads/2014/07/image056-500x83.png)
外部中断分为两种，一种是单引脚中断，一种是端口中断，在单引脚中断中，中断触发引脚与中断函数的对应关系如下：
|引脚|P2.10|P2.11|P2.12|P2.13|
|----|----|----|----|----|
|中断函数|EINT0_IRQHandler|EINT1_IRQHandler|EINT2_IRQHandler|EINT3_IRQHandler|
另外还有一种是对应端口的中断，只有P0和P2端口才能触发，它们对应的中断函数都是EINT3_IRQHandler，至于具体是哪个端口引起的，在中断函数中再进行判断。
下面介绍我们自己编写的中断函数库。要使用中断，必须先将中断定义好，在我们的函数库中，定义单引脚中断的是结构体：
typedef struct{
uint32_t EXTI_EINTx;//使用第几个单引脚外部中断，取值为0~3
uint32_t EXTI_Mode;//设置中断的触发模式，边沿触发或是电平触发，其值为EINT0_EDGE或者 EINT0_level
uint32_t EXTI_Polar;//设置中断触发模式是上升沿还是下降沿，其值为EINT0_RISING或 EINT0_FALLING
uint32_t EXTI_Priority;//中断优先级，其值为一正整数
}EXTI_Def;
如注释中所写，将这四个成员变量定义好，然后调用函数
void LPC_EXTINT_Init(EXTI_Def* EXTI_Param);
将定义的中断初始化，其中的变量就是定义结构体的地址，比如我们定义了一个结构体：
EXTI_Def   EXTI1;
初始化时的语句为：LPC_EXTINT_Init(&EXTI1);
然后使用函数void EXTI_Switch(bool EXTI_ON_OFF, IRQn_Type IRQn)打开或者关闭中断，例如开通0端口的语句为 ：
EXTI_Switch(EXTI_ON, EINT0_IRQn);
接着就在文件lpc_EXTI.c中对应的中断函数里添加所要执行的语句就可以了。
在我们的设计中P2.10~P2.13的端口用来控制电机，因此我们用的是端口中断，使用P0端口中的P0.7和P0.24引脚。使用端口中断，就需要用到另一个结构体：
typedef struct{
       uint32_t EXTI_Pin;//整个端口中断中，有效的引脚
       uint32_t EXTI_Port;//端口中断中，有效的端口，其值为EXTI_P0或者EXTI_P2
       uint32_t EXTI_Polar;//中断触发模式，上升沿或者下降沿
       uint32_t EXTI_Priority;//中断优先级，其值为一个整数
}EXTI_Group_Def;
这是用来端口中断的结构体。在我们的程序中：
LPC_GPIO_Def LPC_GPIO_Structure;
LPC_GPIO_Dat LPC_GPIO_Pin;
EXTI_Group_Def EXTI_Key_Structure;
//定义按钮1和2的输入端口： P0.7，P0.24
LPC_GPIO_Structure.PINDIR = Input;
LPC_GPIO_Structure.PINMASK = disable;
LPC_GPIO_Structure.PIN_DO = disable;
LPC_GPIO_Structure.PIN_Mode = GPIO_MODE_PullUp;
LPC_GPIO_Structure.PIN_SEL = GPIO_FUN_IO;
LPC_GPIO_Structure.Pin = GPIO_Pin7 | GPIO_Pin24;
LPC_GPIO_Init(&LPC_GPIO_Structure,LPC_GPIO0);
//接下来设置中断
EXTI_Key_Structure.EXTI_Pin = GPIO_Pin7 | GPIO_Pin24;
EXTI_Key_Structure.EXTI_Port = EXTI_P0;
EXTI_Key_Structure.EXTI_Polar = EINT0_FALLING;
EXTI_Key_Structure.EXTI_Priority = 9;
LPC_EXTIGrouP_Init(&EXTI_Key_Structure);
//打开中断
EXTI_Switch(EXTI_ON, EINT3_IRQn);
这段程序取自Main.c中的ShowLEDInit(void)，来初始化LED指示灯和两个按钮。
对于IO接口的设置，上一节中已经提过了，下面我们结合我们的设计说明端口模式的外部中断的使用。
首先，我们设置使用的引脚为7和24引脚（GPIO_Pin7和GPIO_Pin24的设定在lpc_GPIO.h中）
然后，设置我们使用的端口为P0口
接着，我们设触发模式为下降沿模式（EINT0_FALLING的设定在lpc_EXTI.h中）
最后，我们用9来表示此中断的优先级。
设置完成员函数后，我们进行初始化，将结构体EXTI_Key_Structure的地址写入函数LPC_EXTIGrouP_Init中，初始化后，我们打开中断，注意，外部端口中断的中断函数对应着EINT3_IRQHandler，所以在函数中我们用EINT3_IRQn。这样，每当P0.7或者P0.24有一个下降沿产生时，函数EINT3_IRQHandler()会被自动调用。
至于判断是哪个引脚产生的中断，我们利用下一段程序来判断：![引脚中断判断函数](http://www.eeboard.com/wp-content/uploads/2014/07/image058-500x267.png)
因为我们的中断用得是下降沿驱动，所以我们用了几步逻辑运算，目的是得到一个32位2进制数，对应与引脚的那一位为1，其余为0。
在lpc_EXTI.c中，我们包含了文件main.h，这样我们就可以调用main.c中的函数Keycontrol(uint32_t).
五、定时器的使用
由于时间的关系，从这一节开始我们直接使用例程库中的代码。我希望我们在不断的开发过程中会用我们自己编写的代码去替换例程中的代码。
在例程库中，定时器的例程共有两个：RITTimer和Timer，相比较来看，RITTimer更简单些，我们将RITTimer中的RITTimer.h和RITTimer.c复制到我们的目录下，在我们使用的编译器中，加入文件的方式与Keil是不同的，只要把文件放入目录下，编译的时候就会自动包含了。
我们来看RITTimer.h文件![RITTimer.h文件](http://www.eeboard.com/wp-content/uploads/2014/07/image060-500x179.png)
我们的目的是每隔0.1秒我们会检查小车的位置和方向，然后进行相应的操作。![检查小车的位置和方向，改动程序](http://www.eeboard.com/wp-content/uploads/2014/07/image062-500x135.png)
在我们的程序中，直接调用init_rit_timer()函数，其变量值用的是在RITTimer.h中定义的TIME_INTERVAL,这个数值为89999，它保证每10ms执行一次中断。在所有的初始化和初值设定工作完成后，我们调用函数：enable_rit_timer(); 开启定时器。
我们的目的是每0.1秒检查车的位置，但是我们设的数值为0.01秒中断，因此我们在中断函数rittimer.c中作了改动：
![中断函数rittimer.c改动](http://www.eeboard.com/wp-content/uploads/2014/07/image064-500x120.png)同样我们在rittimer.c里引用了main.h,可以调用在main.c中的函数adjustCar(),在我们的程序中，基本是复制了例程中的结构，没有用到reset_rit_timer();
六、PWM的使用
关于这一部分，主要进行下面几部分的操作：
首先，在整个的库函数文件夹里找到pwm文件夹，然后打开此文件。
打开里边的.cproject文件![.cproject文件](http://www.eeboard.com/wp-content/uploads/2014/07/image068-500x108.png)
下来，我先说一下，关于LPC1769这块片子和它的硬件搭建。
LPC1769本身含有6路PWM，而我们控制小车，基本上用到的，只有2路，这2路PWM通过L298N模块（接下来会讲到），来控制电机的转速。PWM是通过调节占空比，也就是控制输出脉冲占整个周期的百分比。简单的，PWM对于智能小车来说，就是用来控制速度的。当然，PWM在其他方面也有很多用处，这里只针对智能小车做简单介绍。
软件部分设计：
首先要做的，就是对GOIO的设定，因为PWM引脚作为功能性引脚，在没有设定之前，属于普通输入输出引脚，所以我做了以下设定：![GOIO设定](http://www.eeboard.com/wp-content/uploads/2014/07/image070-500x109.png)
然后，你需要在pwm.c这个c文件中，修改一些参数，主要是对占空比的设定。通过更改m和n的值，从而确定占空比。（m、n是自己设的参数）![占空比 设定](http://www.eeboard.com/wp-content/uploads/2014/07/image072-500x61.png)
接下来是对电机正反转的设定，其实也是对GPIO的设定。![GPIO 设定](http://www.eeboard.com/wp-content/uploads/2014/07/image074.png)
最后就是PWM实现其功能的时候了，
开机时，先对PWM进行初始化![PWM进行初始化](http://www.eeboard.com/wp-content/uploads/2014/07/image076-500x151.png)
这其中的一些函数，例如PWM_Set( CHANNEL_NUM, cycle, offset )和PWM_Start( CHANNEL_NUM )，都是LPC1769这块片子里边给出来的，需要你看明白就好了，不必修改。
做完这一项工作之后，PWM这一块，算是基本完成。
七、串行通信的使用
串行通信的程序我们首先将例程中项目UART中的文件uart.c和uart.h复制到我们项目的文件夹中。我们来看uart.h文件![uart.h文件](http://www.eeboard.com/wp-content/uploads/2014/07/image078-500x91.png)
在我们的程序中，直接使用了好几个例程中的文件，用到缓冲区的例程，缓冲区的长度都定义为BUFSIZE，我们稍稍地加以改动以示区分，在串行通信中，我们改为SERBUFSIZE。
UAERInit()是初始化函数，portNum为端口号，Baudrate为波特率；UART0_IRQHandler()与UAER1_IRQHandler()是串口0和串口1的中断函数，UARTSend()是发送函数，portNum为使用的端口号，BufferPtr为缓冲区，Length为发送字符长度。
在我们的设计中，我们同时使用了串口0和串口1，串口0用来与GSM模块通信，串口1用来和ZigBee模块通信（在main.h中）：
#define    UART_PORT_GSM    0 //GSM通信用串口0
#define    UART_PORT_Zigbee   1 //Zigbee通信用串口1
……
UARTInit(UART_PORT_GSM, 9600); //与GSM通信
UARTInit(UART_PORT_Zigbee, 9600);     //与Zigbee通信
这样，串行通信0和1就打开了。
在例程中，串口0的TXD和RXD分别是P0.2和P0.3，而串口1的TXD和RXD分别是P0.15和P0.16。连接时不要连错，一定要注意的是，LPC1769的TXD要连着Zigbee或GSM的RXD，而其RXD要和Zigbee或GSM的TXD连接。
例程中的串行通信是用到中断的，每当串口接到一个字符，就会调用串口中断函数，串口中断函数比较复杂，有兴趣的同学可以学习学习，学习例程是提高自己水平最有效的方法，而我们这里只告诉大家最简单使用例程的方法。因为我们使用Zigbee或者GSM，其信息发送的模式是有固定格式的，这样才能保证不会错误地接到信息。Zigbee发给我们的信息的格式为：DiRSSIn：XXXX或者DiLQIn：XXXX，其中i是从0~9的数，它表示设备的序号，n=1或2，它表示接到的数值是固定发射器1的数值还是发射器2的数值。它们都是以回车符结尾，回车符的十六进制码是0x0A，因此我们在中断函数UAER1_IRQHandler()的结尾加入（在uart.c中）：
if(UART1Buffer[UART1Count -1] == 0x0A){//当串口接到换行符时，开始处理读入数据
GetUART(1);
}
当收到一个换行符（回车符）时，表示收到了一条完整的信息，调用GetUART(1);
同理，我们也在UAER0_IRQHandler()的结尾加入：
if(UART1Buffer[UART0Count -1] == 0x0A){//当串口接到换行符时，开始处理读入数据
GetUART(0);
}
关于处理传输的数据，在函数GetUART()之中，GSM的传输格式还没有最后定下来，另外我们只用到串口的接收而没有用到串口的发送。
八、I2C总线的使用
我们主要使用I2C来读取电子罗盘的数据，关于电子罗盘的介绍，可参考文件“HMC5883L中文规格书.pdf”。我们在这里只对它进行最简单的介绍。
我们用的电子罗盘一共有5个接口，VCC接3.3V，GND接地，SCL接I2C1的时钟线P0.1，SDA接P0.0，最后一个引脚可以不管。
我们直接使用I2C的例程：其头文件为：![I2C的例程](http://www.eeboard.com/wp-content/uploads/2014/07/image080-500x120.png)
其中I2C1Init()我们修改过，因为I2C1在LPC1769上有两个不同的总线接口，可以根据实际情况来选择使用，我们选择的是P0.1和P0.0，因此我们初始化时：
I2C1Init( 0 );
如果变量改为1，使用的就是P0.15和P0.16引脚。
在I2C.c中，有4个外部数组：
volatile uint8_t I2CMasterBuffer[I2C_PORT_NUM][I2CBUFSIZE];
volatile uint8_t I2CSlaveBuffer[I2C_PORT_NUM][I2CBUFSIZE];
volatile uint32_t I2CReadLength[I2C_PORT_NUM];
volatile uint32_t I2CWriteLength[I2C_PORT_NUM];
其中，在I2CReadLength[I2C_PORT_NUM]中确定读的字节数，
在I2CWriteLength[I2C_PORT_NUM]中确定写的字节数，而I2C_PORT_NUM为端口号。在函数voidGY273_Init(void)中，我们首先对电子罗盘的寄存器进行赋值，来确定电子罗盘的工作方式：
I2CWriteLength[PORT_USED] = 3;                       //写入字节长度定义为3
I2CReadLength[PORT_USED] = 0;                        //读入字节长度定义为0
//在地址为HMC5883_ADDR的器件的第02寄存器中写入0x0，为连续测量模式，即不断地测量角度
I2CMasterBuffer[PORT_USED][0] = HMC5883_ADDR;   //给I2C写入三个数，第一个是电子罗盘
I2CMasterBuffer[PORT_USED][1] = 0x02;               //的地址，第二个是要操作的寄存器地址
I2CMasterBuffer[PORT_USED][2] = 0x00;              //第三个是给此寄存器中放入的数值
I2CEngine( PORT_USED );                              //此函数用来根据数组的数值执行操作
for ( i = 0; i < 0x20000; i++ );                             /* 延时 */
//在地址为HMC5883_ADDR的器件的第01寄存器中写入0xE0，用来设定磁场的测量范围
I2CMasterBuffer[PORT_USED][0] = HMC5883_ADDR;
I2CMasterBuffer[PORT_USED][1] = 0x01;
I2CMasterBuffer[PORT_USED][2] = 0xE0;
I2CEngine( PORT_USED );
for ( i = 0; i < 0x20000; i++ );  /* 延时  */
从我们的程序中可以看出，对于I2C总线的操作就是设定读写字节的长度，然后执行I2CEngine( PORT_USED );即可。PORT_USED=1，表示我们使用的是I2C1。
在I2C总线下，电子罗盘就相当一个存储器，我们不断地从其某个寄存器中读取方向数值，电子罗盘中寄存器的地址和对应数据意义如下：
![电子罗盘中寄存器的地址和对应数据](http://www.eeboard.com/wp-content/uploads/2014/07/image082-500x312.png)     
 每个寄存器中放置一个8位2进制数据，其中MSB为高8位数据。我们的小车只需要得到X与Y的值就可以了，所以我们在函数void GY273_GetAngle(int16_t *x, int16_t *y)中读取数据的代码为：
//转换模式为读写,写2字节,读12字节
I2CWriteLength[PORT_USED] = 2;      //写数据长度为2
I2CReadLength[PORT_USED] = 12;       //读数据长度为12---保险起见，我们读取了所有数据
//写入读的器件地址和器件的寄存器地址0x0
I2CMasterBuffer[PORT_USED][0] = HMC5883_ADDR;   //向I2C总线中写入电子罗盘的地址
I2CMasterBuffer[PORT_USED][1] = 0x00;             //写入读取数据首地址的寄存器地址
I2CMasterBuffer[PORT_USED][2] = HMC5883_ADDR | RD_BIT;//从I2C中电子罗盘地址中
I2CEngine( PORT_USED );//根据上面的数组来执行               //连续读取12个数
for(i=0;i<0x2000;i++);   /* 延时  */
其中，电子罗盘的地址为0x3C，我们已经用#define和HMC5883_ADDR绑定。I2C的地址一共有7位，最后一位为零表示写命令，为一表示读命令，RD_BIT为0x01，它与地址码作或运算的结果为0x3D，意为对地址为0x3C的寄存器开始读操作。又因为HMC5883芯片中寄存器的指针是自动加1的，因此我们只需将首地址定义出来，剩下的连续读取就可以了。
九、以太网模块的使用
关于以太网的模块，我们完全使用例程中的结构，我们发现，如果将程序复制到我们项目的文件夹中，编译总是出错，我们只能将我们编写的程序复制到以太网项目EMAC的文件夹中，这样不会出错，可能是我们建立项目时有一些设置出了问题。
