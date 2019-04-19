# react native白屏及性能优化 - 三少GG - CSDN博客
2016年07月25日 02:06:24[三少GG](https://me.csdn.net/scut1135)阅读数：7446
# RN启动页SplashScreen在出现白屏后才能显示(用js做splash是肯定会出现白屏的)，
# 原生app的启动页不会出现白屏问题[http://bbs.reactnative.cn/topic/572/rn%E5%90%AF%E5%8A%A8%E9%A1%B5splashscreen%E7%99%BD%E5%B1%8F%E5%90%8E%E6%89%8D%E8%83%BD%E6%98%BE%E7%A4%BA-%E5%8E%9F%E7%94%9Fapp%E7%9A%84%E5%90%AF%E5%8A%A8%E9%A1%B5%E4%B8%8D%E4%BC%9A%E5%87%BA%E7%8E%B0%E7%99%BD%E5%B1%8F%E9%97%AE%E9%A2%98/2](http://bbs.reactnative.cn/topic/572/rn%E5%90%AF%E5%8A%A8%E9%A1%B5splashscreen%E7%99%BD%E5%B1%8F%E5%90%8E%E6%89%8D%E8%83%BD%E6%98%BE%E7%A4%BA-%E5%8E%9F%E7%94%9Fapp%E7%9A%84%E5%90%AF%E5%8A%A8%E9%A1%B5%E4%B8%8D%E4%BC%9A%E5%87%BA%E7%8E%B0%E7%99%BD%E5%B1%8F%E9%97%AE%E9%A2%98/2)
**启动白屏Trick:  **
1. 添加LanchImage
[http://liuyy.coding.me/2016/07/22/react-native/react-native_launch/](http://liuyy.coding.me/2016/07/22/react-native/react-native_launch/)给android端设置启动图
为了解决这个问题，找了很多的资料，参考了 [ReactNative安卓首屏白屏优化](http://reactnative.cn/post/754)，
 也并没有解决掉启动慢点问题 ， 原因是全react-native的应用，内存换效率的做法并不起作用。
于是我决定曲线就够，在app启动时先展示一张背景图片，当启动后隐藏。
**效果类似： 饿了么**
2. 设置SplashScreen
[https://github.com/SpikeKing/LearningRN](https://github.com/SpikeKing/LearningRN)  （同一篇，有gif图  [http://www.jianshu.com/p/08f296eb67e4](http://www.jianshu.com/p/08f296eb67e4)）
综上两点： [http://www.vanadis.cn/2016/06/30/react-native-splash-screen/](http://www.vanadis.cn/2016/06/30/react-native-splash-screen/)
# react-native实现launchImage和splash-screen
参考：安卓白屏优化 
[https://github.com/cnsnake11/blog/blob/master/ReactNative%E5%BC%80%E5%8F%91%E6%8C%87%E5%AF%BC/ReactNative%E5%AE%89%E5%8D%93%E9%A6%96%E5%B1%8F%E7%99%BD%E5%B1%8F%E4%BC%98%E5%8C%96.md](https://github.com/cnsnake11/blog/blob/master/ReactNative%E5%BC%80%E5%8F%91%E6%8C%87%E5%AF%BC/ReactNative%E5%AE%89%E5%8D%93%E9%A6%96%E5%B1%8F%E7%99%BD%E5%B1%8F%E4%BC%98%E5%8C%96.md)
##################################################
**性能优化**
1.探索react native首屏渲染最佳实践
[http://web.jobbole.com/85451/](http://web.jobbole.com/85451/)
2. QQ空间终端开发团队  [https://zhuanlan.zhihu.com/p/20587485](https://zhuanlan.zhihu.com/p/20587485)
3.React Native 性能优化建议
[http://www.ghugo.com/react-native-high-performance/](http://www.ghugo.com/react-native-high-performance/)

