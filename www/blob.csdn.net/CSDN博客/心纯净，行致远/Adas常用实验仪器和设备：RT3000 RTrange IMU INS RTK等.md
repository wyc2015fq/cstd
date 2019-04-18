# Adas常用实验仪器和设备：RT3000/RTrange/IMU/INS/RTK等 - 心纯净，行致远 - CSDN博客





2018年05月19日 15:16:22[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：3518








# RTK

（Real - time kinematic，实时动态）载波相位差分技术，是实时处理两个测量站载波相位观测量的差分方法，将基准站采集的载波相位发给用户接收机，进行求差解算坐标。**这是一种新的常用的GPS测量方法**，以前的[静态](https://baike.baidu.com/item/%E9%9D%99%E6%80%81/10643939)、快速静态、动态测量都需要事后进行解算才能获得厘米级的精度，而RTK是能够在野外实时得到厘米级[定位精度](https://baike.baidu.com/item/%E5%AE%9A%E4%BD%8D%E7%B2%BE%E5%BA%A6)的测量方法，它采用了载波相位动态实时差分方法，是GPS应用的重大里程碑，它的出现为工程放样、地形测图，各种控制测量带来了新曙光，极大地提高了外业作业效率。

# **IMU**

惯性测量单元是测量物体三轴姿态角(或角速率)以及加速度的装置。一般的，**一个IMU包含了三个单轴的加速度计和三个单轴的陀螺，加速度计检测物体在载体坐标系统独立三轴的加速度信号，而陀螺检测载体相对于导航坐标系的角速度信号，测量物体在三维空间中的角速度和加速度，并以此解算出物体的姿态。**在导航中有着很重要的应用价值**。IMU中的主要传感器是陀螺仪和加速度计。**

在ADAS中比较常用的是使用IMU输出的**横摆角**（航向角）和**加速度**。



惯性导航应用

一级用途：飞机导航 潜器导航 无人机导航 弹导导弹  汽车导航等。

二级用途：配合收集照相防抖动 等一系列产品

三级用途：手机游戏 VR虚拟 人体姿态等一些列产品

# 陀螺仪、罗经、IMU、MEMS四者的区别：


罗经是航行器用来测量运动方位的，[辅助定位](https://www.baidu.com/s?wd=%E8%BE%85%E5%8A%A9%E5%AE%9A%E4%BD%8D&tn=SE_PcZhidaonwhc_ngpagmjz&rsv_dl=gh_pc_zhidao)的仪器。分为磁罗经和电罗经。磁罗经可以形象的看成[指南针](https://www.baidu.com/s?wd=%E6%8C%87%E5%8D%97%E9%92%88&tn=SE_PcZhidaonwhc_ngpagmjz&rsv_dl=gh_pc_zhidao)，电罗经就要用到用电源驱动的[陀螺仪](https://www.baidu.com/s?wd=%E9%99%80%E8%9E%BA%E4%BB%AA&tn=SE_PcZhidaonwhc_ngpagmjz&rsv_dl=gh_pc_zhidao)。

[陀螺仪](https://www.baidu.com/s?wd=%E9%99%80%E8%9E%BA%E4%BB%AA&tn=SE_PcZhidaonwhc_ngpagmjz&rsv_dl=gh_pc_zhidao)是一种物体角运动[测量装置](https://www.baidu.com/s?wd=%E6%B5%8B%E9%87%8F%E8%A3%85%E7%BD%AE&tn=SE_PcZhidaonwhc_ngpagmjz&rsv_dl=gh_pc_zhidao)。通过对[陀螺仪](https://www.baidu.com/s?wd=%E9%99%80%E8%9E%BA%E4%BB%AA&tn=SE_PcZhidaonwhc_ngpagmjz&rsv_dl=gh_pc_zhidao)双轴基点在不同运动状态下[偏移量](https://www.baidu.com/s?wd=%E5%81%8F%E7%A7%BB%E9%87%8F&tn=SE_PcZhidaonwhc_ngpagmjz&rsv_dl=gh_pc_zhidao)的测量，可以标定出物体水平、垂直、俯仰、[加速度](https://www.baidu.com/s?wd=%E5%8A%A0%E9%80%9F%E5%BA%A6&tn=SE_PcZhidaonwhc_ngpagmjz&rsv_dl=gh_pc_zhidao)、航向方位。

IMU是惯性测量单位。在IMU中包含陀螺仪。

MEMS是[微机电系统](https://www.baidu.com/s?wd=%E5%BE%AE%E6%9C%BA%E7%94%B5%E7%B3%BB%E7%BB%9F&tn=SE_PcZhidaonwhc_ngpagmjz&rsv_dl=gh_pc_zhidao)的缩写。

现代微电子科技不断发展，原有的机械陀螺仪现在可以做的非常小，小到放进手机中使用。
目前中高端[智能手机](https://www.baidu.com/s?wd=%E6%99%BA%E8%83%BD%E6%89%8B%E6%9C%BA&tn=SE_PcZhidaonwhc_ngpagmjz&rsv_dl=gh_pc_zhidao)普遍配备MEMS[惯性测量单元](https://www.baidu.com/s?wd=%E6%83%AF%E6%80%A7%E6%B5%8B%E9%87%8F%E5%8D%95%E5%85%83&tn=SE_PcZhidaonwhc_ngpagmjz&rsv_dl=gh_pc_zhidao)，用于导航、功能控制和游戏。

# INS




INS全称Inertial Navigation System，即[惯性导航系统](https://baike.baidu.com/item/%E6%83%AF%E6%80%A7%E5%AF%BC%E8%88%AA%E7%B3%BB%E7%BB%9F)，有时也简称为惯性系统或惯性导航。惯性导航系统的工作机理是建立在牛顿经典力学的基础上的。牛顿定律告诉人们：一个物体如果没有外力作用，将保持静止或匀速直线运动；而且，物体的加速度正比于作用在物体上的外力。如果能够测量得到加速度，那么通过加速度对时间的连续数学积分就可计算得到物体的速度和位置的变化。
**陀螺仪、IMU、INS区别与联系**



陀螺仪可以测量三轴（roll,pitch,yaw）角运动(角度、角加速度)；

3个单轴陀螺仪+3个单轴加速度计=IMU，称为惯性测量单元；

3个单轴陀螺仪+3个单轴加速度计=INS，称为惯性导航系统；因此可以认为IMU≈INS。



# GPS/INS

（惯性导航系统和GPS系统组合，可输出载体的**位置，速度，时间，姿态角度，航向角度，加速度值，角速度**等）。

主要应用在航空、陆地、海洋导航，跟踪控制，平台稳定，ROV/AGV控制，UAV/RPV控制，精准耕种等。

英国Oxts公司系列产品：RT2000, RT3000, RT4000, Inertial+, RT-Range；

美国GTI公司系列产品：LandMark30/40/50 INS/GPS；

# RT-Range S

**创建追踪多个目标的实时网络，测量距离、方向和相对运动，将目标建模为多点多边形。**

**符合全球 NCAP 标准的目标验证**


RT-Range-S 通过处理多个车辆的距离、方向和相对运动，扩展了我们精确可靠的单辆汽车动态测量。RT-Range S 在全球范围内用于相机、雷达和激光雷达传感器的目标验证，以及制动、车道偏离警告和防碰撞系统的 ADAS 测试。



**车辆-目标之间的测量以及车道位置测量**

RT-Range S 可用于精确测量多达四个目标车辆与受试车辆 (VuT) 或 Hunter 车辆之间的相对运动和距离。在 1 km 以上的距离计算车辆-车辆之间的测量数据。该系统还可以精确地测量相对于车道标记的位置，以及多达 60,000 个测绘特征点，例如交通信号。实时显示测量数据并经由 Hunter 车辆中的 CAN 输出。

**同时测量到四个目标的距离**

RT-Range S 可建立能够追踪多达四个目标，并实时计算距离、碰撞时间和其他相关测量数据的实时网络。目标可以是车辆、软目标、行人模型或附近的物体。




**测量最近点及目标可见性**

绘制 Hunter 和目标车辆的轮廓以创建多达 24 个点的多边形。RT-Range S 可自动计算最近多边形点之间的距离测量数据和目标可见性，这些功能旨在支持停车辅助系统和防碰撞系统测试。

**深受驾驶机器人制造商喜爱的系统**

专用的 AB Dynamics 驾驶机器人接口，具有最大的可靠性、无与伦比的精度和可重复性。

**道路 ADAS 测试**

经由 NTRIP 接收 DGPS 校正以用于在公共道路上进行测试。

**车辆-行人之间的测试**

将 [RT-Backpack](https://www.oxts.com/accessories/rt-backpack/) 配件用于行人携带的背包内完整的交钥匙目标系统。能够追踪行人相对于移动车辆的位置。

**CAN 总线数据**

RT-Range S 系统具备实时 CAN 显示和记录功能。几秒钟内即可将多个车辆的 CAN 数据记录到一个文件。


# RT 3000

适用于动态应用的高性能GNSS/INS，是一个用于汽车动态测量的惯性导航系统。




**精度基准**

通过 **GNSS + 惯性传感器**融合记录完整的 3D 运动和动态轨迹。RT3000 可以低延迟实时提供平稳、稳健和可重复的输出。完美集成的优质 **MEMS IMU 和具备 RTK 能力的 GNSS 接收器**可记录测量数据的详细清单，包括位置、速度、加速度和方向。

**最大可重复性**

执行复杂的测试方案时，例如障碍滑雪车道变换或多车辆 ADAS 测试，可重复性对于恰当验证结果而言至关重要。进行操纵、油门和制动控制的驾驶机器人通常用于消除人类驾驶员的任何潜在变化。RT3000 可以通过以太网与诸如 AB Dynamics 的驾驶机器人连接，从而提供精确的路径跟踪能力。

**符合您的预算和应用**

一系列型号变体和升级可供选择，确保总有一款 RT 产品适合您。当高度动态运动分析比绝对位置精度更重要时，RT3100 系列可提供成本较低的选项。**RT3002 具有厘米级精度的 RTK 功能**。如果您的要求发生变化，可随时添加双天线、GLONASS、CAN 采集和增强输出速率等选项，RT3000 的升级路径可省去重新投资全新系统的烦恼。


**恶劣环境中的可靠数据**

道路测试面临着由于树木遮盖以及诸如桥梁和隧道等障碍物导致城市峡谷中 GNSS 有限的问题。RT3000 的支持技术有助于在所有环境下保持最佳性能。在免费后处理应用 [NAVsolve](https://www.oxts.com/software/rtpostprocess/)中结合前/后处理可将位置漂移降至最低。在卡尔曼滤波器中添加轮速里程计以获得额外的速度更新，从而在静止或缓慢运动时提供帮助。尽可能多地确保您的数据符合我们 gx/ix 紧密耦合技术的规格，以加速 RTK 的重新采集时间，并且即使在较强的多径环境和较低的卫星可见性情况下，也能保持 RTK 级性能。

**一体化、交钥匙解决方案**

一种结合多种功能的工具，无需使用多个单独的设备零部件来使测试复杂化。**RT3000 集成了 GNSS、IMU、星载数据记录、实时处理功能，其 CAN 采集升级成为了车辆其他 CAN 信号（例如转向角度和油门位置）的中央数据采集系统。**







