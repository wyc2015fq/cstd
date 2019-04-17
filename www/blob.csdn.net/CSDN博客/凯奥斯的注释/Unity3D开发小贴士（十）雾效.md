# Unity3D开发小贴士（十）雾效 - 凯奥斯的注释 - CSDN博客





2016年11月15日 22:47:23[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：2740
所属专栏：[Unity3D开发小贴士](https://blog.csdn.net/column/details/12907.html)









# 在Unity3D（5.3.4）中，点击Window->Lighting，切换到Scene页签，就可以选择打开雾效。

![](https://img-blog.csdn.net/20161115221646182)


效果如图所示：

![](https://img-blog.csdn.net/20161115221737714)


雾效分为三种模式，它们的雾效因子计算公式为：

Linear 线性

*factor = (end-z)/(end-start)*


Exponential

*factor = exp(-density*z)*


Exponential Squared

*factor = exp(-(density*z)^2)*




其中z为距离摄像机的距离。







我们还可以导入官方的Effects包

![](https://img-blog.csdn.net/20161115221903574)


为摄像机添加Global Effect的组件：

![](https://img-blog.csdn.net/20161115222052996)


通过调节参数来对雾效进行屏幕后处理。



