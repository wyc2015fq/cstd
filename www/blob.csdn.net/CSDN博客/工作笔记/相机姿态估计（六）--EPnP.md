# 相机姿态估计（六）--EPnP - 工作笔记 - CSDN博客





2018年08月29日 14:17:47[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：6307
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









EnP算法，论文：EPnP: Efficient Perspective-n-Point Camera Pose Estimation

直接给过程：

相机坐标系用Fc，世界坐标系用Fw表示，任何一点可以用四个控制点p表示：

![](https://img-blog.csdn.net/20180829152946444?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

对于相机坐标系同样成立：

![](https://img-blog.csdn.net/2018082915300688?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

对于上面的公式，首先需要说明的是α确实存在。因为cw或cc构成的方程组是欠定的，所以一定存在解。

理论上来说，控制点可以随便选择，这里选择控制点为参考点的中心，其他的点在PCA得到的主轴上单位长度处，从而提高算法的稳定性。

如何求解？如下：

根据投影方程得到世界坐标系中参考点坐标和相机坐标系中参考点的约束关系：

![](https://img-blog.csdn.net/20180829153240868?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

写成矩阵的形式为：

![](https://img-blog.csdn.net/20180829154747791?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

A就是相机内参矩阵。

将等式的第三列代入第一二列，得到：

![](https://img-blog.csdn.net/2018082915341767?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/2018082915344431?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 因此，可以得到下面的线性方程组：

![](https://img-blog.csdn.net/20180829153509950?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

上面的方程中，四个控制点总共12个未知变量，M为2n×12的矩阵。因此，x属于M的右零空间，vi为矩阵M的右奇异向量，可以通过求解MTM 的零空间特征值得到。

![](https://img-blog.csdn.net/20180829153641363?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 [说明]使用MTM比使用M计算量更少，因为MTM是求解是常数复杂度，而M是O(n3)的复杂度，但是计算MTM的复杂度是O(n)的。

上面求解的x中，需要确定βi，也就是确定合适的线性组合。根据参考点的位置不同，矩阵MTM的零空间维数可能为N=1→4维。求解β的策略是控制点在坐标系Fw和Fc中，两两之间的距离是相同，而x的3k+1−3k分量表示分别表示不同的控制点在相机坐标系中的坐标，总共有C24=6个约束。

如果N=1，则根据约束有：

![](https://img-blog.csdn.net/20180829153838849?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

所以：

![](https://img-blog.csdn.net/20180829153902129?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

若N=2，则：

![](https://img-blog.csdn.net/20180829153932228?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

由于β1和β2只以二次项出现在方程中，记β=[β21,β1β2,β22]T, ρ的每一项为∥cwi−cwj∥2，得到相应的方程：L![\beta =\rho](https://private.codecogs.com/gif.latex?%5Cbeta%20%3D%5Crho)

其中L是由v1和v2构成的6×3的矩阵。

上面的方程可以通过β=(LTL)−1LTρ得到，然后通过选择合适的符号从β21,β1β2,β22使得所有的pci有正的z坐标。

如果N=3则和N=2差不多，唯一的区别在于使用的是L的逆，而不是伪逆，此时的L为6×6的矩阵。

共面的情况：

当点共面时，M减少为2n*9，有9个特征向量，上面的结论仍然有效，唯一不同在于约束又6变成了3.

### G-N优化

前面的步骤可以得到目标点在相机坐标系中的闭式解，作为G-N优化的初始值，优化的变量为β=[β1,⋯,βN]T，目标函数为:

![](https://img-blog.csdn.net/20180829154303864?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其中CW的两点距离已知，控制点在相机中的值，用下式计算：



![](https://img-blog.csdn.net/20180829155436460?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

该优化过程和参考点的数目无关，优化步骤和时间是常数。

### 计算R，t

前面的两步计算不同维数的零空间的误差，选择误差最小维数对应的β，从而得到x，恢复出控制点在相机坐标系中的坐标并根据质心坐标系数得到参考点在相机坐标系的坐标。剩下的工作就是已知一组点云在两个坐标系中的坐标，求两个坐标系的位姿变换。

步骤如下：

（1）求中心点，

![](https://img-blog.csdn.net/20180829155906314?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



（2）去中心

![](https://img-blog.csdn.net/20180829155926637?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



（3）计算H矩阵

![](https://img-blog.csdn.net/20180829155946317?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



（4）对H进行SVD

![](https://img-blog.csdn.net/20180829160005814?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



（5）计算X=VUT，如果det(x)=1，则R=X，t=Pcc−RPcw。否则R(2,⋅)=−R(2,⋅)



