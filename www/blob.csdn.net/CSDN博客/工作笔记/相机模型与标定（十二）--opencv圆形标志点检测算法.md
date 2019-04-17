# 相机模型与标定（十二）--opencv圆形标志点检测算法 - 工作笔记 - CSDN博客





2016年07月27日 15:34:29[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：11571
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









本来以为圆形检测比较简单，实际还是花费我近一上午时间，网上几乎没有相关资料（除了OpenCV官网）。这里坐下简单介绍，分享给大家。

非对称圆形标定物检测：

1.findCirclesGrid函数的使用，如下：

```cpp
case ASYMMETRIC_CIRCLES_GRID:
				boardSize.width = 4;
				boardSize.height = 11;

				found = findCirclesGrid(view, boardSize, pointbuf, CALIB_CB_ASYMMETRIC_GRID | CALIB_CB_CLUSTERING, blobDetector);
```

标定图片：![](https://img-blog.csdn.net/20160727145820040)

需要注意的是：

1.boardSize 的宽度，需要设置为图片中固定个数的方向，如上图，水平方向，分别有5，6两种个数，垂直方向只有一种个数，因此，宽度设置为4，此后，以4为一行，则实际该图片在水平方向有11行，因此，高度设置为11.这里弄错，返回就是false了。

2.CALIB_CB_CLUSTERING 标志标识在检测到中心点后，会以层次Kmean方式聚类检测值，并计算检测点围成的凸包角点，并排序外部角点。同时，会根据排序后的2D外部角点和理想估计点，计算单应性H，再计算出所有监测点的投影点，再根据Knn选取跟理想估计点近似最近点，作为实际输出的圆形中点。

3.不设置CALIB_CB_CLUSTERING标志时，则根据CirclesGridFinder 类（几何特征），检测相关圆形，并排序输出。




对称圆形标志检测，流程与上面一致，只是不用关注长宽设置问题。




实际上，主要思路在于利用简单斑点检测器，检测出圆形后，做后续的相关过滤，排序，确认等操作。









