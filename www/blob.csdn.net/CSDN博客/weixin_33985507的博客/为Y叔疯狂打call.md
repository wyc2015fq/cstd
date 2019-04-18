# 为Y叔疯狂打call - weixin_33985507的博客 - CSDN博客
2018年01月23日 10:52:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：27
最近又被Y叔的吐槽文刷屏了，其实还是那些**数据库网页工具**更新不及时的问题，包括早在2009年就停止更新的WEGO，还有我一直搞不清楚到底啥时候停止更新的DAVID。如果仅仅是为了做GO/KEGG等富集分析，Y叔开发的R包`clusterProfiler` 的确是最好用的，没有之一。奈何入门R语言的到现今仍然只占广大生命科学研究者的少部分。所以，我们生信技能树平台跟Y叔讨论并协作开发了这个R包`clusterProfiler` 的网络接口，理论上只需要少量鼠标操作即可完成全部分析流程，妈妈再也不用担心我不会R语言了。
## 包的介绍
这里就不再写 step-by-step 的教程了，大家如果想看，可以去我们生信技能树公众号，或者我的生信菜鸟团博客里面搜索，而且敬业的Y叔自己就为这些包写了大批量的教程，如下：
- Package Homepage: [http://bioconductor.org/packages/devel/bioc/html/clusterProfiler.html](https://link.jianshu.com?t=http%3A%2F%2Fbioconductor.org%2Fpackages%2Fdevel%2Fbioc%2Fhtml%2FclusterProfiler.html)
- Contributions: [https://guangchuangyu.github.io/clusterProfiler](https://link.jianshu.com?t=https%3A%2F%2Fguangchuangyu.github.io%2FclusterProfiler)
- GitHub [https://github.com/GuangchuangYu/clusterProfiler](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2FGuangchuangYu%2FclusterProfiler)
这里我们总结一下这个R包可以做的，然后为这些功能写网页接口。
#### 功能1： ID转换
这里首先Y叔全面借用了KEGG数据库的网页接口，所以支持的物种是有限的，包括 **Eukaryotes: 415    Bacteria: 4438    Archaea: 260** ，详细信息可以在 [http://www.genome.jp/kegg/catalog/org_list.html](https://link.jianshu.com?t=http%3A%2F%2Fwww.genome.jp%2Fkegg%2Fcatalog%2Forg_list.html) 浏览查看。
而且支持转换的ID种类也有限：‘kegg’, ‘ncbi-geneid’, ‘ncbi-proteinid’ or ‘uniprot’ ，其实在GENECARD网站很容易可以看到常见的数据库ID都已经近百种了，这些看似很简单的映射转换如果要计较细节，其实复杂无比。
其次Y叔依赖于 bioconductor中心的 `OrgDb` 系列包，这个包非常少，但是支持的ID超过30多种了。
#### 功能2：基于超几何分布检验的富集分析
对于该功能，Y叔也写文介绍过，只需要给定一个感兴趣的基因集合和背景基因即可：
- [GO analysis using clusterProfiler](https://link.jianshu.com?t=https%3A%2F%2Fguangchuangyu.github.io%2F2016%2F01%2Fgo-analysis-using-clusterprofiler)
- [use simplify to remove redundancy of enriched GO terms](https://link.jianshu.com?t=https%3A%2F%2Fguangchuangyu.github.io%2F2015%2F10%2Fuse-simplify-to-remove-redundancy-of-enriched-go-terms)
- [KEGG Module Enrichment Analysis](https://link.jianshu.com?t=https%3A%2F%2Fguangchuangyu.github.io%2F2016%2F04%2Fkegg-module-enrichment-analysis)
- [KEGG enrichment analysis with latest online data using clusterProfiler](https://link.jianshu.com?t=https%3A%2F%2Fguangchuangyu.github.io%2F2015%2F02%2Fkegg-enrichment-analysis-with-latest-online-data-using-clusterprofiler)
- [DAVID functional analysis with clusterProfiler](https://link.jianshu.com?t=https%3A%2F%2Fguangchuangyu.github.io%2F2015%2F03%2Fdavid-functional-analysis-with-clusterprofiler)
- [functional enrichment for GTEx paper](https://link.jianshu.com?t=https%3A%2F%2Fguangchuangyu.github.io%2F2015%2F08%2Ffunctional-enrichment-for-gtex-paper)
其实本质上就是超几何分布检验，只需要提供TERM2GENE 和 TERM2NAME信息即可，至于这些基因和条目(GO/KEGG)是哪个数据库收集整理的，并没有本质的区别。比如Y叔就以 [DisGeNET](https://link.jianshu.com?t=http%3A%2F%2Fwww.disgenet.org%2Fweb%2FDisGeNET%2Fmenu%2Fhome) which contains 381056 associations, between 16666 genes and 13172 diseases 为例子，介绍了基于它的富集分析
#### 功能3：GSEA分析
这个分析要求所有背景基因的排序即可。
#### 功能4：可视化
这个绝对是Y叔的强项啦，我们生信技能树平台也多次大力推荐Y叔：
- [dotplot for enrichment result](https://link.jianshu.com?t=https%3A%2F%2Fguangchuangyu.github.io%2F2015%2F06%2Fdotplot-for-enrichment-result)
- [dotplot for GSEA result](https://link.jianshu.com?t=http%3A%2F%2Fguangchuangyu.github.io%2F2016%2F12%2Fdotplot-for-gsea-result%2F)
- [enrichment map](https://link.jianshu.com?t=https%3A%2F%2Fguangchuangyu.github.io%2F2014%2F08%2Fenrichment-map)
