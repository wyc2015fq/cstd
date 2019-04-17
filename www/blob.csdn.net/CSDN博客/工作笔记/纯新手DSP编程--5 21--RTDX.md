# 纯新手DSP编程--5.21--RTDX - 工作笔记 - CSDN博客





2012年05月21日 14:10:03[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5837








1. RTDX 介绍

(1)   Real-Time Data Exchange （RTDX）提供了实时，连续了解目标系统程序运行情况的手段，它允许用户在不干扰目标系统程序运行的情况下，在主机和目标系统之间传送数据。

利用主机提供的COM接口，主机中的其他程序可以获取数据，并以自己需要的形式显示和解释数据。因此有利于用户即使了解目标系统的运行情况，加快开发周期。很多开发系统和应用程序都支持COM接口，例如VB,VC,EXCEL,MATLAB等。或者开发者自己编写的程序，实现分析和可视化。

所谓主机客户程序，就是运行在主机上的利用COM接口向目标应用程序发送数据或从目标程序接受数据的程序。

RTDX 包含2个部件：主机和目标。一个小的软件库运行在目标应用上，目标应用调用该库提供的 API 来从主机接收或者向主机发送数据。这个库使用边界扫描仿真器由JTAG 接口来传输数据。发向主机数据传输是在目标应用运行的同时实时进行的。在主机端，RTDX 主机库和 CCS 协作。数据显示和分析的软件通过一个简单易用的 COM API 来从 RTDX 获得数据。可选的标准工具包括：

National Instruments' LabVIEWQuinn-Curtis' Real-Time Graphics ToolsMicrosoft Excel或者使用你自己开发的 VC++/VB 应用程序。

(2)  中断和查询

C28x, C55x, 和 C6x 设备使用中断驱动的 RTDX 实现，而其他设备使用查询驱动的 RTDX 实现.使用中断驱动实现，目标应用不能调用 RTDX_Poll.

在中断驱动的平台上，中断必须被使能。如 C6000, GIE 必须为 1。推荐使用 .cdb 文件，注册 RTDX_Poll 调用可以自动完成，否则得在中断向量表中手工加入。



2. CCS 3.1 RTDX 配置:

  主菜单-〉Tools-〉RTDX 包含 3 个子菜单：

  (1) Dianostics control : 诊断工具，用于诊断 RTDX 是否工作。

  (2) Configuration control: 配置 RTDX.

    使用该窗口，你可以使能/禁止 RTDX 功能，可以显示/修改 RTDX 的配置。

    注意：

    a. 必须先停止程序运行，RTDX 的配置窗口的 Enable RTDX 复选框才可使用

    b. 必须先不选中 Enable RTDX 复选框 ， RTDX 配置窗口的 Configure 按钮才可使用

    c. 使用 Emulator 时，应先将 CCS 3.1 连接到目标设备。

    配置说明

    RTDX Configuration Page 中

      Data Source : 主机端的程序可以选择从 Log 文件还是从设备获得数据。

      Mode ：

       Non-Continuous Mode 数据被写入 log 文件。你必须指定一个文件。

       Continuous Mode 数据被写入主机内存的循环缓冲区。

    Port Configuration Page 中

       确认使用的 Mode 是 RTDX 即可。

  (3) Channel Viewer control: RTDX 通道显示与控制。

    使用该窗口，你可以看到所有被动态声明的 RTDX 通道，可以 使能/关闭 这些通道。


3. 示例程序

   CCS 3.1 的 example 目录下提供了 RTDX 的例子，包括目标和主机程序。请参考之。  



使用：

首先，包含rtdx.h文件，使用目标库API

申明我们要传递的数据值

申明一个全局通道，程序中向此通道写数据。

主函数中，初始化目标库

使用输出通道前，必须启动数据通道

向输出通道发送数据

状态标记为0，表示写入失败

否则，发送数据，完成后关闭通道

接受类似输入





