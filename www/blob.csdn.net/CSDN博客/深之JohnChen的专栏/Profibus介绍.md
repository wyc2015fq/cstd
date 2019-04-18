# Profibus介绍 - 深之JohnChen的专栏 - CSDN博客

2017年10月05日 23:35:30[byxdaz](https://me.csdn.net/byxdaz)阅读数：3378


一、Profibus概念

PROFIBUS是一种国际化．开放式．不依赖于设备生产商的现场总线标准。PROFIBUS传送速度可在9.6kbaud~12Mbaud范围内选择且当总线系统启动时，所有连接到总线上的装置应该被设成相同的速度。广泛适用于制造业自动化．流程工业自动化和楼宇．交通电力等其他领域自动化。 PROFIBUS是一种用于工厂自动化车间级监控和现场设备层数据通信与控制的现场总线技术。可实现现场设备层到车间级监控的分散式数字控制和现场通信网络，从而为实现工厂综合自动化和现场设备智能化提供了可行的解决方案

PROFIBUS由三个兼容部分组成，即PROFIBUS－DP（Decentralized Periphery）．PROFIBUS－PA（ProcessAutomation ）．PROFIBUS-FMS(Fieldbus Message Specification )。主要使用主-从方式，通常周期性地与传动装置进行数据交换。

1）PROFIBUS–DP: 是一种高速低成本通信，用于设备级控制系统与分散式I／O的通信。使用PROFIBUS－DP可取代办24VDC或4－20mA信号传输

2）PORFIBUS－PA：专为过程自动化设计，可使传感器和执行机构联在一根总线上，并有本征安全规范。

3）PROFIBUS－FMS：用于车间级监控网络，是一个令牌结构．实时多主网络。

二、PROFIBUS协议结构

PROFIBUS协议结构是根据ISO7498国际标准，以开放式系统互联网络（Open SystemInterconnection-OSI）作为参考模型的。该模型共有七层。　（1）PROFIBUS－DP：定义了第一．二层和用户接口。第三到七层未加描述。用户接口规定了用户及系统以及不同设备可调用的应用功能，并详细说明了各种不同PROFIBUS－DP设备的设备行为。　（2）PROFIBUS－FMS：定义了第一．二．七层，应用层包括现场总线信息规范（Fieldbus Message Specification
 - FMS）和低层接口(Lower Layer Interface － LLI)。FMS包括了应用协议并向用户提供了可广泛选用的强有力的通信服务。LLI协调不同的通信关系并提供不依赖设备的第二层访问接口。　（3） PROFIBUS－PA：PA的数据传输采用扩展的PROFIBUS－DP协议。另外，PA还描述了现场设备行为的PA行规。根据IEC1158－2标准，PA的传输技术可确保其本征安全性，而且可通过总线给现场设备供电。使用连接器可在DP上扩展PA网络。　注：第一层为物理层，第二层为数据链路层，第三－六层末使用，第七层为应用层。

三、PROFIBUS–DP、PORFIBUS－PA、PROFIBUS－FMS主要区别

Profibus-DP用于现场层的高速数据传送。主要特点就是高速，速率可达12Mbps,在这一级，中央处理器（如PLC，DCS）通过高速串行线同分散的现场设备（i/0，驱动器、阀门等）进行通讯。

Profibus-PA 适用于Profibus过程自动化。主要特点就是本征安全，通信速率为32.15Kbps，PA 将自动化系统和过程控制系统与压力、温度和液位变送器等现场设备连接起来，并可用来替代4-20mA的模拟技术。

Profibus-FMS的设计旨在解决车间监控级通信任务，提供大量的通信服务。可编程序控制器

（如如PLC，PC机等）之间需要比现场层更大量的数据传送，用以完成中等传输速度进行的循环与非循环的通信服务，但通信的实时性要求低于现场层。但是现在用的非常少。

四、profibus、mpi、modbus异同点

1、MPI是Multi-PointInterface,适用于PLC200/300/400、操作面板TP/OP及上位机MPI/PROFIBUS通信卡，MPI网络的通信速率为网络才支持12Mbit/s的通信速率。MPI网络最多可以连接32个接节点，最大通信距离为50m，但是可以通过中继器来扩展长度。

    PPI协议是专门为S7-200开发的通信协议。S7-200 CPU的通信口（Port0、Port1）支持PPI通信协议，S7-200的一些通信模块也支持PPI协议。Micro/WIN与CPU进行编程通信也通过PPI协议。PPI是一种主从协议，主站、从站在一个令牌网。在一个PPI网络中，与一个从站通信的主站的个数并没有限制，但是一个网络中主站的个数不能超过32个。主站既可以读写从站的数据，也可以读写主站的数据。也就是说，S7-200作为PPI主站时，仍然可以作为从站响应其他主站的数据请求。

    MPI是主站之间的通信；PPI可以是多台主站与从站之间通信。

2、MPI协议：西门子内部协议，不公开；

   PROFIBUS-DP协议：标准协议，公开。

3、MODBUS 是MODICON公司最先倡导的一种软的通讯规约，经过大多数公司的实际应用，逐渐被认可，成为一种标准的通讯规约，只要按照这种规约进行数据通讯或传输，不同的系统就可以通讯。目前，在RS232/RS485通讯过程中，更是广泛采用这种规约。

    常用的MODBUS 通讯规约包括，MODBUS ASCII、MODBUS RTU、MODBUS TCP。

一般，PPI MPI DP物理层都是RS485通讯，只是通讯协议不一样，MODBUS物理层是RS232、RS485、RJ45网卡通讯。

五、PPI、MPI和PROFIBUS转换为以太网通讯

SCANET产品将西门子现场总线PPI、MPI和PROFIBUS转换为以太网通讯，为西门子S7-200/300/400/1200/1500提供业界最全面的以太网通讯解决方案。 产品系列包括SCANET2（精简版）、SCANET3（基本版）、SCANET4（高级版）以及SCANET5（专业版），每个系列包括两个型号：S7PPI和S7MPI。其中S7PPI型号用于S7-200、200SMART通讯；S7MPI型号用于S7全系列PLC及840D数控系统。SCANET也可用于国产兼容西门子控制器，如欧辰、合信、亿维等品牌。 整个系列产品的功能包括基本的上位机组态软件通讯，STEP7编程调试，ModbusTCP通讯、高级语言编程通讯，OPC服务器通讯，扩展通讯口的HMI通讯、Modbus主站和从站通讯、Modbus-ModbusTCP和透明传输，基于互联网的数据订阅和STEP7远程调试等功能。 

六、profibus协议报文格式

        Profibus DP通讯协议是一种单一的、一致性通讯协议，用于所有的工厂自动化和过程自动化。Profibus支持多主站系统，每个主站按时间分配其总线控制权，使用令牌（令牌帧协议）在主站之间传递信息，如果主站获得上一个主站传递来的令牌，则立即有对总线的控制权，当其令牌时间到达，则使用此电文将令牌传递给下一个主站。

·        通信端口参数

![](https://img-blog.csdn.net/20171005233634220)

支持的波特率：

![](https://img-blog.csdn.net/20171005233728388)

·        报文格式

![](https://img-blog.csdn.net/20171005233744009)

3）SD3:固定8字节数字域

![](https://img-blog.csdn.net/20171005233754756)

![](https://img-blog.csdn.net/20171005233805190)


![](https://img-blog.csdn.net/20171005233815102)

![](https://img-blog.csdn.net/20171005233824019)

![](https://img-blog.csdn.net/20171005233834878)

![](https://img-blog.csdn.net/20171005233848775)

![](https://img-blog.csdn.net/20171005233903204)

![](https://img-blog.csdn.net/20171005234154612)

![](https://img-blog.csdn.net/20171005234214134)

![](https://img-blog.csdn.net/20171005234225722)

![](https://img-blog.csdn.net/20171005234239287)

3、详细的报文含义

![](https://img-blog.csdn.net/20171005234255302)

![](https://img-blog.csdn.net/20171005234304843)

![](https://img-blog.csdn.net/20171005234314774)

![](https://img-blog.csdn.net/20171005234339594)

7、开源项目

ProfiM - Profibus Master：[http://profim.sourceforge.net/](http://profim.sourceforge.net/)

