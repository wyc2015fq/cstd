
# 何恺明、陈鑫磊新研究：提出实例分割新方法，效果比肩Mask R-CNN - 喜欢打酱油的老鸟 - CSDN博客


2019年04月01日 12:32:51[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：11


[https://www.toutiao.com/a6674769574212141579/](https://www.toutiao.com/a6674769574212141579/)
何恺明大神的团队又有新研究了！
这一次，Facebook的陈鑫磊、何恺明等人，又从全新的角度，再次解决了实例分割任务中的难题：
他们提出一种通用的实例分割框架**TensorMask**，弥补了**密集滑动窗口实例分割**的短板。
在COCO数据集上进行测试实例分割结果可以发现，TensorMask的效果可以比肩Mask R-CNN。
这是种怎样的新方法？
![何恺明、陈鑫磊新研究：提出实例分割新方法，效果比肩Mask R-CNN](http://p1.pstatp.com/large/pgc-image/16d6f04bb2ff405ab53e9681f17cb5b8)

**4D张量**
在论文TensorMask: A Foundation for Dense Object Segmentation中，研究人员详细介绍了通用框架TensorMask。
此前，实例分割的主流方法是Mask R-CNN。在这种方法中，模型先检测物体的边界框，然后进行裁剪和目标分割。
但是，密集滑动窗口的实例分割 (Dense Sliding-window Instance Segmentation)却鲜少被人关注。在这种方法中，每个空间位置的输出本身具有自己空间维度的几何结构，与Mask R-CNN有本质上的区别。
为了形式化说明这一方法，研究人员将密集实例分割看作是4D张量（4D tensors）上的预测任务，并提出通用框架TensorMask获取这种几何结构。
![何恺明、陈鑫磊新研究：提出实例分割新方法，效果比肩Mask R-CNN](http://p3.pstatp.com/large/pgc-image/8d360c38cfbc446c942b43f5d276dc6d)

TensorMask的核心变化是，用结构化的高维张量表示一组密集滑动窗口中的图像内容。
![何恺明、陈鑫磊新研究：提出实例分割新方法，效果比肩Mask R-CNN](http://p1.pstatp.com/large/pgc-image/eb9d855066e5413889f8083bc3345c3b)

TensorMask中包含两部分，一是预测蒙版的Head，负责在滑动窗口中生成蒙版，二是进行分类的Head，负责预测目标的类别。
![何恺明、陈鑫磊新研究：提出实例分割新方法，效果比肩Mask R-CNN](http://p3.pstatp.com/large/pgc-image/4f0400017d26473b9234c08250696586)

两者分工明确，各司其职。
在COCO数据集上进行实例分割检测可以发现，TensorMask在test-dev上的平均精度达到了35.5，与Mask R-CNN的36.8非常接近。
![何恺明、陈鑫磊新研究：提出实例分割新方法，效果比肩Mask R-CNN](http://p1.pstatp.com/large/pgc-image/bde8cbcfcaca446c9e3e2711ff0d520c)

结果证明，在定量和定性上，TensorMask均接近Mask R-CNN。
![何恺明、陈鑫磊新研究：提出实例分割新方法，效果比肩Mask R-CNN](http://p9.pstatp.com/large/pgc-image/862ece7882b442dbb8e0448b7e535af5)

**中国少年×2**
这项新研究的背后，少不了中国研究人员的足迹。
论文一作是中国少年陈鑫磊，目前就职于Facebook，一路走来其研究经历闪闪发光。
陈鑫磊在浙江大学攻读计算机科学专业学习时，师从蔡登教授。本科毕业后奔赴CMU攻读博士，师从Abhinav Gupta教授，主要攻读计算机视觉方向。
![何恺明、陈鑫磊新研究：提出实例分割新方法，效果比肩Mask R-CNN](http://p3.pstatp.com/large/pgc-image/e7d3fd0c17844e08b71383798d785b1d)

在博士毕业前，陈鑫磊在Google Cloud AI部门实习过，当时在李飞飞和李佳两位大牛的项目组内。
在陈鑫磊的个人主页上可以看到，其参与过的多篇论文都被顶会接收。
![何恺明、陈鑫磊新研究：提出实例分割新方法，效果比肩Mask R-CNN](http://p1.pstatp.com/large/pgc-image/12fb6b83d373440a82da69ae317126e5)

三作何恺明大家应该不陌生，作为Mask R-CNN的主要提出者，何恺明三次斩获顶会最佳论文。
![何恺明、陈鑫磊新研究：提出实例分割新方法，效果比肩Mask R-CNN](http://p3.pstatp.com/large/pgc-image/bbf0c69dcd5a45c5ad75e419893df77f)

何恺明曾是广东高考状元，并保送了清华。本科毕业后，何恺明进入香港中文大学读研。期间继续在微软亚洲研究院参与研究，现也就职于Facebook。
论文的二作Ross Girshick和四作Piotr Dollar陈鑫磊和何恺明在Facebook的同事，顶会论文Mask R-CNN、Focal Loss for Dense Object Detection等，都是三人曾合作开展的研究。
有友如此，甚好甚好。
**传送门**
论文TensorMask: A Foundation for Dense Object Segmentation：
https://arxiv.org/abs/1903.12174
![何恺明、陈鑫磊新研究：提出实例分割新方法，效果比肩Mask R-CNN](http://p3.pstatp.com/large/pgc-image/63cdad298ec145caa84d9795810c9d0e)

论文中表示，研究结果将于不久后开源~
值得期待。
—**完**—

