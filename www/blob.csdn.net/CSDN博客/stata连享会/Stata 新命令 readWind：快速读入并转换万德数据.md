# Stata 新命令 readWind：快速读入并转换万德数据 - stata连享会 - CSDN博客





2018年12月25日 10:22:12[arlionn](https://me.csdn.net/arlionn)阅读数：186
所属专栏：[Stata连享会](https://blog.csdn.net/column/details/27605.html)











> 
作者：朱红兵 (南京大学)

Stata 连享会： [知乎](https://zhuanlan.zhihu.com/arlion) | [简书](http://www.jianshu.com/u/69a30474ef33) | [码云](https://gitee.com/arlionn)



- Stata连享会 [精彩推文1](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文2](https://github.com/arlionn/stata/blob/master/README.md)
![](https://img-blog.csdnimg.cn/20181225102212573)
> 
前言：WIND 数据库是大家在科研路上常用的数据下载利器，但遗憾的是从 WIND 上下载的 EXCEL 数据文件通常是横向排列的 (即 Stata 用户熟悉的 **wide** 格式)，需要我们手工整理或采用 `reshape` 等命令进行转换后才能满足 Stata 的分析要求。出于这一需求，我们编写了一个新命令：`readWind`，用以将宽型 (wide) 的上市公司财务数据高效地转换为长型 (long) 的面板数据。


## 1. 下载安装 readWind 命令

### 第一步：在 Stata 中安装 `github` 命令

在 Stata 的 command 框口中输入如下命令自动下载外部命令 `github`。该命令用于搜索、安装 [Github](https://github.com/) 上发布的 Stata 外部命令 (若已经安装 `github` 命令，可以忽略此步)：

```
net install github, from("https://haghish.github.io/github/")
```

### 第二步：使用 `github` 命令安装 [Github](https://github.com/) 上的命令

在 Stata 的 command 框中输入下面的代码，耐心等待安装完成：

```
github install zhbsis/TwoSort
```

上述命名安装完成中你会发现你的 c 盘 ado 文件夹中的 a 文件夹里面多了`readWind`这个 ado 命令，这就是将 Wind 数据终端中下载的 excel 数据转换为能够直接进行实证分析的 long 型数据格式的命令啦！！！

### 第三步：`readWind`对 Wind 下载的 EXCEL 格式有什么要求呢？

只要你的 EXCEL 数据排版格式满足下面的要求，都是可以通过`readWind`读入到 Stata 中的：
|股票代码|股票名称|时间1|时间2|…|时间n|
|----|----|----|----|----|----|
|000016|深康佳A|1.051|…|…|…|
|000017|深中华A|0.951|…|…|…|
|…|…|…|…|…|…|

### 第四步：如何调用执行`readWind`命令呢？

调用`readWind`只需在 Stata 的 command 命令框输入下述命令行即可：

```
readWind, var(EXCEL名称) timeType(时间类型) t0(样本起始时间) [split splitN(样本分割数) erase]
```

其中，
- EXCEL 名称就是从 Wind 下载下来的 EXCEL 名称，建议用英文命名，而且最好以变量名命名，比如你下载了**ROE**的数据，那这个 EXCEL 的文件名就命令为 **ROE**；
- 时间类型是指下载下来的样本是年度，季度还是月度数据，所以它的取值是 y/q/m ，暂时不支持日度
- 样本起始时间，比如你选择从 1995 年 1 月开始下载，那么这个 t0 里面就写 1995m1，如果是 1995 年第 1 季度开始，那就写 1995q1，如果是年度数据以年为变化单位那就写 1995 ；
- `split splitN(n) erase`这 3 个关键词是可选择的命令，当样本中的数据量太大时数据格式转换的速度相对较慢，所以我们采用了个折中的方法将样本分割成 n 个小样本，依次进行处理最后合并在一起。 n 取值越大分割的样本越小。建议 n 的取值是原始数据列数的 1/10 到 1/20 之间；
- 另外需要注意的是，本命令是基于`gather`命令进行的转置，所以需要先安装`tidy`中的`gather`命令，即`ssc install tidy`

## 2.  readWind 的使用案例

举个栗子：我从 Wind 上下载了 A 股上市企业 **ROE** 的数据（ROE.xlsx），格式如下：

![ROE.png](https://img-blog.csdnimg.cn/20181225102212594)

那么只需要在 Stata 中输入下述命令：

```
cd "ROE所在的文件夹"             /// 改变工作路径到ROE文件夹下
readWind, var(ROE) timeType(q) t0(1995q1)      /// 调用readWind命令
```

或者输入：

```
readWind, var(ROE) timeType(q) t0(1995q1) split splitN(10) erase
```

等命令运行完就可以在 Stata 中看到漂亮规范的数据格式啦：

![ROE.png](https://img-blog.csdnimg.cn/20181225102212653)

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

![](https://img-blog.csdnimg.cn/20181225102212573)

![欢迎加入Stata连享会(公众号: StataChina)](https://img-blog.csdnimg.cn/20181225102212688)




