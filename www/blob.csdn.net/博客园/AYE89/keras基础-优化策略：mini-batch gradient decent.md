
# keras基础-优化策略：mini-batch gradient decent - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[keras基础-优化策略：mini-batch gradient decent](https://www.cnblogs.com/eniac1946/p/7423865.html)
|Posted on|2017-08-24 16:43|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7423865)|[收藏](#)
|参考《Keras中文文档》|[http://keras-cn.readthedocs.io/en/latest/](http://keras-cn.readthedocs.io/en/latest/)
|相关概念：神经网络优化器（优化策略）、梯度下降、随机梯度下降、小批的梯度下降（mini-batch gradient decent）、batch_size
|batch

|这个概念与Keras无关，老实讲不应该出现在这里的，但是因为它频繁出现，而且不了解这个技术的话看函数说明会很头痛，这里还是简单说一下。
|深度学习的优化算法，说白了就是梯度下降。每次的参数更新有两种方式。
|第一种，|标准梯度下降|。遍历全部数据集算一次损失函数，然后算函数对各个参数的梯度，更新梯度。这种方法每更新一次参数都要把数据集里的所有样本都看一遍，计算量开销大，计算速度慢，不支持在线学习，这称为Batch gradient descent，批梯度下降。
|另一种，每看一个数据就算一下损失函数，然后求梯度更新参数，这个称为|随机梯度下降|，stochastic gradient descent。这个方法速度比较快，但是收敛性能不太好，可能在最优点附近晃来晃去，hit不到最优点。两次参数的更新也有可能互相抵消掉，造成目标函数震荡的比较剧烈。
|为了克服两种方法的缺点，现在一般采用的是一种|折中手段|，mini-batch gradient decent，小批的梯度下降，这种方法把数据分为若干个批，按批来更新参数，这样，一个批中的一组数据共同决定了本次梯度的方向，下降起来就不容易跑偏，减少了随机性。另一方面因为批的样本数与整个数据集相比小了很多，计算量也不是很大。
|基本上现在的梯度下降都是基于mini-batch的，所以Keras的模块中经常会出现batch_size，就是指这个。
|顺便说一句，Keras中用的优化器SGD是stochastic gradient descent的缩写，但不代表是一个样本就更新一回，还是基于mini-batch的。







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
