# 相机姿态估计（七）--UPnP - 工作笔记 - CSDN博客





2018年08月30日 10:05:29[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5478
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









论文：Exhaustive Linearization for Robust Camera Pose and Focal Length Estimation

整体来看，UPnP跟EPnP差不多，只是同时估计了焦距，因此，适合未标定场合，Uncalibrated PnP.

问题定义:

![](https://img-blog.csdn.net/20180830092747220?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

p世界坐标系点参考点，C控制点，R，t姿态矩阵，u是p对应的2D投射点。建模为最小化P点的投影误差。

![](https://img-blog.csdn.net/20180830093044366?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其中u表示为：

![](https://img-blog.csdn.net/20180830093105940?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

k是尺度因子。 

 跟EPnP类似，增加控制点约束：

![](https://img-blog.csdn.net/20180830093248877?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

注意，这里如上图所示，c是在世界坐标系下。同样，相机坐标系下，c的上标变为c。

3式带入2式，展开得：

![](https://img-blog.csdn.net/20180830093500650?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

到这里，跟EPnP的差别在于焦距， 本质 是一样的，也得到线性方程组：

![](https://img-blog.csdn.net/20180830093655790?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

M也是2n*12的矩阵，得到x:

![](https://img-blog.csdn.net/20180830093737756?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

上面的方程中，四个控制点总共12个未知变量，M为2n×12的矩阵。因此，x属于M的右零空间，vi为矩阵M的右奇异向量，可以通过求解MTM 的零空间特征值得到。

![](https://img-blog.csdn.net/20180830094123105?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

[说明]使用MTM比使用M计算量更少，因为MTM是求解是常数复杂度，而M是O(n3)的复杂度，但是计算MTM的复杂度是O(n)的。

上面求解的x中，需要确定βk，也就是确定合适的线性组合。根据参考点的位置不同，矩阵MTM的零空间维数可能为N=1→4维。求解β的策略是控制点在坐标系Fw和Fc中，两两之间的距离是相同，而x的3k+1−3k分量表示分别表示不同的控制点在相机坐标系中的坐标，总共有C24=6个约束。

同样，也是分类讨论。

N=1，线性化，此时只要求解β1和f，假定β11=β1^2，βff11=f^2β^2,根据控制点距离约束：

![](https://img-blog.csdn.net/20180830094653285?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180830094706742?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

可以得到6个如下形式的方程：

![](https://img-blog.csdn.net/20180830094856244?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其中b=[β11，βff11]T .



计算得到：

![](https://img-blog.csdn.net/20180830094743395?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

当N=2时，穷举线性化，我们需要求β1，β2，f. 则类似N=1的情况，此时b变为：

![](https://img-blog.csdn.net/20180830095309889?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

与N=1类似，只是在相同的相机内外参数约束下，不断穷举，例如：

![](https://img-blog.csdn.net/20180830095627921?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 当N=3时：穷举再线性化。

再线性化技术如下：

以定义线性系统开始：

![](https://img-blog.csdn.net/20180830095853331?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其中， 

![](https://img-blog.csdn.net/20180830095916859?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

具体算法伪代码如下：

![](https://img-blog.csdn.net/20180830100151845?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

后面，为了提高精度，增加迭代优化：

![](https://img-blog.csdn.net/20180830100329796?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



完整的对比结果：

不共面：

![](https://img-blog.csdn.net/20180830100506505?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180830100517341?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

上图可以看出，在点数超过20时，其实UPnP与EPnP没有太大差距，但是，UPnP更适合大焦距图像的姿态估计。当f>4000时，差距就出现了，但一般常见的f在500~2000之间，也就是近似估计为480i~1080p范围。在这种情况下，EPnP与UPnP性能差不多。 

共面：

![](https://img-blog.csdn.net/20180830100711578?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

共面情况下，结论也类似，2者差距不大。

顺便提一下时间：

作者在实测一个任务时，得到的结果如下：

![](https://img-blog.csdn.net/20180830101027921?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

另外：

![](https://img-blog.csdn.net/20180830101108433?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其中颜色标注对应上面不共面情况下的算法类别。 

可以看到，UPnP在时间上跟EPnP比，是没有优势的。 



