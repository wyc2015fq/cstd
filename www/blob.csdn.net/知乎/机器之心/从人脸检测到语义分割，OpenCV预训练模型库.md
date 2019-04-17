# 从人脸检测到语义分割，OpenCV预训练模型库 - 知乎
# 



选自GitHub，机器之心编辑。


**参与：刘晓坤**

> OpenCV 的 GitHub 页面中有一个称为「open_model_zoo」的资源库，里面包含了大量的计算机视觉预训练模型，并提供了下载方法。使用这些免费预训练模型可以帮助你加速开发和产品部署过程。

项目地址：[https://github.com/opencv/open_model_zoo](https://link.zhihu.com/?target=https%3A//github.com/opencv/open_model_zoo)

open_model_zoo 预训练模型概览：




**目标检测模型**

有几种检测模型可以用于检测一系列最常见的目标。大多数网络都是基于 SSD 并提供了合理的准确率/速度权衡。这个列表有人脸、人物、汽车、自行车等目标的检测模型，其中包含一些检测相同类型的目标的网络（例如 face-detection-adas-0001 和 face-detection-retail-0004），因而你可以选择更高准确率/更广泛应用的网络，但同时存在更慢推理速度的代价。
![](https://pic4.zhimg.com/v2-3a72b0e211497daba074d492d6ab8187_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='681'></svg>)



**示例模型：face-detection-adas-0001**

这个人脸检测器用于驾驶员状态检测和类似场景。该网络以 MobileNet 作为骨干，包含深度可分卷积来减少 3x3 卷积的计算量。
![](https://pic3.zhimg.com/v2-07d20f6695c9a48aa893b705f1486122_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='916' height='502'></svg>)face-detection-adas-0001 人脸检测应用示例![](https://pic4.zhimg.com/v2-6f6c3373b2a21f153a0b44fe3fbfe517_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='606' height='513'></svg>)face-detection-adas-0001 性能指标和规格
**示例模型：person-detection-retail-0001**

这个网络用于零售场景的行人检测，基于 hyper-feature+R-FCN 的骨干。
![](https://pic4.zhimg.com/v2-7ccb63ebb76c6e21ff0d6d1b680e3a83_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='662'></svg>)person-detection-retail-0001 人物检测应用示例
**目标识别模型**

目标识别模型被用于分类、回归和特征识别。可以在使用检测模型之后使用这些网络（例如，在人脸检测之后使用年龄/性别识别）。这个列表包含了年龄/性别、头部姿态、车牌号码、汽车属性、情绪、人脸关键点和人物属性等目标的识别模型。
![](https://pic1.zhimg.com/v2-d5648d0cd175ad7434f9293f4719d938_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='959' height='458'></svg>)
**示例模型：vehicle-attributes-recognition-barrier-0039**
![](https://pic1.zhimg.com/v2-14d16e17d3af888791bcd051062c9ff0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='226' height='327'></svg>)vehicle-attributes-recognition-barrier-0039 汽车属性识别应用示例
**示例模型：person-attributes-recognition-crossroad-0031**
![](https://pic3.zhimg.com/v2-0b979d8e888d2a1b3d57139a50edad6e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='319' height='824'></svg>)
person-attributes-recognition-crossroad-0031 人物属性识别应用示例

**再识别模型**

视频中进行目标的精确追踪是计算机视觉的常见应用（例如，人群计数）。以下网络可以用于这样的场景。输入一个人的图像并估计一个表征该人物外观的高维向量。这个向量可以用于进一步评估：对应同一个人的图像会有很接近的向量（基于 L2 距离指标）。以下列表提供了不同准确率/速度权衡的模型选择。列表中包含了人物和人脸再识别任务的模型。
![](https://pic2.zhimg.com/v2-9451e766c3dc3a77dda9f602c49598fd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1070' height='293'></svg>)
**示例模型：person-reidentification-retail-0031**
![](https://pic4.zhimg.com/v2-e447c3444c5fc8229c9b078f458c687f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='900' height='190'></svg>)person-reidentification-retail-0031ren&amp;#39;w 人物再识别应用示例
**示例模型 face-reidentification-retail-0001**
![](https://pic1.zhimg.com/v2-59284a4924c61cf59f07b8df62bad388_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='571'></svg>)face-reidentification-retail-0001 人脸再识别应用示例
**语义分割模型**

语义分割是目标检测的扩展，其输出是目标的按类别区分的彩色掩码，而不是边框。这些网络比对应的检测模型要大得多，但可以对目标实现更精准的定位，并且不受目标的复杂形状所影响。列表中包含了街景和路面图像的语义分割模型。
![](https://pic4.zhimg.com/v2-f82a52e757e65913559a3f934bbc784f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='840' height='180'></svg>)
**示例模型：semantic-segmentation-adas-0001**
![](https://pic2.zhimg.com/v2-e28ccbd09fa1858fc063e3566616acb5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='774' height='421'></svg>)semantic-segmentation-adas-0001 街景图像语义分割应用示例 
模型下载：[https://github.com/opencv/open_model_zoo/blob/2018/model_downloader/README.md](https://link.zhihu.com/?target=https%3A//github.com/opencv/open_model_zoo/blob/2018/model_downloader/README.md)




