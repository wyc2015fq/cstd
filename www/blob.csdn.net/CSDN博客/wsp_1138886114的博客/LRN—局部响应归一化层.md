# LRN—局部响应归一化层 - wsp_1138886114的博客 - CSDN博客





2018年09月21日 21:26:22[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：109








LRN全称(Local Response Normalization)。

##### 参数：
- 
norm_region： 选择：**相邻通道间归一化**还是**通道内空间区域归一化**，默认为ACROSS_CHANNELS，即通道间归一化；

- 
local_size：两种表示：

- **通道间**归一化时表示**求和的通道数**；
- **通道内**归一化时表示**求和区间的边长**；默认值为5；
- alpha：缩放因子（详细见后面），默认值为1；
- beta：指数项（详细见后面）， 默认值为5；

##### 局部响应归一化层

完成一种“临近抑制”操作：**对局部输入区域进行归一化**。
- 
在通道间归一化模式中，局部区域范围在相邻通道间，但没有空间扩展（即尺寸为 local_size x 1 x 1）；

- 
在通道内归一化模式中，局部区域在空间上扩展，但只针对独立通道进行（即尺寸为 1 x local_size x local_size）；

- 
每个输入值都将除以$ (1+\frac{ \alpha}{n}\sum_ix_i2)\beta$。


这一层已经被其它种的Regularization技术，如·**drop out, batch normalization**取代了。知道了这些，似乎也可以不那么纠结这个LRN了。



