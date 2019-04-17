# 人体运动识别：motionlet: Mid-level 3D level parts for human motion recognition - 家家的专栏 - CSDN博客





2014年03月11日 14:02:49[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：1472
个人分类：[乐在科研中](https://blog.csdn.net/yihaizhiyan/article/category/742190)









**应用：human motion recognition （人体运动识别）**

**Motionlet**:  a mid-level and spatio-temporal part...

                    a tight cluster in motion and appearance space...(对应着不同身体部分的运动过程)

                   运动显著性、多尺度表示、表示辨别能力。

首先从视频训练集中，学习一个motionlets。。

具体来书，有三个步骤：                 

                 利用高运动显著性，提取3D区域。

                 区域聚类，并保留中心作为motionlet的候选集。

                 从候选集中，进一步选择一部分有辨别力和识别力的区域。










