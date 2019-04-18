# Fast-rcnn总结 - qq229873466的博客 - CSDN博客

2017年05月23日 21:43:53[qq229873466](https://me.csdn.net/qq229873466)阅读数：274


# Fast-rcnn算法

## 训练

ILSVRC 2012数据库，训练网络A

VOC图片，通过SS 获得 RP（和真值区域的IoU>=0.5为正类，剩下为负类）

VOC整幅图片、ROI区域（RP）、类标签、真值框，输入以网络A权值fine-tuning训练网络B（conv-pooling-ROI pooling；SVD加速；多任务loss）


## 问题

为什么SPPnet不能更新SPP层之前的权值？

当训练的样本（RoI）来自于不同的图片时，反向传播效率很低。低效的原因是每个RoI可能会有很大的感受野，甚至整幅图片，所以在前向传播时需要处理很大的感受野。

