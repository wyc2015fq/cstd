# 用PyTorch搞定GluonCV预训练模型，这个计算机视觉库真的很好用 - 知乎
# 



机器之心编辑，参与：思源。

> 今年上半年，DMLC 团队发布了简单易用的计算机视觉工具箱 GluonCV，它继承了 MXNet 动态图接口 Gluon 的优良传统，并能使用简单易用的 API 快速构建复杂的深度神经网络。这一工具非常好用，因此很多研究者希望在 PyTorch 等其它框架上调用它。Amazon AI 的应用科学家张航博士将 GluonCV 转换为了 PyTorch 版工具，从而能直接在 PyTorch 上调用图像分类和语义分割等各种预训练模型。

项目地址：[https://github.com/zhanghang1989/gluoncv-torch](https://link.zhihu.com/?target=https%3A//github.com/zhanghang1989/gluoncv-torch)

张航博士是 DMLC 团队成员，在 GluonCV 的 Contributors 中可以看到他的贡献量很高，因此他完成的 GluonCV-Torch 库是非常值得尝试的一项工具。在本文中，我们不仅会简要介绍 GluonCV-Torch 及它的使用，同时还会简单地试用这些预训练模型，包括 DeepLabV3 语义分割模型。

## **GluonCV-Torch 简介**

目前 GluonCV 已经包含非常多的预训练模型与 CV 工具，包括 50 多种图像分类模型、SSD 和 Yolo-v3 等目标检测模型、FCN 和 DeepLab-v3 等语义分割模型，除此之外还有实例分割、生成对抗网络和行人再识别等模型。而目前 GluonCV-Torch 主要提供了图像分类与语义分割两部分的预训练模型，其中分类模型都是在 ImageNet 实现的预训练，而语义分割模型分别在 Pascal VOC 和 ADE20K 实现预训练。

安装 GluonCV-Torch 的安装方法非常简单，只要我们预先安装了 PyTorch 就行了：

`pip install gluoncv-torch`

如果在 PyTorch 中加载 GluonCV，我们可以简单地导入 gluoncvth 模块，并从该模块调用比 torchvision 中更好的预训练模型：


```
import gluoncvth as gcv

model = gcv.models.resnet50(pretrained=True)
```


对于图像分类方面的预训练模型，GluonCV-Torch 以便捷的接口提供了准确率非常高的预训练模型。如下所示为不同预训练模型的效果：
![](https://pic1.zhimg.com/v2-f741562fba2080928bcd7232b908cf24_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='833' height='466'></svg>)
对于语义分割模型，GluonCV-Torch 主要支持预训练的 FCN、PSPNet 和 DeepLab-V3，其中 DeepLab-V3 是非常常用的开源模型，它在语义分割任务上有非常好的效果。如下展示了这三种模型在 Pascal VOC 数据集中的预训练效果，其中 Pascal VOC 包含 20 种类别的图像：
![](https://pic2.zhimg.com/v2-9e87fe52e760e1163cb329b3673bada9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='482' height='242'></svg>)
以下展示了三种语义分割模型在 ADE20K 数据集的效果，其中 ADE20K 为 MIT 发布的场景解析数据集，该数据集包含多种情景，包括人物、背景和物体等。
![](https://pic4.zhimg.com/v2-b2a84017748c50666ffbfb380e24e6af_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='584' height='237'></svg>)
## **GluonCV-Torch 使用**

在该项目中，张航展示了一个简单的使用示例，其调用了在 ADE20K 数据集上预训练的 DeepLabV3 语义分割模型。


```
import torch
import gluoncvth

# Get the model
model = gluoncvth.models.get_deeplab_resnet101_ade(pretrained=True)
model.eval()

# Prepare the image
url = 'https://github.com/zhanghang1989/image-data/blob/master/encoding/' + \
    'segmentation/ade20k/ADE_val_00001142.jpg?raw=true'
filename = 'example.jpg'
img = gluoncvth.utils.load_image(
    gluoncvth.utils.download(url, filename)).unsqueeze(0)

# Make prediction
output = model.evaluate(img)
predict = torch.max(output, 1)[1].cpu().numpy() + 1

# Get color pallete for visualization
mask = gluoncvth.utils.get_mask_pallete(predict, 'ade20k')
mask.save('output.png')
```


运行上面的代码会自动从 AWS 云储存上下载 200 多兆的预训练模型，国内的下载可能会比较慢，我们可以使用其它工具下载并解压到对应文件夹。机器之心在 PyTorch 0.4.1 的环境下可以安装 GluonCV-Torch，并成功运行，我们在 CPU 上推断一张图片的时间约为 70 多秒，在 GPU（K80）上的推断时间为 10 多秒。

如果我们要推断其它的图像或使用其它预训练模型，可以直接修改 load_image 的图像路径和 gluoncvth.models 就可以了。

## **API Reference**

以上只是两个简单的示例，更多的模型和用法还需要根据实际情况修改。以下展示了目前 GluonCV-Torch 的主要模型 API：

ResNet
- gluoncvth.models.resnet18(pretrained=True)
- gluoncvth.models.resnet34(pretrained=True)
- gluoncvth.models.resnet50(pretrained=True)
- gluoncvth.models.resnet101(pretrained=True)
- gluoncvth.models.resnet152(pretrained=True)

FCN
- gluoncvth.models.get_fcn_resnet101_voc(pretrained=True)
- gluoncvth.models.get_fcn_resnet101_ade(pretrained=True)

PSPNet
- gluoncvth.models.get_psp_resnet101_voc(pretrained=True)
- gluoncvth.models.get_psp_resnet101_ade(pretrained=True)

DeepLabV3
- gluoncvth.models.get_deeplab_resnet101_voc(pretrained=True)
- gluoncvth.models.get_deeplab_resnet101_ade(pretrained=True)


