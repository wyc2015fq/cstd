
# Yusuke Sugomori 的 C 语言 Deep Learning 程序解读 - peghoty - CSDN博客


2013年06月12日 10:24:04[皮果提](https://me.csdn.net/peghoty)阅读数：14992



最近开始在学习 Deep Learning 的基础知识，找了些文献来读，觉得有些晦涩。目前对于 DBN 的算法框架已经有个大概了解，但具体实现细节还不是太清楚，在网上搜到了日本学者[Yusuke Sugomori](http://yusugomori.com/)在 Github 上的一个[deeplearning](https://github.com/yusugomori/DeepLearning)项目，认真读了其 C 语言的版本，感觉收获蛮大，特将程序翻译成数学描述，希望对正在读或者将要读这个代码的朋友有些帮助。
注意，本文只是客观翻译程序，并作一些相关的评注，个人觉得这个程序与目前流行的 DBN 算法有较大差别，不能直接用来实战。学习 Deep Learning 的朋友可将其作为入门，特别是用来理解 Contrastive Divergence 和 Gibbs Sampling 比较有帮助。
![](https://img-blog.csdn.net/20130612174746062)
![](https://img-blog.csdn.net/20130612174804187)
![](https://img-blog.csdn.net/20130612174824421)
![](https://img-blog.csdn.net/20130612174845953)
![](https://img-blog.csdn.net/20130612174914171)
![](https://img-blog.csdn.net/20130612174942765)
![](https://img-blog.csdn.net/20130612175008390)

![](https://img-blog.csdn.net/20130612175033890)
![](https://img-blog.csdn.net/20130708235012296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGVnaG90eQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20130708235259640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGVnaG90eQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20130708235657109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGVnaG90eQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**补充**：
文中描述的算法在框架上问题不大，但在一些细节处理上可能有些问题，例如，关于 RBM 中的参数刷新公式， fine-tuning 过程中并没有将之前的层联级微调等，望读者知晓。另外，关于 RBM 算法，我后来又花了点功夫学习了一下，并整理成系列博客（见下面的链接），供大家参考。

**目录链接**
（一）预备知识
[（二）网络结构](http://blog.csdn.net/itplus/article/details/19168967)
[（三）能量函数和概率分布](http://blog.csdn.net/itplus/article/details/19168989)
（四）对数似然函数
[（五）梯度计算公式](http://blog.csdn.net/itplus/article/details/19207371)
[（六）对比散度算法](http://blog.csdn.net/itplus/article/details/19408143)
[（七）RBM 训练算法](http://blog.csdn.net/itplus/article/details/19408701)
（八）RBM 的评估
作者: peghoty
出处:[
http://blog.csdn.net/peghoty/article/details/9079973 ](http://blog.csdn.net/peghoty/article/details/9079973)
欢迎转载/分享, 但请务必声明文章出处.



