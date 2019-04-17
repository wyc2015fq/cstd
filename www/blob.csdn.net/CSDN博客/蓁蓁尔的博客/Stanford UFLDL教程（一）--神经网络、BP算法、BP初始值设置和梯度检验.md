# Stanford UFLDL教程（一）--神经网络、BP算法、BP初始值设置和梯度检验 - 蓁蓁尔的博客 - CSDN博客





2017年03月08日 15:20:30[蓁蓁尔](https://me.csdn.net/u013527419)阅读数：459








在看斯坦福大学的UFLDL（非监督特征学习和深度学习）教程时，发现了一个博客，在看教程的过程中不懂得问题，大部分在博客中得到了解答。转到自己帖子中方便以后重新查看。

01.神经网络 

UFLDL：[http://deeplearning.stanford.edu/wiki/index.php/%E7%A5%9E%E7%BB%8F%E7%BD%91%E7%BB%9C](http://deeplearning.stanford.edu/wiki/index.php/%E7%A5%9E%E7%BB%8F%E7%BD%91%E7%BB%9C)

博客：[http://blog.csdn.net/bea_tree/article/details/51174511](http://blog.csdn.net/bea_tree/article/details/51174511)

这一节挺简单。
02.反向传导算法 

UFLDL：[http://deeplearning.stanford.edu/wiki/index.php/%E5%8F%8D%E5%90%91%E4%BC%A0%E5%AF%BC%E7%AE%97%E6%B3%95](http://deeplearning.stanford.edu/wiki/index.php/%E5%8F%8D%E5%90%91%E4%BC%A0%E5%AF%BC%E7%AE%97%E6%B3%95)

博客：[http://blog.csdn.net/bea_tree/article/details/51174776](http://blog.csdn.net/bea_tree/article/details/51174776)
主要问题： 

（1）后向传播算法是基于梯度下降策略的，求梯度时用到了**链式法则**。 

（2）BP算法中W和b的初始值设置问题，文中要求不能设为相同值，推荐设置为一个很小的、接       近零的随机值（比如说，使用正态分布生成的随机值） 

解答：feed-forward神经网络中同层各神经元的地位的相同的，交换与两个神经元相连的所有权重不改变网络的功能，对称性在神经网络中大量存在，随机初始化的目的是**使对称失效**。 

知乎回答：[https://www.zhihu.com/question/36068411/answer/95670563?from=profile_answer_card](https://www.zhihu.com/question/36068411/answer/95670563?from=profile_answer_card)
03.梯度检验与高级优化 

UFLDL：[http://deeplearning.stanford.edu/wiki/index.php/%E6%A2%AF%E5%BA%A6%E6%A3%80%E9%AA%8C%E4%B8%8E%E9%AB%98%E7%BA%A7%E4%BC%98%E5%8C%96](http://deeplearning.stanford.edu/wiki/index.php/%E6%A2%AF%E5%BA%A6%E6%A3%80%E9%AA%8C%E4%B8%8E%E9%AB%98%E7%BA%A7%E4%BC%98%E5%8C%96)

博客：[http://blog.csdn.net/bea_tree/article/details/51175891](http://blog.csdn.net/bea_tree/article/details/51175891)

 主要就是将参数稍微增大和减小一点点带入代价函数，应该和原来参数的值相似，如果满足则梯度下降求导过程没错。 

 根据： 
![这里写图片描述](https://img-blog.csdn.net/20170308151917146?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzUyNzQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)






