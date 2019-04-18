# 生成模型--SPGAN - wydbyxr的博客 - CSDN博客
2018年10月26日 10:21:24[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：374
# SPGAN
  中了CVPR 2018，属于ReID领域。
  全名是“Image-Image Domain Adaptation with Preserved Self-Similarity and Domain-Dissimilarity for Person Re-identification”。
## 论文关键点
**SPGAN = CycleGAN + L(ide)+L(con)**
  similarity preserving GAN (SPGAN)包含两个部分，一个是CycleGAN+L(ide)，另一个是SiaNet（孪生网络）。其中，SiaNet就是用于实现self-similarity 和 domain-dissimilarity的。
  SiaNet的学习目标也是要让正对的距离小（即，self-similarity ），负对的距离大（即，domain-dissimilarity）。因此，作者提出了Similarity preserving loss 来实现这个功能。
  正对：表示同一张图片，转换前后需要尽量相似。
  负对：反映domain-dissimilarity，也有两种类型,表示来自不同domain的图片，即使转换到相同domain，也不能相似。"
