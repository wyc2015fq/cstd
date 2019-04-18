# PreScan第一课：软件简介和基础 - 心纯净，行致远 - CSDN博客





2019年03月13日 23:57:40[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：421








为了自己和他人学习的需要，建了一个PreScan的QQ群：613469333，里面有该软件学习资料，欢迎加入。

**目录**

[1、简介](#1%E3%80%81%E7%AE%80%E4%BB%8B)

[1.1、软件简介](#1.1%E3%80%81%E8%BD%AF%E4%BB%B6%E7%AE%80%E4%BB%8B)

[1.2、功能简介](#1.2%E3%80%81%E5%8A%9F%E8%83%BD%E7%AE%80%E4%BB%8B)

[1.3、 应用举例](#1.3%E3%80%81%20%E5%BA%94%E7%94%A8%E4%B8%BE%E4%BE%8B)

[2、搭建仿真实验基础](#2%E3%80%81%E6%90%AD%E5%BB%BA%E4%BB%BF%E7%9C%9F%E5%AE%9E%E9%AA%8C%E5%9F%BA%E7%A1%80)

# 相关资源下载链接：[PreScan最佳学习资料](https://download.csdn.net/download/zhanshen112/11022206)

# 1、简介

## 1.1、软件简介

PreScan是荷兰国家应用技术研究院衍生出来的TNO公司旗下子公司tass international公司的产品，主要用于驾驶辅助、驾驶预警、避撞和减撞等功能的前期开发和测试。

![](https://img-blog.csdnimg.cn/20190313233355266.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

## 1.2、功能简介

PreScan的工作流程是：1、建立场景 （在PreScan中即可完成）；2、建模传感（在PreScan中即可完成）；3、建模控制算法（在MATLAB/Simulink中可完成）；4、实验仿真（在dSPACE/PreScan等等中完成）。

PreScan可以和CarSim/CarMaker等等构成SIL或者MIL仿真，当连接NI、dSPACE、vector等等硬件时，又可以构成HIL测试。

![](https://img-blog.csdnimg.cn/20190313234137135.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190313234121360.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

 PreScan在开发过程中也遵循“V”型开发流程，包括概念阶段、设计阶段、执行阶段、验证阶段和确认阶段。

![](https://img-blog.csdnimg.cn/20190313234344184.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

## 1.3、 应用举例

1、基于激光雷达的AEBS和CMB(制动减撞系统)；

2、基于相机的LDW；

3、基于相机的的LKA；

4、基于相机的行人检测；

5、基于雷达-相机融合的行人检测；

6、基于相机的交通标识识别（TSR：Traffic Sign Recognition）;

7、原型测试；

8、立体相机深度检测算法；

# 2、搭建仿真实验基础

新建实验-设置地面条件-实验路段和环境的搭建-Path的设置-实验车辆的配置-建模传感-Parse校验和Build构建-3D view-invoke simulink-open cs.mdl。

基本可以分为以上10个步骤，整体来说包括**新建实验场景**、**车辆和传感建模**、**路径选取和运动状态（不同路段的速度、时间）配置、控制算法、仿真调试**这5个步骤。





