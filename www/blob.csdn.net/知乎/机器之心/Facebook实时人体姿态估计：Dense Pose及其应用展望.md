# Facebook实时人体姿态估计：Dense Pose及其应用展望 - 知乎
# 



机器之心原创，作者：Olli Huang ，编辑：Haojin Yang。

Facebook 和 Inria France 的研究人员分别在 CVPR 2018 和 ECCV 2018 相继发表了两篇有关「人体姿态估计」(human pose estimation) 的文章 [1] [2]，用于介绍他们提出的 Dense Pose 系统以及一个应用场景「密集姿态转移」（dense pose transfer）。

本文将简要介绍（1）利用 Dense Pose 实现单张 2D 人体图像到 3D 表面模型的原理 ；（2）如何将 Dense Pose 系统应用在「姿态转移」（pose transfer）这一问题上；（3）粗略展望 Dense Pose 的一些潜在应用。

Dense Pose 的更多信息以及视频 demos：
[DensePose人体姿势识别_腾讯视频​v.qq.com![图标](https://pic3.zhimg.com/v2-a772a2982020f0c43d39432a93d041da_180x120.jpg)](https://link.zhihu.com/?target=https%3A//v.qq.com/x/page/n0545gq88mq.html)
**1. 什么是密集姿态估计（dense pose estimation）?**

密集姿态估计 (dense pose estimation) 将单张 2D 图片中所有描述人体的像素（human pixels），映射到一个 3D 的人体表面模型。如图 1 所示，Facebook 发布了一个名为 DensePose COCO 的大型数据集，包含了预先手工标注的 5 万张各种人类动作的图片。
![](https://pic4.zhimg.com/v2-6084e3e445c77756c0dc35758d812ed7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='254'></svg>)
图 1：密集姿态估计的目标是将 2D 图片中描述人体的像素，映射到一个 3D 表面模型。左：输入的原始图像，以及利用 [1] 中提出的 Dense Pose-RCNN，获得人体各区域的 UV 坐标。UV 坐标又称纹理坐标 (texture coordinates), 用于控制 3D 表面的纹理映射； 中：DensePose COCO 数据集中的原始标注；右：人体表面的分割以及 UV 参数化示意图。

**2. 如何进行密集姿态估计？**

密集姿态估计的核心任务是，训练一个深度网络，用于预测 2D 图片像素 (image pixels）与 3D 表面模型点 (surface points) 之间的密集联系 (dense correspondences)。这个任务最近已经通过基于全连接卷积网络 [4] 的 Dense Regression (DenseReg) 系统 [3] 得到了解决。而 Dense Pose-RCNN 系统 [1]，正是结合了 DenseReg 系统以及 Mask-RCNN 架构 [5]。

图 2 展示了 Dense Pose-RCNN 的级连 (cascade) 架构：这是一个全卷积网络 (fully-convolutional network)，并连接着 ROIAlign 池化层 (ROIAlign pooling)，用于处理两个核心任务，分别是：（1）分类。判断图片的某一像素来自于「背景」，还是「人体部位」；（2）回归。预测该像素在「人体部位」的具体坐标。
![](https://pic4.zhimg.com/v2-998d189f0016f35b3c744404d49814b3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='402'></svg>)图 2：Dense Pose-RCNN 的架构
图 2 中的 ResNet50 FPN (feature pyramid networks) 将输出 feature map，然后通过 ROIAlign 模块对每一个 ROI 生成固定尺寸的 feature map。图 3 展示了 ROIAlign 模块的「跨级连」(cross-cascading) 结构，这种结构利用两个辅助任务 (keypoint estimation & mask) 提供的信息，帮助提高 Dense Pose 系统的姿态估计效果。作为 Dense Pose-RCNN 基础之一的 Mask-RCNN [5] 结构，正是借助两个相关任务（即 keypoint estimation 和 instance segmentation）提供的信息，用于提高分割效果。
![](https://pic3.zhimg.com/v2-717bfd5516760dbb8922a8d7edbacfde_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='643'></svg>)图 3：Dense Pose-RCNN[1] 中的 ROIAlign 模块采用了「跨级连」(cross-cascading) 架构
**3. 主干 (backbone) 网络对于 Dense Pose 系统的影响？**

研究人员在 [1] 中对于两种主干网络，ResNet-50 和 ResNet-101，对于 Dense Pose 任务的表现进行了比较。如表 1 所示，在 AP 和 AR 这两个评估指标上，ResNet-101 的表现稍优于 ResNet-50，但由于大型网络较为「笨重」，并不适用于移动应用。因此，[1] 中的其余实验均使用了 ResNet-50 作为主干网络。

表 1: 当 Dense Pose-RCNN 使用两个不同的主干网络时，在 COCO Minval 数据集上取得的实验结果。[1]
![](https://pic1.zhimg.com/v2-ca6c139ac4ab6d372c685206df6ad380_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='107'></svg>)
**4. 从 Dense Pose 到 Dense Pose Transfer **

除了介绍 Dense Pose 系统的架构和工作流程，研究人员还在 [1] 中展示了一个 Dense Pose 的应用，「纹理转移」(texture transfer)。如图 4 所示，纹理转移这一任务的目标是，将图像中所有人的身体表面纹理，转换为预先提供的目标纹理。
![](https://pic3.zhimg.com/v2-e7d56c382edf76523b2bad0239b92cee_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='654'></svg>)
图 4：Dense Pose 纹理转换 (texture transfer) 的实验结果。该任务的目标是，将输入的视频图像中所有人的身体表面纹理，转换成目标纹理。图中第 1 行为目标纹理 1 和纹理 2。第 2、3 行从左至右依次为，输入图像，转换为纹理 1 的图像，以及转换为纹理 2 的图像。

在 ECCV 2018 上，论文 [1] 的三名作者发表了 Dense Pose 的一个后续应用，即「密集姿态转移」(dense pose transfer，以下简称为 DPT) [2]。与纹理转换不同的是，DPT 这一任务的目标是，根据输入的 2D 人体图像和目标姿态 (target dense pose)，将输入图像中的人体姿态转换成目标姿态，并且不改变人体表面纹理。

如图 5 所示，DPT 系统以 Dense Pose[1] 为基础，并且由两个互补的模块组成，分别是（1）推测模块 (predictive module)，用于根据输入图像，预测出具有目标姿态的人体图像；（2）变形模块 (warping module)，负责从输入图像中提取纹理，并「补全」(inpainting) 具有目标姿态的人体表面纹理。此外，系统中还有一个合成模块 (blending module)，通过端对端、可训练的单一框架，将推测和变形模块的输出进行合成，并产生最终的图像。
![](https://pic4.zhimg.com/v2-2df8961fb6d35fef730876b954f71e2f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='448'></svg>)图 5：密集姿态转移（DPT) 系统的流程图。该系统包括推测模块、变形模块，以及合成模块
图 6 展示了在 DeepFashion 数据集 [6] 上取得的 12 组姿态估计结果。每组姿态结果从左至右依次为：输入图像、正确的目标图像、Deformable GANs (DSC) [7] 获得的转移结果，以及 DPT 系统 [2] 获得的转移结果。由于 DSC 是目前解决「多视角图像合成」(multi-view synthesis) 这一问题中效果最佳的方法，所以 [2] 的作者将这一方法与 DPT 系统进行比较。

从图 6 可以粗略观察到 DPT 系统在纹理转移上还不是特别完善。例如，一些女士上衣的花纹没有被保留，并成功转移到输出图像中；此外，人物的面部特征也在转移中出现一些偏差：身着黄色上衣的男士图像（见图 6 右侧第 3 行），经过姿态转后，人物面部更为「女性化」。论文 [2] 的作者指出，要取得更好的姿态转换结果，可能还需要预先获得一些额外的信息，比如面部特征、性别以及肤色。
![](https://pic3.zhimg.com/v2-36a43d0969f436f693568e4a6edeaed6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='928' height='950'></svg>)图 6：密集姿态转换（dense pose transfer）的实验结果。左右两组结果分别包含了输入图像、正确的目标图像、Deformable GANs (DSC) [7] 得到的转移结果，以及 DPT 系统 [2] 得到的转移结果
**5. Dense Pose 的应用展望**

Dense Pose 为人体姿态估计提供了一种新的解决方法，研究人员也在 demos 中展示了 Dense Pose 能够实时完成纹理转移等任务。尽管 Dense Pose 还有许多需要完善的地方，它的应用前景还是相当乐观的。笔者认为 Dense Pose 未来可以在以下两个方面进行应用：

一个应用方向是，利用单一图片进行服装的虚拟试穿。顾客可以上传一张自己的全身正面照，并从系统中选择一套需要试穿的服装，以及一个目标姿态。Dense Pose 系统则可以根据顾客提供的这些信息，产生出一张合成的图片，显示顾客「试穿」服装后的姿态。

另一个应用方向则是，远程视频诊断背部痛疾。就诊者按照医生的要求，穿戴一套专用的传感器，并依次完成一系列动作（如上身向前倾若干度、蹲下、站直向左扭头等等）。就诊者的这一系列动作将由摄像头拍摄，并生成视频实时传输给医生。一个设想是，医生可以借助 Dense Pose 系统，根据就诊者的动作视频图像，生成就诊者的 3D 人体模型，并与背部无疾患的人体模型进行对比，从而初步判断患者是否患有背部疾病。
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*
参考文献：

[1] Alp Güler, Rıza, Natalia Neverova, and Iasonas Kokkinos. "Densepose: Dense human pose estimation in the wild." Proceedings of the IEEE Conference on Computer Vision and Pattern Recognition. 2018.

[2] Neverova, Natalia, Riza Alp Guler, and Iasonas Kokkinos. "Dense pose transfer." Proceedings of the European Conference on Computer Vision (ECCV). 2018.

[3] Alp Guler, Riza, et al. "Densereg: Fully convolutional dense shape regression in-the-wild." Proceedings of the IEEE Conference on Computer Vision and Pattern Recognition. 2017.

[4] Chen, Liang-Chieh, et al. "Deeplab: Semantic image segmentation with deep convolutional nets, atrous convolution, and fully connected crfs." IEEE transactions on pattern analysis and machine intelligence 40.4 (2017): 834-848.

[5] He, Kaiming, et al. "Mask r-cnn." Computer Vision (ICCV), 2017 IEEE International Conference on. IEEE, 2017.

[6] Liu, Ziwei, et al. "Deepfashion: Powering robust clothes recognition and retrieval with rich annotations." Proceedings of the IEEE conference on computer vision and pattern recognition. 2016.

[7] Siarohin, Aliaksandr, et al. "Deformable gans for pose-based human image generation." CVPR 2018-Computer Vision and Pattern Recognition. 2018.

**技术分析师的个人简介**

> Olli Huang 是澳门科技大学的一名博士研究生，她的研究方向是大规模图像检索 (large-scale image retrieval) 和图像的地理位置估计 (visual-based image geolocalization)。Olli 于 2017 年加入了机器之心的全球团队，并以自由撰稿人的身份，发表了多篇计算机视觉应用的英文技术评论。
Olli 的机器之心主页：[https://www.jiqizhixin.com/users/84204384-374e-4de0-bfc5-79eee677a8ec](https://link.zhihu.com/?target=https%3A//www.jiqizhixin.com/users/84204384-374e-4de0-bfc5-79eee677a8ec)。
Olli 的 LinkedIn 主页：[https://www.linkedin.com/in/ollihuang](https://link.zhihu.com/?target=https%3A//www.linkedin.com/in/ollihuang)。


