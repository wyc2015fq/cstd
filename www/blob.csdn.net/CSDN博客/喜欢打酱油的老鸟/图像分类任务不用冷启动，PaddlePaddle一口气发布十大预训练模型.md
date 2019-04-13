
# 图像分类任务不用冷启动，PaddlePaddle一口气发布十大预训练模型 - 喜欢打酱油的老鸟 - CSDN博客


2019年01月30日 08:08:58[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：48


[https://www.toutiao.com/a6645090596073505293/](https://www.toutiao.com/a6645090596073505293/)
2019-01-11 12:25:20
PaddlePaddle在不断增加官方支持的模型的同时，也在关注预训练模型的丰富度。在过去的版本中，已发布了目标检测Faster-RCNN、MobileNet-SSD、PyramidBox和场景文字识别CRNN-CTC、OCR Attention共计5个预训练模型。近期，在图像分类领域，PaddlePaddle一口气发布了四个系列共十个预训练模型，丰富扩充了预训练模型库，助力用户提高构建模型的效率，大大减轻"炼丹"的烦恼。
主要包括
**MobileNet v1**
针对亟需在移动端应用深度学习技术的需求，MobileNet v1在存储空间和能耗低的地方表现优秀，在损失精度很小的情况下，计算量，存储空间，准确率方面都有明显提升。
**ResNet 系列模型**
ResNet创新性的提出了残差结构，一举在ILSVRC2015比赛中取得冠军，top5错误率为3.57%。斯坦福大学的Joyce Xu将ResNet称为「真正重新定义了我们看待神经网络的方式」的三大架构之一。
在最新发布的PaddlePaddle 预训练模型包括有ResNet50，ResNet101和ResNet152。
**SE_ResNeXt 系列模型**
SE 全称 Sequeeze-and-Excitation，在ILSVRC 2017 的分类项目中取得 了第一名的成绩。在 ImageNet 数据集上将 top-5 错误率从原先的最好成绩 2.991% 降低到 2.251%。
在最新发布的PaddlePaddle 预训练模型包括有SE_ResNeXt50_32x4d和SE_ResNeXt101_32x4d。
**VGG 系列模型**
VGGNet是牛津大学计算机视觉组和DeepMind公司共同研发一种深度卷积网络，2014年在ILSVRC比赛上获得了分类项目的第二名和定位项目的第一名。
在最新发布的PaddlePaddle预训练模型包括有VGG11，VGG13，VGG16，VGG19。
**PaddlePaddle复现结果**
以上预训练模型均经过官方测试验证，在精度上皆达到了应用要求。
您可以在他们的基础上进行开发，省却自己训练参数的过程，具体加载方式参考使用说明书。
![图像分类任务不用冷启动，PaddlePaddle一口气发布十大预训练模型](http://p1.pstatp.com/large/pgc-image/7139201960a0445db9368d867e625dda)
**【PaddlePaddle预训练模型使用说明书】**
**1. 安装PaddlePaddle和模型库**
在PaddlePaddle的模型库中已经包含了最新的相关训练代码，经过简单的配置与加载，即可快速部署研发，首先请安装最新版的**PaddlePaddle**并且下载**PaddlePaddle**模型库：
![图像分类任务不用冷启动，PaddlePaddle一口气发布十大预训练模型](http://p3.pstatp.com/large/pgc-image/b2a310ff106c4c8087daee3000493742)
当模型成功克隆后，可以在fluid/PaddleCV/image_classification下看到用于训练的train.py代码。
**2. 加载预训练模型**
使用之前在ImageNet数据集上训练的预训练模型，可以直接使用相应的结构和权重，将它们应用到您正在面对的问题上。
预训练模型从哪里下载呢？首先进入官方模型介绍链接**https://github.com/PaddlePaddle/models/tree/develop/fluid/PaddleCV/image_classification**
下拉到readme页面的最下方：
![图像分类任务不用冷启动，PaddlePaddle一口气发布十大预训练模型](http://p9.pstatp.com/large/pgc-image/da8058a48af04d0883a1966e89b97c96)
可以看到一个Released models的表格。在表格的model一列是模型的名称，这个名称是一个超链接，链接对应的是这个模型的预训练权重下载地址，点击模型名称即可下载相应的预训练模型。
在train.py文件中，
![图像分类任务不用冷启动，PaddlePaddle一口气发布十大预训练模型](http://p3.pstatp.com/large/pgc-image/d65e0dce7de74d74ba4a3beb46910a0e)
通过fluid.io.load_vars加载相关预训练参数。
运行train.py， 通过指定 pretrained_model= "下载好的预训练模型路径"，加载相应预训练模型进行训练。
例如，加载MobileNet v1预训练模型进行微调：
![图像分类任务不用冷启动，PaddlePaddle一口气发布十大预训练模型](http://p3.pstatp.com/large/pgc-image/c8efb2f764c2453b87afb65d3f846a9a)
可以更改调整预训练模型参数，来适应具体工作方向，例如检测，图像分类等。
**结语**
对于想学习算法或者尝试现有框架的人来说，使用预训练模型能够提供很好的帮助。因为时间与计算量方面的诸多限制，不方便随时从头开始训练一个模型，这也正是预训练模型存在的原因。您可以运用预训练作为基准来改进现有模型，或者根据它测试自己的模型。
年后PaddlePaddle还会继续发布几组新的图像分类预训练模型（包括GoogleNet，MobileNetV2，ShuffleNet系列等），敬请期待，也欢迎大家留言告诉我们您最期待的预训练模型。

