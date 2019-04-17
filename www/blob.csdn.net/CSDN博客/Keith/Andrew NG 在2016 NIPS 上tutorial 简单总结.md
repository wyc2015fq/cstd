# Andrew NG 在2016 NIPS 上tutorial 简单总结 - Keith - CSDN博客





2017年04月08日 14:45:09[ke1th](https://me.csdn.net/u012436149)阅读数：905
个人分类：[MachineLearning](https://blog.csdn.net/u012436149/article/category/6504450)









# Andrew NG 在2016 NIPS 上tutorial 简单总结

水平有限，如有错误，请不吝指正，谢谢！ 
[视频地址youtube](https://www.youtube.com/watch?v=F1ka6a13S9I&t=456s)

## 神经网络已经出现很多年，为什么最近广泛应用起来
- 大数据
- 计算机能力的增强

## DL分类
- 普通DL（全连接）
- 1-D序列模型（RNN，LSTM，GPU）
- 图像模型，2-D，3-D， CNN
- 其它类别：无监督学习，增强学习。

## 趋势
- 模型规模不断增大
- 端到端学习 （Rich output） 
- 现在大多数机器学习算法只是输出一个值，但深度学习可以干更复杂的事，输出富文本（image->caption, audio->transcript, english->frence, parameters->image）


### 端到端学习

端到端学习并不能解决所有事情。 
**拿语音识别举例:**

传统方法： audio -> phonemes -> transcript 

端到端：audio -> transcript
如果想让端到端学习工作的话，需要**大量**的标签数据。(X,Y)

**另一个例子：通过手的X-ray图像判断小孩的年龄**

传统方法：x-ray image -> bones lengths -> age 

端到端： image -> age 

应用端到端方法，碰到的问题是，没有足够的训练数据去训练一个好的网络。`DL`用的更多的地方是传统方法的：x-ray image -> bones lengths 这个过程。
**?多少数据才够，数据不够的话，小心使用端到端学习**

## 碰到问题时，应该怎么解决。（bias/variance）

**Goal:** 人类水平的语音识别系统

定义`bias`和`variance`： 
`bias` : 训练集错误率 <–> 人类水平错误率 
`variance`: `dev-set`错误率 <–> `train-set`错误率|human error|train-set error|dev-set error|test-set error|问题|
|----|----|----|----|----|
|1%|5%|6%||high bias|
|1%|2%|6%||high variance|
|1%|5%|10%||high bias, high variable|

**基本workflow:**

Created with Raphaël 2.1.0StartTraining error is low?Dev error is low?DoneMore data, Regularization, New Model architecture.Bigger model,Train longer, new Model architecture.yesnoyesno

`Bigger Model` 和 `More Data` 总是会得到好结果。

**建议：unified data warehouse**

**问题：当前，用于训练的数据分布，和测试集数据分布很有可能属于不同分布**

假设50000h的语音数据（但不是来自测试分布的数据），和10h的语音数据（来自于测试分布）。如何组织你的`train, dev, test set`？ 

第一种方案： 将50000h分为`train, dev set`，10h 作为测试数据（bad idea） 

第二种方案：50000h作为`train set`，10h分为`dev，test set`。（`dev`和`test` set应该服从同一分布） 

第三种方案：50000h 分为 `train-set， train-dev-set`，10h分为 `dev-set，test-set` 。
重新定义`bias``variance`： 
`bias` : 训练集错误率 <–> 人类水平错误率 
`variance`: `train-dev-set`错误率 <–> `train-set`错误率 
`train-test mismatch`： `train-dev-set` <—> `dev-set`
`overfitting of dev`: `dev-set` <–> `test-set`|human-level|train-set|train-dev-set|dev-set|test-set|问题|
|----|----|----|----|----|----|
|1%|10%|10.1%|10.1%|10.2%|high bias|
|1%|2%|2.1%|10%||train-test mismatch|

**基本workflow：**

Created with Raphaël 2.1.0StartTraining error is low?Train-Dev error is low?Dev error is low?Test error is low?DoneGet more dev data.More data similar to test, Data Synthesis/Augmentation, New Architecture.More data, Regularization, New Model architecture.Bigger model,Train longer, new Model architecture.yesnoyesnoyesnoyesno

## 提高模型准确率

如何定义`human level error`：专家团错误率。
- 当模型准确率低于人类水平时 
- 从人类获得更多的数据
- 错误分析（看哪些样本分错了）
- 评估 `bias/variance` 影响。


## AL/DL 可以干什么？

1) 人类在一秒中可以干的事情，深度学习可以做（给数据打标签）。 

2) 预测序列时间的下一个输出。 

3) 人工智能应该干人类无法干的事（马云）
## 如何学习机器学习
- ML course
- DL schoole
- Phd student process 
- read papers
- replicate results

- dirty work 
- downloading data, clean data
- downloading piece of data, tuning the parameters and see what happend










