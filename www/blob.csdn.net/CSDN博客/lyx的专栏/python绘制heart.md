# python绘制heart - lyx的专栏 - CSDN博客





2016年11月23日 19:32:30[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：1967







python有一个强大的模块，叫做matplotlib，但是，和matlab比，似乎还是弱了些，因为目测无法绘制隐函数！而matlab有ezplot。所以，如何用python去绘制一个心的形状呢？我们先来看一下heart的解析方程。


(x²+y²-1)³-x²2y³=0

看起来很简单的样子，变成python的表达式就是：



```python
(x**2+y**2-1)**3-x**2*2*y**3
```
显然这是一个隐函数，我们只能用sympy这个模块来绘制heart了，不得不鄙视一下下matplotlib。




```python
from sympy.parsing.sympy_parser import parse_expr
from sympy import plot_implicit
ezplot = lambda exper: plot_implicit(parse_expr(exper))
ezplot('(x**2+y**2-1)**3-x**2*2*y**3')
```

![](https://img-blog.csdn.net/20161123192923903?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

如果我们修改一下参数，
```python
(x**2+y**2-9)**3-x**2*9*y**3
```

就是这样的效果

![](https://img-blog.csdn.net/20161123192940544?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


然后有是比较瘦小的心



```python
((2*x)**2+y**2-9)**3-(2*x)**2*10*y**3
```
![](https://img-blog.csdn.net/20161123193021813?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

还有瘦到不能看的心：







```python
((7*x)**2+y**2-9)**3-(7*x)**2*10*y**3
```
![](https://img-blog.csdn.net/20161123193033248?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


也有肥到夹门的心：

```python
((0.7*x)**2+y**2-9)**3-(0.7*x)**2*10*y**3
```
![](https://img-blog.csdn.net/20161123192906219?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

各种各样的心，改一下参数就能获得，但是真正的心，再复杂的系统都难以刻画，又遑论修改参数。


