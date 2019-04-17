# 【转载】github 项目推荐：用 edge-connect 进行图像修复 - 战斗蜗牛的专栏 - CSDN博客





2019年03月02日 17:35:00[vbskj](https://me.csdn.net/vbskj)阅读数：157








雷锋网(公众号：雷锋网) AI 科技评论按，用对抗性边缘学习修复生成图像是一种新的图像修复方法，它可以更好地复制填充区域，它的细节部分展现了开发者对艺术工作者工作方式的理解：线条优先，颜色次之。对应的论文在 arxiv 上可以查看：[https://arxiv.org/abs/1901.00212](https://arxiv.org/abs/1901.00212)。

文中提出了一种 2 阶对抗式边缘连接模型，该模型由一个边缘生成器和一个图像完成网络组成。边缘生成器先描绘出图像缺失区域（规则和不规则）的边缘，图像完成网络先验使用描绘出的边缘填充缺失区域。论文对该系统进行了详细的描述。

![github 项目推荐：用 edge-connect 进行图像修复](https://static.leiphone.com/uploads/new/images/20190220/5c6d255cdc55b.png?imageView2/2/w/740)

（a）输入有缺失区域的图像，缺失区域用白色表示。（b）计算得到的边缘，使用 Canny 边缘检测器计算（针对可用区域）黑色绘制的边缘；而蓝色显示的边缘由边缘生成器网络描绘。（c）拟用方法的图像修复结果。

**准备：**

Python 3

PyTorch 1.0

NVIDIA GPU + CUDA cuDNN

**安装：**

复制下面这个 repo：

> 
git clone https://github.com/knazeri/edge-connect.gitcd edge-connect

cd edge-connect


从 [http://pytorch.org](http://pytorch.org/) 安装 PyTorch 及其相关依赖。

**安装 python 文件：**

> 
pip install -r requirements.txt


**数据集**

**1. 图像**

这里使用 [Places2](http://places2.csail.mit.edu/),[CelebA](http://mmlab.ie.cuhk.edu.hk/projects/CelebA.html) 以及 [Paris Street-View](https://github.com/pathak22/context-encoder)数据集。从官网下载数据集，在整个数据集上训练模型。

下载完成后，运行[scripts/flist.py](http://edge-connect/)这个文件来生成训练、测试和验证集文件列表。例如，要在 Places2 数据集上生成训练集文件列表，请运行：

> 
mkdir datasets

python ./scripts/flist.py --path path_to_places2_train_set --output ./datasets/places_train.flist


**2 .不规则掩膜**

我们的模型是在 [Liu 等人](https://arxiv.org/abs/1804.07723)提供的不规则掩模数据集上进行训练的，你可以从[他们的网站上](http://masc.cs.gmu.edu/wiki/partialconv)下载公开的不规则掩膜数据集。雷锋网

请使用 scripts/flist.py 生成上述训练、测试和验证集掩膜文件列表。

**开始**

使用以下链接下载预先训练的模型，并将其复制到./checkpoints 目录下。

> 
[Places2](https://drive.google.com/drive/folders/1KyXz4W4SAvfsGh3NJ7XgdOv5t46o-8aa) | [CelebA](https://drive.google.com/drive/folders/1nkLOhzWL-w2euo0U6amhz7HVzqNC5rqb) | [Paris-StreetView](https://drive.google.com/drive/folders/1cGwDaZqDcqYU7kDuEbMXa9TP3uDJRBR1)


或者，你可以运行以下脚本自动下载预训练的模型：

> 
bash ./scripts/download_model.sh


**1 .训练**

要训练模型，请创建一个类似于示例配置文件的 config.yaml 文件，并将其复制到检查点目录下。有关模型配置的详细信息，请参阅配置指南。

EdgeConnect 的训练分为三个阶段：1）边缘模型的训练；2）内部模型的训练；3）联合模型的训练。训练模型：

> 
python train.py --model [stage] --checkpoints [path to checkpoints]


例如，要在./checkpoints/places2 目录下的 places2 数据集上训练边缘模型：

> 
python train.py --model 1 --checkpoints ./checkpoints/places2


模型的收敛性因数据集而异。例如，Places2 数据集在两个时期中的一个就能聚合，而较小的数据集（如 CelebA）则需要将近 40 个时期才能聚合。你可以通过更改配置文件中的 MAX_ITERS 值来设置训练迭代次数。雷锋网

**2 .测试**

要测试模型，请创建一个与[示例配置文件](https://github.com/knazeri/edge-connect/blob/master/config.yml.example)类似的 config.yaml 文件，并将其复制到检查点目录下。

你可以在所有三个阶段上测试模型：边缘模型、内部模型和联合模型。在每种情况下，都需要提供一个输入图像（带掩膜的图像）和一个灰度掩膜文件。请确保掩膜文件覆盖输入图像中的整个掩膜区域。测试模型：

> 
python test.py \

--model [stage] 

--checkpoints [path to checkpoints] \

--input [path to input directory or file] \

--mask [path to masks directory or mask file] \

--output [path to the output directory]


我们在./examples 目录下提供了一些测试示例，请下载预训练模型并运行：

> 
python test.py \

--checkpoints ./checkpoints/places2

--input ./examples/places2/images

--mask ./examples/places2/masks

--output ./checkpoints/results


此脚本将在./examples/places2/images 中使用和./examples/places2/mask 对应的掩膜图像，并将结果保存在./checkpoints/results 目录中。默认情况下，test.py 脚本在阶段 3 上运行（--model=3）。

**3 .评估**

要评估模型，你需要首先在测试模式下对 validation 集运行模型，并将结果保存到磁盘上。我们提供了一个实用程序./scripts/metrics.py，使用 PSNR, SSIM 和 Mean Absolute Error 评估模型： 

> 
python ./scripts/metrics.py --data-path [验证集路径] --输出路径 [模型输出路径]


要测量 FID 分数，请运行./scripts/fid_score.py。我们利用了这里的 FID 的 pytorch 实现，它使用了 pytorch 初始模型中的预训练权重。

> 
python ./scripts/fid_score.py --path [验证集路径, 模型输出路径] --gpu [要使用的 gpu id] 


via：[https://github.com/knazeri/edge-connect](https://github.com/knazeri/edge-connect)

雷锋网版权文章，未经授权禁止转载。详情见[转载须知](http://dwz.cn/4ErMxZ)。



