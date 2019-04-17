# CPU实时人脸检测，各种朝向都逃不过 - 知乎
# 



选自Github，机器之心编译，参与：思源。

> 鉴于一般的 CNN 难以处理旋转的图像，中国科学院计算技术研究所的研究者采用一种渐进的方法校准人脸朝向，并精确检测其位置与角度。他们最近放出了该项研究的源代码，并表示这种人脸检测模型可以在 CPU 上快速进行实时推断。本文介绍了这种具有旋转不变性的神经网络，并展示了对应的 CPU 实时人脸检测项目。

作为 CV 重要的组成部分，人脸检测旨在利用卷积神经网络从人脸图像中抽取足够的信息。然而虽然 CNN 能高效处理图像数据，但大多数情况下它的设计都是针对一般图像处理任务。卷积网络本身并不会太考虑旋转等情况，即使考虑也只是通过数据增强稍微优化一点。在这个项目及对应的论文中，作者提出并实现了一种完全旋转平面（RIP）不变的人脸检测。如下图所示它能检测出人脸的正确朝向，并从任何 RIP 角度捕获面部检测框。
![](https://pic2.zhimg.com/v2-73f924089a6c4fc84505e772fb365299_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='944' height='674'></svg>)
如上所示很多计算机视觉任务或目标检测需要处理不同的旋转方向，而为了解决这样的问题，Xuepeng Shi 等研究者提出了一种用于旋转不变性人脸检测的 PCN（progressive calibration networks）。这种 PCN 能逐步校准每个候选面部的 PIP 方向为垂直方向，并更好地从非面部图像中区分出面部图像。
- 论文：Real-Time Rotation-Invariant Face Detection with Progressive Calibration Networks 
- 论文地址：[https://arxiv.org/pdf/1804.06039.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1804.06039.pdf)

具体而言，PCN 会抽选识别候选面部图像块，并将朝下的图像块翻转至正向，这样就会减半 RIP 的角度范围，即从 [−180° , 180° ] 到 [−90° , 90° ]。然后旋转过的面部图像块会进一步区分朝向并校准到垂直向的 [−45° , 45° ] 范围，这样又会减半 RIP 的角度范围。最后，PCN 会分辨到底这些候选图像块是不是人脸，并预测出精确的 RIP 角度。

通过将校准过程分割为几个渐进的步骤，且在早期校准步骤只预测粗略的朝向，PCN 最后能实现精确的校准。此外，每一个校准步骤可以简单地旋转-90°、90°和 180°，因此额外的计算量非常低，这也就是为什么该检测项目能在 CPU 上实时运行的重要原因。通过在逐渐降低的 RIP 范围内执行二元分类（是人脸或不是人脸），PCN 能在 360° RIP 旋转角度内准确地检测到人脸，而本项目重点就是实现这样旋转不变的人脸检测器。
![](https://pic3.zhimg.com/v2-efd37cfabc3c4ea2b70b7b35c0588cb6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='402'></svg>)
原论文图 3：uepeng Shi 等研究者提出的 PCN 概览，它会逐渐降低旋转的角度范围，并最终预测人脸及其旋转的角度。

这种能处理不同旋转方向的人脸检测器有非常高的准确率，因为它会先将候选人脸旋转至正向再预测。此外，这种方法同样有非常小的计算量，该 GitHub 项目表示它甚至可以在 CPU 上实时检测人脸。

PCN 多角度实时人脸检测项目地址：[https://github.com/Jack-CV/PCN-FaceDetection](https://link.zhihu.com/?target=https%3A//github.com/Jack-CV/PCN-FaceDetection)

下图展示了 PCN 项目在不同情境下的检测效果，我们会发现即使面部的朝向多种多样，它也能非常准确地检测出来。
![](https://pic1.zhimg.com/v2-f472cb730ae13150bded120b6fc11a08_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='553'></svg>)
PCN 的目标是在准确地检测人脸及朝向的情况下尽可能降低计算力。Xuepeng Shi 等研究者同样在标准 VGA 图像（640x480）上对比了 PCN 与其它旋转不变的人脸检测器。这些检测器分别在带有 3.4GHz CPU 和 GTX Titan X GPU 的台式电脑上进行测试，速度测试结果与召回率都展示在下表，更详细的内容请查看原论文。值得注意的是，将方形结果转换为矩形或椭圆型有助于更好地拟合标注值，因此能实现更高的准确率，不过下表并没有做这样的转换。
![](https://pic2.zhimg.com/v2-2571806d320f044fe331dc4be14f81e5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='207'></svg>)



**项目使用方法**

根据 GitHub 项目所示，目前该项目所测试的环境主要在 Linux，且需要 Caffe 和 OpenCV（2.4.10 或其它兼容版本）的支持。因为该项目采用的是 Caffe 框架，所以小编放弃测试了～

以下是一些简要的配置，首先可以设置人脸的最小检测尺寸（size >= 20）：
- detector.SetMinFaceSize(size);


配置图像金字塔的缩放因子（1.4 <= factor <= 1.6）：
- detector.SetImagePyramidScaleFactor(factor);

配置人脸检测的置信度阈值（0 <= thresh1, thresh2, thresh3 <= 1）：
- detector.SetScoreThresh(thresh1, thresh2, thresh3);

是否平滑人脸检测框（smooth = true or false），推荐在视频中使用，它可以获得更加稳定的人脸检测框。
- detector.SetVideoSmooth(smooth);

读者可查看 picture.cpp 和 video.cpp 查看细节，如果你们想在 FDDB 上复现结果，可以直接运行 fddb.cpp。我们可以在 FDDB 中旋转图像以获得 FDDB-left、FDDB-right 和 FDDB-down，然后分别测试它们。具体而编译并运行：


```
cd $PCN_ROOT/code# You should set "CAFFEROOT" in lib.sh, compile.sh, and run.sh first. 
sh lib.sh
sh compile.sh picture/video/fddb
sh run.sh picture/video/fddb
```





