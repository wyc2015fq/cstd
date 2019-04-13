
# Jmeter测试并发https请求成功了 - 高科的专栏 - CSDN博客

2011年08月10日 14:31:17[高科](https://me.csdn.net/pbymw8iwm)阅读数：40163


Jmeter2.4 如何测试多个并发https请求，终于成功了借此机会分享给大家
首先要安装jmeter2.4版本的，而且不建议大家使用badboy，因为这存在兼容性问题。对于安装，我就不讲了，我就说说如何测试https，想必大家都在网上找相关资料找疯了，我几乎也是呀。
有这么一个网页，
http://jmeter.512774.n5.nabble.com/HTTPS-problem-td533218.html  虽然讨论之后没有成功，但是给我很多灵感。
好了开始：
**（1）新建threadGroup：**
![](http://hi.csdn.net/attachment/201108/10/0_1312957118NYFW.gif)
**（2）设置并发用户数量：**
![](http://hi.csdn.net/attachment/201108/10/0_13129572293xx1.gif)
**（3）录制脚本**，我听见有人好像在“哇，不会吧”。其实没什么，虽然是按https测得，但是这里就建成http的request。后边你就知道了。
![](http://hi.csdn.net/attachment/201108/10/0_13129573768ugz.gif)
因为https端口值443，注意protocol的设置为https，添加Name只就是你要发送的数据。

![](http://hi.csdn.net/attachment/201108/10/0_1312957550CdXq.gif)

![](http://hi.csdn.net/attachment/201108/10/0_13129577487edD.gif)
![](http://hi.csdn.net/attachment/201108/10/0_1312957714OVjO.gif)

**（4）收功，开始run之前添加证书**，在，

![](http://hi.csdn.net/attachment/201108/10/0_1312957850ZsSf.gif)，run一下，就OK了
（5）
![](http://hi.csdn.net/attachment/201108/10/0_1312957935BR55.gif)


