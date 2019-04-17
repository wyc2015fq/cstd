# asdoc：Stata 结果输出又一利器！ - stata连享会 - CSDN博客





2018年10月29日 08:20:56[arlionn](https://me.csdn.net/arlionn)阅读数：4912








> 
作者：卢梅 | Stata连享会 ([知乎](https://zhuanlan.zhihu.com/arlion) | [简书](http://www.jianshu.com/u/69a30474ef33) | [码云](https://gitee.com/arlionn))


- Stata连享会 [精品专题](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文](https://github.com/arlionn/stata/blob/master/README.md)
![](https://upload-images.jianshu.io/upload_images/7692714-8b1fb0b5068487af.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)



### 文章目录
- [1、输出描述性统计结果](#1_26)
- [Example 1：基本用法](#Example_1_28)
- [Example 2：设定输出格式](#Example_2_46)
- [Example 3：分组统计量](#Example_3_61)
- [2、回归结果输出](#2_78)
- [基本用法](#_82)
- [横向放置的回归表格](#_111)
- [3、表头相同，数据结构相同的数据输出](#3_150)
- [4、多维列表输出](#4_172)
- [asdoc 相关链接](#asdoc__193)
- [关于我们](#_203)
- [联系我们](#_211)
- [往期精彩推文](#_218)



攥写论文过程中，将统计和回归结果以及其他变量输出到 word 是重要一步，如何才能既美观又不耗费大量脑细胞改改改、调调调呢？


`summarize`、`correlate`、`tabstat`、`cross-tabs`、`regressions`、`t-tests`等等等等，这些命令的结果怎么输出？想想都觉得头大。

每次想把几个回归结果显示在一张表上，都要每做一次回归就储存一次，最后集合起来输出。对于表头相同，数据结构相同的数据，怎样才能既省地方又美观的把它们放在一张表上，也经常让人抓狂。

好在， Stata 中一条新发布的命令 `asdoc`，将解救我们于结果输出的苦海中。其最大的特点是：在各种命令前加 `asdoc` 就能轻轻松松输出结果。

本文将重点介绍 `asdoc` 命令关于描述性统计输出、回归结果输出、表头相同数据结构相同的数据输出的功能，篇幅有限，所以挑最经常用到的功能进行介绍，`asdoc` 详情请见`help asdoc`。

接下来，就让我们一起来见证 `asdoc` 的神奇功能吧。

首先，老规矩，在 stata 中敲入如下代码安装 `asdoc`：

```
. ssc install asdoc, replace
```

## 1、输出描述性统计结果

### Example 1：基本用法

以 Stata 系统自带的数据为例，直观说明 `asdoc` 的用法

```
. sysuse auto, clear 
. asdoc sum
```

描述性统计输出结果如下：

> 
![asdoc 基本输出结果 (Stata连享会)](http://upload-images.jianshu.io/upload_images/13541986-6a27206d7ded1d70?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

相关说明如下：- `asdoc sum` 与 `asdoc sum, append` 效果相同，新输出的描述性统计结果连带之前的命令运行结果，一起输出到word文档里。
- `asdoc sum, replace` 表示只输出最新运行的描述性统计结果。
- `asdoc sum price mpg rep78` 表示对 **price mpg rep78** 这些变量进行描述性统计
- `asdoc sum price mpg rep78，save(summary.doc) title(###)` 表示对 **price mgp rep78** 进行描述性统计，表名显示为 **###**，word 文档保存为 `summary.doc`，效果如下：

> 
![image](http://upload-images.jianshu.io/upload_images/13541986-81c22c154770b31d?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)


### Example 2：设定输出格式

我们也可以输出更多的统计量，同时对输出格式进行美化设定：

```
. asdoc sum,  stat(N mean sd tstat p1 p99) fs(7) dec(2)
```

具体释义如下：
- `stat(# # #)` 表示需要输出的描述性统计变量，具体而言，`stat(N mean sd tstat p1 p99)` 表示输出的统计变量为样本数、算术平均数、标准差、t 值、1% 分位数、99% 分位数，
- `fs(#)` 为 **Font size** 的缩写，表示字号大小为 `#` 英镑，
- `dec(#)` 为 **Decimal points** 的缩写，表示输出结果保留到小数点后 **# 位**

输出效果如下：

> 
![asdoc 输出统计结果-格式控制 (Stata连享会)](http://upload-images.jianshu.io/upload_images/13541986-d4ebe5f8c3921b7b?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)


### Example 3：分组统计量

如下两条命令都可以实现将国外样本与国内样本分开进行描述性统计：

```
. asdoc sum, stat(N mean sd tstat p1 p99) by(foreign)
*-或
. bysort foreign: asdoc sum, stat(N mean sd tstat p1 p99)
```

具体释义如下：
- `by(varname)` 表示按照 **varname** 分类进行描述性统计，
- 如果想增加文本描述可以使用 `text` 选项， 例如如下命令就可以在表格下方显示文本，括号内的文本可以自行替换：

```
. asdoc, text(A car is a wheeled motor vehicle used for transportation) append fs(10)
```

输出效果为：

> 
![asdoc 分组统计量输出 (Stata连享会)](http://upload-images.jianshu.io/upload_images/13541986-3627671c14f5367f?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)


## 2、回归结果输出

接下来，我们介绍 `asdoc` 大大解放生产力的两大利器：`nest append` 和 `rowappend` 选项。

### 基本用法

将几个回归结果输出到一张表上是我们在写论文时经常遇到的操作，来看看 `asdoc` 是如何实现的：先运行第一个回归，后面加 `nest` 表示这将会是一张集合输出的表，代码及输出结果如下：

```
. sysuse auto, clear
. asdoc reg price mpg rep78, nest replace
```

输出效果为：

> 
![asdoc 回归结果输出 (Stata连享会)](http://upload-images.jianshu.io/upload_images/13541986-c3e1b034719ab372?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)


接下来，我们再运行第二个回归，代码及输出结果如下：

```
. asdoc reg price mpg rep78 headroom, nest append
```

> 
![asdoc 回归结果输出 (Stata连享会)](http://upload-images.jianshu.io/upload_images/13541986-d48c39e00759c8de?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)


运行第三个回归：

```
. asdoc reg price mpg rep78 headroom weight, nest append
```

结果如下：

> 
![](https://upload-images.jianshu.io/upload_images/13541986-149d77413783ef0e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)


简言之，需要在一张表上显示多个回归结果就依次进行 `nest append`。

### 横向放置的回归表格

以上介绍的是对于被解释变量相同的回归的输出方式，对于被解释变量不同，解释变量相同的多个回归结果的共同输出问题，我们使用 `asdoc` 中的 **wide** 选项，仍然以 Stata 系统自带的数据为例，输入如下命令：

```
. asdoc reg price mpg rep78, replace wide
```

输出结果为：

> 
![](https://upload-images.jianshu.io/upload_images/13541986-d33c83ca50c50be2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)


想把被解释变量为 **trunk** 的回归结果追加于上表，输入如下命令：

```
. asdoc reg trunk mpg rep78, wide
```

输出结果为：

> 
![image](http://upload-images.jianshu.io/upload_images/13541986-ac2697c593bfefe1?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)


加入被解释变量为 `weight` 的回归结果，输出效果为：

> 
![](https://upload-images.jianshu.io/upload_images/13541986-689105fc4089514b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)


有如下几个问题需要说明：
- 在每一条回归命令后加 `t(below)` 表示在系数的下方显示对应的 **t 值**，
- `t(side)`表示在旁边显示t值。

使用如下两条命令重现输出上述三个模型的估计结果：

```
. asdoc reg price mpg rep78, wide replace t(below) // below
. asdoc reg price mpg rep78, wide replace t(side)  // beside
```

效果为：

> 
![](http://upload-images.jianshu.io/upload_images/13541986-ca2ab0c487ba1d55?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![image.png](https://upload-images.jianshu.io/upload_images/13541986-d242ba6583db758e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
如果想显示标准差而不是t值，就把`t(below)`和`t(side)`替换成`se(below)`与`se(side)`。

## 3、表头相同，数据结构相同的数据输出

**t-tests** 结果是表头相同，数据结构相同的典型代表，**t-tests** 针对每一个变量都需要运行一次，但每个变量的 **t-tests**  结果数据结构相同，此时就可以用 `rowappend` 使它们显示在一张表上。输入如下代码时，只显示 **rep78** 的 t-tests 结果。

```
. sysuse auto, clear
. asdoc ttest rep78==0, replace title(T-test results : mean==0)
```

> 
![](http://upload-images.jianshu.io/upload_images/13541986-ddd7a9c5e9d45bb3?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)


接着依次输入：

```
. asdoc ttest  price==0, rowappend
. asdoc ttest    mpg==0, rowappend
. asdoc ttest   turn==0, rowappend
. asdoc ttest weight==0, rowappend
. asdoc ttest length==0, rowappend
```

每输入一条命令，表格都会自动增加一行来显示新增加变量的`t-tests`结果，最终结果如下：

> 
![](http://upload-images.jianshu.io/upload_images/13541986-73cb60188bb9215a?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)


## 4、多维列表输出

强大的 `asdoc` 也可以实现列表统计功能，不加规定默认输出的统计量为频数，可以通过 `contents()` 设定想要输出的统计量名称。`title(###)` 对表格名称进行设定。输出一维列表的命令如下：

```
. sysuse auto, clear
. asdoc table rep78, title(###) c(n mpg mean mpg sd mpg median mpg) replace
```

表名为 **###**，一维列表如下：

> 
![](http://upload-images.jianshu.io/upload_images/13541986-c5f9996b7e3d116d?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

- 二维列表在 `asdoc table` 后加表示分类依据的两个变量
- 三维列表在 `asdoc table` 后面加表示分类依据的三个变量
- 四维列表在 `asdoc table` 后加表示分类依据的三个变量后，需以逗号隔开，再加 `by()`，`()` 里的变量表示第四个分类依据。

试过之后发现，多维列表用 `asdoc` 输出会出现单词被隔开显示成两列的问题，不知道是我 bug 了还是命令本身就 bug 了，如果你试过之后也发现，显示结果有问题，列表显示暂时还是用传统的 `table` 命令吧，**Stata连享会** 2018 年 4 月 8 号的推文有关于 `table` 命令的详细用法，请戳 [[Stata：今天你 “table” 了吗？-简书]](https://www.jianshu.com/p/b0537619e19c)

欢迎提出批评意见，共同探讨，共同学习！

本期 Stata 课堂到此结束，观众朋友们，下期再会！

## asdoc 相关链接
- [asdoc 项目主页](http://fintechprofessor.com/2018/01/31/asdoc/)
- [asdoc 的一些主要用法及注意事项](https://fintechprofessor.com/tag/asdoc/)
- [asdoc: An easy way of creating publication quality tables from Stata commands](https://www.statalist.org/forums/forum/general-stata-discussion/general/1435798-asdoc-an-easy-way-of-creating-publication-quality-tables-from-stata-commands)

> 
#### 关于我们

- 【**Stata 连享会(公众号：StataChina)**】由中山大学连玉君老师团队创办，旨在定期与大家分享 Stata 应用的各种经验和技巧。
- 公众号推文同步发布于 [CSDN-Stata连享会](https://blog.csdn.net/arlionn) 、[简书-Stata连享会](http://www.jianshu.com/u/69a30474ef33) 和 [知乎-连玉君Stata专栏](https://www.zhihu.com/people/arlionn)。可以在上述网站中搜索关键词`Stata`或`Stata连享会`后关注我们。
- 点击推文底部【阅读原文】可以查看推文中的链接并下载相关资料。

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
- Stata连享会 [精品专题](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文](https://github.com/arlionn/stata/blob/master/README.md)
![](https://upload-images.jianshu.io/upload_images/7692714-8b1fb0b5068487af.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![欢迎加入Stata连享会(公众号: StataChina)](http://upload-images.jianshu.io/upload_images/7692714-549e3798bdbcaa99.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)









