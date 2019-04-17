# Stata新命令快讯: 有向无环图、模糊倍分法等 - stata连享会 - CSDN博客





2018年12月25日 10:20:57[arlionn](https://me.csdn.net/arlionn)阅读数：208
所属专栏：[Stata连享会](https://blog.csdn.net/column/details/27605.html)











> 
Stata 连享会： [知乎](https://zhuanlan.zhihu.com/arlion) | [简书](http://www.jianshu.com/u/69a30474ef33) | [码云](https://gitee.com/arlionn)

- Stata连享会 [精彩推文1](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文2](https://github.com/arlionn/stata/blob/master/README.md)

> 
**编者按：** 定期递送 SSC 网站上发布的最新命令，对于我们认为重要的命令进行标注。

对于您感兴趣的命令，可以直接在 Stata 命令窗口中输入 `ssc install 命令名称, replace` 进行下载。下载完成后执行 `help 命令名称` 即可查看帮助文件和详细使用说明。


### 关键命令
- `help ivqreg2` // structural quantile functions || **结构分位数回归**
- `help fuzzydid` // **模糊倍分法**
- `help tvdiff` // **时变倍分法**
- `help balancetable` //||**组间均值差异检验**
- `help dag` //directed acyclic graphs (DAG) || **有向无环图**
- `help rdpermute` // **断点回归 (RDD) 和拐点回归 (RKD) 的组合检验**
- `help gtools` //  **快速计算分组统计量**



## [SSC](http://econpapers.repec.org/software/bocbocode/) Stata modules

created or revised 2018-11-21 to 2018-12-21

[DYNARDL](http://econpapers.repec.org/software/bocbocode/S458572.htm)  module to dynamically simulate autoregressive distributed lag (ARDL) models

Authors: Soren Jordan  Andrew Q. Philips      Req: Stata version 8

Created: 2018-12-10
[IVQREG2](http://econpapers.repec.org/software/bocbocode/S458571.htm)  module to provide **structural quantile function estimation** || **分位数回归**

Authors: J.A.F. Machado  J.M.C. Santos Silva      Req: Stata version 14

Created: 2018-12-09
[NICEWORDS](http://econpapers.repec.org/software/bocbocode/S458570.htm)  module to pay compliments

Authors: Joe Long      Req: Stata version 12

Created: 2018-12-09
[PMSAMPSIZE](http://econpapers.repec.org/software/bocbocode/S458569.htm)  module to calculate the minimum sample size required for developing a multivariable prediction model

Authors: Joie Ensor      Req: Stata version 12.1

Created: 2018-12-04
[CDO](http://econpapers.repec.org/software/bocbocode/S458568.htm)  module to get an update when the dofile stalls

Authors:  Jesse Wursten      Req: Stata version 8

Revised: 2018-12-10
[BFMCORR](http://econpapers.repec.org/software/bocbocode/S458567.htm)  module for correcting surveys using tax data

Authors: Thomas Blanchet  Ignacio Flores   Marc Morgan      Req: Stata version 11

Created: 2018-12-04
[BRAIN](http://econpapers.repec.org/software/bocbocode/S458566.htm)  module to provide neural network || **神经网络**

Authors:  Thorsten Doherr      Req: Stata version 9

Created: 2018-11-26
[MOTIVATE](http://econpapers.repec.org/software/bocbocode/S458565.htm)  module providing motivation to users

Authors:  Kabira Namit      Req: Stata version 10

Revised: 2018-11-26
[SVMACHINES](http://econpapers.repec.org/software/bocbocode/S458564.htm)  module providing **Support Vector Machines** for both Classification and Regression || 支持向量机

Authors: Nick Guenther  Matthias Schonlau      Req: Stata version 13

Revised: 2018-11-22
[ADMETAN](http://econpapers.repec.org/software/bocbocode/S458561.htm)  module to provide comprehensive meta-analysis

Authors: David Fisher      Req: Stata version 11

Revised: 2018-12-04
[PRTAB](http://econpapers.repec.org/software/bocbocode/S458554.htm)  module to compute Precision-recall curves

Authors:  Jonathan Cook      Req: Stata version 11

Revised: 2018-12-04
[FUZZYDID](http://econpapers.repec.org/software/bocbocode/S458549.htm)  module to estimate **Fuzzy Difference-in-Difference** Designs || **模糊倍分法**

Authors:  Cl茅ment de Chaisemartin    Xavier D’Haultfoeuille   Yannick Guyonvarch      Req: Stata version 13.1

Revised: 2018-11-26
[RDPERMUTE](http://econpapers.repec.org/software/bocbocode/S458536.htm)  module to perform a permutation test for the **Regression Kink (RK)** and **Regression Discontinuity (RD) Design** || **断点回归和拐点回归的组合检验**

Authors: Simon J盲ger  Peter Ganong      Req: Stata version 13, rdrobust and rd from SSC (q.v.)

Revised: 2018-12-09
[GTOOLS](http://econpapers.repec.org/software/bocbocode/S458514.htm)  module to provide a fast implementation of common group commands || **快速分组计算**

Authors: Mauricio Caceres Bravo      Req: Stata version 13.1

Revised: 2018-12-09
[DRM](http://econpapers.repec.org/software/bocbocode/S458506.htm)  module to fit Sobel’s Diagonal Reference Model (DRM)

Authors: Caspar Kaiser      Req: Stata version 12

Revised: 2018-11-23
[CV_REGRESS](http://econpapers.repec.org/software/bocbocode/S458469.htm)  module to estimate the leave-one-out error for linear regression models

Authors: Fernando Rios-Avila      Req: Stata version 7

Revised: 2018-11-23
[DAG](http://econpapers.repec.org/software/bocbocode/S458467.htm)  module to provide utilities for **directed acyclic graphs** || **有向无环图**

Authors: Chunsen Wu      Req: Stata version 13.1

Revised: 2018-12-04
[FUNNELPERFORM](http://econpapers.repec.org/software/bocbocode/S458442.htm)  module to produce **funnel plot** for institutional comparison || **漏斗图**

Authors: Brent McSharry      Req: Stata version 9.1

Revised: 2018-14-18
[BALANCETABLE](http://econpapers.repec.org/software/bocbocode/S458424.htm)  module to build a balance table and print it in a LaTeX file or an Excel file || **组间均值差异检验**

Authors: Mattia Chiapello      Req: Stata version 14 and listtab from SSC (q.v.)

Revised: 2018-11-23
[TVDIFF](http://econpapers.repec.org/software/bocbocode/S458384.htm)  module to compute pre- and post-treatment estimation of the Average Treatment Effect (ATE) with binary time-varying treatment || **时变倍分法**

Authors: Giovanni Cerulli  Marco Ventura      Req: Stata version 14 and coefplot from SSC (q.v.)

Revised: 2018-11-23
[WID](http://econpapers.repec.org/software/bocbocode/S458357.htm)  module to download data from the World Wealth and Income Database (WID.world)

Authors: Thomas Blanchet      Req: Stata version 13

Revised: 2018-12-04
[ENTROPYETC](http://econpapers.repec.org/software/bocbocode/S458272.htm)  module for entropy and related measures for categories

Authors: Nicholas J. Cox      Req: Stata version 11.2

Revised: 2018-11-23
[PPML_PANEL_SG](http://econpapers.repec.org/software/bocbocode/S458249.htm)  module to estimate “structural gravity” models via Poisson PML

Authors:  Tom Zylkin      Req: Stata version 11.2, hdfe and reghdfe from SSC (q.v.)

Revised: 2018-12-04
[TIMEIT](http://econpapers.repec.org/software/bocbocode/S458220.htm)  module to provide single line version of timer on/off

Authors:  Jesse Wursten      Req: Stata version 10

Revised: 2018-12-04
[CPRDLINKUTIL](http://econpapers.repec.org/software/bocbocode/S458193.htm)  module for inputting CPRD linkage-source datasets into Stata

Authors: Roger Newson      Req: Stata version 13 and keyby, chardef from SSC (q.v.)

Revised: 2018-12-10

[](http://econpapers.repec.org/software/bocbocode/S458075.htm)ASROL  module to generate rolling window / groups descriptive statistics || 滚动窗口统计量

Authors: Attaullah Shah      Req: Stata version 11

Revised: 2018-12-10

[](http://econpapers.repec.org/software/bocbocode/S458069.htm)STRCS  module to estimate flexible parametric survival models on the log hazard scale || 生存分析

Authors: Hannah Bower  Michael J. Crowther  Paul Lambert      Req: Stata version 13.1

Revised: 2018-12-04

[](http://econpapers.repec.org/software/bocbocode/S457795.htm)MIMRGNS  module to run margins after mi estimate

Authors: Daniel Klein      Req: Stata version 11.2

Revised: 2018-12-04

[](http://econpapers.repec.org/software/bocbocode/S457785.htm)IPDMETAN  module for performing two-stage IPD meta-analysis

Authors: David Fisher      Req: Stata version 11

Revised: 2018-12-04

[](http://econpapers.repec.org/software/bocbocode/S457245.htm)SPPACK  module for cross-section spatial-autoregressive models || 截面空间自回归模型

Authors: David M. Drukker  Hua Peng  Ingmar Prucha  Rafal Raciborski      Req: Stata version 11.2

Revised: 2018-12-04


> 
关于我们- 
【Stata 连享会(公众号：StataChina)】由中山大学连玉君老师团队创办，旨在定期与大家分享 Stata 应用的各种经验和技巧。- 
公众号推文同步发布于 [](https://blog.csdn.net/arlionn)CSDN-Stata连享会 、[](http://www.jianshu.com/u/69a30474ef33)简书-Stata连享会 和 [](https://www.zhihu.com/people/arlionn)知乎-连玉君Stata专栏。可以在上述网站中搜索关键词``Stata或``Stata连享会后关注我们。- 
点击推文底部【阅读原文】可以查看推文中的链接并下载相关资料。- 
Stata连享会 [](https://gitee.com/arlionn/stata_training/blob/master/README.md)精彩推文1  || [](https://github.com/arlionn/stata/blob/master/README.md)精彩推文2
> 
联系我们- 
欢迎赐稿： 欢迎将您的文章或笔记投稿至``Stata连享会(公众号: StataChina)，我们会保留您的署名；录用稿件达``五篇以上，即可免费获得 Stata 现场培训 (初级或高级选其一) 资格。- 
意见和资料： 欢迎您的宝贵意见，您也可以来信索取推文中提及的程序和数据。- 
招募英才： 欢迎加入我们的团队，一起学习 Stata。合作编辑或撰写稿件五篇以上，即可免费获得 Stata 现场培训 (初级或高级选其一) 资格。- 
联系邮件：[](mailto:StataChina@163.com)StataChina@163.com
> 
往期精彩推文- 
[](https://www.jianshu.com/p/de82fdc2c18a)Stata连享会推文列表1- 
[](https://gitee.com/arlionn/jianshu/blob/master/README.md)Stata连享会推文列表2- 
Stata连享会 [](https://gitee.com/arlionn/stata_training/blob/master/README.md)精彩推文1  || [](https://github.com/arlionn/stata/blob/master/README.md)精彩推文2

![](https://img-blog.csdnimg.cn/20181225102058262)

![欢迎加入Stata连享会(公众号: StataChina)](https://img-blog.csdnimg.cn/20181225102058284)



























