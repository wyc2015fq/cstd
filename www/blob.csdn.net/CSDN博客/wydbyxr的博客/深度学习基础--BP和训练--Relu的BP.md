# 深度学习基础--BP和训练--Relu的BP - wydbyxr的博客 - CSDN博客
2018年11月13日 10:10:49[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：156
# Relu的BP
  正向计算的情况下，对于输入大于零保持同样的输出，输入小于零直接置0；ReLu层的输入x，输出是z，反向传播时已知 ，那么有：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181113101038797.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
