# GDC2016: Animating With Math - 逍遥剑客 - CSDN博客
2016年04月21日 23:15:21[xoyojank](https://me.csdn.net/xoyojank)阅读数：7037
Unity和UE4的Material Editor中都有类似VertexOffset的功能, 可以在VertexShader中驱动顶点, 做一些程序控制的顶点动画 
![这里写图片描述](https://img-blog.csdn.net/20160421223646008)
用顶点动画的原因: 骨架是共享的, 骨骼动画是共享的等, 同时节省了骨骼数和动画师的工作量 
![这里写图片描述](https://img-blog.csdn.net/20160421223833118)
使用cos三角函数+Time是基本的技巧, 配合顶点色控制每个部分的权重 
![这里写图片描述](https://img-blog.csdn.net/20160421224304135)
材质参数暴露出来能够使用程序逻辑进行动态控制 
![这里写图片描述](https://img-blog.csdn.net/20160421224500278)
顶点色的RGBA通道可以都用上, 进行不同的权重控制, 配合随时间变化的三角函数值, 可以做出蠕动的效果 
![这里写图片描述](https://img-blog.csdn.net/20160421224833170)
其实水面的做法跟这个很类似 
![这里写图片描述](https://img-blog.csdn.net/20160421224904271)
自从用了Unreal的MaterialEditor, 都懒得直接写Shader了 
![这里写图片描述](https://img-blog.csdn.net/20160421225043999)
另外一个应用就是做头发的动画 
![这里写图片描述](https://img-blog.csdn.net/20160421225219806)
同样的, 顶点红色通道可以控制移动动的权重 
![这里写图片描述](https://img-blog.csdn.net/20160421225303136)
蓝色通道能够控制移动的时间 
![这里写图片描述](https://img-blog.csdn.net/20160421225439982)
模型的运动也可以通过顶点色进行控制 
![这里写图片描述](https://img-blog.csdn.net/20160421230027208)
甚至是面部的结节形状也可以通过顶点色进行微调 
![这里写图片描述](https://img-blog.csdn.net/20160421230343535)
我觉得口型动画也可以这么做 
![这里写图片描述](https://img-blog.csdn.net/20160421230512434)
必要时需要外部控制的材质参数进行配合 
![这里写图片描述](https://img-blog.csdn.net/20160421230623998)
![这里写图片描述](https://img-blog.csdn.net/20160421230736380)
比如这个摆动的动画 
![这里写图片描述](https://img-blog.csdn.net/20160421230827203)
![这里写图片描述](https://img-blog.csdn.net/20160421230853672)
只用顶点色是不够的 
![这里写图片描述](https://img-blog.csdn.net/20160421230926173)
配合关键帧参数进行控制 
![这里写图片描述](https://img-blog.csdn.net/20160421231017627)
其实除了摆动之外, 还可以实现旋转的效果 
![这里写图片描述](https://img-blog.csdn.net/20160421231138038)
一些自转完全可以使用材质来实现, 免去了程序逻辑驱动或骨骼动画 
![这里写图片描述](https://img-blog.csdn.net/20160421231303617)
举个例子 
![这里写图片描述](https://img-blog.csdn.net/20160421231405695)
最后是用到的工具 
![这里写图片描述](https://img-blog.csdn.net/20160421232805230)
