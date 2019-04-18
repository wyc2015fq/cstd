# NN模型设置--设计新的layer或op - wydbyxr的博客 - CSDN博客
2018年12月04日 10:47:23[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：57
个人分类：[算法的实际使用](https://blog.csdn.net/wydbyxr/article/category/8463718)
  关键是反向传播的实现！
  如果你手动实现了反向传播算法但是它不起作用，那么有99%的可能是梯度计算中存在Bug。那么就用梯度检测来定位问题。
  主要思想是运用梯度的定义：如果我们稍微增加某个权重值，模型的误差将会改变多少。
  参考资料：
[http://ufldl.stanford.edu/wiki/index.php/Gradient_checking_and_advanced_optimization](http://ufldl.stanford.edu/wiki/index.php/Gradient_checking_and_advanced_optimization)
