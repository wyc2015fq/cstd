# xtbalance 以后: 非平衡面板之转换 - stata连享会 - CSDN博客





2018年12月21日 09:31:59[arlionn](https://me.csdn.net/arlionn)阅读数：1507
所属专栏：[Stata连享会](https://blog.csdn.net/column/details/27605.html)











> 
作者：吴雄 (湘潭大学)

Stata 连享会： [知乎](https://zhuanlan.zhihu.com/arlion) | [简书](http://www.jianshu.com/u/69a30474ef33) | [码云](https://gitee.com/arlionn)

- Stata连享会 [精品专题](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文](https://github.com/arlionn/stata/blob/master/README.md)
![](https://upload-images.jianshu.io/upload_images/7692714-8b1fb0b5068487af.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
之前，我在处理数据的时候，经常碰到一些非平衡面板数据，当时处理完后才发现连玉君老师之前已经编写了一个 `xtbalance` 的命令，该命令专门用于将连续时间非平衡面板处理成平衡面板。但是，该命令在处理非连续时间的非平衡面板有点缺陷，所以平时我还是自己写命令来处理非平衡面板数据的问题，这两天我发现有同学还有一些别的交流群里的群友在用的时候就碰到这个问题，今天，我就打算细讲如何将非平衡面板转换成平衡面板。
- 目录
- 1.xtbalance 命令的使用
- 2.xtbalance 的流程
- 2.1 生成连续时间的非平衡面板
- 2.2 不用xtbalance命令的处理成平衡面板的方法
- 2.3 xtbalance的使用

- 3.非连续时间的非平衡面板的处理
- 3.1 生成数据
- 3.2 处理成平衡面板
- 3.3 使用 xtbalance

- 4.非平衡面板非连续时间也没有固定间隔

### 1.xtbalance 命令的使用

首先，这个命令是 Stata 外部命令，需要先下载安装才能使用。下载安装的命令为: `ssc install xtbalance`。完成后，输入 `help xtbalance` 查看帮助文档，以了解该命令的使用方法。

`xtbalance` 的语法格式: `xtbalance, range(numlist) [ miss(varlist) ]`
- 请注意，在使用该命令之前你需要设置你的面板数据，你可以通过 `xtset panelvar timevar` 或 `tsset panelvar timevar` 将数据设定成面板数据格式。

接下来，你可以根据所需时间段来处理数据，比如我需要 1992-1998 年的平行面板，命令为 `xtbalance, range(1992 1998)`。
- 局限：当你的 `timevar` 不是 1991、1992、……2000 这样的连续时间，而是类似 1992、1994、1996 …… 2000 这种非连续时间的时，直接使用 `xtbalance, range(1992 1998)` 会导致所有观察值都被删除了。的结果将会删除你的整个样本，这是怎么回事呢？
- 当我对我的非连续时间的平衡面板 1992(2)2000 ，进行上述命令的时候，出来的结果如下：

```
//数据生成
. set obs 100
. gen id = int(_n/5.001)+1
. bysort id:gen year = 1992 +2*(_n-1)
. xtset id year

       panel variable:  id (strongly balanced)
        time variable:  year, 1992 to 2000, but with gaps
                delta:  1 unit

//取1992-1998年段的平衡面板
. xtbalance, range(1992 1998)

(20 observations deleted due to out of range) 
(80 observations deleted due to discontinues)
```
- 即，该命令首先将不在你选定的时间段内的样本删掉，之后，它在清理每个 id 的时候，将组内非连续时间的 id 删掉，但为什么我的平衡面板会被全部删除呢？因为我的不是连续时间。那么连续是怎么计算的呢？我的猜测是这个命令按照连续时间来计算每个 id 的样本数量，将其样本数量与预计间隔相比较，即它在清理每个 id 的时候，将组内没有 1998 - 1992 + 1 数量的id删掉，但是我们的时间间隔不一样，每个组内的样本数是不一样的，我的每个 id 应该只有 ( 1998 - 1992 ) / 2 +1个，它按照连续时间来计算的话会比我多，所以它认为我的组内缺失了样本，不是平衡的，所以会将这些也删掉。结果，数据就全被删了。

### 2.xtbalance 的流程

我们这里就不贴`xtbalance`命令的源代码，但是我通过自己编写的一个do文件来展示我猜测的具体操作流程。

#### 2.1 生成连续时间的非平衡面板

我们首先生成一个 `id` 为 0 (1) 99，`year` 为 1991 (1) 2000 的平衡面板：

```
. clear
. set  obs 1000
. gen id =int(_n/10.000001)
. bysort id :gen  year = 1990 + _n
. xtset id year

       panel variable:  id (strongly balanced)
        time variable:  year, 1991 to 2000
                delta:  1 unit
```

然后，我们生成均匀分布的随机数，将随机数小于 0.2 的样本删掉，即每条都有 20% 的概率被删掉，剩下的样本为非平衡的样本

```
. drop if runiform() <0.2
. xtset id year

       panel variable:  id (unbalanced)
        time variable:  year, 1991 to 2000, but with gaps
                delta:  1 unit
```

同时，通过`tab id`我们可以看每个id的样本数（受限于编辑器，这里只列出几行）都不一样

```
. tab id 

         id |      Freq.     Percent        Cum.
------------+-----------------------------------
          0 |          6        0.73        0.73
          1 |          9        1.10        1.83
          2 |          8        0.98        2.81
          3 |          9        1.10        3.91
          4 |          7        0.86        4.77
          5 |          9        1.10        5.87
```
- 注意：这时候，如果在1991(1)2000时间段上是平衡面板的话，每个id应该有2000-1991+1=10条记录

#### 2.2 不用 xtbalance 命令的处理成平衡面板的方法

实际上，`xtbalance` 命令的实现是这样的:

```
删除不在时间段内的样本；
计算每个id的样本数，如果每个 id 组内缺少样本，则将这个 id 的样本都删除。
```

假如，我们需要将上述数据 id  : 0 (1) 99 year: 1991 (1) 2000的非平衡面板转为 id :0 (1) 99 year: 1992 (1) 1998的平衡面板的话，我们需要知道当转为平衡面板后，每个id相对应的只有1998-1992+1=7条记录

```
. preserve
. drop if year > 1998 | year < 1992
(243 observations deleted)

. bysort id :keep if _N ==7
(400 observations deleted)

. xtset id year
       panel variable:  id (strongly balanced)
        time variable:  year, 1992 to 1998
                delta:  1 unit
. restore
```

#### 2.3 xtbalance 的使用

```
. xtbalance, range(1992 1998)

(243 observations deleted due to out of range) 
(400 observations deleted due to discontinues) 

. xtset id year
       panel variable:  id (strongly balanced)
        time variable:  year, 1992 to 1998
                delta:  1 unit
```

### 3.非连续时间的非平衡面板的处理
- 本节将介绍有固定间隔时间的非平衡面板的处理

实际过程中，有许多数据库并不是连续时间的面板，这可能是因为多种原因，比如，某个数据库在调查数据时候，分别在 1990、1992、1994 每两年调查一次的，其实，这种满好的，因为有固定间隔，然而，当不是固定间隔的时候，处理数据的时候你可能需要多留意。

#### 3.1 生成数据

先生成平衡面板(不是连续时间)
- obs : 1000
- id ：1 (1) 99
- year：1991 (2) 2009

```
*-生成非连续时间的平衡面板
. clear
. set obs 1000
. gen id =int(_n/10.000001)
. bysort id :gen  year = 1990 + 2*_n -1
. xtset id year
       panel variable:  id (strongly balanced)
        time variable:  year, 1991 to 2009, but with gaps
                delta:  1 unit

. xtset id year, delta(2)
       panel variable:  id (strongly balanced)
        time variable:  year, 1991 to 2009
                delta:  2 units

. tab year

       year |      Freq.     Percent        Cum.
------------+-----------------------------------
       1991 |        100       10.00       10.00
       1993 |        100       10.00       20.00
       1995 |        100       10.00       30.00
       1997 |        100       10.00       40.00
       1999 |        100       10.00       50.00
       2001 |        100       10.00       60.00
       2003 |        100       10.00       70.00
       2005 |        100       10.00       80.00
       2007 |        100       10.00       90.00
       2009 |        100       10.00      100.00
------------+-----------------------------------
      Total |      1,000      100.00
```

```
*-直接使用xtbalance的缺陷/
. preserve

. count 
  1,000

. xtbalance, range(1993 2005)

(300 observations deleted due to out of range) 
(700 observations deleted due to discontinues) 

. count 
  0
 
. restore
```
- 注意：
- `xtset id year` 与 `tsset id year` 都可以同时设置面板
- 因为是非连续时间的平衡面板 ，所以 `xtset id year` 命令会提示说你的数据 “strongly balanced" 但是时间有 “gap”，如果你的面板 `time variable` 是有固定间隔的，那么你可以在 `xtset id year, delta()` 的 `delta()` 选项中设置。
- 通过上述 `xtbalance` 命令，我们可以看到，我们的非连续时间的平衡面板直接使用用`xtbalance` 是不合适的。

再通过随机删除部分样本，使其成为非平衡面板:

```
. drop if runiform() <0.2
(168 observations deleted)

. xtset id year,delta(2)
       panel variable:  id (unbalanced)
        time variable:  year, 1991 to 2009, but with gaps
                delta:  2 units
```

#### 3.2 处理成平衡面板

我们所需要的样本时间段为 **1993(2)2005**:

先查看一下数据：

```
. tab year

       year |      Freq.     Percent        Cum.
------------+-----------------------------------
       1991 |         78        9.38        9.38
       1993 |         84       10.10       19.47
       1995 |         80        9.62       29.09
       1997 |         82        9.86       38.94
       1999 |         82        9.86       48.80
       2001 |         88       10.58       59.38
       2003 |         79        9.50       68.87
       2005 |         86       10.34       79.21
       2007 |         87       10.46       89.66
       2009 |         86       10.34      100.00
------------+-----------------------------------
      Total |        832      100.00
```

我们所需要的是 1993(2)2005 的时间段的话，每个 id 在 1993（2）2005时间段内的样本数应该是1+（2005-1993）/2 = 7，所以，我们的操作流程就是：

1.先只保留1993-2005年的所有样本，不在时间段内的删除掉；

2.计算每个id的样本数，只包括样本数为7的。
```
. preserve

. drop if year>2005 | year <1993
(251 observations deleted)

. bysort id :drop if _N != 7
(413 observations deleted)

. xtset id year,delta(2)
       panel variable:  id (strongly balanced)
        time variable:  year, 1993 to 2005
                delta:  2 units

. restore
```

#### 3.3 使用 xtbalance 的新姿势

对于有固定间隔的平衡面板数据，我们可以预先生成一个新的时间变量 (连续的)，进而使用 `xtbalance` 命令进行处理。

```
. xtset id year, delta(2)
       panel variable:  id (unbalanced)
        time variable:  year, 1991 to 2009, but with gaps
                delta:  2 units

. bysort id: gen new_year = _n  //使用该命令前一点要先对数据排序
* 或 gen new_year = 1 + (year - r(min))/2
```

现在 1993 年在 new_year 中就是 2，而 2005 年就是 8 ，然后用新的时间来设置面板，再使用`xtbalance,range(2 8)` 就OK了

```
. xtset id new_year
       panel variable:  id (unbalanced)
        time variable:  new_year, 1 to 10, but with gaps
                delta:  1 unit

. xtbalance,range(2 8)

(251 observations deleted due to out of range) 
(413 observations deleted due to discontinues) 

. xtset id year,delta(2)
       panel variable:  id (strongly balanced)
        time variable:  year, 1993 to 2005
                delta:  2 units
```

### 4.非平衡面板非连续时间也没有固定间隔

股票日交易资料有一个典型的特征：所有公司的资料在非交易日都是缺失的。因此，虽然股票日交易资料本质上是一份连续数据，但 Stata 在接到 `xtset id date` 的通知后，将其视为非平衡面板数据。（Note: 这里暂不考虑那些在交易日里停牌的公司）

此时的处理方法与上一小节相似：可以先使用 `bysort id: gen new_data=_n` 命令或 `egen new_date = group(date)` 命令生成新的时间变量。

对于更为一般的情形，处理方法也会相对复杂：先删掉不在时间段内的样本，然后通过某种方法将你的时间变量转为一个新的具有连续时间的变量，之后通过每个 id 的样本数与为平衡时的样本数进行比较，一样则保留，否则删除。

### 5. 后记 (连玉君)

在写作这篇推文过程中，我与吴雄多次沟通，受到了不少启发。

后续我们会将文中提及的 `xtbalance` 的局限一一解决掉，以便大家能够更为便捷地进行非平行面板 → 平行面板的转换。

计划添加的功能如下(欢迎各位补充，反馈至：arlionn@163.com )：
- 添加 `delta()` 选项：以便处理**有固定间隔的平衡面板数据**；
- 添加 `gen(newvarname)` 选项：以便对平行面板构成的观察值进行标记；
- 添加 `force` 选项：由程序自动对用户的数据进行处理；
- ……



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
- [Stata连享会推文列表2](https://gitee.com/arlionn/jianshu/blob/master/README.md)
- Stata连享会 [精彩推文1](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文2](https://github.com/arlionn/stata/blob/master/README.md)

![](https://upload-images.jianshu.io/upload_images/7692714-8b1fb0b5068487af.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![欢迎加入Stata连享会(公众号: StataChina)](http://upload-images.jianshu.io/upload_images/7692714-c317947be074a605.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)





