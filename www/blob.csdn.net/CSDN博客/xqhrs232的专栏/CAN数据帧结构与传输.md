# CAN数据帧结构与传输 - xqhrs232的专栏 - CSDN博客
2019年02月13日 18:04:27[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：47
个人分类：[车载电子技术](https://blog.csdn.net/xqhrs232/article/category/7465105)
原文地址::[https://blog.csdn.net/huan447882949/article/details/80042417](https://blog.csdn.net/huan447882949/article/details/80042417)
相关文章
1、CAN总线之CAN的报文格式----[https://blog.csdn.net/GUOXINLAN123/article/details/79398875](https://blog.csdn.net/GUOXINLAN123/article/details/79398875)
2、CAN通信的数据帧和远程帧----[https://blog.csdn.net/weixin_37787043/article/details/80906905](https://blog.csdn.net/weixin_37787043/article/details/80906905)
3、CAN2.0B 数据帧详解----[https://blog.csdn.net/daweihuahuo/article/details/78352762](https://blog.csdn.net/daweihuahuo/article/details/78352762)
4、CAN总线操作命令----[https://blog.csdn.net/farevay/article/details/81080166](https://blog.csdn.net/farevay/article/details/81080166)
5、linux CAN编程（二）----------- can_frame中can_id的数据组织形式及处理----[https://blog.csdn.net/qq_35820102/article/details/84646533](https://blog.csdn.net/qq_35820102/article/details/84646533)
6、can收发程序分析----[https://blog.csdn.net/satanwxd/article/details/5761147](https://blog.csdn.net/satanwxd/article/details/5761147)
7、Linux CAN编程详解----[https://blog.csdn.net/zjy900507/article/details/79659601](https://blog.csdn.net/zjy900507/article/details/79659601)
CAN：Controller Area Network，控制器局域网
是一种能有效支持分布式控制和实时控制的串行通讯网络。
CAN-bus： Controller Area Network-bus，控制器局域网总线技术
CAN总线系统结构
- CAN 控制器
接收控制单元中微处理器发出的数据，处理数据并传给CAN收发器
- CAN收发器
将数据传到总线 or 从总线接收数据给控制器
- CAN数据传递终端
避免数据传输终了反射回来
- 
CAN数据总线
传输数据的[数据线](https://www.baidu.com/s?wd=%E6%95%B0%E6%8D%AE%E7%BA%BF&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)，两根 CAN-high and CAN-low，差分信号
![](http://images.cnitblog.com/blog/671019/201409/221939002019060.png)
![](http://images.cnitblog.com/blog/671019/201409/221939042325291.png)
CAN总线系统的数据传输
CAN总线节点上的节点发送数据是以报文的形式广播给网络中所有节点。收发器接收到数据就把数据传送给控制器，再由控制器检查判断是不是所需数据。不是则忽略。
- 网络上任何一个节点在任何时候都可以发送数据
- 多个节点发送数据，优先级低主动退出发送
- 短帧结构，每帧数据信息为0~8字节（具体用户定义），对数据编码而不是地址编码
- CAN每帧都有CRC校验和其他检验措施，严重错误的情况下具有自动关闭输出的功能
CAN总线系统数据格式
CAN-bus通信帧共分为数据帧、远程帧、错误帧、过载帧和帧间隔
五种类型。
- 
数据帧
结构上由7个段组成，其中根据仲裁段ID码长度的不同，分为标准帧（CAN2.0A）和扩展帧（CAN2.0B）。
![](http://images.cnitblog.com/blog/671019/201409/221939050291934.png)
帧起始、帧结束
![](http://images.cnitblog.com/blog/671019/201409/221939055768736.png)
仲裁段
![](http://images.cnitblog.com/blog/671019/201409/221939066542464.png)
显性隐性
![](http://images.cnitblog.com/blog/671019/201409/221939083101237.png)
总线仲裁
![](http://images.cnitblog.com/blog/671019/201409/221939090147852.png)
![](http://images.cnitblog.com/blog/671019/201409/221939094045437.png)
- 从该分析过程得出结论是：帧ID值越小，优先级越高
- 
对于同为扩展格式数据帧、标准格式远程帧和扩展格式远程帧的情况同理。
![](http://images.cnitblog.com/blog/671019/201409/221939099825781.png)
![](http://images.cnitblog.com/blog/671019/201409/221939177015074.png)
控制段
![](http://images.cnitblog.com/blog/671019/201409/221939206239332.png)
数据段
![](http://images.cnitblog.com/blog/671019/201409/221939211394190.png)
CRC段
![](http://images.cnitblog.com/blog/671019/201409/221939216709519.png)
ACK段
![](http://images.cnitblog.com/blog/671019/201409/221939237484121.png)
远程帧
![](http://images.cnitblog.com/blog/671019/201409/221939252013365.png)
数据帧与远程帧的区别
![](http://images.cnitblog.com/blog/671019/201409/221939257959479.png)
CAN-bus 错误类型
![](http://images.cnitblog.com/blog/671019/201409/221939266391137.png)
![](http://images.cnitblog.com/blog/671019/201409/221939271544993.png)
![](http://images.cnitblog.com/blog/671019/201409/221939291072324.png)
过载帧
![](http://images.cnitblog.com/blog/671019/201409/221939306075582.png)
帧间隔
![](http://images.cnitblog.com/blog/671019/201409/221939499825668.png)
![](http://images.cnitblog.com/blog/671019/201409/221939507645540.png)
