# R学习_multitaper包解析1：子函数centre，dpss - 去向前方的博客 - CSDN博客





2018年07月08日 19:58:46[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：237








# 前言

之前讲了MTM（多锥形窗谱估计）的相关原理，现在来分析一下它的R语言的实现，这个实现是提出人的学生写的，和matlab的实现进行对照分析，加深理解，提高大家对这门技术的掌握程度。

想要复习的可以参考一下之前的文件：

mtm原理：[现代谱估计：多窗口谱](https://blog.csdn.net/lvsehaiyang1993/article/details/80361172)

想要复习一下如何实现的可以参考： 

MTM：matlab实现1[MTM：matlab实现1](https://blog.csdn.net/lvsehaiyang1993/article/details/80928212)

MTM：matlab实现2参数解析[MTM参数解析](https://blog.csdn.net/lvsehaiyang1993/article/details/80928955)

MTM：matlab实现3谱功率计算[MTM谱功率计算](https://blog.csdn.net/lvsehaiyang1993/article/details/80928955)

MTM：matlab实现4主函数解析[MTM 主函数](https://blog.csdn.net/lvsehaiyang1993/article/details/80932452)
# 目录



- [前言](#前言)
- [目录](#目录)
- [centre函数](#centre函数)
- [DPSS函数](#dpss函数)




# centre函数

该函数负责对输入的时间序列进行处理，使其满足零均值条件

```
#########################################################################
##
## centre
##
## Takes a time series and converts to zero-mean using one of three 
## methods: Slepian projection, arithmetic mean, or trimmed mean.
## 
#########################################################################
r里面的赋值采用<-格式
输入参数
时间序列x
nw生成几阶dpss序列
k使用的窗口数
deltat 采样间隔
trim截尾平均时丢掉的点数
centre <- function(x, nw=NULL, k=NULL, deltaT=NULL, trim=0) {
判断x是否有非数值
    na.fail(x)
    初始化结果
    res <- NULL
    如果没有输入dpss参数的话，则默认为截尾平均
    if(is.null(nw) && is.null(k) ) {
        res <- x - mean(x, trim=trim)
        如果有dpss参数，则使用slepian投影，截尾数舍弃
    } else {
        if(trim != 0) {
            warning(paste("Ignoring trim =", trim))
        }
        如果采用dpss方法，且相关参数不符合条件，则停止函数
        stopifnot(nw >= 0.5, k >= 1, nw <= 500, k <= 1.5+2*nw)
        如果dpss阶数和长度之比不合适，停止运算
        if (nw/length(x) > 0.5) { 

            stop("half-bandwidth parameter (w) is greater than 1/2")
        }
        如果，deltat没有值且x是时间序列，则获得它的采样间隔，否则，默认为1
        if(is.null(deltaT)) {
            if(is.ts(x)) {
                deltaT <- deltat(ts)
            } else {
                warning("deltaT not specified; using deltaT=1.")
                deltaT <- 1.0
            }
        }
        获得时间序列x的长度n
        n <- length(x)
        获得生成的DPSS序列结果，输入参数为n，k，nw，返回特征值
        dpssRes <- dpss(n, k=k, nw=nw,
                        returnEigenvalues=TRUE)
         dw值等于返回结果中的特征值*采样间隔的单位根
        dw <- dpssRes$v*sqrt(deltaT)
        ev值等于返回结果中的特征加窗矩阵，每一列对应第k个。
        ev <- dpssRes$eigen
        swz是特征向量的列值和
        swz <- apply(dw, 2, sum)
        ## zero swz where theoretically zero; odd tapers
        如果k》2则swz偶数位的值是0
        if(k >=2) {
          swz[seq(2,k,2)] <- 0.0
        }
        ssqswz是幅频率功率之和。
        ssqswz <- sum(swz^2)
        如果不是复值x，对x采取meave函数进行处理
        if(!is.complex(x)) {
          res <- .mweave(x, dw, swz,
                         n, k, ssqswz, deltaT)
          res <- x - res$cntr
        } else {
          res.r <- .mweave(Re(x), dw, swz,
                           n, k, ssqswz, deltaT)
          res.i <- .mweave(Im(x), dw, swz,
                           n, k, ssqswz, deltaT)
          res <- x - complex(real=res.r$cntr, imaginary=res.i$cntr)
        }
    }
    return(res)
}
```

# DPSS函数

负责生成给定的k个正交的离散扁球序列，即用来加权的窗 

使用对角法生成。 

参照论文和谱分析教材那本书，会讲如何得出给定的值
```
##########################################################
##
##  dpss
## 
##  Generates k orthogonal discrete prolate spheroidal
##  sequences (dpss) using the tridiagonal method. See
##  Slepian (1978) page 1379 and Percival and Walden
##  chapter 8.4
##
##########################################################
函数dpss
输入 
n值序列个数
k值加窗个数
nw值窗口给定值
ReturnEigenvalues=true真
返回特征值

dpss <- function(n, k, nw, returnEigenvalues=TRUE) {
如果不满足条件停止计算
    stopifnot(n >= 1, nw/n >0, nw/n < 0.5, k >= 1)
    将k变成一个整数。
    ## if k is passed in as floating point, the cast to 
    ## as.integer() in the Fortran call does not quite work properly
    if(!is.integer(k)) {
      k<-as.integer(floor(k));
    } 
    使用专门的包计算特征值
    ##eigen is of length for use by lapack functoins.
    ## this will use lapack functions in place of the
    ## eispack functions referenced in Percival and Waldern
    输入相应参数，得到相应的结果
    out <- .Fortran("fdpss", as.integer(n), as.integer(k),
              as.double(nw), 
              v=double(n*k), eigen=double(k),
              PACKAGE='multitaper')
        输出v值矩阵      
    out$v <- matrix(data=out$v, nrow=n, ncol=k, byrow=FALSE)
    如果返回特征值为真
    if(returnEigenvalues) {
    输出的特征值利用特定子函数生成
        out$eigen <- dpssToEigenvalues(out$v, nw)
    } else {
    特征值由对称公式返回
        ## eigen values returned from the tridiagonal formulation
        ## Slepian eqn #13 (1978)
        out$eigen <- out$eigen
    }
    结果是输出的v和特征值
    res <- list(v=out$v,
                eigen=out$eigen)
    结果类型 dpss            
    class(res) <- "dpss"
    return(res)
}

##########################################################
##
##  dpssToEigenvalues
## 给定一个离散扁球窗，找到对应生成dpss序列的特征值
##  Given a set of dpss tapers, find the eigenvalues corresponding
## to the generated dpss's
##
##  See Percival and Walden (1993) exercise 8.4, and
## associated LISP code.
##
##########################################################

dpss序列到特征值矩阵
输入
v
nw

dpssToEigenvalues <- function(v, nw) {
将v矩阵化
    v <- as.matrix(v)
    获得v的第一维序列长度
    n <- length(v[,1])
    获得v的第二维，k值
    k <- length(v[1,])
    获得窗口百分比
    w <- nw/n
    找到序列长度最接近的2的次方的npot数量
    npot <- 2**(ceiling(log2(2*n)))

    ## pad
    生成v值矩阵
    scratch <- rbind(v, matrix(data=0, nrow=npot-n, ncol=k))
    ## n * acvs
    矩阵的值是对应特侦序列的功率。
    scratch <- Re(mvfft(abs(mvfft(scratch))**2))/npot

    j <- 1:(n-1)
    生成比率向量
    vectorOfRatios <- c(2*w, sin(2*pi*w*j)/(pi*j))
    这两个矩阵都随着序号的上升，幅值下降
    ## Note: both vector of ratios and scratch
    ##  roughy decrease in amplitude with increasing index,
    ##  so sum things up in reverse order
    特征值为空
    eigenvalues <- NULL
    如果k大于1
    if(k>1) {
    特征值按列相乘假和
        eigenvalues <- apply(scratch[n:2,] * vectorOfRatios[n:2], 2, sum)
    } else {
    其他情况下不用加
        eigenvalues <- sum(scratch[n:2,] * vectorOfRatios[n:2])
    }
    返回特征向量加上第一列的和。
    return(2*eigenvalues +
           vectorOfRatios[1]*scratch[1,1:k])
}
```





