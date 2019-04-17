# improved GAN Part1 - sinat_32043495的博客 - CSDN博客





2017年11月28日 11:02:49[LZXandTM](https://me.csdn.net/sinat_32043495)阅读数：144








## **1.F-divergence:衡量P和Q有多不一样**

**内容有函数f要满足的条件:为什么可以满足f(1)=0、为什么可以满足Df的最小值是0（f是凸函数的条件）**

**![](https://img-blog.csdn.net/20171128111504484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**




**举例，有哪些函数可以作为F-divergence**

**KL divergence就是一种F-divergence**





![](https://img-blog.csdn.net/20171128111546712?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



## 2.Fenchel Conjugate

**每一个f都有一个和它配对的f***

**f*到底是在做什么**

**先考虑一个点的情况，当t=t1,t=t2....都找出对应的x值与t配对使得f*最大**



![](https://img-blog.csdn.net/20171128111627501?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**这个function长什么样子**

**如果给定x，f*是个线性函数**

**不同的x，有不同的直线**

**每一个t的点都去取它的max，得到红色的这条线**

**f*和t也是convex的**

**![](https://img-blog.csdn.net/20171128111648100?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**



**来看一个实际的例子:**

**![](https://img-blog.csdn.net/20171128111738167?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**



**下面是f*(t)对应的这个例子的推导过程**

**![](https://img-blog.csdn.net/20171128111706234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**



## 3.Connection with GAN

**sup表示一个集合中的上确界,就是说任何属于该集合的元素都小于等于该值但是不一定有某个元素就正好等于sup的值,只能说明该集合有上界,这是它和max的区别,一般用在无限集中比较多相对应的下确界用inf表示**

**(f*)*=f**

**天外飞来一个D D任意挑?**

**![](https://img-blog.csdn.net/20171128111838137?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**



**继续推导**

**从p中sample x出来，计算D（x）的期望值**

**从q中sample x出来，计算F*(D(x))的期望值**

**目的就是计算某种f-divergence**

**V的定义过程，是如何定义出来的？**



**![](https://img-blog.csdn.net/20171128111903579?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**

**![](https://img-blog.csdn.net/20171128111921374?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**



**各式各样的divergence**

**![](https://img-blog.csdn.net/20171128111931490?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**



## 4.WGAN


**Wgan  (distance的变化)**

**Wasserstein**

**Earth mover’s distance 推土机距离 把土P推到Q处 一维空间的距离**

**![](https://img-blog.csdn.net/20171128220929468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**

**若P和Q分布比较复杂**

**把P的土铲成Q的样子，会有多种方案**

**不同的方法定义出来的distance会是不一样的（第二种方法舍近求远了）**

**采用搬土距离最小的plan**

**![](https://img-blog.csdn.net/20171128221009247?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**

**一个moving plan用一个矩阵来表示**

**矩阵每一个element:把P的土运送到Q处的值（量）**

**Row的和=P的几率（运送出去的土的量）**

**Column的和=Q的几率**

**穷举所有的plan，找出最小的B**
**![](https://img-blog.csdn.net/20171128221044284?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**


目标是使得Pg和Pdata重合（多次迭代）

例子说明：

JS divergence经迭代并没有越来越小

Earth mover’s distance越来越小，所以训练的时候会往后面收敛
![](https://img-blog.csdn.net/20171128221116221?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



D一定要是1-Lipschitz的一员

1-Lipschitz:输出的变化相对于输入的变化不会太大：绿色的那个是
![](https://img-blog.csdn.net/20171128221139350?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






为什么需要1-Lipschitz function的限制

下面的例子：Earth mover’s distance是d ,如果没有这个限制,实现目标函数的max,会是D(x1)=+无穷大and D(x2)=-无穷大

二元分类器（右下图）:绿色：learn的时候容易，梯度值不为0
![](https://img-blog.csdn.net/20171128221207273?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



如何求解optimization?

有了1-Lipschitz的限制，不能单纯用gradient 拉格朗日方法求解，这个是非凸优化，只能用迭代的方法求解，无法一步找到最优解，拉格朗日的条件是以变量的不等式形式表示的，但是这里是对某个函数进行约束


![](https://img-blog.csdn.net/20171128221240433?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20171128221254144?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



## 5.improved WGAN


1-Lipschitz function的变化不能太大，D(x) 对 x的梯度值的norm小于1

  加入penalty项 ，norm大于1的时候才有penalty

  使得找出来的function不一定满足但是会倾向于满足D(x) 对 x的梯度值的norm小于1 的条件

  对所有的x做积分不行，所有从所有的x中sample出一部分样本,x~Ppenalty,如何sample:

先从Pdata中sample出一个点，从Pg中sample出一个点，再从两点连线中间sample出一个点即可，为什么这样sample：论文说这样的实验结果是好的为什么是好的：因为train的过程中是使得Pg向着Pdata靠拢，所以只有蓝色部分的gradient是重要的，所以只对这一部分的gradient加以限制

![](https://img-blog.csdn.net/20171129101511122?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




![](https://img-blog.csdn.net/20171129101524274?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



**更近一步，improved WGAN真正做的事情，它不是取一个max,它是希望gradient接近1：**

**因为我们希望红色部分的D(x)越大越好，黄色部分的D(x)越小越好，那么蓝色部分的gradient越大越陡越好，那么就越接近1越好**

**![](https://img-blog.csdn.net/20171129101602687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**











