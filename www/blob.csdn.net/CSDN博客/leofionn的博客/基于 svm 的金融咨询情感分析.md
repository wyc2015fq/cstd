
# 基于 svm 的金融咨询情感分析 - leofionn的博客 - CSDN博客


2018年07月31日 18:30:55[leofionn](https://me.csdn.net/qq_36142114)阅读数：122


详细代码与代码说明可见我的 github：[https://github.com/ouprince/svm](https://github.com/ouprince/svm)
处理流程：
**（1）金融咨询处理**
1.利用 7 万多条利好/利空语料（已经标注好的，分为 1 正性，-1 负性两类），首先采用 B-gram 卡方差提取特征词汇
2.使用卡方提取的特征词为每一篇咨询建立向量表示模型
3.使用向量进行 svm 分割，训练语料 80%，测试语料 20%，并评估模型准确率，保存模型。
4.加载保存的模型进行 利好/利空 预测。
模型效果：正面咨询识别率 0.96 ，负面咨询识别率 0.82，总体识别率 0.91 左右。
**（2）app 评论处理**
流程同上，只不过训练的语料不同并且采用 ngram = 1，由于语料是我自己标注的 502 条评论数据，因此模型的准确率达不到金融咨询的基于 7万 多条语料处理的模型。对负面评论的识别率大概在 0.82 左右，总体识别率在 0.86 左右。
![](https://img-blog.csdn.net/20180511160345376?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzMyMDIzNTQx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

