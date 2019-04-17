# 科学网—关于Newton Raphson算法的一个注 - 蒋迅的博文




# 关于Newton Raphson算法的一个注                           

已有 11635 次阅读2013-1-29 09:46|个人分类:[科技](http://blog.sciencenet.cn/home.php?mod=space&uid=420554&do=blog&classid=136794&view=me)|系统分类:[科研笔记](http://blog.sciencenet.cn/home.php?mod=space&do=blog&view=all&uid=420554&catid=1)|关键词:数学,非线性,Newton,Raphson算法|[数学](misc.php?mod=tag&id=286), [非线性](misc.php?mod=tag&id=7932), [Newton](misc.php?mod=tag&id=47331), [Raphson算法](misc.php?mod=tag&id=176720)



作者：蒋迅

在科学网上看到有人[谈Newton Raphson算法](http://blog.sciencenet.cn/blog-253428-646510.html)。想借此机会也来说一句。

[牛顿-拉弗森方法](http://zh.wikipedia.org/wiki/%E7%89%9B%E9%A1%BF%E6%B3%95) ([Newton's method](http://en.wikipedia.org/wiki/Newton-Raphson)) 是一种在实数域和复数域上近似求解方程的方法。假定函数*f*(*x*)可导。我们要求它的零点。选择一个接近函数*f*(*x*)零点的*x*0，利用下式进行迭代。

![](http://image.sciencenet.cn/album/201301/08/204328ulhcmgfzggclquxy.png)

在实际应用中，我们最可能面临的是，我们无法得到函数*f*(*x*) 的导数 *f* '(*x*)，从而使得这个方法失效。笔者在计算中遇到的就是要用软件来得到*f*(*x*) 的值，而软件本身并不计算导数。这时候最常见的办法就是用差分代替导数：

![](http://image.sciencenet.cn/album/201301/10/1738234hq225pg4gqrzr3z.png)

下面用一段FORTRAN程序说明在实际应有中如何使用Newton Raphson算法。假定有一个计算软件能近似计算所需*y*值。这个软件的输出文件是“output.txt”。先写一个shell script，叫“myscript.sh”。然后在FORTRAN里做一个system call去调动“myscript.sh”。程序如下：


System('bash myscript.sh')

open(5,file='output.txt')

read(5,'(1x,f16.9)') y


error_old = error

error = y - y_target


if (error > 0.01) then

     slope = (error - error_old)/(x - x_old)

     x_old = x

     x = x - relax*error/slope

endif

上述程序中有些简化，比如没有检查分母为零的情况。还有就是Newton-Raphson算法是在第二步以后才有效。第一步的初始猜测值和第二步的算法都很重要。通常第二步可以取 *x* = 0.95**x*_old (或 *x* = 1.05**x*_old)，
或者更精确的工程数学公式。很多经验丰富的工作者都有这些公式，是我们应该注意的财富。上面还有一个新的参数“relax”(0 < relax ≤
 1)。一般有 relax = 1.0。但我们调用的软件不一定很理想，所以可能不能过于依赖。这个时候就可以取一个稍小一点的“relax”值。

有时函数变化过于迅速，可以先用速度比较慢的二分法迭代数次，再换成Newton-
Raphson法。下面是我遇到的一个问题。要找的零点在1490 - 1500 之间。我从 1425 
开始计算。如果直接用Newton-Raphson算法的话就会将迭代的点落在 1500 之外，而从物理上我知道必须在 1500 
之内。所以我决定先用二分法，到一定的精度后才换成Newton-Raphson算法。其它的注意事项不一一叙述。

![](http://image.sciencenet.cn/album/201301/22/174031ge7cam7gxgsme4ac.gif)

非线性系统常常用迭代方法来解决，而Newton Raphson算法往往简单有效。笔者屡试不爽。


转载本文请联系原作者获取授权，同时请注明本文来自蒋迅科学网博客。
链接地址：[http://blog.sciencenet.cn/blog-420554-657505.html](http://blog.sciencenet.cn/blog-420554-657505.html)

上一篇：[认真比较读研与坐牢](blog-420554-656862.html)
下一篇：[这美甲够数学的](blog-420554-658463.html)



