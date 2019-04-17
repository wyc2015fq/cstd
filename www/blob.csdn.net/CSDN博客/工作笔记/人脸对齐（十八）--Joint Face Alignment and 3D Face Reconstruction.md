# 人脸对齐（十八）--Joint Face Alignment and 3D Face Reconstruction - 工作笔记 - CSDN博客





2018年08月21日 14:24:13[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5599
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









### ****Joint Face Alignment and 3D Face Reconstruction(2016)****

 本文主要提出了一种新的方法来解决任意姿态和表情的2D人脸图片的特征点定位和3D人脸重构。该方法采用两个级联回归，一个回归用来更新2D特征点，另一个回归用来更新重构的姿态-表情3D人脸形状。3D与2D人脸形状之间是通过3D-2D的映射矩阵相关联起来的。在每一次迭代中，首先通过一个特征点回归器来更新人人脸特征点，特征点更新的同时也通过形状回归器对3D人脸形状进行了调整。3D-to-2D的映射矩阵是基于更新的3D人脸形状和2D人脸特征点计算得到，并且进一步提升2D特征点的定位精度。一个有效的算法是基于成对的标注的3D人脸形状和2D人脸图片训练得到。相比于其他算法，本文提出的算法能够从一幅2D人脸图片实时的产生3D人脸形状的同时对其进行可见点、不可见点的标定。

本文示意图如下所示：

![](https://img-blog.csdn.net/20180821140231640?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

本文基于2D人脸特征点和3D人脸形状之间很强的相关性，进一步尝试将二维人脸图像特征点检测（即人脸对齐）与三维人脸重建过程耦合起来，在回归的框架下同时实现这两个任务。

如上图所示，之前研究者大都将2D特征点定位和3D人脸重建两个过程割裂开来解决，而这两个工作本质是一个“鸡生蛋、蛋生鸡”问题。一方面，2D特征点 U 可由中性3D人脸 S 经过表情（FE ）、姿态变换（ FP）及投影（ FC）得到，即

![](https://img-blog.csdn.net/2018082114025834?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

另一方面，2D特征点携带有丰富的几何信息，这也是3D重建方法的基础。

现有的2D特征点检测方法大部分是基于2D人脸形状建模的，主要存在以下几个问题：i）很难去刻画3D平面外旋转的人脸特征点；ii）在人脸姿态不是很大的情况下，通过变化人脸轮廓特征点语义位置来解决自遮挡的情况，这样会导致不同姿态下检测的特征点语义信息不一致（如上图，人脸图像中蓝色点所示）；iii）在更大姿态下，尤其是yaw方向超过60度以后，人脸区域存在近一半自遮挡，遮挡区域的纹理特征信息完全缺失，导致特征点检测失败。

现有的利用2D特征点来恢复3D人脸形状的方法也存在以下几个问题：i）需要第三方2D特征点检测算法或者手动得到2D特征点；ii）不同姿态下检测的特征点语义信息不一致，难以确定3D点云中与其对应的点；iii）只生成与输入人脸图像同样姿态和表情的3D人脸，而这样的3D人脸，相对于姿态和表情归一化的3D人脸而言，显然并不有利于人脸识别。

![](https://img-blog.csdn.net/20180821140319568?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

为了在一个框架内处理2D特征点定位和3D人脸重建，本文利用两组级联的线性回归，一组用来更新2D特征点，另一组用来更新3D人脸形状。在每一次迭代中，先用SDM方法得到特征点更新量，基于方法[3]再用特征点的更新量去估计出3D人脸形状的更新量。新的3D人脸一旦更新就可以粗略地计算出3D-to-2D投影矩阵，同时再利用3D人脸对特征点进行修正，尤其是自遮挡区域的特征点位置及特征点可见性信息。整个过程2D特征点、3D人脸形状、3D-to-2D投影矩阵的更新都是一个由粗到精的估算过程。

先给出利用训练好的回归模型检测任意一张二维人脸图像上的特征点，并重建其三维模型的过程。

![](https://img-blog.csdn.net/2018082114034085?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

值得指出的是：Step 5中，从3D人脸投影得到2D特征点对人脸形状和姿态都有很强的约束。而Step 2中，特征点是通过纹理特征指导得到的，其中自遮挡区域由于纹理信息的缺失，回归得到的特征点常常是不准确的。通过此步骤3D投影来修正能够有效地提高特征点检测的准确度。

在训练过程中，为了得到上述回归模型，需要提供成对的标定好特征点的二维人脸图像及其对应的三维人脸数据

![](https://img-blog.csdn.net/20180821140400290?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

为了更好地处理任意姿态、任意表情的二维人脸图像，训练数据中需要包括尽量多不同姿态和不同表情的人脸，而对应的三维人脸则都是中性表情的、且已经稠密对齐的点云数据。下面我们重点介绍一下用于人脸对齐的2D特征点回归的目标函数和用于三维人脸重建的3D形状回归的目标函数。

![](https://img-blog.csdn.net/20180821140418618?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

该目标函数建立当前2D特征点周围的纹理特征与其距离真实位置的偏移量之间的回归关系。我们训练所用2D特征点是从3D形状投影得到的，因而确保了语义上的一致性。同时为了处理大姿态人脸图像，如果某个特征点被判定为不可见点，那这个点的SIFT特征向量置为0。

![](https://img-blog.csdn.net/20180821140435600?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

3D形状回归建立的是2D特征点修正量与3D形状修正量之间的关系。所有训练3D人脸都进行了稠密对齐，且2D特征点之间也作好了对齐，所以并不需要增加额外的平滑约束，同时也尽量保持了3D人脸的个性化差异。训练数据中的3D形状是姿态-表情归一化（Pose and Expression Normalized，简称PEN）3D人脸，如此重建得到的PEN 3D人脸更适用于人脸识别。

在公开测试集上的实验结果证明了在统一的回归框架下同时解决人脸对齐和三维重建的有效性。ECCV2016论文中还进一步证明了重构出来的姿态与表情归一化的三维人脸在提升人脸识别准确率方面的有效性。最后，我们展示利用ECCV2016方法得到的人脸对齐和三维重建的几个典型结果.

![](https://img-blog.csdn.net/20180821140454596?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



