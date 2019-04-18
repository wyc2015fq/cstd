# matlab数值积分中函数积分的4种方法 - 心纯净，行致远 - CSDN博客





2017年10月30日 10:00:33[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：32188标签：[数值积分																[matlab](https://so.csdn.net/so/search/s.do?q=matlab&t=blog)](https://so.csdn.net/so/search/s.do?q=数值积分&t=blog)
个人分类：[【MatLab/Simulink】](https://blog.csdn.net/zhanshen112/article/category/7184373)










**1.       采用inline内联函数**

Matlab中可以有采用几种不同的方式来指定被积函数。对于简单的、长度不超过一行的公式采用inline命令比较方便。例如，



可用下面的语句进行计算

>> f=inline('1/sqrt(1+x^4)')    %采用inline内联函数



f =



     内联函数:

     f(x) = 1/sqrt(1+x^4)



>> q=quadtx(f,0,1) 



q =



0.9270

从matlab第七版开始，内联（inline）对象被一种功能更强大的结构匿名函数（anonymousfunction）所替代。在matlab第七版内联对象还允许被使用，但推荐用匿名函数，因为后者可以生成更高效率的程序代码。采用匿名函数，上面的例子变为

>> f=@(x) 1/sqrt(1+x^4)   %采用匿名函数@（x）



f = 



    @(x)1/sqrt(1+x^4)



>> q=quadtx(f,0,1) 



q =



0.9270



**2.      特殊点不可积函数，采用realmin**

如果我们想要计算



可能使用下面的语句

>> f=inline('sin(x)/x')



f =



     内联函数:

     f(x) = sin(x)/x



>> q=quadtx(f,0,pi) 

已达到最大递归限制 500。使用set(0,'RecursionLimit',N) 可更改此限制。请注意，超出可用堆栈空间可能会使 MATLAB 和/或计算机崩溃。



出错 inlineeval



此时inline函数积分时好像出现了问题，因为在计算f(0)时，出现了除以0的情况，并且最终产生递归限制错误。一种补救的方法是，将积分的下限由0变为最小的正浮点数，realmin。

>> q=quadtx(f,realmin,pi) 



q =



1.8519

这样就可以避免递归错误情况的发生。



**3.      使用m文件写函数**

针对上面出现的问题，另一种方法是使用M文件，而不是内联函数。创建包含下面程序的文件sinc.m



function f=sinc(x)

%函数sin(x)/x

if x==0

    f=1;

else

    f=sin(x)/x;

end



然后使用函数句柄计算积分

>> Q=quadtx(@sinc,0,pi)



Q =



    1.8519



**4.      依赖于参数的积分**

一个典型的例子是β函数，它定义为



matlab中已经实现了一个现成的β函数，但我们可以以它为例，说明如何处理积分中的参数。创建一个带三个参数的内联函数

>> F=inline('t^(z-1)*(1-t)^(w-1)','t','z','w')

或者创建一个M文件：

function f=betaf(t,z,w)

f= t^(z-1)*(1-t)^(w-1)

并将其命名为betaf.m。

就像任何函数一样，参数的顺序是很重要的。定义被积函数时，必须让积分变量为其第一个参数。然后给出其他参数的值，作为传递给quadtx的附加参数。要计算β（8/3，10/3）

应该先设

>> z=8/3;

>> w=10/3;

>> tol=1e-6;

然后运行命令

>> Q=quadtx(F,0,1,tol,z,w)



Q =



0.0348

或Q=quadtx(@betaf,0,1,tol,z,w)




