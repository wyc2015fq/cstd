# R语言 牛顿-拉夫森迭代法求方程组 - littlely_ll的博客 - CSDN博客





2016年12月11日 23:00:30[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：5286








牛顿-拉夫森迭代法： 


$x_{k+1}=x_{k}-{[f'(x)]^{-1}}f(x)$

其中，$f'(x)$为Jacobi矩阵。 

例，设非线性方程组为： 
$x^2+y^2=1,$
$y^2=2x$

求方程组的解。 

Jocabi行列式： 
![雅克比式](https://img-blog.csdn.net/20161211224358858?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**R**代码如下：
```php
fun <- function(x){
  f <- c(x[1]^2+x[2]^2-1, x[2]^2-2*x[1])
  joc <- matrix(c(2*x[1],-2,2*x[2],2*x[2]),nr=2)
  list(f=f,jac=jac)
}

Newton <-function(fun, x,eps = 1e-5){
  k <- 0
  repeat{
  k <- k+1
    x1 <- x
    obj <- fun(x)
    x <- x - solve(obj$jac,obj$f)
    if((x-x1)%*%(x-x1)<eps){
      return(list(root=x,iter=k))
      break
    }
  }
}
```

最后设初始值为c(1,1.2). **注:** 选择初始值必须式Jacobi行列式不为零。
`Newton(fun,c(1,1.2)`
$root 

[1] 0.4142136 0.9101797

$iter 

[1] 4 

而利用rootSolve包解方程组`multiroot(model,c(1,1.2)`解出结果与上述结果一致，而迭代次数为5.




