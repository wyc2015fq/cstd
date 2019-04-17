# 让普通视频变成慢动作：「AI加帧」技术现已开源 - 知乎
# 



机器之心报道，机器之心编辑部。

> 出于节省空间等方面的考虑，手机、普通摄像机录制的视频单位时间包含的帧数往往较少，慢放时帧率进一步下降，无法还原太多细节。高速摄像机的出现弥补了这一不足，但成本较高。那么，只拥有普通设备的人要如何获取高帧率视频呢？

今年 6 月份，英伟达发布了一篇生成高质量慢动作视频的论文——《Super SloMo: High Quality Estimation of Multiple Intermediate Frames for Video Interpolation》，探讨了如何将普通设备录制的视频转换为高帧率慢动作视频。这项工作的原理是在临近的两帧之间补充额外的画面帧。让我们先来看一下效果：
![](https://pic2.zhimg.com/v2-b7c40c4cf9e9b0bfd5fc8097d4644e91_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='498' height='210'></svg>)
额... 英伟达给出的视频中展示了一个用网球拍快速打破水球的例子，正常速度下的视频如下：
![](https://pic3.zhimg.com/v2-b392be57d1bbd9f26b9fca212f7ab0c2_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='225'></svg>)
下图分别是用原始 SloMo 和 Super SloMo 软件制作的慢速视频。
![](https://pic3.zhimg.com/v2-f6e6f3d64cbe617c23c1b8b60842e066_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='225'></svg>)
从图中可以可出，用原始 SloMo 软件制作出的慢速视频虽然也让我们看到了一些肉眼无法捕捉到的细节，但相比之下，用 Super SloMo 制作的慢速视频还原的细节更多，画面也更加流畅。这是因为二者有着本质的区别，前者并没有生成新的视频帧，而后者则利用神经网络生成了新的视频帧，使得画面包含的帧数更多，从而增加了视频细节和流畅度。

在看看下方的赛车甩尾，原视频为 30FPS。看起来没毛病吧，那慢速播放之后呢？
![](https://pic1.zhimg.com/v2-e7188c1799435a18075bfd04a068347c_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='225'></svg>)
看下图上方，原来流畅的甩尾变得像是一张张照片摆拍凑出来的定格动画（P 水花比甩尾简单多了吧），但经过算法补帧变成 240FPS 之后，下方的片段立刻有了 Fast & Furious 即视感。
![](https://pic2.zhimg.com/v2-a999f8eebd24b721aa046d6b667fb205_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='225'></svg>)
遗憾的是，作者发布论文的时候并没有放出数据集和代码，让想要实现这一炫酷技术的 geek 们大失所望。但是（划重点），群众的力量是伟大的。近日，GitHub 一位名为 avinashpaliwal 的用户开源了自己对 Super SloMo 的 PyTorch 实现：

Github 地址：[https://github.com/avinashpaliwal/Super-SloMo](https://link.zhihu.com/?target=https%3A//github.com/avinashpaliwal/Super-SloMo)
[英伟达慢动作生成新技术_腾讯视频​v.qq.com![图标](https://pic3.zhimg.com/v2-a772a2982020f0c43d39432a93d041da_180x120.jpg)](https://link.zhihu.com/?target=https%3A//v.qq.com/x/page/p06946gx1kr.html)
## **Super SloMo 的 PyTorch 实现**

**结果**

使用作者提供的评估脚本在 UCF101 数据集上的结果。用的脚本是 et_results_bug_fixed.sh。它在计算 PSNR、SSIM 和 IE 时使用了运动掩码（motions mask）。

**先决条件**

该代码库是用 pytorch 0.4.1 和 CUDA 9.2 进行开发测试的。

**训练**

准备训练数据

要使用提供的代码训练模型，首先需要以某种方式格式化数据。

create_dataset.py 脚本使用 ffmpeg 从视频中提取帧。

至于 adobe240fps，下载下面这个数据集，解压并运行以下命令：

`python data\create_dataset.py --ffmpeg_dir path\to\ffmpeg --videos_folder path\to\adobe240fps\videoFolder --dataset_folder path\to\dataset --dataset adobe240fps`

数据集：[http://www.cs.ubc.ca/labs/imager/tr/2017/DeepVideoDeblurring/DeepVideoDeblurring_Dataset_Original_High_FPS_Videos.zip](https://link.zhihu.com/?target=http%3A//www.cs.ubc.ca/labs/imager/tr/2017/DeepVideoDeblurring/DeepVideoDeblurring_Dataset_Original_High_FPS_Videos.zip)

**评估**

预训练模型

你可以从以下地址下载在 adobe240fps 数据集上训练的预训练模型：

[https://drive.google.com/open?id=1IvobLDbRiBgZr3ryCRrWL8xDbMZ-KnpF](https://link.zhihu.com/?target=https%3A//drive.google.com/open%3Fid%3D1IvobLDbRiBgZr3ryCRrWL8xDbMZ-KnpF)

这个模型怎么玩才好玩呢？拿出过去渣手机拍摄的珍藏片段，慢动作回放一下是不是更有味道？
![](https://pic4.zhimg.com/v2-5b989b86ccbae31869906bacabd21dfb_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='225'></svg>)
那些为自制电影设备费用发愁的是不是来劲了？
![](https://pic4.zhimg.com/v2-c7dc34ea58cbecc3f5f7295679a09ab3_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='150'></svg>)
还有，那些每次一到高速战斗情节就经费严重不足的动画制作组是不是觉得相见恨晚？
![](https://pic3.zhimg.com/v2-16d69da10beb767744bac7ae7c47d37a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='360'></svg>)![](https://pic3.zhimg.com/v2-e0d72cabb4f7b8d5ca1aa35648676f4a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='450'></svg>)
有了它，一些动画是不是能多更新半年了？
![](https://pic2.zhimg.com/v2-b2b05a83bbf6f89d4bce57afc4b9d3f5_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='375' height='204'></svg>)
## **论文**
![](https://pic1.zhimg.com/v2-14bafa8633e395e9a32a1a7ee181a950_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='174'></svg>)- 项目链接：[https://people.cs.umass.edu/~hzjiang/projects/superslomo/](https://link.zhihu.com/?target=https%3A//people.cs.umass.edu/~hzjiang/projects/superslomo/)
- 论文链接：[https://arxiv.org/pdf/1712.00080.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1712.00080.pdf)

**摘要**：给定两个连续的帧，视频插值旨在生成中间帧，以形成空间和时间相干的视频序列。大多已有的方法都集中于单帧插值上，本文提出了一种用于可变长度多帧视频插值的端到端卷积神经网络，其中运动解释（motion interpretation）和遮挡推理（occlusion reasoning）是联合建模的。该研究从使用 U-Net 架构计算输入图像之间的双向光流开始。随后，这些流在每个时间步被线性组合，以近似中间双向光流。然而，这些近似流仅在局部平滑区域有用，且在运动边界周围产生伪影。

为了解决这个不足，作者利用另一个 U-Net 来细化近似流并预测软可视图（soft visibility map）。最终，两个输入图像被变形并线性融合以形成每个中间帧。通过在融合之前把可视图应用于变形的图像上，作者排除了遮挡像素对插值中间帧的贡献，以避免伪影。由于所学到的网络参数都不依赖于时间，所以本文的方法能够根据需要产生很多中间帧。作者使用 1132 个每秒 240 帧的视频片段（包含 30 万个单独的视频帧）来训练其网络。在多个数据集上的实验（预测不同数量的插值帧）结果表明本文的方法比现有的方法性能更好。

**方法**

研究者使用了光流插值的方法来合成中间帧，基于两个关键要素：时间一致性和遮挡推断。

时间一致性是指：中间帧 I_t 可以通过初始帧 I_0 结合光流（optic flow，F）的转换 g(·) 而成，也可以通过结束帧 I_1 结合光流 F 转换而成，一般形式是两者的线性组合。
![](https://pic1.zhimg.com/v2-10ea15215097cba5fda2d44e181486f4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='636' height='65'></svg>)
一般而言，随着时间流动，I_t 的图像特征会从更接近 I_0 变得更接近 I_1，所以α_0 应该是 t 的线性函数（不考虑遮挡时）。当 t=0 时，α_0=1；当 t=1 时，α_0=0。

如图 2 所示：
![](https://pic1.zhimg.com/v2-d2220ffc18d23fa08b813103c55d5fc0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='740' height='738'></svg>)
图 2：光流插值结果的示例，其中 t=0.5。完整的场景向左移动（摄像头平移），摩托车也向左移动。最后一行展示了光流插值 CNN 的微调主要在运动边界处进行（像素越白，微调程度越高）。

遮挡推断是指：在 I_0 中出现的像素（或物体），在 I_1 中可能不出现，反之亦然。那么就要考虑 I_t 在遮挡情况下对两个输入帧的权重，用可视图（visibility maps，V）表示。

所以，最终的中间帧合成等式为（Z 是归一化因子）：
![](https://pic1.zhimg.com/v2-38521a96dbd9778d09c040dc9b0f5210_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='759' height='91'></svg>)
如图 3 所示：
![](https://pic4.zhimg.com/v2-e53beac1d9dcbc59ec74e4e7f721499b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='643' height='662'></svg>)*![](https://pic3.zhimg.com/v2-8b55107ebfd8c1ebb5c8d55cb951df96_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='655' height='626'></svg>)*
图 3：预测可视图的样本，其中 t=0.5。运动员的手从 T=0 到 T=1 往上举。因此手臂右上方区域在 T=0 时是可见的，在 T=1 时是不可见的（被遮挡）。第四行的可视图清晰地展示了这个现象。V_t←0 中手臂周围的白色区域表示 I_0 中的这些像素对合成ˆI_t 的贡献最大，黑色区域表示这些像素对合成ˆI_t 的贡献最小。V_t←1 也是同样的道理。

由于中间帧本身是需要预测的，不是预先存在的，因此需要用 I_0 和 I_1 之间的光流对 I_t 和 I_0、I_1 之间的光流进行近似：
![](https://pic1.zhimg.com/v2-8486c23e6debc03e0a184e479074db8c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='510' height='94'></svg>)
此外关于遮挡推断，同样很自然地假设：当 t 接近 0 的时候，I_t 更接近 I_0；当 t 接近 1 的时候，I_t 更接近 I_1。
![](https://pic1.zhimg.com/v2-846ce507b2dc006828a3ac3addcf4884_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='267' height='53'></svg>)
所以，最终的架构设计如下：其分为两个阶段，第一个阶段将 I_0、I_1 输入到光流计算 CNN 中，得到两者之间正向和反向的光流；第二个阶段再以 I_0 和 I_1 之间的光流、I_0 和 I_1、I_0 和 I_1 到 I_t 的变换函数、I_0 和 I_1 到 I_t 的近似光流为输入，得到可视图以及 I_0 和 I_1 到 I_t 的近似光流的增量。结合这些量，就可以直接算出最终的 I_t，如下图所示。
![](https://pic4.zhimg.com/v2-b4cc17fa3948b7f3d188a70a44c6e77f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='769' height='356'></svg>)
损失函数分为四个项：重建损失（Reconstruction loss），合成帧和数据集中间帧的 L1 损失；感知损失（perceptual loss），减少图像模糊；转换损失（Warping loss.），图像帧和经其它帧光流变换输出之间的 L1 损失；平滑度损失（Smoothness loss），限制光流函数的梯度。
![](https://pic4.zhimg.com/v2-c84890592029d76713a6c03ac36db733_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='645'></svg>)图 9：主要 CNN 架构为 U-Net![](https://pic3.zhimg.com/v2-c15a08131bc60d2bafdeca94332dc786_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='622' height='348'></svg>)图 8：在高帧数 Sintel 数据集上生成 31 张中间帧时每个时间步的 PSNR 值对比
读者还可以查看论文在 CVPR 2018 spotlight video 的论文讲解：
[英伟达slomo视频4_腾讯视频​v.qq.com![图标](https://pic3.zhimg.com/v2-a772a2982020f0c43d39432a93d041da_180x120.jpg)](https://link.zhihu.com/?target=https%3A//v.qq.com/x/page/m0820gwsans.html)



