# Stata 外部命令：最常用和最新的命令 - stata连享会 - CSDN博客





2017年09月08日 10:25:11[arlionn](https://me.csdn.net/arlionn)阅读数：5390








> 
作者：连玉君 （中山大学岭南学院）


原载：[连玉君知乎专栏](https://zhuanlan.zhihu.com/p/28425870)- Stata连享会 [精品专题](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文](https://github.com/arlionn/stata/blob/master/README.md)
![](https://upload-images.jianshu.io/upload_images/7692714-8b1fb0b5068487af.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
## ### 文章目录- [@[toc]](#toc_13)- [1 提要](#1__17)- [2 外部命令清单](#2__24)- [3 最常用的外部命令](#3__29)- [4 最新发布的外部命令](#4__94)- [5 其他说明](#5__157)

## 1 提要

> 
时至今日，stata 已经发布了第15版，功能不断增强。然而，勤奋的 stata 用户们每天仍然在开发新的程序，不断缩小理论计量与实际应用之间的差距。下面，我们就梳理一下 stata 外部命令的相关资源，包括：从哪些地方获取外部命令？哪些外部命令是最常用的，最流行的？哪些是最新发布的？关注一下这些外部命令，可以大幅提高我们的分析效率。


获取外部命令的最佳方式是使用 - findit - 命令，在搜索完成后，可以按照 stata 的提示直接下载安装相应的命令和作者提供的范例数据或dofiles(如果有的话)。详情可以参考：[Stata | Adding user-written commands](http://link.zhihu.com/?target=http%3A//www.stata.com/support/add-user-written-commands/)。

## 2 外部命令清单

Stata官网上给出了一份完整的 [外部命令列表](http://link.zhihu.com/?target=http%3A//www.stata.com/links/resources2.html)。

## 3 最常用的外部命令

输入 `ssc hot, n(10)`  可以呈现过去三个月关注度最高的 10 个命令：

```
May 2017                  
Rank  # hits    Package     Author(s)
----------------------------------------------------
        Aug 2018   
  Rank   # hits    Package       Author(s)
  ----------------------------------------------------------------------
     1  331271.0    findname      Nicholas J. Cox                         
     2  19504.6    outreg2       Roy Wada                                
     3  18223.6    estout        Ben Jann                                
     4  11066.7    distinct      Gary Longton, Nicholas J. Cox           
     5   7746.3    winsor        Nicholas J. Cox                         
     6   6881.7    winsor2       Lian Yu-jun                             
     7   6598.7    ivreg2        Mark E Schaffer, et al.                      
     8   6579.3    ivreg210      Mark E Schaffer, et al.                        
     9   6571.1    ivreg28       Mark E Schaffer, et al.                         
    10   6561.8    ivreg29       Christopher F Baum, et al.                        
----------------------------------------------------
(Click on package name for description)
```

可以看出，用于输出回归结果和统计表格的 -`outreg2`- 以及 -`estout`- 命令分别位列第 2 和第 2；处理离群值时最常用的缩尾处理命令 -`winsor`- 和 -`winsor2`- 命令则分别位列第 5 和 6。

这些命令都可以使用 -`ssc install`- 命令直接安装到你的电脑里。当然，也可以直接点击命令名称（`stata` 窗口中显示为蓝色，带连接），在如下弹出界面中点击 `click here to install` 即可。

```
--------------------------------------------------
package winsor2 
from http://fmwww.bc.edu/RePEc/bocode/w
--------------------------------------------------
TITLE
      'WINSOR2': module to winsorize data

DESCRIPTION/AUTHOR(S)

   winsor2 can winsorize a varlist, 
   operate with the by prefix,
   and offers a replace option.

   KW: winsor
   KW: winsorize
   KW: data management

   Requires: Stata version 8

   Distribution-Date: 20141222

   Author: Lian Yu-jun, Department of Finance, 
          Sun Yat-Sen University, China
   Support: email arlionn@163.com


INSTALLATION FILES     (click here to install)
      winsor2.ado
      winsor2.sthlp
--------------------------------------------------
(click here to return to the previous screen)
```

## 4 最新发布的外部命令

若想了解最近一个月发布了哪些新的外部命令，只需输入 `ssc new` 即可。检索页面中显示了几十个新命令，下面选择几个比较有趣的，简单解释一下。
- `ddid` 命令用于实现更为一般化的 DID 模型，主要特征在于允许多期政策冲击，且政策冲击可以发生在不同时点上。
- `CORR2DOCX` 是中南财经政法的李春涛老师发布的，用于将 Spearman 和 Pearson 相关系数输出到 word 文档中。
- `TWITTER2STATA` 则可以把 Twitter （推特儿）上的数据直接下载到 stata 中。
- `XTGCAUSE` 命令使用 Dumitrescu & Hurlin (Economic Modelling, 2012) 提出的方法，检验异质性面板中是否存在 Granger 因果关系。

```
SSC Stata modules created or 
revised 2017-07-09 to 2017-08-09
--------------------------------
THSEARCH
 module to evaluate threshold search model for non-linear models 
 based on information criterion
 Authors: Ho Fai Chan  Brenda Gannon  David Harris  Mark Harris       
 Req: Stata version 7
 Created: 2017-08-05

DDID
 module to compute pre- and post-treatment estimation of the  
 Average Treatment Effect (ATE) with binary time-varying treatment
 Authors: Giovanni Cerulli       
 Req: Stata version 14
 Created: 2017-07-31

CORR2DOCX
 module to report Pearson & Spearman correlation coefficients 
 to formatted table in DOCX file
 Authors: Chuntao Li   Zijian Li  Yuan Xue       
 Req: Stata version 15
 Revised: 2017-08-04

SICFF
 module to create Fama French Industry Variable from SIC Code
 Authors:   Tyson Van Alfen       
 Req: Stata version 10
 Created: 2017-07-24

TWITTER2STATA
 module to import data from Twitter
 Authors: Kevin Crow       
 Req: Stata version 15
 Revised: 2017-07-31

SMCLPRES
 module to create a SMCL presentation from a do file
 Authors: Maarten L. Buis       
 Req: Stata version 8
 Revised: 2017-07-16

XTGCAUSE
 module to test for Granger non-causality in heterogeneous panels
 Authors: Luciano Lopez  Sylvain Weber       
 Req: Stata version 13.1
 Revised: 2017-07-31
```

若想查看 ssc 发布的所有外部命令，可以通过浏览网页 [Boston College Department of Economics](https://ideas.repec.org/s/boc/bocode.html)， 亦可在 Stata 命令窗口中查看，命令为：`net from Index of /RePEc/bocode`

## 5 其他说明
- 如需查看已经安装的所有外部命令，只需输入 - `ado` - 命令即可；
- 更新这些外部命令，可以使用 -`adoupdate`-；
- 卸载这些外部命令，可以使用 -`ado uninstall`- ；
- 除了 [SSC](https://www.stata.com/support/ssc-installation/)，[UCLA](https://stats.idre.ucla.edu/) 的网站上也提供了不少 Stata 外部命令（这些命令都可以使用 `findit` 命令在 Stata 命令窗口中搜索到）。
- UCLA 的 [Stata 专栏](http://www.ats.ucla.edu/stat/stata) 中提供了大量学习 Stata 的资源。

> 
作者：连玉君 （中山大学岭南学院金融系副教授）

网络课程：  [连玉君在线 Stata 课程](http://www.peixun.net/author/3.html)   |   [连玉君优酷 Stata 公开课](http://i.youku.com/arlion)     | [专题课程](https://gitee.com/arlionn/stata_training/blob/master/README.md)


![Stata连享会二维码](http://wx1.sinaimg.cn/mw690/8abf9554gy1fj9p14l9lkj20m30d50u3.jpg)






