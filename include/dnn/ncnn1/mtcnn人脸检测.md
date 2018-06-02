# MTCNN人脸检测算法

## 参考论文
 - 2016 Joint Face Detection and Alignment using Multi-task Cascaded Convolutional Networks


## 算法流程
stage1: 在构建图像金字塔的基础上，利用fully convolutional network来进行检测，同时利用boundingbox regression 和 NMS来进行修正。（注意：这里的全卷积网络与R-CNN里面带反卷积的网络是不一样的，这里只是指只有卷积层，可以接受任意尺寸的输入，靠网络stride来自动完成滑窗）

stage2: 将通过stage1的所有窗口输入作进一步判断，同时也要做boundingbox regression 和 NMS。

stage3: 和stage2相似，只不过增加了更强的约束：5个人脸关键点。

## 源码框架
基于优化过的NCNN框架