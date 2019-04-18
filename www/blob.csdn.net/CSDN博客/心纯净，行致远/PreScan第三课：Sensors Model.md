# PreScan第三课：Sensors Model - 心纯净，行致远 - CSDN博客





2019年03月15日 11:20:51[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：365








为了自己和他人学习的需要，建了一个PreScan的QQ群：613469333，里面有该软件学习资料，欢迎加入。

**目录**

[1 Idealized sensors](#1%20Idealized%20sensors)

[1.1 SELF snesor/GPS receiver 自车/GPS接收器](#1.1%20SELF%20snesor%2FGPS%20receiver%20%E8%87%AA%E8%BD%A6%2FGPS%E6%8E%A5%E6%94%B6%E5%99%A8)

[1.2 AIR sensor  执行器信息接收器](#1.2%C2%A0AIR%20sensor%C2%A0%20%E6%89%A7%E8%A1%8C%E5%99%A8%E4%BF%A1%E6%81%AF%E6%8E%A5%E6%94%B6%E5%99%A8)

[1.3 Antenna&DSRC transmitter/receiver 天线和DSRC的发送器/接收器](#1.3%20Antenna%26DSRC%20transmitter%2Freceiver%20%E5%A4%A9%E7%BA%BF%E5%92%8CDSRC%E7%9A%84%E5%8F%91%E9%80%81%E5%99%A8%2F%E6%8E%A5%E6%94%B6%E5%99%A8)

[1.3.1 Antenna transmitter/receiver 天线发送器/接收器](#1.3.1%20Antenna%20transmitter%2Freceiver%20%E5%A4%A9%E7%BA%BF%E5%8F%91%E9%80%81%E5%99%A8%2F%E6%8E%A5%E6%94%B6%E5%99%A8)

[1.3.2 DSRC transmitter/receiver DSRC的发送器/接收器](#1.3.2%20DSRC%20transmitter%2Freceiver%20DSRC%E7%9A%84%E5%8F%91%E9%80%81%E5%99%A8%2F%E6%8E%A5%E6%94%B6%E5%99%A8%C2%A0)

[1.4 Beacon/OBU  信标（信号塔）/车载单元](#1.4%20Beacon%2FOBU%C2%A0%20%E4%BF%A1%E6%A0%87%EF%BC%88%E4%BF%A1%E5%8F%B7%E5%A1%94%EF%BC%89%2F%E8%BD%A6%E8%BD%BD%E5%8D%95%E5%85%83)

[2 Detailed sensors](#2%20Detailed%20sensors)

[2.1 TIS: Technology Independent Sensor（radar/lidar/ultrasonic）](#2.1%20TIS%3A%20Technology%20Independent%20Sensor%EF%BC%88radar%2Flidar%2Fultrasonic%EF%BC%89)

[2.2 Radar](#2.2%20Radar)

[2.3 Lidar](#%C2%A02.3%20Lidar)

[2.4 Ultrasonic](#2.4%20Ultrasonic)

[2.5 Camera](#2.5%20Camera)

[2.6 Fish eye Camera](#2.6%20Fish%20eye%20Camera)

[3 Ground-truth sensors](#3%C2%A0Ground-truth%20sensors)

[3.1 Depth Camera](#3.1%20Depth%20Camera)

[3.2 Lane maker sensor](#3.2%20Lane%20maker%20sensor)

[3.3 Bounding Rectangle Sensor & Object Camera](#3.3%C2%A0Bounding%20Rectangle%20Sensor%20%26%20Object%20Camera)

相关资源下载链接：[PreScan最佳学习资料](https://download.csdn.net/download/zhanshen112/11022206)

PreScan提供了丰富的传感器模型，包括部分在现实世界不存在的理想传感器，称为真值传感器。整体来说，PreScan包含了三类传感器：
- **Idealized sensors理想传感器：**包括了理论研究需要的传感器，这些传感器有些已经存在，如三雷达、GPS、OBU等等；有些不存在，但是通过现有技术可以做到，只是可能还不存在，比如车联网DSRC和OBU通信。Idealized sensors是三种类型传感器里面唯一支持车联网开发的传感器类型。**由于是理想传感器，支持智能驾驶算法前期开发逻辑的快速验证。**
- **Detailed sensors详细传感器：**真实世界中存在的传感器，有传感建模，考虑了现实世界中传感信息的损失——比如雷达考虑了路径发散衰减、大气衰减和目标反射衰减，相机考虑了畸变、光照等等因素。包含现在市场已经存在的所有传感器类型，为实车试验提供很大帮助。**支持智能驾驶算法研究中期的算法鲁棒性验证，并可以基于Detailed sensors建模真实世界，所得到的传感数据是三种类型传感器中最接近实车试验的传感数据。**
- **Ground-truth sensors真值传感器：**类似于PanoSim，提供一些真值传感器。真值传感器理论上并不存在，但是通过Detailed sensors和传感处理算法可以得到真值传感器的结果，因此可以用于传感器的精确调校，主要是视觉传感器。包括利用视觉检测目标和障碍物距离，车道线，目标检测和分类等等。**支持做智能驾驶前期理论算法的开发。**

# 1 Idealized sensors

主要包含以下几种：
- SELF snesor/GPS receiver 自车/GPS接收器
- AIR sensor( radar/lidar/ultrasonic ) 执行器信息接收器
- Antenna&DSRC transmitter/receiver 天线和DSRC的发送器/接收器
- Beacon/OBU 信标（信号塔）/车载单元

## 1.1 SELF snesor/GPS receiver 自车/GPS接收器

功能：自带的GPS传感器输出本车最精确的GPS位置信息；执行器自带的GPS传感，不能删除。

输出信号：
- Own position 自身位置：XYZ
- Own orientation 自身方向：Rotx,Roty,Rotz
- Own GPS coordinates 自身GPS坐标：lat,long,alt经度纬度高度
- Own velocity 自身速度
- Own heading 自身航向角
- Own Yaw rate 自身横摆角速度

![](https://img-blog.csdnimg.cn/20190315091434461.png)

![](https://img-blog.csdnimg.cn/20190315091452462.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

## 1.2 AIR sensor  执行器信息接收器

AIR：Actor information receiver

功能：快速、简单、通用的目标检测

应用领域：跟踪逻辑研究、简单建模

每个目标的输出信号：
- Distance to target  相对距离
- Azimuth angle (θ) with target  水平角度
- Elevation angle (ϕ) with target  竖直角度
- Absolute velocity of target  目标绝对距离
- Absolute heading of target  目标航向角
- Target ID  目标ID

![](https://img-blog.csdnimg.cn/20190315092110901.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

** 3种不同的可能检测方法：**
- Bounding Box；检测边界
- Center of Bounding Box； 检测中心
- Cebter of Gravity；检测质心

注意：咬合封闭的物体也可以被AIR传感检测到。三种检测方法示意图如下：

![](https://img-blog.csdnimg.cn/20190315092552757.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

**AIR传感的局限：**该传感不考虑重叠、遮挡和目标的实际形状，检测点可能落在检测范围之外。具体应用时应该根据具体情况判断。

## 1.3 Antenna&DSRC transmitter/receiver 天线和DSRC的发送器/接收器

### 1.3.1 Antenna transmitter/receiver 天线发送器/接收器

功能：执行器和目标之间的信息交换；360度环绕。

应用领域：V2X车联网应用

参数确认：
- Position & orientation  位置&方位角
- Channel  传输频道
- Port names  端口名称
- Range (Tx)  传输距离
- Detectable objects (Rx)  可检测的目标数

![](https://img-blog.csdnimg.cn/20190315093304156.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

### 1.3.2 DSRC transmitter/receiver DSRC的发送器/接收器 

> 
DSRC即Dedicated Short Range Communications（专用短程通信技术），是一种应用广泛的专用短程通信技术。在[ETC](https://baike.baidu.com/item/ETC)（Electronic Toll Collection，电子不停车收费系统）系统中，[OBU（](https://baike.baidu.com/item/OBU)On board Unit，车载单元[）](https://baike.baidu.com/item/OBU)采用DSRC技术，建立与[RSU（](https://baike.baidu.com/item/RSU)Road Side Unit，路侧单元[）](https://baike.baidu.com/item/RSU)之间微波通讯链路，在车辆行进途中，在不停车的情况下，实现车辆身份识别，电子扣费，实现不停车、免取卡，建立[无人值守](https://baike.baidu.com/item/%E6%97%A0%E4%BA%BA%E5%80%BC%E5%AE%88/1774380)车辆通道。

在高速公路收费，或者在[车场管理](https://baike.baidu.com/item/%E8%BD%A6%E5%9C%BA%E7%AE%A1%E7%90%86/6025308)中，都采用DSRC技术实现不停车快速车道。自2013年开始，所有的军车都安装OBU，通过DSRC技术实现车辆身份识别。


在PreScan中，也有DSRC短程车联网传感器，车车可根据特定的DSRC通信协议进行数据交换，目前Prescan支持SAE J2735 BSM（basic safety message）协议。

![](https://img-blog.csdnimg.cn/2019031509413468.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

## 1.4 Beacon/OBU  信标（信号塔）/车载单元

功能：当车载装置在beacon检测范围之内，可实现通信；双向通信

应用领域：V2I通信

参数确认：
- Position & orientation 位置&方位角
- Field of view  视野角度
- Range  距离
- Port names  端口名

存在两种可用形式：
- Radio -> no occlusion 不考虑遮挡
- IR -> occlusion possible  考虑遮挡

# 2 Detailed sensors

PreScan的扫描传感器根据不同的频率和领域可以分为：

激光雷达：电磁谱（红外光），波长在10^(-4)~10^(-5);

雷达：一般所指雷达多数为毫米波雷达。电磁谱（微波），波长在10^(-2)~10^(-3);

超声波雷达：声谱（压力波），声波频率在1MHz~100M Hz;

![](https://img-blog.csdnimg.cn/20190315100259433.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

在PreScan中，传感器大多数都相似，只有很小的细节不同。传感器发射出特定的波， 当波遇到目标物体，部分能量被反射回到接收器，接收器分析反射波。

![](https://img-blog.csdnimg.cn/2019031510051228.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190315100611199.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

从上图可以看出，天线Antenna首先将电磁波或者声波（简称信号波）增益之后发送出去，信号波传输过程中由于距离原因在路径上会发散，因此存在**路径发散损损失（loss to path divergence）**，由于存在大气这种介质阻隔，会存在**大气衰减损失（loss due to atmospheric attenuation）**，信号波抵达目标时，并不是所有的信号波都能返回，因此存在**目标反射损失（Reflection loss at target）**，在反射过程中，会再经过**路径发散损损失（loss to path divergence）**和**大气衰减损失（loss due to atmospheric attenuation）**，最后返回的信号波被天线接收到再进行增益放大。

分析上述信号波传输过程，可以看出信号波传输过程中需要经过5个过程的损耗。

上图的最后一段话说的很好：**每一个传感器检测反射波时都存在一个能量阈值，即如果接收到的能量低于该阈值，反射回的的目标就不能被检测到。**

PreScan也可以让用户控制反射率。

![](https://img-blog.csdnimg.cn/20190315101823547.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

主要包含以下几种：
- TIS sensor（radar/lidar/ultrasonic）
- Radar
- Lidar
- Ultrasonic
- Camera
- Fisheye camera

## 2.1 TIS: Technology Independent Sensor（radar/lidar/ultrasonic）

简介：TIS是通用的扫射传感器，模型，可以通过改变参数模拟雷达、激光雷达和超声波雷达。

功能：
- TIS gives realistic radar/laser readings  真实的模拟雷达
- TIS scans the actual geometry of the object  扫描实际形状(so no “bounding-box approach” as with the AIR sensor)
- Occluded objects will not be detected  考虑遮挡(in contrast to the AIR sensor)

注意：AIR sensor不考虑重叠、遮挡和目标的实际形状，检测点可能落在检测范围之外。

参数确认：
- Position & orientation  位置&方位角
- Capture frequency  扫描频率
- Number of beams  波束数
- Beam range  检测距离
- Beam type  波束类型
- Noise & Drift  噪音和漂移

每个目标的输出信号：
- Distance to target  距目标距离
- Velocity relative to target  与目标相对速度
- Detection angles in azimuth (θ) and elevation (φ)  目标的水平竖直角度
- Local incidence angles in azimuth (α) and elevation (β)  水平竖直入射角
- Energy loss figure (dB loss)  能量损失
- Target ID  目标ID

TIS还可以进行分布式计算以节省实验运行时间。

## 2.2 Radar

![](https://img-blog.csdnimg.cn/20190315104426346.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)



![](https://img-blog.csdnimg.cn/20190315104451745.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)



![](https://img-blog.csdnimg.cn/20190315104531762.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

##  2.3 Lidar

![](https://img-blog.csdnimg.cn/20190315104621699.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190315104642748.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

## 2.4 Ultrasonic

![](https://img-blog.csdnimg.cn/20190315104740396.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

## 2.5 Camera

![](https://img-blog.csdnimg.cn/20190315105017444.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

## 2.6 Fish eye Camera

![](https://img-blog.csdnimg.cn/2019031510491042.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

# 3 Ground-truth sensors

主要包含以下几种：
- Depth camera
- Lane maker sensor
- Bounding Rectangle Sensor & Object Camera

## 3.1 Depth Camera

简介：根据距离远近成像，相当于双目相机检测障碍物距离算法对于图像所有像素点的一个遍历。PreScan中介绍这个传感基于不存在的技术（non-existing technology），实际上现在已经有了，封装最好的就是中国经纬恒润独家代理的MobileEye Qx芯片，现在基本在用的都是Q3和Q4。

功能：为双目摄像头调校提供最真实数据。

参数确认Configurable parameters：
- Position & orientation
- Field of view
- Resolution
- Frame-rate
- Range
- Precision

![](https://img-blog.csdnimg.cn/2019031511161396.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

## 3.2 Lane maker sensor

![](https://img-blog.csdnimg.cn/20190315111732347.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

## 3.3 Bounding Rectangle Sensor & Object Camera

![](https://img-blog.csdnimg.cn/20190315111924442.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)



