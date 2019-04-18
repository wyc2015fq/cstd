# 深度学习基础--卷积--group conv - wydbyxr的博客 - CSDN博客
2018年11月15日 09:55:02[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：177
# group conv
  在Xception、ResNext、Mobilenet、shufflenet中都有。
## 作用
  group convolution还是很有用的，比如训练人脸的五官，五个是不同的东西，特征不同，分5个group就可以分开学，相当方便。
  实现locally share的convolution layer的时候能用上。
