
# Android binder 框架和学习资料 - 嵌入式Linux - CSDN博客

2019年04月03日 21:15:32[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：4


![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Qof5hj3zMPfGVCVLOEkSJZLKEib97e55aFgqBR5CvbkHscCrGezCMQWHxS5LqUHXje39wibstLHXWtWF0DKv0Hmg/640?wx_fmt=jpeg)
１
Android binder 是学习 Android 系统一定要啃得硬骨头，可能你刚开始的时候并不理解其中的精髓，但是在 android 系统的很多地方你都会遇到它。

不过要我自己写明白其中的逻辑脉络需要花费太多的时间和精力，而且传播效果也不是非常好，还不如直接用其他大神的资源。

2
博客文章
转转一篇 Android binder 的文章，最后再给一些 binder 的视频学习，学完这些应该能够掌握这个鬼东西了。其中包括一个博客，博客不能直接跳转，我把链接放出来，然后在**阅读原文**里面我也放了原博客的链接，可以直接点击进去，不需要复制到浏览器打开。

*https://wangkuiwu.github.io/2014/09/01/Binder-Introduce/*

*http://zhoujinjian.cc/2018/01/01/Android-7-1-2-Android-N-Android-Binder%E7%B3%BB%E7%BB%9F%E5%88%86%E6%9E%90/index.html*

里面的框架说明的非常不错了，可以利用碎片时间来学习一下，然后直接按照这里一篇文章一篇文章往下看，就慢慢学到精髓了。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Qof5hj3zMPfGVCVLOEkSJZLKEib97e55aclsryLXcia9yNKQPdRJibDTh8fKffBdYq4xMzJDibdk1FHQCodibsowCHg/640?wx_fmt=jpeg)
3
视频资料
只看书籍来学习那是非常枯燥的，如果能直接把文章看完，又理解了，那学习能力是非常厉害的，我一直认为，在所有能力里面，学习能力是最牛的，这就像张无忌学会了乾坤大挪移，然后再去学一些其他的武功非常容易，这种就叫做学习能力。比如，我今天不会 android ，我不会算法，但是我会学啊。我总有一天能够成为大牛的。

所以，在这里我推荐一个比较好的 binder 学习视频，韦东山老师的，也是免费的，如果是学生，我建议要把这个部分多看几遍，这免费的东西真的不多，花钱的话，大家又心理嘀咕着又少了好多可以出去浪的资金。

公众号后台回复 「 binder 」获取视频教程。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/Qof5hj3zMPfGVCVLOEkSJZLKEib97e55aFMTBdfvibxbTIOtgM79cjVzYq3CMFdq6IWqT2S7ib8ntF944EAloZPYg/640?wx_fmt=png)
4
实例
我写代码有个习惯，如果要实现一个新的功能，喜欢去找这方面的例程代码，我觉得不管做什么事情，最终的目标都是解决问题，不以解决问题为目的的学习都是浪费时间，你现在学习 android 就是希望以后在工作上能用到，在面试上能把面试官给拍死。所以，我这里给了个例程链接，大家可以看看，如果很着急，直接跳过上面，直接测试例程。

*https://github.com/yuanhuihui/BinderSample*

binder 里面包括 app 之间通信，framework 之间通信 ，hal/native层之间通信。
native 层 binder 通信框架图
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Qof5hj3zMPfGVCVLOEkSJZLKEib97e55aU46bOkP6znS67dRg5Zo7Oe3MIFnOko5XYwaHKNvHhrqOTVA1fpnNiaw/640?wx_fmt=jpeg)
framework 层 binder 通信框架图
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Qof5hj3zMPfGVCVLOEkSJZLKEib97e55a6ZEKManGGy72td76mzxicKiapqkvTpRJM4qhcREd213dDEPAibujKuQ8g/640?wx_fmt=jpeg)
app 层 binder 通信框架层
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Qof5hj3zMPfGVCVLOEkSJZLKEib97e55apPicHZ9vdHLJO6eJn6f6oAqkU3JjZhnyWia2JvovibbsVpFpo9uCex4PQ/640?wx_fmt=jpeg)
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/Qof5hj3zMPfGVCVLOEkSJZLKEib97e55a4PPoWD55g3mYuluaiaqGW1QdahialOQmGia6HlKAvhVfbFk3hkqOX0cLA/640?wx_fmt=gif)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Qof5hj3zMPfGVCVLOEkSJZLKEib97e55aLBLkqQyv4muOgmCdpNfibSTYZbQqswOVJykuOJEQliaPCGicFMjxwXgCQ/640?wx_fmt=jpeg)



