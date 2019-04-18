# 楔积(Wedge Procut) - 無名黑洞 - CSDN博客
2013年12月31日 23:28:41[wangxiaojun911](https://me.csdn.net/wangxiaojun911)阅读数：4346
由拓扑学中表面(Surface)的定义及实例引入楔积的概念。
[基础知识](http://blog.csdn.net/wangxiaojun911/article/details/6850809)
先看Surface在欧几里得空间内的定义：
![](https://img-blog.csdn.net/20131231232310656)
 所有在Omega中的点w（参数空间中的点）被记作：
![](https://img-blog.csdn.net/20131231232326609)
对应在R3中（欧几里德空间里的点）记作：
![](https://img-blog.csdn.net/20131231232346250)
 w的雅各比矩阵X_{\star}（w的一阶偏导数以一定次序排列成的矩阵）定义如下：
![](https://img-blog.csdn.net/20131231232412796)
 用雅各比矩阵可定义切平面为如X_{u}(w)和X_{v}(w) 张成的空间，如下：
![](https://img-blog.csdn.net/20131231232435406)
 其外积分Varpi定义如下(因为Omega是二维的，楔形运算符的结果在这里的意义是两个一阶倒数张成的面积向量,即结果是面法线向量,而模是面积)：
![](https://img-blog.csdn.net/20131231232559265)
上式的证明如下：
![](https://img-blog.csdn.net/20131231232611750)
我们知道，二阶行列式的绝对值的几何意义就是面积。所以1式表示了由X_{u} wedge X_{v}和X_{u} wedge X_{v}作为邻边的平行四边形的面积。证毕。
相应的，w点所在的小块平面的法线可以由Varpi直接推出，有：
![](https://img-blog.csdn.net/20131231232636078)
注意以上两式跟三维空间的向量积很相似。实际上，向量积即叉乘只是外积在三维空间的一个特例。有的解析几何文章直接把向量积叫做外积，容易引起混淆。或者可以说，在很多情况下，一些人认为外积和向量积根本就是一个东西的不同表述形式（面积向量和法线向量，或向量和其几何意义）。目前来说，理解楔积可以推广到任何维度的空间就可以了。
已知曲面上任意点位置的微小面积，就可以积分获得整块表面的面积A.
![](https://img-blog.csdn.net/20131231232656000)
高斯映射(Gauss Map,或normal map或spherical map)：把三维表面Omega映射到二维单位球面S2的映射。利用上述的单位法线N，有：
![](https://img-blog.csdn.net/20131231232712000)
**参考文献：**
Minimal Surfaces I Boundary Value Problems, Grundlehren der mathematischen Wissenschaften, 1992
            
