# 心中无码：这是一个能自动脑补漫画空缺部分的AI项目 - 知乎
# 



选自Github，机器之心编译，参与：思源、路雪。

> 图像修复在应用上非常吸引人，通常设计师需要使用 Photoshop 根据图像周围修复空缺部分。这一过程非常耗时和细致，因此很早就有研究尝试使用机器学习模型自动化这一过程。这篇文章介绍了 DeepCreamPy 项目，它可以自动修复漫画图像中的空缺部分和马赛克。该项目主要基于几个月前 Nvidia 提出使用部分卷积修复图像不规则空缺的研究。

本文将简要介绍这项研究与 DeepCreamPy 实现项目，读者可下载项目代码或预构建的二进制文件，并尝试修复漫画图像或马赛克。这一个项目可以直接使用 CPU 进行推断，Windows 用户甚至都不需要安装环境都可以直接运行预构建的文件修复图像。

项目地址：[https://github.com/deeppomf/DeepCreamPy](https://link.zhihu.com/?target=https%3A//github.com/deeppomf/DeepCreamPy)

图像修复任务可用于多种应用。比如用于图像编辑：移除不需要的图像内容，用合理的图像内容填补移除后的空缺。之前的深度学习方法都聚焦在图像中心的矩形区域，往往依赖昂贵的后处理。而 DeepCreamPy 项目基于的方法提出一种图像修复的新模型，可在不规则的空缺模式上鲁棒地生成有意义的预测（图 1），预测结果与图像其余部分完美契合，无需进行额外的后处理或混合操作（blending operation）。
![](https://pic2.zhimg.com/v2-25d20c10e4903c7b937fa85343a90081_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='540'></svg>)图 1：原始图像和使用本研究提出的基于部分卷积的网络进行修复的对应修复结果
近期不使用深度学习技术的图像修复方法均使用图像剩余部分的统计信息来填补空缺。当前最优方法之一 PatchMatch [3] 迭代地搜索最适合的图像块来填补空缺。尽管该方法生成的结果通常较为流畅，但它受限于可用的图像统计信息，且不具备视觉语义学的概念。例如，在图 2（b）中，PatchMatch 能够使用来自周围阴影和墙体的图像快流畅地填补画作的空缺部分，但是语义感知方法利用的是来自画作本身的图像块。

深度神经网络以端到端的方式学习语义优先（semantic prior）和有意义的隐藏表征，这已经用于近期的图像修复工作。这些网络对图像采用卷积滤波器，用固定值替代缺失的内容。结果，这些方法依赖于初始空缺的值，初始空缺的值通常表现为空缺区域缺乏纹理和明显的颜色对比或空缺周围的人工边缘响应。图 2（e）和 2（f）展示了使用具备不同空缺值初始化的典型卷积层的 U-Net 架构的例子（两者的训练和测试使用同样的初始化方案）。
![](https://pic1.zhimg.com/v2-e3e3c4b906ffaf9b51f4db66c3fd7480_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='644'></svg>)图 2：不同图像修复方法的效果
很多近期方法的另一个曲线是只关注矩形空缺部分，通常位于图像中心。本文介绍的研究发现这些缺陷可能导致对矩形空缺部分的过拟合，最终限制这些模型的应用可用性。Pathak 和 Yang 等人假设一个 128×128 图像的中心处有一个 64 × 64 的正方形空缺部分，而 Iizuka 等人进一步移除了这一中心空缺假设，能够处理不规则形状的空缺部分，但是无法在大量具备不规则 mask 的图像（[8] 中有 51 个测试图像）上执行定量分析。为了解决实践中更常见的不规则 mask 问题，DeepCreamPy 项目使用的方法收集了具备不同大小的不规则 mask 的大量图像，并分析了 mask 大小的影响，以及 mask 与图像边界的关联。

为了恰当处理不规则 mask，Nvidia 的这项研究提出了部分卷积层（Partial Convolutional Layer），包括 mask 和重新标准化卷积操作以及后续的 mask 更新（mask-update）。mask 和重新标准化卷积操作的概念在 [9] 中指图像分割任务的分割感知卷积（segmentation-aware convolution），但它们不对输入 mask 进行修改。该研究使用部分卷积，即给出一个二元 mask，卷积结果仅依赖于每一层的非空缺区域。该研究的主要扩展是自动 mask 更新步，其移除任意 mask，部分卷积能够在 unmasked 值上运行。给出足够多层的连续更新，即使最大的 mask 空缺也能最终被消除，只在特征图中留下有效响应（valid response）。部分卷积层最终使得该模型不用理会占位符空缺值。

**DeepCreamPy 图像修复项目**

最近 deeppomf 开源了 Image Inpainting for Irregular Holes Using Partial Convolutions 的修复实现，它主要使用深度全卷积网络修复漫画图像。DeepCreamPy 能将遮挡的漫画图像重构为可信的画像，并且与通常的图像修复不一样，它使用的是不规则的 Mask。

用户需要事先使用绿色指定被遮住的区域，这一过程可以用简单的图画工具或 Photoshop 等。如下展示了加上绿色 Mask 的「损坏图像」和通过 DeepCreamPy 重构的图像。
![](https://pic1.zhimg.com/v2-eaae9e1caa9d82735ba6be8f38c86748_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='902' height='738'></svg>)
其实以前也开源过很多优秀的图像修复项目，例如 JiahuiYu 等研究者完成的 DeepFillv1 和 DeepFillv2，不过 DeepFillv2 一直没有放出代码来。机器之心也尝试过 DeepFillv1，不过它的效果在给定的测试图像上非常好，而在我们提供的图像上效果一般。
![](https://pic1.zhimg.com/v2-e0d8aa462c4992a886db9a262904be2c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='848' height='684'></svg>)图3：第一行为 deepfillV1 项目展示的效果，第二行为重构效果
按照 DeepCreamPy 的项目所述，这个 GitHub 项目的重点是可以修复任意尺寸的漫画图像和任意形状的 mask，同样也能修复漫画中的马赛克，不过这一部分还是不太稳定。此外，项目作者表示他正在做可视化界面，说不定过一段时间就能使用可视化界面试试漫画图像修复的威力。

目前项目作者已经发布了预构建的二进制文件，Windows 系统只需要下载该文件就可以直接运行。当然其它系统同样可以根据该项目运行预训练模型，或者直接重新训练该模型。
- 预构建模型下载地址：[https://github.com/deeppomf/DeepCreamPy/releases](https://link.zhihu.com/?target=https%3A//github.com/deeppomf/DeepCreamPy/releases)
- 预训练模型地址：[https://drive.google.com/open?id=1byrmn6wp0r27lSXcT9MC4j-RQ2R04P1Z](https://link.zhihu.com/?target=https%3A//drive.google.com/open%3Fid%3D1byrmn6wp0r27lSXcT9MC4j-RQ2R04P1Z)

如果读者使用预训练模型或者重新训练，该项目要求我们的计算环境包含以下几个工具：
- Python 3.6
- TensorFlow 1.10
- Keras 2.2.4
- Pillow
- h5py

重要的是，仅运行推断过程修复图像并不需要 GPU 的支持，而且也已经在 Ubuntu 16.04 和 Windows 64 位系统得到测试。这个项目使用的 TF 1.10 版是在 Python 3.6 中完成编译的，因此它并不与 Python 2 或 3.7 兼容。想要试一试的读者可以运行以下代码安装项目所需要的库：

> $ pip install -r requirements.txt

## **DeepCreamPy 使用方法**

**1. 修复条形空缺**

对于你想修复的每个图像，使用图像编辑软件（如 Photoshop 或 GIMP）将你想修复的区域的颜色涂成绿色 (0,255,0)。强烈推荐使用铅笔工具，不要用刷子。如果你没用铅笔，那么确保你所使用的工具关闭了抗锯齿（ANTI-ALIASING）功能。

作者自己使用的是 wand selection 工具（关闭了抗锯齿功能）来选择空缺区域。然后稍微扩展选中区域，并在选中区域上使用绿色 (0,255,0) 的油漆桶工具。

要在 Photoshop 中扩展选中区域，可以执行该操作：Selection > Modify > Expand 或 Contract。要在 GIMP 中扩展选中区域，可执行该操作：Select > Grow。将这些图像保存为 PNG 格式，并保存到 decensor_input 文件夹中。

A. 使用二进制编译文件（Windows）

双击 decensor 文件来修复图像。

B. 从头开始运行

运行以下行进行图像修复：

> $ python decensor.py

修复后图像将保存至 decensor_output 文件夹。每张图像的修复需要几分钟。




**2. 修复马赛克空缺部分**

和修复条形空缺一样，执行同样的着色步骤，将着色后图像放入 decensor_input 文件夹。此外，将原始、未着色图像放入 decensor_input_original 文件夹，并确保每个原始图像和着色后版本名称一样。

例如，如果原始图像名是 mermaid.jpg，你将其放入 decensor_input_original 文件夹；着色后的图像命名为 mermaid.png，放入 decensor_input 文件夹。

A. 使用 binary

双击 decensor_mosaic 文件修复图像。

B. 从头开始运行

运行以下行进行图像修复：

> $ python decensor.py --is_mosaic=True

修复后图像将保存至 decensor_output 文件夹。每张图像的修复需要几分钟。

**疑难解答**

如果你的 decensor 输出如下，则修复区域未能正确着色。
![](https://pic2.zhimg.com/v2-5f2ffe429b4be1d30597bf40383d0001_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='512' height='512'></svg>)
以下是一些好的和坏的着色图像示例。
![](https://pic4.zhimg.com/v2-5007f300c27fe562375bddd7c4f92a9f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='915' height='598'></svg>)
**论文：Image Inpainting for Irregular Holes Using Partial Convolutions**
![](https://pic2.zhimg.com/v2-564469e00b63d39cfa856ed16ea14a55_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1024' height='355'></svg>)
论文链接：[https://arxiv.org/pdf/1804.07723.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1804.07723.pdf)

摘要：基于深度学习的现有图像修复方法利用标准卷积网络修复受损图像，使用以有效像素以及 mask 区域中的替代值（通常为平均值）为条件的卷积核响应。这种做法通常会导致图片出现色差和模糊等问题。后处理通常被用于减少此类问题，但价格昂贵且存在失败的风险。我们提出使用部分卷积网络，其中卷积被掩蔽并重新归一化为仅以有效像素为条件。我们还包含了一种机制，可自动为下一层生成更新的 mask 作为前向传递的一部分。对于不规则 mask，我们的模型优于其它方法。我们通过与其它方法进行定性、定量对比对我们的方法进行了验证。
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*



