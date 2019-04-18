# 广播操作(Broadcasting operation) - wydbyxr的博客 - CSDN博客
2018年12月26日 14:59:51[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：97
个人分类：[计算机基础和编程基础](https://blog.csdn.net/wydbyxr/article/category/7751906)
  一种用numpy-style broadcasting来保证tensor参数的形态兼容的操作。
  广播术语描述了在算术运算过程中numpy如何处理具有不同形状的数组。受到某些约束，较小的数组是跨越较大阵列的“广播”，以便它们具有兼容的形状。广播提供了一种向量化数组操作的方法，以便循环发生在C而不是Python中。它不会造成不必要的数据副本，通常会导致高效的算法实现。然而，广播是一个坏主意，因为它导致低效的内存使用减慢了计算的情况。
