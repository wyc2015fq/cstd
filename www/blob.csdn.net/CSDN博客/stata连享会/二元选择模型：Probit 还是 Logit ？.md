# 二元选择模型：Probit 还是 Logit ？ - stata连享会 - CSDN博客





2018年12月10日 17:18:49[arlionn](https://me.csdn.net/arlionn)阅读数：3691








> 
作者：黄彩虹 [编译] ([知乎](https://zhuanlan.zhihu.com/arlion) | [简书](http://www.jianshu.com/u/69a30474ef33) | [码云](https://gitee.com/arlionn))


#### Stata连享会 [Stata现场班报名中……](https://gitee.com/arlionn/stata_training/blob/master/README.md)

- Stata连享会 [精品专题](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文](https://github.com/arlionn/stata/blob/master/README.md)
![](https://upload-images.jianshu.io/upload_images/7692714-8b1fb0b5068487af.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
> 
Source:
- [Probit or Logit: ladies and gentlemen, pick your weapon](https://blog.stata.com/2016/01/07/Probit-or-Logit-ladies-and-gentlemen-pick-your-weapon/)
- [regress, Probit, or Logit?](https://blog.stata.com/2016/01/14/regress-Probit-or-Logit/)





## 1 解读 Probit 模型和 Logit 模型的边际效应估计

对于二元被解释变量的分析常采用 **Probit** 或 **Logit** 模型。就模型设定而言，Logit 模型更简单。不过，借助 Stata 中的 `margins`  命令，上述两个模型都易于实现。两种模型估计将返回同样的结果，可以根据习惯或偏好来选用。

下面通过研究人员感兴趣的一系列效应来展示 Probit 和 Logit 模型的相同拟合结果。主要考虑连续协变量与离散协变量的变化对正向估计结果的影响，计算作用效应的平均值及协变量的均值。换句话说，我研究了平均边际效应（AME），平均处理效应（ATE），协变量均值处的边际效应（MEM）、协变量均值处的处理效应（TEM）。

### 1.1 估计结果

表1给出了真实数据生成过程符合 Probit 模型假定的估计结果，并给出 AME 和 ATE 估计的均值以及在原假设 5% 拒绝概率下的 Probit 和 Logit 估计。同时使用 2 千万观测样本，计算了 AME 和 ATE 的近似真实值，以及系数的真实值。



> 
表1： AME和ATE ：真实 DGP Probit


N=10000, k=4000 次重复的拟合结果

**DGP**: Data Generation Process (数据生成过程)
|统计量|Approximate True Value|Probit|Logit|
|----|----|----|----|
|x1的AME|-.1536|-.1537|-.1537|
|5%拒绝概率||.050|.052|
|x2的ATE|.1418|.1417|.1417|
|5%拒绝概率||.050|.049|



> 
表2： MEM 和 TEM ：真实 DGP Probit


N=10000, k=4000 次重复的拟合结果
|统计量|Approximate True Value|Probit|Logit|
|----|----|----|----|
|x1的MEM|-.1672|-.1673|-.1537|
|5%拒绝概率||.056|.06|
|x2的TEM|.1499|.1498|.1471|
|5%拒绝概率||.053|.058|

Logit 估计值接近真实值，拒绝率接近 5% 。当真实 DGP 满足 Probit 模型的假设时，用 Logit 对模型参数进行拟合不会带来偏误。如果真实 DGP 满足 Logit 模型的假设，结论是一致的，表 3 -表 4 给出了估计结果。



> 
表3：平均边际和处理效应：真实 DGP Logit


N=10000, k=4000 次重复的拟合结果
|统计量|Approximate True Value|Probit|Logit|
|----|----|----|----|
|x1的AME|-.1090|-.1088|-.1089|
|5%拒绝概率||.052|.052|
|x2的ATE|.1046|.1044|.1045|
|5%拒绝概率||.053|.051|



> 
表4： MEM 和 TEM ：真实 DGP Logit


N=10000, k=4000次重复的拟合结果
|统计量|Approximate True Value|Probit|Logit|
|----|----|----|----|
|x1的MEM|-.1146|-.1138|-.1146|
|5%拒绝概率||.050|.051|
|x2的TEM|.1086|.1081|.1085|
|5%拒绝概率||.058|.058|



### 1.2 原因是什么？

从上面的蒙特卡洛模拟分析结果可以看出，Logit 和 Probit 估计得到的结果非常接近。

极大似然估计为我们寻找使得数据符合分布假设的参数。所选择的似然是对真实似然的近似，如果二者接近的话，这就是一种有效的近似。将基于似然的模型视为有用的近似，而不是作为真实似然的模型，是拟似然理论的基础。详见 White(1996) 和 Wooldridge(2010) 。假设 Probit 模型和 Logit 模型中的不可观测随机变量分别来自标准正态分布和 Logistic 分布。这两种情况下的累积分布函数 (CDFs) 相互接近，特别是在平均值附近。因此，这两组假设下的估计量产生了相似的结果。为了说明这些论点，我们可以画出这两支 CDF 及其不同之处：
![图1](http://upload-images.jianshu.io/upload_images/7692714-d9536dbd207ad0d5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

从左侧或右侧靠近均值时，两种分布函数的差异接近于 0 且始终小于 0.15 。
### 1.3 模特卡洛模拟分析过程解析

下面是进行模拟的代码。
- 第一部分，即 4-12 行，生成满足 Probit 模型假设的结果变量 y1 ,及满足 Logit 模型假设的结果变量 y2 。
- 第二部分，即 13-16 行，计算 Logit 模型和 Probit 模型的边际效应。对于离散协变量，边际效应是一种处理效应。
- 第三部分，即 17-25 行，计算均值处估计的边际效应。后面将用这些估计值来对这些影响的真实值进行近似估计。

```
program define mkdata
        syntax, [n(integer 1000)]
        clear
        quietly set obs `n'
        // 1. Generating data from Probit, Logit, and misspecified 
        generate x1    = rnormal()
        generate x2    = rbeta(2,4)>.5
        generate e1    = rnormal()
        generate u     = runiform()
        generate e2    = ln(u) -ln(1-u)
        generate xb    = .5*(1 -x1 + x2)
        generate y1    =  xb + e1 > 0
        generate y2    =  xb + e2 > 0
        // 2. Computing Probit & Logit marginal and treatment effects 
        generate m1    = normalden(xb)*(-.5)
        generate m2    = normal(1 -.5*x1 ) - normal(.5 -.5*x1)
        generate m1l   = exp(xb)*(-.5)/(1+exp(xb))^2
        generate m2l   = exp(1 -.5*x1)/(1+ exp(1 -.5*x1 )) - ///
                         exp(.5 -.5*x1)/(1+ exp(.5 -.5*x1 ))
        // 3. Computing Probit & Logit marginal and treatment effects at means
        quietly mean x1 x2 
        matrix A         = r(table)
        scalar a         = .5 -.5*A[1,1] + .5*A[1,2]
        scalar b1        =  1 -.5*A[1,1]
        scalar b0        = .5 -.5*A[1,1]
        generate mean1   = normalden(a)*(-.5)
        generate mean2   = normal(b1) - normal(b0)
        generate mean1l  = exp(a)*(-.5)/(1+exp(a))^2
        generate mean2l  = exp(b1)/(1+ exp(b1)) - exp(b0)/(1+ exp(b0))
end
```

我用 2000 万样本观测数据近似估计了真实边际效应，在这个例子中是一种合理策略。例如，对 Probit 模型中连续的协变量 xk 取平均边际效应：

$\frac{1} {N}\sum_{i=1}^N\phi(x_i\beta)\beta_k \qquad (1)$

上式是对 $E(\phi(x_i\beta)\beta_k)$ 的近似估计，为了获得这个期望值，需要对所有协变量的分布进行整合。这并不实际，且会限制协变量的选择。因此，采用2000万样本观测数据的 $\frac{1} {N}\sum_{i=1}^N\phi(x_i\beta)\beta_k$ 来代替，把它视作真实值。对于其他边际效应，遵循相同的逻辑。下面是计算近似真实边际效应的代码。调用2000万的观测结果，计算了在模拟中将使用的平均值，并为每一个近似的真实值创建了暂元。

```
. mkdata, n(20000000)

. local values "m1 m2 m1l m2l mean1 mean2 mean1l mean2l"

. local means  "mx1 mx2 mx1l mx2l meanx1 meanx2 meanx1l meanx2l"

. local n : word count `values'

. forvalues i= 1/`n' {
  2.         local a: word `i' of `values'
  3.         local b: word `i' of `means'
  4.         sum `a', meanonly
  5.         local `b' = r(mean)
  6. }
```

现在可以进行以上部分用于生成结果的所有估计了。当真实 DGP 符合 Probit 模型时估计 ATE 和 AM 的命令是：

```
. postfile mProbit y1p y1p_r y1l y1l_r y2p y2p_r y2l y2l_r ///
                   using simsmProbit, replace 

. forvalues i=1/4000 {
  2.  quietly {
  3.     mkdata, n(10000)
  4.     Probit y1 x1 i.x2, vce(robust)
  5.     margins, dydx(*) atmeans post 
  6.     local y1p = _b[x1]
  7.     test _b[x1] = `meanx1'
  8.     local y1p_r   = (r(p)<.05) 
  9.     local y2p = _b[1.x2]
 10.     test _b[1.x2] = `meanx2'
 11.     local y2p_r   = (r(p)<.05) 
 12.     Logit  y1 x1 i.x2, vce(robust)
 13.     margins, dydx(*) atmeans post 
 14.     local y1l = _b[x1]
 15.     test _b[x1] = `meanx1'
 16.     local y1l_r   = (r(p)<.05) 
 17.     local y2l = _b[1.x2]
 18.     test _b[1.x2] = `meanx2'
 19.     local y2l_r   = (r(p)<.05) 
 20.     post mProbit (`y1p') (`y1p_r') (`y1l') (`y1l_r') ///
                 (`y2p') (`y2p_r') (`y2l') (`y2l_r')
 21.  }
 22. }
```

```
. use simsProbit
. summarize

    Variable |        Obs        Mean    Std. Dev.       Min        Max
-------------+---------------------------------------------------------
         y1p |      4,000   -.1536812    .0038952  -.1697037  -.1396532
       y1p_r |      4,000         .05    .2179722          0          1
         y1l |      4,000   -.1536778    .0039179  -.1692524  -.1396366
       y1l_r |      4,000      .05175    .2215496          0          1
         y2p |      4,000     .141708    .0097155   .1111133   .1800973
-------------+---------------------------------------------------------
       y2p_r |      4,000       .0495    .2169367          0          1
         y2l |      4,000    .1416983    .0097459   .1102069   .1789895
       y2l_r |      4,000        .049     .215895          0          1
```

对于真实 DGP 符合 Probit 模型的 MEM 和 TEM ，使用包含 `atmeans` 选项的 `margins` 命令。使用稳健标准误以确保最逼近真实似然，并且使用选项`vce`以说明使用了两步 M 估计。两步 M 估计详见 Wooldridge (2010) 。

### 1.4 总结

给出了 Probit 与 Logit 的估计结果差异可以忽略不计的模拟运算证据，原因在于拟似然理论，特别是， Probit 与 Logit 模型的累积分布函数是相似的，尤其是在均值附近。
[相关code](https://www.stata.com/users/epinzon/blog/pvsl.do)

## 2. Probit/Logit 与线性概率模型对比

也有很多人直接使用 `regress` 估计二值选择模型，这其实是在估计 **线性概率模型**，其实就是一个简单的 OLS 回归。由于模型的被解释变量是概率值 (取值介于 0 和 1 之间)，而右侧的线性拟合值的取值范围未必在 0 和 1 之间，这个模型的局限也就显而易见了。

当真实模型是 Probit 或 Logit 时，运算结果显示，对于研究者所关注的边际效应，使用线性概率模型将生成不一致的估计。结果取决于真实模型究竟是 Probit 模型还是 Logit 模型。

### 2.1 估计结果

以下模拟使用 1000 个观察值，执行 5000 次重复（replications）。真实的数据生成过程 （DGPs） 是用一个离散的协变量和一个连续的协变量来构造的。

我研究了连续变量变化对条件概率 (AME) 的平均影响和离散协变量变化对条件概率 (ATE) 的平均影响。根据协变量的平均值计算，我还研究了连续变量的变化对条件概率的影响 (MEM) ，以及离散协变量的变化对条件概率的影响 (TEM) 。

**表 1** 给出了真实 DGP 满足 Logit 模型假设时的模拟结果。给出了 AME 和 ATE 估计的平均值，以及真实的零假设的 5% 的拒绝率，还提供了 AME 和 ATE 的近似真值。在系数的真实值处利用 2000 万个观测数据，通过计算 ATE 和 AME ，得到了近似的真实值。



> 
表1： AME和ATE ：真实 DGP Probit


N=10000, k=5000 次重复的拟合结果
|统计量|Approximate True Value|Logit|Regress (LPM)|
|----|----|----|----|
|x1的AME|-.084|-.084|-0.1537|
|5%拒绝概率||.050|.99|
|x2的ATE|.092|.091|.091|
|5%拒绝概率||.058|.058|

从表1中可以看出， Logit 模型估计接近真实值，真零假设的拒绝率接近 5% 。对于线性概率模型， AME 的拒绝率为 99% 。对于 ATE ，拒绝率和点估计接近使用 Logit 估计的值。对于 MEM 和 TEM ，有:



> 
表2： MEM 和 TEM ：真实 DGP Probit


N=10000, k=5000次重复的拟合结果
|统计量|Approximate True Value|Logit|Regress (LPM)|
|----|----|----|----|
|x1的MEM|-.099|-.099|-.094|
|5%拒绝概率||.054|.618|
|x2的TEM|.109|.109|.092|
|5%拒绝概率||.062|.073|

同样， Logit 估计与预期一致。对于线性概率模型，真实零假设的拒绝率为 62% 。对 TEM 的拒绝率为 7.3% ，估计效应小于真实效应。对于 AME 和 ATE ，当真实 DGP 为 Probit 模型时，结果如下：



> 
表3：平均边际和处理效应：真实 DGP Probit


N=10000, 5000次重复的拟合结果
|统计量|Approximate True Value|Probit|Regress (LPM)|
|----|----|----|----|
|x1的AME|-.094|-.094|-.121|
|5%拒绝概率||.047|1|
|x2的ATE|.111|.111|.111|
|5%拒绝概率||.065|.061|

Probit 模型估计接近真实值，真实原假设的拒绝率接近 5% 。对于线性概率模型， AME 的拒绝率为 100% 。对于 ATE ，拒绝率和点估计与 Probit 估计的值近似。对于 MEM 和 TEM，结果如下：



> 
表4： MEM 和 TEM ：真实 DGP Probit


N=10000, 5000次重复的拟合结果
|统计量|Approximate True Value|Probit|Regress (LPM)|
|----|----|----|----|
|x1的MEM|-.121|-.122|-.121|
|5%拒绝概率||.063|.054|
|x2的TEM|.150|.150|.110|
|5%拒绝概率||.059|.158|

对于 MEM ， Probit 模型和线性概率模型给出了可靠的推论。对于 TEM ， Probit 边际效应表现与预期一致，但线性概率模型的拒绝率为 16% ，且点估计与真实值不接近。

### 2.2 模特卡洛模拟分析过程解析
- 在第一部分，第 6 至 13 行，生成满足 Logit 模型和 Probit 模型假设的结果变量， y 和 yp 。
- 在第二部分，第 15 至 19 行，计算了 Logit 和 proit 模型的边际效应。对于离散协变量，边际效应是一种处理效应。
- 在第三部分，第 21 至 29 行，计算了均值处的边际效应。稍后将使用这些估计来计算真实效应的近似值。

```
program define mkdata
    syntax, [n(integer 1000)]
    clear
    quietly set obs `n'
    // 1. Generating data from Probit, Logit, and misspecified 
    generate x1    = rchi2(2)-2
    generate x2    = rbeta(4,2)>.2
    generate u     = runiform()
    generate e     = ln(u) -ln(1-u) 
    generate ep    = rnormal()
    generate xb    = .5*(1 - x1 + x2)
    generate y     =  xb + e > 0
    generate yp    = xb + ep > 0 
    // 2. Computing Probit & Logit marginal and treatment effects 
    generate m1   = exp(xb)*(-.5)/(1+exp(xb))^2
    generate m2   = exp(1 -.5*x1)/(1+ exp(1 -.5*x1 )) - ///
	              exp(.5 -.5*x1)/(1+ exp(.5 -.5*x1 ))
    generate m1p  = normalden(xb)*(-.5)
    generate m2p  = normal(1 -.5*x1 ) - normal(.5 -.5*x1)
    // 3. Computing marginal and treatment effects at means
    quietly mean x1 x2 
    matrix A        = r(table)
    scalar a        = .5 -.5*A[1,1] + .5*A[1,2]
    scalar b1       =  1 -.5*A[1,1]
    scalar b0       = .5 -.5*A[1,1]
    generate mean1  = exp(a)*(-.5)/(1+exp(a))^2
    generate mean2  = exp(b1)/(1+ exp(b1)) - exp(b0)/(1+ exp(b0))
    generate mean1p = normalden(a)*(-.5)
    generate mean2p = normal(b1) - normal(b0)
end
```

用 2000 万个观测数据来近似真实的边际效应。在这种情况下，这是一个合理的策略。例如，以连续协变量 xk 的平均边际效应为例，在 Probit 模型中：

$\frac{1} {N}\sum_{i=1}^N\phi(x_i\beta)\beta_k \qquad (2)$

上式是对 $E(\phi(x_i\beta)\beta_k)$ 的近似估计，为了获得这个期望值，需要对所有协变量的分布进行整合。这是不实际的，将限制协变量的选择。相反，我抽取了 2000 万个观测数据，计算 $\frac{1} {N}\sum_{i=1}^N\phi(x_i\beta)\beta_k$，将其作为真实值。其他边际效应的估计遵循同样的逻辑。下面是计算近似真实边际效应的代码。采用 2000 万个观测数据，计算了模拟中使用的平均值，并为每一个近似的真实值创建了暂元。

```
. mkdata, n(`L')
(2 missing values generated)

. local values "m1 m2 mean1 mean2 m1p m2p mean1p mean2p"

. local means  "mx1 mx2 meanx1 meanx2 mx1p mx2p meanx1p meanx2p"

. local n : word count `values'

. 
. forvalues i= 1/`n' {
  2.         local a: word `i' of `values'
  3.         local b: word `i' of `means'
  4.         sum `a', meanonly
  5.         local `b' = r(mean)
  6. }
```

现在可以进行以上部分用于生成结果的所有估计了。当真实 DGP 符合 Logit 模型时估计 TEM 和 MEM 的命令是：

```
. postfile lpm y1l y1l_r y1lp y1lp_r y2l y2l_r y2lp y2lp_r ///
>                 using simslpm, replace 

. forvalues i=1/`R' {
  2.         quietly {
  3.                 mkdata, n(`N')
  4.                 Logit  y x1 i.x2, vce(robust) 
  5.                 margins, dydx(*) atmeans post  vce(unconditional)
  6.                 local y1l = _b[x1]
  7.                 test _b[x1] = `meanx1'
  8.                 local y1l_r   = (r(p)<.05) 
  9.                 local y2l = _b[1.x2]
 10.                 test _b[1.x2] = `meanx2'
 11.                 local y2l_r   = (r(p)<.05) 
 12.                 regress  y x1 i.x2, vce(robust) 
 13.                 margins, dydx(*) atmeans post  vce(unconditional)
 14.                 local y1lp = _b[x1]
 15.                 test _b[x1] = `meanx1'
 16.                 local y1lp_r   = (r(p)<.05) 
 17.                 local y2lp = _b[1.x2]
 18.                 test _b[1.x2] = `meanx2'
 19.                 local y2lp_r   = (r(p)<.05) 
 20.                 post lpm (`y1l') (`y1l_r') (`y1lp') (`y1lp_r') ///
>                          (`y2l') (`y2l_r') (`y2lp') (`y2lp_r')
 21.         }
 22. }

. postclose lpm

. use simslpm, clear 

. sum 

    Variable |        Obs        Mean    Std. Dev.       Min        Max
-------------+---------------------------------------------------------
         y1l |      5,000   -.0985646      .00288  -.1083639  -.0889075
       y1l_r |      5,000       .0544     .226828          0          1
        y1lp |      5,000   -.0939211    .0020038  -.1008612  -.0868043
      y1lp_r |      5,000       .6182    .4858765          0          1
         y2l |      5,000    .1084959     .065586  -.1065291   .3743112
-------------+---------------------------------------------------------
       y2l_r |      5,000       .0618     .240816          0          1
        y2lp |      5,000    .0915894     .055462  -.0975456   .3184061
      y2lp_r |      5,000       .0732    .2604906          0          1
```

同样地，对于真实 DGP 符合 Logit 模型的 MEM 和 TEM ，使用包含`atmeans`选项的`margins`命令。似然模型逼近真实似然，因此使用稳健标准误，并且使用选项`vce`以说明使用了两步 M 估计。两步 M 估计详见 Wooldridge (2010)  。

### 2.3 总结

使用 Probit 或 Logit 模型可以产生等价的边际效应，但线性概率模型的边际效应与 Probit 及 Logit 模型不同。
[相关code](https://www.stata.com/users/epinzon/blog/lpm.do)

> 
White, H. 1996. Estimation, Inference, and Specification Analysis>. Cambridge: Cambridge University Press.


> 
Wooldridge, J. M. 2010. Econometric Analysis of Cross Section and Panel Data. 2nd ed. Cambridge, Massachusetts: MIT Press.


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

- Stata连享会 [精彩推文1](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文2](%5Bhttps://www.jianshu.com/p/de82fdc2c18a%5D(https://www.jianshu.com/p/de82fdc2c18a))
![](https://upload-images.jianshu.io/upload_images/7692714-8b1fb0b5068487af.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![欢迎加入Stata连享会(公众号: StataChina)](http://upload-images.jianshu.io/upload_images/7692714-f3cf42df597145a1.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)






