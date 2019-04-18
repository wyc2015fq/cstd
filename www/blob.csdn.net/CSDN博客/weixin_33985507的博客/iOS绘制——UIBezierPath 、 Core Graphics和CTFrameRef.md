# iOS绘制——UIBezierPath 、 Core Graphics和CTFrameRef - weixin_33985507的博客 - CSDN博客
2018年03月08日 12:36:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
### UIBezierPath 、 Core Graphics和CTFrameRef这三者都是来绘制渲染的，
但是三者又有很多区别，使用上呢都有各自的特殊功能。如果我们简单绘制图形UIBezierPath和Core Graphics就可以实现，UIBezierPath其实是对Core Graphics的封装，功能没有Core Graphics强大。但常用的功能基本够我们使用
很多重复的东西我就不介绍了：在这里总结了几篇好的文章，大家参考就可以了：
讲解UIBezierPath 和 Core Graphics的简单使用写很不错：[https://www.jianshu.com/p/bbb2cc485a45](https://www.jianshu.com/p/bbb2cc485a45)
### **UIBezierPath我们常用的接口：**
> 
![2872842-2cfc42403e6dd63a.png](https://upload-images.jianshu.io/upload_images/2872842-2cfc42403e6dd63a.png)
UIBezierPath的一些接口
![2872842-4f08c7f499e69bfc.png](https://upload-images.jianshu.io/upload_images/2872842-4f08c7f499e69bfc.png)
绘制设置
### **Core Graphics常用的一些接口：（跟上面的功能类似）**
![2872842-d3ccfc84e8efe12b.png](https://upload-images.jianshu.io/upload_images/2872842-d3ccfc84e8efe12b.png)
获取图形上下文
![2872842-de5c6fe2e1e6ca67.png](https://upload-images.jianshu.io/upload_images/2872842-de5c6fe2e1e6ca67.png)
添加划线
![2872842-9dab69e3bcbb6cef.png](https://upload-images.jianshu.io/upload_images/2872842-9dab69e3bcbb6cef.png)
设置绘制属性
![2872842-768ea59e47c9eaaa.png](https://upload-images.jianshu.io/upload_images/2872842-768ea59e47c9eaaa.png)
设置图形变形
CTFrameRef：这个类是位了绘制文字的，也是富文本，讲解的很清楚！也很明白
大家可以看一看，有惊喜的哦！而且也是做电子书非常重要的文档！！！！！
很不错！[http://www.cnblogs.com/XYQ-208910/p/6222931.html](https://link.jianshu.com?t=http%3A%2F%2Fwww.cnblogs.com%2FXYQ-208910%2Fp%2F6222931.html)
希望大家给我点个赞！谢谢各位看客老爷们！
