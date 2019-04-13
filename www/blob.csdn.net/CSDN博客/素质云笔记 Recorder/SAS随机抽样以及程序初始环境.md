
# SAS随机抽样以及程序初始环境 - 素质云笔记-Recorder... - CSDN博客

2016年10月17日 11:40:10[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：1794



**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)
———————————————————————————
本来转载于[SAS随机抽样](http://blog.csdn.net/xyilu/article/details/8203566)

在统计研究中，针对容量无限或者容量很大以至于无法直接对其进行研究的总体，都是通过从中抽取一部分个体作为研究对象，以考察总体的特征。被抽取的部分个体称为该总体的一个样本。从总体中抽取样本的过程，称为抽样。
抽样包括随机抽样和非随机抽样。非随机抽样是从总体中抽取指定的个体，具有主观意向性，这里不做讨论。
随机抽样是按照随机原则，保证个体都有一定概率被抽取到的抽样方法。常见的随机抽样方式有：简单随机抽样、系统抽样、分层抽样、整群抽样、多阶段抽样、二重抽样以及比率抽样。
以下将依次介绍各种随机抽样方法的原理、应用场景及其SAS实现。在论述之前，需要准备好测试数据。我们从互联网上找了一批数据形成一张表，数据的内容是国内股票市场各只股票的若干财务数据，字段如下：
|列名|中文名|
|StockCode
|股票代码
|
|StockName
|股票名称
|
|Source
|来源板块
|
|EPS
|每股收益（元）
|
|EPS_YOY
|每股收益同比（|%|）
|
|NAPS
|每股净资产（元）
|
|ROE
|净资产收益率（|%|）
|
|CFPS
|每股现金流量（元）
|
|NP
|净利润（万元）
|

该表共有2472条观测（记录），按照Source（来源板块）进行统计，则各组观测数如下：
创业板             351
沪市主板           948
深市主板           473
中小板             700
数据下载地址：http://pan.baidu.com/share/link?shareid=134615&uk=1258687326
构建程序初始环境：
**data**_null_;
workspace       ="D:\SASWorkspace\练习";*工作区根目录;
callsymput("workspace", workspace);
**run**;
libnamePractice"&workspace.\中间数据\";
然后把原始数据上传到Practice逻辑库中，并命名为MainIndex_2012sea3。

*为了不破坏原始数据，把表copy到work逻辑库中;
**data**Work.MainIndex_2012sea3;
setPractice.MainIndex_2012sea3;
**run**;

下面逐一介绍各种随机抽样方法及其SAS实现。
**（1）简单随机抽样**
简单随机抽样，指从总体中等概率地抽取出n个个体组成样本。在SAS中，可以使用surveyselect过程步来实现随机抽样。Surveyselect过程步的基本格式如下：
|PROC SURVEYSELECT
|DATA=  * 输入数据集;
|OUT=  * 输出数据集;
|METHOD=  * 抽样方法;
|SAMPSIZE=  * 选择项指定需要抽样的样本量;
|SAMPRATE= * ;
|REP=
|SEED=
|NOPRINT;
|ID variable; 指定抽取的样本所保留的源数据集变量
|STRATA variables;  指定分层变量
|CONTROL variables; 控制变量
|SIZE variables; 不等概抽样指标变量
|RUN;
|

在第一个程序中，我们来实现最简单的场景：从2472条观测中随机抽取100条。在程序中，除了必要的data和out选项外，还需使用method设置抽样方法为简单随机抽样，其值为srs；并设置抽取的样本容量sampsize = 100或n = 100。代码如下：
*随机抽取100条记录，保留所有字段，不打印;
**proc****surveyselect**
data = Work.MainIndex_2012sea3
out = Work.MainIndex_2012sea3_srs1
method = srs
sampsize =**100**/*也可以使用n = 100 */
noprint
;
**run**;
上面的程序对于结果表保留了原始表的所有字段，如果我们只需要保留其中的某几个字段，则可以使用id语句。
*随机抽取100条记录，只保留StockCode和StockName字段，不打印;
**proc****surveyselect**
data = Work.MainIndex_2012sea3
out = Work.MainIndex_2012sea3_srs2
method = srs
sampsize =**100**
noprint;
idStockCode StockName;
**run**;
如果没有指定随机数种子（seed），则SAS程序会使用计算机的时间作为种子。可以使用seed选项设定随机数初始种子。Seed的值必须是一个正整数，否则SAS会使用计算机的时间作为种子（零或负整数的情况），或者出错（小数的情况）。
*随机抽取100条记录，保留所有字段，不打印;
*指定随机数种子;
**proc****surveyselect**
data = Work.MainIndex_2012sea3
out = Work.MainIndex_2012sea3_srs4
method = srs
sampsize =**100**
seed =**1000**
noprint;
**run**;
在实际应用场景中，有时候需要独立重复抽取多组样本，这时可以使用rep选项。SAS程序会以rep设定的值独立重复抽取若干次样本，每组样本的容量是sampsize或n选项指定的值。
*随机抽取100条记录，保留所有字段，不打印;
*指定独立重复抽样的次数;
**proc****surveyselect**
data = Work.MainIndex_2012sea3
out = Work.MainIndex_2012sea3_srs5
method = srs
sampsize =**100**
rep =**3**
noprint
;
**run**;
样本容量的另一种表述是其占总体的比例。比如，抽取10%的样本。这时我们使用samprate或rate替代sampsize。Samprate的值可以是正小数，也可以是正整数。当samprate的值是正小数时，其值在(0, 1]之间，不可为零；为1时表示100%。当samprate是正整数时，表示相应的百分比，如10表示10%，需要注意的是，整数1表示100%，而不是1%。
*随机抽取总体的10%作为样本，保留所有字段，不打印;
**proc****surveyselect**
data = Work.MainIndex_2012sea3
out = Work.MainIndex_2012sea3_srs6
method = srs
samprate =**0.1**/*也可以使用rate =0.1 */
noprint
;
**run**;
*随机抽取总体的10%作为样本，保留所有字段，不打印;
**proc****surveyselect**
data = Work.MainIndex_2012sea3
out = Work.MainIndex_2012sea3_srs7
method = srs
samprate =**10**/*也可以使用rate =10 */
noprint
;
**run**;
有时候，我们并不需要把原始表的所有观测都作为研究对象，而只是针对其中的某一子集来抽样。比如如果我们只需要研究沪市主板的股票，那么只需要在相关的观测中抽取样本作为研究对象。Data选项后面可以使用where=语句来实现对总体观测的筛选。
*如果只想在沪市主板上抽取100个样本;
**proc****surveyselect**
data = Work.MainIndex_2012sea3(where=(Source ='沪市主板'))
out = Work.MainIndex_2012sea3_srs8
method = srs
sampsize =**100**
noprint
;
**run**;
**（2）分层抽样**
分层抽样是将总体按某种特征分为若干次级总体（层），再在每一层中进行随机抽样，把结果组成一个样本的方法。描述层次特征的变量称为分层变量，比如在我们的测试数据中，我们可以使用Source（来源板块）变量把原始数据分为沪市主板、深市主板、中小板、创业板四类（层）。Surveyselect过程步使用strata语句来指定分层变量。在抽样之前，需要对原始数据按照strata指定的分层变量进行排序。最简单的分层抽样场景是，最总体中的所有样本，指定一个分层变量，每一层都使用同样的抽样比例。以下是最简单分层抽样场景的代码：
*由于分层抽样需要对原始数据进行排序，因此我们再复制一张临时表;
**data**Work.MainIndex_2012sea3_tmp;
setWork.MainIndex_2012sea3;
**run**;
*按照分层变量Source排序;
**proc****sort**data=Work.MainIndex_2012sea3_tmp;bySource;
*用Source分层，每一层抽取10%的样本;
**proc****surveyselect**
data = Work.MainIndex_2012sea3_tmp
out = Work.MainIndex_2012sea3_strata1
method = srs
samprate =**0.1**
noprint;
strataSource;*使用Source作为分层变量;
**run**;
如果各层抽取的比例不一样，则应赋予samprate一个数组，数组的每一个元素的值分别代表各个层的抽样比例。数组元素的顺序需与分层变量排序后的顺序一致。
*用Source分层，一共有4层，各层抽取的比例不一样，在samprate中定义;
*分层变量Source的排序顺序是：创业板 沪市主板 深市主板 中小板;
**proc****surveyselect**
data = Work.MainIndex_2012sea3_tmp
out = Work.MainIndex_2012sea3_strata2
method = srs
samprate = (**0.1**,**0.3**,**0.5**,**0.2**)
noprint
;
strataSource;*使用Source作为分层变量;
**run**;
同样，也可以使用sampsize分别指定每一层的抽样个数。
*用Source分层，一共有4层，各层抽取的个数不一样，在sampsize中定义;
*分层变量Source的排序顺序是：创业板 沪市主板 深市主板 中小板;
**proc****surveyselect**
data = Work.MainIndex_2012sea3_tmp
out = Work.MainIndex_2012sea3_strata3
method = srs
sampsize = (**10**,**60**,**50**,**30**)
noprint;
strataSource;*使用Source作为分层变量;
**run**;
如果层数较多，且需要对不同层分别指定抽样比例或抽样个数，则需要建立抽样表。抽样表需要包含分层变量，以及每一层对应的抽样比例或抽样个数；如果是抽样比例，则变量必须命名为_rate_，如果是抽样个数，则变量必须命名为_nsize_。
*按比例分层抽样，建立抽样表;
**proc****sql**;
createtableWork.Samptab_rate (
Sourcechar(**10**),
_rate_num
);
insertintoWork.Samptab_ratevalues('创业板',**0.1**);
insertintoWork.Samptab_ratevalues('沪市主板',**0.3**);
insertintoWork.Samptab_ratevalues('深市主板',**0.5**);
insertintoWork.Samptab_ratevalues('中小板',**0.2**);
**quit**;
*按比例分层抽样，将抽样表赋值给samprate;
**proc****surveyselect**
data = Work.MainIndex_2012sea3_tmp
out = Work.MainIndex_2012sea3_strata4
method = srs
samprate = Work.Samptab_rate
noprint;
strataSource;*使用Source作为分层变量;
**run**;
*按个数分层抽样，建立抽样表;
**proc****sql**;
createtableWork.Samptab_size (
Sourcechar(**10**),
_nsize_num
);
insertintoWork.Samptab_sizevalues('创业板',**10**);
insertintoWork.Samptab_sizevalues('沪市主板',**60**);
insertintoWork.Samptab_sizevalues('深市主板',**50**);
insertintoWork.Samptab_sizevalues('中小板',**30**);
**quit**;
*按个数分层抽样，将抽样表赋值给sampsize;
**proc****surveyselect**
data = Work.MainIndex_2012sea3_tmp
out = Work.MainIndex_2012sea3_strata5
method = srs
sampsize = Work.Samptab_size
noprint;
strataSource;*使用Source作为分层变量;
**run**;
**（3）系统抽样**
系统抽样是把总体的个体进行排序，计算出抽样距离，然后按照这一固定的抽样距离抽取样本的方法。第一个样本采用简单随机抽样的办法抽取，此后每隔一个抽样距离的大小抽取一个样本。抽样距离等于总体容量除以样本容量。
*每隔10个抽取一个1个;
**proc****surveyselect**
data = Work.MainIndex_2012sea3
out = Work.MainIndex_2012sea3_sys1
method = sys
sampsize =**248**/*总体容量2472，样本容量248，意味着抽样距离为10 */
noprint;
**run**;
在系统抽样中，可以使用控制变量来对原始数据进行排序。控制变量使用control语句。SAS程序首先安装control中的变量排序，然后采用系统抽样抽取样本。
*每隔10个抽取一个1个;
*使用Source作为控制变量，这样程序会对输入数据按照Source进行排序;
**proc****surveyselect**
data = Work.MainIndex_2012sea3
out = Work.MainIndex_2012sea3_sys2
method = sys
sampsize =**248**
noprint;
control Source;
**run**;
下面的程序是将系统抽样与分层抽样相结合，实现较为复杂的抽样方式，以满足实际应用的需求。在这个例子中，程序按照strata指定的变量对原始数据进行分层，在每一层中使用control变量排序，然后分别进行系统抽样各抽取248个个体，因此，最终的结果有992条观测。

*分层系统抽样;
**proc****surveyselect**
data = Work.MainIndex_2012sea3_tmp
out = Work.MainIndex_2012sea3_sys3
method = sys
sampsize =**248**
noprint;
strataSource;
control EPS;
**run**;


**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)
———————————————————————————


