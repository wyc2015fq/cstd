# 目标检测（二十三）--DenseBox - 工作笔记 - CSDN博客





2017年09月12日 09:40:01[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：10967
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









﻿﻿

转自：[http://blog.csdn.net/cv_family_z/article/details/50172741](http://blog.csdn.net/cv_family_z/article/details/50172741)

**DenseBox: Unifying Landmark Localization with End to End Object Detection**
- arxiv: [http://arxiv.org/abs/1509.04874](http://arxiv.org/abs/1509.04874)
- demo: [http://pan.baidu.com/s/1mgoWWsS](http://pan.baidu.com/s/1mgoWWsS)
- KITTI result: [http://www.cvlibs.net/datasets/kitti/eval_object.php](http://www.cvlibs.net/datasets/kitti/eval_object.php)

百度深度学习研究院 的目标检测工作 类似 Faster R-CNN,后来好像是地平线ADAS的车辆检测 核心算法

![这里写图片描述](https://img-blog.csdn.net/20151204111731890)

输入多尺度图像，经过CNN处理，输出目标框

Model Design

![这里写图片描述](https://img-blog.csdn.net/20151204111907725)

模型是从 19层的VGG模型演变来的。

Multi-Level Feature Fusion： 将不同卷积层的特征联系起来可以提高检测效果

Multi-Task Training： 两个任务：1）目标有无，2）目标的位置

这里使用了 landmark 来改善定位模型
![这里写图片描述](https://img-blog.csdn.net/20151204112219629)

![这里写图片描述](https://img-blog.csdn.net/20151204113226306)

KITTI 数据库 车辆检测上面的结果：
![这里写图片描述](https://img-blog.csdn.net/20151204113301625)

![这里写图片描述](https://img-blog.csdn.net/20151204113352021)

还有一个 DenseBox2 敬请期待！



