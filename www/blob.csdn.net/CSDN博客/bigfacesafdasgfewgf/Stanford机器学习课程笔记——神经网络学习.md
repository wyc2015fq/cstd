# Stanford机器学习课程笔记——神经网络学习 - bigfacesafdasgfewgf - CSDN博客





2015年01月28日 14:45:46[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：901标签：[神经网络																[正向传递																[反向传递																[梯度下降法																[偏导](https://so.csdn.net/so/search/s.do?q=偏导&t=blog)
个人分类：[Machine Learning&&Data Mining](https://blog.csdn.net/puqutogether/article/category/2254295)





**Stanford机器学习课程笔记——神经网络学习**



    前面引入了神经网络的概念，也说了如何通过输入层、中间层和输出层来表示一个神经网络，也说了如何通过前向传递来得到每一层的元素，最后可以得到输出层的假设。

    这一篇中，我们来学习如何通过神经网络来训练它，具体而言，如何通过代价函数来学习每一层的每一个元素。

    要完成神经网络的学习需要有如下两个步骤：




**1） 选择网状结构**




    这个步骤中，我们需要选择网络结构，确定神经网络的层数，以及每一层都多少个单元。我们知道第一层的单元数是我们训练样本的个数，最后一层的元素数是我们分类结果的类别数。中间层可以是1，那么就是最简单的三层神经网络。如果中间层，不只一个的话，那么最好让每个隐藏层的单元个数相同，通常情况下隐藏层单元的个数越到越好，当时太多了也会影响到模型的复杂程度。




# 2）训练神经网络




    这个步骤比较重要。主要是包括：


- 每层之间权重参数矩阵的随机初始化（不要初始化为全0矩阵）；
- 利用正向传递，先求出每一层中单元，以及最后的假设。这个是可以通过前面一篇得到；
- 确定代价函数J_theta；
- 利用反向传递，计算代价函数J_theta的在每一层上的偏导数；
- 利用梯度检验的方法，检验偏导数求解是否正确；
- 利用优梯度下降法来迭代求出最小化代价函数的最优theta。




    前面的权重矩阵初始化、正向传递求出一个初始的假设，这两步在前面一篇博文中已经讲过了，这边用下面这个图来表示：




![](https://img-blog.csdn.net/20150128145936783?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





这篇主要是后面几步。







首先我们来说神经网络中的代价函数，它的形式也是和LR中的代价函数类似的。

    LR的代价函数如下：

![](https://img-blog.csdn.net/20150128150011687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





而神经网络中的代价函数如下：

![](https://img-blog.csdn.net/20150128150120024?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


我们看看二者之间的共同点和不同点。相同点就是大家整体的结构都是一样的，前面一项都是误差，后面一项都是正则项。那么不同点，第一个是参数不同，一个是每个特征前在线性回归下的参数（系数），一个是每层之间的权重系数，我们暂且都成为theta；另一个就是后面的sigma求和变多了。这是因为神经网络中学习到的是一个多类的分类器，K类，这样我们的误差肯定是每类都会有的，所有需要额外的一个sigma从k=1:K，而正则项中由于权重系数增多，不再是每个特征维度一个系数，而是一个矩阵形式的，一共有L-1层有权重系数，而是是从s_l到s_l+1的匹配（一一对应的），所以我们需要3个sigma来表示正则项。这也算是详细解读了神经网络中的代价函数了。





然后，我们需要利用反向传递技术来确定代价函数的偏导数。这部分比较重要。

    为什么需要求代价函数的偏导数呢？因为我们要令代价函数最小，为什就使用反向传递技术呢？因为神经网络中的误差是一级一级传递的，我们可以直接获得的是最后一层输出层和真实输出的误差，所以我们需要一层层的往前推导，求得代价函数的偏导。

    举个例子：

![](https://img-blog.csdn.net/20150128151205569?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


这一部分有比较详细的推导。

    我们需要求的是代价函数J关于每一层权重系数theta_j_i的偏导，如下：

![](https://img-blog.csdn.net/20150128152508378?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


解释一下，为什么把原始的偏导公式拆开。因为这样方便求解，而为什么是z_l+1呢，而不是z_l呢？这就要联系上面的z关于a的表达式了，我们都是通过权重系数，结合第l层的单元，来表示第l+1层的单元，而要求z关于theta的偏导的话，我们就一定只有通过z_l+1啦！所以这部分是合理的。

    这个偏导公式包括两项。

    第一项，因为是偏导，所以这里的z_l+1已经被精确到每个单元i了，也就是z_l+1_i单元了。这也从另一个角度来说明每一层的误差都是前面一层中每个单元叠加起来的。好了，现在我们需要求第l+1层的第i个单元对误差E的贡献，也就是偏导，我们引入error变量delta![](https://img-blog.csdn.net/20150128153103531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center).它的求解需要分最后一层和中间层。如下：

（这部分在Rachel Zhang的博文中已经讲的比较清楚了，我就直接贴过来）

![](https://img-blog.csdn.net/20150128153347484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





我们可以发现，第l层的delta是需要前面第l+1层的delta的，这也就是反向传递技术的关键点。

![](https://img-blog.csdn.net/20150128154333156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


    第二项，因为是z_l+1对于theta_l的偏导，而前面提到了公式，它们之间是有个表示关系的，也就是：

![](https://img-blog.csdn.net/20150128153835227?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


所以它们的偏导就等于a_l_j。

    这样我们原始的误差E关于theta的偏导就可以求得，如下：

![](https://img-blog.csdn.net/20150128153917171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


有了偏导公式，我们就可以用下面的公式来更新theta：

![](https://img-blog.csdn.net/20150128154010687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


这样从后往前，反向传递代价函数的偏导，可以更新每一层的theta。




    接着是梯度检验法来核实前面的偏导求解是否正确。

    这种思路的基本思路就是通过估计某一点的梯度值，来检验上面偏导求解是否正确。比如，对于某个theta, 我们可以计算theta-eps和theta+eps两个点的代价函数值，即选择2个非常接近的点，计算它们的平均值来估计theta处的梯度。看看和前面求得的是否相似。其实这个思路就是回到了梯度，或者说是求导的本质，它们的定义公式一开始就是这样子的。

 记住，虽然梯度检验法也是可以得到每个theta的偏导值的，当时我们只把它当作一种检验的方法，而不是所有学习更新的theta的梯度都是它来做，因为它的计算速度是很慢的，就像我们求x^2的导数会直接用公式来计算，而不是同两个接近的点函数值的平均值来算。




    好了，有了上面的偏导和检验的方法，我们就有了梯度下降法所有的东西了，算是万事俱备了。后面的就是梯度下降法的步骤了。

    这样就可以学习到让代价函数最小化的最优解theta了。






参考：

Coursera机器学习笔记

http://blog.csdn.net/abcjennifer/article/details/7758797](https://so.csdn.net/so/search/s.do?q=梯度下降法&t=blog)](https://so.csdn.net/so/search/s.do?q=反向传递&t=blog)](https://so.csdn.net/so/search/s.do?q=正向传递&t=blog)](https://so.csdn.net/so/search/s.do?q=神经网络&t=blog)




