# Google面部追踪468点，无缝贴合3D人脸，手机上就能玩：新版ARCore快去试试 - 知乎
# 



> 圆栗子 发自 凹非寺 
量子位 整理 | 公众号 QbitAI
![](https://pic3.zhimg.com/v2-56e6fa3fbb6efbbe1ad3547066be3ede_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='300' height='316'></svg>)
谷歌发布了ARCore 1.7，人脸贴合几乎**天衣无缝**。

因为，有了**面部增强API**，**不用依靠深度传感器**，就能让AR效果紧贴3D人脸，手机端也没有压力。

许多安卓用户已经开心地玩耍了起来。
![](https://pic4.zhimg.com/v2-6d243b582e7d60c427c944316ecddc5f_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='270'></svg>)
除此之外，**有了视觉上的直观导引**，新的ARCore更加易于开发者食用：上图是在提示游戏开发者，不要让玩家倒着走。

## **468个点，穷追不舍**
![](https://pic2.zhimg.com/v2-2e16b27e05c3fbd5c043201444a5c285_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='200' height='293'></svg>)
谷歌团队说，面部增强API (Augmented Faces) 提供了高质量的**3D网格**，包含**468个追踪点**。

脸部追踪分成两步：

> 先定义**中心姿势** (Center Pose) ，是指人头的物理中心。所以说，这个点不在表面而在里面。
然后是**脸部网格** (Face Mesh) ，包含数百个顶点。每个点的位置，都是参照物理中心点来定义的。
![](https://pic4.zhimg.com/v2-361ef81b8a87fe986c79e27bd3e90847_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='544'></svg>)
这个API把人脸分为**三个区域**来识别姿势：**左前额** (Left Forehead) ，**右前额** (Right Forehead) ，以及**鼻尖** (Nose Tip) 。

把中心姿势、脸部网格以及三个区域结合到一起，ARCore就这样支持精细的三维AR效果了。

比如，把3D狐狸耳朵和鼻子，美好地贴到人类脸上。
![](https://pic2.zhimg.com/v2-0639ddabdc21e7197f2ac03da2ac8025_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='990' height='855'></svg>)
这三个部分，有各自独立的骨骼，可以单独移动，去找它们该在的位置。

除此之外，也有眼影、雀斑和其他细节：
![](https://pic2.zhimg.com/v2-e687bb5de3306fa4193acebc815bcd29_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='383' height='253'></svg>)
不知道狐狸妆的效果怎样，但这个脸上满是数字的男子，看上去还是很开心的：
![](https://pic4.zhimg.com/v2-1176baeb48d19e3f518f13a002d92863_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='320' height='471'></svg>)
你也可以去Google Play下个新版应用试一下。
![](https://pic2.zhimg.com/v2-0d7caa95bec860243317d8e6743ce38d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='350'></svg>)
## **来吧，开发者**

新版ARCore，增加了一个叫做**ARCore Elements**的版块。把各种预置的UI组件整合在里面，让开发者们轻易上手。

Elements分成两个部分。

一是**寻找平面**：
![](https://pic4.zhimg.com/v2-c1093da3d9eec8690581622a345bde47_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='270'></svg>)
二是**操纵物体**：
![](https://pic2.zhimg.com/v2-393314228d0be3128e647a5b009a40f5_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='270'></svg>)
除此之外，Elements还给了游戏开发者一些**温柔的提醒**。

比如，物体不要太小，要设置成玩家容易点击到的尺寸：
![](https://pic1.zhimg.com/v2-282fc8ec8b60484d7ea7ad23ae85cb34_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='270'></svg>)
比如，如果小鸟飞出了屏幕，可以加个方向标，帮用户追踪屏幕之外的物体运动：
![](https://pic1.zhimg.com/v2-36a047f49fa55aa1d1cd5efa41d5683c_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='270'></svg>)
这个应用，也可以到Google Play去下载：
![](https://pic1.zhimg.com/v2-753ad038dab39010967902f6b1afd46c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='358'></svg>)
里面的世界很丰富，有清澈的流水，有兴奋的恐龙……就算不是开发者，也不妨前往一游。

下载SDK：

[https://developers.google.com/ar/develop/downloads](https://link.zhihu.com/?target=https%3A//developers.google.com/ar/develop/downloads)

谷歌AR的各种SDK列表：

[https://github.com/google-ar](https://link.zhihu.com/?target=https%3A//github.com/google-ar)

更多详情：
[https://developers.googleblog.com/2019/02/new-ui-tools-and-richer-creative-canvas.html](https://link.zhihu.com/?target=https%3A//developers.googleblog.com/2019/02/new-ui-tools-and-richer-creative-canvas.html)
![](https://pic2.zhimg.com/v2-c93763b2526bd9c12fa3fecaf48c11dd_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='480'></svg>)△ 开发者自制效果
—**完**—

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤


