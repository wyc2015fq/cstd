# 超火的漫画线稿上色AI出新版了！无监督训练，效果更美好 | 代码+Demo - 知乎
# 



> 夏乙 安妮 编译整理
量子位 出品 | 公众号 QbitAI

给喜欢的动漫形象建个了收藏夹，里面收集她的各种图片……懂，谁没几个喜欢的动漫萌妹呢。

一些手绘线稿也很可爱，但黑白配色总会略显单调。
![](https://pic3.zhimg.com/v2-e7a7a6e5ce9051948a6fb15e6207fbf6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='521' height='669'></svg>)
**△** 请记住这张线稿

半年前线稿上色AI style2paints的破壳曾让自动上色火了一阵。好消息是，昨天，升级版本style2paints 2.0也问世了！

Demo也一并放出，用户可以随意试玩~
![](https://pic1.zhimg.com/v2-466d94929b89a9c80f2f06d366880dbc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='707'></svg>)
**△** Demo界面

据作者说，style2paints 2.0比一代效果更好，而且模型的训练过程是无监督的。

**效果惊艳**

**只需线稿一张，轻点一下，就能给你珍藏的心爱萌妹上个色。**
![](https://pic2.zhimg.com/v2-1aaa4d0eff02ac4727292cfd957a5bd5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='562' height='660'></svg>)
**△** 上面线稿的转换效果

赞赞的！不仅如此，换一张其他的彩色参考图，也能将线稿转化成另一种颜色风格。

比如我们输入下面这张参考图：
![](https://pic3.zhimg.com/v2-cc3b7933a95abc45a3eace49e0e52bf2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='300' height='449'></svg>)
上面那张示例图就变成了下面的效果——
![](https://pic1.zhimg.com/v2-57caeb77707aa465c035eaed5c00ec4c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='610' height='798'></svg>)
还可以从参考图中选取颜色，然后用笔在线稿的某个区域上点一下，AI就会按你的提示去重新填充——
![](https://pic2.zhimg.com/v2-863be6563a9c1e81867b6d920c1bfba5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='630' height='794'></svg>)
支持微调的线稿上色软件不止一家，不过作者介绍说，和同类软件相比，style2paints的“提示笔”是最精确的，用户可以用3×3的笔迹，在1024×2048的画面上，控制13×13的区域。

为了确保最终画作颜色协调，style2paints还玩了一点小心机：用户不能自己输入颜色，只能从参考图上选。

**△** 使用教程 

**并不简单**

**玩得尽兴之后，你可能会说，风格迁移嘛，我们见得多了~**

但是，把黑白照片变成彩色照片，和把没有阴影高光的线稿变成彩色图画难度不同。

这种由纯线条构成的草图不包含图案的明暗和纹理。也就是说，AI需要自动脑补出这些信息。

何况style2paints作者对线稿上色的期望，实在是比较高，并不是在线条之间填一些颜色就完事了：

> 在一幅好漫画里，妹纸的眼睛要像星星一样闪亮，脸颊要弥漫着红晕，皮肤要精致迷人。
![](https://pic2.zhimg.com/v2-9219df2658ad2e1953b92db23bcf97a9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1024' height='704'></svg>)
△ 大概是这个效果吧

这怎么做得到！

style2paints 2.0目前还没有相关论文放出，只有代码可供参考。不过，今年6月问世的第一版其实效果还可以，实现了基于语义信息迁移的颜色提示，让上色效果更加和谐。

具体的原理可以看当时的论文Style Transfer for Anime Sketches with Enhanced Residual U-net and Auxiliary Classifier GAN
，来自苏州大学的三位作者Lvmin Zhang, Yi Ji, Xin Lin介绍了怎样将集合的剩余U-Net样式应用到灰度图中，并借助分类器生成的对抗网络(AC-GAN)自动为图像上色。生成过程迅速，效果尚好。

论文地址： [https://arxiv.org/abs/1706.03319](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1706.03319)

**不一样的2.0**

**style2paints第一版推出之后非常火，量子位的不少朋友都上手玩过。**

不过，他们的主要竞品paintschainer不停迭代，后来，上色的效果渐渐超越了style2paints 1.0，于是，作者们也坐不住了，开始研究新方法，更新了一版出来。

那么，2.0和1.0有什么不一样吗？

巧了，国外的网友也很关心。作者在Reddit上回答说，和上一版相比，style2paints 2.0大部分训练都是纯粹无监督，甚至无条件的。

也就是说，在这个模型的训练过程中，除了对抗规则之外没有添加其他的人工定义规则，没有规则来强迫生成器神经网络照着线稿画画，而是靠神经网络自己发现，如果遵照线稿，会更容易骗过鉴别器。

pix2pix、CycleGAN等同类模型为了确保收敛，会对学习对象添加l1 loss，鉴别器接收到的数据是成对的[input, training data]和[input, fake output]。而style2paints 2.0模型的学习目标和经典DCGAN完全相同，没有添加其他规则，鉴别器收到的也不是成对的输出。

作者说，让这样一个模型收敛其实是很难的，何况神经网络这么深。

不过你看，结果还不错。

**线稿上色的江湖**

**线稿上色的程序，其实有很多，比如说：**

**Paintschainer**[https://paintschainer.preferred.tech/index_en.html](https://link.zhihu.com/?target=https%3A//paintschainer.preferred.tech/index_en.html)

**Deepcolor**[https://github.com/kvfrans/deepcolor](https://link.zhihu.com/?target=https%3A//github.com/kvfrans/deepcolor)

**Auto-painter**[https://arxiv.org/abs/1705.01908](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1705.01908)

除了paintschainer，其他的同类产品其实作者是不太看得上的。

他说，很多亚洲论文都号称能迁移漫画风格，但是仔细看论文，会发现他们所谓的“新方法”就是个调整过的VGG，虽说VGG在风格迁移任务上普遍表现不错，但是用在漫画上，效果总是不怎么好。

还是得靠GAN，而且得允许用户上传风格参考图，像Prisma那样从莫奈梵高里选可不行，莫奈梵高又不会画漫画。

**试试不？**

上手玩一下Demo：[PaintsTransfer](https://link.zhihu.com/?target=http%3A//paintstransfer.com/)

论文还没出来，不过可以看源代码呀：
[https://github.com/lllyasviel/style2paints](https://link.zhihu.com/?target=https%3A//github.com/lllyasviel/style2paints)

[https://zhuanlan.zhihu.com/p/29331219](https://zhuanlan.zhihu.com/p/29331219)




— 完 —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


