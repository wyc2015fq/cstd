
# word2vec 中的数学原理详解 - peghoty - 博客园






# [word2vec 中的数学原理详解](https://www.cnblogs.com/peghoty/p/3857839.html)
word2vec 是 Google 于 2013 年开源推出的一个用于获取 word vector 的工具包，它简单、高效，因此引起了很多人的关注。由于 word2vec 的作者 Tomas Mikolov 在两篇相关的论文 [3,4] 中并没有谈及太多算法细节，因而在一定程度上增加了这个工具包的神秘感。一些按捺不住的人于是选择了通过解剖源代码的方式来一窥究竟。
第一次接触 word2vec 是 2013 年的 10 月份，当时读了复旦大学郑骁庆老师发表的论文[7]，其主要工作是将 SENNA 的那套算法（[8]）搬到中文场景。觉得挺有意思，于是做了一个实现（可参见[20]），但苦于其中字向量的训练时间太长，便选择使用 word2vec 来提供字向量，没想到中文分词效果还不错，立马对 word2vec 刮目相看了一把，好奇心也随之增长。
后来，陆陆续续看到了 word2vec 的一些具体应用，而 Tomas Mikolov 团队本身也将其推广到了句子和文档（[6]），因此觉得确实有必要对 word2vec 里的算法原理做个了解，以便对他们的后续研究进行追踪。于是，沉下心来，仔细读了一回代码，算是基本搞明白里面的做法了。第一个感觉就是，“明明是个很简单的浅层结构，为什么会被那么多人沸沸扬扬地说成是 Deep Learning 呢？”
解剖 word2vec 源代码的过程中，除了算法层面的收获，其实编程技巧方面的收获也颇多。既然花了功夫来读代码，还是把理解到的东西整理成文，给有需要的朋友提供点参考吧。
在整理本文的过程中, 和深度学习群的群友@**北流浪子**（[15,16]）进行了多次有益的讨论，在此表示感谢。另外，也参考了其他人的一些资料，都列在参考文献了，在此对他们的工作也一并表示感谢。
![](https://images0.cnblogs.com/i/601094/201407/210939080257860.png)![](https://images0.cnblogs.com/i/601094/201407/210939186669010.png)![](https://images0.cnblogs.com/i/601094/201407/210939289792259.png)![](https://images0.cnblogs.com/i/601094/201407/210939427295392.png)![](https://images0.cnblogs.com/i/601094/201407/210939495417944.png)![](https://images0.cnblogs.com/i/601094/201407/210939559474854.png)![](https://images0.cnblogs.com/i/601094/201407/210940031668348.png)![](https://images0.cnblogs.com/i/601094/201407/210940135578340.png)![](https://images0.cnblogs.com/i/601094/201407/210940241504504.png)![](https://images0.cnblogs.com/i/601094/201407/210940344166468.png)![](https://images0.cnblogs.com/i/601094/201407/210940413072062.png)![](https://images0.cnblogs.com/i/601094/201407/210940479797657.png)![](https://images0.cnblogs.com/i/601094/201407/210940541199582.png)![](https://images0.cnblogs.com/i/601094/201407/210941011502134.png)![](https://images0.cnblogs.com/i/601094/201408/211030184871235.png)![](https://images0.cnblogs.com/i/601094/201408/211030267845170.png)![](https://images0.cnblogs.com/i/601094/201407/210941187135336.png)![](https://images0.cnblogs.com/i/601094/201407/210941265411244.png)![](https://images0.cnblogs.com/i/601094/201407/210941528228981.png)![](https://images0.cnblogs.com/i/601094/201407/210942007757618.png)![](https://images0.cnblogs.com/i/601094/201407/210942069794056.png)![](https://images0.cnblogs.com/i/601094/201407/210942204633204.png)![](https://images0.cnblogs.com/i/601094/201407/210942335415708.png)![](https://images0.cnblogs.com/i/601094/201407/210942417449802.png)![](https://images0.cnblogs.com/i/601094/201407/210942483858938.png)![](https://images0.cnblogs.com/i/601094/201407/210942542131894.png)![](https://images0.cnblogs.com/i/601094/201407/210943002136860.png)![](https://images0.cnblogs.com/i/601094/201407/210943065108568.png)![](https://images0.cnblogs.com/i/601094/201408/202353566287394.png)![](https://images0.cnblogs.com/i/601094/201408/202354083152133.png)![](https://images0.cnblogs.com/i/601094/201408/202354195492799.png)![](https://images0.cnblogs.com/i/601094/201408/202354295039804.png)![](https://images0.cnblogs.com/i/601094/201408/202354393159470.png)![](https://images0.cnblogs.com/i/601094/201408/202354497998507.png)![](https://images0.cnblogs.com/i/601094/201408/202354597062499.png)![](https://images0.cnblogs.com/i/601094/201408/202355103931065.png)![](https://images0.cnblogs.com/i/601094/201408/202355175818326.png)![](https://images0.cnblogs.com/i/601094/201407/210943549634934.png)![](https://images0.cnblogs.com/i/601094/201407/210944018549527.png)![](https://images0.cnblogs.com/i/601094/201407/210944084166623.png)![](https://images0.cnblogs.com/i/601094/201407/210944163383504.png)![](https://images0.cnblogs.com/i/601094/201407/210944219639285.png)![](https://images0.cnblogs.com/i/601094/201407/210957135101739.png)


![](https://images0.cnblogs.com/i/601094/201407/210944317759619.png)![](https://images0.cnblogs.com/i/601094/201408/202355310498109.png)
作者：peghoty
出处:[http://blog.csdn.net/itplus/article/details/37969519](http://blog.csdn.net/itplus/article/details/37969519)
欢迎转载/分享, 但请务必声明文章出处.





