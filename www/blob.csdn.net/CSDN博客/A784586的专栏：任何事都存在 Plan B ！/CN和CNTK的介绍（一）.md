# CN和CNTK的介绍（一） - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年04月06日 23:41:23[QuJack](https://me.csdn.net/A784586)阅读数：1334








> 
> 
**CN和CNTK的介绍（一）**



From：《An Introduction to Computational Networks and the Computational Network Toolkit》CNTKBook

**           摘要中文翻译             备注：第一次尝试翻译，不当错误请见谅。**

我们介绍一个计算网络 computational network (CN)，它是一个统一的描述任意机器学习的框架，例如深度神经网络 deep neural networks (DNNs)，卷积神经网络 convolutional neural networks (CNNs),
 递归神经网络recurrent neural networks (RNNs),长短时记忆 long short term memory(LSTM),   逻辑回归 logistic  regression, 和最大熵模型maximum entropy model。这些能够被一系列的步骤展示说明。一个CN是一个有向图，有向图中，每个叶子节点表示输入的数值或者一个参数，每个非叶子节点表示一个依赖于它的孩子的矩阵操作。在CN中，我们描述算法执行前向计算和梯度计算，在典型的CN
 中，介绍最流行的计算节点类型。

进一步我们介绍CNTK，一种支持GPU和CPU的CN的实现。我们描述架构和它的关键组成部分，如何使用命令行使用CNTK，网络定义，模型编辑语言，为声学模型，语言模型，语音理解提供样例步骤。我们也提供语音识别解码用CNTK集成的例子。



