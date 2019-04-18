# bounding box overlap（IoU） - wydbyxr的博客 - CSDN博客
2018年09月28日 17:52:06[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：157
所属专栏：[经典机器学习算法](https://blog.csdn.net/column/details/28812.html)
# bounding box overlap（IoU）
  即Intersection over Union，交并比。IoU这一值，可以理解为系统预测出来的框与原来图片中标记的框的重合程度。 IoU的值定义：Region Proposal与Ground Truth的窗口的交集比并集的比值，如果IoU低于0.5，那么相当于目标还是没有检测到。
　　而2014年以来出现的MS COCO竞赛规则把这个IoU变成了0.5-1.0之间的综合评价值，也就是说，定位越准确，其得分越高。复杂的说，检测结果(DetectionResult)与 Ground Truth 的交集比上它们的并集，即为检测的准确率 IoU 。
　　矩形框和原来矩形框相交面积比（IOU）。如果IOU大于某个阈值（如，我使用了0.64），那么就可以认为经过“采样”得到了一个新的正样本。
## 计算
  1）是计算两个bounding box的交。计算x方向与y方向的重叠，然后相乘得到的就是交的值。
　　2）是计算两个bounding box的并。
　　3）是两个bounding box的交除于并。
## 图示
![在这里插入图片描述](https://img-blog.csdn.net/20180929110558922?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
蓝色的框是：GroundTruth
黄色的框是：DetectionResult
绿色的框是：DetectionResult ⋂ GroundTruth
红色的框是：DetectionResult ⋃ GroundTruth
