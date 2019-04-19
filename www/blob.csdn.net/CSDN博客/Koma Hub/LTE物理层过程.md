# LTE物理层过程 - Koma Hub - CSDN博客
2019年03月23日 22:17:12[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：73
> 
百度词条：LTE物理层过程
**Table of Contents**
[小区搜索与下行同步](#%E5%B0%8F%E5%8C%BA%E6%90%9C%E7%B4%A2%E4%B8%8E%E4%B8%8B%E8%A1%8C%E5%90%8C%E6%AD%A5)
[上行传输时间的调整与同步](#%E4%B8%8A%E8%A1%8C%E4%BC%A0%E8%BE%93%E6%97%B6%E9%97%B4%E7%9A%84%E8%B0%83%E6%95%B4%E4%B8%8E%E5%90%8C%E6%AD%A5)
[功率控制](#%E5%8A%9F%E7%8E%87%E6%8E%A7%E5%88%B6)
[3.1上行功率控制](#3.1%E4%B8%8A%E8%A1%8C%E5%8A%9F%E7%8E%87%E6%8E%A7%E5%88%B6)
## 小区搜索与下行同步
通过小区搜索的过程，终端与服务小区实现下行信号时间和频率的同步，并且确定小区的物理层ID。
物理层小区搜索的过程主要涉及两个同步信号，即主、辅同步信号（PSS/SSS）。过程中包括了下行时间和频率的同步、小区物理ID的检测和OFDM信号CP长度的检测（Normal或ExtendedCP）。完成这些操作后，终端就可以开始读取服务小区的广播信道（PBCH）中的系统信息，进行进一步的操作。
这期间，在通过同步信号的检测与服务小区获得同步以后，终端可以利用下行导频信号（CRS）进行更精确的时间与频率同步以及同步的维持。小区搜索过程，如图1所示。
![](https://img-blog.csdnimg.cn/20190323215514481.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
## 上行传输时间的调整与同步
通过上行传输时间的调整，终端与服务小区实现上行信号时间的同步，使得不同用户的上行信号同步到达基站。相关过程包括异步随机接入过程中的传输时间调整，以及连接状态下的上行同步保持。
在异步随机接入过程中，作为随机接入的响应消息，基站向终端发送长度为11bit的定时调整命令（TimingAdvanceCommand），终端根据该信息调整上行的发送时间，实现上行同步。
在连接状态下，MAC层的控制信息携带了长度为6bit的定时调整命令，终端将根据该信息对上行的发送时间进行调整，实现上行同步的保持。
定时调整命令的精度是（即15/(15000*2048)），从收到命令到调整后上行发送之间的延时是6ms，即在子帧收到调整命令之后，该信息将终端应用于从子帧开始的上行发送中，如图2所示。
![](https://img-blog.csdnimg.cn/20190323215609179.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
## 功率控制
针对上行和下行信号的发送特点，LTE物理层定义了相应的功率控制机制。
对于上行信号，终端的功率控制在节电和抑制用户间干扰的方面具有重要意义，所以，相应地采用闭环功率控制的机制，控制终端在上行单载波符号上的发送功率。
对于下行信号，基站合理的功率分配和相互间的协调能够抑制小区间的干扰，提高同频组网的系统性能，所以，相应地采用开环功率分配的机制，控制基站在下行各个子载波上的发送功率。
### 3.1上行功率控制
上行功率控制以各个终端为单位，控制终端到达基站的接收功率，使得不同距离的用户都能以适当的功率到达基站，避免“远近效应”。同时，通过小区之间交换干扰情况的信息，进行协调的调度，抑制小区间的同频干扰。
作为上行调度和功率控制的参数，在小区间X2接口上交互的信息有两种。
（1）过载指示（OverloadIndicator，OI）：指示本小区每个PRB上受到的上行干扰情况。相邻小区通过交换该消息了解对方目前的负载情况，并进行适当的调整。
（2）高干扰指示（HighInterferenceIndicator，HII）：指示本小区每个PRB对于上行干扰的敏感度情况。该消息反应了本小区的调度安排，相邻小区通过交换该信息了解对方将要采用的调度安排，并进行适当的调整以实现协调的调度。例如，本小区用于调度小区边缘用户的PRB将对干扰比较敏感，而用于调度小区中心用户的PRB对干扰比较不敏感。
[1](https://baike.baidu.com/item/1)．上行共享信道的功率控制
LTE物理层上行共享信道PUSCH采用部分功率控制（FractionalPowerControl）结合闭环功率控制的方案，对无线链路的大尺度衰落和小尺度衰落进行补偿。在子帧i，终端PUSCH信道的发射功率可以表示为
![P_{PUSCH}(i)=min\left ( P_{CMAX},10lg\left ( M_{PUSCH\left ( i \right )}^{} \right ) \right +P_{O_PUSCH}\left ( j \right )+\alpha \left ( j \right )*PL+\Delta _{IF}\left ( i \right )+f\left ( i \right ))](https://private.codecogs.com/gif.latex?P_%7BPUSCH%7D%28i%29%3Dmin%5Cleft%20%28%20P_%7BCMAX%7D%2C10lg%5Cleft%20%28%20M_%7BPUSCH%5Cleft%20%28%20i%20%5Cright%20%29%7D%5E%7B%7D%20%5Cright%20%29%20%5Cright%20&plus;P_%7BO_PUSCH%7D%5Cleft%20%28%20j%20%5Cright%20%29&plus;%5Calpha%20%5Cleft%20%28%20j%20%5Cright%20%29*PL&plus;%5CDelta%20_%7BIF%7D%5Cleft%20%28%20i%20%5Cright%20%29&plus;f%5Cleft%20%28%20i%20%5Cright%20%29%29)
其中，
（1）PCMAX表示终端的最大发射功率。
（2）MPUSCH(i)表示PUSCH的传输带宽（RB数目）。
（3）![](https://img-blog.csdnimg.cn/20190323220300895.jpg)
，是由高层信令设置的功率基准值。它可以反应上行接收端的噪声水平，针对小区内用户不同类型的上行传输数据包有不同的数值，例如，由PDCCH调度的数据包，没有PDCCH的半静态SPS调度的数据包，根据随机接入响应上行发送的数据包。
（4）表示部分功率控制算法中对大尺度衰落的补偿量。由高层信令使用3bit信息指示本小区所使用的数值。其中，是终端测量得到的下行大尺度损耗。
（5）![](https://img-blog.csdnimg.cn/20190323220336840.jpg)
表示由调制编码方式和数据类型（控制信息或者数据信息）所确定的功率偏移量。它的数值满足
![](https://img-blog.csdnimg.cn/20190323220428898.jpg)
，其中MPR与采用的调制编码方式相关，表示每个资源符号上传输的比特数；=1.25或者0，表示是否针对不同的调制方式进行补偿；而
![](https://img-blog.csdnimg.cn/20190323220458933.jpg)
则表示当PUSCH用于传输控制信息时可能进行的补偿。
（6）是由终端闭环功率控制所形成的调整值，它的数值根据PDCCHformat0/3/3A上的功率控制命令进行调整。物理层有两种闭环功率控制类型——“累计型（accumulation）”和“绝对值型（absolute）”。与上行数据调度相类似，在FDD情况下，PDCCHformat0/3/3A功率控制命令和相应的PUSCH发送之间的时延是4ms；在TDD情况下，该时延的数值根据上下行时间分配比例的不同而有所不同。
定义：终端的功率空间（PowerHeadroom）—作为功率控制过程的参数，物理层对终端剩余的功率空间进行测量，即终端最大发射功率与当前实际发射功率的差值，并上报高层。
[2](https://baike.baidu.com/item/2)．上行控制信道的功率控制
上行控制信道PUCCH采用大尺度衰落补偿结合闭环功率控制的方案。在子帧i，终端PUCCH信道的发射功率可以表示为下式：
![](https://img-blog.csdnimg.cn/20190323220534378.jpg)
其中，
（1）![](https://img-blog.csdnimg.cn/20190323220615672.jpg)
表示终端的最大发射功率。
（2）![](https://img-blog.csdnimg.cn/20190323220635306.jpg)
是由高层信令设置的功率基准值。
（3）是终端测量得到的下行大尺度损耗。与PUSCH不同的是，这里使用了完全功率补偿，即a=1（alpha）；
（4）![](https://img-blog.csdnimg.cn/20190323220744441.jpg)
是根据所承载的CQI和ACK/NACK比特的数目所设置的PUCCH发送功率的偏移量。
（5）![](https://img-blog.csdnimg.cn/2019032322080469.jpg)
表示由PUCCH格式（1/1a/1b/2/2a/2b）所设置的发送功率的偏移量。
（6）![](https://img-blog.csdnimg.cn/2019032322082554.jpg)
是由终端闭环功率控制所形成的调整值。它的功率控制命令由下行调度消息PDCCHformat1/1A/1B/1D/2/2A或者功率控制消息PDCCHformat3/3A所承载。
[3](https://baike.baidu.com/item/3)．SRS的功率控制
除了数据信道和控制信道之外，物理层上行还对SRS的发射功率进行控制，采用了与数据信道PUSCH类似的部分功率补偿结合闭环功率控制的方法。在子帧i，终端SRS的发射功率可以表示为
![](https://img-blog.csdnimg.cn/20190323220918736.jpg)
其中，
（1）  PSRS_OFFSET表示用于SRS的功率偏移，由用户高层信令半静态地进行指示。
（2）MSRS 表示SRS的传输带宽（RB数目）。
（3）其他参数与PUSCH中的定义相同。
详情见：[https://baike.baidu.com/item/LTE%E7%89%A9%E7%90%86%E5%B1%82%E8%BF%87%E7%A8%8B/16844898](https://baike.baidu.com/item/LTE%E7%89%A9%E7%90%86%E5%B1%82%E8%BF%87%E7%A8%8B/16844898)
