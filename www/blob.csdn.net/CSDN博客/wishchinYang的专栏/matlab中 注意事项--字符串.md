# matlab中 注意事项--字符串 - wishchinYang的专栏 - CSDN博客
2013年07月17日 14:26:16[wishchin](https://me.csdn.net/wishchin)阅读数：680
**Matlab中的字符串操作**
**原文链接：[http://hi.baidu.com/dreamflyman/item/bd6d8224430003c9a5275a9f](http://hi.baidu.com/dreamflyman/item/bd6d8224430003c9a5275a9f)**
**（1）.字符串是以ASCII码形式存储的。abs和double函数都可以用来获取字符串矩阵所对应的ASCII码数值矩阵。相反，char函数可以把ASCII码矩阵转换为字符串矩阵。**
**例2-12  用求特征值的方法解方程。3x5-7x4+5x2+2x-18=0**
```python
p=[3,-7,0,5,2,-18];
A=compan(p);           %A的伴随矩阵
x1=eig(A)              %求A的特征值
x2=roots(p)            %直接求多项式p的零点
```
**与字符串有关的另一个重要函数是eval，其调用格式为：    eval(t)其中t为字符串。它的作用是把字符串的内容作为对应的MATLAB语句来执行。**
```python
sum(A,dim)：当dim为1时，该函数等同于sum(A)；当dim为2时，返回一个列向量，其第i个元素是A的第i行的各元素之和。
prod(A,dim)：当dim为1时，该函数等同于prod(A)；当dim为2时，返回一个列向量，其第i个元素是A的第i行的各元素乘积。
mean(A,dim)：当dim为1时，该函数等同于mean(A)；当dim为2时，返回一个列向量，其第i个元素是A的第i行的算术平均值。
median(A,dim)：当dim为1时，该函数等同于median(A)；当dim为2时，返回一个列向量，其第i个元素是A的第i行的中值。
cumsum(A,dim)：当dim为1时，该函数等同于cumsum(A)；当dim为2时，返回一个矩阵，其第i行是A的第i行的累加和向量。
cumprod(A,dim)：当dim为1时，该函数等同于cumprod(A)；当dim为2时，返回一个向量，其第i行是A的第i行的累乘积向量。
cumsum(X)：返回向量X累加和向量。
cumprod(X)：返回向量X累乘积向量
```
**std函数的一般调用格式为：**
```python
Y=std(A,flag,dim)
```
**其中dim取1或2。当dim=1时，求各列元素的标准方差；当dim=2时，则求各行元素的标准方差。flag取0或1，当flag=0时，按σ1所列公式计算标准方差，当flag=1时，按σ2所列公式计算标准方差。缺省flag=0，dim=1。**
**sort函数也可以对矩阵A的各列或各行重新排序，其调用格式为：**
```python
[Y,I]=sort(A,dim)
```
**其中dim指明对A的列还是行进行排序。若dim=1，则按列排；若dim=2，则按行排。Y是排序后的矩阵，而I记录Y中的元素在A中位置。**
**polyfit函数的调用格式为：**
```python
[P,S]=polyfit(X,Y,m)
```
**函数根据采样点X和采样点函数值Y，产生一个m次多项式P及其在采样点的误差向量S。其中X,Y是两个等长的向量，P是一个长度为m+1的向量，P的元素为多项式系数。**
**polyval函数的功能是按多项式的系数计算x点多项式的值，**
用于数值积分和微分的有quad,quad8,dblquad，diff等
用于符号运算的命令有symadd,symsub,symmul，symdiv，numden，factor，expand，collect，transpose，determ，limit，diff，int，fourier，ifourier，laplace，ilaplace，ztrans，iztrans，taylor，solve，dsolve等。
