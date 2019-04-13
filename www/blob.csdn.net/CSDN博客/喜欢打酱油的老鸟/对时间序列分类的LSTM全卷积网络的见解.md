
# 对时间序列分类的LSTM全卷积网络的见解 - 喜欢打酱油的老鸟 - CSDN博客


2019年04月02日 08:39:26[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：12


[https://www.toutiao.com/a6674883188617118220/](https://www.toutiao.com/a6674883188617118220/)
对时间序列分类的LSTM全卷积网络的见解
**题目：**
Insights into LSTM Fully Convolutional Networks for Time Series Classification
**作者：**
Fazle Karim, Somshubra Majumdar, Houshang Darabi
**来源：**
Accepted at IJCNN 2019
Machine Learning (cs.LG)
Submitted on 27 Feb 2019
**文档链接：**
arXiv:1902.10756
**代码链接：**
https://github.com/titu1994/LSTM-FCN
**摘要**
长期、短期记忆全卷积神经网络(LSTM-FCN)和注意力LSTM-FCN (ALSTM-FCN)在旧的加州大学河滨分校(UCR)时间序列存储库中对时间序列信号进行分类的任务中表现出了最先进的性能。然而，对于LSTM-FCN和ALSTM-FCN为何表现良好，目前还没有研究。在本文中，我们对LSTM-FCN和ALSTM-FCN进行了一系列烧蚀试验(3627个实验)，以便更好地理解模型及其各个子模块。对ALSTM-FCN和LSTM-FCN的烧蚀试验结果表明，联合使用时，这两种材料的烧蚀效果较好。使用Wilcoxson符号秩检验比较了两种z归一化技术，即单独对每个样本进行z归一化和对整个数据集进行z归一化，以显示性能上的统计差异。此外，我们还通过比较维度洗牌与LSTM-FCN在不应用维度洗牌时的性能，了解维度洗牌对LSTM-FCN的影响。最后，我们证明了LSTM- fcn的性能，当LSTM块被一个GRU、基本RNN和密集块替换时。
**要点**
![对时间序列分类的LSTM全卷积网络的见解](http://p1.pstatp.com/large/pgc-image/5e32cfc77260475c96518532ba169934)
表一:LSTM-FCN与
带有基线模型的ALSTM-FCN。绿色单元格指定我们的性能匹配或超过最先进结果的实例。粗体值表示性能最佳的模型。
![对时间序列分类的LSTM全卷积网络的见解](http://p1.pstatp.com/large/pgc-image/1bc23c2d26d54f8b8a3a55edc164e396)
图1烧蚀试验-通过从每个卷积层随机选择的滤波器进行转换后输入信号的可视化表示
![对时间序列分类的LSTM全卷积网络的见解](http://p1.pstatp.com/large/pgc-image/1f2cfade3c56458f81e8675bece8abc6)
表二:烧蚀试验- LSTM/ALSTM块、FCN块与Raw的线性SVM性能比较
信号。绿色单元格和橙色单元格指定块上的线性SVM模型在原始信号上超过线性SVM的实例。粗体表示使用线性SVM分类器性能最好的块。Count∗表示该列中粗体值的数量。
![对时间序列分类的LSTM全卷积网络的见解](http://p3.pstatp.com/large/pgc-image/95ba9266cd35472bbbb9aaad78a68a9d)
表三:烧蚀试验- LSTM/ALSTM块、FCN块、LSTM/ALSTM-FCN的MLP性能比较
块和原始信号。绿色单元格和橙色单元格指定块上的MLP模型超过原始信号上的MLP的实例。粗体值表示使用MLP分类器性能最好的块。Count∗表示该列中粗体值的数量。
**英文原文**
Long Short Term Memory Fully Convolutional Neural Networks (LSTM-FCN) and Attention LSTM-FCN (ALSTM-FCN) have shown to achieve state-of-the-art performance on the task of classifying time series signals on the old University of California-Riverside (UCR) time series repository. However, there has been no study on why LSTM-FCN and ALSTM-FCN perform well. In this paper, we perform a series of ablation tests (3627 experiments) on LSTM-FCN and ALSTM-FCN to provide a better understanding of the model and each of its sub-module. Results from the ablation tests on ALSTM-FCN and LSTM-FCN show that the these blocks perform better when applied in a conjoined manner. Two z-normalizing techniques, z-normalizing each sample independently and z-normalizing the whole dataset, are compared using a Wilcoxson signed-rank test to show a statistical difference in performance. In addition, we provide an understanding of the impact dimension shuffle has on LSTM-FCN by comparing its performance with LSTM-FCN when no dimension shuffle is applied. Finally, we demonstrate the performance of the LSTM-FCN when the LSTM block is replaced by a GRU, basic RNN, and Dense Block.

