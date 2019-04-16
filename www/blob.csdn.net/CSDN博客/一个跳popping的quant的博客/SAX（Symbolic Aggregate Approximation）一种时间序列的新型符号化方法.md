# SAX（Symbolic Aggregate Approximation）一种时间序列的新型符号化方法 - 一个跳popping的quant的博客 - CSDN博客





2018年07月13日 11:01:21[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：2871
所属专栏：[时间序列分析](https://blog.csdn.net/column/details/34308.html)









### Introduction

简言之，SAX算法就是将时间序列进行符号化表示。

这个算法最初是由[Lin et al.](http://www.cs.ucr.edu/~eamonn/SAX.htm)提出的，它扩展了基于PAA的方法并继承了原始方法的简单和低复杂度的特点，同时在范围查询的过程中提供了令人满意的灵敏度和可选择性。除此之外，符号化的特征表示为现在存在的丰富的数据结构和字符串处理算法（哈希、正则表达式、模式匹配、后缀树和语法推断）开启了一扇大门。

### The algorithm

SAX将一段长度为n的时间序列X转换为一段任意时间长度的字符串。这个算法包括两个步骤：

(1)、将原始时间序列数据转换为PAA特征表示。

(2)、将PAA数据转换为字符串。

PAA的使用带来了简单有效的降维性，同时也提供了重要的下边界属性。使用查表的方法将PAA系数转换为字母的计算效率也很高，Lin et al.证明了符号距离的收缩性。

将一段时间序列的PAA特征表示离散化到SAX中，得到的符号与时间序列的特征相对应，具有相同的概率。对原始算法的作者所使用的各种时间序列的数据集进行了广泛而又严格的分析，结果表明z-normalized的时间序列的值遵循正态分布。通过使用它的属性，可以很容易的在正常曲线下使用查表法来确定直线坐标，分割高斯曲线下的区域。

这些行的x坐标在SAX上下文中称为断点（breakpoints），列表中的断点将在N(0,1)分布中的数据划分到了a个相同的区域。通过将相应的字母符号对应到每个区间，矢量的转换PAA系数C~到字符串C^的实现如下：

![](https://img-blog.csdn.net/20180713103038552?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





SAX通过拓展欧式距离和PAA距离引入了度量字符串之间距离的新指标。这个函数返回两个原始时间序列Q^和C^的字符串特征之间的最小距离。

![](https://img-blog.csdn.net/20180713103310926?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

如下表所示，使用查表的方式实现dist函数，并计算每个单元格的cell(r, c)的值：

![](https://img-blog.csdn.net/20180713103907795?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

四个字母的查找表


||a|b|c|d|
|----|----|----|----|----|
|a|0|0|0.67|1.34|
|b|0|0|0|0.67|
|c|0.67|0|0|0|
|d|1.34|0.67|0|0|

如Li等人所示，这个SAX距离指标的下界要低于PAA距离指标的下界：

![](https://img-blog.csdn.net/20180713104134131?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

Ding等人对SAX下界进行了详细的检验，发现了其精度优于bursty（非周期）数据集的谱分解方法。

### SAX primer

#### 1 时间序列数据

我们将用到下边的时间序列来作为例子（ts1和ts2的欧式距离是11.4）

```python
> ts1=c(2.02, 2.33, 2.99, 6.85, 9.20, 8.80, 7.50, 6.00, 5.85, 3.85, 4.85, 3.85, 2.22, 1.45, 1.34)
> ts2=c(0.50, 1.29, 2.58, 3.83, 3.25, 4.25, 3.83, 5.63, 6.44, 6.25, 8.75, 8.83, 3.25, 0.75, 0.72)
> dist(rbind(ts1,ts2), method = "euclidean")
         ts1
ts2 11.42126
```

![](https://img-blog.csdn.net/20180713104728118?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

我们将把它转换为长度为9的字符串，它的字母来自大小为4的字母表。

#### 2 Z-normalization

在用SAX转换字符串之前，我们将对数据进行Z-normalization处理。

```python
znorm <- function(ts){
  ts.mean <- mean(ts)
  ts.dev <- sd(ts)
  (ts - ts.mean)/ts.dev
}

ts1_znorm=znorm(ts1)
ts2_znorm=znorm(ts2)
```

![](https://img-blog.csdn.net/20180713105051644?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

PAA遵循这个标准过程。

#### 3 PAA转换

PAA

```python
paa <- function(ts, paa_size){
  len = length(ts)
  if (len == paa_size) {
    ts
  }
  else {
    if (len %% paa_size == 0) {
      colMeans(matrix(ts, nrow=len %/% paa_size, byrow=F))
    }
    else {
      res = rep.int(0, paa_size)
      for (i in c(0:(len * paa_size - 1))) {
        idx = i %/% len + 1# the spot
        pos = i %/% paa_size + 1 # the col spot
        res[idx] = res[idx] + ts[pos]
      }
      for (i in c(1:paa_size)) {
        res[i] = res[i] / len
      }
      res
    }
  }
}
paa_size=9
s1_paa = paa(ts1_znorm,paa_size)
s2_paa = paa(ts2_znorm,paa_size)
```

![](https://img-blog.csdn.net/20180713105420184?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

#### 4 将PAA值转换为字母

如上表所示，我是用了四个字母（a, b, c, d）。这四个字母的分割线如下图中蓝色的线所示。

![](https://img-blog.csdn.net/20180713105546924?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

SAX通过9点PAA将ts1转换为字符串abddccbaa。

SAX通过9点PAA将ts2转换为字符串abbccddba。

SAX距离：0 + 0 + 0.67 + 0 + 0 + 0 + 0.67 + 0 + 0 = 1.34

在图中，橙色描绘了被计数的符号之间的距离（它们在表格中不是相邻的）。




原文链接 [https://jmotif.github.io/sax-vsm_site/morea/algorithm/SAX.html](https://jmotif.github.io/sax-vsm_site/morea/algorithm/SAX.html)




