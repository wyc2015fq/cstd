
# EM算法 - 喜欢打酱油的老鸟 - CSDN博客


2018年08月22日 11:14:27[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：32标签：[EM算法																](https://so.csdn.net/so/search/s.do?q=EM算法&t=blog)个人分类：[人工智能																](https://blog.csdn.net/weixin_42137700/article/category/7820233)


[https://www.cnblogs.com/DicksonJYL/p/9516464.html](https://www.cnblogs.com/DicksonJYL/p/9516464.html)
**EM****算法**
EM算法是一种迭代法，其目标是求解似然函数或后验概率的极值，而样本中具有无法观测的隐含变量。因为隐变量的存在，我们无法直接通过最大化似然函数来确定参数的值。可以采用一种策略，构造出对数似然函数的一个下界函数，这个函数不含有隐变量，然后优化这个下界。不断的提高这个下界，使原问题达到最优解，这就是EM算法所采用的思路。算法的构造依赖于Jensen不等式。
算法在实现时首先随机初始化参数![](https://img-blog.csdn.net/20180822111249956?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)的值，接下来循环迭代，每次迭代时分为两步：
E步，基于当前的参数估计值![](https://img-blog.csdn.net/20180822111304238?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)，计算在给定x时对z的条件概率的数学期望：
![](https://img-blog.csdn.net/20180822111317212?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
M步，求解如下极值问题，更新![](https://img-blog.csdn.net/201808221113415?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)的值：
![](https://img-blog.csdn.net/20180822111358647?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
实现*Qi*时可以按照下面的公式计算：
![](https://img-blog.csdn.net/20180822111417426?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
迭代终止的判定规则是相邻两次函数值之差小于指定阈值。需要注意的是，EM算法只能保证收敛到局部极小值。


