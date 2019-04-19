# Windows CE下触摸屏驱动实现的分析 - xqhrs232的专栏 - CSDN博客
2010年02月26日 16:29:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：924标签：[windows																[timer																[嵌入式																[interface																[工作																[存储](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=工作&t=blog)](https://so.csdn.net/so/search/s.do?q=interface&t=blog)](https://so.csdn.net/so/search/s.do?q=嵌入式&t=blog)](https://so.csdn.net/so/search/s.do?q=timer&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
本文介绍了Windows CE操作系统的触摸屏驱动程序模型，详细阐述嵌入式系统中电阻式触摸屏的Windows CE驱动程序的设计和实现方法。 
1. 前言 
触摸屏是嵌入式设备中常用的计算机输入设备，它可使操作简单直观，人人都会使用，这一点无论是键盘还是鼠标都无法与其相比。在手机、PDA等手持产品及公共服务设备中大量采用触摸屏。触摸屏分为电阻式、电容式、表面声波式等多种，电阻式触摸屏是目前应用比较广泛的一种，有四线、五线、七线等几种。本文将分析Windows CE操作系统下的触摸屏驱动程序模型及实现方法。
2. Windows CE触摸屏驱动程序模型 
在Windows CE操作系统中触摸屏驱动是一种分层驱动。其驱动模型如图1所示。上层是模型设备驱动程序（Model Device Driver, MDD），下层是依赖平台的驱动程序（Platform. Dependent Driver, PDD）。MDD通常无需修改直接使用，MDD链接PDD层并定义它希望调用的函数接口：设备驱动程序提供器接口（Device Driver Service Provider Interface, DDSI）。同时MDD把不同的函数集提供给操作系统，这些函数叫做设备驱动程序接口（Device
 Driver Interface, DDI），这部分为也就是我们通常驱动需要实现的部分。 
3 Windows CE的触摸屏驱动程序接口 
Windows CE的触摸屏驱动链接了tch_cal.lib和tchmdd.lib两个静态链接库。触摸屏驱动由GWES加载，GWES通过DDI调用驱动程序获取设备状态，设置驱动功能等，而驱动本身通过DDSI直接获得硬件信息来确定当前触摸屏的状态。
Windows CE触摸屏驱动要求的DDI接口包括：TouchPanelGetDeviceCaps、TouchPanelEnable、TouchPanelDisable、TouchPanelSetMode、TouchPanelReadCalibrationPoint、TouchPanelReadCalibrationAbort、TouchPanelSetCalibration、TouchPanelCalibrateAPoint、TouchPanelPowerHandler。
Windows CE触摸屏驱动要求的DDSI接口包括：DdsiTouchPanelAttach、DdsiTouchPanelDetach、DdsiTouchPanelDisable、DdsiTouchPanelEnable、DdsiTouchPanelGetDeviceCaps、DdsiTouchPanelGetPoint、DdsiTouchPanelPowerHandler。
4 Windows CE的触摸屏数据采集 
Windows CE触摸屏驱动程序采用中断方式对触摸笔的按下状态进行检测，如果检测到触摸笔按下将产生中断并触发一个事件通知一个工作线程开始采集数据。同时，驱动将打开一个硬件定时器，只要检测到触摸笔仍然在按下状态将定时触发同一个事件通知工作线程采集数据，直到触摸笔抬起后关闭该定时器，并重新检测按下状态。驱动中采用了触摸屏中断以及定时器中断两个中断源，不仅可以监控触摸笔按下和抬起状态，而且可以检测触摸笔按下时的拖动轨迹。
触摸屏驱动在初始化过程调用TouchPanelEnable函数使能触摸屏。该函数调用的DDSI函数为：DdsiTouchPanelEnable和DdsiTouchPanelDisable。该函数实现如下 内容：
1) 创建事件hTouchPanelEvent和hCalibrationSampleAvailable。hTouchPanelEvent事件在正常状态下当有触摸笔按下或者按下后需要定时采集数据时被触发。而hCalibrationSampleAvailable事件在校准状态下当有校准数据输入时被触发；
2) 检查并初始化所需的中断gIntrTouch（触摸屏中断）和gIntrTouchChanged（定时器中断），并将中断gIntrTouch、gIntrTouchChanged关联到事件hTouchPanelEvent。当gIntrTouch，gIntrTouchChanged中断产生时将触发hTouchPanelEvent事件；
3) 创建一个ISR线程TouchPanelpISR。TouchPanelpISR用于等待和处理触摸屏事件hTouchPanelEvent，它是整个驱动程序中唯一的事件源。
TouchPanelpISR函数是实现触摸屏数据采集关键函数，它实现的内容为： 
1) 等待循环，用于接收hTouchPanelEvent事件，并构成函数的主体； 
2) 通过调用DdsiTouchPanelGetPoint函数获取当前触摸屏位置和状态信息； 
3) 在获取有效数据且在校准状态下，收集并提交按下的位置信息； 
4) 在正常状态下，校准数据，并检查校准后数据的有效性； 
5) 最后调用由GWES传入的回调函数，提交位置信息和状态信息。 
因此，在触摸屏驱动程序中DdsiTouchPanelEnable、DdsiTouchPanelDisable和DdsiTouchPanelGetPoint三个DDSI接口函数是驱动实现的关键所在。
在DdsiTouchPanelEnable和DdsiTouchPanelDisable函数中分别打开和关闭触摸屏硬件，这两个函数其实可以不真正操作硬件，而只是实现软件上的控制，但是为了降低功耗最好在DdsiTouchPanelDisable中将触摸屏控制器电源关闭并在DdsiTouchPanelEnable函数中打开。
在DdsiTouchPanelGetPoint函数中实现对触摸屏数据的采样。从上面的分析得知MDD通过检测hTouchPanelEvent和hCalibrationSampleAvailable事件控制采样，这两个事件被触发都将调用该函数。而这两个事件触发条件有两个：
1) 触摸笔按下时产生触摸屏中断gIntrTouch时触发； 
2) 触摸笔按下后，定时器被打开，定时器将定时产生中断gIntrTouchChanged，并触发事件，直到触摸笔抬起为止。 
因此该函数不仅需要对触摸屏数据采样，而且需要对触发条件进行状态控制，其流程如图2所示。图中定义了三个变量，它们分别为： 
1) TouchIrq为静态变量或全局变量，且初始值为TRUE，该变量必须在触摸屏按下并产生触摸屏中断时设置为FALSE； 
2) InterruptType为静态变量或全局变量，且初始值为SYSINTR_NOP，当在处理触摸屏中断时设置为SYSINTR_TOUCH，在处理定时器中断时设置为SYSINTR_TOUCH_CHANGED，其余设置为SYSINTR_NOP，且在处理完毕后必须将其作为参数传入InterruptDone函数以清除中断；
3) g_NextExpectedInterrupt为静态变量或全局变量，该变量表示下一个希望产生的中断，初始状态为PEN_DOWN，也就是触摸笔在抬起状态，因此希望下一个产生的中断为PEN_DOWN。当触摸屏中断产生以及定时器中断产生时该变量为PEN_UP_OR_TIMER，也就是下一个可能产生的状态为触摸笔抬起状态或者触摸笔按下但定时器中断产生。
DdsiTouchPanelGetPoint函数一开始从触摸笔抬起状态开始执行，此时TouchIrq等于TRUE。如果此时触摸笔按下，将设置TouchIrq为FALSE，表示本次采样是由于触摸屏中断产生并设置下一次调用由定时器产生。然后设置InterruptType状态为SYSINTR_TOUCH，接着开始采集数据并设置g_NextExpectedInterrupt变量为PEN_UP_OR_TIMER，表示下一次产生的中断为定时器中断。接着判断在触摸笔按下状态（g_NextExpectedInterrupt等于PEN_UP_OR_TIMER）下触摸笔是否抬起，如果抬起则设置g_NextExpectedInterrupt为PEN_DOWN恢复到抬起状态。最后通过将InterruptType作为参数传入InterruptDone函数以清除中断。当触摸笔按下，并产生定时器中断时，TouchIrq等于FALSE，此时InterruptType被设置为SYSINTR_TOUCH_CHANGED，其余的动作基本和上面的流程一致。
5 Windows CE下的触摸屏校准 
电阻触摸屏需要校准。应用程序需要一些参考值，以便将接收到的触摸屏坐标数据转换成高层软件所需的屏幕坐标。理想情况下校准程序只要在产品初次加电测试过程中运行一次就可以了，参考值被存储在非易失性存储器中。在理想情况下只需两组原始数据，即在屏幕对角读取的最小和最大值。而在实际应用中，因为许多电阻触摸屏存在显著的非线性，因此如果在最小和最大值之间简单的插入位置数值会导致驱动程序非常的不精确。
在Windows CE中通过在函数DdsiTouchPanelGetDeviceCaps 中设置校准点的个数，在TouchDriverCalibrationPointGet中获取每个校准点的屏幕坐标。常用的校准点数量为5。校准UI将在校准点坐标处相应显示一个十字叉，用户需要精确地在该十字叉位置按下触摸屏，驱动通过TouchPanelReadCalibrationPoint函数读取相应的触摸屏坐标值，然后开始下一个校准点。循环设定的次数后，将采集到的触摸屏坐标值和校准点屏幕坐标送到TouchPanelSetCalibration函数中进行处理。该函数将产生校准基准参数。
TouchPanelSetCalibration函数执行的动作是一套数学算法，具体内容为: 
在触摸屏数据与其位置偏移关系且屏幕像素与其位置偏移关系同为线性关系假设情况下，触摸屏返回的位置信息与像素位置信息之间成2D坐标变换关系。则对于触摸屏按下点的触摸屏坐标(Tx,Ty)与其在显示设备位置关系上匹配的点的屏幕坐标(Sx,Sy)之间的转换关系，可以通过下述坐标变换表示：
Sx = A1*Tx + B1*Ty + C1 
Sy = A2*Tx + B2*Ty + C2 
TouchPanelSetCalibration的具体工作就是通过校准的动作获取的屏幕坐标和触摸屏坐标TouchCoordinate来确定A1，B1，C1和A2, B2, C2。
6. 结束语 
本文作者的创新点：从分析嵌入式Windows CE操作系统中触摸屏驱动程序的模型及实现方法的角度深入剖析了Windows CE中触摸屏数据采集和校准的执行流程，对于类似系统的驱动开发具有一定的借鉴性。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/gooogleman/archive/2009/03/12/3984276.aspx](http://blog.csdn.net/gooogleman/archive/2009/03/12/3984276.aspx)
