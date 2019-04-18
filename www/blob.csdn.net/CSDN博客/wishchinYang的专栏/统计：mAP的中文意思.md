# 统计：mAP的中文意思 - wishchinYang的专栏 - CSDN博客
2016年08月05日 18:05:29[wishchin](https://me.csdn.net/wishchin)阅读数：795
         原文链接：[http://blog.csdn.net/Lu597203933/article/details/41802155](http://blog.csdn.net/Lu597203933/article/details/41802155)
        之前写过一篇blog叫做机器学习实战笔记之非均衡分类问题：[http://blog.csdn.net/lu597203933/article/details/38666699](http://blog.csdn.net/lu597203933/article/details/38666699)  其中对Precision和Recall及ROC都有所讲解，其中区别在于Precision,Recall, F-score,
 MAP主要用于信息检索，而ROC曲线及其度量指标AUC主要用于分类和识别，ROC的详细介绍见上面的blog，这里的Precision、Recall和上篇blog的计算结果其实是一样的，只是这里从检索的角度进行理解。
一：Precision, Recall, F-score
       信息检索、分类、识别、翻译等领域两个最基本指标是召回率(Recall Rate)和准确率(Precision Rate------注意统计学习方法中precesion称为精确率，而准确率为accuracy 是分类正确的样本除以总样本的个数。)，召回率也叫查全率，准确率也叫查准率，概念公式:
              召回率(Recall)=  系统检索到的相关文件 / 系统所有相关的文件总数；；；亦即预测为真实正例除以所有真实正例样本的个数
             准确率(Precision)=  系统检索到的相关文件 / 系统所有检索到的文件总数；；；亦即等于预测为真实正例除以所有被预测为正例样本的个数
图示表示如下：
![](https://img-blog.csdn.net/20141208123859863?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTHU1OTcyMDM5MzM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
注意：
 ( 1 ) 准确率和召回率是互相影响的，理想情况下肯定是做到两者都高，但是一般情况下准确率高、召回率就低，召回率低、准确率高，当然如果两者都低，那是什么地方出问题了。
（2）如果是做搜索，那就是保证召回的情况下提升准确率；如果做疾病监测、反垃圾，则是保准确率的条件下，提升召回。
 所以，在两者都要求高的情况下，可以用F1(或者称为F-score)来衡量。计算公式如下：
                     F1= 2 * P * R / (P + R) 
(1) 公式基本上就是这样，但是如何算图1中的A、B、C、D呢？这需要人工标注，人工标注数据需要较多时间且枯燥，如果仅仅是做实验可以用用现成的语料。当然，还有一个办法，找个一个比较成熟的算法作为基准，用该算法的结果作为样本来进行比照，这个方法也有点问题，如果有现成的很好的算法，就不用再研究了。
(2)    形象直观的理解就是Recall要求的是全，宁可错杀一千，不能放过一人，这样Recall就会很高，但是precision就会最低。比如将所有的样本都判为正例，这是Recall就会等于1，但是很多负样本都被当做了正例，在某些情况就不适用，比如邮件过滤，此时要求的是准确率，不能是召回率，将所有的邮件都当做垃圾邮件肯定是最坏的结果(此时Recall=1)。
如果没有证据证明你有罪，那么你就有罪，召回率会很高；如果没有证据证明你有罪，那么你就无罪，召回率会很低，不全，很多人逍遥法外；
二：MAP
MAP:全称mean average precision(平均准确率)。mAP是为解决P，R，F-measure的单点值局限性的，同时考虑了检索效果的排名情况。
计算如下：
假设有两个主题，主题1有4个相关网页，主题2有5个相关网页。某系统对于主题1检索出4个相关网页，其rank分别为1, 2, 4, 7；对于主题2检索出3个相关网页，其rank分别为1,3,5。对于主题1，平均准确率为(1/1+2/2+3/4+4/7)/4=0.83。对于主题 2，平均准确率为(1/1+2/3+3/5+0+0)/5=0.45。则MAP=(0.83+0.45)/2=0.64。”
参考文献：
1：http://blog.csdn.net/marising/article/details/6543943信息检索（IR）的评价指标介绍 - 准确率、召回率、F1、mAP、ROC、AUC
2：http://blog.sina.com.cn/s/blog_662234020100pozd.htmlMAP(Mean Average Precision)
3：http://wenku.baidu.com/view/ef91f011cc7931b765ce15ec.html
