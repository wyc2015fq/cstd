# 基于LED的可见光通信系统FPGA - 墨流觞的博客 - CSDN博客





2019年03月31日 12:16:53[墨氲](https://me.csdn.net/dss_dssssd)阅读数：26








LED可作为照明；把信号调制到LED所发出的可见光光束上进行无线信号传输。

通信距离2m，what？

参考文章：- 9，室外的远距离通信
- 13
- 18-20:l离线高速可见光通信
- 21-23：FPGA用于系统收发端的信号处理

与WIFI技术相结合，数据下行利用高速的可见光技术，数据上行或者光信号较弱，切换到WIFI连接

光电探测器系统：
- 信号发送端： 信号生成部分(设备： 任意信号发生器、误码仪、FPGA、单片机、DSP开发板等)，信号调制部分，可见光光源信号发生器和信号调制部分整合在同一个电子设备上增加直流偏置
- 信道
- 信号接收端: PIN(光电探测转换器)，后接信号分析解调部分： Matlab，或误码仪、FPGA、单片机、安卓开发板等、

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181228123157566.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

离线可见光通信系统：信号的调制和解调可以异步进行普通电脑利用相关软件生成需要传送的信号数据，将信号拷贝到信号生成器找那个，有信号生成器或驱动电路直接驱动LED发出光信号，光信号经过光学信道传输给PIN，PIN将接收到的光信号转换成电信号存储在示波器中，在将示波器中存储的信号拷贝到普通电脑中，有相关软件中进行信号处理工作、
在线可见光通信：在信号发送端信号发送的同时信号接收端对信号进行还原处理。信号发送端可以使用ＦＰＧＡ、误码仪信号生成器或者其他集成电路如ＤＳＰ开发板等作为信号生成器，个别系统也会使用超级计算机作为信号生成器

可见光光源：功率(光强)和频率响应作出权衡与优化大功率光源一般来说频率响应较差，光强较强；小功率光源恰好相反。

驱动电路的设计也成为高速可见光通信的重点和难点。在线可见光通信系统实现上的另一个难点就是信号调制解调时的同步问题

本论文两类：
- 信源和信宿都采用误码仪的可见光通信实验系统（简称基于误码仪的可见光通信系统）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181228123222656.png)- 利用ＦＰＧＡ实现基带业务处理的可见光通信实验系统（简称基于ＦＰＧＡ的可见光通信系统）![2. 利用ＦＰＧＡ实现基带业务处理的可见光通信实验系统（简称基于ＦＰＧＡ的可见光通信系统）](https://img-blog.csdnimg.cn/20181228123237850.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

信号由误码仪信号生成器生成，信号生成后经过一些ＬＥＤ驱动设备，例如预加重电路，功率放大器等电路处理后传输给ＬＥＤ，ＬＥＤ发送的光信号经过光学信道之后由ＰＩＮ接收，并通过一些电压幅值调整的电信号预处理设备，然后交由误码分析仪进行处理并实时给出误码率，整个通信过程同步进行，特别值得注意的就是误码仪信号生成器和误码分析仪之间的一根物理连接，这个物理连接就是用来同步时钟的。

参考指标：通信速率、通信距离和误码率接收端的PIN对蓝光响应性能好！！

调制解调：OOK调制，PAM，CAP，OFDM **采用OOK调制**

同步时钟编码：跳过了同步时钟问题卧槽！！！

白光LED的发光原理：多晶型白光LED，可以有的

背靠背通信系统：一个数据帧
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181228123320485.png)
奈奎斯特采样定理：当采样频率高于信号频率的2倍及以上时，信号中的信息得以完整保存。

分相过采样：采样时钟相位相差72度。刚好5个采样时钟可以采样一个发送端的信号周期信号缓存池队列：将5个时钟采样dealing的数据位分别存储在5个FIFO中，在整合时利用时钟从5个FIFO中取数据为后进行判决。比如有三个为1，两个为0则判决为1。在作者这里，判决门限与过采样倍数相同。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181228123344657.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181228123354212.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181228123527253.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181228123431990.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181228123539843.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

可见光系统图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181228123551313.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

PIN： 可以选择对不同光敏感的PIN，实验采用的是雪崩二极管
**值得注意的论文：**

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181228123723956.png)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181228123758647.png)

**阵列为人眼识别，通过识别的01数据序列来定义图案吧？？？目标：实现20~30m远距离通信**

误码率： BER bit error rate

文章3：刘景龙等人通过采用强度调制、直接检测的OOＫ（ on-off-keying） 调制方

案， 对不同天气条件下的系统性能、工作区内通信质量进行了仿真

文章1： 2011 叠加编码 30~70m范围接收
文章6： PPM

文章：PPM+同步帧头

基于FPGA的LED交通灯可见光通信系统

实现4m的通信距离，使用更高级的PPM冒充位置调制

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181228155342870.png)

可见光接收检测模块：Ｐ Ｄ Ａ １ ０ Ａ － ＥＣ

**近红外光谱：**

光电模块：光电二极管和放大器 – --200-1100nm的紫外和近红外光谱波段接受特性很好。

可见光被不容易光电二极管检测到，所以在聚光检测器的前端或光收集器上安置非球面聚光透镜

将信号驱动以及检测放大， 达到采集的标准后采用美国ＡＤ 公司的ＡＤ９６５ ０ 宽带模／ 数转换器对其进行处理。

AD9650 数模/模数转换器

自适应算法滤波[8]最优滤波

交通信号灯： AY-200-TYN-2YD 的 200 mm红绿圆灯。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181228160544390.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

##### PPM调制技术











