# 何恺明、陈鑫磊最新研究：提出实例分割新方法TensorMask，效果比肩Mask R-CNN - 知乎
# 



> 铜灵 发自 凹非寺
量子位 出品 | 公众号 QbitAI

何恺明大神的团队又有新研究了！

这一次，Facebook的陈鑫磊、何恺明等人，又从全新的角度，再次解决了实例分割任务中的难题：

他们提出一种通用的实例分割框架**TensorMask**，弥补了**密集滑动窗口实例分割**的短板。

在COCO数据集上进行测试实例分割结果可以发现，TensorMask的效果可以比肩Mask R-CNN。

这是种怎样的新方法？
![](https://pic2.zhimg.com/v2-58929e3404bff7e0a4228c18ad5af0d1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='315'></svg>)
## **4D张量**

在论文TensorMask: A Foundation for Dense Object Segmentation中，研究人员详细介绍了通用框架TensorMask。

此前，实例分割的主流方法是Mask R-CNN。在这种方法中，模型先检测物体的边界框，然后进行裁剪和目标分割。

但是，密集滑动窗口的实例分割 (Dense Sliding-window Instance Segmentation)却鲜少被人关注。在这种方法中，每个空间位置的输出本身具有自己空间维度的几何结构，与Mask R-CNN有本质上的区别。

为了形式化说明这一方法，研究人员将密集实例分割看作是4D张量（4D tensors）上的预测任务，并提出通用框架TensorMask获取这种几何结构。
![](https://pic4.zhimg.com/v2-7a78dfe41df504a41e4b5d199548b89f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='329'></svg>)
TensorMask的核心变化是，用结构化的高维张量表示一组密集滑动窗口中的图像内容。
![](https://pic2.zhimg.com/v2-8db3362609f925184dd0b57d18951689_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='516'></svg>)
TensorMask中包含两部分，一是预测蒙版的Head，负责在滑动窗口中生成蒙版，二是进行分类的Head，负责预测目标的类别。
![](https://pic1.zhimg.com/v2-43a03acd6a3d08e2b93ffd65c626cfe0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='488'></svg>)
两者分工明确，各司其职。

在COCO数据集上进行实例分割检测可以发现，TensorMask在test-dev上的平均精度达到了35.5，与Mask R-CNN的36.8非常接近。
![](https://pic2.zhimg.com/v2-3065763b95403aa0bc991f04179fc0f9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='273'></svg>)
结果证明，在定量和定性上，TensorMask均接近Mask R-CNN。
![](https://pic1.zhimg.com/v2-fec4185dd4263eddc6c9ee9231af7734_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='269'></svg>)
## **中国少年×2**

这项新研究的背后，少不了中国研究人员的足迹。

论文一作是中国少年陈鑫磊，目前就职于Facebook，一路走来其研究经历闪闪发光。

陈鑫磊在浙江大学攻读计算机科学专业学习时，师从蔡登教授。本科毕业后奔赴CMU攻读博士，师从Abhinav Gupta教授，主要攻读计算机视觉方向。
![](https://pic1.zhimg.com/v2-2390280690ef49e4d7c8c00a70ed2998_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='280'></svg>)
在博士毕业前，陈鑫磊在Google Cloud AI部门实习过，当时在李飞飞和李佳两位大牛的项目组内。

在陈鑫磊的个人主页上可以看到，其参与过的多篇论文都被顶会接收。
![](https://pic3.zhimg.com/v2-cd912ea8229e65662ccc4923b3be4f5a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='382'></svg>)
三作何恺明大家应该不陌生，作为Mask R-CNN的主要提出者，何恺明三次斩获顶会最佳论文。
![](https://pic4.zhimg.com/v2-704027a9889ff395ad94885aa4b11c37_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='353'></svg>)
何恺明曾是广东高考状元，并保送了清华。本科毕业后，何恺明进入香港中文大学读研。期间继续在微软亚洲研究院参与研究，现也就职于Facebook。

论文的二作Ross Girshick和四作Piotr Dollar陈鑫磊和何恺明在Facebook的同事，顶会论文Mask R-CNN、Focal Loss for Dense Object Detection等，都是三人曾合作开展的研究。

有友如此，甚好甚好。

**传送门**

论文TensorMask: A Foundation for Dense Object Segmentation：

[https://arxiv.org/abs/1903.12174](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1903.12174)
![](https://pic2.zhimg.com/v2-90afaae0f5e9d3c5cb6afa76a641f9b1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='268'></svg>)
论文中表示，研究结果将于不久后开源~

值得期待。

— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤








