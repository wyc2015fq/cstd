# matlab中的randn randi rand randsrc的用法以及区别 - zqjackking的博客 - CSDN博客





2017年03月25日 19:50:16[zqjackking](https://me.csdn.net/zqjackking)阅读数：2783标签：[matlab](https://so.csdn.net/so/search/s.do?q=matlab&t=blog)
个人分类：[matlab数据处理](https://blog.csdn.net/zqjackking/article/category/6778267)









1，rand 生成均匀分布的伪随机数。分布在（0~1）之间 

   主要语法：rand(m,n)生成m行n列的均匀分布的伪随机数 

                      rand(m,n,’double’)生成指定精度的均匀分布的伪随机数，参数还可以 

                     是’single’ 

                     rand(RandStream,m,n)利用指定的RandStream(我理解为随机种子)生成伪 

                     随机数
2，randn 生成标准正态分布的伪随机数（均值为0，方差为1） 

     主要语法：和上面一样

3, randi 生成均匀分布的伪随机整数 

     主要语法：randi（iMax）在开区间（0，iMax）生成均匀分布的伪随机整数 

                       randi（iMax，m，n）在开区间（0，iMax）生成mXn型随机矩阵 

                       r = randi([iMin,iMax],m,n)在开区间（iMin，iMax）生成mXn型随机矩阵 

4， randsrc
无参数形式，随机输出-1或1；

randsrc(m,n);

randsrc(m);

输出m*n阶或m*m阶矩阵，元素为随机出现的-1或1，概率为1/2；

而rand是随机产生0——1中某一数呢， 

例如

> 
> 
rand(2,3)



ans =

```
0.9501    0.6068    0.8913
0.2311    0.4860    0.7621
```

> 
> 
rand(3)



ans =

```
0.4565    0.4447    0.9218
0.0185    0.6154    0.7382
0.8214    0.7919    0.1763
```





