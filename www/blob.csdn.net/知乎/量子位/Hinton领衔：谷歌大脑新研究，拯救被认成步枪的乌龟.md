# Hinton领衔：谷歌大脑新研究，拯救被认成步枪的乌龟 - 知乎
# 



> 栗子 安妮 编译整理
量子位 出品 | 公众号 QbitAI

最近，深度学习之父Geoffrey Hinton带领的谷歌大脑团队，提出了一种防御**对抗攻击**的新方法。

一种叫作DARCCC的技术，能将重构图像和输入图像作对比，继而识别出对抗图像，检测系统有没有受到攻击。

对抗攻击，是一种专职欺骗图片识别AI的方法。此前，那个广为流传的AI将乌龟识别成步枪的操作，就是对抗攻击的杰作。
![](https://pic3.zhimg.com/v2-1f639f4af9cacbf581009b7b76b2f206_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
悄悄修改图片的纹理，就可以在人类毫无察觉的情况下，骗过AI。

不过这一次，对抗攻击遇到了对手。

## **利用对抗攻击的弱点**

虽然，对抗攻击是种高超的骗术，但也有弱点。

还以乌龟和步枪的故事为栗：

即便和步枪分在一类，**乌龟还是和步枪长得很不一样**。

Hinton团队就是利用它们之间的区别，来把那些分类错误的图像，从大部队里揪出来。

模型识别图像的时候，除了输出一个分类 (如乌龟/步枪) ，还会输出一个重构 (Reconstruction) 的图像。

如果是对抗图像，重构出来会和原图差别很大 (在AI眼里已靠近步枪) 。若是未加篡改的真实图像，重构结果应该和输入图像 (乌龟) 很接近：
![](https://pic3.zhimg.com/v2-2d719c649d532f5181fd4b62ff8611c6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='518' height='368'></svg>)△ 受到白盒攻击之后，AI看到的东西，已经变了
所以，要做一个识别算法，就先给验证集的**重构误差** (Reconstruction Error) 定义一个阈值。

只要一张图的重构误差超过这个阈值，就把它标记成**对抗图像**。



![](https://pic1.zhimg.com/v2-6d85d9fb78f602a2c1578875044e0064_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='362'></svg>)△ 输入与重构之间的距离
如此，这个算法就可以识别出，系统有没有受到不明力量的攻击。

## **魔高一尺，道高一丈**

随后，研究人员将DARCCC扩展到更多标准图像分类网络中，进一步探索这种检测方法在黑盒攻击和典型的白盒攻击下是否有效。

Frosst等人选用MNIST、Fashion-MNIST和SVHN三个数据集，进行下一步：**有效性验证**。这一次，研究人员用了三种常用的白盒攻击方法。

前两种是白盒攻击中的常用的FGSM（Fast Gradient Sign Method）和BIM（Basic Iterative Methods）算法，这两次的结果让研究人员兴奋，因为这两种日常方法都没有攻击成功。
![](https://pic2.zhimg.com/v2-46c591cf37092fadaafcb5439615031d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='728' height='298'></svg>)
虽然DARCCC抵挡住FGSM和BIM的攻势，但在第三种白盒攻击面前，还是败下阵来。

这种更强大的白盒攻击，叫做R-BIM（Reconstructive BIM）。可以把重构损失计算在内，不断迭代地扰乱图片。

这样一来，就算是对抗图像，AI还是可以为它生成一个优雅的重构。

系统就没有办法检测到攻击了。

这是在MNIST和fashionMNIST数据集中检测时的结果：
![](https://pic1.zhimg.com/v2-ac7a684fb26d518f63fc824f922ee01c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='730' height='310'></svg>)
当用SVHN数据集检测时，结果如下：
![](https://pic1.zhimg.com/v2-3c443258a234c2743389ef81b4ebce58_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='317'></svg>)
## **传送门**

目前，这篇论文DARCCC: Detecting Adversaries by Reconstruction from Class Conditional Capsules已经被NeurIPS的安全Workshop收录。

论文地址：
[Detecting Adversaries by Reconstruction from Class Conditional Capsules​arxiv.org](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1811.06969)
推特原文：
[https://twitter.com/nickfrosst/status/1064593651026792448​twitter.com](https://link.zhihu.com/?target=https%3A//twitter.com/nickfrosst/status/1064593651026792448)
— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！

相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


