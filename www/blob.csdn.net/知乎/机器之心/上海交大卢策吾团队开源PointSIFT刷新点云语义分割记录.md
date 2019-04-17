# 上海交大卢策吾团队开源PointSIFT刷新点云语义分割记录 - 知乎
# 



> 上海交通大学卢策吾团队 MVIG 实验室最近开源了 PointSIFT，这是一个点云特征的提取模块。在 Stanford Large-Scale 3D Indoor Spaces(S3DIS) [1] 中可以达到 70.23 的 mIoU（对比 PointCNN 62.74, 相对提高 11.9%）。在另一个常用数据集Stanford ScanNet [2]上可以达到 41.50 的 mIoU（对比 PointNet++ 38.28, 相对提高 8.1%）
![](https://pic2.zhimg.com/v2-a2bbc2800a508d6e6398f1f93104b161_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='721'></svg>)
**论文：PointSIFT: A SIFT-like Network Module for 3D Point Cloud Semantic Segmentation**

论文作者：Mingyang Jiang、Yiran Wu、Cewu Lu （通讯作者）
- 阅读论文：arXiv:1807.00652, 2018；[https://arxiv.org/abs/1807.00652](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1807.00652)
- 论文主页：[http://www.mvig.org/publications/pointSIFT.html](https://link.zhihu.com/?target=http%3A//www.mvig.org/publications/pointSIFT.html)
- 代码链接：[https://github.com/MVIG-SJTU/pointSIFT](https://link.zhihu.com/?target=https%3A//github.com/MVIG-SJTU/pointSIFT)
![](https://pic1.zhimg.com/v2-f9c0b91ec6f94958513022dc01389544_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='695' height='584'></svg>)
众所周知，三维语义分割存在着很多的挑战。由于计算量的原因，我们无法将 2D 卷积神经网络直接推广到 3D。自从 PointNet 系列出现之后，大家开始使用原始点云作为基本输入。这样做能够的保留原始数据的内在关系，并且也能够减少很多不必要的计算。

这个框架现阶段仍有一些问题, 比如每个点操作过于独立，而无法高效刻画相关区域的语义结构。针对这些问题，受到传统 SIFT feature 设计的启发，上海交大 MVIG 组提出了基于 PointSIFT 算子的框架。在结构语义描述上，传统 SIFT feature 设计是最有效的描述算子之一。在图像上，SIFT 算子能编码区间上各个方向的信息，同时选择最合适的表征尺度。我们的 pointSIFT 将其设计思想推广到 3D 点云域上，对于每一个点云能端对端地输出一个表征向量，该向量编码了各个方向的信息，同时自适应地选择合适的表征尺度。不同于 SIFT 传统算法，我们采用网络结构，网络参数是由训练获得。
![](https://pic3.zhimg.com/v2-d3d905b127d83d7d3e17f143b48bbd96_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='309'></svg>)三维点云 PointSift 模块与图像 SIFT 算子的类比
pointSift 模块作为一个通用提高表征能力的模块，可以灵活地嵌入在各种 pointnet 框架中，比如下图所示。
![](https://pic4.zhimg.com/v2-3cff4ec9e560a85baa125e8b43fa2683_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='313'></svg>)基于 pointSIFT 嵌入的点云分割网络 SA 和 FP 分别为编码器 (Set Abstraction) 和解码器 (Feature Propagation) 模块
**参考文献：**
- Iro Armeni, Ozan Sener, Amir R. Zamir, Helen Jiang, Ioannis Brilakis, Martin Fischer, and Silvio Savarese. 3d semantic parsing of large-scale indoor spaces. In Proceedings of the IEEE International Conference on Computer Vision and Pattern Recognition, 2016.

- Angela Dai, Angel X. Chang, Manolis Savva, Maciej Halber, Thomas Funkhouser, and Matthias Nießner. Scannet: Richly-annotated 3d reconstructions of indoor scenes. In Proc. Computer Vision and Pattern Recognition (CVPR), IEEE, 2017.
- Charles R Qi, Hao Su, Kaichun Mo, and Leonidas J Guibas. Pointnet: Deep learning on point sets for 3d classification and segmentation. arXiv preprint arXiv:1612.00593, 2016.
- Lyne P. Tchapmi, Christopher B. Choy, Iro Armeni, JunYoung Gwak, and Silvio Savarese. Segcloud: Semantic segmentation of 3d point clouds. CoRR, abs/1710.07563, 2017.
- Loïc Landrieu and Martin Simonovsky. Large-scale point cloud semantic segmentation with superpoint graphs. CoRR, abs/1711.09869, 2017.
- Charles R Qi, Li Yi, Hao Su, and Leonidas J Guibas. Pointnet++: Deep hierarchical feature learning on point sets in a metric space. arXiv preprint arXiv:1706.02413, 2017.
- Y. Li, R. Bu, M. Sun, and B. Chen. PointCNN. ArXiv e-prints, January 2018.
![](https://pic1.zhimg.com/v2-7617d4bf78d79b404507278f7431d074_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='300' height='300'></svg>)
Prof. Cewu Lu is a research Professor at Shanghai Jiao Tong University, leading Machine Vision and Intelligence Group. He is also one of MIT TR35 -"MIT Technology Review, 35 Innovators Under 35 (China)". He was Postdoc at Stanford AI lab (under Fei-Fei Li and Leonidas Guibas) and selected as the 1000 Overseas Talent Plan (Young Talent) (中组部青年千人计划).  




