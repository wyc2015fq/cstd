# DSP TMS320C6000基础学习（6）—— gel文件 - xiahouzuoxin - CSDN博客





2013年07月31日 22:10:18[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：11010








**什么是gel文件？gel文件能干什么？**

gel全称General Extended Language，即通用扩展语言文件，gel文件中由类似C语言的代码构成，gel语言是一种解释性语言，gel文件扩展名为.gel；

gel文件用于（1）扩展CCS功能，比如菜单选项等，（2）通过gel可以访问目标板的存储器。




***1. gel基本语法——类C***

gel函数和gel参数不需要在DSP程序中定义。gel具有C语言的很多相似的东西：函数、return语句、if-else语句、while语句、与C一样的注释方式、#define，这些函数或语句的用法也与C中的非常类似。




**GEL函数**

funcName(param1 "discription" [,param2 
"discription", param3 "discription",...])

{

statements;

}

gel函数中不用声明返回类型和参数类型，但函数中可以使用return语句返回；

参数使用“参数+字符串类型的描述”组成，参数不需要定义，可以是以下的任意一种：实际/仿真的DSP目标板的符号值；数字常量（表达式或常值）；字符串常量。

GEL函数调用：通常可以在输入C表达式的任意地方调用GEL函数，也可以在另一个GEL函数中调用GEL函数。GEL函数无法递归调用。




**GEL语句**

返回语句：



```cpp
return [expression];
```



条件语句：



```cpp
if (exp)
	statements 1;
else 
	statements 2;
```
循环语句：




```cpp
while (exp) {
	statements;
}
```






**GEL预处理**



```cpp
#define identifier(arguments list)     token-expression
```




**GEL注释**

// 注释

/* 注释 */




***2. gel特有关键字***


menuitem/hotmenu

在CCS v4.2中测试，menuitem添加Scripts菜单下的子菜单项，hotmenu添加menuitem定义菜单项的子菜单项，参考本文后面的例子。

这两个关键字声明的函数都不需要参数，比如



```cpp
menuitem "Addressing Modes";
hotmenu C27x_Mode()
{
    AMODE = 0;
    OBJMODE = 0;
}

hotmenu C28x_Mode()
{
    AMODE = 0;
    OBJMODE = 1;
}

hotmenu C2xLP_Mode()
{
    AMODE = 1;
    OBJMODE = 1;
}
```
上面代码将产生如下的菜单结构，



Scripts

    - Addressing Modes

- C27x_Mode

- C28x_Mode

- C2xLP_MODE


**dialog**

向menuitem定义的菜单中添加一个入口子菜单，并在点击子菜单时弹出对话框。

menuitem "MyFunc"

dialog InitTarget(StartAddr "Starting Address", EndAddr "Ending Address")

{

statements;

}

dialog RefreshTarget()

{

statements;

}




**slider**

添加滑动条，每次移动滑动条都用滑动条上的新值重新调用GEL文件，定义格式如下，

slider param_def(minVal, maxVal, increment, pageIncrement, paramName)

{

statements;

}




***3. gel文件的例子***



```cpp
/* 
 * This GEL file (DSP621x_671x.gel) provides example code on how to 
 * reset the C6x DSP and initialize the External Memory Interface.
 *
 * You will have to edit settings in emif_init() to your own 
 * specifications as the example is applicable to the C6711 DSK.
 *
 * This file is has minimal functionality and is designed to be used
 * as a starting point for custom GEL files.
 *
 * Refer to CCS Help for detailed information on GEL commands.
 *
 */

/*
 * The StartUp() function is called every time you start Code Composer.
 * It should only include functions that do not "touch the hardware" -
 * Hardware initialization should be invoked from the OnTargetConnect() 
 * function or the GEL menu.
 */ 
StartUp()
{

	/* setMemoryMap;  
	this should be a function to initialize the mem map based
	on the particular hardware that is used
	*/
}

/*--------------------------------------------------------------*/
/* OnTargetConnect() -- this function is called after a target  */
/* connect.                                                     */
/*--------------------------------------------------------------*/
OnTargetConnect()
{
	/* 	GEL_Reset is used to deal with the worst case senario of
		unknown target state.  If for some reason a reset is not
		desired upon target connection, GEL_Reset may be removed 
		and replaced with something "less brutal" like a cache
		initialization function
	GEL_Reset();  
	*/
}

OnReset(int nErrorCode){
/*	emif_init();  */
}

/*
 * OnPreFileLoaded()
 * This function is called automatically when the 'Load Program'
 * Menu item is selected  .....
 */
OnPreFileLoaded()
{
	CleanCache();
}

/*
 * CleanCache()
 * Actually Invalidate L1D, L1P, and L2 
 */
CleanCache()   {
	*(int *)0x01845004 = 1;
	}  

emif_init()
{
/*---------------------------------------------------------------------------*/
/* EMIF REGISTERS */
/*---------------------------------------------------------------------------*/

	#define EMIF_GCTL       0x01800000
	#define EMIF_CE1        0x01800004
	#define EMIF_CE0        0x01800008
	#define EMIF_CE2        0x01800010
	#define EMIF_CE3        0x01800014
	#define EMIF_SDRAMCTL   0x01800018
	#define EMIF_SDRAMTIMING  0x0180001C
  	#define EMIF_SDRAMEXT     0x01800020

/*---------------------------------------------------------------------------*/
/* EMIF REGISTER VALUES - these should be modified to match TARGET hardware  */
/*---------------------------------------------------------------------------*/

  	*(int *)EMIF_GCTL = 0x00003040;/* EMIF global control register         */
  	*(int *)EMIF_CE1 = 0xFFFFFF23; /* CE1 - 32-bit asynch access after boot*/
  	*(int *)EMIF_CE0 = 0xFFFFFF30; /* CE0 - SDRAM                          */
  	*(int *)EMIF_CE2 = 0xFFFFFF23; /* CE2 - 32-bit asynch on daughterboard */
  	*(int *)EMIF_CE3 = 0xFFFFFF23; /* CE3 - 32-bit asynch on daughterboard */
  	*(int *)EMIF_SDRAMCTL = 0x07117000; /* SDRAM control register (100 MHz)*/
  	*(int *)EMIF_SDRAMTIMING = 0x0000061A; /* SDRAM Timing register        */
}
```
上面的gel来自于CCS v4安装目录下ccsv4\emulation\gel\DSP621x_671x.gel文件，上面不仅使用了#define定义寄存器地址，还使用了类似C中的指针对EMIF（外部存储器接口）进行配置。








```cpp
/******************************************************************/
/* Code Composer Studio supports five reserved GEL functions that */
/* automatically get executed if they are defined. They are:      */
/*                                                                */
/* StartUp()              - Executed whenever CCS is invoked      */
/* OnReset()              - Executed after Debug->Reset CPU       */
/* OnRestart()            - Executed after Debug->Restart         */
/* OnPreFileLoaded()      - Executed before File->Load Program    */
/* OnFileLoaded()         - Executed after File->Load Program     */
/*                                                                */
/******************************************************************/

StartUp()
{
    /* Initialize F2812 memory map */
    GEL_Reset();
    F2812_Memory_Map();
    /* Enable_DFT(); */
    GEL_TextOut("Gel StartUp Complete.\n");
}

OnReset(int nErrorCode)
{
    Enable_DFT();
}

/* commented out to avoid execution
OnRestart(int nErrorCode)
{
}

OnPreFileLoaded()
{
}

OnFileLoaded(int nErrorCode, int bSymbolsOnly)
{
}
*/

menuitem "Initialize Memory Map";

/*------------------- F2812 Memory Map, MPNMC=0 --------------------*/
/*                                                                  */
/*   Note: M0M1MAP and VMAP signals tied high on F2812 core         */
/*                                                                  */
/*   0x000000 - 0x0007ff   M0/M1 SARAM             (Prog and Data)  */
/*   0x000800 - 0x000fff   Peripheral Frame0 (PF0) (Data only)      */
/*   0x002000 - 0x003fff   XINTF ZONE 0            (Prog and Data)  */
/*   0x004000 - 0x005fff   XINTF ZONE 1            (Prog and Data)  */
/*   0x006000 - 0x006fff   Peripheral Frame1 (PF1) (Data only)      */
/*   0x007000 - 0x007fff   Peripheral Frame2 (PF2) (Data only)      */
/*   0x008000 - 0x009fff   L0/L1 SARAM             (Prog and Data)  */
/*   0x080000 - 0x0fffff   XINTF ZONE 2            (Prog and Data)  */
/*   0x100000 - 0x17ffff   XINTF ZONE 6            (Prog and Data)  */
/*   0x3d7800 - 0x3d7fff   OTP                     (Prog and Data)  */
/*   0x3d8000 - 0x3f7fff   FLASH                   (Prog and Data)  */
/*   0x3f8000 - 0x3f9fff   H0 SARAM                (Prog and Data)  */
/*   0x3fc000 - 0x3fffff   XINTF ZONE 7 (MPNMC=1)  (Prog and Data)  */
/*   0x3ff000 - 0x3fffff   BOOT ROM     (MPNMC=0)  (Prog and Data)  */
/*------------------------------------------------------------------*/
hotmenu F2812_Memory_Map()
{
    GEL_MapReset();
    GEL_MapOn();

    /* Program memory maps */
    GEL_MapAdd(0x0,0,0x800,1,1);                  /* M0/M1 SARAM  */
    GEL_MapAdd(0x2000,0,0x2000,1,1);              /* XINTF ZONE 0 */
    GEL_MapAdd(0x4000,0,0x2000,1,1);              /* XINTF ZONE 1 */
    GEL_MapAdd(0x8000,0,0x2000,1,1);              /* L0/L1 SARAM  */
    GEL_MapAdd(0x80000,0,0x80000,1,1);            /* XINTF ZONE 2 */
    GEL_MapAdd(0x100000,0,0x80000,1,1);           /* XINTF ZONE 6 */
    GEL_MapAdd(0x3d7800,0,0x800,1,0);             /* OTP          */
    GEL_MapAdd(0x3d8000,0,0x20000,1,0);           /* FLASH        */
    GEL_MapAdd(0x3f8000,0,0x2000,1,1);            /* H0 SARAM     */

    /* Data memory maps */
    GEL_MapAdd(0x0,1,0x800,1,1);                  /* M0/M1 SARAM  */
    GEL_MapAdd(0x800,1,0x800,1,1);                /* PF0          */
    GEL_MapAdd(0x2000,1,0x2000,1,1);              /* XINTF ZONE 0 */
    GEL_MapAdd(0x4000,1,0x2000,1,1);              /* XINTF ZONE 1 */
    GEL_MapAdd(0x6000,1,0x1000,1,1);              /* PF1          */
    GEL_MapAddStr(0x7000,1,0x1000,"R|W|AS2",0);   /* PF2          */
    GEL_MapAdd(0x8000,1,0x2000,1,1);              /* L0/L1 SARAM  */
    GEL_MapAdd(0x80000,1,0x80000,1,1);            /* XINTF ZONE 2 */
    GEL_MapAdd(0x100000,1,0x80000,1,1);           /* XINTF ZONE 6 */
    GEL_MapAdd(0x3d7800,1,0x800,1,0);             /* OTP          */
    GEL_MapAdd(0x3d8000,1,0x20000,1,0);           /* FLASH        */
    GEL_MapAdd(0x3f8000,1,0x2000,1,1);            /* H0 SARAM     */

    /* Uncomment the map that corresponds to the MPNMC value.     */
    F2812_Boot_ROM_Map();
    /* F2812_XINTF_Zone7_Map(); */
}

/* Map Boot ROM if MPNMC = 0                                      */
F2812_Boot_ROM_Map()
{
    GEL_MapAdd(0x3ff000,0,0x1000,1,0);            /* BOOT ROM     */
    GEL_MapAdd(0x3ff000,1,0x1000,1,0);            /* BOOT ROM     */
}

/* Map External Interface Zone 7 if MPNMC = 1                     */
F2812_XINTF_Zone7_Map()
{
    GEL_MapAdd(0x3fc000,0,0x4000,1,1);            /* XINTF ZONE 7 */
    GEL_MapAdd(0x3fc000,1,0x4000,1,1);            /* XINTF ZONE 7 */
}

/* Enable DFT read/write for SARAM blocks */ 
Enable_DFT()
{
    *0x950 = 0x0300;    /* M0 */
    *0x951 = 0x0300;    /* M1 */
    *0x952 = 0x0300;    /* L0 */
    *0x953 = 0x0300;    /* L1 */
    *0x954 = 0x0300;    /* H0 */
}

menuitem "Watchdog";
hotmenu Disable_WD()
{
    /* Enable WD override */
    *0x7029 = *0x7029 | 0x0068;
    *0x7025 = 0x0055; 
    *0x7025 = 0x00AA;
}

menuitem "Code Security Module"
hotmenu Unlock_CSM()
{
    /* Assumes flash is erased */ 
    *0xAE0 = 0xFFFF; 
    *0xAE1 = 0xFFFF; 
    *0xAE2 = 0xFFFF; 
    *0xAE3 = 0xFFFF; 
    *0xAE4 = 0xFFFF; 
    *0xAE5 = 0xFFFF; 
    *0xAE6 = 0xFFFF; 
    *0xAE7 = 0xFFFF; 

    /* Read the password locations */                
    XAR0 = *0x3F7FF8; 
    XAR0 = *0x3F7FF9; 
    XAR0 = *0x3F7FFA; 
    XAR0 = *0x3F7FFB; 
    XAR0 = *0x3F7FFC; 
    XAR0 = *0x3F7FFD; 
    XAR0 = *0x3F7FFE; 
    XAR0 = *0x3F7FFF;    
}

menuitem "Addressing Modes";
hotmenu C27x_Mode()
{
    AMODE = 0;
    OBJMODE = 0;
}

hotmenu C28x_Mode()
{
    AMODE = 0;
    OBJMODE = 1;
}

hotmenu C2xLP_Mode()
{
    AMODE = 1;
    OBJMODE = 1;
}
```


上面的代码是DSP320F2812的一个GEL文件，上面添加了许多菜单项：



Scripts

- Initialize Memory Map

- F2812_Memory_Map

- Watchdog

- Disable_WD

- Code Security Module

- Unlock_CSM

- Addressing Modes

- C27x_Mode

- C28x_Mode

- C2xLP_Mode

结果的一个截图如下，

![gelè��å��](https://img-my.csdn.net/uploads/201308/02/1375411757_8500.png)







***4. 关于GEL文件中的一些函数***

**StartUp**

启动CCS时执行该函数，此时目标板可能还没有连接，该调用如果有对目标板的操作则会出错。

**OnTargetConnect**

连接目标板（点击Connect Target时）时执行。

**OnReset**

执行Reset命令时执行。

**OnRestart**


执行Restart命令时执行。


**OnFileLoaded**


执行Load File...命令时执行。




***5. CCS自带GEL函数***


参考《手把手教你DSP》，其中提到的函数都能在CCS 3.3中使用，但并不能保证能在更高版本中使用。

关于如何编写设备初始化的GEL，参考文档TI官方文档《Creating Device Initialization GEL Files》，链接为[http://www.ti.com.cn/cn/lit/an/spraa74a/spraa74a.pdf](http://www.ti.com.cn/cn/lit/an/spraa74a/spraa74a.pdf)。

























