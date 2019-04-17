# Minkowski-p范数与Frobenius弗罗贝尼乌斯范数 - Machine Learning with Peppa - CSDN博客





2018年04月20日 16:17:40[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：792










**Minkowski-P范数**

两个n维变量a(x11,x12,…,x1n)与b(x21,x22,…,x2n)间的闵可夫斯基距离(其中p是一个变参数)定义为：


- 当p=1时，就是曼哈顿距离
- 当p=2时，就是欧氏距离
- 当p→∞时，就是切比雪夫距离


根据变参数的不同，闵氏距离可以表示一类的距离。​此距离又叫做P范数

p-范数：![](https://img-blog.csdn.net/20130624112135593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdF9sYQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，即向量元素绝对值的p次方和的1/p次幂，matlab调用函数norm(x, p)。




**Frobenius-弗罗贝尼乌斯范数**

### 弗罗贝尼乌斯范数（Frobenius norm）是P范数在P=2时的一种特例，在希尔伯特空间中又叫做[希尔伯特-施密特范数](http://zh.wikipedia.org/wiki/%E5%B8%8C%E5%B0%94%E4%BC%AF%E7%89%B9-%E6%96%BD%E5%AF%86%E7%89%B9%E7%AE%97%E5%AD%90)（ Hilbert–Schmidt norm），这个范数可用不同的方式定义：


![\|A\|_F=\sqrt{\sum_{i=1}^m\sum_{j=1}^n |a_{ij}|^2}=\sqrt{\operatorname{trace}(A^{{}^*} A)}=\sqrt{\sum_{i=1}^{\min\{m,\,n\}} \sigma_{i}^2}](http://upload.wikimedia.org/math/3/9/a/39a998ba0e24ee854346ed2575cc1de0.png)
这里 *A** 表示 *A* 的[共轭转置](http://zh.wikipedia.org/wiki/%E5%85%B1%E8%BD%AD%E8%BD%AC%E7%BD%AE)，*σi* 是 *A* 的[奇异值](http://zh.wikipedia.org/wiki/%E5%A5%87%E5%BC%82%E5%80%BC%E5%88%86%E8%A7%A3)，并使用了[迹函数](http://zh.wikipedia.org/wiki/%E8%BF%B9)。弗罗贝尼乌斯范数与 *K**n* 上欧几里得范数非常类似，来自所有矩阵的空间上一个[内积](http://zh.wikipedia.org/wiki/%E5%86%85%E7%A7%AF)。

弗罗贝尼乌斯范范数是服从乘法的且在[数值线性代数](http://zh.wikipedia.org/wiki/%E6%95%B0%E5%80%BC%E7%BA%BF%E6%80%A7%E4%BB%A3%E6%95%B0)中非常有用。这个范数通常比诱导范数容易计算。




