# 生成模型--PTGAN - wydbyxr的博客 - CSDN博客
2018年10月26日 10:18:22[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：148
# PTGAN
  针对于ReID的生成对抗网络PTGAN，可以实现不同ReID数据集的行人图片迁移，在保证行人本体前景不变的情况下，将背景转换成期望的数据集style。
## 论文关键点
  1）论文提出LID损失，用PSPNet提取的前景，这个前景就是一个mask;
  2）将另外一个数据集通过 PTGAN 迁移到 MSMT 上都能增加 MSMT17 数据集上的performance。
  3）cycleGAN + mask_loss
