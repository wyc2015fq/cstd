# 柔性电子  热电效应: 塞贝克(seebeck  )效应 帕尔贴(Peltier)效应和 汤姆逊(Thomson )效应 - 墨流觞的博客 - CSDN博客





2018年11月28日 23:53:16[墨氲](https://me.csdn.net/dss_dssssd)阅读数：800








本文主要讲述热电效应的三个基本原理：
- 1 塞贝克效应 seebeck effect
- 2 帕尔贴效应 Peltier effect
- 3 汤姆逊效应 Thomson effect
- 4 热点转换效率$\eta$与热电优值$ZT$的关系

ZT (thermoelectric figure of merit) 热电优值

### 1. 热电效应的三个原理

1.1 塞贝克效应

在热电材料中，由自由电子或空穴携带电荷和热量。热电半导体材料中的电子和空穴表现的像带电粒子的气体，如果一个正常的（不带电的）气体放置在一个温度梯度的盒体内，一端为冷而另一端热，则热端的气体分子的运动比冷端更快，更容易扩散，因此冷端分子密度很高，密度梯度将驱散分子扩散回热端。在稳定状态下，密度梯度的影响将完全抵消温度梯度的影响，因此不存在分子的净流量。如果分子带电，冷端电荷的集聚也会产生一个排斥静电场(电势)，将电荷推回到热端。由温差产生的电势(电压)称为塞贝克效应。
**在这里就是电压和密度共同的作用与温差作用相抵消**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181128214710442.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

塞贝克效应电势差的计算公式：
$$V=\int_{T_1}^{T_2}(S_B(T)-S_A(T)), {\rm d}T$$
其中，$S_A和S_B$分别为两种材料的塞贝克系数，T为温度。如果$S_A与S_B$不随温度变化，则上式可简化为：
$$V=(S_A - S_B)(T_2 - T_1)$$

1.2 帕尔贴效应

帕尔贴发现电流会在两种不同金属的结合处产生加热或冷却。1838年， Lenz表明， 根据电流的方向，热量可以从**金属**的交汇处散去从而将水结成冰，也可以通过逆电流， 在交汇处产生热量来融化冰。在金属交汇处吸收或产生的热量与电流成正比，这种现象称为帕尔贴效应。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181128215605295.png)

吸收或释放的热量满足一下关系：
$$Q=(\pi_X - \pi_Y)I$$,其中，$\pi_X和 \pi_Y$分别是导体X和导体Y的帕尔贴系数

1.3 汤姆逊效应

二十年后，汤姆逊发表了对塞贝克和帕尔贴效应的全面解释， 并描述了他们之间的相互关系（称为凯尔文关系），

$$帕尔贴系数=塞贝克系数\times绝对温度$$

,简单推导如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181128221213711.png)
假设交汇处的电阻为$R$,则：$$V=IR \approx ST$$
$$Q=I^2R\approx \pi I=V I=STI$$

,所以 $\pi = ST$
这种热力学推导导致了汤姆逊预测了第三种热点效应，汤姆逊效应。在汤姆逊效应中，当电流流过具有温度梯度三维材料时，热量被吸收或者产生，热量与电流和温度梯度成正比，比例常数为汤姆森系数。

以$Je$表示电流密度，在单位时间内，单位体积的导体放出的汤姆热为：$$qT=-\tau Je \cdot \Delta T$$

式中$\tau$为汤姆逊系数，它与温度和材料性质有关

### 2. ZT

Thermoelectric efficiency($\eta$)与材料品质因数($ZT$)直接相关：

**ZT值，又叫热电优值**（thermoelectric figure of merit）。它是衡量热电材料热电性能的指标和量度，Z是材料的热电系数（单位是/k），有量纲，T是热力学温度,单位是k。ZT乘积来表示热电性能的高低（ZT值越高，热电性能越好）[ZT值（热电优值）怎么换算成转换效率？](https://www.zhihu.com/question/20895956)

$$\eta = \frac{T_h - T_c}{T_h}\left[ \frac{\sqrt{1 + ZT_{avg}} -1 }{ \sqrt{1 + ZT_{avg}}  + \frac{T_c}{T_h} }\right]$$

$T_c和T_h$是冷端和热端的温度，$T_{avg} =\frac {T_c +T_h}2$, $ZT_{avg}$通过对$T_c和T_h$之间所有的峰值ZTs积分获得，在特定温度$T$下,材料的$ZT$:
$$ZT=\alpha^2\sigma T / \kappa$$
$\alpha$是Seebeck coefficient， $\sigma$是导电率， $\kappa$是材料的热导率，$\kappa$有两部分构成，载流子(charge-carriers $\kappa_e$)和晶格(lattice, $\kappa_l$)






