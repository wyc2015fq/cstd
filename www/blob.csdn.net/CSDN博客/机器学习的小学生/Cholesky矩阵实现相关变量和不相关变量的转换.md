
# Cholesky矩阵实现相关变量和不相关变量的转换 - 机器学习的小学生 - CSDN博客


2017年06月06日 15:56:09[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：875


给定一个协方差矩阵$\Sigma$, 其可以唯一的分解为点乘形式:

$$
\Sigma = U^T U
$$
其中$U$是上三角矩阵。
几何上讲，Cholesky矩阵将无相关的变量转换为由$\Sigma$给定的方差和协方差的相关变量。特别的，如果产生了$p$个标准正态分布的变量，Cholesky转换将这些变量映射为多元正态分布（其协方差矩阵为$\Sigma$，中心在原点（表示为$\text{MVN}(\mathbf 0,\Sigma)$））
在高斯过程的例子中，我们通过下面的形式形成高斯分布数据点：
```python
n1 =
```
```python
80
```
```python
;
n2 =
```
```python
40
```
```python
;
S1 =
```
```python
eye
```
```python
(
```
```python
2
```
```python
);
S2 =
```
```python
[
```
```python
1
```
```python
0.95
```
```python
;
```
```python
0.95
```
```python
1
```
```python
]
```
```python
;
```
```python
% 两个协方差矩阵
```
```python
m1 =
```
```python
[
```
```python
0.75
```
```python
;
```
```python
0
```
```python
]
```
```python
;
m2 =
```
```python
[-
```
```python
0.75
```
```python
;
```
```python
0
```
```python
]
```
```python
;
```
```python
%两个均值
```
```python
x1 =
```
```python
bsxfun
```
```python
(@plus,chol(S1)
```
```python
'*gpml_randn(0.2,2,n1),m1);
x2 = bsxfun(@plus,chol(S2)'
```
```python
*gpml_randn(
```
```python
0.3
```
```python
,
```
```python
2
```
```python
,n2),m2);
x =
```
```python
[x1 x2]'
```
```python
;
y =
```
```python
[-ones(
```
```python
1
```
```python
,n1) ones(
```
```python
1
```
```python
,n2)]'
```
```python
;
plot(x1(
```
```python
1
```
```python
,:),x1(
```
```python
2
```
```python
,:),
```
```python
'b+'
```
```python
);
hold on;
plot(x2(
```
```python
1
```
```python
,:),x2(
```
```python
2
```
```python
,:),
```
```python
'r+'
```
```python
);
```
```python
% check
```
```python
cov1 = cov(
```
```python
x1'
```
```python
);
cov2 = cov(
```
```python
x2'
```
```python
);
```
参考文献:
1.[http://blogs.sas.com/content/iml/2012/02/08/use-the-cholesky-transformation-to-correlate-and-uncorrelate-variables.html](http://blogs.sas.com/content/iml/2012/02/08/use-the-cholesky-transformation-to-correlate-and-uncorrelate-variables.html)
2.[http://www.gaussianprocess.org/gpml/code/matlab/doc/](http://www.gaussianprocess.org/gpml/code/matlab/doc/)

