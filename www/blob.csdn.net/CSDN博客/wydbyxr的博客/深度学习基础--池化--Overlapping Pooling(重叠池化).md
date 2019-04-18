# 深度学习基础--池化--Overlapping Pooling(重叠池化) - wydbyxr的博客 - CSDN博客
2018年11月15日 10:03:42[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：855
# Overlapping Pooling(重叠池化)
  alexnet中提出和使用。
  相对于传统的no-overlapping pooling，采用Overlapping Pooling不仅可以提升预测精度，同时一定程度上可以减缓过拟合。
  相比于正常池化（步长s=2，窗口z=2） 重叠池化(步长s=2，窗口z=3) 可以减少top-1, top-5分别为0.4% 和0.3%；重叠池化可以避免过拟合。
