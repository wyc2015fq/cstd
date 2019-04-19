# 选择支持向量机(SVM)核函数 - 数据之美的博客 - CSDN博客
2018年01月02日 16:03:12[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：563
版权声明：本文为博主原创文章，未经博主允许不得转载。
目录[(?)](http://blog.csdn.net/qq_33232071/article/details/50949410#)[[-]](http://blog.csdn.net/qq_33232071/article/details/50949410#)
- [2-dimensions](http://blog.csdn.net/qq_33232071/article/details/50949410#t0)- [多项式核函数](http://blog.csdn.net/qq_33232071/article/details/50949410#t1)
SVM核函数通常有四种： 
1. Linear 
2. Polynomial 
3. Gaussian (RBF) 
4. Sigmoid/Logistic 
不知为何，RBF最常用
- 一般情况下，给定随意的数据集，你不知道该用什么样的核函数，通常先用简单的映射方式，然后再一步步的复杂化。
- 如果数据集是线性可分的，用linear当然足够；如果数据集线性不可分，这个核函数就显然不行了
# 2-dimensions
- 为了描述简单和可视化，我们使用二维数据集 
下图是iris数据集使用linear的SVM分类器的分类结果 
![这里写图片描述](https://qph.is.quoracdn.net/main-qimg-2d621c756e95ac3cdd97bd79eec0b700?convert_to_webp=true)
结果还不错！下图是iris数据集使用RBF的SVM分类器的分类结果 
![这里写图片描述](https://qph.is.quoracdn.net/main-qimg-7c915c75f60a6457b04479c6e2d9eeb4?convert_to_webp=true)
OK，很显然，两种核函数都能让我们接受，具有很强的说服力。这时候通常选择简单的那个，即线性核函数，因为不论是参数个数上还是复杂度上，还是映射计算上，线性核函数比RBF强，另外RBF可能会导致过拟合。
好吧，似乎上面说的对linear很有好感。但事实上，如果要一句话总结如何选择核函数，那就是—dataset。毫无疑问，如果是下图的数据，再用linear核函数，显然是不行的。 
![这里写图片描述](https://qph.is.quoracdn.net/main-qimg-02673095a53496a0e3e4f19cabf6f661?convert_to_webp=true)
下面是RBF核函数的结果，很精妙！ 
![这里写图片描述](https://qph.is.quoracdn.net/main-qimg-92c366e602444d991fc9d1073ae321b6?convert_to_webp=true)
告一段落
## 多项式核函数
- 在任何情况下，都很少用多项式核，效率不高，结果也不优于RBF
- 于是现在得到一个结论：线性可分时用linear，线性不可分时用RBF核。 
![这里写图片描述](https://qph.is.quoracdn.net/main-qimg-f7a6d72518f0114bfbd303c4608f7892?convert_to_webp=true)
事实上，更朴实一点的说法，RBF通过映射到高维的最终目标还是线性可分。
