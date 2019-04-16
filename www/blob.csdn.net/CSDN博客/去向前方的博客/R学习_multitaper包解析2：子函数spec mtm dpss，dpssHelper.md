# R学习_multitaper包解析2：子函数spec.mtm.dpss，dpssHelper - 去向前方的博客 - CSDN博客





2018年07月09日 15:29:28[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：124








# 前言

之前讲了MTM（多锥形窗谱估计）的相关原理，现在来分析一下它的R语言的实现，这个实现是提出人的学生写的，和matlab的实现进行对照分析，加深理解，提高大家对这门技术的掌握程度，解析的顺序依旧是从下至上，先从简单的子程序，最后到复杂的主程序。

想要复习的可以参考一下之前的文件：

mtm原理：[现代谱估计：多窗口谱](https://blog.csdn.net/lvsehaiyang1993/article/details/80361172)

想要复习一下如何实现的可以参考： 

MTM：matlab实现1[MTM：matlab实现1](https://blog.csdn.net/lvsehaiyang1993/article/details/80928212)

MTM：matlab实现2参数解析[MTM参数解析](https://blog.csdn.net/lvsehaiyang1993/article/details/80928955)

MTM：matlab实现3谱功率计算[MTM谱功率计算](https://blog.csdn.net/lvsehaiyang1993/article/details/80928955)

MTM：matlab实现4主函数解析[MTM 主函数](https://blog.csdn.net/lvsehaiyang1993/article/details/80932452)

R学习_multitaper包解析1：子函数centre，dpss：[R 子函数](https://blog.csdn.net/lvsehaiyang1993/article/details/80962010)
# 目录



- [前言](#前言)
- [目录](#目录)
- [子函数：spec.mtm.dpss](#子函数specmtmdpss)
- [子函数：dpssHelper](#子函数dpsshelper)




# 子函数：spec.mtm.dpss

```
使用slepian窗序列计算多窗口谱估计
代码对应论文在下面
##############################################################
##
##  .spec.mtm.dpss
##
##  Computes multitaper spectrum using Slepian tapers
##  References: 
##    Percival and Walden "Spectral Analysis
##    for Physical Applications" 1993 and associated LISP code
##
##    Thomson, D.J. Spectrum Estimation and Harmonic Analysis,
##    Proceedings of the IEEE, 1982 and associated Fortran code
## 
##############################################################
输入

.spec.mtm.dpss <- function(timeSeries,时间序列
                     nw,窗口阶数
                     k,使用多少个窗口序列
                     nFFT,   使用多少点计算              
                     dpssIN,输入离散扁球序列
                     returnZeroFreq,零频率的幅值需不需要
                     Ftest,F检验
                     jackknife,是否用剪刀法计算数据
                     jkCIProb,t分布的概率值
                     adaptiveWeighting, 自适应的权重值
                     maxAdaptiveIterations,最大自适应的迭代数
                     returnInternals,是否返回间隔
                     n,时间序列长度
                     deltaT,等待间隔
                     sigma2,时间序列的方差
                     series,解析好的时间序列
                     dtUnits,时间间隔单位
                     ...) {

    # Complex check case
    复值检测
    if(is.complex(timeSeries)) {
    如果序列时复值且设定返回零值谱，则调整为返回点1处的谱。
    并且警告
      if(!returnZeroFreq) {
        returnZeroFreq <- 1 
        warning("Cannot set returnZeroFreq to 0 for complex time series.")
      } 
    }
   初始化dw，ev，receivedDW为真

    dw <- NULL
    ev <- NULL
    receivedDW <- TRUE
如果没有初始化dpss序列，则设置为假，使用dpss生成DPssin相关序列参数
    if(!.is.dpss(dpssIN)) {
      receivedDW <- FALSE
      dpssIN <- dpss(n, k, nw=nw, returnEigenvalues=TRUE)
       dw等于v值乘以v值*采样间隔的均方根 是一个特征值矩阵
       dw <- dpssIN$v*sqrt(deltaT)
       eigen是对应的k个特征向量中心能量
      ev <- dpssIN$eigen 
    }
    如果初始化了dpss，则将对应值域赋值给相应对象。
    else {
      dw <- .dpssV(dpssIN)
      ev <- .dpssEigen(dpssIN)
      如果ev是个空值，则按照公式生成对应的ev值
      if(all(is.null(ev))) {
        ev <- dpssToEigenvalues(dw, nw) }
        dw <- dw*sqrt(deltaT) 
    }
    频率点是nfft点数的一半，加上偏移的零值矩阵
    nFreqs <- nFFT %/% 2 + as.numeric(returnZeroFreq)
    偏移点值如果是1，则反馈0
    offSet <- if(returnZeroFreq) 0 else 1 
    注意频率轴是使用默认值设定的单位值。
    # Note that the frequency axis is set by default to unit-less
    默认是0.5 hz，其他情况则是按照既定参数设置的。
    # scaling as 0 through 0.5 cycles/period. The user parameter
    # dtUnits modifies this scaling in the plot.mtm function.
    尺度频率是1/采样持续时间
    scaleFreq <- 1 / as.double(nFFT * deltaT)
    初始化k个中心能量
    swz <- NULL ## Percival and Walden H0
    初始化特征向量能量
    ssqswz <- NULL

    swz <- apply(dw, 2, sum)
    if(k >= 2) {
      swz[seq(2,k,2)] <- 08041
    }
    ssqswz <- as.numeric(t(swz)%*%swz)
  加窗数据初始化
    taperedData <- dw * 1
    需要补领的点=nfft-n点
    nPadLen <- nFFT - n 
    如果时间序列非复
    if(!is.complex(timeSeries)) {
        补零加窗数据为一个nfft*k的矩阵
      paddedTaperedData <- rbind(taperedData, matrix(0, nPadLen, k))
    } else {
       或者复值矩阵补零
      paddedTaperedData <- rbind(taperedData, matrix(complex(0,0), nPadLen, k)) 
    }
    cft等于对加窗数据，使用快速傅里叶变换
    cft <- mvfft(paddedTaperedData)
    如果不是复值数据
    if(!is.complex(timeSeries)) {
    cft等于cft（1：nfreqs）的数据
      cft <- cft[(1+offSet):(nFreqs+offSet),]
    } else {
    复值数据双边展示
      cft <- rbind(cft[(nFreqs+offSet+1):nFFT,],cft[(1+offSet):(nFreqs+offSet),])
    }
    谱能量等于cft的平方
    sa <- abs(cft)^2
    如果时间序列不是复值数值
    if(!is.complex(timeSeries)) {
       结果频率点构造
      resultFreqs <- ((0+offSet):(nFreqs+offSet-1))*scaleFreq 
    } else {
         结果频率点构造
      resultFreqs <- (-(nFreqs-1):(nFreqs-2))*scaleFreq
    }
    初始化自适应参数
    adaptive <-  NULL
    初始化剪刀参数
    jk <- NULL
    初始化功率谱频率密度
    PWdofs <- NULL
    如果不使用剪刀法
    if(!jackknife) {
    如果 是实数序列
        if(!is.complex(timeSeries)) {
         使用mw2wta法计算自适应的结果
          adaptive <- .mw2wta(sa, nFreqs, k, sigma2, deltaT, ev)
        } else {
        如果是复数序列
          adaptive <- .mw2wta(sa, nFFT, k, sigma2, deltaT, ev) 
        }
        其他
    } else {
        如果概率密度不符合要求，则停止程序
        stopifnot(jkCIProb < 1, jkCIProb > .5)
        如果是实数序列，同时采用自适应方法
        if(!is.complex(timeSeries) & adaptiveWeighting) {
          自适应计算
          adaptive <- .mw2jkw(sa, nFreqs, k, sigma2, deltaT, ev)
        } else {
          adaptive <- .mw2jkw(sa, nFFT, k, sigma2, deltaT, ev)
        }
        计算对应的比例尺
        scl <- exp(qt(jkCIProb,adaptive$dofs)*
                   sqrt(adaptive$varjk))
          上届=自适应s*scl
        upperCI <- adaptive$s*scl
        lowerCI <- adaptive$s/scl
        下界，最小值最小的哪一个
        minVal = min(lowerCI)
        上界，最大值最大的哪一个
        maxVal = max(upperCI)
        jk剪刀值，因为我计算的时候没用，具体参数也不是很明白
        jk <- list(varjk=adaptive$varjk,
                   bcjk=adaptive$bcjk,
                   sjk=adaptive$sjk,
                   upperCI=upperCI,
                   lowerCI=lowerCI,
                   maxVal=maxVal,
                   minVal=minVal)
   } 
  自适应程序有可能没有很好的停止
   ## Short term solution to address bug noted by Lenin Castillo noting that adaptive weights are not properly turned off (Karim 2017).     
   初始化特征谱，自由度
   resSpec <- NULL
   dofVal <- NULL
    如果，不是自适应的方法
   if(!adaptiveWeighting) {
      获得总能量的平均值，自由度2k
    resSpec <- apply(sa, 1, mean)
    dofVal <- 2*k
   } else {
   谱来自于自适应
    resSpec <- adaptive$s
    自由度来自于具体参数
        dofVal <- adaptive$dofs
   }                    

  f检验，空值
   ftestRes <- NULL
如果要进行f检验
   if(Ftest) {
       如果特征频率为空
        if(is.null(swz)) {
        则swz可以应用
            swz <- apply(dw, 2, sum)
        }
        使用。hf4mpl计算f检验的结果
        ftestRes <- .HF4mp1(cft,
                            swz,
                            k,
                            ssqswz)
    }
  初始化特征权重，加权因子
    eigenCoef1 <- NULL
    wtCoef1 <- NULL
    如果返回频率间隔
    if(returnInternals) {
    特征因子是傅里叶变换
        eigenCoef1 <- cft
        如果使用自适应的方法
        if(adaptiveWeighting) {
        则加权因子是自适应因子的平方根
          wtCoef1 <- sqrt(adaptive$wt)
        } 
    }
    自适应参数列表
    auxiliary <- list(dpss=dpssIN,
                      eigenCoefs=eigenCoef1,
                      eigenCoefWt=wtCoef1,
                      nfreqs=nFreqs,
                      nFFT=nFFT,
                      jk=jk,
                      Ftest=ftestRes$Ftest,
                      cmv=ftestRes$cmv,
                      dofs=dofVal,
                      nw=nw,
                      k=k,
                      deltaT=deltaT,
                      dtUnits=dtUnits,
                      taper="dpss")

    ##   Thomson, D.J. Spectrum Estimation and Harmonic Analysis,
    ##   Proceedings of the IEEE, 1982.

    ## note that the weights are squared, they are |d_k(f)^2 from equation
    ## (5.4)
    ## These weights correspond to Thomoson's 1982 Fortran code.
    ## dof fix for one taper, only value.
    如果k=1自由度为2
    if(k==1) {
        auxiliary$dofs <- 2
    }
    spec谱的结果
    spec.out <- list(origin.n=n,
                     method="Multitaper Spectral Estimate",
                     pad= nFFT - n,
                     spec=resSpec,
                     freq=resultFreqs,
                     series=series,
                     adaptive=adaptiveWeighting, 
                     mtm=auxiliary)
       
    class(spec.out) <- c("mtm", "spec")

    if(Ftest) {
        class(spec.out) <- c("mtm", "Ftest", "spec")
    }
    返回谱计算的结果
    return(spec.out)
}
```

# 子函数：dpssHelper

获得对应的参数

```
.dpssV <- function(obj) obj$v

.dpssEigen <- function(obj) obj$eigen
使用相应的计算手段。
.is.dpss <- function(obj) {
    return( sum ( "dpss"==class(obj) ) >= 1 )
}
```




