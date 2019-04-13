
# 特征分析之SVD - OraYang的博客 - CSDN博客

2017年11月09日 11:56:36[OraYang](https://me.csdn.net/u010665216)阅读数：488标签：[svd																](https://so.csdn.net/so/search/s.do?q=svd&t=blog)[奇异值																](https://so.csdn.net/so/search/s.do?q=奇异值&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=svd&t=blog)个人分类：[Machine_learning																](https://blog.csdn.net/u010665216/article/category/7066495)
所属专栏：[机器学习](https://blog.csdn.net/column/details/16605.html)



## 引言
前面我们分享[降维分析之PCA分析及实现](http://blog.csdn.net/u010665216/article/details/78485442)，说PCA除了应用在数据降维上，还可用于特征分析。今天我们就来分享个新的特征分析的方法，叫做奇异值分解(Singular Value Decomposition，SVD)。
## SVD背后的数学原理
我们如果在Google搜索引擎中输入SVD这个单词，会弹出好多图片，如下其中一幅：
![svd](https://img-blog.csdn.net/20171109112436906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[；如果我们在Baidu搜索引擎中搜索SVD的话，百度百科的解释是这样的：SVD德拉贡诺夫狙击步枪的英文缩写。哈哈哈~，咱们这次可不是给大家来个军事武器普及，言归正传，我们来看看SVD背后的数学原理。](https://img-blog.csdn.net/20171109112436906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### SVD的起源
奇异值分解技术(简称SVD)的历史很长，也有些令人惊讶。它开始于社会科学和智力测试。早期的情报研究人员指出，用来测量智力不同方面的测试，比如语言和空间，往往是紧密相关的。
### 矩阵分解
矩阵分解可以将原矩阵表示成新的易于处理的形式，这个新形式是两个或多个矩阵的乘积。最常见的一种矩阵分解技术就是SVD。
奇异值分解是一种将一个矩阵分解成三个矩阵的方法：

$$
Data_{m\times n}=U_{m\times m}\Sigma_{m\times n}V^T_{n\times n}
$$
上述分解会构建出一个矩阵$\Sigma$，该矩阵只有对角元素，其他元素均为0。$\Sigma$的对角元素是从大到小排列的。这些对角元素称为奇异值，它们对应原始数据集中的重要特征。
### python实现
SVD在Numpy中有现成的工具箱linalg。使用起来很简单：
```python
U，Sigma,V = linalg.svd(Data)
```
## 总结
SVD的数学原理很简单，并且实现也不复杂，但是SVD在很多领域有着极其广泛的应用。最典型的就是推荐系统，博主准备找个合适的时间来做个应用SVD的demo，然后给大家分享。

