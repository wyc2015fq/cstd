# PageRank算法 - wsp_1138886114的博客 - CSDN博客





2018年07月10日 18:03:43[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：77
所属专栏：[机器学习算法](https://blog.csdn.net/column/details/24447.html)











- - - - - - [PageRank算法](#pagerank算法)
- [终止点问题](#终止点问题)
- [陷阱问题](#陷阱问题)
- [算法改进](#算法改进)
- [算法总结](#算法总结)









###### PageRank算法

```
In：每个球代表一个网页；球的大小反应了网页的pagerank值的大小；
Out：指向网页B和网页E的链接很多，所以B和E的pagerank值较高；
虽然很少有网页指向C，但是最重要的网页B指向了C，所以C的pagerank值比E还要大。 (图1所示：)
```

![这里写图片描述](https://img-blog.csdn.net/20180710165303731?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```
图2将网页之间的关系用转移矩阵 M 所示：（如上图） 

如果一个网页有k条出链，那么跳转任意一个出链上的概率是1/k ；
如果用n表示网页的数目，则转移矩阵M是一个n*n的方阵；
如果网页j有k个出链，那么对每一个出链指向的网页i，有M[i][j]=1/k，
而其他网页的M[i][j]=0 

假设上网者在每一个网页的概率都是相等的，
    即1/n，于是初试的概率分布就是一个所有值都为1/n的n维列向量V0，用V0去右乘转移矩阵M，
    就得到了第一步之后上网者的概率分布向量MV0,（nXn）*(nX1)依然得到一个nX1的矩阵
```

![这里写图片描述](https://img-blog.csdn.net/20180710171537984?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

###### 终止点问题

```
要满足收敛性，需要具备一个条件：
图是强连通的，即从任意网页可以到达其他任意网页 

互联网上的网页不满足强连通的特性，因为有一些网页不指向任何网页，如果按照上面的计算， 
上网者到达这样的网页后便走投无路、四顾茫然，
导致前面累计得到的转移概率被清零，这样下去，最终的得到的概率分布向量所有元素几乎都为0。
```

![这里写图片描述](https://img-blog.csdn.net/20180710172754662?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

###### 陷阱问题

```
上网者跑到C网页后，就像跳进了陷阱，陷入了漩涡，再也不能从C中出来，
这使得其他网页的概率分布值为0，从而整个网页排名就失去了意义。（如上图 网页漩涡）
```

###### 算法改进

```
解决终止点问题和陷阱问题。 
    地址栏输入而跳转到各个网页的概率是1/n；
    上网者每一步查看当前网页的概率为a，那么从浏览器地址栏跳转的概率为(1-a)；
```



$V' = \alpha MV+(1-\alpha)e$

```
[注]采用矩阵相乘，不断迭代，直到迭代前后概率分布向量的值变化不大，
一般迭代到30次以上就收敛了,取e为所有分量都为 1/n 的列向量。 

真的web结构的转移矩阵非常大，目前的网页数量已经超过100亿， 
    转移矩阵是100亿*100亿的矩阵，直接按矩阵乘法的计算方法不可行，
    需要借助Map-Reduce的计算方式来解决
```

![这里写图片描述](https://img-blog.csdn.net/20180710174118818?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

###### 算法总结

```
PR值：决定了网页在搜索结果中的排名
```

![这里写图片描述](https://img-blog.csdn.net/20180710175640225?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```
在一般情况下，一个网页的PR值计算如下：
```



$PR(p_i) = \alpha \sum_{p_i \in M_{p_i}} \frac{PR(p_j)}{L(p_j)} + \frac{1-\alpha}{N}$

```
其中Mpi是所有对pi网页有出链的网页集合，L(pj)是网页pj的出链数目，N是网页总数，α 一般取0.85。
根据上面的公式，我们可以计算每个网页的PR值，在不断迭代趋于平稳的时候，即为最终结果。
```

**PageRank算法的缺点**

```
这是一个天才的算法，原理简单但效果惊人。而，PageRank算法还是有一些弊端。
    第一，没有区分站内导航链接。很多网站的首页都有很多对站内其他页面的链接，称为站内导航链接。 
         这些链接与不同网站之间的链接相比，肯定是后者更能体现PageRank值的传递关系。

    第二，没有过滤广告链接和功能链接（例如常见的“分享到微博”）。这些链接通常没有什么实际价值， 
         前者链接到广告页面，后者常常链接到某个社交网站首页。

    第三，对新网页不友好。一个新网页的一般入链相对较少，即使它的内容的质量很高， 
          要成为一个高PR值的页面仍需要很长时间的推广。
```



