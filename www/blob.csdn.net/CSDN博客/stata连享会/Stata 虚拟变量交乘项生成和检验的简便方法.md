# Stata: 虚拟变量交乘项生成和检验的简便方法 - stata连享会 - CSDN博客





2018年12月25日 10:23:31[arlionn](https://me.csdn.net/arlionn)阅读数：1179
所属专栏：[Stata连享会](https://blog.csdn.net/column/details/27605.html)









> 
作者：胡杰 (中山大学岭南学院本科生) ([知乎](https://zhuanlan.zhihu.com/arlion) | [简书](http://www.jianshu.com/u/69a30474ef33) | [码云](https://gitee.com/arlionn))


Stata连享会 [精彩推文1](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文2](https://github.com/arlionn/stata/blob/master/README.md)
## 简介 虚拟变量（Dummy variables）和交乘项(Interaction)

在对有组别或者等级的数据进行处理时，常常需要利用虚拟变量和交乘项来探究各组之间或各等级之间的结构性的差异（Structural Difference）

## 例： 探究婚姻对女性工资造成的结构性的差异

```
sysuse nlsw88.dta, clear
sum
```

```
Variable |   Obs    Mean     Std. Dev.      Min        Max
-------------+------------------------------------------------
      idcode | 2,246  2612.654   1480.864         1       5159
         age | 2,246  39.15316   3.060002        34         46
        race | 2,246  1.282725   .4754413         1          3
     married | 2,246  .6420303   .4795099         0          1
never_marr~d | 2,246  .1041852   .3055687         0          1
-------------+------------------------------------------------
       grade | 2,244  13.09893   2.521246         0         18
    collgrad | 2,246  .2368655   .4252538         0          1
       south | 2,246  .4194123   .4935728         0          1
        smsa | 2,246  .7039181   .4566292         0          1
      c_city | 2,246  .2916296   .4546139         0          1
-------------+------------------------------------------------
    industry | 2,232  8.189516   3.010875         1         12
  occupation | 2,237  4.642825   3.408897         1         13
       union | 1,878  .2454739   .4304825         0          1
        wage | 2,246  7.766949   5.755523  1.004952   40.74659
       hours | 2,242  37.21811   10.50914         1         80
-------------+------------------------------------------------
     ttl_exp | 2,246  12.53498   4.610208  .1153846   28.88461
      tenure | 2,231   5.97785   5.510331         0   25.91667
```

### 基础模型（Basic Model）

$wage=\beta_0 +\beta_1tenure+\beta_2hours+\beta_3ttl\_exp+\epsilon$

### 添加虚拟变量及交乘项的复杂方法

```
gen marriedtenure = married*tenure
   	gen marriedhours = married*hours
   	gen marriedttl = married*ttl_exp
   	reg wage tenure hours ttl_exp married* 
   	test marriedtenure marriedhours marriedttl
```

```
Source |       SS           df       MS    
-------------+---------------------------------- 
       Model |  6140.31754         7  877.188219 
    Residual |  67880.4931     2,219  30.5905782 
-------------+---------------------------------- 
       Total |  74020.8106     2,226  33.2528349 

 Number of obs   =     2,227
  F(7, 2219)      =     28.68
  Prob > F        =    0.0000
  R-squared       =    0.0830
  Adj R-squared   =    0.0801
  Root MSE        =    5.5309
  
  -----------------------------------------------
         wage |      Coef.   Std. Err.      t  
--------------+--------------------------------
       tenure |   .1048823   .0412746     2.54 
        hours |   .0874067   .0222925     3.92 
      ttl_exp |   .2183548   .0515089     4.24 
      married |   1.029717    1.12407     0.92 
marriedtenure |   -.110726   .0532406    -2.08 
 marriedhours |  -.0418236   .0261311    -1.60 
   marriedttl |   .0869538   .0652744     1.33 
        _cons |   1.208404   .9551692     1.27 
-----------------------------------------------
------------------------------------------------
         wage |   P>|t|     [95% Conf. Interval]
--------------+---------------------------------
       tenure |   0.011     .0239415    .1858232
        hours |   0.000     .0436904    .1311231
      ttl_exp |   0.000     .1173441    .3193655
      married |   0.360    -1.174622    3.234056
marriedtenure |   0.038    -.2151326   -.0063194
 marriedhours |   0.110    -.0930675    .0094204
   marriedttl |   0.183    -.0410515     .214959
        _cons |   0.206    -.6647154    3.081522
------------------------------------------------

 ( 1)  marriedtenure = 0
 ( 2)  marriedhours = 0
 ( 3)  marriedttl = 0

       F(  3,  2219) =    2.31
            Prob > F =    0.0748
```

### 利用Factor Indicator 的便捷方法

Factor Indicator 的更多应用及详情请见于fvvarlist。

```
help fvvarlist
```

简便方式

```
global cx "tenure hours ttl_exp"
   reg wage i.married##c.($cx)
   testparm married married#c.($cx)
```

```
Source |       SS           df       MS    
-------------+---------------------------------- 
       Model |  6140.31754         7  877.188219 
    Residual |  67880.4931     2,219  30.5905782 
-------------+---------------------------------- 
       Total |  74020.8106     2,226  33.2528349 

  Number of obs   =     2,227
  F(7, 2219)      =     28.68
  Prob > F        =    0.0000
  R-squared       =    0.0830
  Adj R-squared   =    0.0801
  Root MSE        =    5.5309


----------------------------------------------------
             wage |      Coef.   Std. Err.      t   
------------------+---------------------------------
          married |
         married  |   1.029717    1.12407     0.92  
           tenure |   .1048823   .0412746     2.54  
            hours |   .0874067   .0222925     3.92  
          ttl_exp |   .2183548   .0515089     4.24  
                  |
 married#c.tenure |
         married  |   -.110726   .0532406    -2.08  
                  |
  married#c.hours |
         married  |  -.0418236   .0261311    -1.60  
                  |
married#c.ttl_exp |
         married  |   .0869538   .0652744     1.33  
                  |
            _cons |   1.208404   .9551692     1.27  
----------------------------------------------------

---------------------------------------------------
             wage |  P>|t|     [95% Conf. Interval]
------------------+--------------------------------
          married | 
         married  |  0.360    -1.174622    3.234056
           tenure |  0.011     .0239415    .1858232
            hours |  0.000     .0436904    .1311231
          ttl_exp |  0.000     .1173441    .3193655
                  | 
 married#c.tenure | 
         married  |  0.038    -.2151326   -.0063194
                  | 
  married#c.hours | 
         married  |  0.110    -.0930675    .0094204
                  | 
married#c.ttl_exp | 
         married  |  0.183    -.0410515     .214959
                  | 
            _cons |  0.206    -.6647154    3.081522
---------------------------------------------------

 ( 1)  1.married#c.tenure = 0
 ( 2)  1.married#c.hours = 0
 ( 3)  1.married#c.ttl_exp = 0

       F(  3,  2219) =    2.31
            Prob > F =    0.0748
```

### 注意此处应使用命令 testparm 而非 test

test 不支持 factor indicator 的#语法

若要用test，则需要改写为

```
test married married#c.tenture married#c.hours married#c.ttl_exp
```

这样则极为冗长和复杂。

## 总结
- 利用factor indicator 的语法极大的方便了虚拟变量交乘项的生成
- 在回归和检验中均可使用，注意test应用testparm命令替代
- 在自变量多的时候，该方法的便捷性更加明显
- 可以利用global 命令将其他需要交乘变量，放入一个全局暂元中，之后直接$引用就好，极大地减少代码的书写量

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

![](https://img-blog.csdnimg.cn/20181225102332275)

![欢迎加入Stata连享会(公众号: StataChina)](https://img-blog.csdnimg.cn/20181225102332297)





