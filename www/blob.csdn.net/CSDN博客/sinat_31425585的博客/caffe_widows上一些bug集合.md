# caffe_widows上一些bug集合 - sinat_31425585的博客 - CSDN博客
2017年11月15日 21:18:48[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：107
windows上使用caffe过程中，一些bug集合：
错误1：
F1114 10:00:29.401401  2280 common.cpp:160] Check failed: error ==cudaSuccess (
35 vs. 0) CUDA driver version is insufficient for CUDA runtime version
*** Check failure stack trace: ***
原因：显卡内存不足
解决方法：修改.prototxt文件中的batch_size，将其调小一点
参考博客：[点击打开链接](http://blog.csdn.net/forest_world/article/details/70210348)
