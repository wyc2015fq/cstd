# Stata: 实时估计个股贝塔(beta)系数 - 简书


# Stata: 实时估计个股贝塔(beta)系数



> 
作者：连玉君 ([知乎](https://zhuanlan.zhihu.com/arlion) | [简书](https://www.jianshu.com/u/69a30474ef33) | [码云](https://gitee.com/arlionn))


> 
Stata连享会 [精彩推文1](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文2](https://github.com/arlionn/stata/blob/master/README.md)


## 目的

使用 `cntrade` 命令下载个股和指数日资料，合并后估计 CAPM 模型，以便得到个股的**贝塔(beta)系数**，进而对个股的 **贝塔(beta)系数** 进行统计分析。

## 1. 下载个股交易资料

### 1.1 下载方法

**特别说明：** - 后文使用外部命令 `cntrade` 下载某只股票的日交易资料。请确保使用最新版的 `cntrade` 命令。有两个版本来源，请自行测试哪个版本可用：
- **版本1：ssc**： 使用 `ssc install cntrade, replace` 命令下载最新版命令。
- **版本2：github**：
- 使用 `ssc install github, replace` 下载 `github` 命令 (若已安装，请忽略此步骤)；
- 使用 `github install Stata-club/cntrade, replace` 下载最新版的 `cntrade` 命令。


只需输入如下命令即可下载 **贵州茅台** 自上市起至昨日的日交易数据，下载后的数据自动存储于当前工作路径下，名称为 “**600519.dta**”：

```
cntrade 600519
```

数据详情如下：

```
. use "600519.dta", clear
. describe
Contains data from d:\cnstock\beta\data/600519.dta
  obs:         3,956                          
 vars:            13                        12 Apr 2018 09:17
 size:       395,600                          
--------------------------------------------------------------
              storage      
variable name   type       variable label
--------------------------------------------------------------
stkcd           long       Stock Code
date            double     Trading Date
stknme          str12      Stock Name
clsprc          double     Closing Price
hiprc           double     Highest Price
lowprc          double     Lowest Price
opnprc          double     Opening Price
rit             double     Daily Return
turnover        double     Turnover rate
volume          long       Trading Volume
transaction     double     Trading Amount in RMB
tcap            double     Total Market Capitalization
mcap            double     Circulation Market Capitalization
--------------------------------------------------------------
Sorted by: date
```

### 1.2 Stata 范例

```
global id="600519"    //600519  贵州茅台
  cntrade $id
  global nn = stknme[1]   //股票简称
  
*-股价时序图
  use "$id.dta", clear
  tsset date  //设定为时间序列变量
  tsline clsprc , xlabel(,angle(60)) subtitle("$nn $id")
  graph export "$out\price_$id.png", replace
```

输出图片为：

> 




![](https://upload-images.jianshu.io/upload_images/7692714-2d52b56a2942fce5.png)

贵州茅台-600519-股价走势-Stata连享会.png




## 2. 下载指数数据

### 2.1 基本下载方法

只需在 `cntrade` 命令后附加 `index` 选项即可下载如下指数：
- 000001   The Shanghai Composite Index (上证综合指数).
- 000300   CSI 300 Index (沪深 300).
- 399001   Shenzhen Component Index (深成指).

例如：

```
.   cntrade 399001, index
d:\cnstock\beta\data
  File tempcsvfile.csv (text file)
file 399001.dta has been saved

.   use "399001.dta", clear
.   describe
Contains data from 399001.dta
  obs:         6,587                          
 vars:            10         12 Apr 2018 09:26
 size:       526,960                          
----------------------------------------------------
              storage   
variable name   type       variable label
----------------------------------------------------
indexcd         long       Index Code
date            double     Trading Date
indexnme        str12      Index Name
clsprc          double     Closing Price
hiprc           double     Highest Price
lowprc          double     Lowest Price
opnprc          double     Opening Price
rmt             double     Daily Return
volume          double     Trading Volume
transaction     double     Trading Amount in RMB
----------------------------------------------------
Sorted by: date
```

### 2.2 批量下载多个指数并做进一步处理

```
foreach i in "399001" "000001" "000300" {
    cntrade `i', index
    rename rmt retmkt_`i'  //更该指数收益名称，便于与个股合并
    save index_`i'.dta, replace
    erase `i'.dta
  }
```

### 2.3 Stata 范例：沪深 300 时序图

```
* 调用其中一个指数数据：
  use index_000300.dta, clear // 沪深300指数
  tsset date
  tsline clsprc , xlabel(,angle(60)) subtitle("沪深300指数")
```

> 




![](https://upload-images.jianshu.io/upload_images/7692714-37ca8ca0182eb71b.png)

沪深300指数-Stata连享会




## 3. 合并个股和指数数据

```
use "index_399001.dta", clear
   
   merge 1:1 date using "$id.dta", nogen
   
   gen year = year(date)
   order date year index* stk* ret*

   save "merge_$id.dta", replace
```

## 4. 估计 CAPM 模型

### 4.1 基本模型

```
use "merge_$id.dta", clear
  *winsor2 rit, replace  // winsor 处理
  reg rit retmkt
```

结果如下：

```
.   reg rit retmkt

      Source |       SS           df       MS      Number of obs   =     3,956
-------------+----------------------------------   F(1, 3954)      =   1246.43
       Model |  .415684439         1  .415684439   Prob > F        =    0.0000
    Residual |  1.31865845     3,954    .0003335   R-squared       =    0.2397
-------------+----------------------------------   Adj R-squared   =    0.2395
       Total |  1.73434289     3,955  .000438519   Root MSE        =    .01826

-------------------------------------------------------------------------------
          rit |      Coef.   Std. Err.      t    P>|t|     [95% Conf. Interval]
--------------+----------------------------------------------------------------
retmkt_399001 |      0.565      0.016    35.30   0.000        0.533       0.596
        _cons |      0.001      0.000     4.42   0.000        0.001       0.002
-------------------------------------------------------------------------------
```

### 4.2 散点图和拟合情况

```
#delimit ;
  twoway 
    (scatter rit retmkt if year<2008&abs(retmkt)<0.4,  
       xline(0,lp(dash) lc(red)) 
       yline(0,lp(dash) lc(red)) msize(*0.2))
    (lfit rit retmkt);
  graph export "$out\scatter_$id.png", replace  ;
  #d cr
```

> 




![](https://upload-images.jianshu.io/upload_images/7692714-49753012c0f51f9d.png)

scatter_600519-Stata连享会.png




### 4.3 分时段估计

#### 1998-2007

```
global yr = 2008
  reg rit retmkt if year<$yr //&abs(retmkt)<0.4  // OLS
  
  aaplot rit retmkt if e(sample), xline(0,lp(dash) lc(red)) ///
        yline(0,lp(dash) lc(red)) msize(*0.6) ///  
        title("$nn ($id) 的 Beta 系数, 1998-2007") ///
        xtitle("市场指数收益率(深成指)")
  graph export "$out\aaplot_$id_b$yr.png", replace  
  *-下载 regfit 命令
  . github install arlionn/regfit  // 若已经有该命令，可以忽略本行命令
  regfit, f(%4.2f) tvalue   //呈现拟合方程
```





![](https://upload-images.jianshu.io/upload_images/7692714-bc56643d336d9e66.png)

贵州茅台beta系数：1998-2007


```
.   regfit, f(%4.2f) tvalue   //呈现拟合方程

rit =  0.00 + 0.61*retmkt_399001
       (3.90) (20.44)
       N = 1466, R2 = 0.22, adj-R2 = 0.22
Note: t-value in parentheses
```

#### 2008-2018

```
reg rit retmkt if year>=$yr //&abs(retmkt)<0.4  // OLS
  aaplot rit retmkt if e(sample), xline(0,lp(dash) lc(red)) ///
        yline(0,lp(dash) lc(red)) msize(*0.6)    ///
        title("$nn ($id) 的 Beta 系数, 2008-2017") ///
        xtitle("市场指数收益率(深成指)")
  graph export "$out\aaplot_$id_a$yr.png", replace
  
  regfit, f(%4.2f) tvalue   //呈现拟合方程
```





![](https://upload-images.jianshu.io/upload_images/7692714-09dac4e07ee4bc6c.png)

贵州茅台beta系数：2008-2018


```
.   regfit, f(%4.2f) tvalue   //呈现拟合方程        

rit =  0.00 + 0.54*retmkt_399001
       (2.44) (28.93)
       N = 2490, R2 = 0.25, adj-R2 = 0.25
Note: t-value in parentheses
```

## 5. 分析个股 Beta 系数变化特征

### 5.1 逐年回归

```
.  bysort year: reg rit retmkt, noheader
-------------------------------------------------------------------------------
-> year = 1998
no observations

-------------------------------------------------------------------------------
-> year = 1999
no observations

-------------------------------------------------------------------------------
-> year = 2000
no observations

-------------------------------------------------------------------------------
-> year = 2001
-------------------------------------------------------------------------------
          rit |      Coef.   Std. Err.      t    P>|t|     [95% Conf. Interval]
--------------+----------------------------------------------------------------
retmkt_399001 |      0.598      0.117     5.09   0.000        0.364       0.831
        _cons |      0.004      0.002     1.65   0.102       -0.001       0.008
-------------------------------------------------------------------------------

                     (略)

-------------------------------------------------------------------------------
-> year = 2017
-------------------------------------------------------------------------------
          rit |      Coef.   Std. Err.      t    P>|t|     [95% Conf. Interval]
--------------+----------------------------------------------------------------
retmkt_399001 |      0.624      0.118     5.28   0.000        0.392       0.857
        _cons |      0.003      0.001     2.90   0.004        0.001       0.005
-------------------------------------------------------------------------------

-------------------------------------------------------------------------------
-> year = 2018
-------------------------------------------------------------------------------
          rit |      Coef.   Std. Err.      t    P>|t|     [95% Conf. Interval]
--------------+----------------------------------------------------------------
retmkt_399001 |      0.731      0.158     4.64   0.000        0.416       1.047
        _cons |      0.001      0.002     0.30   0.764       -0.004       0.005
-------------------------------------------------------------------------------
```

### 逐年回归: 更为简洁的处理方法

```
statsby _b[retmkt], by(year) saving("beta_data.dta", replace): ///
          reg rit retmkt
```

##### 变化趋势

```
*-变化趋势      
    preserve
    use "beta_data.dta", clear
    keep if _stat!=.
    qui sum year
    local byr = r(min)
    local eyr = r(max)
    list
    #d ;
    twoway connect _stat year,  
           yline(1, lpattern(dash) lcolor(red*0.3)) 
         xlabel(`byr'(2)`eyr')
         ylabel(,angle(0) format(%2.1f)) 
         subtitle("$nn ($id) 的Beta系数")
         ytitle("beta") xtitle("");
    #d cr
    graph export "$out\beta $nn $id.png", replace
  restore
```





![](https://upload-images.jianshu.io/upload_images/7692714-fb0eaf54b5b3bf65.png)

beta 贵州茅台 600519


### 附：完整命令

```
*         =============================
*         
*          Stata 连享会: 证券分析系列 
*
*                 2. Beta 系数
* 
*         =============================




*-注意：执行后续命令之前，请先执行如下命令
  global path "d:\cnstock\beta"  //只需修改自路径即可，其他文件夹会自动生成
  cd $path
  cap mkdir data
  cap mkdir refs
  cap mkdir out
  cap mkdir adofiles  
  global D    "$path\data"      //范例数据
  global R    "$path\refs"      //参考文献
  global out  "$path\out"       //图形和表格结果
  adopath +   "$path\adofiles"  //添加外部命令所在路径  
  cd $D

  set scheme s2color 
  
/*  
*------------------
*- 0 安装外部命令
*------------------

  *-设定安装外部路径的位置
    net set ado "$path\adofiles"
    net set other "$D"  //外部命令附带的 example datasets 等存放于此
    
  *-安装外部命令
    ssc install cntrade, replace //下载个股和市场指数实时数据, SJ 14(2):381--388
    ssc install openall, replace //批量合并
    ssc install aaplot, replace 
    ssc install regfit, replace  
*/

  
  
*---------------------
*- 1 下载个股交易资料
*---------------------

  *-请输入股票代码
  *-股票代码查询
    view browse "http://quote.stockstar.com/stock/stock_index.htm"
    *-部分股票代码
/*
    global id="000002"    //000002  万科
    global id="600028"    //600028  中石化
    global id="000527"    //000527  美的电器,已经退市
    global id="000651"    //000651  格力电器 
    global id="000429"    //000429  粤高速A  
    global id="000541"    //000541  佛山照明  
    global id="000596"    //000596  古井贡酒  
    global id="000799"    //000799  酒鬼酒  
    global id="600048"    //600048  保利地产 2006年07月31日
    global id="600115"    //600115  东方航空 
    global id="600085"    //600085  同仁堂
    global id="600249"    //600249  两面针
    global id="300398"    //300398  飞凯材料    2014年10月09日
    global id="603799"    //603799  华友钴业,2015年01月29日
    global id="300431"    //300431  暴风影音, 2015年03月24日
*/
    
    global id="600519"    //600519  贵州茅台
    
    cntrade $id
    global nn = stknme[1]   //股票简称
    
    use "$id.dta", clear
    tsset date  //设定为时间序列变量
    *-股价时序图
    tsline clsprc , xlabel(,angle(60)) subtitle("$nn $id")
    graph export "$out\price_$id.png", replace

*------------------
*- 2 下载指数数据
*------------------ 

  *  -Index Codes and Index Names:
  *
  *   000001   The Shanghai Composite Index (上证综合指数).
  *   000300   CSI 300 Index (沪深 300).
  *   399001   Shenzhen Component Index (深成指).
    
    foreach i in "399001" "000001" "000300" {
      cntrade `i', index
      rename rmt retmkt_`i'
      save index_`i'.dta, replace
      erase `i'.dta
    } 

/* 调用其中一个指数数据：
    use index_000300.dta, clear // 沪深300指数
    tsset date
    tsline clsprc , xlabel(,angle(60)) subtitle("沪深300指数")
*/


*-----------------------
*- 3 合并个股和指数数据
*-----------------------

   use "index_399001.dta", clear
   
   merge 1:1 date using "$id.dta", nogen
   
   gen year = year(date)
   order date year index* stk* ret*

   save "merge_$id.dta", replace
    
    
*---------------------------------
*- 4 估计CAPM模型并分析 Beta 系数
*---------------------------------

*-估计CAPM模型
  
  use "merge_$id.dta", clear
  *winsor2 reti, replace  // winsor 处理
  reg rit retmkt
    
  #delimit ;
  twoway 
    (scatter rit retmkt if year<2008&abs(retmkt)<0.4,  
       xline(0,lp(dash) lc(red)) 
       yline(0,lp(dash) lc(red)) msize(*0.2))
    (lfit rit retmkt);
  #d cr 
        
    // 图示拟合情况   
    global yr = 2008
    reg rit retmkt if year<$yr //&abs(retmkt)<0.4  // OLS
    
    aaplot rit retmkt if e(sample), xline(0,lp(dash) lc(red)) ///
            yline(0,lp(dash) lc(red)) msize(*0.6) ///  
            title("$nn ($id) 的 Beta 系数, 1998-2007") ///
            xtitle("市场指数收益率(深成指)")
    graph export "$out\aaplot_$id_b$yr.png", replace    
    
    regfit, f(%4.2f) tvalue   //呈现拟合方程
    
    reg rit retmkt if year>=$yr //&abs(retmkt)<0.4  // OLS
    aaplot rit retmkt if e(sample), xline(0,lp(dash) lc(red)) ///
            yline(0,lp(dash) lc(red)) msize(*0.6)    ///
            title("$nn ($id) 的 Beta 系数, 2008-2017") ///
            xtitle("市场指数收益率(深成指)")
    graph export "$out\aaplot_$id_a$yr.png", replace
    
    regfit, f(%4.2f) tvalue   //呈现拟合方程  
    
    
    *-逐年回归
    bysort year: reg rit retmkt, noheader
    
    *-逐年统计
    statsby _b[retmkt], by(year) saving("beta_data.dta", replace): ///
          reg rit retmkt
          
    *-变化趋势    
    preserve
      use "beta_data.dta", clear
      keep if _stat!=.
      qui sum year
      local byr = r(min)
      local eyr = r(max)
      list
      #d ;
      twoway connect _stat year,  
             yline(1, lpattern(dash) lcolor(red*0.3)) 
             xlabel(`byr'(2)`eyr')
             ylabel(,angle(0) format(%2.1f)) 
             subtitle("$nn ($id) 的Beta系数")
             ytitle("beta") xtitle("");
      #d cr
      graph export "beta $nn $id.wmf", replace
    restore     
  *--------------------------over------------------------
```

> 
#### 关于我们

- 【**Stata 连享会(公众号：StataChina)**】由中山大学连玉君老师团队创办，旨在定期与大家分享 Stata 应用的各种经验和技巧。
- 公众号推文同步发布于 [【简书-Stata连享会】](https://www.jianshu.com/u/69a30474ef33) 和 [【知乎-连玉君Stata专栏】](https://zhuanlan.zhihu.com/arlion)。可以在**简书**和**知乎**中搜索关键词`Stata`或`Stata连享会`后关注我们。
- 点击推文底部【阅读原文】可以查看推文中的链接并下载相关资料。

> 
Stata连享会 [精彩推文1](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文2](https://github.com/arlionn/stata/blob/master/README.md)


> 
#### 联系我们

- **欢迎赐稿：** 欢迎将您的文章或笔记投稿至`Stata连享会(公众号: StataChina)`，我们会保留您的署名；录用稿件达`五篇`以上，即可**免费**获得 Stata 现场培训 (初级或高级选其一) 资格。
- **意见和资料：** 欢迎您的宝贵意见，您也可以来信索取推文中提及的程序和数据。
- **招募英才：** 欢迎加入我们的团队，一起学习 Stata。合作编辑或撰写稿件五篇以上，即可**免费**获得 Stata 现场培训 (初级或高级选其一) 资格。
- **联系邮件：**[StataChina@163.com](mailto:StataChina@163.com)

> 
#### 往期精彩推文


[Stata连享会推文列表](https://www.jianshu.com/p/de82fdc2c18a)

> 
Stata连享会 [精彩推文1](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文2](https://github.com/arlionn/stata/blob/master/README.md)






![](https://upload-images.jianshu.io/upload_images/7692714-85a56a28aa8caecd.jpg)

欢迎加入Stata连享会(公众号: StataChina)








