# 生成模型--TP-GAN - wydbyxr的博客 - CSDN博客
2018年10月26日 10:17:00[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：399
# TP-GAN
  中科院自动化所（CASIA），中科院大学和南昌大学的一项合作研究，提出了双路径 GAN（TP-GAN），通过单一侧面照片合成正面人脸图像，取得了当前较好的结果。
  这里采用双通道网络，Two Pathway Generator，一个是 local pathway,另一个是 global pathway。
  1）local pathway 用于解决人脸的细节问题，输入侧脸的四个特征图像块：分别是 两个眼睛、鼻子、嘴巴。输出正脸的对应四个图像块。
  2）global pathway 用于生产人脸大的结构，缺少细节，输入完整的侧脸图像输出完整的模糊的正脸图像。
  3）这两个特征图会融合在一起，用于接下来的最终合成。
