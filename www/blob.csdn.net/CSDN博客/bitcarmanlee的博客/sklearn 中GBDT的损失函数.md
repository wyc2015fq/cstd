
# sklearn 中GBDT的损失函数 - bitcarmanlee的博客 - CSDN博客


2017年09月09日 21:33:03[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：2049标签：[GBDT																](https://so.csdn.net/so/search/s.do?q=GBDT&t=blog)[损失函数																](https://so.csdn.net/so/search/s.do?q=损失函数&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=GBDT&t=blog)个人分类：[ml algorithm																](https://blog.csdn.net/bitcarmanlee/article/category/6182356)



## 1.分类模型
分类模型的损失函数包括有”deviance”和指数损失函数”exponential”，默认为对数损失函数。
相关参考链接：
1.损失函数[http://www.csuldw.com/2016/03/26/2016-03-26-loss-function/](http://www.csuldw.com/2016/03/26/2016-03-26-loss-function/)
2.Adaboost与指数损失[http://breezedeus.github.io/2015/07/12/breezedeus-adaboost-exponential-loss.html](http://breezedeus.github.io/2015/07/12/breezedeus-adaboost-exponential-loss.html)
## 2.回归模型
回归模型的损失函数包括均方差”ls”，绝对损失”lad”，Huber损失”huber”，分位数损失”quantile”。
相关参考链接：
1.Huber Loss function[http://blog.csdn.net/lanchunhui/article/details/50427055](http://blog.csdn.net/lanchunhui/article/details/50427055)
参看文献：
1.GBDT算法原理深入解析：[https://www.zybuluo.com/yxd/note/611571](https://www.zybuluo.com/yxd/note/611571)
2.XGBoost 与 Boosted Tree[http://www.52cs.org/?p=429](http://www.52cs.org/?p=429)

