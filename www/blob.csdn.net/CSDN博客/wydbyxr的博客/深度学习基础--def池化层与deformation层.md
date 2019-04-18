# 深度学习基础--def池化层与deformation层 - wydbyxr的博客 - CSDN博客
2018年11月12日 10:05:12[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：43
个人分类：[深度学习基础](https://blog.csdn.net/wydbyxr/article/category/6829999)
# def池化层与deformation层的区别
  1）deformation层只有一个输出，def层在多个空间位置有多个输出，可学习共享的模式。
  2）def-pooling层的目标的局部大小不一，设计不同尺寸的分类器与conv5层卷积。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181112100459813.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
