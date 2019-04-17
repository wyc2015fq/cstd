# gen-和-egen-中的-sum()-函数 - stata连享会 - CSDN博客





2018年02月15日 16:09:21[arlionn](https://me.csdn.net/arlionn)阅读数：2434
个人分类：[stata数据处理](https://blog.csdn.net/arlionn/article/category/7157125)

所属专栏：[Stata连享会](https://blog.csdn.net/column/details/27605.html)









> 
作者：连玉君 ([知乎](https://zhuanlan.zhihu.com/arlion) | [简书](http://www.jianshu.com/u/69a30474ef33) | [码云](https://gitee.com/arlionn))


#### 此求和非彼求和

实证分析中，进场需要进行加总计算。Stata 中的 `generate` 命令以及更为强大的 `egen` 命令都提供了 `sum()` 函数。然而，需要特别注意的是，二者的功能有很大的差异。先看看如下范例：

```
clear
input x
      1
      2
      3
      4
end

 gen sx_gen  = sum(x)
egen sx_egen = sum(x)

list , clean noobs
```

结果如下：

```php
. list , clean noobs
    x   sx_gen   sx_egen  
    1        1        10  
    2        3        10  
    3        6        10  
    4       10        10
```

可见，`gen` 提供的 `sum()` 函数执行的是**累积加总**，而 `egen` 提供的 `sum()` 函数则进行**整体加总**。

#### 扩展应用：分组求和

计算各个年度的销售总额 (**sx_egen**)，以及每家公司当年的市场份额 (**sale_per**)：

```
clear
input id     year    sale  
      601    2011    0.1
      602    2011    0.2
      601    2012    0.3
      602    2012    0.4
      603    2012    0.5
end

bysort year:  gen sx_gen  = sum(sale)
bysort year: egen sx_egen = sum(sale)

gen sale_per = sale/sx_egen*100 //市场份额

format sx* sale* %3.1f
list, noobs sepby(year)
```

结果如下：

```
. list, noobs sepby(year)

  +-------------------------------------------------+
  |  id   year   sale   sx_gen   sx_egen   sale_per |
  |-------------------------------------------------|
  | 601   2011    0.1      0.1       0.3       33.3 |
  | 602   2011    0.2      0.3       0.3       66.7 |
  |-------------------------------------------------|
  | 601   2012    0.3      0.3       1.2       25.0 |
  | 602   2012    0.4      0.7       1.2       33.3 |
  | 603   2012    0.5      1.2       1.2       41.7 |
  +-------------------------------------------------+
```

> 
#### 关于我们
- 【**Stata 连享会(公众号：StataChina)**】由中山大学连玉君老师团队创办，旨在定期与大家分享 Stata 应用的各种经验和技巧。
- 公众号推文同步发布于 [【简书-Stata连享会】](http://www.jianshu.com/u/69a30474ef33) 和 [【知乎-连玉君Stata专栏】](https://www.zhihu.com/people/arlionn)。可以在**简书**和**知乎**中搜索关键词`Stata`或`Stata连享会`后关注我们。
- 推文中的相关数据和程序，以及 [Markdown 格式原文](https://gitee.com/arlionn/jianshu) 可以在 [【Stata连享会-码云】](https://gitee.com/arlionn) 中获取。[【Stata连享会-码云】](https://gitee.com/arlionn) 中还放置了诸多 Stata 资源和程序。如 [Stata命令导航](https://gitee.com/arlionn/stata/wikis/Home) ||  [stata-fundamentals](https://gitee.com/arlionn/stata-fundamentals) ||  [Propensity-score-matching-in-stata](https://gitee.com/arlionn/propensity-score-matching-in-stata) || [Stata-Training](https://gitee.com/arlionn/StataTraining) 等。

#### 联系我们
- **欢迎赐稿：** 欢迎将您的文章或笔记投稿至`Stata连享会(公众号: StataChina)`，我们会保留您的署名；录用稿件达`五篇`以上，即可**免费**获得 Stata 现场培训 (初级或高级选其一) 资格。
- **意见和资料：** 欢迎您的宝贵意见，您也可以来信索取推文中提及的程序和数据。
- **招募英才：** 欢迎加入我们的团队，一起学习 Stata。合作编辑或撰写稿件五篇以上，即可**免费**获得 Stata 现场培训 (初级或高级选其一) 资格。
- **联系邮件：** StataChina@163.com

#### 特别说明

文中包含的链接在微信中无法生效。请点击本文底部左下角的`【阅读原文】`，转入本文`【简书版】`。


##### 近期推文
- [Stata：用 bytwoway 实现快速分组绘图](http://www.jianshu.com/p/1471cf87f25f)
- [Stata可视化：让他看懂我的结果！](http://www.jianshu.com/p/43fe2339c90c)
- [Stata: 用esttab生成带组别名称的 LaTeX 回归表格](http://www.jianshu.com/p/0d327ec1f204)
- [Stata： 双重差分的固定效应模型 (DID)](http://www.jianshu.com/p/e97c1dc05c2c)
- [Stata：Mata 笔记](http://www.jianshu.com/p/03d138ff81da)
- [可汗学院风格电子板书攻略: Wacom+ArtRage](http://www.jianshu.com/p/717b2e689d96)
- [怎么在Stata图形中附加水平线或竖直线？](http://www.jianshu.com/p/0f962953593a)
- [EndNote X7，X8 使用说明](http://www.jianshu.com/p/af1c15c079a3)
- [Stata15 Unicode：一次性转码，解决中文乱码问题](http://www.jianshu.com/p/4347685cc11e)



