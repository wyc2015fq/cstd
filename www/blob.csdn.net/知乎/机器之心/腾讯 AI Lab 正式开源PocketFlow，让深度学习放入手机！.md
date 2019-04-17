# 腾讯 AI Lab 正式开源PocketFlow，让深度学习放入手机！ - 知乎
# 



> 9 月，机器之心曾报道腾讯 AI Lab 提出的自动化模型压缩框架 [PocketFlow](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650748704%26idx%3D3%26sn%3Da0273534c0490ae62537482c2e13fbdc%26chksm%3D871af35eb06d7a483be83d6d83332f9a3f5e1ee89458e968072b711ac068340cecc8d3a0fff8%26scene%3D21%23wechat_redirect)。近日，腾讯 AI Lab 在南京举办的腾讯全球合作伙伴论坛上宣布正式开源「PocketFlow」项目。

项目访问地址：[https://github.com/Tencent/PocketFlow](https://link.zhihu.com/?target=https%3A//github.com/Tencent/PocketFlow)

据介绍，该项目是一个自动化深度学习模型压缩与加速框架，整合多种模型压缩与加速算法并利用强化学习自动搜索合适压缩参数，解决传统深度学习模型由于模型体积太大，计算资源消耗高而难以在移动设备上部署的痛点，同时极大程度的降低了模型压缩的技术门槛，赋能移动端 AI 应用开发。

这是一款适用于各个专业能力层面开发者的模型压缩框架，基于 Tensorflow 开发，集成了当前主流与 AI Lab 自研的多个模型压缩与训练算法，并采用超参数优化组件实现了全程自动化托管式的模型压缩。开发者无需了解具体模型压缩算法细节，即可快速地将 AI 技术部署到移动端产品上，实现用户数据的本地高效处理。

目前该框架在腾讯内部已对多个移动端 AI 应用模型进行压缩和加速，并取得了令人满意的效果，对应用整体的上线效果起到了非常重要的作用。

**PocketFlow 框架**

PocketFlow 框架本次开源内容主要由两部分组件构成，分别是模型压缩/加速算法部分和超参数优化部分，具体结构如下图所示：



![](https://pic2.zhimg.com/v2-30633f53e27a9f7a4b2512fe47fd7de1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='391'></svg>)
模型压缩/加速算法部分包括多种深度学习模型压缩和加速算法：
- 通道剪枝（channel pruning）： 在 CNN 网络中，通过对特征图中的通道维度进行剪枝，可以同时降低模型大小和计算复杂度，并且压缩后的模型可以直接基于现有的深度学习框架进行部署。PocketFlow 还支持通道剪枝的分组 finetune/retrain 功能，通过实验发现此方法可以使原本压缩后的模型精度有明显的提升。
- 权重稀疏化（weight sparsification）：通过对网络权重引入稀疏性约束，可以大幅度降低网络权重中的非零元素个数；压缩后模型的网络权重可以以稀疏矩阵的形式进行存储和传输，从而实现模型压缩。
- 权重量化（weight quantization）：通过对网络权重引入量化约束，可以降低用于表示每个网络权重所需的比特数；同时提供了对于均匀和非均匀两大类量化算法的支持，可以充分利用 ARM 和 FPGA 等设备的硬件优化，以提升移动端的计算效率，并为未来的神经网络芯片设计提供软件支持。
- 网络蒸馏（network distillation）：对于上述各种模型压缩组件，通过将未压缩的原始模型的输出作为额外的监督信息，指导压缩后模型的训练，在压缩/加速倍数不变的前提下均可以获得 0.5%-2.0% 不等的精度提升。
- 多 GPU 训练（multi-GPU training）：深度学习模型训练过程对计算资源要求较高，单个 GPU 难以在短时间内完成模型训练，因此提供了对于多机多卡分布式训练的全面支持，以加快使用者的开发流程。无论是基于 ImageNet 数据的 Resnet-50 图像分类模型还是基于 WMT14 数据的 Transformer 机器翻译模型，均可以在一个小时内训练完毕。

超参数优化（hyper-parameter optimization）部分可以通过强化学习或者 AutoML，在整体压缩率一定的情况下，搜索出每一层最合适的压缩比例使得整体的精度最高。多数开发者对模型压缩算法往往了解较少，调节压缩算法参数需要长期的学习和实验才能有所经验，但超参数取值对最终结果往往有着巨大的影。PocketFlow 的超参数优化部分正是帮助开发者解决了这一大痛点，并且通过实验发现，其优化结果也要好于专业的模型压缩工程师手工调参的结果。其结构如下图：
![](https://pic4.zhimg.com/v2-e4bc4e03e40df1ae10ffeb2d1a1d6c47_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='577'></svg>)



**PocketFlow 性能**

通过引入超参数优化组件，不仅避免了高门槛、繁琐的人工调参工作，同时也使得 PocketFlow 在各个压缩算法上全面超过了人工调参的效果。以图像分类任务为例，在 CIFAR-10 和 ImageNet 等数据集上，PocketFlow 对 ResNet 和 MobileNet 等多种 CNN 网络结构进行有效的模型压缩与加速。

在 CIFAR-10 数据集上，PocketFlow 以 ResNet-56 作为基准模型进行通道剪枝，并加入了超参数优化和网络蒸馏等训练策略，实现了 2.5 倍加速下分类精度损失 0.4%，3.3 倍加速下精度损失 0.7%，且显著优于未压缩的 ResNet-44 模型； 在 ImageNet 数据集上，PocketFlow 可以对原本已经十分精简的 MobileNet 模型继续进行权重稀疏化，以更小的模型尺寸取得相似的分类精度；与 Inception-V1、ResNet-18 等模型相比，模型大小仅为后者的约 20~40%，但分类精度基本一致（甚至更高）。
![](https://pic3.zhimg.com/v2-10164847b959be5829597a20265812a2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='645'></svg>)


![](https://pic2.zhimg.com/v2-8db2fd4dd08d7e37a8fe2aa3a35ba775_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='648'></svg>)
相比于费时费力的人工调参，PocketFlow 框架中的 AutoML 自动超参数优化组件仅需 10 余次迭代就能达到与人工调参类似的性能，在经过 100 次迭代后搜索得到的超参数组合可以降低约 0.6% 的精度损失；通过使用超参数优化组件自动地确定网络中各层权重的量化比特数，PocketFlow 在对用于 ImageNet 图像分类任务的 MobileNet-v1 模型进行压缩时，取得了一致性的性能提升；用 PocketFlow 平均量化比特数为 8 时，准确率不降反升，从量化前的 70.89% 提升到量化后的 71.29%。
![](https://pic1.zhimg.com/v2-eef288f2993413dad7f4da7e4855cd18_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='641'></svg>)
**PocketFlow 内部应用**

据了解，在腾讯公司内部，PocketFlow 框架正在为多项移动端业务提供模型压缩与加速的技术支持。例如，在手机拍照 APP 中，人脸关键点定位模型是一个常用的预处理模块，通过对脸部的百余个特征点（如眼角、鼻尖等）进行识别与定位，可以为后续的人脸识别、智能美颜等多个应用提供必要的特征数据。腾讯 AI Lab 基于 PocketFlow 框架，对人脸关键点定位模型进行压缩，在保持定位精度不变的同时，大幅度地降低了计算开销，在本身已经十分精简的网络上取得了 1.3 ~ 2 倍不等的加速效果，压缩后的模型已经在实际产品中得到部署。

在人体体态识别项目中，PocketFlow 更是在满足上线精度的要求下，使得模型推理速度有 3 倍以上的加速，为项目的移动端落地起到了决定性的作用。


