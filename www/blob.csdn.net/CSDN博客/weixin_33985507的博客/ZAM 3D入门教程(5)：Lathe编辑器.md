# ZAM 3D入门教程(5)：Lathe编辑器 - weixin_33985507的博客 - CSDN博客
2010年12月27日 17:22:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
在上一篇文章中，大致介绍了Extrusion编辑器的用法，在本篇博客中继续讲解另一个编辑器，即Lathe编辑器。
先来看一下Lathe编辑器的界面，和Extrusion编辑器很类似。
![](https://images.cnblogs.com/cnblogs_com/alexis/20101230Zam3D01.jpg)
**5.1 lathe Editor**
Lathe：车床的意思，那么什么是Lathe编辑器呢？
其英文解释是这样的：
The lathe editor is a special function in Zam 3D that lets you draw a curve or a shape of some sort. Then, once in the scene editor, your curve/shape is rotated and duplicated in a circle as many times as it needs to until it meets again. 
翻译一下：lathe 编辑器在Zam 3D中是一个允许你绘制曲线或者其他图形的特殊功能。然后，你一旦转换到场景编辑器中，你的曲线或者图形将会被旋转复制直到满足要求位置。
我们在lathe中绘制这样一个曲线，来看一下具体的效果：
我们在lathe 编辑器中绘制如下的曲线，注意要沿着绿线，即y轴
![](https://images.cnblogs.com/cnblogs_com/alexis/20101230Zam3D02.jpg)
则，我们到场景编辑器中就看见这样的效果，很是神奇。
![](https://images.cnblogs.com/cnblogs_com/alexis/20101230Zam3D03.jpg)
**5.2 lathe 属性面板**
在场景编辑器中，我们选中刚刚Zam 3D自动绘制的图形，则属性面板会自动选择为lathe，如下图
![](https://images.cnblogs.com/cnblogs_com/alexis/20101230Zam3D04.jpg)
扫描角度（Sweep Angle）：调整扫描的角度，默认是360，即一圈全扫了，我们将其改变为150，并在旋转轨迹球旋转角度，会发现上面完整的图形被切割了，如下图
![](https://images.cnblogs.com/cnblogs_com/alexis/20101230Zam3D05.jpg)
实际上是，在扫描绘制的时候就绘制了150度的角度
分割半径：我们来看一下两张对比图，就知道这个是什么意思了
![](https://images.cnblogs.com/cnblogs_com/alexis/20101230Zam3D06.jpg)
将半径调小 
![](https://images.cnblogs.com/cnblogs_com/alexis/20101230Zam3D07.jpg)
还有一个辐射状是否平滑的勾选框
**5.3 从模型库中选择lathe**
我们可以在场景编辑器中选择自带的lathe进行编辑，自带的lathe还是蛮漂亮的，如下图
![](https://images.cnblogs.com/cnblogs_com/alexis/20101230Zam3D08.jpg)
