# 原创翻译 通过拨号上网的Windows CE6.0 GPRS连接 - xqhrs232的专栏 - CSDN博客
2013年02月05日 10:31:34[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：771
原文地址::[http://daimajishu.iteye.com/blog/1089191](http://daimajishu.iteye.com/blog/1089191)
**介绍**Windows CE提供两种方式与GPRS调制解调器连接：使用"cell core"函数和使用"拨号上网"。因为大多数的GPRS调制解调器不支持前一种方式需要的无线接口层（Radio Interface Layer, RIL）驱动，所以拨号上网是常被使用的方式。
本文阐述了怎样在Windows CE 6.0设备上通过拨号上网来使用GPRS调制解调器。同时也对在实施这一方法时会碰到的典型问题给出了解决方法。
**GPRS连接**GPRS系统通常被称为2.5G－一种介于第二代和第三代移动通信之间的技术。GPRS通过使用未用的TDMA信道（例如在GSM系统中的）来提供中等速度数据传输。GPRS基于包交换（packet switching）方式，替代了早期的电路交换（circuit switch）数据连接技术。它可以提供最高50Kbits/sec的上下行速度。GPRS最近也开始被一些以兆位/秒级别为单位传输数据的高速传输技术逐渐替代，如EDGE，UMTS等。然而在一些发展中国家，如印度等，GPRS仍然是移动设备传输数据的主要方式。
**GPRS调制解调器和对RIL的需要**调制解调器最近已经变得非常复杂，特别是当需要一个内部多路复用器和支持它的设备驱动时。这些是必须的，因为现在的蜂窝网络调制解调器必须支持多个接口，如USB、共享内存和UART等。当调制解调器只有一个简单的UART接口，如果此接口被配置用于数据传输，那么就没有办法通过这个接口用AT命令来获得状态等信息。例如有的调制解调器需要在GPRS模式下断开数据连接并开始语音连接来进行通话。最近的调制解调器开始倾向于为每一种服务都提供一个端口来支持语音、短消息和数据传输的同时使用。调制解调器内部有一个多路复用器来处理此工作。为了让开发人员不用考虑用于处理多路复用等的复杂的AT命令，微软提出了用无线接口层（Radio
 Interface Layer-RIL）驱动来和无线部分的硬件交互。
RIL提供了代理设备或者COM端口来处理调制解调器内的每个单个设备。图1显示了RIL为应用程序开发人员提供了怎样的代理设备。当用户需要同时使用语音、短信和数据传输并且调制解调器支持这些功能时，这种处理方式就变得必须了。一个RIL驱动原则上可以支持一个代理或者虚COM端口（virtual COM port）来处理语音、数据和短信；然而实践上，一个RIL可以仅支持单个COM端口来和调制解调器接口。
![](http://download.winitpro.com.cn/imgs/Embedded_bbs/11060201.jpg)
图1：RIL为应用程序开发人员提供的代理设备
**RIL vs 拨号上网**有些Windows CE移动设备，如手持式POS终端（销售终端），不需要语音或者短信连接；它们仅需要通过蜂窝网络的数据连接来从远端服务器获得数据或者把数据写入远端服务器。当一个移动设备需要GPRS连接时，有两种方式可以管理调制解调器接口：
1)通过RIL连接
2)通过拨号上网的方式连接
由于大多数的GPRS调制解调器没有RIL驱动，所以常常必须使用拨号上网的方式。如果调制解调器只用于数据传输而不用语音和短信，或者要重复地获取连接状态时，那么最好使用Windows CE的标准拨号上网功能。这是因为应用程序只需要一个串口连接，而不需要用到调制解调器的一些特殊功能。
![](http://download.winitpro.com.cn/imgs/Embedded_bbs/11060202.jpg)
图2：通过拨号上网的连接
本文分析了用Windows CE的标准调制解调器拨号上网方式建立GPRS连接时需要的步骤和可能会遇到的问题。达到此目的的过程包含四个步骤：
1)修改源代码
2)设置注册表
3)编译选项和OS design组件的选择
4)连接GPRS时的运行时工作
**修改源代码**GPRS连接需要一个Windows CE层的驱动组件。通用调制解调器（Unimodem）是一个标准的19200波特率的调制解调器驱动，用于通过串口和GPRS调制解调器连接。此驱动和PC的标准19.2Kbps串口调制解调器驱动一致。Unimodem驱动支持GSM调制解调器，即支持"Hayes兼容"调制解调器"AT命令"和数据传输。
Unimodem驱动是Windows CE驱动包的一部分，Unimodem驱动的源代码也可以得到。此驱动不需任何修改就应该可以正常工作，并应该支持连接到串口上的任何串行56Kbps的拨号电话调制解调器。然而，当一个GPRS设备连接到串口上时，Unimodem看上去不是像支持过去的拨号调制解调器那样也支持新的GPRS调制解调器。
当使用GPRS时会出现计时上的问题。这是因为GSM/GPRS调制解调器的命令处理要慢于通常的PSTN（有线）拨号调制解调器，这导致了更长的时间响应。其后果是在初始化阶段可能会出现命令响应的超时，从而导致不能建立连接。
Unimodem驱动里有一个看门狗计时器线程来维护计时器。驱动在看门狗计时器超时之前期望得到"OK"响应。不幸的是，在GPRS的情况下，计时器会超时。
这样，当在连接阶段发出一个AT命令时，Unimodem驱动在收到期望的"OK"响应之前就判断处理过程超时。为了解决这个问题，需要对Unimodem驱动做以下改动：
将#define定义的INIT_WATCHDOG_TIMEOUT参数改为原有值的10倍。
**注册表设置和修改**为了成功地建立连接，和GPRS连接相关的注册表项必须要正确设置。这部分将讨论一些重要的注册表项。
刚才我们已经提到GPRS调制解调器的慢速时间响应导致了Unimodem驱动的看门狗计时器问题。有些超时会导致Unimodem驱动重启调制解调器。这可以通过以下注册表设置来改变。
在platform.reg文件的\Unimodem\Setting项中，注册表键"Reset"="ATZ"会导致调制解调器的重启。为了禁止此情况的发生并让调制解调器保留上次的设置，需要使用以下的设置："Reset"="AT"。以下是从platform.reg文件中摘取的片断：
;**********************OverridingUnimodemsettings***********************
[HKEY_LOCAL_MACHINE\Drivers\Unimodem\Init]
"1"="AT<cr>"
"2"="ATE0V1&C1&D2<cr>"
[HKEY_LOCAL_MACHINE\Drivers\Unimodem\Settings]
"Prefix"="AT"
"Terminator"="<cr>"
"DialPrefix"="D"
"DialSuffix"=";"
"Pulse"="P"
"Tone"="T"
"Blind_Off"="X4"
"Blind_On"="X3"
"CmdSendDelay"="500"
"CallSetupFailTimeout"="ATS7=<#><cr>"
"Reset"="AT<cr>"
[HKEY_LOCAL_MACHINE\Drivers\Unimodem\Config]
"CallSetupFailTimer"=dword:B4
<!--{13080187362180}-->注册表设置带来的另一个问题和拨号号码及其相关细节内容有关。
当组件被选择后，缺省的注册表设置的国家代码和地区代码被初始化，但这很可能和你的应用程序要求不一致。这将导致拨号时使用不正确的号码。显然要解决此问题必须要改变注册表设置。新的设置将取代缺省的注册表设置，并只能接受自己设置的号码。对于GPRS, *number#被用于建立连接。
在platform.reg文件中，必须要注意确保输入正确的号码。以下是platform.reg中的示例片断：
************************OverridingDialingSettings***********************
[HKEY_CURRENT_USER\ControlPanel\Dial]
;当前位置的位置ID（值名）
"CurrentLoc"=dword:0
;当生成一个新项时用到的下一个永久位置ID
"HighLocID"=dword:2
;缺省位置设置，需要被本地化
[HKEY_CURRENT_USER\ControlPanel\Dial\Locations]
;每个MultiSz位置(值名=PermLocationID"包含有：
;LocationName,LocalFmt,LdFmt,IntntlFmt,AreaCode,CwCode,CountryCode,&Options
"0"=multi_sz:"Work",
"G",
"G",
"G",
"425",
"",
"1",
"0"
"1"=multi_sz:"Home",
"G",
"1FG",
"011,EFG",
"425",
"",
"1",
"0"
;*****************************************************************
<!--{13080187362181}-->**编译选项和OS Design组件选择**编译"GPRS Connection through Dial Up and networking"时没有特别的编译选项，但是必须加入以下组件才能正确编译：
Communication services and Networking->Networking Wide Area Network (WAN)
Dial Up Networking(RAS/PPP) (包括所有子组件)
Telephony API(TAPI 2.0) (包括所有子组件)
**通过拨号上网连接GPRS时的运行时步骤**
此部分详述了通过Windows CE的拨号上网功能连接GPRS时的有关步骤。一些屏幕截图被用来说明此过程。
由于所有的拨号选项都在注册表里有设置，所以在当点击连接属性时，弹出的拨号向导的拨号属性中没有太多需要修改的。
图3显示了拨号属性的设置：
![](http://download.winitpro.com.cn/imgs/Embedded_bbs/11060203.jpg)
图3. 拨号属性
在控制面板里选择网络和拨号向导。点击"创建新连接"。输入连接的名称，例如"GPRS"。选择连接类型为拨号连接。点击"下一步"继续。图4显示了新建连接向导：
![](http://download.winitpro.com.cn/imgs/Embedded_bbs/11060204.jpg)
图4. 创建新连接
在"选择调制解调器"选项里选择"COM1上的Hayes兼容"做为调制解调器驱动，并点击"配置"按钮。图5显示了此步骤。
![](http://download.winitpro.com.cn/imgs/Embedded_bbs/11060205.jpg)
图5. 选择调制解调器
接下来"设备属性"对话框会被打开，端口设置页面里的内容不需要改动，点击"呼叫选项"页面。图6显示了这些页面。
![](http://download.winitpro.com.cn/imgs/Embedded_bbs/11060206.jpg)
图6. 设备属性
在"呼叫选项"里的文字输入框里输入特殊的初始化命令，如图7所示。此处的命令是AT+CGDCONT=1,"IP","airtelgprs.com"。这里的AT命令根据不同的服务提供者而有所不同，所以请和实际的服务提供者确认来设置正确的命令。
![](http://download.winitpro.com.cn/imgs/Embedded_bbs/11060207.jpg)
图7. 呼叫选项
点击"下一步"并在电话号码对话框里输入号码*99#。图8显示了电话号码对话框。点击"完成"按钮。
![](http://download.winitpro.com.cn/imgs/Embedded_bbs/11060208.jpg)
图8. 电话号码
新的"GPRS"连接将出现在网络和连接向导里。图9显示了这个新连接。
![](http://download.winitpro.com.cn/imgs/Embedded_bbs/11060209.jpg)
图9. 网络连接
右击"GPRS"图标并点击"连接"来连接到GPRS。图10显示了此步骤，图11显示了设备已连接到GPRS网络。
![](http://download.winitpro.com.cn/imgs/Embedded_bbs/11060210.jpg)
图10. 连接到GPRS
![](http://download.winitpro.com.cn/imgs/Embedded_bbs/11060211.jpg)![](http://download.winitpro.com.cn/imgs/Embedded_bbs/11060212.jpg)
图11. 设备已连接到GPRS网络
**总结**Windows CE拨号接入网络组件和Unimodem驱动可以被有效地利用来支持基于Windows CE 6.0的系统和设备通过GPRS调制解调器传输数据。
