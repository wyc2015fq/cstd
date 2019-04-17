# 柔性电子：铁电材料， 基于P(VDF-TrFE)纳米线阵列的压电，热电混合纳米能量产生器 - 墨流觞的博客 - CSDN博客





2019年01月08日 21:20:54[墨氲](https://me.csdn.net/dss_dssssd)阅读数：169标签：[柔性电子](https://so.csdn.net/so/search/s.do?q=柔性电子&t=blog)
个人分类：[柔性电子](https://blog.csdn.net/dss_dssssd/article/category/8333955)









![在这里插入图片描述](https://img-blog.csdnimg.cn/20190108212039219.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

压电效应和热释电效应

常用的材料：
- ZnO，
- PZT lead ziconate titanate，
- PMN-PT lead magnesium niobate-lead titanate，

上边都是无机的

下边应该是有机的吧- PVDF及其聚合物 polyvinylidene fluoride or its copolymers

电纺丝工艺能生产P(VDF-TrFE) 但是很复杂且昂贵。

微纳米打印工艺： nanoimprinting。

The freestanding nanowire array was synthesized by nanoimprinting via infiltration of P(VDF-TrFE) polymer melt into the anodic aluminum oxide (AAO) nanochannels and subsequent removal of the template

优点：能通过商业中获取不同大小的纳米模板

### 制备

A， 材料准备

10 wt% 的PVDF溶液，

将P(VDF-TrFE)粉末加入到DMF中，在60摄氏度下搅拌1h。

在Kapton基底上溅射一层Au(使用explorer multi-target sputtering system (Denton Vacuum)),之后在丙酮，酒精，去离子水中清洗，之后干燥。

将溶液spin-coating到Au-Kapton基底上，500r/min  60s

之后再120摄氏度下干燥2h取出DMF溶液。

商用的AAO纳米多孔模板， 400nm孔径，5um孔长

最后导电聚合物PEDOT:PSS 1wt%, 做上表面电极

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190108215319858.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

B, 材料制备
先将P(VDF-TrFE) 旋涂到基底上 4-5um，之后蒸发溶剂DMF。

将AAO模板轻压到P(VDF-TrFE)薄膜上，在170摄氏度下维持1h，比P(VDF-TrFE)的熔点**稍微高一点。**

之后再120度下退火1h得到结晶体，之后**缓慢冷却**到室温。

最后将AAO溶解掉：
- 在2mol/L的CuCl2水溶液和 36wt%的HCl溶液混合溶液下，两者体积比为(2:1), Al的厚度~200um，在5-6min后可完全去除。
- 在4mol/L的NaOH溶液中 30-40min中溶解氧化铝

之后在纳米微阵列上  spin-coater 1000r/min 一层PPMA 防止短路

spinning 一层导电PEDOT：PSS，增加50MV/m的电压来沿垂直方向极化

结构：
- Au/Kapton, which acts as the bottom electrode,
- P(VDF-TrFE) nanowire array as the piezoelectric
- pyroelectric material and PEDOT: PSS conducting polymer as the top electrode, as shown in Fig. 1(e).

### 材料表征

X-RD图像

SEM图像

电压测量： 示波器

电流：low-noise current preamplifier (Model No. SR570, Stanford Research Systems, Inc.).

红外激光器(XI’AN SAPLING LASER)周期性改变温度

thermal infrared imager (FLIR SC7300-M)热红外成像仪记录温度数据

连接10~100兆欧的电阻，测试输出电压

在测量输出时: 用的是水平弯曲和拉直，没在竖直方向上施加形变

测量的变量： 形变大小，形变速度： 没有给出过多的解释
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190111111553830.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
测量热电效应： pyroelectric

激光器周期性改变温度，热红外成像仪实时监测温度。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019011111203717.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
解释：

Once the infrared radiation was switch on, the oscillation of the dipoles in the P(VDF-TrFE) is increased due to the absorption of thermal energy, which causes the net surface polarization at the top electrode to decrease, resulting in a positive pyroelectric output.

Similarly, turning off the infrared laser causes cooling of polarized regions in the P(VDF-TrFE), which leads to an enhanced surface polarization, and is recorded as a negative peak.







