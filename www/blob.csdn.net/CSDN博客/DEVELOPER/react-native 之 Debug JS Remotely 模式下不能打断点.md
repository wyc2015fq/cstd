# react-native 之"Debug JS Remotely"模式下不能打断点? - DEVELOPER - CSDN博客





2018年06月08日 14:41:28[学术袁](https://me.csdn.net/u012827205)阅读数：1236
所属专栏：[react-native](https://blog.csdn.net/column/details/23827.html)









正确打开远程调试，看到下图 
![这里写图片描述](https://img-blog.csdn.net/20180605102607364?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
`不知道有没有别的朋友遇到过，反正我是实实在在、真真切切的遇到了这种奇葩问题！`

情景是这样的，之前刚开始学习的时候，进入`ReactNative``debug模式下` 就能直接打断点并进行调试。可最近项目写的越来越大，然后里面出现了很多`黄色警告` ，本来以为是警告太多问题导致，然后就花了点时间逐个把黄色警告的问题解决！再次进入RN的调试页面还是不能打断点进行正常的调试。[至于断点调试的博客有很多，想要更多了解可以看下这篇文章。](https://www.jianshu.com/p/30e1d7d99a9e)

回到我的话题，然后我查阅了很多的博客，了解了很多别人进行调试的经验， 

而问题依然存在我这里且得不到解决。。。 

如果有朋友有解决该问题的经验可以传授一下，谢谢！
现在我就分享一个绕过`远程断点` 调试的方式，使用`代码断点` 调试来代替。 
![这里写图片描述](https://img-blog.csdn.net/20180608143825399?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




