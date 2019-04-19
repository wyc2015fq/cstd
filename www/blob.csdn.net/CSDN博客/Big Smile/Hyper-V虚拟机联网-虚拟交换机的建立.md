# Hyper-V虚拟机联网-虚拟交换机的建立 - Big Smile - CSDN博客
2016年07月10日 00:07:39[王啸tr1912](https://me.csdn.net/tr1912)阅读数：6451
        在上一篇博客中我们了解了虚拟机的建立方法，现在我们来介绍一下虚拟机联网的实现过程。
        我们都知道，自己电脑的联网是通过插网线，输入宽带账户密码实现的，但是我们在虚拟机当中是找不到这些设备的，那么我们要如何去作呢，首先我们需要了解多台联合电脑的上网方式。
# 一、现实电脑的网络连接
        首先我们来看网络拓扑图，其中在左边的是直接连接路由器的类型，而右边的是连接交换机的类型。
![](https://img-blog.csdn.net/20160709231824965?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
      其中router是路由器，switch是交换机。
        在这个网络系统中假使路由器是连接互联网的根源设备，而PC是终端的话，那么，多台设备的连接就有多种的方案。如下：
![](https://img-blog.csdn.net/20160709232943725?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
     其中Hub是集线器的意思，通过集线器和交换机就可以实现用一个路由(或者叫做一个固有IP)可以让多台设备上网了。但是在虚拟的环境下，我们的虚拟设备是相当于独立于自己电脑系统的一个存在，并且我们要求两个设备同时联网，所以我们就需要一个虚拟的交换机或者集线器来实现这个功能。
# 二、虚拟交换机的设置
        幸好我们使用的Hyper-V这个虚拟机自带创建网络虚拟交换机的功能，所以我们可以直接进行设置。
        首先打开虚拟交换机的创建界面：
![](https://img-blog.csdn.net/20160709233920537?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
          然后点击新建续集网络交换机，我们要选择外部，因为我们要连接的是外网(互联网，局域网就是内网)，然后点击右下的创建虚拟交换机。
![](https://img-blog.csdn.net/20160709234325148?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        然后就是填写交换机的名称还有选择物理网卡了，其中名字就不多说了，网卡呢就是你需要创建交换机的联网方式的网卡，也可以理解为把哪个物理网卡模拟为交换机，选择好了以后点击应用就可以创建了。创建好了之后，你的网络设备中会出现这样的设备：
![](https://img-blog.csdn.net/20160709234735156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
         然后你就可以看到你的以太网（也就是原来的物理网卡产生的连接设配器）也已经连接到了新的交换机上了。![](https://img-blog.csdn.net/20160709234949831?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        至此，虚拟交换机的配置已经完成了。
# 三、虚拟机的联网
        首先点击需要联网的虚拟机，打开设置这个选项，点击就版网络适配器(或者网络适配器，暂没有发现区别)，在虚拟交换机上选择刚才创建好的交换机就可以了。
![](https://img-blog.csdn.net/20160709235223933?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
         然后进入系统，进行网关和IP的配置，Windows的就用说了，主要在网络里面进入IPV4属性界面自定义就行了，网关就是路由的地址，IP就是分配的地址，下面主要说一下Ubuntu系统的配置：
       首先进入编辑连接的界面：
![](https://img-blog.csdn.net/20160709235959251?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        然后再选择新出现的连接图示，这里以连接2为例，点击编辑：
![](https://img-blog.csdn.net/20160710000131550?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        最后点击IPV4的设置，填写好各项信息，注意IP是分配或者自己定义的，网关是上方的最近的路由器的地址，DNS是公用的DNS，随便就行。
![](https://img-blog.csdn.net/20160710000318522?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        填写完毕之后就可以正常上网了，注意是在你的物理电脑正常上网的前提下。
# 四、小结
        通过了这次的虚拟机之旅，让我意识到了以前学习过的东西都是有用的，我们要时常的复习，并且活用这些已有的知识，这样才能通过构建知识网学习到新的东西。
至于为什么这里只有交换机没有集线器，下篇博客将会介绍交换机和集线器的区别。
