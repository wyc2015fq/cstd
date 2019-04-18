# 再理解RankNet算法 - bigfacesafdasgfewgf - CSDN博客





2015年02月09日 09:44:09[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：5884








**再理解RankNet算法**



    前面的一篇博文介绍了学习排序算法（Learning to Rank）中的RankNet算法。如下：

http://blog.csdn.net/puqutogether/article/details/42124491





    那次的入门对其中的算法流程和基本原理没有深入了解，这次看自动摘要提取算法的时候，里面有一个排序单元，使用到的就是学习排序算法中的RankNet，这个时候才理解了。这就说明，有的时候回过头来看某些算法，你的认识会加深的。




   好了，这次主要说一下RankNet这个算法的基本流程和原理。




    RankNet算法是从概率的角度解决排序问题。




    首先，我们需要求得的是一个排序函数，就是当我们输入样本的特征向量的时候，可以输出该样本的顺序“得分”，实现排序。在RankNet中，排序函数定义为一个三层的神经网络模型。输入层和样本特征维数有关，输出层是一个节点（得分），排序函数定义为：

![](https://img-blog.csdn.net/20150209095216504?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


其中权值参数w和偏置参数b的上标表示节点所在的层，下标表示同一层中节点的编号；x_n_k表示特征向量x_n的第k个分量，这是是输入到输入层的。f_x_n的输出就是一个得分。




    然后，由于RankNet是一个pairwise的排序学习算法，把样本两两组成一个pair，对于一个pair，有两个概率需要定义，一个是预测概率：

![](https://img-blog.csdn.net/20150209095625980?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


其物理意义就是第i个样本排在第j个样本前面的概率，其中的s_i和s_ j的都是前面排序函数的输出得分。

还有一个概率是真实概率，定义如下：





![](https://img-blog.csdn.net/20150209095836287?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



其中For a given query, let S_i_ j∈ {0, ±1} be defined to be 1 if document i has been labeled to be more relevant than document j, −1 if document i has been labeled to be less relevant than document j, and 0 if they have the same
 label. 

    然后，基于交叉熵函数建立RankNet算法的损失函数，并用梯度下降法解决。如下：

![](https://img-blog.csdn.net/20150209100133001?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


上式我们可以化简，如下：

![](https://img-blog.csdn.net/20150209101520333?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（好吧，天气寒冷，手抖……）

也就是下面这个式子：

![](https://img-blog.csdn.net/20150209101643671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


    最后，我们让损失函数C对排序函数中的w求导，可以得到：

![](https://img-blog.csdn.net/20150209101852587?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


=》

![](https://img-blog.csdn.net/20150209101939078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


=》

![](https://img-blog.csdn.net/20150209102111773?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





代入可得损失函数C关于w的偏导了，这样就可以使用梯度下降法了。




最终求得排序函数f_x_n。



     可以看书，RankNet算法在学习过程中，用到了一对样本之间的关系，主要在预测概率部分，所以它是一个pairwise的方法。










参考：

http://blog.csdn.net/puqutogether/article/details/42124491


From RankNet to LambdaRank to LambdaMART: An Overview        Christopher J.C. Burges


http://blog.csdn.net/huagong_adu/article/details/40710305





































