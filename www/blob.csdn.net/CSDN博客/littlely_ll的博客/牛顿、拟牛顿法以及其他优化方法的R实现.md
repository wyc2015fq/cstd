# 牛顿、拟牛顿法以及其他优化方法的R实现 - littlely_ll的博客 - CSDN博客





2017年02月04日 14:04:52[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：3086








$\quad$牛顿法（Newton method）和拟牛顿法（quasi Newton method）是求解无约束最优化问题的常用方法，有收敛速度快的优点。

## 1. 牛顿法

考虑无约束最优化问题 


$min_{x\in R^n} f(x)$

其中$x^*$为目标函数极小点。 

假设$f(x)$有二阶连续偏导数，若第k次迭代值为$x^{(k)}$,则可将$f(x)$在$x^{(k)}$附近进行二阶泰勒展开： 


$f(x)=f(x^{(k)})+g_k^T(x-x^{(k)})+\frac{1}{2}(x-x^{(k)})^TH(x^{(k)})(x-x^{(k)})$

这里，$g_k=g(x^{(k)})=\bigtriangledown f(x^{(k)})$是$f(x)$的梯度向量在点$x^{(k)}$的值，$H(x^{(k)})$是$f(x)$的海塞矩阵 


$H(x)=[\frac{\partial^2f}{\partial x_i\partial x_j}]_{n\times n}$

在点$x^{(k)}的值。$当$H(x^{(k)})$是正定矩阵是，函数$f(x)$的极值为极小值。 

假设$x^{(k)}$满足： 


$\bigtriangledown f(x^{(k+1)})=0$

对$f(x)$求导，得 


$\bigtriangledown f(x) = g_k+H_k(x-x^{(k)})$

其中$H_k=H(x^{(k)})$,则 


$g_k+H_k(x^{(k+1)}-x^{(k)})=0$

因此，$x^{(k+1)}=x^{(k)}-H_k^{-1}g_k$ (**), 

或者$x^{(k+1)}=x^{(k)}+p_k$,其中，$H_kp_k=-g_k$

式（**）作为迭代公式的算法就是牛顿法。 
**牛顿法：**

输入：目标函数$f(x),梯度g(x)=\bigtriangledown f(x), 海塞矩阵H(x),精读要求\epsilon;$

输出：$f(x)的极小点x^*.$

（1）取初始点$x^{(0)}$,置$k=0，$

（2）计算$g_k=g(x^{(k)})$, 

（3）若$||g_k||\lt \epsilon$,则停止计算，得近似解$x^*=x^{(k)}$, 

（4）计算$H_k=H(x^{(k)}),并求p_k$


$H_kp_k=-g_k$

（5）置$x^(k+1)=x^{(k)}+p_k$， 

（6）置$k=k+1$,转至（2）。 

步骤（4）要求$H_k^{-1}$，计算复杂，所以有其他改进的方法，比如拟牛顿法等。
## 牛顿-拉夫森方法

牛顿-拉夫森方法是一种确定性的优化方法，它的公式为： 


$x_{i+1} = x_i-[\frac{\delta^2f}{\delta x\delta x^T}(x_i)]^{-1}\frac{\delta f}{\delta x}(x_i)$

R语言中`nlm`函数是求解非线性函数最优化的。
```
nlm(f, p, ..., hessian = FALSE, typsize = rep(1, length(p)),
    fscale = 1, print.level = 0, ndigit = 12, gradtol = 1e-6,
    stepmax = max(1000 * sqrt(sum((p/typsize)^2)), 1000),
    steptol = 1e-6, iterlim = 100, check.analyticals = TRUE)

f:要优化的函数
p:初始值
hessian:如果TRUE,返回f函数最小值的hessian矩阵
。。。
```

假如要优化一个函数： 


$(1-x)^2+100(y-x^2)^2+0.3(0.2-2y)^2+100(x-y^2)^2-0.5(x^2+5y^2)$

```
n <- 300
## to define a grid
x <- seq(-1, 2, length.out = n)
y <- seq(-1, 2, length.out = n)
## evaluate on each grid point
z <- mountains(expand.grid(x, y))
## contour plot
par(mar = c(4,4,0.5,0.5))
contour(x, y, matrix(log10(z), length(x)),
xlab = "x", ylab = "y", nlevels = 20)
## Warning in matrix(log10(z), length(x)): NaNs produced
## starting value
sta <- c(0.5,-1)
points(sta[1], sta[2], cex = 2, pch = 20)
## solutions for each of 20 steps
sol <- matrix(, ncol=2, nrow = 21)
sol[1, ] <- sta
for(i in 2:20){
sol[i, ] <- nlm(mountains, sta, iterlim = i)$est
}
## optimal solution
sol[21, ] <- nlm(mountains, sta)$est
points(sol[21, 1], sol[21, 2], cex = 3, col = "red", pch = 20)
## path visually
lines(sol, pch=3, type="o")
## now let's start better (dashed line)
sta <- c(0,-1)
for(i in 2:20){
    sol[i, ] <- nlm(mountains, sta, iterlim = i)$est
}
sol[1, ] <- sta
sol[21, ] <- nlm(mountains, sta)$est
points(sta[1], sta[2], cex = 2, pch = 20)
points(sol[21, 1], sol[21, 2], cex = 3, col = "red", pch = 20)
lines(sol, pch=3, type="o")
```

![这里写图片描述](https://img-blog.csdn.net/20170531103831173?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

当函数是凸的或凹的，能达到全局最优。反之，可能达到局部最优。

## 其他优化方法

解决一般问题的优化问题的函数是`optim`，可实现的方法为：

> - Nelder-Mead方法：默认的，速度慢
- BFGS方法：拟牛顿法，hessian矩阵不可求解时很有用
- CG方法：比BFGS方法稳定性差，但是计算效率高
- L-BFGS-B方法：允许有约束条件
- SANN方法：模拟退火方法，相对较慢


我们仍然优化上一个函数`mountains`

```
optims <- function(x,meth = 'Nelder-Mead', start=c(0.5,-1)){
       sol <- matrix(,nc=2,nr=21)
       for(i in 2:20){
             sol[i,] <- optim(start,mountains,method=meth, control = list(maxit=i))$par
         }
       sol[21,] <- optim(start, mountains)$par
       points(start[1],start[2],pch=20, cex=2)
       points(sol[21,],sol[21,],pch=20,col='red',cex=3)
       lines(sol[,1],sol[,2],type='o',pch=3,col='blue')
}
```

```
par(mar=c(4,4,2,.5))
contour(x,y,matrix(log10(z),length(x)),xlab='x',ylab='y',nlevels=20,main='Method:Nelder-Mead')
optims()
```

![这里写图片描述](https://img-blog.csdn.net/20170531111757518?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```
contour(x,y,matrix(log10(z),length(x)),xlab='x',ylab='y',nlevels=20,main='Method:BFGS')
optims(meth='BFGS')
```

![这里写图片描述](https://img-blog.csdn.net/20170531111836879?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```
contour(x,y,matrix(log10(z),length(x)),xlab='x',ylab='y',nlevels=20,main='Method:CG')
optims(meth='CG')
```

![这里写图片描述](https://img-blog.csdn.net/20170531111902567?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```
contour(x,y,matrix(log10(z),length(x)),xlab='x',ylab='y',nlevels=20,main='Method:L-BFGS-B')
optims(meth='L-BFGS-B')
```

![这里写图片描述](https://img-blog.csdn.net/20170531111932755?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```
contour(x,y,matrix(log10(z),length(x)),xlab='x',ylab='y',nlevels=20,main='Method:SANN')
optims(meth='SANN')
```

![这里写图片描述](https://img-blog.csdn.net/20170531111955959?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

修改初始值可以达到全局最优。





