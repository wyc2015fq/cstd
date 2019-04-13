
# 谷歌发布TensorFlow Privacy​：大幅提升AI模型中的隐私保护 - 喜欢打酱油的老鸟 - CSDN博客


2019年03月07日 12:53:05[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：196


[https://www.cnbeta.com/articles/tech/824909.htm](https://www.cnbeta.com/articles/tech/824909.htm)
基于机器学习框架TensorFlow的谷歌最新模块，可以让开发者只添加几行额外代码就能改善AI模型中的隐私。TensorFlow是目前用于构建机器学习应用程序最流行的工具之一，它被世界各地的开发人员用于创建文本、音频和图像识别算法等程序。而伴随着[TensorFlow Privacy](https://medium.com/tensorflow/introducing-tensorflow-privacy-learning-with-differential-privacy-for-training-data-b143c5e801b6)的引入，这些开发人员能够使用“差异隐私”的统计数据来保护用户的数据。
![](https://static.cnbetacdn.com/thumb/article/2019/0307/c60e8f698dc7a98.jpg)
谷歌产品经理Carey Radebaugh向外媒The Verge透露，发布这款工具是谷歌履行对人工智能的承诺和愿景。他说道：“如果我们没有为TensorFlow引入差异化隐私技术，那么无论是团队内部还是Google外部用户在使用过程中就会发现有些不太简单。因此对于我们来说将其引入TensorFlow是非常重要的，我们还将会对其进行开源，并开始围绕着它创建新的社区。”
差异隐私的机制有点复杂，但它本质上是一种数学方法，这意味着用于培训AI模型的用户数据并不能编码个人可识别信息。这是在AI模型中保护个人信息的常用方法：苹果在iOS 10上引入了自家的AI服务的，而Google在Gmail的Smart Reply等部分AI功能中也使用到了这项技术。
![](https://static.cnbetacdn.com/thumb/article/2019/0307/b2636ec3792c223.png)
已经在的数据隐私领域工作了20年的谷歌研究科学家ÚlfarErlingsson表示：差异化隐私技术以“数学确定性”消除了编码个人数据的可能性，他表示这是一种从数据集中删除可识别的异常值而不改变数据的聚合含义的技术。
谷歌的研究科学家ÚlfarErlingsson说，他已经在数据隐私领域工作了20年。 Erlingsson告诉The Verge，这是一种从数据集中删除可识别的异常值而不改变数据的聚合含义的技术， “你的结果独立于任何一个人的[数据]，但这仍然是一个很好的结果。”

