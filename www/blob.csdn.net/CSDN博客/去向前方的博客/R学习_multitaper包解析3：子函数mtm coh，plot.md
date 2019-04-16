# R学习_multitaper包解析3：子函数mtm.coh，plot - 去向前方的博客 - CSDN博客





2018年07月10日 10:26:16[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：153标签：[R																[MTM																[jackknife](https://so.csdn.net/so/search/s.do?q=jackknife&t=blog)




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

R学习_multitaper包解析2：子函数spec.mtm.dpss，dpssHelper[R 子函数2](https://blog.csdn.net/lvsehaiyang1993/article/details/80971365)
# 目录



- [前言](#前言)
- [目录](#目录)
- [mtm.coh子函数](#mtmcoh子函数)
- [plot子函数](#plot子函数)




# mtm.coh子函数

剪刀法相干性和辅助光滑函数以及画图函数程序

```bash
#########################################################################
##
## jackknife coherence and helper smoother and plotting functions
## 例子已经淘汰了，不建议尝试，手动删除
#########################################################################
#mtm.coh函数
输入:
mtm1 来自spec.mtm计算出的数据
fr 计算cohereence的频率点
tau 
mtm.coh <- function(mtm1, mtm2, fr=NULL, tau=0, phcorr = TRUE, 
                    plot=TRUE,...) {

    ## note Dave saves the cft
    ## in ./odinlibs-1.1/src/mw/mw2pakt as weighted
    ## 1000 blkcft(n,k,curblk,cur
    ##  cft(n*ndecfr,k)*sqrt(wt(n*ndecfr,k))

    ## we require auxiliary data
    if(is.null(mtm1$mtm$eigenCoefs) || is.null(mtm2$mtm$eigenCoefs)) {
        stop("Both mtm objects must have been computed with returnInternals=TRUE.")
    }

    if(mtm1$mtm$k != mtm1$mtm$k) {
        stop("Both mtm objects must have the same value for k.")
    }
    ##k <- mtm1$auxiliary$

    if(mtm1$mtm$nfreqs != mtm1$mtm$nfreqs) {
        stop("Both mtm objects must have the same value for nFFT.")
    }

    nord <- mtm1$mtm$k
    nfreqs <- mtm1$mtm$nfreqs
    cft1 <- mtm1$mtm$eigenCoefs
    cft2 <- mtm2$mtm$eigenCoefs

    fr <-  if(is.null(fr))  array(as.double(0), nfreqs) else fr

    blklof <-  if(nfreqs %%2 ==0) 1 else 0
    blkhif <- nfreqs -1 + blklof

    nordP2 <- nord +2
    out <- .Fortran("jkcoh1", cft1=as.complex(cft1),
                    cft2=as.complex(cft2), nord=as.integer(nord),
                    blklof=as.integer(blklof), blkhif=as.integer(blkhif),
                    fr=as.double(fr),  tau=as.double(tau),
                    phcorr=as.integer(phcorr),
                    NTmsc=double(nfreqs), NTvar=double(nfreqs),
                    msc=double(nfreqs), ph=double(nfreqs),
                    phvar=double(nfreqs),
                    s1=double(nordP2), s2=double(nordP2),
                    jkmsc=double(nordP2), TRmsc=double(nordP2),
                    bias=double(nfreqs),
                    cx=complex(nordP2),
                    PACKAGE="multitaper")

    auxiliary <- list(nfreqs=mtm1$mtm$nFreqs,
                      nFFT=mtm1$mtm$nFFT,
                      nw=mtm1$mtm$nw,
                      k=mtm1$mtm$k,
                      deltaT=mtm1$mtm$deltaT,
                      dtUnits=mtm1$mtm$dtUnits,
                      taper=mtm1$mtm$taper
                      )


    coh.out <- list(NTmsc=out$NTmsc, NTvar=out$NTvar,
                    msc=out$msc, nfreqs=mtm1$mtm$nfreqs,
                    freq=mtm1$freq, k=nord,
                    ph=out$ph, phvar=out$phvar, mtm=auxiliary)
    class(coh.out) <- "mtm.coh"


   if(plot) {
        plot.mtm.coh(coh.out, ...)
        return(invisible(coh.out))
    } else {
        return(coh.out)
    }
}
```

# plot子函数

```
##################################################################
##
##  plot.mtm
##
##  Takes a mtm object, and plots either the associated spectrum
##  (obj$spec) or the harmonic F-test statistic (obj$Ftest).
##
##################################################################
plot.mtm <- function(x, 
                     jackknife=FALSE, 
                     Ftest=FALSE, 
                     ftbase=1.01,
                     siglines=NULL, 
                     ...) {

    ## Set frequency axis and label
    dtUnits <- x$mtm$dtUnits
    deltaT <- x$mtm$deltaT

    ## if the user has not set 'xlab' ... set it for them:
    if(!hasArg("xlab")) {
        if(!(x$mtm$dtUnits == "default")) {
            xlab <- paste("Frequency in cycles/",dtUnits,sep="") }
        else {
            xlab <- paste("Frequency")
        }
    } 

    if(Ftest) {
        if(!hasArg("xlab")) {
        .plotFtest(x,xlab=xlab,siglines=siglines,ftbase=ftbase, ...)
    } else {
        .plotFtest(x, siglines=siglines, ftbase=ftbase, ...)
    }  
    } 
    else 
    { ## plot spectrum only
        ## modified to remove calls to plot.spec
        ## for R version 3.1.0
        ##
        class(x) <- "spec"
        if(x$mtm$taper=="sine") {
            if(!hasArg("xlab")) {
                plot( x, xlab=xlab, sub=" ", ...)
            } else {
                plot( x, sub=" ", ...) 
            }  
        }
        else { ## case of taper=="dpss"
            nw <- x$mtm$nw
            k <- x$mtm$k
            sub <- paste("(NW = ", nw, " K = ", k,")", sep="")
            log <- match.call(expand.dots = )$log
            if(jackknife) {
                dBPlot <- FALSE
                if(!is.null(log) && log== "dB" ) {
                    dBPlot <- TRUE }
                if(jackknife && !is.null(x$mtm$jk)) {
                    if(dBPlot) {
                        upperCI <- 10*log10(x$mtm$jk$upperCI)
                        lowerCI <- 10*log10(x$mtm$jk$lowerCI)
                        minVal <- 10*log10(x$mtm$jk$minVal)
                        maxVal <- 10*log10(x$mtm$jk$maxVal) 
                    } 
                    else {
                        upperCI <- x$mtm$jk$upperCI
                        lowerCI <- x$mtm$jk$lowerCI
                        minVal <- x$mtm$jk$minVal
                        maxVal <- x$mtm$jk$maxVal
                    }
                    yRange <- c(minVal, maxVal)
                    if(!hasArg("xlab")) {
                        .lplotSpec( x, xlab=xlab, sub=sub, ylim=yRange, ...)
                    } else {
                        .lplotSpec( x, sub=sub, ylim=yRange, ...)
                    }  
                    lines(x$freq, upperCI, lty=2, col=2)
                    lines(x$freq, lowerCI, lty=2, col=3)
                }
            }
            else {
                if(!hasArg("xlab")) {
                    .lplotSpec( x, xlab=xlab, sub=sub, ...) 
                } else {
                    .lplotSpec( x, sub=sub, ...)
                }
            } 
        } ## end of dpss case
    } ## spectrum plot end
} ## end of function

##################################################################
##
##  plot.mtm.coh
##
##  Takes a mtm.coh object, and plots the Magnitude-Squared 
##  Coherence, with multiple y-axes.
##
##################################################################
plot.mtm.coh <- function(x, 
                         percentGreater=NULL,
                         nehlim=10, 
                         nehc=4,
                         cdfQuantilesTicks=NULL,
                         drawPercentLines=TRUE,
                         percentG=c(.1,.2,.5,.8,.9), 
                         ...) {

    if(  is.null(x$NTmsc) || is.null(x$NTvar)  || is.null(x$msc)
       || is.null(x$freq) || is.null(x$nfreqs) || is.null(x$k)) {
        stop("Requires mtm.coh object. Run mtm.coh on two mtm objects with returnInternals=TRUE.")
    }

    TRmsc <- x$NTmsc
    NTvar <- x$NTvar
    freqs <- x$freq
    nfreqs <- x$nfreqs
    k <- x$k

    ##nehlim and nehc are for smoothing 
    ## currently we plot the smoothed transformed coherence
    ## and lower CI after smoothing the variance
    plotTRmsc <- .lftr3p(TRmsc, NTvar, nfreqs,
                       nehlim,nehc, "even", "ext")
    trnrm_ <- .trnrm(k)
    par(oma=c(2,4,0,2))
    plot.new()
    ## note the ... was mainly implemented for xaxs="i"
    ## Undefined behaviour with other options 
    plot.window(range(freqs), range(plotTRmsc[,2]), ...)
    xy <- xy.coords(freqs,plotTRmsc[,2])
    ## plot smoothed msc
    plot.xy(xy, type="l", lwd=1, ...)
    ## plot one sd dev lower jackknife variance
    lines(freqs, plotTRmsc[,1], lty=3, lwd=1)
    box()
    axis(1)

    ## allow for user-settable xlabel, or unit display
    if(!hasArg("xlab")) {
        if(!(x$mtm$dtUnits == "default")) {
            xlab <- paste("Frequency in cycles/",x$mtm$dtUnits,sep="") }
        else {
            xlab <- paste("Frequency")
        }
    }
    mtext(xlab, side=1, line=3)

    ## basic left axis
    axis(2)
    mtext("Arctanh Transform of MSC",
          side=2, line=2, cex=par()$cex)

    ##  outer MSC axis on the left
    msc <- .FtoMSC(plotTRmsc[,2], trnrm_)
    mscTicks <- pretty(msc)


    ## transform ticks for at
    ##C2toF is coherence to inverse transform
    TRmscTicks <- .C2toF(mscTicks, trnrm_)
    axis(2, at=TRmscTicks, labels=mscTicks, outer=TRUE)
    mtext("Magnitude Squared Coherence", side=2, line=6)

    ##mscToCDF values may have issues for highly coherent values
    ## values over .9 will cause issues
    if(is.null(cdfQuantilesTicks)) {
        cdfQuantiles <- .mscToCDFquantiles(msc, k)
        cdfQuantilesTicks <- pretty(cdfQuantiles)
    }

    ## put right axis
    Qlvl <- .cdfToMSqCoh(cdfQuantilesTicks, k)
    TRQlvl <- .C2toF(Qlvl, trnrm_)

    cumulativeDistVals <- .C2toF(msc, trnrm_)
    axis(4, at=TRQlvl, labels=cdfQuantilesTicks)

    mtext("CDF for Independent Data",
          side=4, line=2) 

    if(drawPercentLines == TRUE) {
        percentGprob <- percentG
        percentG <- .C2toF(.cdfToMSqCoh(percentG, k),  trnrm_)
        lenPercentG <- length(percentG)
        for(i in 1:lenPercentG) {
            lines(freqs, array(percentG[i], nfreqs), lty=2)
        }
    }

    if(!is.null(percentGreater)) {
        mtext(paste("CDF for C=   10.0% 20.0% 50.0% 80.0% 90.0%"),
              side=1, line=4, adj=-1, cex=.8)
        mtext(paste("% of data > Q     ",
                    100*round( percentGreater[1], digits=3),
                    "% ",
                    100*round( percentGreater[2], digits=3),
                    "% ",
                    100*round( percentGreater[3], digits=3),
                    "% ",
                    100*round( percentGreater[4], digits=3),
                    "% ",
                    100*round( percentGreater[5], digits=3),
                    "%", sep=""),
              side=1, line=5, adj=-1, cex=0.8)
    }
    return(list(sigProb = percentGprob, sigNT = percentG))
}
```](https://so.csdn.net/so/search/s.do?q=MTM&t=blog)](https://so.csdn.net/so/search/s.do?q=R&t=blog)




