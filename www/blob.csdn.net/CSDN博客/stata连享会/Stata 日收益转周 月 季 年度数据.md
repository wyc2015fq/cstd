# Stata: 日收益转周\月\季\年度数据 - stata连享会 - CSDN博客





2018年12月25日 10:19:24[arlionn](https://me.csdn.net/arlionn)阅读数：613
个人分类：[stata数据处理](https://blog.csdn.net/arlionn/article/category/7157125)

所属专栏：[Stata连享会](https://blog.csdn.net/column/details/27605.html)











> 
作者：朱红兵 (南京大学)


责编：王俊 (中山大学)


Stata 连享会： [知乎](https://zhuanlan.zhihu.com/arlion) | [简书](http://www.jianshu.com/u/69a30474ef33) | [码云](https://gitee.com/arlionn)

- Stata连享会 [精彩推文1](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文2](https://github.com/arlionn/stata/blob/master/README.md)

![](https://img-blog.csdnimg.cn/20181225101924596)

> 
#### 本文要点
- 解释简单收益率和对数收益率的差异；
- 使用 `ascol` 将日收益率数据转换为**周\月\季\年 度数据；
- 使用 `mtoq` 将月数据转换为季度数据；




### 1 两种收益率的计算

**例如** 我们有下述股票的日度价格序列：
|股票id|日期|价格|
|----|----|----|
|1|20180101|15|
|1|20180102|19|
|1|20180103|17|
|1|20180104|16|
|1|20180105|20|
|1|20180106|18|
|1|20180107|19|

根据简单收益率和对数收益率计算公式：

简单算术收益率 *EQ1*：

$$(1)SimpleReturn_{i,t}=\frac{Price_{i,t}-Price_{i,t-1}}{Price_{i,t-1}} \tag{1}$$

连续对数收益率 *EQ2*：

$$(2)SimpleReturn_{i,t}=ln(\frac{Price_{i,t}}{Price_{i,t-1}}) \tag{2}$$

我们可以计算出简单收益率和对数收益率如下：
|股票id|日期|简单收益率|对数收益率|1+简单收益率|
|----|----|----|----|----|
|1|20180102|0.2667|0.2364|1.2667|
|1|20180103|-0.1053|-0.1112|0.8947|
|1|20180104|-0.0588|-0.0606|0.9412|
|1|20180105|0.2500|0.2231|1.2500|
|1|20180106|-0.1000|-0.1054|0.9000|
|1|20180107|0.0556|0.0541|1.0556|

### 2 区间内收益率的计算

在实证研究中，很多时候我们希望得到周收益率或者月度收益率，那么如何根据日度收益率得到这些指标呢？

可能很多同学会想当然地认为：直接加总日收益率即可！为题在于：直接对简单收益率进行加总得到的周（月）收益率等于对数日收益率的加总吗？我们可以用上述数据来测试看一下（计算 2018 年 1 月 2 日至 2018 年 1 月 7 日的持有期收益率）：
- 简单收益率的加总：

$$0.2667-0.1053-0.0588+0.2500-0.1000+0.0556=0.3082$$
- 对数收益率的加总：

$$0.2364-0.1112-0.0606+0.2231-0.1054+0.0541=0.2364$$
- 两者差异：

$$0.3082-0.2364=0.0718$$

> 
**问题：** 为什么两者会相差 0.0718 呢？哪种计算方法才能准确反应投资者在 2018 年 1 月 2 日至 2018 年 1 月 7 日 期间内的持有收益呢？


**答案**： **对数日收益率的加总**，即下式 (*EQ3*)：
$$(3)Return_{i,[t_{t0},t_{T}]}=\sum_{t=t0}^T{LogReturn_{i,t}}  \tag{3}$$

因为，区间内简单日收益率的加总意味着投资者每天都在进行交易，而非在区间内买入并持有至末期。当然，我们也可以通过简单算术日收益率计算区间内的持有期收益，但是需要采用下述方法：

持有期收益率 (*EQ4*)：

$$(4)Return_{i,[t_{t0},t_{T}]}=\frac{Price_{i,T}-Price_{i,t_{t0}}}{Price_{i,t_{t0}}} \tag{4} $$

或者采用 (*EQ5*)：

$$(5)Return_{i,[t_{t0},t_{T}]}=\prod_{t=t0}^T{(1+SimpleReturn_{i,t})}-1\tag{5}$$

### 3 Stata命令介绍：ascol

#### 3.1 安装 ascol

在 Stata 命令框中输入下述命令并回车，即可实现安装：

```
ssc install ascol, replace
```

#### 3.2 如何调用该命令进行收益率计算？

`ascol` 的完整语法格式如下：

```
ascol varname, returns([simple | log]) ///
      prices ///
      keep([all | vars]) ///
      frequency_options  ///
      timevar(varname) panelvar(varname) ///
      generate(newvar)
```

其中：
- `varname` 主要是收益率序列或者价格序列。
- `returns()` 中可选择 **simple** 或 **log**。其中，**simple** 表明 varname 是简单收益率；**log** 代表对数收益率。若 varname 设定的是价格序列，则这里的选择会指导具体收益率序列的计算方式。
- `prices` 指定了 varname 中序列的类型是否为价格序列，如果是收益率序列则无需填写。
- `keep` 选项可保留计算后的重复数据，例如日收益率转换为月收益率后，在相同月会出现多个相同的收益率观测，如果选择了 keep 命令则保留了相同的观测，反之则只每个时间点上保留一个观测，剔除了重复观测
- `frequency_options` 中可设定 **toweek**，**tomonth**，**toquarter**, **toyear**，即由日收益率转换为不同频率 (周，月，季，年) 的收益率。
- `timevar()` 和 `panelvar()` 选项可设定数据的时间变量和截面变量。
- `generate()` 可设定新生产的收益率序列的名称。

#### 3.3 Stata 范例

> 
下述代码来源于 `ascol` 的官方帮助文档，读者可自行前往查看


```
/*生产收益率序列*/
clear
set obs 1000
gen date=date("1/1/2012" , "DMY")+_n
format %td date
tsset date
gen pr=10
replace pr=pr[_n-1]+uniform() if _n>1
gen simpleRi=(pr/l.pr)-1
gen logRi = ln(pr/l.pr)
save "stocks.dta", replace

/*Example 1: From Daily to weekly -  simple returns*/
use stocks, clear
ascol simpleRi, toweek returns(simple)

/*Example 2: From Daily to weekly -  log returns*/
use "stocks", clear
ascol logRi, toweek returns(log)

/* Example 3: From Daily to monthly -  prices*/
use "stocks", clear
ascol pr, tomonth price
```

#### 3.4 补充一个新命令 mtoq

前文介绍的 `ascol` 命令虽然可以将日收益率转换为其他频率的收益率，但是有些时候我们下载的收益率**月度**收益率，那如何将其转换为季度收益率呢？我们写了一个新的命令 `mtoq` 来实现这一功能。

##### Step1 安装 mtoq

```
/* 1. 没有安装 github 命令的请先通过下述命令进行安装 */
net install github, from("https://haghish.github.io/github/")  replace

/* 2. 安装过 github 命令的同学可忽略上述命令，直接进行下述安装 */
github install zhbsis/mtoq
```

##### Step2 调用 mtoq

`mtoq` 命令的语法非常简洁，如下：

```
mtoq varlist, by(string) [Statistics(string)]
```

其中,
- `varlist` 中填写对数收益率序列，可同时填写多个序列。
- `by` 选项中填写个体和时间截面的id，例如股票类数据可填写 stkcd（股票代码） ym（年月时间标记）
- `Statistics` 选项中可填写 Stata 官方命令 `egen` 中的常见 option ，例如 mean、sum 等，默认为 mean 处理，在月度收益率转换为季度收益率的时候通常填写 sum 选项。

> 
**Tips**：`mtoq` 命令可以有更为广泛的用途。上面的例子是对**收益率**序列进行转换，但 `mtoq` 也可以转换其他变量，如 **换手率 (TurnOver)**。若想通过加总月度换收益率得到季度的换手率，可输入：


```
mtoq TurnOver, by(stkcd ym) s(sum)
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
- [Stata连享会推文列表2](https://gitee.com/arlionn/jianshu/blob/master/README.md)
- Stata连享会 [精彩推文1](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文2](https://github.com/arlionn/stata/blob/master/README.md)

![](https://img-blog.csdnimg.cn/20181225101924596)

![欢迎加入Stata连享会(公众号: StataChina)](https://img-blog.csdnimg.cn/20181225101924629)







