# PreScan第二课：构建实验 - 心纯净，行致远 - CSDN博客





2019年03月15日 08:36:33[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：234








为了自己和他人学习的需要，建了一个PreScan的QQ群：613469333，里面有该软件学习资料，欢迎加入。

**目录**



[1 Conventions坐标系统](#1%20Conventions%E5%9D%90%E6%A0%87%E7%B3%BB%E7%BB%9F)

[2 Roads](#2%20Roads)

[3 Path&trajectories路径和轨迹](#3%20Path%26trajectories%E8%B7%AF%E5%BE%84%E5%92%8C%E8%BD%A8%E8%BF%B9)

[3.1 Path、Trajectory、Route](#3.1%20Path%E3%80%81Trajectory%E3%80%81Route)

[3.2 Path和Trajectory的构建](#3.2%20Path%E5%92%8CTrajectory%E7%9A%84%E6%9E%84%E5%BB%BA)

[4 Actors&objects 执行器&物体](#4%20Actors%26objects%20%E6%89%A7%E8%A1%8C%E5%99%A8%26%E7%89%A9%E4%BD%93)

[5 3DVisView](#5%203DVisView)

# 相关资源下载链接：[PreScan最佳学习资料](https://download.csdn.net/download/zhanshen112/11022206)

# 1 Conventions坐标系统

PreScan有多个坐标系，可以分为以下三种：
- Reference/Global coordinate system 参考/全局坐标系
- Actor/Object coordinate system 执行器/目标坐标系
- Sensor/Light coordinate system 传感器/光坐标系

对应的图示如下：

![](https://img-blog.csdnimg.cn/20190315075344616.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

# 2 Roads

道路的编辑很简单，左键双击可以旋转，按住ctrl还可以拉伸。具体属性可以在各自的property中设置。下面是搭建的一个实例。

![](https://img-blog.csdnimg.cn/2019031507565822.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

相比于PanoSim和CarMaker，PreScan的场景构建简单了很多 ，包括建立内置于道路的Path。

# 3 Path&trajectories路径和轨迹

## 3.1 Path、Trajectory、Route

PreScan提供了Path、Trajectory、Route，这个三个名词有必要区分清楚：

![](https://img-blog.csdnimg.cn/20190315081303440.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

 从上图可以看出，Route是从起点到终点的直线矢量，Path是实际行驶的几何路线，Trajectory是包含了速度参数表的Path。这一点和多数科研论文的定义时相同的。

## 3.2 Path和Trajectory的构建

Path的创建很简单，有三种方式：
- Free drafted 任意手画
- Inherited 根据道路形状（内置于道路）
- GRS

![](https://img-blog.csdnimg.cn/20190315081823278.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

构建好Path之后，只需要拖动Actor到对应的Path上就会出现Trajectory，PreScan默认的速度参数表是15km/h，当然也可以在Experiment tree下找到对应的Speed Profile进行编辑。

![](https://img-blog.csdnimg.cn/20190315082147555.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190315082252157.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

这个很容易理解，毕竟不是平行世界。

Path可以在GUI中定义，但是speed profile可以在GUI和Simulink中定义。

# 4 Actors&objects 执行器&物体

PreScan中的Actors和Objects的种类和区别如下：

Actors:
- Vehicles
- Humans
- Calibration elements 标定物

Objects:
- Infrastructure elements
- buildings
- Nature elements
- Abstract objects

Actors&objects 每一个元素都可以编辑，改变其属性。

![](https://img-blog.csdnimg.cn/20190315083041263.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190315083103293.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190315083123568.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

# 5 3DVisView

这个模块很好用，但是每次调整GUI之后，要Parse-Build之后才会有3D，模块不复杂，每一个试一下就知道对应功能，还可以输出照片和视频，并且可以调整视角，或者增加驾驶人视角，还是很不错的！



