# 惊了，DeepFakes不仅骗过人，还能骗过人脸识别系统？！ - 知乎
# 



选自 arXiv，作者：Pavel Korshunov、Sebastien Marcel，机器之心编辑部。

> 今年年初，DeepFakes 技术火爆全网，它可以轻松替换视频中的人脸。网络上各种恶搞视频（其中大量是色情视频）让人分不清真真假假，那么人脸识别系统能够检测出哪些是 DeepFakes 生成的视频吗？这项研究告诉我们不太行……

自动视频音频编辑工具、生成对抗网络（GAN）和社交媒体的近期发展使得高质量的「修改版」视频内容出现并快速传播。此类内容已经导致「假新闻」的出现，影响了多个国家的政治形势。近期使用神经网络（即 Deepfakes）对视频进行换脸的大量（色情）视频的出现引起了公众的极大担忧。相关的开源软件和 app 使得社交媒体和新闻中出现大量此类合成 Deepfake 视频，为此类内容的检测和过滤带来了很大的技术挑战。因此，开发能够自动检测此类换脸视频的高效工具非常重要。

直到最近，多数研究仍专注于提升换脸技术 [2, 3, 4, 5]。然而，为了满足大众对检测换脸技术的需要，一些研究者也开始研究数据集和检测方法，包括用 Face2Face [7] 换脸方法生成的图像和视频数据 [6] 或用 Snapchat 应用收集的视频 [8]。

在这篇论文中，作者展示了首个公共可用的换脸视频数据集，其中的视频使用基于 GAN 的开源方法实现换脸，这种方法由基于自编码器的原始 DeepFakes 算法发展而来。作者从开放的 VidTIMIT 数据集中手动选取了 16 个相似的人脸对，即共有 32 个对象。对于每一个对象，作者训练了两种不同的模型。在这篇论文中，两种不同的模型即有 64 × 64 输入/输出尺寸的低质模型（LQ）和有 128×128 尺寸的高质模型（HQ），下图 1 展示了两种模型的输入输出样本。

因为 VidTIMIT 数据集中每个人有 10 个视频，所以作者每个版本生成了 320 个视频，最后得到一共 620 个换脸视频。至于视频中的音频，作者保留了原音轨，即在音频通道上没有做任何修改。
![](https://pic3.zhimg.com/v2-e868fe42b9fe9b9299b91b0b9f5ca15e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='328'></svg>)图 1：来自 VidTIMIT 数据集原始视频的截图，LQ：低质量；HQ：高质量
理解 DeepFakes 视频对人脸识别系统造成的威胁非常重要。因为如果这些系统不会被 Deepfakes 视频欺骗，那么创建能够检测 DeepFakes 的系统则无必要。为了检测人脸识别系统能否检测出 DeepFakes 视频，研究者评估了两个顶尖的系统（基于 VGG 和 Facenet 的神经网络）在原始视频和换脸后视频上的性能。

对于 DeepFakes 视频的检测，研究者首先使用视听方法检测音画不同步，这可以帮助理解 Deepfakes 视频模仿唇部运动的逼真程度，以及嘴唇是否与声音同步。研究者还使用了 presentation attack 检测领域的多个基线方法，将 DeepFakes 视频看作 digital presentation attack，这些基线方法包括主成分分析 (PCA) 和线性判别分析 (LDA) 方法、基于图像质量度量（image quality metrics，IQM）和支持向量机 (SVM) 的方法。

为了使广大研究人员可以验证、复现和扩展该研究，本论文作者提供了 Deepfakes 视频数据集、人脸识别和 DeepFakes 检测系统以及对应得分作为开源 Python 包。

本论文的贡献如下：
- 使用 GAN 方法基于 VidTIMIT 数据集生成的低质、高质换脸视频数据集，且公开可用；
- 基于 VGG 和 Facenet 人脸识别系统检测 Deepfakes 视频时的脆弱度分析；
- 评估 DeepFakes 的多种检测方法，包括音画同步方法和 IQM+SVM 方法。

**论文：DeepFakes: a New Threat to Face Recognition? Assessment and Detection**
![](https://pic1.zhimg.com/v2-6449e39d40078ccd653f00827afb136c_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='218'></svg>)
论文链接：[https://arxiv.org/pdf/1812.08685.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1812.08685.pdf)

**摘要**：通过预训练 GAN 来自动替换视频中的人脸变得越来越容易。近期的丑闻（如名人的脸被换到色情视频中）表明我们需要一种自动化方式来检测这些 Deepfakes 视频。为了帮助开发此类方法，本论文公开发布了首个基于 VidTIMIT 数据库视频生成的 Deepfakes 视频集合。本研究使用基于 GAN 的开源软件创建 Deepfakes，并强调训练和混合参数可极大影响生成视频的质量。为此，研究者使用不同的参数生成了低视觉质量和高视觉质量的视频（每类包含 320 个视频）。研究表明当前最优的基于 VGG 和 Facenet 神经网络的人脸识别系统无法抵御 Deepfakes 视频的「攻击」，这两个模型在高质量视频上的误识率（FAR）分别为 85.62% 和 95.00%，这表明开发检测 Deepfakes 视频方法的必要性。考虑了多个基线方法之后，该研究发现基于音画不同步的视听方法无法分辨 Deepfakes 视频。性能最好的方法基于视觉质量度量，通常用于 presentation attack 检测领域，该方法在高质量 Deepfakes 视频上的等错误率（EER）为 8.97%。实验表明 GAN 生成的 Deepfakes 视频对人脸识别系统和现有的检测方法都构成挑战，未来进一步的换脸技术将加重这一挑战。

**DeepFakes 数据集**

研究者抽取 VidTIMIT 数据集中的视频作为原始数据。该数据集共有 43 个对象，每个对象有 10 个视频。视频在受控环境下拍摄，画面中的人直面镜头，诵读预先指定的短语。研究者从这 43 个对象中手动选择了 16 对，同一对人脸有着相似的显著视觉特征，如胡子或发型。使用基于可用代码的 GAN 换脸算法，研究者为每一对人脸生成了换脸后的视频，将对象 1 和对象 2 的脸进行互换（视频截图见图 1）。

研究者针对每对对象训练了两个不同的 GAN 模型，生成了两个版本的视频：

1）低质量（LQ）模型的输入、输出图像（仅人脸区域）大小为 64 × 64。

2）高质量（HQ）模型的输入、输出图像大小为 128 × 128。

不同的模型需要不同的混合技术。LQ 模型使用来自目标视频的帧作为输入来生成每一帧的人脸。然后，使用 [4] 提出的基于 CNN 的人脸分割算法，检测出面部掩码（facial mask）。再使用该掩码，将生成的人脸与目标视频中的人脸混合。而对于 HQ 模型来说，混合是基于目标视频中生成的人脸和原始人脸之间的人脸关键点对齐来完成的。人脸关键点检测使用广泛可用的预训练 MTCNN 模型完成 [22]。最后，将生成的人脸混合到目标视频中时，应用直方图归一化（histogram normalization）调整光照条件。

**DeepFakes 视频分析**
![](https://pic2.zhimg.com/v2-8b2de4641eaeb223e745034463c59a79_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='346'></svg>)
图 2：上面的直方图展示了 VGG 和 Facenet 人脸识别系统轻易被高质量换脸视频欺骗，以及 IQM+SVM 检测方法在低质量和高质量 Deepfakes 视频上的性能。
![](https://pic1.zhimg.com/v2-47968aa152b9bcde2c8551b09a4eae00_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='648' height='272'></svg>)
表 1：VidTIMIT 数据集中低质量和高质量 Deepfake 视频的基线检测系统。当 FAR 等于 10% 时，在测试集上计算的 EER 和 FRR 数值。 
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*



