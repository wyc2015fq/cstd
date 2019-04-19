# Touchpad驱动分析 - xqhrs232的专栏 - CSDN博客
2010年02月26日 16:08:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：908
原文地址：：[http://blog.csdn.net/gooogleman/archive/2009/03/13/3984257.aspx](http://blog.csdn.net/gooogleman/archive/2009/03/13/3984257.aspx)
1.      接口
WindowsCe.net要求的触摸屏驱动DDI列表如下：
TouchPanelGetDeviceCaps
TouchPanelEnable
TouchPanelDisable
TouchPanelSetMode
TouchPanelReadCalibrationPoint
TouchPanelReadCalibrationAbort
TouchPanelSetCalibration
TouchPanelCalibrateAPoint
TouchPanelPowerHandler
如果在WCESHELLFE_MODULES_TRANSCRIBER或SHELLW_MODULES_CGRTOUCH模块参与编译的时候还需要有如下的DDI扩展接口
TouchReset
       TouchRegisterWindow
       TouchUnregisterWindow
       TouchSetValue
       TouchGetValue
       TouchCreateEvent
       TouchGetFocusWnd
       TouchGetLastTouchFocusWnd
       TouchGetQueuePtr
对于标准的MDD-PDD分层模型，还定义了如下的DDSI接口，用于MDD与PDD之间的连接。
DdsiTouchPanelAttach
DdsiTouchPanelDetach
DdsiTouchPanelDisable
DdsiTouchPanelEnable
DdsiTouchPanelGetDeviceCaps
DdsiTouchPanelGetPoint
DdsiTouchPanelPowerHandler
2.      驱动构成
对于windowsCE.net的典型驱动而言，驱动link了tch_cal.lib和tchmdd.lib两个静态链接库，这两个静态链接库的源代码分别在:
PUBLIC/COMMON/OAK/DRIVERS/TCH_CAL(tch_cal.lib) 该模块负责触摸屏的校准算法。
有意思的是tchmdd.lib则比较特殊，在OAK的lib文件夹中居然找不到这个文件，而在项目文件的OAK目录下该文件确是存在的，也就是说该文件在CESYSGEN的过程中产生。根据该链接库提供的接口来看，可以猜测该lib应当由另外几个零散的lib组成，这些链接库的source文件夹在OAK/COMMON/Drivers/Touch目录下。后来在PUBLIC/COMMON/CESYSGEN下的makefile文件内找到如下内容
tchmdd::
        @set TARGETTYPE=LIBRARY
        @set TARGETNAME=$@
        @set RELEASETYPE=OAK
        @set TARGETLIBS=
!IFDEF SYSGEN_TRANSCRIBER
        @set SOURCELIBS=$(SG_INPUT_LIB)/tchmain.lib $(SG_INPUT_LIB)/tch_trns.lib
        echo touch includes transcriber hooks
!ELSE
        @set SOURCELIBS=$(SG_INPUT_LIB)/tchmain.lib $(SG_INPUT_LIB)/tchbasic.lib
        echo touch is minimal
!ENDIF
确定了Tchmdd.lib的内容就是tchmain.lib+ tch_trns.lib或tchbasic.lib.因为tchmdd需要通过专门的Nmake指令完成生成的动作，所以在Touch驱动目录下都会有一个bat文件用于产生tchmdd.lib文件用于后面的链接库。
之所以需要将tchmdd.lib设定为这种方式生成，而不直接产生可以猜测为不同的编译条件下DDI的接口不同(见第一部分)，所以通过这里来选择不同的DDI接口实现。
这样一来Touch驱动就分成两种不同的类型了:
1.       不使用扩展DDI接口的Touch驱动
2.       在TRANSCRIBER编译条件下产生的带扩展的Touch驱动。

3.      DDI函数概况和DDSI的调用关系：
在分层的软件模型中需要明确的内容有下面几点:
1.       层与层之间的调用接口和调用关系
2.       接口外的参数传递
3.       逻辑时序关系
4.       约束
下图为基本的驱动调用关系模型，Touch驱动由GWES加载，通过DDI调用驱动程序获取设备状态，设置驱动功能等，而驱动本身直接获得硬件信息来确定当前Touch的状态。驱动本身由两个部分组成:MDD和PDD。其中MDD通常无需修改直接使用，该部分提供了面向GWES的DDI的接口，而MDD通过指定的DDSI函数接口调用PDD，这部分为需要由HIV或OEM厂商完成硬件相关的特定部分，也就是我们通常驱动需要实现的部分。PDD部分与MDD部分之间出了DDSI外还需要实现一些指定的变量定义或变量初始化动作，也就是说MDD与PDD之间并不一定是以严格的分层模型来实现，有时候也需要通过共享变量的方式来完成交互。
下面看看接口的情况。关于DDI和DDSI的接口情况第一部分已经做过介绍了，下面需要的是明确接口的触发条件和DDI-DDSI的调用关系。由于DDI有两种实现所以这部分需要分两个不同的情况来看待，在这里我仅仅对基本的Touch驱动的情况进行简单的介绍。
与调用关系相关的还有调用时间逻辑，调用的时间逻辑上决定了公共资源初始化的次序，可用的资源的内容。因此，我们最后还需要以时间为主线。对驱动的调用情况作一个简单的分析。
TouchPanelDllEntry
该函数不是DDI接口函数，但是为Driver DLL的入口函数.也就是在驱动中第一个被调用到的函数。
被调用到的DDSI函数有：
DdsiTouchPanelAttach
DdsiTouchPanelDetach
这两个函数分别在DLL加载和卸载的最早和最晚的过程中被加载。 
TouchPanelEnable
该函数在驱动的中起到初始化的作用。
调用的DDSI函数为
DdsiTouchPanelEnable
DdsiTouchPanelDisable
该函数的执行动作为：
1.       创建事件hTouchPanelEvent，hCalibrationSampleAvailable
2.       互拆量csMutex
3.       校准器初始化
4.       检查并初始化所需的中断gIntrTouch，和gIntrTouchChanged
5.       挂接事件回调函数
6.       关联中断gIntrTouch，gIntrTouchChanged到事件hTouchPanelEvent
7.       创建ISR TouchPanelpISR,并设置ISR的优先级
TouchPanelDisable
该函数与上面提到的TouchPanelEnable执行条件相反，提供停用设备的能力
被调用到的DDSI函数有：
DdsiTouchPanelDisable
该函数的执行动作为：
1.       关闭ISR
2.       停止中断
3.       注销事件及其他同步手段
TouchPanelPowerHandler
该函数在进入或退出PowerOff状态时产生,由于该内容仅仅与PDD相关内容上仅仅是调用
被调用到的DDSI函数有：
DdsiTouchPanelPowerHandler
TouchPanelpISR
该函数在也不是DDI的接口。用于等待和处理触摸屏时间中断，为整个驱动程序提供唯一的事件源。
调用到的DDSI函数为
DdsiTouchPanelGetPoint
该函数实现的内容为：
1.       等待循环，用于接收Touch中断消息，并构成函数的主体
2.       通过DDSI获取当前触摸屏位置和获取信息
3.       在获取有效数据且在校准状态下，收集/递交按下的位置信息
4.       在正常状态下，校准数据(如果PDD已经校准，则无需此步)，并检查校准后数据的有效性
5.       最后调用由GWES传入的回调函数，提交位置信息和状态信息
TouchPanelGetDeviceCaps
该函数为DDI接口函数。用于查询触摸屏设备支持的具体功能。
调用到的DDSI函数为
DdsiTouchPanelGetDeviceCaps
该函数动作为
1.       通过DDSI函数查询相应的信息
2.       当查询屏幕坐标信息时保存屏幕信息，供后面程序映射屏幕坐标
TouchPanelSetMode
该函数同样为DDI接口函数，用于设置触摸屏的工作模式。
调用的PDD函数为DdsiTouchPanelSetMode
该函数的动作为
1.当设置IST优先级时直接通过API完成
2.直接将其他设置转入PDD完成
TouchPanelReadCalibrationPoint
该函数同样为DDI接口函数，在效验交互的过程中读取相应的输入值。直到ISR获取相应的tip事件后，通过传入的指针返回位置信息。
TouchPanelReadCalibrationAbort
该函数在效验取消的时候被调用。仅仅设置状态位和事件后返回。
TouchPanelSetCalibration
该函数为DDI接口函数，为校准器的实现函数之一。该函数用于产生校准参数。在校准UI结束校准动作后将按键数据送至本函数，计算基准参数。
该函数没有所需要的DDSI.
执行的动作并不为逻辑流控，而是一套数学算法。具体内容为:
在触摸屏数据与其位置偏移关系且显示设备像素与其位置偏移关系同为线性关系假设情况下。触摸屏返回的位置信息与像素位置信息之间成2D坐标变换关系.则对于触摸屏按下点TouchCoordinate(Tx,Ty)与其在显示设备位置关系上匹配的点ScreenCoordinate(Sx,Sy)之间的转换关系，可以通过下述坐标变换表示：
Sx =  A1*Tx + B1*Ty + C1
Sy =  A2*Tx + B2*Ty + C2
TouchPanelSetCalibration的具体工作就是通过校准的动作获取的ScreenCoordinate和TouchCoordinate来确定A1，B1，C1,和A2, B2, C2.
对于已知Tx，Ty和Sx,Sy的情况下。把A B C视为未知数来进行求解。由于未知数为两组三个，所以需要用两组，三个方程的方程组来进行求解。
在这里构造矩阵
|∑Tx*Tx  ,∑Tx*Ty  , ∑Tx|  |A1(A2)|     |∑Tx*Sx(Tx*Sy) |
|∑Tx*Ty  ,∑Ty*Ty  , ∑Ty|  |B1(B2)|  =  |∑Ty*Sx(Ty*Sy) |
|∑Tx     ,∑Ty     ,  ∑1 |  |C1(C2)|     |∑Sx(Sy)       |
然后通过克莱姆法则来求解上述方程组。最后获得校准数据，也就是说校准参数就是线形变换的参数。校准数据中的Delta不具备逻辑意义，仅仅是在求解校准数据的时候未除的数字量而已。
在这里比较疑惑的是，上面的行列式对应的方阵不满秩(实际上三个方程的差别仅仅是同乘了一个变量系数而已，这样也能解出A,B,C来的么？)，算法中构造的对称阵事实上使得Deltra必定为0，为什么还能得到非零的求解并得到校准参数。我本人数学功底很差，未能想明白其中的原因。
TouchPanelCalibrateAPoint
该函数为DDI接口函数，目的在于校准输入的坐标信息。
根据TouchPanelSetCalibration内容可知，该函数通过公式
Sx =  A1*Tx + B1*Ty + C1
Sy =  A2*Tx + B2*Ty + C2
完成数据的校准，其中函数中用到的Deltra变量为求方程过程中的遗留因子，之所以保留到这里再作除法运算目的在于减少数据的误差。
到这里已经基本可以看出驱动构成的轮廓，基本上分为几个部分
1．  初始化和关闭流程，这部分内容全世界的软件都有，作用我也不多说了
2．  ISR实体，事实上在整个驱动运作的过程中只有一个部分是真正的系统事件源和信息传递手段，该部分对应的PDD程序，用于获取按键的状态和具体的位置信息，理想状态下来说仅仅要这部分整个驱动就可以工作了。(事实上虚拟Touch驱动就可以这样做)
3．  校准程序集，校准程序在整个Touch驱动中占据很大的比例，使得整个驱动看起来似乎很复杂。由于该部分程序的驱动源为用户对GWES的干涉，该部分不纳入驱动运行时许的过程。所以完全可以丢开这个部分也可以对整个触摸屏驱动看出个大概来。
4.      校准UI的实现与驱动的关系
事实上校准UI对于Touch驱动而言是完全隔离的，UI部分仅仅是实现了显示。逻辑上不参与数据采集的过程。
5.      遗补
在驱动中使用了Mutex作为同步化的手段，事实上使用Mutex是需要进行系统调用的，在这里不使用CriticalSection而使用Mutex的原因我始终没有想明白。如有哪位对此有理解请不吝指教。
在很多平台上Touch控制器与AC97控制器是在同一个CHIP上的，并公用一套寄存器接口。因为Touch驱动和Audio驱动分别在不同的进程空间内，不能使用CriticalSection，这种情况下务必要使用Mutex来保障寄存器操作的原子性。
GWES调用的驱动接口和运行方式与流式驱动有很大不同。除去显示驱动外，这些驱动的PDD实现都很简单，有时候看某些个别驱动程序内容很长，似乎很复杂。其实抛开系统硬件带来的某些繁杂特征，实现这些PDD部分还算是比较容易的。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/gooogleman/archive/2009/03/13/3984257.aspx](http://blog.csdn.net/gooogleman/archive/2009/03/13/3984257.aspx)
