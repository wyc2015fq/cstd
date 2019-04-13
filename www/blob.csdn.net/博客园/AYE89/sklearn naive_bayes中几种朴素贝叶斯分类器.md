
# sklearn.naive_bayes中几种朴素贝叶斯分类器 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[sklearn.naive_bayes中几种朴素贝叶斯分类器](https://www.cnblogs.com/eniac1946/p/7407099.html)
|Posted on|2017-08-21 22:37|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7407099)|[收藏](#)
|区别：
|几种朴素贝叶斯分类器的区别在于对于![](https://images2017.cnblogs.com/blog/1181483/201708/1181483-20170821215956308-2101078768.png)|分布的假设，即假设![](https://images2017.cnblogs.com/blog/1181483/201708/1181483-20170821220124714-944208537.png)|满足的形式。
|一、高斯NB
|导入
|from|sklearn.naive_bayes|import|GaussianNB|假设特征的似然函数满足，
![](https://images2017.cnblogs.com/blog/1181483/201708/1181483-20170821220441293-680482042.png)
![\sigma_y](http://scikit-learn.org/stable/_images/math/0b30a2d48c2805095245dba3b4be2dfd2e2fba2e.png)|和![\mu_y](http://scikit-learn.org/stable/_images/math/1d179ee3ab9ccc290192219dc80ee72cff12acb2.png)|采用“最大似然估计”
|二、Multinomial NB
|导入
|from|sklearn.naive_bayes|import|MultinomialNB|特征是离散值，通常用样本的概率去估计![](https://images2017.cnblogs.com/blog/1181483/201708/1181483-20170821222329964-1472560012.png)
|为避免有的特征值缺省，一般对样本的概率做Laplace平滑：（a=1时）
![](https://images2017.cnblogs.com/blog/1181483/201708/1181483-20170821222454355-1975685340.png)
|三、Bernoulli NB
|原型
|class|sklearn.naive_bayes.|BernoulliNB|(|alpha=1.0|,|binarize=0.0|,|fit_prior=True|,|class_prior=None|)
|假设特征的似然满足多元Bernoulli分布，每个特征是一个二值变量
|Bernoulli NB判别准则满足：
![](https://images2017.cnblogs.com/blog/1181483/201708/1181483-20170821223449949-159044778.png)
|区别于Multinomial NB
|有文献证明，在短文本分类上Bernoulli NB可能优于Multinomial NB







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
