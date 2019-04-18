# Bregman Divergences on Riemannian Manifold   黎曼流形上的Bregman散度 - curryche的博客 - CSDN博客





2018年06月14日 19:54:44[curryche](https://me.csdn.net/whwan11)阅读数：216








## SPD流形

SPD流形是由对称正定矩阵组成的流形，记为 


$Sym_{++}^{d}=\{X:X\in R^{d\times d},X=X^T,v^TXv>0,\ for\ any\ v\in R^{d}\}$

## Bregman散度

令$\varsigma\ :\ Symc_{++}^{d}\ \rightarrow \ R$为定义在SPD流形上的严格凸可微函数，Breman矩阵散度定义为： 


$d_{\varsigma}(X,Y)=\varsigma(X)-\varsigma(Y)-\langle\bigtriangledown_{\varsigma}(Y),X-Y\rangle$

其中$\langle X,Y\rangle=Tr(X^TY)$，$\bigtriangledown_{\varsigma}(Y)$表示函数$\varsigma$在$Y$上的梯度。

## J散度

令$\varsigma(X)=-log|X|$，得到Jeffery散度的表达式为： 


$\begin{align*}J(X,Y)&=\frac{1}{2}d_{\varsigma}(X,Y)+\frac{1}{2}d_{\varsigma}(Y,X)\\&=\frac{1}{2}Tr(X^{-1}Y)-\frac{1}{2}log|X^{-1}Y|+\frac{1}{2}Tr(Y^{-1}X)-\frac{1}{2}log|Y^{-1}X|-n\\&=c\frac{1}{2}Tr(X^{-1}Y)+\frac{1}{2}Tr(Y^{-1}X)-n\end{align*}$

## S散度

令$\varsigma(X)=-log|X|$，得到Stein散度的表达式为： 


$\begin{align*}S(X,Y)&=\frac{1}{2}d_{\varsigma}(X,\frac{X+Y}{2})+\frac{1}{2}d_{\varsigma}(Y,\frac{X+Y}{2})\\&=\frac{1}{2}log|\frac{X+Y}{2}|-\frac{1}{2}log|XY|\end{align*}$

## 性质

J散度和S散度都是仿射不变的。



