# MQTT+ApolloServer实现Android的消息推送功能 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年05月13日 17:24:13[boonya](https://me.csdn.net/boonya)阅读数：7393








**转载自**：[http://www.codeforge.cn/article/256428?go_blog_box=1](http://www.codeforge.cn/article/256428?go_blog_box=1)



**应用背景**

最近，领导让研究android系统的应用开发，于是就开始研究数据库访问+消息推送+实时视频播放等基本应用，下面主要是android消息推送功能的研究。

消息的推送就是从服务器端向移动终端发送连接请求，传输特定的信息。例如一些新闻客户端，每隔一段时间都会受到一条或者多条通知（Notification），这就是从服务器端传过来的推送消息。

推送的技术主要是以下三种： 

（1）通过SMS（Short Message Service，短信群发服务系统）进行服务器端和客户端的交流通信。在Android平台上，可以通过拦截SMS消息并解析内容来了解服务器的意图，就可以实现完全的实时操作。但是该方案成本比较高，而且基本依赖运营商。

（2）循环主动定时获取。该方法需要客户端编写一个定时或者周期性访问服务器端接口，从而获取最新的消息。但是，轮询的频率太快会消耗网络带宽和电量，太慢会导致消息更新不及时。

（3）持久连接方案。需要在客户端开通一个服务来保持和服务器端的持久连接（Apple和谷歌的C2DM采用该机制），从而当服务器端有最新的消息时，可以及时接收到通知。但是该方案也会消耗系统的性能和电量，当Android系统可用资源较低时，系统会关闭服务，从而与系统的连接会终端。

     相比较其它两种方案，第三种方案的优势还是比较明显，而且可行性最高。在Android系统软件中编写系统服务或开机启动功能，当系统资源利用率较低而关闭服务器时，可以再重启该服务，进而实现持久连接的方式。使用谷歌的C2DM（Android Cloud to Device Message）应该是实现PUSH消息的最便捷方式，但是C2DM必须依赖谷歌的服务器，国内网络无法正常使用。

    目前，有两种较好的方式可以实现这种持久连接的功能，一种是IBM的MQTT（Message Queuing Telemetry Transport，消息队列遥测传输）协议实现PUSH消息功能，第二种是采用基于XMPP协议的第三方Androidpn实现消息推送。

MQTT是IBM开发的一个即时通信协议，该协议支持所有平台，可以将所有物联网设备和外部连接起来，被用来当作传感器和致动器的通信协议。

         使用它作为消息推送协议的基本原理是：使用消息代理服务器（Message Broker），客户端连接上消息代理服务器，跟服务器协商可以接受哪些类型的消息，同时也可以发布自己的消息，这些消息根据协议的内容可以被其它客户端收到，业务流程如图1所示。

<!--[if gte vml 1]><!--[if !vml]--><!--[endif]--><!--[if gte mso 9]>![](http://www.codeforge.cn/attached/image/20150601/20150601041145_74698.png)

关键技术

## 通知功能的实现

## 1. Apollo服务器的安装和配置

想要实现基于MQTT的消息推送功能，必须先配置message broker服务器，这里选择服务器Apollo1.7来搭建环境。具体的搭建主要分为下面几个步骤：

（1）先到官方网站现在最新版的Apollo1.7服务器。

![](http://www.codeforge.cn/attached/image/20150601/20150601042458_84817.jpg)

  （2）然后运行apollo.cmd 创建服务器实例。

![](http://www.codeforge.cn/attached/image/20150601/20150601042644_26491.png)

（3）在服务器实例的文件夹下面配置参数。

![](http://www.codeforge.cn/attached/image/20150601/20150601042713_14771.png)

（4）运行apollo-broker.cmd run开启服务器，在浏览器中输入服务器的IP地址，即可以访问Apollo服务器界面。




![](http://www.codeforge.cn/attached/image/20150601/20150601042751_87935.png)

![](http://www.codeforge.cn/attached/image/20150601/20150601042804_47805.png)

（5）输入之前配置好的用户名和密码参数，即可以访问Apollo Console控制台。

![](http://www.codeforge.cn/attached/image/20150601/20150601042830_56840.png)

### 2. MQTT客户端的实现

使用WebService服务器向Android端发送通知，实际上可以看成是两个MQTT 客户端之间的通信过程，消息在传播过程中间经过了Message Broker服务器的转发，所以需要在WebService服务器和Android端分别配置MQTT客户端。两边的代码实现过程基本一致，都是要在工程中导入mqtt-client-0.4.0.jar的类库，然后要使用如下图这些工具类实现MQTT功能。

![](http://www.codeforge.cn/attached/image/20150601/20150601043326_67707.png)

具体的代码可以参考：

![](http://www.codeforge.cn/attached/image/20150601/20150601043346_93725.png)![](http://www.codeforge.cn/attached/image/20150601/20150601043356_28185.png)

源代码地址：[http://www.codeforge.cn/article/256428?go_blog_box=1](http://www.codeforge.cn/article/256428?go_blog_box=1)












