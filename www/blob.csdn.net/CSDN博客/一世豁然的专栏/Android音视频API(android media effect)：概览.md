# Android音视频API(android.media.effect)：概览 - 一世豁然的专栏 - CSDN博客





2017年07月06日 10:38:29[一世豁然](https://me.csdn.net/Explorer_day)阅读数：822








提供允许您将各种视觉效果应用于图像和视频的类。 例如，您可以轻松修复红眼，将图像转换为灰度，调整亮度，调整饱和度，旋转图像，应用鱼眼效果等等。 该系统对GPU执行所有效果处理，以获得最佳性能。




为了获得最佳性能，效果直接应用于OpenGL纹理，因此您的应用程序必须具有有效的OpenGL上下文才能使用效果API。 您应用效果的纹理可能来自位图，视频甚至相机。 但是，纹理必须满足一些限制：


1、它们必须绑定到GL_TEXTURE_2D纹理图像

2、它们必须至少包含一个mipmap级别




“effect”对象定义了可应用于图像帧的单个媒体效果。 创建效果的基本工作流是：


1、从您的OpenGL ES 2.0上下文调用EffectContext.createWithCurrentGlContext（）。

2、使用返回的EffectContext调用EffectContext.getFactory（），返回一个EffectFactory的实例。

3、调用createEffect（），从EffectFactory传递一个效果名称，例如EFFECT FISHEYE或EFFECT VIGNETTE。




您可以通过调用setParameter（）并传递参数名称和参数值来调整效果的参数。 每种类型的效果接受不同的参数，这些参数用效果名称记录。 例如，EFFECT_FISHEYE有一个参数用于缩放的失真。





要对纹理应用效果，请在效果上调用apply（），并传入输入纹理，其宽度和高度以及输出纹理。 输入纹理必须绑定到GL_TEXTURE_2D纹理图像（通常通过调用glTexImage2D（）函数完成）。 您可以提供多个mipmap级别。 如果输出纹理没有绑定到纹理图像，它将被效果自动地绑定为GL_TEXTURE_2D和一个mipmap级别（0），它们的大小与输入的大小相同。





注意：有保证支持EffectFactory中列出的所有效果。 但是，所有设备都不支持外部库提供的一些附加效果，因此您必须首先检查是否通过调用isEffectSupported（）来支持来自外部库的所需效果。





一、类

[Effect](https://developer.android.google.cn/reference/android/media/effect/Effect.html)


效果是可以应用于图像帧的高性能转换。





[EffectContext](https://developer.android.google.cn/reference/android/media/effect/EffectContext.html)


EffectContext保持所有必需的状态信息在Open GL ES 2.0上下文中运行效果。





[EffectFactory](https://developer.android.google.cn/reference/android/media/effect/EffectFactory.html)


EffectFactory类定义可用效果的列表，并提供检查和实例化功能的功能。




