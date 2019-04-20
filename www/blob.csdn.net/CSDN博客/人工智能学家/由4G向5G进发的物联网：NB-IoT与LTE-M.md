# 由4G向5G进发的物联网：NB-IoT与LTE-M - 人工智能学家 - CSDN博客
2019年01月27日 21:54:21[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：280
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/pEr7SdGKoDBHMicVXMedHeOStTibYz82brA9N2aPz8gmsdTv1rH3pokKUFUQ5tFjjP5f0I4J5Cia1CIJkjnFgtxPg/640?wx_fmt=jpeg)
来源：北京物联网智能技术应用协会
2019年1月16号广东联通联合中兴通信在深圳5G规模测试外场，打通了全球第一个基于3GPP最新协议版本的5G手机外场通话， 率先在5G网络下畅享了微信、视频等精彩应用。
本次测试采用了遵循3GPP 2018年9月30日协议版本的核心网、传输承载网、无线网到终端的5G端到端解决方案，现网验证了大规模天线阵列、5G新空口、NSA 双连接模式、FlexE传输技术、全NFV虚拟化核心网等最新关键技术。
广东联通全球率先通过无线方式实现了5G手机终端与网络成功对接，加速推动5G终端产业链的成熟。对5G商用进程意义重大。
这表示今年我们可能就能与5G手机正式见面了。和以前一样如果你想体验5G，你不仅需要一张5G的SIM卡，你还需要一部支持5G的手机。对于物联网领域可能会有些不同，物联网对网络的兼容性更好。
## **物联网的5G方案**
2018年上半年第三代合作伙伴计划（3GPP）向国际电信联盟（ITU）提交NB-IoT和LTE-M技术作为物联网在5G时代的候选方案，以满足IMT-2020中描述的5G LPWAN要求。
3GPP承诺LPWAN将继续仅通过演进LTE-M和NB-IoT作为5G规范过程的一部分，并且3GPP同意不会为5G研究新的LPWAN解决方案。而且大多数NB-IoT和LTE-M设备的固件都可以升级以支持5G，这几乎可以说我们已经跨入了5G的大门。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/pEr7SdGKoDBHMicVXMedHeOStTibYz82brWpBXMm4gSUXYuwgEEeqco2ibLuPROn9UYImibQknKy16leH8cbMwibZ5w/640?wx_fmt=jpeg)
NB-IoT和LTE-M都是为物联网应用开发的两种LPWAN技术，都是用于低带宽蜂窝通信的协议，专为需要传输少量数据的物联网设备设计的，具有较低成本和较长时间的电池寿命。
值得注意的是NB-IoT和LTE-M都是4G技术，最初设计NB-IoT和LTE-M时确保了它们可以在LTE系统内进行带内操作，并且可以共享LTE频谱，对于5G NR也是如此。未来会使NB-IoT和LTE-M可以在5G中进行带内操作或共存。这为NB-IoT和LTE-M提供了对5G的向前兼容性。
## **物联网对网络的需求**
NB-IoT与LTE-M的出现并非偶然，并且在这之前也有其他的针对物联网的解决方案。LPWAN的兴起主要是由于物联网的普及，但像LTE之类的传统蜂窝网络解决方案通常消耗太多功率。此外，它们不适合不经常传输数据且传输数据量较少的应用。例如读取水位、气体消耗或电力使用的仪表。
**物联网需要一个能够提供低功耗和覆盖广的解决方案。一般需要满足以下四点：**
- 
技术成本低，支持更广泛的部署并提高应用程序的投资回报率
- 
低功耗，电池的使用寿命需要很长
- 
覆盖范围广，可以连接到地下、建筑物内和农村环境中的设备
- 
高连接容量，今后将会有大量的物联网设备接入物联网，这无疑会是个巨大的负荷
## **物联网方案的发展**
### **Cat-1**
Cat-1代表了使用现有LTE网络连接物联网设备的早期推动力。虽然性能不如3G网络，但他是需要浏览器界面或语音功能的物联网应用的绝佳选择。重要的是它已经标准化并且物联网很容易过渡到Cat-1网络。
### **Cat-0**
Cat-1之后有了Cat-0。相对于Cat-1，它优化了成本，消除了Cat-1支持的高数据要求的功能，放弃了对MIMO的支持，简化为半双工，峰值速率降低为1Mbit/s，终端复杂度降低为普通LTE终端的40%。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/pEr7SdGKoDBHMicVXMedHeOStTibYz82bry4NxEIszrYAQvKLZjichRcyibGVPia1K2Jf8AU6n64OEum4KsfqIicNBVQ/640?wx_fmt=jpeg)
### **LTE Cat-M1/Cat-M/LTE-M**
LTE-M通常被视为物联网应用而构建的第二代LTE芯片。他完成了Cat-0最初设想的低成本和低功耗。通过将最大系统带宽限制在1.4MHz，而不是Cat-0的20MHz。Cat-M相对于其他方案的真正优势在于Cat-M与现有的LTE网络兼容。对于运营商来说，这是一个好消息，因为他们不需要花钱来建造新的天线，尽管Cat-M与LTE网络相连需要一个软件补丁。
### **NB-IoT/Cat-M2**
NB-IoT的目的类似于Cat-M，但是它使用DSSS调制而不是LTE无线电。因此NB-IoT不在LTE频段中运行，这意味着供应商前期部署NB-IoT的成本会更高。尽管如此NB-IoT被吹捧为可能更便宜的选择，因为它消除了对网关的需求。其他方案的基础设施通常具有聚合传感器数据的网关，然后网关与主服务器通信。但是，使用NB-IoT，传感器数据将直接发送到主服务器。
## **NB-IoT与LTE-M的优劣**
LTE-M和NB-IoT都属于M2M通信的类别，也称MTC（机器类型通信）。并且两者都是专门为长时间传输少量数据的物联网设备而设计优化的。两者都有特定的应用场景。
**LTE-M**
LTE-M的设备复杂度更低，支持大规模的连接密度、低设备功耗、低延迟并提供广阔的覆盖范围，同时允许服复用LTE的基础设施。
对于任务关键型的应用，LTE-M是唯一的选择，它支持需要实时通信的设备，以确保应用程序满足用户体验要求。LTE-M支持语音，并且LTE-M上行链路和下行链路速度高达1 Mbps，远远超过NB-IOT。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBUYD0BCfPyNQCiacoZuibTibfEnuBjRUm0ErI3bhjg7IYRJ3VjoibC53yBNLPEhup9JJcgCjGmVveq9rQ/640?wx_fmt=jpeg)
尽管LTE-M技术支持语音，但每个网络运营商都要决定是否在LTE-M技术支持语音。其次在扩展覆盖面的情况下，语音根本无法用于LTE-M，仅在标准覆盖区域中支持语音。
**NB-IoT**
NB-IOT是为了实现大规模分布式设备的高效通信和长电池寿命的方案，使用移动互联网来连接这些事物。NB-IoT的特点是改善室内覆盖（拥有更强的穿透力），支持大量低吞吐量设备，低延迟灵敏度，超低设备成本，低设备功耗和优化的网络架构。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/pEr7SdGKoDBHMicVXMedHeOStTibYz82br38NeBGnslpmcd9yAQbMOgMzmD6HX1UPtv7tCd7iby8rkKLy5Sy4KzcA/640?wx_fmt=png)
在医疗保健领域，许多患者监测设备仅需要记录并将患者病情的少量数据传输到医院。在农业方面，需要传输的天气或土壤状况数据量很少，而对于智慧城市，传感器检测城市垃圾箱的充满程度或空气污染程度对数据量的要求也不高。 这时NB-IoT会是更好的选择。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/pEr7SdGKoDBHMicVXMedHeOStTibYz82brh0RZVQKOu90pqxYmAnawHO1HZF30Rayeck1oq6pC3fjqrOVU6REt7w/640?wx_fmt=png)
物联网涵盖了广泛的应用。对于实时监控，你可能需要高带宽。对于资产跟踪，数据吞吐量很小，但随着对象的移动，不可避免地会有很多切换。智能电表和许多智能城市用例需要每天一次或两次小型数据传输。这意味着没有一种技术可以满足物联网设备的特定需求。
## **正在路上的物联网**
3G UMTS和4G LTE技术使用不同的核心网络，这会产生额外的复杂性和成本，运营商也需要弃用3G技术以进入单核心网络。同样的问题不会出现在5G核心网络，它可以连接到5G NR以及5G LTE。
为了确保LTE-M和NB-IoT的类似兼容性，3GPP正在研究允许NB-IoT和LTE-M连接到5G核心网络的机制。这将使未来的5G系统能够使用相同的核心网络支持LTE、NR、NB-IoT和LTE-M。
NB-IoT和LTE-M刚刚起步。在接下来的几年中，这些低功耗蜂窝网络将为大量新应用打开大门，连接数十亿需要远程连接并延长电池寿命的设备。
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
