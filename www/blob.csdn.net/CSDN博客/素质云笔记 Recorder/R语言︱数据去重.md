
# R语言︱数据去重 - 素质云笔记-Recorder... - CSDN博客

2016年04月19日 10:09:44[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：26539所属专栏：[R的数据操作与清洗](https://blog.csdn.net/column/details/13587.html)




**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)
———————————————————————————
笔者寄语：unique对于一个向量管用，对于matrix、data frame那些就不管用了。（谢益辉）
查看重复的方式，有点像分类变量个数一样，[unique()](http://blog.csdn.net/sinat_26917383/article/details/51182358)或者 table()  都是很好的方式去检测。

1、unique函数

`> rt
    年 月 公司名 利率
1 2000  1      A    a
2 2000  1      A    a
3 2001  2      A    b
4 2001  3      A    c
5 2000  1      B    d
6 2000  2      B    e
7 2000  2      B    e
> unique(rt)
    年 月 公司名 利率
1 2000  1      A    a
3 2001  2      A    b
4 2001  3      A    c
5 2000  1      B    d
6 2000  2      B    e
> unique(rt,fromLast=TRUE)
    年 月 公司名 利率
2 2000  1      A    a
3 2001  2      A    b
4 2001  3      A    c
5 2000  1      B    d
7 2000  2      B    e`以上是根据你的数据得到的，R中默认的是fromLast=FALSE,即若样本点重复出现，则取首次出现的；
否则去最后一次出现的。列名不变，去掉重复样本值之后的行名位置仍为原先的行名位置。

2、duplicated函数
在数据框中应用较为广泛。
`#源数据
> data.set
   Ensembl.Gene.ID Gene.Biotype Chromosome.Name Gene.Start..bp. Gene.End..bp.
1  ENSG00000236666    antisense                  22        16274560      16278602
2  ENSG00000236666    antisense                  22        16274560      16278602
3  ENSG00000234381   pseudogene              22        16333633      16342783
4  ENSG00000234381   pseudogene              22        16333633      16342783
5  ENSG00000234381   pseudogene              22        16333633      16342783
6  ENSG00000234381   pseudogene              22        16333633      16342783
7  ENSG00000234381   pseudogene              22        16333633      16342783
8  ENSG00000234381   pseudogene              22        16333633      16342783
9  ENSG00000234381   pseudogene              22        16333633      16342783
10 ENSG00000224435   pseudogene             22        16345912      16355362
#构建一个布尔向量，索引
> index<-duplicated(data.set$Ensembl.Gene.ID)
> index
 [1] FALSE  TRUE FALSE  TRUE  TRUE  TRUE  TRUE  TRUE  TRUE FALSE
#筛选数据
> data.set2<-data.set[!index,]  #选中了非重复的数据
> data.set2
#用法与is.na()对比
x[!is.na(x)]  #选中不是缺失值的数据`


**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)
———————————————————————————


