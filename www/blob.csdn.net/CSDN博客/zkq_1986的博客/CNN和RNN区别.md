# CNN和RNN区别 - zkq_1986的博客 - CSDN博客





2017年09月07日 11:25:16[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：7968








CNN和RNN区别

CNN主要用于图像；RNN主要用于时序和NLP。

当CNN、RNN都用于NLP时，它们的区别在于：

RNN（循环神经网络），当前节点的输入包含之前所有节点信息。

RNN（递归神经网络），当前节点的输入以树结构形式包含部分之前节点信息。

CNN（卷积神经网络），当前节点的输入以树结构形式仅包含上一层节点信息。

![](https://img-blog.csdn.net/20170907110114318)


![](https://img-blog.csdn.net/20170907112245485)![](https://img-blog.csdn.net/20170907112250524)







