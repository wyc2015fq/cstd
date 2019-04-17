# Shader山下（二十一）多重变体（Multiple Variants） - 凯奥斯的注释 - CSDN博客





2016年10月21日 23:34:08[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：4295
所属专栏：[Shader山下](https://blog.csdn.net/column/details/12981.html)









# [Shader山下（二十）编译指令（Compilation Directives）](http://blog.csdn.net/ecidevilin/article/details/52879485)介绍了如何使用编译指令，本文就专文介绍一下多重编译（multi_compile）指令。

我们可以使用multi_compile来为shader创建多个稍微有点区别的shader变体。这个Shader被称为宏着色器（mega shader）或者超着色器（uber shader）。实现原理：根据不同的情况，使用不同的预处理器指令，来多次编译Shader代码。

在Unity里面，可以使用：



```cpp
#pragma multi_compile
#pragma shader_feature
```
两个指令来实现。适用于VF着色器和表面着色器。


运行时，会根据材质（Material）的关键字（Material的对象方法EnableKeyword和DisableKeyword）或者全局着色器关键字（Shader的类方法EnableKeyword和DisableKeyword）。

注：之前在[Shader山下（十二）材质属性绘制器](http://blog.csdn.net/ecidevilin/article/details/52801009)里，我们已经介绍了多重编译的运用





使用multi_compile指令：



```cpp
#pragma multi_compile TEST_OFF TEST_ON
```

这样会编译两个Shader变体，一个是TEST_OFF的版本，一个是TEST_ON的版本。运行的时候会根据材质关键之或者Shader全局关键字判断应该使用哪个，如果两个关键字都为false，那么会使用第一个（TEST_OFF）变体。


我们也可以创建多个关键字：



```cpp
#pragma multi_compile TEST_A TEST_B TEST_C TEST_D
```

或者只创建一个关键字，这时候就需要使用__来代替另一个：




```cpp
#pragma __ TEST_KEY_ON
```
这样依然会生成两个shader变体，一个啥都没定义，另外一个定义了TEST_KEY_ON。






我们还可以通过多行指令来组合关键字：



```cpp
#pragma multi_compile A B C
#pragma multi_compile D E
```
这样就会生成六个shader变体（A+D、B+D、C+D、A+E、B+E、C+E）。但是这样shader变体的数量就可能会变得很多，10行multi_compile，每行有两个关键字，那么就会有1024个shader变体。



除了multi_compile之外，还有另外一个类似的指令shader_feature，唯一的区别在于shader_feature不会将不用的shader变体添加到程序中去。shader_feature更适用于材质的关键字，而multi_compile更适用于代码设置的全局关键字。

示例：



```cpp
#pragma shader_feature TEST_STUFF
```

这是下面这种写法的简写形式：




```cpp
#pragma shader_feature __ TEST_STUFF
```




注：Unity中最多可以有128个关键字，但是Unity自己已经定义了一部分了，所以实际上会更少。所以使用multi_compile和shader_feature的时候，就需要特别注意不要超过这个限制。




Unity自身还内建一些multi_compile的快捷写法：

multi_compile_fwdbase：为基本前向渲染通道编译多个变体，不同的变体可能需要处理不同的光照贴图类型，或者一些使用了主平行光的阴影，而另一些禁用了。（参考[Unity渲染管线](https://docs.unity3d.com/Manual/SL-RenderPipeline.html)）

multi_compile_fwdadd：为附加前向渲染通道编译多个变体，不同的变体可能需要处理不同类型的光源——平行光、聚光灯或者点光源，亦或者它们附带cookie纹理的版本。

multi_compile_fwdadd_fullshadows：同上，不过包含了可以让光源拥有实时阴影的功能。

multi_complie_fog：为处理不同的雾效类型（off/linear/exp/exp2）扩展了多个变体。




大部分内建快捷写法会导致许多shader变体，如果某些不需要使用，那么可以使用#pragma skip_variants来忽略它们。



```cpp
#pragma multi_compile_fwdadd
// will make all variants containing
// "POINT" or "POINT_COOKIE" be skipped
#pragma skip_variants POINT POINT_COOKIE
```





使用multi_compile的目的一般是为了创建一些fallback或者简化shader，它们在同一个目标平台（例如OpenGL ES）的高端硬件和低端硬件上都可以高效地运行。这种时候，我们可以使用shader硬件变体来为不同性能等级的硬件创建一个特殊优化的变体集合。

为了生成shader硬件变体，需要添加#pragma hardware_tier_variants*renderer*，这里*renderer*表示某个可运行的渲染平台。使用这个指令，Unity会对每个shader生成3个shader变体（其他关键字会不起作用），而对应的关键字是：

UNITY_HARDWARE_TIER1

UNITY_HARDWARE_TIER2


UNITY_HARDWARE_TIER3


在编辑器里，可以通过Graphics Emulation菜单来测试他们。

注：这些shader并不受Quality Setting的影响，它们纯粹是通过检测GPU的相关性能来决定的。








