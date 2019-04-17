# 多期DID：平行趋势检验图示 - stata连享会 - CSDN博客





2018年12月25日 10:46:06[arlionn](https://me.csdn.net/arlionn)阅读数：3920








> 
作者：侯新烁 [编译] ([知乎](https://zhuanlan.zhihu.com/arlion) | [简书](http://www.jianshu.com/u/69a30474ef33) | [码云](https://gitee.com/arlionn))


> 
Stata连享会 [精品专题](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文](https://github.com/arlionn/stata/blob/master/README.md)


> 
**编者按：** 平行趋势假设检验是多期 DID 模型中必不可少的检验步骤，本期为大家介绍在 Stata 中如何进行多期平行趋势假设检验并绘制平行趋势检验图。


> 
资料参考来源：[mostly-harmless-replication - 05 Fixed Effects, DD and Panel Data](https://github.com/vikjam/mostly-harmless-replication/blob/master/05%20Fixed%20Effects%2C%20DD%20and%20Panel%20Data/05%20Fixed%20Effects%2C%20DD%20and%20Panel%20Data.md)


## ### 文章目录- [@[toc]](#toc_8)- [1. 导言](#1__11)- [2. 资源准备](#2__17)- [2.1 基本原理图示](#21__19)- [2.2  安装 coefplot](#22___coefplot_38)- [3. 多期 DID 平行趋势图](#3__DID__54)- [Step 1: 准备与数据下载](#Step_1__58)- [Step 2: 调用数据及数据预处理](#Step_2__82)- [Step 3:  估计 DID 模型](#Step_3___DID__125)- [step 4 图形的输出与优化](#step_4__147)- [4. 代码汇总](#4__192)- [关于我们](#_274)- [联系我们](#_281)- [往期精彩推文](#_288)

## 1. 导言

**双重差分模型**（ Difference in Differences , **DID** ）是政策效果评估中常用的一种计量识别策略。其原理是基于反事实框架来评估政策发生和不发生这两种情景下被观测变量（因变量）的变化，因而样本被划分为实验组（ Treated 或 Treat ）和对照组（ Untreated 或 Control ）。

使用该方法的一个尤为重要的假设条件即满足 **“平行趋势”（ Parallel Trend ）**，即两组样本在冲击或者政策发生前必须具有可比性，因为控制组的表现被假定为实验组的反事实。本次推文，将结合 `coefplot` 命令为大家介绍多期 DID 模型平行趋势检验图形的绘制。

## 2. 资源准备

#### 2.1 基本原理图示

在仅有两期的DID模型中，我们通常使用如下图形展示其原理，以 2008 年为政策发生时间点为例：

![两期 DID 图示 ](http://upload-images.jianshu.io/upload_images/7692714-f46ee2ca2da223e7.jpeg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

其中，**C** 对应 **Control** 组，**T** 对应 **Treat** 组，**TD** 代表 T 的反事实情形，则此时 T 与 TD 在 2008 年的差值即为 DID 识别的处理效应，而此时因为前后各仅有一期，实际上较难以识别平行假设是否成立，因而我们通常会对通过样本本身的匹配性加强这一假设的稳健性。

而在多期情形中，图形可能展示如下：

![多期 DID 原理图](http://upload-images.jianshu.io/upload_images/7692714-64fed7b4e48abc7b.jpeg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

多个时间节点信息的出现，使我们可以对不同样本中组因变量的变化趋势进行观察。此时，平行趋势要求处理组 T 和控制组 C 在政策时点前具有一致的趋势（平行）。当然，不同组别样本前具有平行趋势，并不代表着政策发生之后的时间段内依然具有反事实意义上的平行趋势。

从文献来看，最为常见的展示是否符合平行趋势假设的检验方法有两个：**其一**，对比不同组别因变量均值的时间趋势；**其二**，回归中加入各时点虚拟变量与政策变量的交互项（如本例中年份虚拟变量乘以实验组虚拟变量），若政策或称为处理发生前的交互项系数不显著，则表明的确有着平行趋势。

接下来，本推文将给出 Stata 中进行第二种检验方法的代码操作和图形输出。

#### 2.2  安装 coefplot

该命令由 Ben Jann （ University of Bern, [ben.jann@soz.unibe.ch](mailto:ben.jann@soz.unibe.ch) ）于 2017 年 9月编写，用于对回归系数和其他结果进行图形绘制，展示系数结果和置信区间范围，更多信息参见 `help coefplot` 。

若您的电脑尚未安装该命令，请执行如下代码：

```
ssc install coefplot, replace
```

若安装出现问题请使用 findit 命令手动安装

```
findit coefplot
```

## 3. 多期 DID 平行趋势图

本期推介的是由 **vikjam** 编写的 [Replication of tables and figures from “Mostly Harmless Econometrics” in Stata, R, Python and Julia](https://github.com/vikjam/mostly-harmless-replication/blob/master/05%20Fixed%20Effects%2C%20DD%20and%20Panel%20Data/05%20Fixed%20Effects%2C%20DD%20and%20Panel%20Data.md) 中关于 DID 的图形输出部分代码。

#### Step 1: 准备与数据下载

```
clear all
set more off
eststo clear
capture version 14

* 下载 zip 压缩包数据文件
. shell curl -o outsourcingatwill_table7.zip   ///
    "http://economics.mit.edu/~dautor/outsourcingatwill_table7.zip"  
* 解压到当前文件夹
. unzipfile outsourcingatwill_table7.zip
```

此时，若正常运行，则会在 dos 窗口中看到如图情形。

![下载 zip 数据](https://upload-images.jianshu.io/upload_images/7692714-a9d3849c65c2697f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

解压后得到文件夹 table7 ，其中的  `autor-jole-2003.dta 数据即为本次示例数据。经验证，部分 Stata 用户可能无法正确执行上述 dos 下载过程，此时需要将网址 [http://economics.mit.edu/~dautor/outsourcingatwill_table7.zip](http://economics.mit.edu/~dautor/outsourcingatwill_table7.zip) 复制到浏览器进行下载，然后手动解压获得数据文件。

![解压后文件夹](http://upload-images.jianshu.io/upload_images/7692714-f3e885207a55bdbb.jpeg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

#### Step 2: 调用数据及数据预处理

作者对数据进行了论文的仿制处理，具体经济含义请参考原文，或者请忽略，因为不是敲黑板呦。

```
use "table7/autor-jole-2003.dta", clear

/* 对数总就业人数：来自劳工统计局（BLS）就业和收入数据 */
gen lnemp = log(annemp)

/* 非商业服务部门就业：来自美国海关（ CBP ） */
gen nonemp  = stateemp - svcemp
gen lnnon   = log(nonemp)
gen svcfrac = svcemp / nonemp

/* 商业服务部门总就业：来自美国海关（ CBP ） */
gen bizemp = svcemp + peremp
gen lnbiz  = log(bizemp)

/* 生成时间趋势 */
gen t  = year - 78 // 线性时间趋势
gen t2 = t^2       // 二次时间趋势

/* 限制样本时间区间 */
keep if inrange(year, 79, 95) & state != 98

/* 生成更多的统计数据 */
gen clp     = clg + gtc
gen a1624   = m1619 + m2024 + f1619 + f2024
gen a2554   = m2554 + f2554
gen a55up   = m5564 + m65up + f5564 + f65up
gen fem     = f1619 + f2024 + f2554 + f5564 + f65up
gen white   = rs_wm + rs_wf
gen black   = rs_bm + rs_bf
gen other   = rs_om + rs_of
gen married = marfem + marmale

/* 修正工会变量 */
replace unmem = . if inlist(year, 79, 81) 
replace unmem = unmem * 100
```

#### Step 3:  估计 DID 模型

数据中，各时间节点与政策处理的交互项都是手动生成的，具体说明如下：
- 以 `admico` 开头带 `_`  的表示政策执行前多少期对应的交互项，如 `admico_2` 表示政策发生前 2 期的虚拟变量与处理组变量的交互项；
- 以 `admico` 开头不带 `_`  的表示政策发生后对应交互项，其中 `admico0` 表示政策发生当期。

此时可以直接展开多期 DID 回归了。**因此，在应用该方法时，需要事先生成交互项**，此处需注意。

![虚拟变量交互项已经生成](http://upload-images.jianshu.io/upload_images/7692714-c4d17171f0ffc31d.jpeg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

```
reg lnths lnemp admico_2 admico_1 admico0 admico1 admico2 admico3 mico4 admppa_2 admppa_1   ///
    admppa0 admppa1 admppa2 admppa3 mppa4 admgfa_2 admgfa_1 admgfa0 admgfa1 admgfa2 admgfa3 ///
    mgfa4 i.year i.state i.state#c.t, cluster(state)
```

该回归通过 `i.year` 、 `i.state` 和 `i.state#c.t` 对时间固定效应、个体固定效应和个体与时间趋势的交叉效应进行了控制。

![回归结果展示](http://upload-images.jianshu.io/upload_images/7692714-d362a1115bf38361.jpeg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

#### step 4 图形的输出与优化

使用 `coefplot` 可较为便捷和快速的生成多期动态效应图，更多使用方法请 `help coefplot` 。

```
coefplot, keep(admico_2 admico_1 admico0 admico1 admico2 admico3 mico4)   vertical  addplot(line @b @at)
```

![多期图](http://upload-images.jianshu.io/upload_images/7692714-6615436a3afaa8f3.jpeg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

通过 coefplot 选项对图形进行优化，对横纵坐标的标记、标题以及样式等进行设定：

```
coefplot, ///
   keep(admico_2 admico_1 admico0 admico1 admico2 admico3 mico4)  ///
   coeflabels(admico_2 = "2 yr prior"   ///
   admico_1 = "1 yr prior"              ///
   admico0  = "Yr of adopt"             ///
   admico1  = "1 yr after"              ///
   admico2  = "2 yr after"              ///
   admico3  = "3 yr after"              ///
   mico4    = "4+ yr after")            ///
   vertical                             ///
   yline(0)                             ///
   ytitle("Log points")                 ///
   xtitle("Time passage relative to year of adoption of implied contract exception") ///
   addplot(line @b @at)                 ///
   ciopts(recast(rcap))                 ///
   rescale(100)                         ///
   scheme(s1mono)
```

![多期图优化](http://upload-images.jianshu.io/upload_images/7692714-449fda8dd263b59e.jpeg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

当然，也可以通过 scheme() 将风格更改为自己熟悉或常用的格式，例如 qleanmono 风格：

![多期图优化qleanmono](http://upload-images.jianshu.io/upload_images/7692714-52efb5040bcac6d7.jpeg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

最后，图片的输出保存：

```
graph export "figure.png", replace
```

## 4. 代码汇总

```
ssc install coefplot, replace
*findit coefplot

clear all
set more off
eststo clear
capture version 14

shell curl -o outsourcingatwill_table7.zip "http://economics.mit.edu/~dautor/outsourcingatwill_table7.zip"
unzipfile outsourcingatwill_table7.zip

use "table7/autor-jole-2003.dta", clear
gen lnemp = log(annemp)

gen nonemp  = stateemp - svcemp
gen lnnon   = log(nonemp)
gen svcfrac = svcemp / nonemp

gen bizemp = svcemp + peremp
gen lnbiz  = log(bizemp)

gen t  = year - 78
gen t2 = t^2      

keep if inrange(year, 79, 95) & state != 98

gen clp     = clg + gtc
gen a1624   = m1619 + m2024 + f1619 + f2024
gen a2554   = m2554 + f2554
gen a55up   = m5564 + m65up + f5564 + f65up
gen fem     = f1619 + f2024 + f2554 + f5564 + f65up
gen white   = rs_wm + rs_wf
gen black   = rs_bm + rs_bf
gen other   = rs_om + rs_of
gen married = marfem + marmale

replace unmem = . if inlist(year, 79, 81) 
replace unmem = unmem * 100              

reg lnths lnemp admico_2 admico_1 admico0 admico1 admico2 admico3 mico4 admppa_2 admppa_1   ///
    admppa0 admppa1 admppa2 admppa3 mppa4 admgfa_2 admgfa_1 admgfa0 admgfa1 admgfa2 admgfa3 ///
    mgfa4 i.year i.state i.state#c.t, cluster(state)

coefplot, keep(admico_2 admico_1 admico0 admico1 admico2 admico3 mico4)   vertical  addplot(line @b @at) 


coefplot, ///
   keep(admico_2 admico_1 admico0 admico1 admico2 admico3 mico4)  ///
   coeflabels(admico_2 = "2 yr prior"   ///
   admico_1 = "1 yr prior"              ///
   admico0  = "Yr of adopt"             ///
   admico1  = "1 yr after"              ///
   admico2  = "2 yr after"              ///
   admico3  = "3 yr after"              ///
   mico4    = "4+ yr after")            ///
   vertical                             ///
   yline(0)                             ///
   ytitle("Log points")                 ///
   xtitle("Time passage relative to year of adoption of implied contract exception") ///
   addplot(line @b @at)                 ///
   ciopts(recast(rcap))                 ///
   rescale(100)                         ///
   scheme(s1mono)

graph export "figurename.png", replace
```

> 
后记：本质上是根据平行假设检验的回归设计，获得系数和对应标准误信息，然后利用标记置信区间的 connected 进行绘图。






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

- 
[Stata连享会推文列表1](https://www.jianshu.com/p/de82fdc2c18a)

- 
[Stata连享会推文列表2](https://github.com/arlionn/Stata_Blogs/blob/master/README.md)

- 
Stata连享会 [精品专题](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文](https://github.com/arlionn/stata/blob/master/README.md)
![](https://upload-images.jianshu.io/upload_images/7692714-8b1fb0b5068487af.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![欢迎加入Stata连享会(公众号: StataChina)](http://upload-images.jianshu.io/upload_images/7692714-aac735aa45404445.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)





