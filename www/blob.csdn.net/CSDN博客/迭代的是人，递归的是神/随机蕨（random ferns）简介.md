# 随机蕨（random ferns）简介 - 迭代的是人，递归的是神 - CSDN博客





2012年03月15日 21:24:19[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：22180标签：[random																[存储																[linux																[语言																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)
个人分类：[机器学习](https://blog.csdn.net/yang_xian521/article/category/1087510)





之前看到的TLD代码中里机器学习的部分就是用的这个方法。不过TLD作者在论文中实现使用的是random forest，不过归到底，random forest和random ferns都是一种东西。

英文比较好的，推荐链接[http://cvlab.epfl.ch/alumni/oezuysal/ferns.html](http://cvlab.epfl.ch/alumni/oezuysal/ferns.html)，ferns方法的提出者在这里面介绍的已经很详细了。这个链接也是之前交流tld时一个网友提供的，感谢一下。

只想看中文的同志下面跟着我继续啃骨头吧，虽然我是明白了这个方法，但本人语言表述能力还有待加强啊，于是在网上搜到一篇母校的文章，以下基本摘抄自陈冰博士的paper。

ferns方法就是样本分类情况用ci，i = 1,2，。。。H表示，设fj，j = 1，2，。。。N为输入样本的二元特征集，则样本的所属类别为

![](http://hi.csdn.net/attachment/201203/15/0_1331816241zr1A.gif)


式中，C表示类的随机变量，则有

![](http://hi.csdn.net/attachment/201203/15/0_1331816429W9wr.gif)


设先验概率P（C）为均匀分布，（2）式分母部分与类别无关，则（1）简化为

![](http://hi.csdn.net/attachment/201203/15/0_13318165305hw9.gif)


这里的二元特征fj的值取决于样本中在分类器训练时随机生成的两像素位置dj1和dj2的灰度大小Idj1，Idj2比较的结果，即

![](http://hi.csdn.net/attachment/201203/15/0_1331816650q8yU.gif)


由于fj的形式很简单，因此需要足够多的特征确保分类结果，但求解需要的储存量为H*2^N字节。为降低存储量，并保证fj之间具有足够的相关性，并假设不同组的二元特征之间相互独立，组内二元特征之间具有相关性，将这些组定义为ferns feature。则（3）式的条件概率近似为

![](http://hi.csdn.net/attachment/201203/15/0_1331816944ugKO.gif)


（6）式的存储量只有M*2^S。

以上这些简单的公式都很好理解，我也很讨厌介绍东西的时候用很多公式，不过这些公式真的很简单，如果你看了这些还觉得理解的不深刻，去英文链接里看看那些图吧，会很轻松搞定的，最后还是惯例，附上ferns的代码，不过不是我写的啦，还是方法提出人的作品，老外真好，写的方法都敢拿出来和大家分享。[http://cvlab.epfl.ch/software/ferns/index.php](http://cvlab.epfl.ch/software/ferns/index.php)。比较遗憾的是，这个代码是在linux环境下编译的，不过肯定难不倒各位~](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=random&t=blog)




