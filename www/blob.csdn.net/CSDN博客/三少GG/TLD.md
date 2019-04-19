# TLD - 三少GG - CSDN博客
2012年11月18日 12:08:35[三少GG](https://me.csdn.net/scut1135)阅读数：1019
个人分类：[PKU Research](https://blog.csdn.net/scut1135/article/category/801457)
原作者：
[http://info.ee.surrey.ac.uk.sixxs.org/Personal/Z.Kalal/tld.html](http://info.ee.surrey.ac.uk.sixxs.org/Personal/Z.Kalal/tld.html)
新闻报道：
[http://www.cvchina.info.sixxs.org/2011/04/05/tracking-learning-detection/](http://www.cvchina.info.sixxs.org/2011/04/05/tracking-learning-detection/)
优酷视频：
[http://v.youku.com/v_show/id_XMjU2MzA2MDcy.html](http://v.youku.com/v_show/id_XMjU2MzA2MDcy.html)
**转载库： [http://blog.csdn.net/gxiaob/article/category/1312584](http://blog.csdn.net/gxiaob/article/category/1312584)**
源码学习：
### [TLD（Tracking-Learning-Detection）学习与源码理解之（一）](http://blog.csdn.net/zouxy09/article/details/7893011)
TLD(Tracking-Learning-Detection)是英国萨里大学的一个捷克籍博士生Zdenek Kalal在其攻读博士学位期间提出的一种新的单目标长时间（long term tracking）跟踪算法。该算法与传统跟踪算法的显著区别在于将传统的跟踪算法和传统的检测算法相结合来解决被跟踪目标在被跟踪过程中发生的形变、部分遮挡等问题。同时，通过一种改进的在线学习机制不断更新跟踪模块的“显著特征点”和检测模块的目标模型及相关参数，从而使得跟踪效果更加稳定、鲁棒、可靠。
作者网站的链接[http://info.ee.surrey.ac.uk/Personal/Z.Kalal/](http://info.ee.surrey.ac.uk/Personal/Z.Kalal/)
其开放源代码，在网站上可以下载到源代码已经其demo程序，但是源代码是由Matlab和C写的，对于我这种不懂Matlab的菜鸟来说，看代码就像天书；但很庆幸，有一个大牛已经用c++把TLD重新写好了，而且代码很规范。并且提供源码下载：
[https://github.com/arthurv/OpenTLD](http://www.google.com/url?sa=D&q=https://github.com/arthurv/OpenTLD&usg=AFQjCNGDXLUFLgmCuf4Np-Tr6241h_G6fA)
源码为Linux版本，基于Opencv2.3在源码/doc文件夹下有其程序设计接口，很清晰。
ZK关于这个TLD框架发表了很多论文，感觉对理解代码非常有用的论文有下面三个：
（1）Tracking-Learning-Detection
（2）Forward-Backward Error Automatic Detection of Tracking Failures
（3）Online learning of robust object detectors during unstable tracking
在作者的网站上好像也提供下载了。
另外自己学习的过程中，也搜到了不少大牛对TLD的分析，得到了很多帮助，具体有：
（1）《庖丁解牛TLD》系列：
[http://blog.csdn.net/yang_xian521/article/details/7091587](http://blog.csdn.net/yang_xian521/article/details/7091587)
（2）《再谈PN学习》：
[http://blog.csdn.net/carson2005/article/details/7647519](http://blog.csdn.net/carson2005/article/details/7647519)
（3）《比微软kinect更强的视频跟踪算法--TLD跟踪算法介绍》
[http://blog.csdn.net/carson2005/article/details/7647500](http://blog.csdn.net/carson2005/article/details/7647500)
（4）《TLD视觉跟踪技术解析》
[http://www.asmag.com.cn/number/n-50168.shtml](http://www.asmag.com.cn/number/n-50168.shtml)
### [TLD（Tracking-Learning-Detection）学习与源码理解之（七）](http://blog.csdn.net/zouxy09/article/details/7893090)
