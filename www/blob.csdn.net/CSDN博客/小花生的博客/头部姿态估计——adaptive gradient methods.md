
# 头部姿态估计——adaptive gradient methods - 小花生的博客 - CSDN博客


2018年10月06日 10:08:37[Peanut_范](https://me.csdn.net/u013841196)阅读数：524


《Head pose estimation in the wild using Convolutional Neural Networks and adaptive gradient methods》
2017，Massimiliano Patacchiola, Angelo Cangelosi. adaptive gradient methods
本文使用4种最新的网络结构来训练头部姿态估计，作者也探讨dropout和adaptive gradient methods对结果性能得影响。
网络结构：
![在这里插入图片描述](https://img-blog.csdn.net/20181006100238977?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
四种网络结构得细节：
![在这里插入图片描述](https://img-blog.csdn.net/20181006100307656?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
对于Dropout：
The probability p is another hyperparameter to tune. However numerous experimental results suggest that a value of p = 0.5 produces the best performance , so we used this value in our experiments.
实验结果：
![在这里插入图片描述](https://img-blog.csdn.net/20181006100431375?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![在这里插入图片描述](https://img-blog.csdn.net/20181006100440384?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
可以发现：Adam和RMSProp优化器效果最好。
[
](https://img-blog.csdn.net/20181006100440384?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)训练阶段比较：
![在这里插入图片描述](https://img-blog.csdn.net/20181006100547316?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
收敛速度比较：
![在这里插入图片描述](https://img-blog.csdn.net/20181006100621577?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
小结：
可以发现，在进行CNNs训练过程中，与更传统的优化器相比，Adam和RMSProp优化器效果和收敛速度都比较好，建议使用这两种优化器。
---
注：博众家之所长，集群英之荟萃。

