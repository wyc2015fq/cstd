# DCDC纹波小实验 - xiahouzuoxin - CSDN博客





2014年08月10日 20:02:14[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：10551








## 关于使用示波器测试纹波的注意事项
- 使用示波器的AC耦合方式测量
- 由于示波器的头套容易引人噪声，因此在测试前必需把探头的头套去掉
- 因为电源的高频噪声很容易通过小电感就可以滤掉，因此更关心的是中低频的噪声。测试时将示波器的带宽限制调到尽可能的低（20MHz），避免从表笔引入噪声（我之前就吃过这方面的亏）

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/DCDC%E7%BA%B9%E6%B3%A2%E5%B0%8F%E5%AE%9E%E9%AA%8C.md#dcdc%E5%90%8E%E7%AB%AF%E6%8E%A5ldo)DCDC后端接LDO

AMS1117-5V的Datasheet上要求的最低压差（VIN-VOUT）为1.1V到1.25V，这就要求输入要大于5V+1.1V=6.1V，如果输入不满足这个条件会怎么样呢？

输入分别使用AC/DC适配器和MP1593输出的5V实验，后端AMS1117的输出都约为4.1V左右，无法稳定要所需要的5V电压。

但输出纹波确实有所改善，MP1593输出5V纹波峰峰值达140mV，经过AMS1117后纹波约40mV（测试未考虑示波器探头的影响）。

我遇到一种情况：使用5V电源给运放或AD等有低纹波要求的芯片供电，为节省能量，常常前端使用DCDC输出5V电压，由于DCDC的输出纹波大，所以有人推荐DCDC后接一块LDO稳定纹波。基于上述的测试结果，像AMS1117这种LDO是远不能满足要求的。

因此，在后端使用LDO滤除DCDC纹波要选择低压差或超低压差的LDO，其输入输出VIn-VOUT最小值在0.1~0.2V最佳，这样输出电压损失最小。

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/DCDC%E7%BA%B9%E6%B3%A2%E5%B0%8F%E5%AE%9E%E9%AA%8C.md#%E9%87%91%E5%8D%87%E9%98%B3wrb0512s-3w2r%E7%BA%B9%E6%B3%A2%E6%B5%8B%E8%AF%95)金升阳WRB0512S-3W2R纹波测试

原本想买金升阳的产品，在淘宝上竟然被骗了（错把MICRODC当成了MORNSUN），买成了WRB0512YS-3W2R。

因为我的输出12V要用于对传感器供电，因此对纹波要求非常高，这里对这两个产品测试对比。

输出滤波使用LC滤波电路，电感取值为1mH，电容取值100uF，测得金升阳的产品纹波dVpp=40mV。另一家的使用50mH电感，电容取值100uF，纹波也才40mV左右，若电感值取1mH，在纹波能达到100mV。

由此可见，金升阳的DCDC产品还是要比一般的DCDC要好一些！

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/DCDC%E7%BA%B9%E6%B3%A2%E5%B0%8F%E5%AE%9E%E9%AA%8C.md#%E4%BD%BF%E7%94%A8%E4%BA%8C%E7%BA%A7lc%E6%BB%A4%E6%B3%A2)使用二级LC滤波

LC电路对滤除纹波的作用很明显，对MP1593输出5V的电路进行测试，在输出端接二级LC滤波前，纹波峰峰值60mV，接二级LC滤波后峰峰值变为10mV。

二级滤波的LC等效电路就是下图，实际测试MP1593时后端电容使用了大电容（100uF）+小电容（0.1uF）并联方式：

![2LC](https://github.com/xiahouzuoxin/notes/raw/master/images/DCDC%E7%BA%B9%E6%B3%A2%E5%B0%8F%E5%AE%9E%E9%AA%8C/2LC.jpg)

但使用LC也需要注意些问题，那就是DCDC的反馈电压（FB）采样点：
- 若采样点在二级LC之前（Pa），则由于经过电感的阻抗作用，负载端电压会有所降低，而且降低值随负载电流变化，因此二级LC的电感值不能太大
- 若采样点在二级LC之后（Pa），这样虽然能得到稳定输出电压，但是这样在电源系统内部引入了一个电感和一个电容，有可能会导致系统不稳定



