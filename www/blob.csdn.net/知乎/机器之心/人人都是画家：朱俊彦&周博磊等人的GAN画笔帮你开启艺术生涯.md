# 人人都是画家：朱俊彦&周博磊等人的GAN画笔帮你开启艺术生涯 - 知乎
# 



> 既然 GAN 可以用来给黑白[线稿上色](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650735415%26idx%3D3%26sn%3De272154488c4e28b18b0c1ceb9f96201%26chksm%3D871ac749b06d4e5f3d0357888ec455cb1b70f5ca405a37a61e09fcaa93766275f0434732d2c0%26scene%3D21%23wechat_redirect)、[填充纹理](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650723632%26idx%3D3%26sn%3D88f89ee39a145f4bf0c0add7a42fa4ca%26chksm%3D871b114eb06c985809a27507310d1d1394f908c9cd231d3bf0c9180f462d2fedd1a60f505ab6%26scene%3D21%23wechat_redirect)，那能不能直接用来画画？来自 MIT、港中文、IBM 等机构的 David Bau、朱俊彦、Joshua B. Tenenbaum、周博磊等人在论文 GAN Dissection 上放出的示意工具一时惊艳了众人：只要鼠标划过，GAN 就可以立即在你指定的地方画出云彩、草地、门和树等景物，并直接改变构图以保持图像的「合理性」。

选自MITCSAIL，作者： David Bau、朱俊彦、Joshua B. Tenenbaum等，机器之心编辑。

这个名为 GANpaint 的工具一出，立即引来了众人尝试，有人惊呼：「这工具让我这辈子第一次可以动手『画出』点东西了！」
![](https://pic4.zhimg.com/v2-991451063d72d64031ff05ddbcb447af_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='805' height='831'></svg>)网友「画作」之一：绿色屋顶的房子
让我们看看它是怎么操作的：
![](https://pic2.zhimg.com/v2-7283a7e97848ffab41a57e98c89b83bd_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='368' height='300'></svg>)- 工具链接：[http://gandissect.res.ibm.com/ganpaint.html?project=churchoutdoor&layer=layer4](https://link.zhihu.com/?target=http%3A//gandissect.res.ibm.com/ganpaint.html%3Fproject%3Dchurchoutdoor%26layer%3Dlayer4)

生成对抗网络（GAN）近期在很多现实世界任务中取得了巨大成就，很多 GAN 的变体都在样本质量和训练稳定性上获得了很大的进展。然而，对 GAN 的可视化和理解仍然是缺乏探究的课题。GAN 网络是如何表征视觉世界的？导致 GAN 输出结果失真的原因是什么？架构选择如何影响 GAN 学习？回答这些问题能够帮助我们发展新的理论洞察和更好的模型。

在 MIT CSAIL 主导的这项工作中，研究者展示了在单元、对象和场景级别上可视化和理解 GAN 的框架。研究者首先通过基于分割的网络剖析方法（network dissection method）识别出一组和对象概念很相关的可解释单元。然后通过评估人类干预（控制输出中的对象）的能力来量化可解释单元的因果效应。最后，通过将发现的对象概念嵌入到新图像中来测试这些单元和周围事物之间的语境关系。研究者展示了通过该框架实现的多种实际应用，从对比不同层、模型和数据集的内部表征，到通过定位和移除导致失真的单元来改善 GAN，从而实现交互式地控制场景中的对象。

该 GAN 解释工具已开源，以帮助同行研究者更好地理解他们的 GAN 模型。
- 项目地址：[https://github.com/CSAILVision/GANDissect](https://link.zhihu.com/?target=https%3A//github.com/CSAILVision/GANDissect)
![](https://pic3.zhimg.com/v2-9375afe9591ce68e12754fee71061316_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='320'></svg>)



研究者开发了一个网页应用展示 GANpaint app，来帮助读者更好理解其作用。如上图所示，选中「draw」功能和特定的对象，如 tree，就能在图中随意用画笔涂抹，GAN 会在涂抹区域自动生成树木。选中「remove」功能和特定的对象，如 grass，GAN 会在涂抹区域自动删除草地。读者可以自己尝试一下，探索更多有趣的现象。

目前小编发现，删除用「draw」添加到原始图像的目标是有效的，而想在原始图像上删除目标也可以。如下图所示，第一张是原图，第二张是删除树木和草地之后的图，效果还是可观的。不过对 door 这个目标的生成和删除似乎对环境背景的联系要求很高，失败率很高。
![](https://pic1.zhimg.com/v2-f0a8e3f29dd28b19fac69c190b16fc8c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='585'></svg>)![](https://pic2.zhimg.com/v2-ef8e7356bddeb7589284d7e0749ac3d9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='585'></svg>)
如下图所示，论如何快速绿化环境：
![](https://pic2.zhimg.com/v2-a83988fa6f41748b6ad222bce0618855_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='624'></svg>)![](https://pic2.zhimg.com/v2-759fa37e0dc4a3827f9d09295be8bffd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='646'></svg>)
或者宜居一点的：
![](https://pic3.zhimg.com/v2-ec774be1a9c4626e8243a41359026df2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='806' height='654'></svg>)
GAN paint app 的工作原理是直接激活或者去激活深度网络中的神经元组，这些神经元被训练用来生成图像。左边每个按钮（如 door、brick 等）都对应 20 个神经元。该 app 表明，通过学习绘画，GAN 网络还能学习树、门还有屋顶等对象。通过直接切换神经元，你可以观察该网络建模的视觉世界的结构。
- GANpaint 试用地址：[http://gandissect.res.ibm.com/ganpaint.html?project=churchoutdoor&layer=layer4](https://link.zhihu.com/?target=http%3A//gandissect.res.ibm.com/ganpaint.html%3Fproject%3Dchurchoutdoor%26layer%3Dlayer4)




**为什么用 GAN 绘画很有趣？**

计算机可以用两种方式绘画：

1. 利用它知道的对象来构图。

2. 记住一幅图像，然后重现。

近年来，创新的生成对抗网络（GANs, I. Goodfellow, et al, 2014）已经在生成逼真图像方面显示出了超凡的能力。然而，尚不清楚这些网络是学习构图，还是仅仅通过记忆像素图案来复现图像。

本文的 GAN Paint demo 和 GAN Dissection 方法证明 GAN 网络了解构图的一些知识。
![](https://pic2.zhimg.com/v2-83218bfa4fc9356ba176ff4a6a3323c1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='52'></svg>)
Unit 365 在 Progressive GAN (T. Karras, et al, 2018) 生成的教堂图上画树。
![](https://pic2.zhimg.com/v2-487e10cc813d82954ba66c7290edadad_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='52'></svg>)
Unit 43 画圆屋顶。
![](https://pic2.zhimg.com/v2-0deb3cebaaf3b50186d269a935aa23bd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='52'></svg>)
Unit 14 画草地。
![](https://pic1.zhimg.com/v2-332dabb8588836754a122c6a50f1b4d0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='52'></svg>)
Unit 276 画塔。

**论文：GAN Dissection: Visualizing and Understanding Generative Adversarial Networks**
![](https://pic4.zhimg.com/v2-a57147927bce587e0478488a4ba73ed7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='147'></svg>)
论文地址：[https://arxiv.org/pdf/1811.10597.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1811.10597.pdf)
![](https://pic4.zhimg.com/v2-0ee01c35e72223479d364a198c6b36a3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1007' height='650'></svg>)
图 1：概览：(a) Progressive GAN (Karras et al., 2018) 生成的若干室外教堂逼真图像。(b) 给定一个预训练的 GAN 模型（如 Progressive GAN），首先识别一组可解释单元，其特征图与不同图像中的对象类别区域高度相关。例如，layer4 中的一个单元可以定位视觉外观不同的树区域。(c) 通过强制激活为零来消融这些单元，量化消融的平均偶然效果。在这里，树被成功地从教堂图像中移除掉。(d) 可以把这些树单元插入到其它位置。同一组单元可以合成外观与周围环境兼容的不同的树。此外，该方法可以通过识别引起视觉假象的单元来诊断并改进 GAN。(e) 移除出现在 (f) 中的视觉假象，并通过消融「artifact」单元显著改善结果。(g) 详情请看 demo 视频。




**GAN Dissection 框架**

论文介绍了一个可视化和理解生成网络学得结构的框架。GAN dissection 可以回答以下问题：

1. 该网络是否学习匹配有意义概念的内部神经元？

2. 这些神经元组仅与对象相关，还是 GAN 使用这些神经元对对象进行推理？

3. 因果神经元（causal neuron）可以用来改善 GAN 的输出结果吗？
![](https://pic4.zhimg.com/v2-588b8a02f87f9c948485f070f56f953f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1014' height='541'></svg>)GAN dissection 使用分割网络 (T. Xiao, et al, 2018) 和剖析（dissection）方法 (D. Bau, et al, 2017)，寻找匹配有意义对象类别（比如树）的生成器单个单元。![](https://pic1.zhimg.com/v2-ea6fc70189e72802299a8c92658af818_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='953' height='436'></svg>)GAN 学习的神经元依赖它学习绘制的场景的类型：当学习会议室时，会出现商务正装神经元，当学习厨房时，会出现炉子神经元。
**每个神经元控制什么？**

为了验证神经元组能够控制对象的绘制，而不只是简单的相关，研究者干预网络内部，直接激活和去活化神经元。
![](https://pic1.zhimg.com/v2-050878528fa7bb6fe04f871925ea25e0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='414'></svg>)
一个惊人的发现是同样的神经元在不同的背景下控制特定的对象类别，即使该对象的最终外观截然不同。例如，同样的神经元可以控制「门」的概念，即使大石墙需要一扇朝左的沉重大门，而小木屋需要朝右的帘门。

该网络还能理解何时能够绘制对象。例如，在建筑物的合适位置激活「门」的神经元就可以添加一扇门。而在天空或树上执行同样的动作通常是无效的。该结构可以量化。
![](https://pic2.zhimg.com/v2-d23a192dbb019ded6bc3b48b2a7b8b1d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='428'></svg>)
上图：黄色框表示可激活以添加门的神经元位置。(d) 中绘制大门的方式是突出小门，但是在很多位置 GAN 拒绝绘制门。下图：GAN Paint 与普通绘图程序不同的原因。它不会总是按照你的意愿去做，它会把对象放在合适的位置。

**GAN 犯的错误可以调试和修复吗？**

理解网络的内部概念之所以重要，是因为对 GAN 网络的理解可以帮助改善该网络的行为。

例如，GAN 有时会生成非常不真实的图像，导致这些错误的原因之前是未知的。而这项研究发现，这些错误可能是由引起视觉假象的特定神经元组导致的。

通过识别和停用这些引起错误的神经元，我们可以进一步提高 GAN 输出结果的质量。如下图所示，第一张图展示的都是失真图像，后两张分别展示了修复的第一个和第二个图像。
![](https://pic3.zhimg.com/v2-16c0dc5c50f1b69688a51884f9bf5da6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='786' height='738'></svg>)
**大家来找茬**

即使研究者称其目标生成可以很好地结合背景信息，小编还是挖出了很多 bug。
![](https://pic4.zhimg.com/v2-fae9cffcbbaa628d19de296489dba853_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='567'></svg>)一半的天空被换成了砖头![](https://pic2.zhimg.com/v2-419bc8b6a50c1cb6397903c2a02e5281_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='960' height='696'></svg>)在天空中添加「砖头」画出的「海市蜃楼」![](https://pic1.zhimg.com/v2-8cf8619c22f1361d288bc195d2a6437c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='913' height='679'></svg>)飘在空中的树~![](https://pic1.zhimg.com/v2-c51533098f9493a0f70cae6f5f0299d8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='510'></svg>)
原图大片区域是干净的蓝天，然而在蓝天画了树之后……房子成了废墟……
![](https://pic3.zhimg.com/v2-c107e687d1ed6c21606708a5ff84be76_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='577'></svg>)







