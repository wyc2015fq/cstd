# Approaches for evaluate the accuracy of classifier for streaming data - 一个跳popping的quant的博客 - CSDN博客





2018年09月04日 00:03:55[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：797标签：[holdout](https://so.csdn.net/so/search/s.do?q=holdout&t=blog)
个人分类：[concept drift](https://blog.csdn.net/FrankieHello/article/category/7961323)









在传统的batch learning中，在评估模型训练结果好坏的时候，常用cross-validation（交叉验证）的方法，如K-fold cross-validation。但是当面临stream data learning时，数据的有限性就暴露出了一些挑战。一种解决方式就是在每个时间点taking snapshot（快照），然后观察模型的进步。而如何建立这个快照有下面几种方法。

### Holdout

在介绍holdout时，先说一下batch learning中的cross-validation。以常用的K-fold cross-validtion为例，它的过程是：

初始将数据样本采样分割为K个子样本，保留一个单独的子样本作为验证模型的数据，其他的K-1个样本用来训练模型。然后这样重复K次，直至K个子样本都作为一遍验证数据，再将得到的K个结果进行平均或者其他的方式来得到最终的结果。

但使用交叉验证太费时间的时候，holdout方法常被采用。holdout验证则只是从最初的样本中选出部分进行训练，剩余的部分作为验证数据，一般来说少于原本样本三分之一的数据被选为验证数据。并且，当训练集和测试集被pre-defined时，使用holdout是最有效的，不同的研究方法可以直接得到结果。

### Interleaved Test-Then-Train or Prequential

每个单独的样本个体在被用来训练之前，都可以作为测试。这样的话，准确率就会一直不断被更新。这种方式的好处就是可以让模型不断被看不到的数据来测试，并且不需要holdout集合用来测试，还可以最大限度的利用已有的数据。这样，还可以确保准确率曲线的平滑性。

另外，由于数据流分类是一个相对较新的领域，这类评价并没有像传统的批学习那样得到很好的研究和建立。



ref:[https://moa.cms.waikato.ac.nz/tutorial-1-introduction-to-moa/](https://moa.cms.waikato.ac.nz/tutorial-1-introduction-to-moa/)



