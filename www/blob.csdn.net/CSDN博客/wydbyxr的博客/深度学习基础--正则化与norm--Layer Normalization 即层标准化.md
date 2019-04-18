# 深度学习基础--正则化与norm--Layer Normalization	即层标准化 - wydbyxr的博客 - CSDN博客
2018年11月20日 10:29:33[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：388
# Layer Normalization	即层标准化
  Layer Normalization和普通的标准化类似，是将网络中的层进行归一化的操作。
  一般用于RNN。
## 原因
  对于RNN模型来说，归一化的时候Batch Normalization不再适合，在RNN模型里，sequence的长度是不一致的，正是由于这种不一致的问题，导致BN使用起来效果不佳，借用网上的一句话，RNN的深度不是固定的，不同的time-step需要保存不同的statics特征，可能存在一个特殊的sequence比其sequence长，所以对sequence进行Batch Normalization计算起来比较麻烦。于是乎，Layer Normalization登场了。
## 优点
  跟BN类似，也具有Normalization的优点，但更适用于RNN模型。
## 计算和使用
  需要明确的是LN是对层进行归一化的，但是也是对某一层所有的dataset进行归一化的。
**LN中同层输入拥有相同的均值和方差（即整个一层都共享同一个均值和方差），不同输入(训练样本)拥有不同的均值和方差；** 而BN是同一个minibatch中的输入拥有相同的均值和方差，而不同minibatch的输入拥有不同的均值和方差。
  1）一般的BN通式：	
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120102802848.png)
  2）LN的公式：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120102919763.png)
