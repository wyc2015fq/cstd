# mac与phy如何实现网络自适应 - maopig的专栏 - CSDN博客
2016年10月06日 11:35:21[maopig](https://me.csdn.net/maopig)阅读数：2450
个人分类：[网络](https://blog.csdn.net/maopig/article/category/869532)
这两天修改网卡驱动以实现10/100/1000M自适应，因此研究了下phy芯片和emac驱动如何兼容10/100/1000M网络环境，记录在此。
网络中设备端数据链路层由mac芯片和phy芯片组成，phy芯片根据外部网络环境完成自动协商以及配置，驱动中根据phy状态来配置mac，以达到phy与mac的相互配合工作。
现在主流的网卡phy芯片为100M和1000M，都是向下兼容。100M phy支持10/100M环境，1000M phy支持10/100/1000M环境。
首先说10/100/1000M都是指的数据收发速率，单位是bps。所以这里要从mac以及phy的数据收发接口下手。
mac与phy主流数据接口有GMII MII RMII等。接口规范中定义了数据收发信号线个数。接口定义可以看这篇文章：
http://blog.csdn.net/skyflying2012/article/details/8252843
以GMII/MII为例来研究，GMII是有8根rx/tx线，MII有4根rx/tx线。
首先以外接100Mphy芯片来研究。phy与外界网络环境完成自动协商来确定其本身的速度。100M phy都支持MII接口，因此mac端就需要以MII接口定义与phy相连。
数据收发最重要的是提供正确的数据时钟，来保证数据正确的采样与发送。
对于100M phy，这个问题好解决，因为MII接口支持10/100M，4根数据线，只需要mac给phy提供2.5/25MHZ数据时钟即可。
最值得研究的是外接1000M phy时如何适应10/100/1000M网络环境。
首先说，这是一个倒推的过程，首先来看phy是如何处理10/100M和1000M环境的。mac的接口模式以及时钟再根据phy的配置进行配置。
随便找了一个1000M phy芯片的datasheet，RTL88E1111，对于GMII/MII接口的描述如下：
![](https://img-blog.csdn.net/20150729184142803?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20150729184939368?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
根据这段描述看出，GMII接口模式支持1000M环境，但是在10/100M环境下phy完成自动协商确定为10/100BASE-TX后就切换为MII模式，GMII引脚都是与MII兼容的（MII下8根数据线有4根不用），GMII接口定义中有2个clk线，GTX_CLK TX_CLK（GMII/MII下rx clk由phy提供），GTX_CLK在GMII模式下提供125MHZ，TX_CLK在MII模式下提供2.5/25MHZ。下面给出一个mac与1000M
 phy硬件电路连接图。
![](https://img-blog.csdn.net/20150729184254142?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
可以看出对于1000M phy，GTX_CLK TX_CLK都需要与mac相连，在10/100/1000M环境下我分别用示波器测量clk的确如上所说。
**综上，**
**对于100M phy，外部网络10/100M切换，仅需要改变mac提供的数据时钟即可，mac接口模式不变，因为MII兼容10/100M。**
**对于1000M phy，外部网络10/100/1000M切换，首先需要改变mac的接口模式（使用的数据线个数不同了），因为10/100M下phy会切换为MII模式，根据接口模式在改变其数据时钟。**
当然phy在自动协商完成后是其硬件逻辑会完成模式GMII/MII的转换，而对于mac，则要由驱动根据phy的工作状态来确定mac的接口模式以及需要提供的clk。
这也是我们软件开发人员最需要关注的地方，根据phy的状态，如何配置mac（接口模式 数据时钟）来保证与phy一致。
