# 声学模型学习笔记（七） advanced deep models - xmdxcsj的专栏 - CSDN博客





2016年10月08日 20:09:03[xmucas](https://me.csdn.net/xmdxcsj)阅读数：1416








## multitask and transfer learning

multitask learning：不同的任务网络，可以共享一部分网络结构（比如说某个隐层） 

transfer learning：迁移学习

### SHL-MDNN

![这里写图片描述](https://img-blog.csdn.net/20161008200838495)

shared-hidden-layer multilingual DNN，用于训练不同语言的模型，所有的模型共享同一个隐层，输出层跟语言有关。 

共用的隐层可以认为是一个特征提取器，最后的输出层为分类器。 

SHL-MDNN需要多个语言同时进行训练，一个mini-batch包括多个语言的训练语料。 

试验表明多语言训练出来的SHL-MDNN相比单语言训练出来的DNN性能有提升，共享的隐层训练在一定程度上面减弱了overfitting问题。 

隐层作为特征提取器，可以把它对音素的区分性迁移到其他语言上面。 

- 如果需要增加一种语言，只需要增加一个输出层，还是复用前面训练好的隐层，训练是固定隐层，只需要训练最后一层的参数即可 

- 如果新增语言的训练数据比较充足，整体重新训练效果更好。 

- 可以由英语迁移到中文，依然有效
## RNN

[RNN](http://blog.csdn.net/xmdxcsj/article/category/5855803)
[LSTM](http://blog.csdn.net/xmdxcsj/article/details/52526843)

## 参考

《automatic speech recognition a deep learning approach》 chapter 12-15




