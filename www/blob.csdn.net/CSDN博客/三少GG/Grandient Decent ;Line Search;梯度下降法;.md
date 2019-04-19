# Grandient Decent ;Line Search;梯度下降法; - 三少GG - CSDN博客
2011年05月31日 17:32:00[三少GG](https://me.csdn.net/scut1135)阅读数：2140
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
listwise方法一般从建立objective function角度不同分为最小化loss function和直接优化IR meature（MAP，NDCG）两类方法。一些学者证明二者的关系，一些IR meature 的变形如（1-NDCG）是loss function 的上界。而由于直接优化IR meature的方法缺乏些理论支持，又一些meature是非连续的，不便于优化，所以通过优化loss function 的方法更被接受。下面是三种主流的基于loss function 的方法，都是MSRA提出的。
RankCosine (IP&M 2007) 以truth rank和所得的rank间的cosine相似度作为loss function。
ListNet（ICML2007）以truth rank和所得的rank排列间的KL距离作为loss function。
ListMLE（ICML2008）以似然函数作为loss function。
合理的loss function 是机器学习的关键，提出ListMLE的文章同样介绍了什么样的loss function 才是好的。有了loss funtion 之后学习的过程就比较常规了，一般采用神经网络，优化参数用gradient decent。
**Problems**
Learning to Rank 是利用machine learning 的理论来解决IR中Rank的问题，Rank任务是根据某个标准（一般是指文档和查询的匹配程度）对对象进行排序，显然Rank是IR最核心的部分。Learning to rank任务使IR任务增加了理论深度也使ML理论得到了应用，理应是个很好的研究方向。
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
[http://blog.qinyujie.net/2010/03/18/327/](http://blog.qinyujie.net/2010/03/18/327/)
I take part in a lesson of Machine Intelligence 1  in this semester. And here is my version of the solution of  the 5. exercise.
Task1: Using Neural Network with **Gradient Decent, Line Search**and Conjungate Gradient methods
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
[http://blog.163.com/yutian-flliu/blog/static/1155352942009111215423839/](http://blog.163.com/yutian-flliu/blog/static/1155352942009111215423839/)
以下为转载： 
（梯度下降法，就是利用负梯度方向来决定每次迭代的新的搜索方向，使得每次迭代能使待优化的目标函数逐步减小。梯度下降法是2范数下的最速下降法。
    最速下降法的一种简单形式是：x(k+1)=x(k)-a*g(k),其中a称为学习速率，可以是较小的常数。g（k）是x(k)的梯度。 
    直观的说，就是在一个有中心的等值线中，从初始值开始，每次沿着垂直等值线方向移动一个小的距离，最终收敛在中心。  
    对于某一个性能指数，我们能够运用梯度下降法，使这个指数降到最小。若该指数为均方误差，我们便得到了最小均方误差（LMS）算法。 
    BP算法也是要使性能指数-均方误差达到最小，它与LMS算法的差别在于对于权值导数（即梯度）的计算方式上。     
    LMS是运用在单层线性网络中的，误差是网络权值的显示线性函数，权值导数可直接求得。但其只能解决线性可分问题。     
    BP算法用于有隐藏层的多层网络中，权值和误差的关系比较复杂，需要用到微积分链法则。该算法可以解决线性不可分问题。 如何使用链法则，是个比较复杂的问题，可以参考《神经网络设计》一书） 
**最速下降梯度法matlab程序**
