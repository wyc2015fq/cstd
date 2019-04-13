
# 梯度下降（Gradient Descent）相关概念 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[梯度下降（Gradient Descent）相关概念](https://www.cnblogs.com/eniac1946/p/7815824.html)
|Posted on|2017-11-10 17:54|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7815824)|[收藏](#)
|梯度，直观理解：
|梯度: 运算的对像是纯量,运算出来的结果会是向量在一个标量场中,
|梯度的计算结果会是"在每个位置都算出一个向量,而这个向量的方向会是在任何一点上从其周围(极接近的周围,学过微积分该知道甚么叫极限吧?)标量值最小处指向周围标量值最大处.而这个向量的|大小|会是上面所说的那个|最小与最大的差距程度|"
|举例子来讲会比较简单,如果现在的纯量场用一座山来表示,纯量值越大的地方越高,反之则越低.经过梯度这个运操作数的运算以后,会在这座山的每一个点上都算出一个向量,这个|向量|会|指向每个点最陡的那个方向|,而向量的大小则代表了这个最陡的方向到底有多陡.
|梯度下降|的直观理解：简而言之，就是下山不知道路怎么走，就走一步算一步
|重要概念：
|[梯度下降（Gradient Descent）小结](http://www.cnblogs.com/pinard/p/5970503.html)
|如下图，梯度下降不一定能够找到全局的最优解，有可能是一个|局部最优解|。当然，如果损失函数是凸函数，梯度下降法得到的解就一定是全局最优解。
![](https://images2017.cnblogs.com/blog/1181483/201711/1181483-20171110173926434-1527391293.png)
|在详细了解梯度下降的算法之前，我们先看看相关的一些概念。
|1. 步长（Learning rate）：步长决定了在梯度下降迭代的过程中，每一步沿梯度负方向前进的长度。用上面下山的例子，步长就是在当前这一步所在位置沿着最陡峭最易下山的位置走的那一步的长度。
|2.特征（feature）：指的是样本中输入部分，比如样本（x|0|,y|0|）,（x|1|,y|1|）,则样本特征为x，样本输出为y。
|3. 假设函数（hypothesis function）：在监督学习中，为了拟合输入样本，而使用的假设函数，记为h|θ|(x)。比如对于样本（x|i|,y|i|）(i=1,2,...n),可以采用拟合函数如下： h|θ|(x) = θ|0|+θ|1|x。
|4. 损失函数（loss function）：为了评估模型拟合的好坏，通常用损失函数来度量拟合的程度。损失函数极小化，意味着拟合程度最好，对应的模型参数即为最优参数。在线性回归中，损失函数通常为样本输出和假设函数的差取平方。比如对于样本（x|i|,y|i|）(i=1,2,...n),采用线性回归，损失函数为：
|\(J(\theta_0, \theta_1) = \sum\limits_{i=1}^{m}(h_\theta(x_i) - y_i)^2\)
|其中\(x_i\)表示样本特征x的第i个元素，\(y_i\)表示样本输出y的第i个元素，\(h_\theta(x_i)\)为假设函数。
|Cost Function

|Cost Function的用途：对假设的函数进行评价，cost function越小的函数，说明拟合训练数据拟合的越好；
|下图详细说明了当cost function为黑盒的时候，cost function 的作用；
![](http://img.my.csdn.net/uploads/201209/06/1346901932_6098.png)
|但是我们肯定想知道cost Function的内部构造是什么？因此我们下面给出公式：

![](http://img.my.csdn.net/uploads/201209/06/1346902047_6543.png)
|其中：
![](https://images2017.cnblogs.com/blog/1181483/201711/1181483-20171110175300450-2029971600.png)|表示向量x中的第i个元素；
![](https://images2017.cnblogs.com/blog/1181483/201711/1181483-20171110175231591-565322931.png)|表示向量y中的第i个元素；
![](https://images2017.cnblogs.com/blog/1181483/201711/1181483-20171110175327591-1779892731.png)|表示已知的假设函数；
|m为训练集的数量；
|比如给定数据集(1,1)、(2,2)、(3,3)
|则x = [1;2;3]，y = [1;2;3]     （此处的语法为Octave语言的语法，表示3*1的矩阵）
|如果我们预测theta0 = 0，theta1 = 1，则h(x) = x，则cost function：
|J(0,1) = 1/(2*3) * [(h(1)-1)^2+(h(2)-2)^2+(h(3)-3)^2] = 0；
|如果我们预测theta0 = 0，theta1 = 0.5，则h(x) = 0.5x，则cost function：
|J(0,0.5) = 1/(2*3) * [(h(1)-1)^2+(h(2)-2)^2+(h(3)-3)^2] = 0.58；
|








|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
