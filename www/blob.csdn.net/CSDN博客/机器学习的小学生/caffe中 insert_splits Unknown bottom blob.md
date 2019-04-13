
# caffe中 insert_splits Unknown bottom blob - 机器学习的小学生 - CSDN博客


2017年06月17日 14:54:20[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：2113


问题：
F0617 14:52:44.533993  2948 insert_splits.cpp:29] Unknown bottom blob ‘trainFeature’ (layer ‘Conv1’, bottom index 0)
原因：
训练协议中，训练数据层的：top 和测试数据层的: top，的名字不一致导致的，例如，如果第一层操作是卷积层，卷积层的bottom名字，和训练数据层和测试数据层top名字一致。

