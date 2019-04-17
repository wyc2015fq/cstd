# matlab 函数及小技巧 （转载 持续更新） - 西西弗的石头 - CSDN博客





2011年07月21日 20:55:26[hzhaly](https://me.csdn.net/hzhaly)阅读数：1536标签：[matlab](https://so.csdn.net/so/search/s.do?q=matlab&t=blog)








## 1   matlab中sin(90)为什么不等于1

运行sin(90)，结果等于0.8940，为什么不等于1？

sin(x)里的x不是指角度吗？



x是弧度，不是角度，楼主看看sin(pi/2)可以看到



如果想用角度值，则用sind函数。sind(90)=1;

另外还有cosd，tand，atand，asind，acosd。



## 2  Matlab取整函数小结

取整函数小结，容易混淆，整理一下，大家多补充啊！


    以a = [-1.9, -0.2, 3.4, 5.6, 7.0, 2.4+3.6i]为例

（1）floor函数：取小于等于a中元素的最近的整数

floor(a)

ans =

  -2.0000            -1.0000             3.0000             5.0000             7.0000             2.0000 + 3.0000i

（2）round函数：取离a中元素最近的整数（注意与floor函数的区别）

round(a)

ans =

  -2.0000                  0             3.0000             6.0000             7.0000             2.0000 + 4.0000i

（3）ceil函数：取大于等于a中元素的最近的整数
ceil(a)

ans =

  -1.0000                  0             4.0000             6.0000             7.0000             3.0000 + 4.0000i

（4）fix函数：取离0最近的整数（对于正数而言，和floor函数等同；对于负数而言，和ceil函数等同）
fix(a)

ans =

  -1.0000                  0             3.0000             5.0000             7.0000             2.0000 + 3.0000i




这让我想到了求余的两个函数

M = mod(X,Y) 等同于M = X - floor(X./Y).*Y

R = rem(X,Y)  等同于R = X -  fix(X./Y).*Y


有整数就有余数，哈哈，没有偏太多哦～

当对字符串求整时，返回的是ASCII码吧



## 3  检查矩阵元素是不是全0

两种方法：

|1.all(all(a==0))2.sum(sum(a.*a))## 4  泰勒展开 （符号变量的替代）clearclcsyms x      %设定符号变量y=sin(x);r=taylor(y,7,0);   %泰勒展开。参数一：展开对象；参数二：展开阶数；参数三：展开点a=linspace(-5,5,1000);b=subs(r,x,a);   %substitution 替代等式中的符号变量。 参数一：等式；参数二：等式中要被替代的变量；参数三：新变量hold on plot(a,sin(a),'.')plot(a,b,'r.')hold off|
|----|

## 5 归一化问题



[点击打开链接](http://www.ilovematlab.cn/forum.php?mod=viewthread&tid=74021)






