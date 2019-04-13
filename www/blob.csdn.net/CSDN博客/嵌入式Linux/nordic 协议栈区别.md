
# nordic 协议栈区别 - 嵌入式Linux - CSDN博客

2017年02月25日 13:48:08[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：1853


S110 指的是从设备的协议栈
S120 指的是主设备的协议栈
S130 SoftDevice是符合蓝牙4.1规范的协议堆栈，包含了所有蓝牙智能协议层直至包含GATT/GAP。它支持多链路中心、周边、观察器和播送器任务、GATT服务器和客户机，以及由事件驱动、异步和线程安全的GATT/GAP和L2CAP API。S130能够支持并行多链路中心和周边任务，使之成为非智能手机或平板电脑的蓝牙智能中心的理想选择。
S130 SoftDevice的其它特性包括无线设备固件升级 (Over-The-Air Device Firmware Upgrade, OTA-DFU) 能力，采用Nordic nRF51系列SoC的闪存架构进行现场产品固件升级。





