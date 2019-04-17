# coursera-斯坦福-机器学习-吴恩达-第9周笔记（下）-推荐系统 - 博客堂 - CSDN博客





2017年12月21日 21:18:38[最小森林](https://me.csdn.net/u012052268)阅读数：4263
所属专栏：[斯坦福-吴恩达《机器学习》之路](https://blog.csdn.net/column/details/18191.html)









# coursera-斯坦福-机器学习-吴恩达-第9周笔记（下）-推荐系统



- [coursera-斯坦福-机器学习-吴恩达-第9周笔记下-推荐系统](#coursera-斯坦福-机器学习-吴恩达-第9周笔记下-推荐系统)- [1预测电影等级](#1预测电影等级)- [1任务设想](#11任务设想)
- [2基于内容的推荐](#12基于内容的推荐)

- [2协同过滤](#2协同过滤)- [1协同过滤](#21协同过滤)
- [2协同过滤算法](#22协同过滤算法)

- [3低秩矩阵分解Low Rank Matrix Factorization](#3低秩矩阵分解low-rank-matrix-factorization)- [1向量化低秩矩阵分解Low Rank Matrix Factorization](#31向量化低秩矩阵分解low-rank-matrix-factorization)
- [2实现细节](#32实现细节)

- [4复习](#4复习)- [1quiz](#41quiz)
- [2编程](#42编程)






## 1预测电影等级

推荐系统(recommender systems)，比如对像 Netflix 这样的公司 ，他们向用户推荐的电影 占了用户观看的电影的 相当大一部分 。

对于机器学习来说 特征量是重要的， 你选择的特征 对你学习算法的表现有很大影响。 在机器学习领域 有这么一个宏大的想法， 就是对于一些问题  存在一些算法， 能试图自动地替你学习到一组优良的特征量。 

而推荐系统 就是这种情形的一个例子。还有其他很多例子 但通过学习推荐系统 ，我们将能够 对这种学习特征量的想法 有一点理解。

### 1.1任务设想

![image](https://img-blog.csdn.net/20150403094152996?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGlwaXNvcnJ5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

我这里有5部电影 《爱到最后》 《浪漫永远》 《小爱犬》 《无尽狂飙》 还有 《剑与空手道》。 我们有4位用户 名叫 Alice Bob Carol 和 Dave 。首字母为A B C和D 我们称他们用户1 2 3和4 。比方说 ，Alice 她非常喜欢 《爱到最后》 把它评为5颗星。 她还喜欢 《浪漫永远》 也把它评为5颗星。 她没看过 《小爱犬》 也就没评分， 这样我们没有这个评分数据。

我们查看数据并查看所有缺失的电影评级，并试图预测这些问号的值应该是多少。

### 1.2基于内容的推荐

每个items都有一些features，如果我们知道它们的值是多少，同时每个用户通过θj告诉我们他们有多喜欢romantic或者action movies。这种按照内容的特征来推荐的算法就是——基于内容推荐。

![](http://oqy7bjehk.bkt.clouddn.com/17-12-21/94450266.jpg)

使用梯度下降优化： 
![](http://oqy7bjehk.bkt.clouddn.com/17-12-21/22789159.jpg)
如果你觉得这个 梯度下降的更新 看起来跟之前 线性回归差不多的话， 那是因为这其实就是线性回归， 唯一的一点区别 是在线性回归中 我们有1/m项 。

通过这节课 你应该知道了 怎样应用一种 事实上是线性回归的一个变体， 来预测不同用户对不同电影的评分值 ，这种具体的算法叫 ”基于内容的推荐“ 或者”基于内容的方法“。 因为我们假设 我们有不同电影的特征 ，我们有了电影 内容的特征 比如电影的爱情成分有多少？动作成分有多少？ 我们就是用电影的这些特征 来进行预测 。

但事实上 对很多电影 我们并没有这些特征 或者说 很难得到 所有电影的特征 很难知道 我们要卖的产品 有什么样的特征 。所以在下一段视频中 我们将谈到一种不基于内容的推荐系统：协同过滤。

## 2协同过滤

### 2.1协同过滤

在这段视频中 我们要讲 一种构建推荐系统的方法 叫做协同过滤(collaborative filtering) 。

算法 有一个值得一提的 特点 ，那就是它能实现 对特征的学习。 我的意思是 这种算法能够 自行学习所要使用的特征 。

我们建一个数据集 ，假定是为每一部电影准备的 ，对每一部电影 我们找一些人来 告诉我们这部电影 浪漫指数是多少 动作指数是多少。  

但想一下就知道 这样做难度很大， 也很花费时间 。你想想 要让每个人 看完每一部电影 告诉你你每一部电影有多浪漫 多动作 这是一件不容易的事情。

现在我们稍稍改变一下这个假设 ，假设我们采访了每一位用户 而且每一位用户都告诉我们 他们是否喜欢 爱情电影。

总结一下， 这一阶段要做的 就是为所有 为电影评分的 用户 j 选择特征 x(i)。 这一算法同样也预测出一个值 ，表示该用户将会如何评价某部电影。 而这个预测值 在平方误差的形式中 与用户对该电影评分的实际值尽量接近 。

优化目标： 
$\displaystyle \min_{x^{(1)},...,x^{(n_m)}} \frac{1}{2}\sum_{i=1}^{n_m}\sum_{j:r(i,j)=1}\left((\theta^{(j)})^T x^{(i)}-y^{(i,j)}\right)^2 + \frac{\lambda}{2}\sum_{i=1}^{n_m}\sum_{k=1}^n(x_k^{(i)})^2$

梯度下降（注意是对x求偏导）： 
$x_k^{(i)} := x_k^{(i)} - \alpha\left(\sum_{j:r(i,j)=1}\left((\theta^{(j)})^T(x^{(i)}) - y^{(i,j)}\right)\theta_k^{(j)}+ \lambda x_k^{(i)}\right)$

我们之前 这个视频中讲的是 ，如果用户愿意 为你提供参数 ，那么你就可以为不同的电影估计特征 。

这有点像鸡和蛋的问题 到底先有鸡还是先有蛋？就是说 如果我们能知道 θ 就能学习到 x ，如果我们知道 x 也会学出 θ 来 。

我们可以一开始随机 猜测出的 θ 的值， 你可以继续下去 运用我们刚刚讲到的 步骤 我们可以学习出 不同电影的特征 。

同样的给出已有的一些电影的 原始特征， 你可以运用 我们在上一个视频中讨论过的 第一种方法 ，可以得到 对参数 θ 的更好估计 ，这样就会为用户提供更好的参数 θ 集。

我们可以继续 迭代 不停重复 优化θ x θ x θ 这非常有效

### 2.2协同过滤算法

在前面几个视频里 我们谈到几个概念 ，首先 ，如果给你几个特征表示电影， 我们可以使用这些资料去获得用户的参数数据。 第二 ，如果给你用户的参数数据， 你可以使用这些资料去获得电影的特征。

本节视频中 我们将会使用这些概念 并且将它们合并成 协同过滤算法 (Collaborative Filtering Algorithm) 。

总结一下我们之前做过的事情 ：其中之一是， 假如你有了电影的特征 ，你就可以解出 这个最小化问题 ，为你的用户找到参数 θ 。然后我们也 知道了， 如果你拥有参数 θ， 你也可以用该参数 通过解一个最小化问题 去计算出特征 x 。

所以你可以做的事 是不停地重复这些计算 ，首先随机地初始化这些参数 ，然后解出 θ 解出 x 解出 θ 解出 x ….但我将它们给合在一起 

![](http://oqy7bjehk.bkt.clouddn.com/17-12-21/95135789.jpg)

然后同时计算两个的梯度：

![](http://oqy7bjehk.bkt.clouddn.com/17-12-21/64513372.jpg)

## 3低秩矩阵分解Low Rank Matrix Factorization

在上几节视频中 我们谈到了协同过滤算法 ，本节视频中我将会 讲到有关 该算法的向量化实现。

### 3.1向量化：低秩矩阵分解Low Rank Matrix Factorization

如果你有 预测评分矩阵 ，你就会有 以下的这个 有着(i, j)位置数据的矩阵 。 
![](http://oqy7bjehk.bkt.clouddn.com/17-12-21/28950511.jpg)
也就是令：$X = \begin{bmatrix} - & (x^{(1)})^T & - \\ & \vdots & \\ - & (x^{(n_m)} & - \end{bmatrix},\ \Theta = \begin{bmatrix} - & (\theta^{(1)})^T & - \\ & \vdots & \\ - & (\theta^{(n_u)} & - \end{bmatrix}$

则：$X\Theta^T = \displaystyle \begin{bmatrix} (x^{(1)})^T(\theta^{(1)}) & \ldots & (x^{(1)})^T(\theta^{(n_u)})\\ \vdots & \ddots & \vdots \\ (x^{(n_m)})^T(\theta^{(1)}) & \ldots & (x^{(n_m)})^T(\theta^{(n_u)})\end{bmatrix}$

这就是低秩矩阵分解 。

现在既然你已经 对特征参数向量进行了学习 ，那么我们就会有一个很方便的方法 来度量两部电影之间的相似性。 例如说 ，电影i有一个特征向量x(i)， 你是否能找到一部 不同的电影 j，保证两部电影的特征向量之间的距离x(i)和x(j)很小 ，那就能 很有力地表明 电影 i 和电影 j在某种程度上有相似。

就是计算 small ||$x^{(i)}-x^{(j)}$||。

### 3.2实现细节

到目前为止 你已经了解到了 推荐系统算法或者 协同过滤算法的所有要点 。在这节视频中 我想分享最后一点实现过程中的细节 ，这一点就是均值归一化 有时它可以让算法 运行得更好 。

所有item减去其（所有用户给它的）打分均值（没有评分的user_item不计入均值的计算），参数推断完成后再加回来。 
![image](https://img-blog.csdn.net/20150403175733481?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGlwaXNvcnJ5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
如果电影没有评分，就将列和均值设置为0；如果用户没有评分，就将行均值设置为0。

否则新用户评分为0，则为负样例。

## 4复习

### 4.1quiz
- 
Suppose you run a bookstore, and have ratings (1 to 5 stars) of books. Your collaborative filtering algorithm has learned a parameter vector θ(j) for user j, and a feature vector x(i) for each book. You would like to compute the “training error”, meaning the average squared error of your system’s predictions on all the ratings that you have gotten from your users. Which of these are correct ways of doing so (check all that apply)? For this problem, let m be the total number of ratings you have gotten from your users. (Another way of saying this is that m=∑nmi=1∑nuj=1r(i,j)). [Hint: Two of the four options below are correct.]  

答案AC
- 
[ ] 1m∑nmi=1∑j:r(i,j)=1(∑nk=1(θ(j))kx(i)k−y(i,j))2

- 
[ ] 1m∑nuj=1∑i:r(i,j)=1(∑nk=1(θ(k))jx(k)i−y(i,j))2

- 
[ ] 1m∑(i,j):r(i,j)=1((θ(j))Tx(i)−y(i,j))2

- 
[ ] 1m∑(i,j):r(i,j)=1((θ(j))Tx(i)−r(i,j))2


- 
In which of the following situations will a collaborative filtering system be the most appropriate learning algorithm (compared to linear or logistic regression)? 

答案BCD（这个题醉了 怎么选都错）
- 
[ ] You manage an online bookstore and you have the book ratings from many users. You want to learn to predict the expected sales volume (number of books sold) as a function of the average rating of a book.

- 
[ ] You manage an online bookstore and you have the book ratings from many users. For each user, you want to recommend other books she will enjoy, based on her own ratings and the ratings of other users.

- 
[ ] You run an online news aggregator, and for every user, you know some subset of articles that the user likes and some different subset that the user dislikes. You’d want to use this to find other articles that the user likes.

- 
[ ] You’ve written a piece of software that has downloaded news articles from many news websites. In your system, you also keep track of which articles you personally like vs. dislike, and the system also stores away features of these articles (e.g., word counts, name of author). Using this information, you want to build a system to try to find additional new articles that you personally will like.


- 
You run a movie empire, and want to build a movie recommendation system based on collaborative filtering. There were three popular review websites (which we’ll call A, B and C) which users to go to rate movies, and you have just acquired all three companies that run these websites. You’d like to merge the three companies’ datasets together to build a single/unified system. On website A, users rank a movie as having 1 through 5 stars. On website B, users rank on a scale of 1 - 10, and decimal values (e.g., 7.5) are allowed. On website C, the ratings are from 1 to 100. You also have enough information to identify users/movies on one website with users/movies on a different website. Which of the following statements is true? 

答案D
- 
[ ] You can combine all three training sets into one without any modification and expect high performance from a recommendation system.

- 
[ ] Assuming that there is at least one movie/user in one database that doesn’t also appear in a second database, there is no sound way to merge the datasets, because of the missing data.

- 
[ ] It is not possible to combine these websites’ data. You must build three separate recommendation systems.

- 
[ ] You can merge the three datasets into one, but you should first normalize each dataset’s ratings (say rescale each dataset’s ratings to a 1-100 range).


- 
Which of the following are true of collaborative filtering systems? Check all that apply. 

答案AC
- 
[ ] If you have a dataset of users ratings’ on some products, you can use these to predict one user’s preferences on products he has not rated.

- 
[ ] When using gradient descent to train a collaborative filtering system, it is okay to initialize all the parameters (x(i) and θ(j)) to zero.

- 
[ ] Recall that the cost function for the content-based recommendation system is J(θ)=12∑nuj=1∑i:r(i,j)=1((θ(j))Tx(i)−y(i,j))2+λ2∑nuj=1∑nk=1(θ(j)k)2. Suppose there is only one user and he has rated every movie in the training set. This implies that nu=1 and r(i,j)=1 for every i,j. In this case, the cost function J(θ) is equivalent to the one used for regularized linear regression.

- 
[ ] To use collaborative filtering, you need to manually design a feature vector for every item (e.g., movie) in your dataset, that describes that item’s most important properties.


- 
Suppose you have two matrices A and B, where A is 5x3 and B is 3x5. Their product is C=AB, a 5x5 matrix. Furthermore, you have a 5x5 matrix R where every entry is 0 or 1. You want to find the sum of all elements C(i,j) for which the corresponding R(i,j) is 1, and ignore all elements C(i,j) where R(i,j)=0. One way to do so is the following code: Which of the following pieces of Octave code will also correctly compute this total? Check all that apply. Assume all options are in code. 

答案AB
- 
[ ] total = sum(sum((A * B) .* R))

- 
[ ] C = (A * B) .* R; total = sum(C(:));

- 
[ ] total = sum(sum((A * B) * R));

- 
[ ] C = (A * B) * R; total = sum(C(:));



1m∑(i,j):r(i,j)=1((θ(j))Tx(i)−y(i,j))2

1m∑nuj=1∑i:r(i,j)=1((θ(j))ix(i)j−y(i,j))2

1m∑(i,j):r(i,j)=1(∑nk=1(θ(j))kx(i)k−y(i,j))2

1m∑nuj=1∑i:r(i,j)=1(∑nk=1(θ(j))kx(i)k−y(i,j))2

1m∑(i,j):r(i,j)=1∑nk=1((θ(j))kx(i)k−y(i,j))2

### 4.2编程

在本练习中，您将实施异常检测算法并将其应用于检测网络上发生故障的服务器。 在第二部分中，您将使用协作过滤来构建电影的推荐系统

1异常检测 

在这个练习中，您将实现一个异常检测算法来检测服务器计算机中的异常行为。 这些功能衡量每个服务器响应的吞吐量（mb / s）和延迟（ms）。 当你的服务器正在运行时，你收集了他们如何行为的例子，因此有一个未标记的数据集{x（1），…，x（m）}。 您怀疑这些示例中绝大多数是服务器正常运行的“正常”（非异常）示例，但也可能有一些服务器在此数据集中异常运行的示例。

打开estimateGaussian.m

```
mu = 1/m * sum(X);
sigma2 = 1/m * sum((X - repmat(mu, m, 1)).^2);
```

打开selectThreshold.m填入：

```xml
predictions = (pval < epsilon);
    fp = sum((predictions == 1) & (yval == 0));
    fn = sum((predictions == 0) & (yval == 1));
    tp = sum((predictions == 1) & (yval == 1));

    prec = tp / (tp + fp);
    rec = tp / (tp + fn);

    F1 = 2 * prec * rec / (prec + rec);
```

打开cofiCostFunc.m填入：

```
X_grad = (((X*Theta').*R*Theta-Y.*R*Theta)+lambda.*X);
Theta_grad = ((X'*((X*Theta').*R)-X'*(Y.*R)))'+lambda.*Theta;
```






