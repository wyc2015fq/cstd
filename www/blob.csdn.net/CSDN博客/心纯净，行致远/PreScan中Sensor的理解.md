# PreScan中Sensor的理解 - 心纯净，行致远 - CSDN博客





2019年03月25日 23:04:06[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：54








PreScan中提供的Sensor可以参考[https://mp.csdn.net/postedit/88565400]()，但是一直并没有完全搞清楚每一种传感，直到看了对应的help，果然help是最好的学习资料。能够仅仅依靠help就掌握好一个软件或者方法，是学习能力的体现。

下面谈一下我对sensor的理解。

首先所用的actor都是自带GPS传感器的，也就是SELF-Sensor，可以输出自身对应的信息。如下：

![](https://img-blog.csdnimg.cn/20190325230136522.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)图1 Self-Sensor

然而也并不是所有的东西都会有Self-sensor，只有Actor会有，object没有。这是很重要的一个区分。

![](https://img-blog.csdnimg.cn/20190326074632180.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)图2 actor与object的区别

其次Idealized Sensor提供了AIR Sensor,IR Beacon,IR OBU,RF Beacon,RF OBU五种传感，对应的解释如下：

AIR的意思是actor information receiver，即执行器的信息接收器，主要作用是用来做目标检测，既然是目标检测，确定目标所需要的信息则应该全部具有，如相对距离、角度等等，除此之外，AIR还提供了三种检测方式：BBox，Center，CoGnitive。AIR的缺陷是不考虑重叠遮挡，检测可能落在被检测物体外面。

PreScan中V2X的通信传感之一是使用DSRC通信，通信主要的媒介是Beacon和OBU，Beacon是灯塔，信号塔，OBU是on board unit(车载单元)，两者搭配可进行短距离通信，一般也搭配进行使用。通信的两种主要方式是RF（Radio-frequency：无线电频率）和IR（Infra-red：红外线），两种方式也有对应的优缺点，如下：
|RF|IR|
|----|----|
|fast|slower|
|no obstructed view (no occluded objects)|obstructed view (occluded objects)|

两种方式就对应着Idealized Sensor剩余的传感：IR Beacon，IR OBU；RF Beacon，RF OBU

Detailed Sensor

该类型中包含了实际应用中使用的三雷达（Lidar激光雷达/Radar雷达，一般指毫米波雷达/Ultrasonic超声波雷达）一相机。为了提供超广角的视角，PreScan还提供了鱼眼相机。以上都比较容易理解和使用。值得注意的是PreScan还提供了TIS，TIS是是一种通用的主动扫描传感器，在help中解释为active scanning sensor，TIS可以帮助使用者使用和理解active scanning sensor。它并不是依赖于某一种雷达的技术，而是一种根据这些雷达的通用原则设计的扫描型传感，三种雷达无论是哪一种，本质上都是对周围环境进行扫描，区别在于扫描的方法不同，即在于是使用激光、毫米波还是超声波。因此TIS的全称是Technology Independent Sensor。使用时需要注意TIS的输出很多，最多可以在一束光柱中输出100个目标的数据，根据需要可以输出不同的signal。如下：

![](https://img-blog.csdnimg.cn/20190326081840331.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)图3 TIS signal

GTS（Ground Truth Sensor真值传感）不用多说了，是一种依赖于建模过程的理想传感。

2019.03.26



