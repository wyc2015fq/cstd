
# sklearn-MultinomialNB朴素贝叶斯分类器 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[sklearn-MultinomialNB朴素贝叶斯分类器](https://www.cnblogs.com/eniac1946/p/7406438.html)
|Posted on|2017-08-21 20:23|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7406438)|[收藏](#)
|原型
|class|sklearn.naive_bayes.|MultinomialNB|(|alpha=1.0|,|fit_prior=True|,|class_prior=None|)
|参数
|Parameters:|alpha|: float, optional (default=1.0)
|Additive (Laplace/Lidstone) smoothing parameter (0 for no smoothing).
|fit_prior|: boolean, optional (default=True)
|Whether to learn class prior probabilities or not. If false, a uniform prior will be used.
|class_prior|: array-like, size (n_classes,), optional (default=None)
|Prior probabilities of the classes. If specified the priors are not adjusted according to the data.
|
|alpha的说明——
|The parameters![\theta_y](http://scikit-learn.org/stable/_images/math/fa63fe7665119a0bbb2468099d0048af771b5a1c.png)|is estimated by a smoothed version of maximum likelihood, i.e. relative frequency counting:
![\hat{\theta}_{yi} = \frac{ N_{yi} + \alpha}{N_y + \alpha n}](http://scikit-learn.org/stable/_images/math/ac8b939d0915826ce833b43ac13efa8a0fef99d9.png)
|where![N_{yi} = \sum_{x \in T} x_i](http://scikit-learn.org/stable/_images/math/54f18d12bb04079aeccdee4418942d30073acc14.png)|is the number of times feature![i](http://scikit-learn.org/stable/_images/math/df0deb143e5ac127f00bd248ee8001ecae572adc.png)|appears in a sample of class![y](http://scikit-learn.org/stable/_images/math/276f7e256cbddeb81eee42e1efc348f3cb4ab5f8.png)|in the training set![T](http://scikit-learn.org/stable/_images/math/f2d283a2071f9d043c9e0b0f794a8880fa0d3ce9.png)|, and![N_{y} = \sum_{i=1}^{|T|} N_{yi}](http://scikit-learn.org/stable/_images/math/125143ac10e186061d40627dae00010cb4eeb04f.png)|is the total count of all features for class![y](http://scikit-learn.org/stable/_images/math/276f7e256cbddeb81eee42e1efc348f3cb4ab5f8.png)|.
|The smoothing priors![\alpha \ge 0](http://scikit-learn.org/stable/_images/math/9d068518ed0efc64b3e2587f29137ab2f56a6ec2.png)|accounts for features not present in the learning samples and prevents zero probabilities in further computations. Setting![\alpha = 1](http://scikit-learn.org/stable/_images/math/42839bc86a7e883dcb0a636add188e5c920bb513.png)|is called Laplace smoothing, while![\alpha < 1](http://scikit-learn.org/stable/_images/math/f7c5135a83ac112fff6f442a31cda3a7944b7549.png)|is called Lidstone smoothing.
|示例
|>>> import numpy as np
>>> X = np.random.randint(5, size=(6, 100))
>>> y = np.array([1, 2, 3, 4, 5, 6])
>>> from sklearn.naive_bayes import MultinomialNB
>>> clf = MultinomialNB()
>>> clf.fit(X, y)
MultinomialNB(alpha=1.0, class_prior=None, fit_prior=True)
>>> print(clf.predict(X[2:3]))







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
