# 如何编译WinCE Bluetooth Stack并打开的默认LOG - xqhrs232的专栏 - CSDN博客
2015年07月15日 13:56:30[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：564
个人分类：[Bluetooth技术](https://blog.csdn.net/xqhrs232/article/category/2908395)
原文地址::[http://www.cnblogs.com/strive-forever/archive/2011/09/18/2180354.html](http://www.cnblogs.com/strive-forever/archive/2011/09/18/2180354.html)
相关文章
1、[如何分析HCI数据](http://www.cnblogs.com/strive-forever/archive/2011/09/18/2180295.html)----[http://www.cnblogs.com/strive-forever/archive/2011/09/18/2180295.html](http://www.cnblogs.com/strive-forever/archive/2011/09/18/2180295.html)
2、[如何获取并分析L2CAP包](http://www.cnblogs.com/strive-forever/archive/2011/11/12/2246766.html)----[http://www.cnblogs.com/strive-forever/archive/2011/11/12/2246766.html](http://www.cnblogs.com/strive-forever/archive/2011/11/12/2246766.html)
3、[SDP协议译稿（Part
 1）](http://www.cnblogs.com/strive-forever/archive/2011/11/04/2236640.html)----[http://www.cnblogs.com/strive-forever/archive/2011/11/04/2236640.html](http://www.cnblogs.com/strive-forever/archive/2011/11/04/2236640.html)
4、[如何： 使用 BTDC 工具来调试 Windows CE 中的蓝牙无线技术](http://blog.csdn.net/xiaolei05/article/details/6343401)----[http://blog.csdn.net/xiaolei05/article/details/6343401](http://blog.csdn.net/xiaolei05/article/details/6343401)
### **1 WinCE Bluetooth Stack在哪里**
以WinCE6.0为例，在文件夹/PRIVATE/WINCEOS/COMM/BLUETOOTH下。
### **2 如何编译**
**编译源码**
从VS2005中进入命令行窗口，切换到/PRIVATE/WINCEOS/COMM/BLUETOOTH下，并执行Build –c（*地球人都知道，呵呵*）。
**编译Dll**
切换到PUBLIC/COMMON/CESYSGEN路径下（*makefile就在这里，有兴趣可以看一下*），运行sysgen btd –p，等待编译结束即可。
### **3 如何打开协议栈的LOG**
最好的调试方法，莫过于编译一个Debug Version的协议栈代码，然后通过如下注册表配置的方式打开相关的Debug Zone来进行分析。
[HKEY_LOCAL_MACHINE/Software/Microsoft/Bluetooth/Debug]
"Mask"=dword:3 // file&uart
"Console"=dword:7fffffff // open all debug zone
有关Mask和Console的配置可以参考public/common/oak/inc/bt_debug.h中的定义，这里简单的列举一部分Debug Zone的定义：
#define DEBUG_ERROR 0x00000001
#define DEBUG_WARN 0x00000002
#define DEBUG_OUTPUT 0x00000004
#define DEBUG_VERBOSE 0x00000008
#define DEBUG_SHELL_ALL 0x00000030
#define DEBUG_SHELL_INIT 0x00000010
#define DEBUG_SHELL_TRACE 0x00000020
#define DEBUG_HCI_TRANSPORT_ALL 0x000000c0
#define DEBUG_HCI_DUMP 0x00000040
#define DEBUG_HCI_TRANSPORT 0x00000080
#define DEBUG_HCI_ALL 0x00000f00
#define DEBUG_HCI_INIT 0x00000100
#define DEBUG_HCI_TRACE 0x00000200
#define DEBUG_HCI_PACKETS 0x00000400
#define DEBUG_HCI_CALLBACK 0x00000800
#define DEBUG_L2CAP_ALL 0x000f0000
#define DEBUG_L2CAP_INIT 0x00010000
#define DEBUG_L2CAP_TRACE 0x00020000
#define DEBUG_L2CAP_PACKETS 0x00040000
#define DEBUG_L2CAP_CALLBACK 0x00080000
#define DEBUG_RFCOMM_ALL 0x00f00000
#define DEBUG_RFCOMM_INIT 0x00100000
#define DEBUG_RFCOMM_TRACE 0x00200000
#define DEBUG_RFCOMM_PACKETS 0x00400000
#define DEBUG_RFCOMM_CALLBACK 0x00800000
#define DEBUG_SDP_ALL 0x0f000000
#define DEBUG_SDP_INIT 0x01000000
#define DEBUG_SDP_TRACE 0x02000000
#define DEBUG_SDP_PACKETS 0x04000000
#define DEBUG_SDP_CALLBACK 0x08000000
#define DEBUG_TDI_ALL 0x00f00000
#define DEBUG_TDI_INIT 0x00100000
#define DEBUG_TDI_TRACE 0x00200000
#define DEBUG_TDI_PACKETS 0x00400000
#define DEBUG_TDI_CALLBACK 0x00800000
#define DEBUG_PAN_ALL 0x0000f000
#define DEBUG_PAN_INIT 0x00001000
#define DEBUG_PAN_TRACE 0x00002000
#define DEBUG_PAN_PACKETS 0x00004000
#define DEBUG_PAN_CALLBACK 0x00008000
#define DEBUG_AVDTP_ALL 0xf0000000
#define DEBUG_AVDTP_INIT 0x10000000
#define DEBUG_AVDTP_TRACE 0x20000000
#define DEBUG_AVDTP_PACKETS 0x40000000
#define DEBUG_AVDTP_CALLBACK 0x80000000
#define OUTPUT_MODE_DEBUG 0
#define OUTPUT_MODE_CONSOLE 1
#define OUTPUT_MODE_FILE 2
OK，现在就可以来分析和调试协议栈的代码了，至于分析到什么程度，就看对协议的理解了，呵呵。
附带说一句，Debug Version的协议栈可以编译到Release Version的Image中调试起来更加方便。
