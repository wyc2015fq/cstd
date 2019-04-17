# 模式识别经典算法——LDA - 家家的专栏 - CSDN博客





2012年05月18日 16:27:12[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：47139








线性判别式分析，又称为Fisher线性判别~（Linear discriminant analysis）(Fisher linear discriminant)

**最大化类间均值，最小化类内方差**

通过调整权重向量组件，可选择一个投影方向，最大化地类别分离性~

![](https://img-my.csdn.net/uploads/201205/18/1337326287_3114.png)

两个类的均值向量：

![](https://img-my.csdn.net/uploads/201205/18/1337325486_1812.png)

对样本进行投影时，使得类别最简单的分离，是投影的类别均值的分离~

**最大化类间均值**

![](https://img-my.csdn.net/uploads/201205/18/1337325642_2956.png)   其中约束![](https://img-my.csdn.net/uploads/201205/18/1337325756_2735.png)





**最小化类内方差**

![](https://img-my.csdn.net/uploads/201205/18/1337326852_8450.png)，其中![](https://img-my.csdn.net/uploads/201205/18/1337326908_9311.png)



Fisher 判别准则：

![](https://img-my.csdn.net/uploads/201205/18/1337326989_3888.png)

等价于：![](https://img-my.csdn.net/uploads/201205/18/1337328217_9381.png)

其中：![](https://img-my.csdn.net/uploads/201205/18/1337328268_2223.png)![](https://img-my.csdn.net/uploads/201205/18/1337328277_7051.png)

对权重W求微分，使得J(W)最大化，当：![](https://img-my.csdn.net/uploads/201205/18/1337328416_3326.png)

化简之~

![](https://img-my.csdn.net/uploads/201205/18/1337329795_4490.png)



参考文献：Pattern recognition and machine learning (Christopher M. Bishop) pp: 186-189

此博文中，也有介绍~ 讲的非常好~  [http://blog.csdn.net/warmyellow/article/details/5454943](http://blog.csdn.net/warmyellow/article/details/5454943)



