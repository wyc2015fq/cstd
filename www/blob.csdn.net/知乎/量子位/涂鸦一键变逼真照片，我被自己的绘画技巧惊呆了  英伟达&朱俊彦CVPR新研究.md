# 涂鸦一键变逼真照片，我被自己的绘画技巧惊呆了 | 英伟达&朱俊彦CVPR新研究 - 知乎
# 



> 安妮 发自 凹非寺
量子位 出品 | 公众号 QbitAI

平静的水面上，剧变突然出现。
![](https://pic4.zhimg.com/v2-89e7ba091402fb92f709cbf8c159cefb_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='586'></svg>)
一条黑色弧线从半空延伸而下，划过半个圈。继而自己填满，俨然一座岩山，就这样落在水中间。

水面漾起波纹。揉揉眼睛，绝不是看花了眼：你看那岩山纹路崎岖，倒影也是清晰可见。

第二座、第三座，也接踵而至。

透过现象看本质，控制这一切的，都是电脑前那个手握鼠标乱涂鸦的程序员。
![](https://pic2.zhimg.com/v2-6b4cc96e843293276e396cda21faae01_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='365'></svg>)
英伟达程序员の现实扭曲力场，发动！
![](https://pic4.zhimg.com/v2-d1e5181a12973c21299cba9ded446af7_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
他手握的工具，名叫GauGAN，和印象派大师高更(Gauguin)不到一字之差，而绘画的逼真程度又远在其上。

除了凭空造山，还能秒加飞流直下的大瀑布：
![](https://pic1.zhimg.com/v2-1e079517e27c95a8e84de52a627f7ffc_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
栽棵树什么的，就更是小菜一碟：
![](https://pic2.zhimg.com/v2-e436bcf8ff7d829ff5607a9b8e8ca391_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='318'></svg>)
照片里该有什么，全凭鼠标安排。

除了造出不存在的物体，GauGAN还能穿越日夜，扭曲季节：
![](https://pic1.zhimg.com/v2-d53018efeef0afb5820dd3a81f49a1d8_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='352'></svg>)
而且它造出来的景物，细致到以前的算法都望尘莫及。
![](https://pic4.zhimg.com/v2-a040e31c594051077a1b3ea584b2a9a7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='230'></svg>)
那些对细节要求甚高的作品，比如运动场景、动物照片等等，它也都能根据一张涂鸦生成出来。
![](https://pic2.zhimg.com/v2-ed4d712cc2019fd1f02e8e00c7a403f9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='539'></svg>)
有Twitter网友发出来自英伟达GTC展厅的惊呼：

妈呀我被自己的艺术能力惊呆了！
![](https://pic1.zhimg.com/v2-d44837086cd0c095cfc7cdb9bd8f3264_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='535'></svg>)
神仙操作，大洋此岸的量子位自愧不如。

好了，正式介绍一下英伟达出品的**GauGAN**：你画一幅涂鸦，用颜色区分每一块对应着什么物体，它就能照着你的大作，合成以假乱真的真实世界效果图。在AI界，你的涂鸦有个学名，叫“语义布局”。

要实现这种能力，GauGAN靠的是空间自适应归一化合成法**SPADE**架构。这种算法的论文Semantic Image Synthesis with Spatially-Adaptive Normalization已经被CVPR 2019接收，而且还是口头报告（oral）。

这篇论文的一作，照例还是实习生。另外几位作者来自英伟达和MIT，CycleGAN的创造者华人小哥哥**朱俊彦**也在其中。

在基于语义合成图像这个领域里，这可是目前效果最强的方法。

## **神奇的空间适应**

在论文中，研究人员揭开了SPADE的神秘面纱。

此前，在语义图像合成领域有一套“流水线式”的加工流程：直接将语义布局（Semantic Layout）作为深度神经网络的输入，然后通过卷积、归一化和非线性层的处理，输出合成图像。
![](https://pic2.zhimg.com/v2-a2ab3bdd8d76583c6ca3fb4287a4238d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='922' height='306'></svg>)
**△** 推断语义布局合成图像 | 图片来自论文Inferring Semantic Layout for Hierarchical Text-to-Image Synthesis

但是，这种传统神经网络架构并不是最优解，其中的归一化层通常会让输入语义蒙版中的信息**流失**，导致合成效果变差。

为了解决这个问题，研究人员提出了一种新的方法，空间适应标准化（**SP**atially-**A**daptive (**DE**)normalization），简称SPADE。

这是一种条件归一化层，它通过学习到的空间适应变换，用语义布局调节激活函数，让语义信息在整个网络中有效传播，避免语义信息流失。

SPADE与批标准化（Batch Normalization）类似，激活函数channel-wise是标准化的，但在很多标准化技术中，实际标准化操作后就应用到了学习过的仿射层（Affine Layer）。
![](https://pic2.zhimg.com/v2-fbb9b188054fcc07b09a37e7f9b55fe5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='625'></svg>)
但在SPADE中，仿射层是从语义分割图中学习的。这类似于**条件标准化**，不过所学习的仿射参数现在需要空间自适应，也就是对每个语义标签使用不同的缩放和偏差。
![](https://pic3.zhimg.com/v2-f7018525b9ad44e01d10f4f7552ca55a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='490'></svg>)
就这样一个小小的改变，让语义信号可以作用于所有层输出，不会在生成图像中丢失语义信息。
![](https://pic1.zhimg.com/v2-2205df62ee446c74cba7e3e96ae3dbe0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='312'></svg>)
**△** SPACE生成器结构

此外，因为语义信息是通过SPADE层提供的，因此随机向量成为神经网络的输入，所以，你还能随心改变图像的画风。

**效果+++**

研究人员用COCO-Stuff、ADE20K和Cityscapes等数据集测试SPADE与前人的效果如何。

结果发现，这项新研究面前，此前CRN、pix2pixHD等明星语义图像合成方法效果已经**成为渣渣**：

此前的pix2pixHD和CRN算法只能分辨开天和海的颜色，而GauGAN却遥遥领跑，连渐变的海水颜色和四散的浪花都合成出来了，甚至运动场的场地线：
![](https://pic3.zhimg.com/v2-05ae54d1bc6bba59b5a3248bc482d7da_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='422'></svg>)
此外，研究人员用平均检测评价函数（mIoU）、像素准确度（accu）和FID（Frechet Inception Distance）三个维度评估SPADE与其他语义合成模型的评分，SPADE均优于其他模型。
![](https://pic3.zhimg.com/v2-1d097e218233c57902d7bae69e5631ca_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='412'></svg>)
## **作者介绍**

这篇论文由英伟达的研究人员Taesung Park、Ming-Yu Liu、Ting-Chun Wang和Jun-Yan Zhu（朱俊彦）共同完成。

一作Taesung Park是一位韩国小哥，本科和硕士就读于斯坦福，现在还在攻读UC伯克利计算机专业的博士学位。Park哥的论文分别被ICCV 2017、ICML 2019和CVPR 2019三场顶会接收过。
![](https://pic1.zhimg.com/v2-b6039d69334354137b1e98ba96c637a0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='550' height='542'></svg>)
目前，Park哥在英伟达实习，由研究员Ming-Yu Liu（劉洺堉）领导。

Ming-Yu Liu在参与过很多CV圈内耳熟能详的项目，包括vid2vid、pix2pixHD、CoupledGAN、FastPhotoStyle、MoCoGAN、FastPhotoStyle等，他的研究在NeurIPS、CVPR、ECCV大会上频频现身。
![](https://pic2.zhimg.com/v2-3f026041c64de9761b829a1921ecf4e5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='498' height='444'></svg>)
三作Ting-Chun Wang是Ming-Yu Liu的英伟达研究院同事，也是一作Park哥在UC伯克利的“嫡系”师兄。
![](https://pic1.zhimg.com/v2-e1a5d2960e1a489dd48231866bc9f1d0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='624' height='522'></svg>)
四作是一个熟悉的华人名字：**朱俊彦**。在计算机圈，朱俊彦是大神一样的存在。

2012年清华大学计算机科学系的工学学士毕业后，朱俊彦在CMU和UC伯克利经过5年学习获得UC伯克利电气工程与计算机科学系的博士学位，也师从Alexei Efros。
![](https://pic4.zhimg.com/v2-e8a4d29ffb0d05fbaa2dca4f2653aa53_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='470'></svg>)
提出了经典的“马变斑马”的CycleGAN后，还其博士毕业毕业论文Learning to Generate Images，获得了计算机图形学顶会ACM SIGGRAPH 2018“杰出博士论文奖”。

博士毕业后，朱俊彦成为MIT计算机与人工智能实验室（CSAIL）一名博士后研究员。

关于朱俊彦，这篇论文里还有个值得注意的小细节：他为自己标注了两家所属机构：MIT和**英伟达**。他此前发表的论文中，即便没少和英伟达合作，所属机构也都是MIT或者之前读博的UC伯克利。
![](https://pic2.zhimg.com/v2-7a04444a62294401fe577daef6c26e69_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='295'></svg>)
**△** 论文截图

**传送门**

目前，论文已经放出，研究人员表示代码、训练模型和所有图像马上就要来了。

在正在举办的英伟达GTC 19大会上，**GauGAN**已经亮相了。美国时间周三周五Ting-Chun Wang和Ming-Yu Liu还将进行相关演讲。

论文地址：

[https://arxiv.org/abs/1903.07291](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1903.07291)

GitHub地址（代码即将上线）：

[https://github.com/NVlabs/SPADE](https://link.zhihu.com/?target=https%3A//github.com/NVlabs/SPADE)

项目地址：

[https://nvlabs.github.io/SPADE/](https://link.zhihu.com/?target=https%3A//nvlabs.github.io/SPADE/)
![](https://pic2.zhimg.com/v2-1dea2dc6aab5b8f49da3d36bb02b35f9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='311'></svg>)
— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤


