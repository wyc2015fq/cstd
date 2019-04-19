# Java的native方法java.lang.UnsatisfiedLinkError - Koma Hub - CSDN博客
2018年01月19日 15:50:39[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：151
个人分类：[Java																[Java native](https://blog.csdn.net/Rong_Toa/article/category/7157493)](https://blog.csdn.net/Rong_Toa/article/category/7156200)
native为java关键字，是为了调用本机方法（不同语言之间的混合编程），Java与C语言混合编程
今天仔细搞了一下native，但是一直出错：
```
Exception in thread "main" java.lang.UnsatisfiedLinkError: Toa_TIFD.swap_float(LV
        at Toa_TIFD.swap_float(Native Method)
        at Toa_TIFD.main(Toa_TIFD.
```
在建立linux下的库（.so文件）时，native方法在一个类名为swap的类中，虽然native方法被定义为static，但是在调用这个native方法时，即便-Djava.library.path=PATH路径是正确的，编译无错，在运行到调用native方法时仍然会有这个LinkError错误。
我就在想到底是哪里错了，意识在调用这个native方法的java文件中吧这个native方法放在一个名为swap的类中，就他妈没错了。
