# PyTorch—Mask R-CNN（概述） - wsp_1138886114的博客 - CSDN博客





2019年01月06日 16:45:14[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：404








图像分割与识别平台，10个月前Facebook曾发布过一款，名叫[Detecron](https://mp.weixin.qq.com/s?__biz=MzA3MzI4MjgzMw==&mid=2650736547&idx=3&sn=a326f17f832cec4827596291930e76dd&chksm=871ac3ddb06d4acb3ffc56ffd56b340d5f15b7180a8aff5a38ea41c3c4e41e8d96da4c5a0a08&scene=21#wechat_redirect)的项目。其中也包含Mask R-CNN。不过它是基于Caffe 2深度学习框架编写的。Detectron 目前包含以下目标检测算法的实现：Mask R-CNN 、RetinaNet、Faster R-CNN、RPN、Fast R-CNN、R-FCN。（本人一直期待基于PyTorch版本）这一次，Facebook换上了更受欢迎的PyTorch框架。除了更改框架，Mask R-CNN Benchmark相比它的“前辈”Detectron，训练速度提高了一倍（这才是重点）。

另外，商汤和香港中文大学的多媒体实验室也开源了一个类似项目：[mmdetection](https://blog.csdn.net/wsp_1138886114/article/details/83757192)。它支持Faster R-CNN、Mask R-CNN、RetinaNet等等，相比Facebook的Detecron有5%到20%的性能提升。这个模型还在2018年的COCO Detection竞赛中拿下了冠军。
- 但是此次Pytorch有如下优点：
PyTorch 1.0编写：RPN、Faster R-CNN和Mask R-CNN均可实现，达到甚至超出Detectron的准确度

快速：训练速度是Detectron的2倍，比mmdetection高30%。

显存效率更高：大约比mmdetection少使用500MB显存

支持多GPU训练与推断

支持以CPU进行推断

支持图像批处理：可分批分GPU对多图进行推断

提供预训练模型：针对几乎所有引用Faster RCNN和Mask RCNN的架构
使用Mask R-CNN Benchmark需要安装以下组件：

```python
PyTorch 1.0
torchvision
cocoapi
yacs
matplotlib
opencv-python
```
- 
R-CNN发展历史

R-CNN是卷积神经网络（CNN）在图像识别领域的应用，Facebook AI研究团队在这条到道路上做出了颇多贡献，其中不得不提一位大神：Ross Girshick。他发明了RCNN，又提出速度更快的Fast R-CNN。

2016年，微软研究院提出了Faster R-CNN，降低了在边框搜索上的运算量，进一步提高了算法的速度。

2017年，Facebook AI研究团队又再次提出了Mask R-CNN：通过添加与现有分支并行的对象掩码（object mask）分支，以增强Faster RCNN在边框识别上的性能。- 
传送门

Mask R-CNN Benchmark项目地址：[https://github.com/facebookresearch/maskrcnn-benchmark](https://github.com/facebookresearch/maskrcnn-benchmark)

印度小哥Mask R-CNN项目地址：[https://github.com/wannabeOG/Mask-RCNN](https://github.com/wannabeOG/Mask-RCNN)

Detecron项目地址：[https://github.com/facebookresearch/Detectron](https://github.com/facebookresearch/Detectron)

mmdetection项目地址：[https://github.com/open-mmlab/mmdetection](https://github.com/open-mmlab/mmdetection)
关于该框架的使用请查看以后的文章







