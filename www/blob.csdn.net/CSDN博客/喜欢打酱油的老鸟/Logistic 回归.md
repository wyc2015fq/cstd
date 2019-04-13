
# Logistic 回归 - 喜欢打酱油的老鸟 - CSDN博客


2018年08月22日 11:04:43[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：358


[https://www.cnblogs.com/DicksonJYL/p/9516387.html](https://www.cnblogs.com/DicksonJYL/p/9516387.html)
**logistic****回归**
logistic回归是一种二分类算法，直接为样本估计出它属于正负样本的概率。先将向量进行线性加权，然后计算logistic函数，可以得到[0,1]之间的概率值，它表示样本x属于正样本的概率：
![](https://img-blog.csdn.net/20180822110307999?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
正样本标签值为1，负样本为0。使用logistic函数的原因是它单调增，并且值域在(0, 1)之间，刚好符合概率的要求。
训练时采用最大似然估计，求解对数似然函数的极值：
![](https://img-blog.csdn.net/20180822110319973?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
可以证明这是一个凸优化问题，求解时可以用梯度下降法，也可以用牛顿法。如果正负样本的标签为+1和-1，则可以采用另外一种写法：
![](https://img-blog.csdn.net/20180822110333276?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
训练时的目标同样是最大化对数似然函数：
![](https://img-blog.csdn.net/20180822110346686?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
同样的，这也是一个凸优化问题。预测时并不需要计算logistic函数，而是直接计算：
![](https://img-blog.csdn.net/20180822110409487?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Logistic回归是一种二分类算法，虽然使用了概率，但它是一种判别模型！另外要注意的是，logistic回归是一种线性模型，这从它的预测函数就可以看出。它本身不能支持多分类问题，它的扩展版本softmax回归可以解决多分类问题。


