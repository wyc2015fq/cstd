
# 结构化数据转换方式之一：box-cox转换 - 素质云笔记-Recorder... - CSDN博客

2017年09月06日 11:42:50[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：13269



> 之前在《
> [笔记︱信用风险模型（申请评分、行为评分）与数据准备（违约期限、WOE转化）](http://blog.csdn.net/sinat_26917383/article/details/51721107)
> 》中提到过WOE转换，WOE转换=分箱法=Logit值，与等深、等宽不同是根据被解释变量来重新定义一个WOE值
**笔者将其定位于对自变量的数据转换。**
**现在来看看对于因变量的数据转换：BOX-COX转换。**
内容主要参考交大的课件：[BoxCox-变换方法及其实现运用.pptx](https://wenku.baidu.com/view/96140c8376a20029bd642de3.html)
### 优势：
线性回归模型满足线性性、独立性、方差齐性以及正态性的同时，又不丢失信息，此种变换称之为Box—Cox变换。
误差与y相关，不服从正态分布，于是给线性回归的最小二乘估计系数的结果带来误差
使用Box-Cox变换族一般都可以保证将数据进行成功的正态变换，但在二分变量或较少水平的等级变量的情况下，不能成功进行转换，此时，我们可以考虑使用广义线性模型，如LOGUSTICS模型、Johnson转换等。
Box-Cox变换后，残差可以更好的满足正态性、独立性等假设前提，降低了伪回归的概率
**其中：**
在一些情况下（P值<0.003）上述方法很难实现正态化处理，所以优先使用Box-Cox转换，但是当P值>0.003时两种方法均可，优先考虑普通的平方变换。
**此时的检验步骤为：**先对数据进行正态性检验 -> 观察检验的P值 -> 根据P值挑选合适的box-cox转换函数
---常规的经济学转换方式：

---log，对数转换，是使用最多的（数据必须大于0）

---还有：

---平方根转换

---倒数转换

---平方根后取倒数

---平方根后再取反正弦

---幂转换

![这里写图片描述](https://img-blog.csdn.net/20170906113630012?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---Box-Cox变换的正态变换：

---数据不比大于>0

![这里写图片描述](https://img-blog.csdn.net/20170906113746311?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---没有Box-Cox变换的回归：

![这里写图片描述](https://img-blog.csdn.net/20170906114029990?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---Box-Cox变换之后的回归：

![这里写图片描述](https://img-blog.csdn.net/20170906114036321?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---**公众号“素质云笔记”定期更新博客内容：**

---![这里写图片描述](https://img-blog.csdn.net/20180226155348545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


