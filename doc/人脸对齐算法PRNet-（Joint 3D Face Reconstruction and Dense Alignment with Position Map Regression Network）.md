# 人脸对齐算法PRNet-（Joint 3D Face Reconstruction and Dense Alignment with Position Map Regression Network）

![96](https://upload.jianshu.io/users/upload_avatars/8448616/61a9b2b4-0f71-425c-8da4-d84be149fa34?imageMogr2/auto-orient/strip|imageView2/1/w/96/h/96)

 

[踩坑第某人](https://www.jianshu.com/u/da3aece2a436)

 

关注

 0.1 2018.09.15 13:18 字数 1547 阅读 3240评论 12喜欢 3

### 摘要

[cvpr2018](https://arxiv.org/pdf/1803.07835.pdf)上交作品，采用encoder-decoder的网络模式，可以端到端地实现由单张RGB人脸图像进行3D人脸重构和密集人脸对齐的联合任务。采用300W-LP作为训练集，对于大姿态人脸也能得到较好的结果。并且速度非常快，GTX1080可达10ms/帧。代码已开源，参见[github](https://github.com/YadiraF/PRNet)（注：关于下文提到的3DDFA，3dmmasSTN等项目的代码，在readme里最后作者有给出，需要的看仔细啦）

### 方法

该方法基本是在前人的基础上一步步改进过来的，如[3DDFA](http://www.cbsr.ia.ac.cn/users/xiangyuzhu/projects/3DDFA/main.htm),
[3DMMasSTN](https://github.com/anilbas/3DMMasSTN)等。总体来讲，可大致分为3个部分：3D人脸模型，3D点云的uv图表示以及网络结构设计

- 3D人脸模型
  参考3DDFA，3D人脸可看做是形状和表情的和，其中3D形状模型可基于BFM模型，通过3DMM（3D Morphable Model）系数来构建，而3D表情模型可FW 模型构建。二者之和再通过Pose(姿态角，平移，尺度)参数投影到摄像机坐标系下，即得到3D点云的图像坐标。需要注意的是，BFM模型的点云数约为53490, FW表情模型的点云数约为53215，因此二者不能直接相加，需首先对BFM模型的点云进行筛选，仅从中提取53215个点（貌似是去除了嘴巴内部的点），同时修正相应的网格(triangles)，这部分工作也是3DDFA团队的贡献。

- 3D点云的UV图表示
  通过BFM形状模型和表情模型，可以得到最终的3D点云的图像坐标(共53215个)，每个点有x,y,z 3个坐标，共有53215x3个值，这些点云中的68个点x,y坐标即为常用的68个人脸关键点，约40k个点的x，y坐标即为密集人脸关键点。因此，本文的目标就是从单张2D人脸RGB图像中直接预测这约53k的点的3维坐标值。一个简单且普遍的是用一个1D向量来表示，即将3D点信息用一个向量来表示，然后用网络预测；然而，这种方法丢失了空间信息。
  相关研究中也有预测3DMM等模型的系数，然后同坐模型建模来得到3D点云，但这些方法太过依赖3DMM模型，并且流程复杂；最近的VRN用Volumetric来表示，成功摆脱了上述问题，但是其网络需要输出192x192x200的一个Volume，计算量相当大，重建分辨率将会受到限制。
  针对这些问题，作者非常巧妙地将53215x3个值用一个3通道256x256的图像来表示，即UV position map来表示，如下图

  

  ![img](https://upload-images.jianshu.io/upload_images/8448616-a21a2b979cd578c3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/854/format/webp)

  UV 位置图

  左图是输入图像的3D图和3D点云ground-truth；右边第1行，分别是2D图像，UV文理映射图，相应的UV位置映射图；右边第2行，分别是UV位置映射图的x，y，z通道。实际上UV 位置图最初来源于3dmmasSTN，与3DDFA中的PNNC有类似的功能，二者详细内容及区别请参见这两篇论文介绍。

- 网络结构设计
  论文网络基于tensorflow实现，共包含一层卷积层，10层resblock残差块（encoder）和17层转置卷积(decoder)，其中中间各层卷积、转置卷积后跟着batchnorm和relu，最后一层转置卷积的激活采用Sigmoid。encoder中各层的kernel均为4， out channel从16到512，输出尺寸从256到8(每两个resblock尺寸减半)，decoder则反之，但细节与encoder仍存在些许差别，详见代码。输入和输出均为256x256 RGB图像，其中输入是人脸图像，输出是3D点云坐标（共65536个）。最后分别从中提起相应点的2d、3d坐标来进行3D重建或人脸关键点。详细结构见下图

  

  ![img](https://upload-images.jianshu.io/upload_images/8448616-49e2e3cee53fc8fe.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/854/format/webp)

  resfcn256

### 结果



![img](https://upload-images.jianshu.io/upload_images/8448616-31f50fd57e431f3a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)

68人脸关键点



![img](https://upload-images.jianshu.io/upload_images/8448616-e81d45627b4cfd9c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/840/format/webp)

3D重建

### 训练

- 数据集
  在300W-LP数据集上训练，该数据集是3DDFA团队基于现有的AFW，IBUG，HEPEP，FLWP等2D人脸对齐数据集，通过3DMM拟合得到的3DMM标注，并对Pose，light，color等进行变化以及对原始图像进行flip(镜像)得到的一个大姿态3D人脸对齐数据集。
  当然作者也提出，可以采用其他的数据集来训练，那样的话可能就不需要3dmm模型等相关的处理技巧，反正无论如何，流程都是：得到3D点云-> 插值形成uv位置图->训练网络这个步骤。只是得到3d点云的方式不同而已。
- 预处理
  关于由3DMM系数得到3D点云的过程前文已介绍，此处需指出的是，groundtruth 点云共53215个，通过3dmmasSTN提供的插值方法，将这53k的点云通过3角插值，最终形成训练网络的label:256x256。其他细节诸如数据增广，输入输出归一化，输出z坐标大于0等的处理不再详述，论文都有介绍啦。
- 代码实现
  采用的框架是tensorflow，不得不说的是，作者代码功底算是算法领域很好的水平，代码无论是网络实现，还是api接口，都非常简洁易懂，并且完全用python实现了3ddfa，3dmmasSTN中相关处理的matlab代码，大大的佩服。

注：鉴于本人对3D模型的了解也只是皮毛，3d模型那块的介绍并不深入甚至会有纰漏，因此想深入了解的还是看论文比较简单粗暴。