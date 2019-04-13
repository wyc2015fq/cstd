
# 梯度消失（vanishing gradient）和梯度爆炸（exploding gradient） - 郭云飞的专栏 - CSDN博客


2017年10月19日 11:47:28[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：5329


**神经网络中梯度不稳定的根本原因：**在于前层上的梯度的计算来自于后层上梯度的乘积（链式法则）。当层数很多时，就容易出现不稳定。下边3个隐含层为例：
![](https://img-blog.csdn.net/20171019115019329?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
其**b****1**的梯度为：
![](https://img-blog.csdn.net/20171019115124468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
加入激活函数为**sigmoid**，则其导数如下图：
![](https://img-blog.csdn.net/20171019115510853?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**sigmoid**导数*σ'*的最大值为1/4。同常一个权值**w**的取值范围为*abs(w) < 1*，则：**|****w****j****σ'(****z****j****)****| < 1/4**，从而有：
![](https://img-blog.csdn.net/20171019120032778?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
从上式可以得出结论：前层比后层的梯度变化更小，变化更慢，进而引起**梯度消失**的问题。相反，如果*|***w****j****σ'(****z****j****)***| > 1*时，前层比后层的梯度变化更大，就引起**梯度爆炸**的问题。**实际中，当使用sigmoid作为激活函数时，更普遍的是梯度消失的问题**。
在重复一遍，从根本上讲无论是梯度消失还是梯度爆炸，其背后的原因是前层网络的梯度是后层网络的乘积，所以神经网络不稳定。唯一可能的情况是以上连续乘积**刚好平衡在1左右**，但这种几率很小。
**解决梯度消失的方法：**
隐含层神经元的激活函数用Relu！![大笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/laugh.gif)




