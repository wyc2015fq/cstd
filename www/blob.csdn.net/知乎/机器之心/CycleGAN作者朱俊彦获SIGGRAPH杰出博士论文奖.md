# CycleGAN作者朱俊彦获SIGGRAPH杰出博士论文奖 - 知乎
# 



**机器之心报道。**

> 计算机图形顶级会议 ACM SIGGRAPH 2018 即将于 8 月 12-16 日在加拿大温哥华举行。在大会开始前，部分奖项结果已经揭晓。我们刚刚得到消息：毕业于加州大学伯克利分校的朱俊彦（Jun-Yan Zhu）获得了大会的最佳博士论文奖。
![](https://pic3.zhimg.com/v2-3aae161b2f83bbd9ad74af44b7cb2c32_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='473'></svg>)
朱俊彦于 2012 年获得清华大学计算机科学系的工学学士学位，在 CMU 和 UC Berkeley 经过 5 年学习后，于 2017 年获得 UC Berkeley 电气工程与计算机科学系的博士学位，他的导师是 Alexei Efros。朱俊彦的博士研究由一项 Facebook 奖学金支持。朱俊彦目前是 MIT 计算机与人工智能实验室（CSAIL）的一名博士后研究员。
- 朱俊彦个人主页地址：[http://people.csail.mit.edu/junyanz/#sect-publications](https://link.zhihu.com/?target=http%3A//people.csail.mit.edu/junyanz/%23sect-publications)
- 博士论文地址：[http://people.csail.mit.edu/junyanz/pdf/thesis_highres.pdf](https://link.zhihu.com/?target=http%3A//people.csail.mit.edu/junyanz/pdf/thesis_highres.pdf)
- CycleGAN 项目地址：[https://github.com/junyanz/CycleGAN](https://link.zhihu.com/?target=https%3A//github.com/junyanz/CycleGAN)

在获奖结果公布后，GAN 发明者 Ian Goodfellow 第一时间献上祝贺。
![](https://pic3.zhimg.com/v2-35c37b7c5718121aa30c7d448f5240fe_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='364'></svg>)
朱俊彦博士是计算机图形学领域现代机器学习应用的开拓者。他的论文可以说是第一篇用深度神经网络系统地解决自然图像合成问题的论文。因此，他的研究对这个领域产生了重大影响。他的一些科研成果，尤其是 CycleGAN，不仅为计算机图形学等领域的研究人员所用，也成为视觉艺术家广泛使用的工具。
![](https://pic2.zhimg.com/v2-22afcc86427b506976447983686ce06d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='571' height='749'></svg>)朱俊彦博士论文封面，他的博士生导师为 Alexei A. Efros
以数据驱动的图像合成领域的一个关键问题是如何确保合成后的图像看起来真实。在论文的第 I 部分，朱俊彦采用一种判别方法来解决这类问题的一个案例，他训练一个分类器来评估合成图像的逼真度。由于难以获取足够的人工标注训练数据来判断图像是否真实，他学习对真实图像和自动生成的合成图像进行分类，不管这些图像看起来真实与否。他惊奇地发现：得出的分类器可以预测新的合成图像的逼真度。此外，逼真度分数可通过学得的变换来迭代更新图像，进而改善合成图像的逼真度。该研究可以被视为条件生成对抗网络（GAN）架构的「先锋」。他还开发了一种类似的判别学习方法，以改善人像的照片美感（SIGAsia 2014）。

在第二部分中，作者使用相反的生成方法建模自然图像，将图像编辑工具的输出控制在该流形上。他基于典型的图像平均模型（image averaging model，SIGGRAPH 2014）和近期的生成对抗模型，构建了实时数据驱动探索和编辑界面。后者起到作用，相关软件 iGAN 是 GAN 首次应用于实时应用程序中，它对 GAN 在社区中的流行起到很大作用。
![](https://pic2.zhimg.com/v2-6f295598bcb44ff8e7314da8ba6130a1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='487'></svg>)给定两个无序图像集 X 和 Y，CycleGAN 可以自动对它们进行互相「翻译」
在第三部分中，作者结合他在早期研究中获得的经验，开发了一套新型图像到图像的转换算法。其中非常重要的是 CycleGAN 框架（ICCV 2017），它变革了基于图像的计算机图形学，可作为一种通用框架将一组图像中的视觉风格迁移到其它图像。例如，将夏天转化为冬天、将马转换为斑马及利用计算机图形渲染生成真实图像等。

该研究首次展示了艺术收藏品的风格迁移效果（例如，使用所有梵高的作品，而不是只用《星月夜》），并将绘画转换为照片。自发布以来，CycleGAN 在短短时间内就已经被应用到了很多不同的问题中，其范围远远超越了计算机图形学，从生成合成训练数据（计算机视觉）到将 MRI 影像转换为 CT 扫描影像（医学影像），再到 NLP 和语音合成的应用。除博士论文外，他还提出了基于学习的交互式着色方法（SIGGRAPH 2017）和光场摄像方法（SIGGRAPH 2017）。

除了在顶级图像与视觉会议上发表的文章以外，朱俊彦的成果在其他方面也颇具影响。他的研究多次出现在大众媒体上，包括《纽约客》、《经济学人》、《福布斯》、《连线》等。朱俊彦在推进研究复现方面堪称典范，这令研究人员和从业者更容易「站在他的肩膀上」。他的许多项目都是开源的，影响力也很大，他在 GitHub 上的项目已经获得 22000 次收藏和 1900 个关注者。最令人印象深刻的是，他的代码不仅被研究人员和开发人员广泛使用，而且还被视觉艺术家使用（例如 Twitter 上的 #cycleGAN）。
![](https://pic1.zhimg.com/v2-bae8f56b187e4b99978f4389278a9bb4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='793'></svg>)
朱俊彦此前也获得了多个学术奖项，仅在 2018 年，他就获得了 UC Berkeley 颁发的 David J. Sakrison Memorial Prize，以及英伟达的 Pioneer Research Award。




朱俊彦还曾获得过以下奖项和奖学金：
- CVPR Outstanding Reviewer (2017)
- Facebook Fellowship (2015)
- Outstanding Undergraduate Thesis in Tsinghua University (2012)
- Excellent Undergradua
- te Student in Tsinghua University (2012)
- National Scholarship, by Ministry of Education of China (2009 and 2010)
- Singapore Technologies Engineering China Scholarship (2010, 2011, and 2012)

参考内容：[学界 | 让莫奈画作变成照片：伯克利图像到图像翻译新研究](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650725257%26idx%3D4%26sn%3Dbf367ff90e03f8189f7c67ae0e5ab76f%26chksm%3D871b1ff7b06c96e1e355d8b360abd0c256af04e2ba72a8d2a3364bfea8ff80b347a734d17e9d%26scene%3D21%23wechat_redirect)




参考链接：[https://www.siggraph.org/outstanding-doctoral-dissertation-award-jun-yan-zhu](https://link.zhihu.com/?target=https%3A//www.siggraph.org/outstanding-doctoral-dissertation-award-jun-yan-zhu)


