# Python MTCNN（人脸检测）项目附代码讲解(1)-原理与论文介绍 - weixin_33985507的博客 - CSDN博客
2018年12月16日 22:06:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：44
**首先介绍下什么是MTCNN？**
论文：Joint Face Detection and Alignment using Multi-task Cascaded Convolutional Networks （获取原论文，私信我回复“MTCNN”即可获得）
**MTCNN提出了一种Multi-task的人脸检测框架，将人脸检测和人脸特征点检测同时进行（我这里没有进行人脸特征点检测，其实了解了你就知道了，多几行数据代码就可以了）。论文使用3个CNN级联的方式**。
这个MTCNN的简介在网上一搜一大把，所以也就不再做太多的简介和名头讲解了，总结下里面的主要结构点和后续的讲解重点。
项目建立说明：
**数据集：**CelebaA(这个数据集不是很大，大概20G，不过也可以使用3万多张的进行训练）
**GPU型号：**GTX1060 6G(虽然这个网络不是很大，但是还是GPU运行起来比较快）
**框架：**Torch
**语言**：Python
项目流程
**MTCNN的主要结构介绍：**
![13692432-f119e5fbe7d69ec9](https://upload-images.jianshu.io/upload_images/13692432-f119e5fbe7d69ec9)
**思维导图的梳理：**
![13692432-1ba820dc3cb1b6c7](https://upload-images.jianshu.io/upload_images/13692432-1ba820dc3cb1b6c7)
图一直不清楚，但是这个没事的，等我讲解完，会把pdf上传到网盘
**模型建立code：**
![13692432-9536d5f996cc95ab](https://upload-images.jianshu.io/upload_images/13692432-9536d5f996cc95ab)
**训练优化：**
![13692432-ee12a4caa18aedeb](https://upload-images.jianshu.io/upload_images/13692432-ee12a4caa18aedeb)
**效果展示：**
![13692432-6c09f39878cf7a83](https://upload-images.jianshu.io/upload_images/13692432-6c09f39878cf7a83)
**问题总结：**
![13692432-dd9167eb0fae83fe](https://upload-images.jianshu.io/upload_images/13692432-dd9167eb0fae83fe)
如果你看过我之前的AI建模流程的话，那么这个流程对于你来说其实并不难，
以上就是后续要讲解的所有过程，我就先不做过多的详细解释了，大概看下流程，后续会进行数据说明与代码展现，带领大家一点一点建立起这个网络~人脸检测~
喜欢就关注我哦，持续日更中……
