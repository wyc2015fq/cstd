# Stata数据处理: 面板数据填充和补漏 - stata连享会 - CSDN博客





2018年12月25日 10:47:10[arlionn](https://me.csdn.net/arlionn)阅读数：660
所属专栏：[Stata连享会](https://blog.csdn.net/column/details/27605.html)









> 
Stata连享会 ([知乎](https://zhuanlan.zhihu.com/arlion) | [简书](http://www.jianshu.com/u/69a30474ef33) | [码云](https://gitee.com/arlionn))

Stata连享会 [精彩推文1](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文2](https://github.com/arlionn/stata/blob/master/README.md)


> 
Source: [http://www.statalist.org/forums/forum/general-stata-discussion/general/17996-substitute-rows-with-average-of-row-above-and-below](http://www.statalist.org/forums/forum/general-stata-discussion/general/17996-substitute-rows-with-average-of-row-above-and-below)




## ### 文章目录- [@[toc]](#toc_9)- [问题描述](#_11)- [解决方法1： 使用 tssmooth ma 命令](#1__tssmooth_ma__45)- [解决方法2： 使用 duplicates tag 命令](#2__duplicates_tag__113)- [附：文中使用的代码](#_184)- [关于我们](#_236)- [联系我们](#_243)- [往期精彩推文](#_250)

## 问题描述

我有一份面板数据，有些年份上的数据有两行或多行记录 (例如，本例中 2007 年的数据)。棘手的是，这两行数据存在差异，且无法判断哪一个记录是正确的。此时，比较稳妥的选择是：将这两汉数据都舍弃，使用相邻年度 (2006 和 2008 年) 的均值作为 2007 年的观察值 (插值)。

```
clear
input ID    year    var1    var2    var3
1    2006    34    45    65
1    2007    45    43    41
1    2007    3    56    59
1    2008    39    54    76
1    2009    41    57    68
end
save "data00.dta", replace
```

原始数据详情：

```
. list

     +--------------------------------+
     | ID   year   var1   var2   var3 |
     |--------------------------------|
  1. |  1   2006     34     45     65 |
  2. |  1   2007     45     43     41 |
  3. |  1   2007      3     56     59 |
  4. |  1   2008     39     54     76 |
  5. |  1   2009     41     57     68 |
     +--------------------------------+
```



## 解决方法1： 使用 tssmooth ma 命令

**思路：**
- 先删除重复的观察值 (2007 年的数据)
- 继而使用 `tsfill` 填充年份，让数据变成平行面板；
- 最后用 `tssmooth ma` 命令插值 (用前后两年的平均值代替 2007 年的缺失值)。

说明：此处 **ma** 是 **moving average** 的简写。

命令如下：

```
. use "data00.dta", clear
*-删除重复值  (数据有误)
. drop if year == 2007
. tsset ID year
. tsfill
. list
     +--------------------------------+
     | ID   year   var1   var2   var3 |
     |--------------------------------|
  1. |  1   2006     34     45     65 |
  2. |  1   2007      .      .      . |
  3. |  1   2008     39     54     76 |
  4. |  1   2009     41     57     68 |
     +--------------------------------+
*-填充缺失值：
forv i = 1/3 {
    tssmooth ma v`i' = var`i' , w(1, 0, 1)
    replace var`i' = v`i' if var`i' == .
}
drop v?
list
```

运行结果如下：

```
. forv i = 1/3 {
  2.     tssmooth ma v`i' = var`i' , w(1, 0, 1)
  3.     replace var`i' = v`i' if var`i' == .
  4. }
The smoother applied was
     (1/2)*[x(t-1) + 0*x(t) + x(t+1)]; x(t)= var1
(1 real change made)
The smoother applied was
     (1/2)*[x(t-1) + 0*x(t) + x(t+1)]; x(t)= var2
(1 real change made)
The smoother applied was
     (1/2)*[x(t-1) + 0*x(t) + x(t+1)]; x(t)= var3
(1 real change made)

. drop v?

. list
     +--------------------------------+
     | ID   year   var1   var2   var3 |
     |--------------------------------|
  1. |  1   2006     34     45     65 |
  2. |  1   2007   36.5   49.5   70.5 |
  3. |  1   2008     39     54     76 |
  4. |  1   2009     41     57     68 |
     +--------------------------------+
```

从结果的提示信息可以看出 **w(1, 0, 1)** 选项的含义：

```
The smoother applied was
     (1/2)*[x(t-1) + 0*x(t) + x(t+1)]; x(t)= var1
```



## 解决方法2： 使用 duplicates tag 命令

**思路：**
- 使用 `duplicates tag` 命令标记重复的观察值，并使用 `gen()` 选项产生新变量用以记录标记情况；
- 删除重复的观察值 (其实，这两步可以合为一步)；
- 使用 `_n` 标记观察先后顺序 (**x[_n]** 表示 $x_{t}；$**x[_n-1]** 表示 $x_{t-1}$；**x[_n+1]** 表示 $x_{t+1}$)，进行插值；

```
clear
input id year var1 var2 var3
1 2006 34 45 65
1 2007 45 43 41
1 2007 3 56 59
1 2008 39 54 76
1 2009 41 57 68
end
```

使用 `duplicates tag` 标记重复的观察值：

```
. duplicates tag id year, gen(mistake)  
. list
. bysort id year: keep if (_n == 1)
  foreach v of varlist var1 var2 var3 {  
     replace `v' =  (`v'[_n-1] + `v'[_n+1])/2 if mistake
  }
```

结果如下：

```
.  duplicates tag id year, gen(mistake)  
Duplicates in terms of id year

.  list   // A-1
     +------------------------------------------+
     | id   year   var1   var2   var3   mistake |
     |------------------------------------------|
  1. |  1   2006     34     45     65         0 |
  2. |  1   2007     45     43     41         1 |
  3. |  1   2007      3     56     59         1 |
  4. |  1   2008     39     54     76         0 |
  5. |  1   2009     41     57     68         0 |
     +------------------------------------------+

.  bysort id year: keep if (_n == 1)
(1 observation deleted)

.  list   // A-2
     +------------------------------------------+
     | id   year   var1   var2   var3   mistake |
     |------------------------------------------|
  1. |  1   2006     34     45     65         0 |
  2. |  1   2007     45     43     41         1 |
  3. |  1   2008     39     54     76         0 |
  4. |  1   2009     41     57     68         0 |
     +------------------------------------------+

.  foreach v of varlist var1 var2 var3 {  
  2.     replace `v' =  (`v'[_n-1] + `v'[_n+1])/2 if mistake
  3.  }

.  list   // A-3
     +------------------------------------------+
     | id   year   var1   var2   var3   mistake |
     |------------------------------------------|
  1. |  1   2006     34     45     65         0 |
  2. |  1   2007   36.5   49.5   70.5         1 |
  3. |  1   2008     39     54     76         0 |
  4. |  1   2009     41     57     68         0 |
     +------------------------------------------+
```



## 附：文中使用的代码

```
*------------
*-Solution 1:
* You could use tssmooth ma.
* For example:

clear
input ID    year    var1    var2    var3
1    2006    34    45    65
1    2007    45    43    41
1    2007    3    56    59
1    2008    39    54    76
1    2009    41    57    68
end

list

drop if year == 2007
tsset ID year
tsfill
list 

forv i = 1/3 {
    tssmooth ma v`i' = var`i' , w(1, 0, 1)
    replace var`i' = v`i' if var`i' == .
}
drop v?
list

*------------
*-Solution 2:  

clear
input id year var1 var2 var3
1 2006 34 45 65
1 2007 45 43 41
1 2007 3 56 59
1 2008 39 54 76
1 2009 41 57 68
end

duplicates tag id year, gen(mistake)  
list
bysort id year: keep if (_n == 1)
list
foreach v of varlist var1 var2 var3 {  
   replace `v' =  (`v'[_n-1] + `v'[_n+1])/2 if mistake
}
list
```

> 
#### 关于我们

- 【**Stata 连享会(公众号：StataChina)**】由中山大学连玉君老师团队创办，旨在定期与大家分享 Stata 应用的各种经验和技巧。
- 公众号推文同步发布于 [CSDN-Stata连享会](https://blog.csdn.net/arlionn) 、[简书-Stata连享会](http://www.jianshu.com/u/69a30474ef33) 和 [知乎-连玉君Stata专栏](https://www.zhihu.com/people/arlionn)。可以在上述网站中搜索关键词`Stata`或`Stata连享会`后关注我们。
- 点击推文底部【阅读原文】可以查看推文中的链接并下载相关资料。
- Stata连享会 [精彩推文1](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文2](https://github.com/arlionn/stata/blob/master/README.md)

> 
#### 联系我们

- **欢迎赐稿：** 欢迎将您的文章或笔记投稿至`Stata连享会(公众号: StataChina)`，我们会保留您的署名；录用稿件达`五篇`以上，即可**免费**获得 Stata 现场培训 (初级或高级选其一) 资格。
- **意见和资料：** 欢迎您的宝贵意见，您也可以来信索取推文中提及的程序和数据。
- **招募英才：** 欢迎加入我们的团队，一起学习 Stata。合作编辑或撰写稿件五篇以上，即可**免费**获得 Stata 现场培训 (初级或高级选其一) 资格。
- **联系邮件：**[StataChina@163.com](mailto:StataChina@163.com)

> 
#### 往期精彩推文

- [Stata连享会推文列表1](https://www.jianshu.com/p/de82fdc2c18a)
- [Stata连享会推文列表2](https://github.com/arlionn/Stata_Blogs/blob/master/README.md)
- Stata连享会 [精彩推文1](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文2](https://github.com/arlionn/stata/blob/master/README.md)
![](https://img-blog.csdnimg.cn/2018122617482359)
![欢迎加入Stata连享会(公众号: StataChina)](https://img-blog.csdnimg.cn/2018122617482377)




