# 深度学习「一键P图」：为原画无缝添加新元素 - 知乎
# 



选自arXiv，作者：栾福军等，机器之心编译。

> 把照片中的一个元素「复制粘贴」到绘画作品上，很简单？Nonono… 要想成品不像拼贴画，二者风格一致甚至融为一体可没有那么容易～来自康奈尔大学和 Adobe Research 的研究者提出一种 two-pass 算法，轻松实现快速无痕 P「画」。
- 论文链接：[https://arxiv.org/abs/1804.03189](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1804.03189)
- 项目GitHub：[https://github.com/luanfujun/deep-painterly-harmonization](https://link.zhihu.com/?target=https%3A//github.com/luanfujun/deep-painterly-harmonization)
![](https://pic4.zhimg.com/v2-ff9d5577daa0fdcc8596744a6ac6f04b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='900' height='853'></svg>)
## **引言**

图像合成是创建新视觉内容的关键操作。它帮助艺术家将现有材料合成新的作品，Man Ray 和 David Hockney 等艺术家使用该技术创造了很多杰出作品。合成可用于不同的语境中。照片拼贴等应用希望有明显的拼合痕迹，但其他应用希望使合成不易被人察觉，例如将一个对象添加到图像中，使之好像原本就在图像中一样。

人们开发了很多工具进行照片合成，如移除边界层 [PGB03]、匹配颜色 [XADR12] 或匹配细密结构 [SJMP10]。但是，没有适合绘画的工具。如果我们想在绘画作品上添加对象，选择就很有限了。你可以手动绘制该对象或使用绘画引擎 [CKIW15]，但这需要时间和技巧（并没有几个人会）。为照片设计的算法对于绘画生成的结果低于标准，因为它们无法处理绘画作品的画刷纹理和抽象化。使用现有的绘画风格化算法效果也不好，因为它们是为全局风格化设计的，而我们想要寻求的是颜色、纹理和结构属性的局部协调。

本论文中，研究者解决了这些挑战，使得人们可以将图像中的对象复制到绘画作品中，且合成作品看起来就像原作一样。研究者基于绘画风格化方面的近期研究 [GEB16] 构建方法，以协调粘贴过来的对象的外观，使之与原画作匹配。研究者的策略是将绘画作品的神经反应相关数据迁移到对象中，主要贡献是如何确定哪些数据需要迁移。与之前研究类似，本论文使用 VGG 神经网络 [SZ14] 的反应作为推动该过程的数据。本论文展示了空间一致性和尺度间一致性的重要性。即，迁移来自画作小片区域的数据比使用大量独立位置得到的结果更好。此外，保存网络层之间的神经反应关联性还可以提升输出质量。为了达到这两个目标，研究者引入了一种 two-pass 算法：第一阶段达到单一尺度的大致协调，作为第二阶段的起点，来实现精细的多尺度改善。图 1（右）显示本文方法与相关技术的对比结果。
![](https://pic3.zhimg.com/v2-a4c369b587207994e07facc066c6528a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='693' height='234'></svg>)
图 1：本论文提出的方法可以将元素与画作合成到一起，并使元素与画作相协调。左侧是给出的画作和元素，研究者展示了未调整的剪切复制结果、深度图像类比（Deep Imange Analogy）[LYY∗ 17] 和本论文提出方法的合成结果。

研究者在多个样本上执行了该方法。绘画合成是一项要求很高的任务，因为合成的风格要和原作风格一致，否则一眼就可以看出来。因此，单独看效果还不错的全局风格化技术结果在合成语境中可能不太好，因为合成元素和原作并列在一起，很容易就能看出细微差别。研究者进行了一项用户调研，结果显示其算法产生的合成结果通常被认为就是原作。




**绘画协调算法（Painterly Harmonization Algorithm）**

研究者设计了一种 two-pass 算法以达到绘画协调。之前的研究使用单程方法，如 Gatys et al. [GEB16] 将整个风格图像匹配到整个输入图像中，然后在格拉姆矩阵上使用 L2 范数来重建最终结果。Li & Wand [LW16a] 使用最近邻进行匹配，在激活向量上使用 L2 范数来重建最终结果。在早期实验中，研究者发现此类单程策略在本论文研究背景中效果并没有那么好，结果未达到预期。于是研究者开发了一种 two-pass 方法，第一阶段进行大致协调，第二阶段精细化视觉质量（算法 1）。
![](https://pic1.zhimg.com/v2-22c05dbc673d9b0c2a629c01b4580fb8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='421' height='278'></svg>)
第一阶段生成了一个接近期望风格的中间结果，但是研究者这时的意图并不是输出最高质量的输出。研究者放松了对质量的要求，设计了一个鲁棒算法，该算法可以处理大量不同的风格。这一阶段通过执行粘贴区域颜色、纹理属性与画作语义类似区域的对应属性的粗略匹配，达到了大致的协调。研究者找到独立于每一网络层的最近邻神经 patch（算法 3）来匹配粘贴区域和背景的神经反应。这提供了中间结果（图 2b），对于第二阶段来说是一个更好的起点。
![](https://pic3.zhimg.com/v2-5dd04fbe2696e226d826ef175ee50b86_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='425' height='310'></svg>)![](https://pic3.zhimg.com/v2-625722b6a446dae79afdfa35b2217042_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='432' height='413'></svg>)
第二阶段中，研究者从中间结果开始，聚焦于改善视觉质量。直观来看，由于中间图像和风格图像在视觉上是非常接近的，因此研究者对输出质量的要求更加严格。研究者研究捕捉图像局部纹理属性的单个中间层，生成一个对应映射，可用于移除空间异常值。之后研究者对该空间一致映射执行上采样，以获取网络的更精细层级，从而确保在每个输出位置上，任意尺度的神经反应都来自画作的同一位置（算法 4）。这使得纹理更连贯流畅，结果看起来更好（图 2c）。
![](https://pic3.zhimg.com/v2-d1847abea0cdae5a6362347f42bcefee_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='273' height='687'></svg>)![](https://pic1.zhimg.com/v2-f76cadd5a992ef875890d8c1928f4168_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='436' height='330'></svg>)
图 2：从差异巨大的输入和风格图像开始（a），研究者首先协调粘贴元素的整体外观（b），并细化结果，以精细化匹配纹理，移除伪影（c）。




**绘画估计器**

上述算法有两个影响输出风格属性的重要参数——风格和直方图权重（ws 和 whist）。我们观察到，不同的参数集根据风格化程度的不同为不同画作给出了最佳结果。例如，立体主义绘画往往包含多面的小区域，这些区域具有强烈而锐利的笔触，而文艺复兴鼎盛时期和巴洛克时期的绘画则更写实。研究者没有为每个输入调整参数，而是开发了一个训练过的权重估计器，使其权重选择方法更加鲁棒。

研究者训练一个绘画估计器来预测算法的优化参数，以便在背景画作风格更强烈时使用允许更深风格改变的参数，反之亦然。为了训练这种估计器，研究者将参数值分成三个类别（「弱」、「中」、「强」），并手动将每个绘画风格归入其中一个类别。表 1 显示了绘画风格的子集及其类别和权重值（其他风格在补充材料中）。
![](https://pic1.zhimg.com/v2-57d5b01d0ec346a580eeb81095e43164_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='403' height='116'></svg>)
表 1：所选艺术风格的权重。其他艺术风格和参数权重详见补充文档。最后的权重是使用本文研究者训练的绘画估计器网络对不同艺术风格计算出的线性插值。TV 权重是根据绘画图像的噪声水平单独计算的（第 4 部分）。




研究者从 [http://wikiart.org](https://link.zhihu.com/?target=http%3A//wikiart.org) 收集了 8 万幅绘画作品，并调整 VGG-16 网络 [SZ14] 对 18 种不同风格进行分类。训练结束后，去掉最后一个分类层，在基于风格类别的 softmax 层上使用加权线性插值输出 ws 和 whist 的浮点值，ws 和 whist 表示风格化程度。然后在优化中使用这些参数值（如表 1 所示）。
![](https://pic1.zhimg.com/v2-d38a0ef17132dfe0a1be1bf00b6bcfe8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='875' height='254'></svg>)
图 7：在蒙娜丽莎（b）上粘贴 Ginevra de' Benci（a）的脸时，泊松混合 [PGB03] 无法匹配二者的纹理（c），Mulitscale Harmonization [SJMP10] 添加纹理但没有复现画作裂缝（d），Image Melding [DSB∗ 12] 添加裂缝，但不是处处添加，例如右侧眼睛下方没有裂缝（e）。相比之下，本论文的结果在各处都产生了裂缝（f）。
![](https://pic4.zhimg.com/v2-6ba0088a626c15d9dbbac9887799f52b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='437' height='306'></svg>)
图 8：「编辑与否」用户调查的结果。绘画分类率越高，表明协调效果越好，因为用户无法识别研究者所做的编辑。大图标表示每个类别的平均值。
![](https://pic3.zhimg.com/v2-822f570ca8dc4acd34da6c6957a9fa46_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='413' height='151'></svg>)
图 9：「对比」用户调查的结果。在四种算法中，本研究的算法通常是最受欢迎的。
![](https://pic4.zhimg.com/v2-0b7f02ce76e347bcae93e271cf0de5b3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='403' height='246'></svg>)
图 10：热气球（左上）的典型对象协调结果。




**论文：Deep Painterly Harmonization**
![](https://pic3.zhimg.com/v2-60ebf090709fab07f5be06743dbb87ea_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1005' height='597'></svg>)
论文链接：[https://arxiv.org/abs/1804.03189](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1804.03189)

**摘要：**将图像中的一个元素复制粘贴到绘画作品中是一项难度很高的任务。在这一情境下，使用照片合成技术获得的结果欠佳，看起来像拼贴画。现有的全局绘画风格化算法在局部使用时性能也不好。我们使用一种专用算法解决了这些问题，该算法仔细确定要被迁移的局部数据。我们确保空间和尺度内数据的一致性，并展示了二者是生成高质量结果的关键。为了处理画作的不同抽象水平和类型，我们引入了一种技术，根据具体画作调整迁移参数。我们展示了该算法生成的结果显著优于照片合成或全局风格化技术，它使原本比较困难的有创造性的绘画编辑成为可能。


