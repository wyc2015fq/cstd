# dx,dy是什么？ - 马同学 - CSDN博客
2018年10月23日 17:39:03[马同学高等数学](https://me.csdn.net/ccnt_2012)阅读数：849
这个问题让我们从曲线的微分开始说起。
**1 曲线的微分**
比如，有曲线![f(x)](https://www.zhihu.com/equation?tex=f(x)) :
![](https://img-blog.csdn.net/20181023173437479?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
给出![x\in U(x_0,\Delta x)](https://www.zhihu.com/equation?tex=x%5Cin%20U(x_0%2C%5CDelta%20x)) 的曲线段:
![](https://img-blog.csdn.net/20181023173437648?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
要找到一个直线段来近似这个曲线段，也就是找到这个曲线段的微分：
![](https://img-blog.csdn.net/20181023173437685?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
此微分的特点是，当![\Delta x\to 0](https://www.zhihu.com/equation?tex=%5CDelta%20x%5Cto%200) 时，越来越逼近曲线段：
![](https://img-blog.csdn.net/20181023173437834?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**2 切线**
这个微分其实就是切线。
**2.1 最初印象**
初学几何的时候，切线是这么定义的：
![与圆、椭圆交于一点的直线，称为切线](https://www.zhihu.com/equation?tex=%E4%B8%8E%E5%9C%86%E3%80%81%E6%A4%AD%E5%9C%86%E4%BA%A4%E4%BA%8E%E4%B8%80%E7%82%B9%E7%9A%84%E7%9B%B4%E7%BA%BF%EF%BC%8C%E7%A7%B0%E4%B8%BA%E5%88%87%E7%BA%BF)
比如这就是圆、椭圆的切线：
![](https://img-blog.csdn.net/20181023173437683?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
但是这个定义推广到所有曲线上是不成立的：
![](https://img-blog.csdn.net/20181023173437747?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**2.2 割线的极限**
我们需要用极限来定义切线。比如说，要求曲线![f(x)](https://www.zhihu.com/equation?tex=f(x)) 在![A](https://www.zhihu.com/equation?tex=A) 点的切线：
![](https://img-blog.csdn.net/20181023173437775?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在![A](https://www.zhihu.com/equation?tex=A) 附近找一点![B_1](https://www.zhihu.com/equation?tex=B_1) ，过两点作直线![B_1A](https://www.zhihu.com/equation?tex=B_1A) ，这根直线也称为割线：
![](https://img-blog.csdn.net/2018102317343845?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
然后寻找![A](https://www.zhihu.com/equation?tex=A) 与![B_1](https://www.zhihu.com/equation?tex=B_1) 之间的点![B_2](https://www.zhihu.com/equation?tex=B_2) ，作出割线![B_2A](https://www.zhihu.com/equation?tex=B_2A) ：
![](https://img-blog.csdn.net/20181023173438230?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
以此类推，找到点![B_3,B_4,\cdots,B_n](https://www.zhihu.com/equation?tex=B_3%2CB_4%2C%5Ccdots%2CB_n) ，作出割线：
![](https://img-blog.csdn.net/20181023173438232?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
把这些割线组成数列：
![\{a_n\}=\{B_1A,B_2A,B_3A,\cdots,B_nA\}](https://www.zhihu.com/equation?tex=%5C%7Ba_n%5C%7D%3D%5C%7BB_1A%2CB_2A%2CB_3A%2C%5Ccdots%2CB_nA%5C%7D)
它的极限![\lim_{n\to\infty}a_n](https://www.zhihu.com/equation?tex=%5Clim_%7Bn%5Cto%5Cinfty%7Da_n) 就是切线：
![](https://img-blog.csdn.net/20181023173438330?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**3 导数**
刚才只是给出了切线的定义，但是还是不能把切线求出来。下面来看看怎么求。
**3.1 斜率**
要求![A](https://www.zhihu.com/equation?tex=A) 点的切线，知道了![A](https://www.zhihu.com/equation?tex=A) 点坐标为![A=(x_0, f(x_0))](https://www.zhihu.com/equation?tex=A%3D(x_0%2C%20f(x_0))) ，以及切线的斜率：
![](https://img-blog.csdn.net/20181023173438217?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
其中![斜率=\tan\alpha](https://www.zhihu.com/equation?tex=%E6%96%9C%E7%8E%87%3D%5Ctan%5Calpha) ，根据直线的点斜式，可求得切线函数![g(x)](https://www.zhihu.com/equation?tex=g(x)) ：
![\frac{g(x)-f(x_0)}{x-x_0}=\tan\alpha\implies g(x)=\tan\alpha(x-x_0)+f(x_0)](https://www.zhihu.com/equation?tex=%0A%5Cfrac%7Bg(x)-f(x_0)%7D%7Bx-x_0%7D%3D%5Ctan%5Calpha%5Cimplies%20g(x)%3D%5Ctan%5Calpha(x-x_0)%2Bf(x_0)%0A)
就可以得到切线的函数。
**3.2 导数**
容易有以下推论：
![切线=割线的极限\implies 切线的\color{Magenta}{斜率}=割线的\color{Magenta}{斜率}的极限](https://www.zhihu.com/equation?tex=%E5%88%87%E7%BA%BF%3D%E5%89%B2%E7%BA%BF%E7%9A%84%E6%9E%81%E9%99%90%5Cimplies%20%E5%88%87%E7%BA%BF%E7%9A%84%5Ccolor%7BMagenta%7D%7B%E6%96%9C%E7%8E%87%7D%3D%E5%89%B2%E7%BA%BF%E7%9A%84%5Ccolor%7BMagenta%7D%7B%E6%96%9C%E7%8E%87%7D%E7%9A%84%E6%9E%81%E9%99%90)
所以来看看割线的斜率怎么求吧。假设要求![A](https://www.zhihu.com/equation?tex=A) 点的切线的斜率，随便在附近找一点![B](https://www.zhihu.com/equation?tex=B) 作割线：
![](https://img-blog.csdn.net/20181023173438238?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
可以看到当![B\to A](https://www.zhihu.com/equation?tex=B%5Cto%20A) 的时候（这也表明了切线是割线的极限），两者斜率不断逼近：
![](https://img-blog.csdn.net/20181023173438725?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
先把割线的斜率![\tan\beta](https://www.zhihu.com/equation?tex=%5Ctan%5Cbeta) 算出来，假设![A=(x_0,f(x_0)),B=(x,f(x))](https://www.zhihu.com/equation?tex=A%3D(x_0%2Cf(x_0))%2CB%3D(x%2Cf(x))) ：
![](https://img-blog.csdn.net/20181023173438702?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
因此：
![\tan\beta=\frac{\Delta y}{\Delta x}=\frac{f(x)-f(x_0)}{x-x_0}](https://www.zhihu.com/equation?tex=%5Ctan%5Cbeta%3D%5Cfrac%7B%5CDelta%20y%7D%7B%5CDelta%20x%7D%3D%5Cfrac%7Bf(x)-f(x_0)%7D%7Bx-x_0%7D)
根据刚才的分析可知：
![切线的斜率=\tan\alpha=\lim_{B\to A}\tan\beta=\lim_{x\to x_0}\frac{f(x)-f(x_0)}{x-x_0}](https://www.zhihu.com/equation?tex=%E5%88%87%E7%BA%BF%E7%9A%84%E6%96%9C%E7%8E%87%3D%5Ctan%5Calpha%3D%5Clim_%7BB%5Cto%20A%7D%5Ctan%5Cbeta%3D%5Clim_%7Bx%5Cto%20x_0%7D%5Cfrac%7Bf(x)-f(x_0)%7D%7Bx-x_0%7D)
这个极限就被称为![\color{Salmon}{导数}](https://www.zhihu.com/equation?tex=%5Ccolor%7BSalmon%7D%7B%E5%AF%BC%E6%95%B0%7D) 。
如果，不光在![A](https://www.zhihu.com/equation?tex=A) 点可以作出切线，也就是不光在![A](https://www.zhihu.com/equation?tex=A) 点可导，而是在某个开区间![x\in I](https://www.zhihu.com/equation?tex=x%5Cin%20I) 内都可导，这就是![\color{Salmon}{导函数}](https://www.zhihu.com/equation?tex=%5Ccolor%7BSalmon%7D%7B%E5%AF%BC%E5%87%BD%E6%95%B0%7D) ：
![](https://img-blog.csdn.net/20181023173438941?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
不少教科书、文档会出现如下的符号，这里也一并引入：
> 
**定义![D=\frac{d}{dx}](https://www.zhihu.com/equation?tex=D%3D%5Cfrac%7Bd%7D%7Bdx%7D) ，称之为![\color{Salmon}{D算子}](https://www.zhihu.com/equation?tex=%5Ccolor%7BSalmon%7D%7BD%E7%AE%97%E5%AD%90%7D) ，导函数可以用之表示为：**
**![Df(x)=\frac{d}{dx}f(x)=f'(x)](https://www.zhihu.com/equation?tex=Df(x)%3D%5Cfrac%7Bd%7D%7Bdx%7Df(x)%3Df%27(x))**
**有时候写作![D_x](https://www.zhihu.com/equation?tex=D_x) ，表明对自变量![x](https://www.zhihu.com/equation?tex=x) 求导。**
算子，英文为“operator”，操作的意思。
算子和函数还是很接近的，只是有以下区别：
![\begin{array}{c|c}    \hline\\    \quad 函数 \quad&\quad 数到数的映射 \quad\\     \quad 算子 \quad&\quad 函数到函数的映射 \quad\\    \\\hline\end{array}](https://www.zhihu.com/equation?tex=%0A%5Cbegin%7Barray%7D%7Bc%7Cc%7D%0A%20%20%20%20%5Chline%5C%5C%0A%20%20%20%20%5Cquad%20%E5%87%BD%E6%95%B0%20%5Cquad%26%5Cquad%20%E6%95%B0%E5%88%B0%E6%95%B0%E7%9A%84%E6%98%A0%E5%B0%84%20%5Cquad%5C%5C%20%0A%20%20%20%20%5Cquad%20%E7%AE%97%E5%AD%90%20%5Cquad%26%5Cquad%20%E5%87%BD%E6%95%B0%E5%88%B0%E5%87%BD%E6%95%B0%E7%9A%84%E6%98%A0%E5%B0%84%20%5Cquad%5C%5C%0A%20%20%20%20%5C%5C%5Chline%0A%5Cend%7Barray%7D%0A)
在这里，![D](https://www.zhihu.com/equation?tex=D) 算子完成了如下函数之间的映射：
![f(x)\xrightarrow{\quad \color{red}{D}\quad}f'(x)](https://www.zhihu.com/equation?tex=f(x)%5Cxrightarrow%7B%5Cquad%20%5Ccolor%7Bred%7D%7BD%7D%5Cquad%7Df%27(x))
**4 切线函数与微分函数**
好了，咱们有了导数，可以来求切线函数以及微分函数了。
**4.1 切线函数**
就切线而言，知道要经过![A=(x_0,f(x_0))](https://www.zhihu.com/equation?tex=A%3D(x_0%2Cf(x_0))) ，也知道斜率是导数![f'(x_0)](https://www.zhihu.com/equation?tex=f%27(x_0)) ，可以用直线的点斜式得到切线函数：
![g(x)=f'(x_0)(x-x_0)+f(x_0)](https://www.zhihu.com/equation?tex=g(x)%3Df%27(x_0)(x-x_0)%2Bf(x_0))
**4.2 微分函数**
虽然之前一直说切线就是微分，但是微分函数和切线函数有所不同，因为它们在不同的坐标系。让我们一步步来，把这个关键点说清楚。
首先令![\Delta x=x-x_0](https://www.zhihu.com/equation?tex=%5CDelta%20x%3Dx-x_0) ，切线函数就变为了：
![g(x)=f'(x_0)(x-x_0)+f(x_0)\implies g(x)=f'(x_0)\Delta x+f(x_0)](https://www.zhihu.com/equation?tex=%0Ag(x)%3Df%27(x_0)(x-x_0)%2Bf(x_0)%5Cimplies%20g(x)%3Df%27(x_0)%5CDelta%20x%2Bf(x_0)%0A)
然后在以![A](https://www.zhihu.com/equation?tex=A) 点为原点建立直角坐标系（姑且称为微分坐标系吧）：
![](https://img-blog.csdn.net/20181023173438732?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
以![A](https://www.zhihu.com/equation?tex=A) 点为原点建立的微分坐标系中有，![f(x_0)=0](https://www.zhihu.com/equation?tex=f(x_0)%3D0) 。这样在微分坐标系中切线方程就很简单了：
![g(x)=f'(x_0)\Delta x+f(x_0)\implies h(x)=f'(x_0)\Delta x](https://www.zhihu.com/equation?tex=%0Ag(x)%3Df%27(x_0)%5CDelta%20x%2Bf(x_0)%5Cimplies%20h(x)%3Df%27(x_0)%5CDelta%20x%0A)
经过一系列操作终于得到了微分函数：
![f(x)\xrightarrow{\ D\ }f'(x)\xrightarrow{\ 点斜式\ }切线函数g(x)\xrightarrow{\ 更换坐标系\ }微分函数h(x)](https://www.zhihu.com/equation?tex=f(x)%5Cxrightarrow%7B%5C%20D%5C%20%7Df%27(x)%5Cxrightarrow%7B%5C%20%E7%82%B9%E6%96%9C%E5%BC%8F%5C%20%7D%E5%88%87%E7%BA%BF%E5%87%BD%E6%95%B0g(x)%5Cxrightarrow%7B%5C%20%E6%9B%B4%E6%8D%A2%E5%9D%90%E6%A0%87%E7%B3%BB%5C%20%7D%E5%BE%AE%E5%88%86%E5%87%BD%E6%95%B0h(x))
数学上把一系列操作用一个符号![\textrm{d}](https://www.zhihu.com/equation?tex=%5Ctextrm%7Bd%7D) 来表示，也可称为![\color{Salmon}{\textrm{d}算子}](https://www.zhihu.com/equation?tex=%5Ccolor%7BSalmon%7D%7B%5Ctextrm%7Bd%7D%E7%AE%97%E5%AD%90%7D) ：
![f(x)\underbrace{\xrightarrow{\ D\ }f'(x)\xrightarrow{\ 点斜式\ }切线函数g(x)\xrightarrow{\ 更换坐标系\ }微分函数h(x)}_{\color{red}{\textrm{d}}}](https://www.zhihu.com/equation?tex=f(x)%5Cunderbrace%7B%5Cxrightarrow%7B%5C%20D%5C%20%7Df%27(x)%5Cxrightarrow%7B%5C%20%E7%82%B9%E6%96%9C%E5%BC%8F%5C%20%7D%E5%88%87%E7%BA%BF%E5%87%BD%E6%95%B0g(x)%5Cxrightarrow%7B%5C%20%E6%9B%B4%E6%8D%A2%E5%9D%90%E6%A0%87%E7%B3%BB%5C%20%7D%E5%BE%AE%E5%88%86%E5%87%BD%E6%95%B0h(x)%7D_%7B%5Ccolor%7Bred%7D%7B%5Ctextrm%7Bd%7D%7D%7D)
微分![\textrm{d}](https://www.zhihu.com/equation?tex=%5Ctextrm%7Bd%7D) 算子完成了下列的函数映射：
![f(x)\xrightarrow{\quad \color{red}{\textrm{d}}\quad }微分函数h(x)](https://www.zhihu.com/equation?tex=f(x)%5Cxrightarrow%7B%5Cquad%20%5Ccolor%7Bred%7D%7B%5Ctextrm%7Bd%7D%7D%5Cquad%20%7D%E5%BE%AE%E5%88%86%E5%87%BD%E6%95%B0h(x))
所以微分函数也写作：
![\textrm{d}y=\textrm{d}f(x)=f'(x_0)\Delta x](https://www.zhihu.com/equation?tex=%5Ctextrm%7Bd%7Dy%3D%5Ctextrm%7Bd%7Df(x)%3Df%27(x_0)%5CDelta%20x)
表示把原函数![y=f(x)](https://www.zhihu.com/equation?tex=y%3Df(x)) 通过![\textrm{d}](https://www.zhihu.com/equation?tex=%5Ctextrm%7Bd%7D) 操作变为了微分函数![\textrm{d}y](https://www.zhihu.com/equation?tex=%5Ctextrm%7Bd%7Dy) ，这样也区别了微分函数和![f(x),g(x)](https://www.zhihu.com/equation?tex=f(x)%2Cg(x)) 坐标系不同。
![\Delta x=x-x_0](https://www.zhihu.com/equation?tex=%5CDelta%20x%3Dx-x_0) ，因为![x](https://www.zhihu.com/equation?tex=x) 是变量，所以![\Delta x](https://www.zhihu.com/equation?tex=%5CDelta%20x) 实际上表示的是整个![x](https://www.zhihu.com/equation?tex=x) 轴：
![](https://img-blog.csdn.net/20181023173438930?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
因为![\Delta x](https://www.zhihu.com/equation?tex=%5CDelta%20x) 代表![x](https://www.zhihu.com/equation?tex=x) 轴这根直线，而直线的微分，根据以直代曲的思想，其实就是自己，所以：
![\textrm{d}x=\Delta x](https://www.zhihu.com/equation?tex=%5Ctextrm%7Bd%7Dx%3D%5CDelta%20x)
因此，这就是微分的代数形式：
![\textrm{d}y=f'(x_0)\Delta x\implies\textrm{d}y=f'(x_0)\textrm{d}x](https://www.zhihu.com/equation?tex=%5Ctextrm%7Bd%7Dy%3Df%27(x_0)%5CDelta%20x%5Cimplies%5Ctextrm%7Bd%7Dy%3Df%27(x_0)%5Ctextrm%7Bd%7Dx)
切线函数和微分函数的区别在于，前者在![xy](https://www.zhihu.com/equation?tex=xy) 坐标系下，后者在![\textrm{d}y\textrm{d}x](https://www.zhihu.com/equation?tex=%5Ctextrm%7Bd%7Dy%5Ctextrm%7Bd%7Dx) 坐标系下：
![](https://img-blog.csdn.net/20181023173438937?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
因为微分的代数形式如上，所以导数也可以记作：
![\textrm{d}y=f'(x_0)\textrm{d}x\implies f'(x_0)=\frac{\textrm{d}y}{\textrm{d}x}](https://www.zhihu.com/equation?tex=%5Ctextrm%7Bd%7Dy%3Df%27(x_0)%5Ctextrm%7Bd%7Dx%5Cimplies%20f%27(x_0)%3D%5Cfrac%7B%5Ctextrm%7Bd%7Dy%7D%7B%5Ctextrm%7Bd%7Dx%7D)
所以导数也称为“微商”，即微分与微分的商。
**4.3 微分的自变量、因变量**
本节一直都在说，微分是函数：
![\textrm{d}y=f'(x_0)\textrm{d}x](https://www.zhihu.com/equation?tex=%5Ctextrm%7Bd%7Dy%3Df%27(x_0)%5Ctextrm%7Bd%7Dx)
那么它的自变量是什么，因变量是什么？
微分函数在![\textrm{d}y\textrm{d}x](https://www.zhihu.com/equation?tex=%5Ctextrm%7Bd%7Dy%5Ctextrm%7Bd%7Dx) 坐标系下，令![\textrm{d}y=h(x),\textrm{d}x=x](https://www.zhihu.com/equation?tex=%5Ctextrm%7Bd%7Dy%3Dh(x)%2C%5Ctextrm%7Bd%7Dx%3Dx) ，换元之后就回到了![xy](https://www.zhihu.com/equation?tex=xy) 坐标系：
![\textrm{d}y=f'(x_0)\textrm{d}x\implies h(x)=f'(x_0)x](https://www.zhihu.com/equation?tex=%5Ctextrm%7Bd%7Dy%3Df%27(x_0)%5Ctextrm%7Bd%7Dx%5Cimplies%20h(x)%3Df%27(x_0)x)
可见，自变量是![\textrm{d}x=x](https://www.zhihu.com/equation?tex=%5Ctextrm%7Bd%7Dx%3Dx) ，因变量是![\textrm{d}y=h(x)](https://www.zhihu.com/equation?tex=%5Ctextrm%7Bd%7Dy%3Dh(x)) 。
如果不光是求![x_0](https://www.zhihu.com/equation?tex=x_0) 点的微分，就像导函数一样，求某个开区间的微分，那么微分函数是二元函数：
![\textrm{d}y=f'(x)\textrm{d}x\implies h(w,x)=f'(w)x](https://www.zhihu.com/equation?tex=%5Ctextrm%7Bd%7Dy%3Df%27(x)%5Ctextrm%7Bd%7Dx%5Cimplies%20h(w%2Cx)%3Df%27(w)x)
**4.4 微分是线性函数**
虽然两者都是直线，但因为所在坐标系不同，所以切线函数和微分函数有一个重大的区别：
![](https://img-blog.csdn.net/20181023173439178?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这个区别说明：
![\begin{array}{c|c}    \hline    \quad\quad&\quad线性函数\quad\\    \hline \\    \quad \color{blue}{切线函数} \quad&\quad ☓\quad\\     \quad \color{orange}{微分函数} \quad&\quad \checkmark\quad\\    \\\hline\end{array}](https://www.zhihu.com/equation?tex=%0A%5Cbegin%7Barray%7D%7Bc%7Cc%7D%0A%20%20%20%20%5Chline%0A%20%20%20%20%5Cquad%5Cquad%26%5Cquad%E7%BA%BF%E6%80%A7%E5%87%BD%E6%95%B0%5Cquad%5C%5C%0A%20%20%20%20%5Chline%20%5C%5C%0A%20%20%20%20%5Cquad%20%5Ccolor%7Bblue%7D%7B%E5%88%87%E7%BA%BF%E5%87%BD%E6%95%B0%7D%20%5Cquad%26%5Cquad%20%E2%98%93%5Cquad%5C%5C%20%0A%20%20%20%20%5Cquad%20%5Ccolor%7Borange%7D%7B%E5%BE%AE%E5%88%86%E5%87%BD%E6%95%B0%7D%20%5Cquad%26%5Cquad%20%5Ccheckmark%5Cquad%5C%5C%0A%20%20%20%20%5C%5C%5Chline%0A%5Cend%7Barray%7D%0A)
根据微分是线性函数这点，我们可以很方便地运用线性代数的知识来求解法线函数。
**4.5 法线函数**
在切点与切线垂直的直线就是法线：
![](https://img-blog.csdn.net/20181023173439148?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
放在![\textrm{d}y\textrm{d}x](https://www.zhihu.com/equation?tex=%5Ctextrm%7Bd%7Dy%5Ctextrm%7Bd%7Dx) 坐标系中，随便找到切线方向、法线方向两个向量：
![](https://img-blog.csdn.net/20181023173439344?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
即（t代表tangent，n代表normal，分别是英文的切线和法线）：
![\boldsymbol{t}=\begin{pmatrix}1\\f'(x_0)\end{pmatrix}\quad\boldsymbol{n}=\begin{pmatrix}-1\\N\end{pmatrix}](https://www.zhihu.com/equation?tex=%5Cboldsymbol%7Bt%7D%3D%5Cbegin%7Bpmatrix%7D1%5C%5Cf%27(x_0)%5Cend%7Bpmatrix%7D%5Cquad%5Cboldsymbol%7Bn%7D%3D%5Cbegin%7Bpmatrix%7D-1%5C%5CN%5Cend%7Bpmatrix%7D)
根据线性代数的知识，知道两个正交向量点积为0，因此：
![\boldsymbol{t}\cdot\boldsymbol{n}=\begin{pmatrix}1\\f'(x_0)\end{pmatrix}\cdot\begin{pmatrix}-1\\N\end{pmatrix}=0\implies -1+f'(x_0)N=0\implies N=\frac{1}{f'(x_0)}](https://www.zhihu.com/equation?tex=%5Cboldsymbol%7Bt%7D%5Ccdot%5Cboldsymbol%7Bn%7D%3D%5Cbegin%7Bpmatrix%7D1%5C%5Cf%27(x_0)%5Cend%7Bpmatrix%7D%5Ccdot%5Cbegin%7Bpmatrix%7D-1%5C%5CN%5Cend%7Bpmatrix%7D%3D0%5Cimplies%20-1%2Bf%27(x_0)N%3D0%5Cimplies%20N%3D%5Cfrac%7B1%7D%7Bf%27(x_0)%7D)
所以：
![\boldsymbol{n}=\begin{pmatrix}-1\\\frac{1}{f'(x_0)}\end{pmatrix}\implies 法线斜率为：-\frac{1}{f'(x_0)}](https://www.zhihu.com/equation?tex=%5Cboldsymbol%7Bn%7D%3D%5Cbegin%7Bpmatrix%7D-1%5C%5C%5Cfrac%7B1%7D%7Bf%27(x_0)%7D%5Cend%7Bpmatrix%7D%5Cimplies%20%E6%B3%95%E7%BA%BF%E6%96%9C%E7%8E%87%E4%B8%BA%EF%BC%9A-%5Cfrac%7B1%7D%7Bf%27(x_0)%7D)
知道法线斜率，并且知道过![A=(x_0,f(x_0))](https://www.zhihu.com/equation?tex=A%3D(x_0%2Cf(x_0))) ，就可以求出![xy](https://www.zhihu.com/equation?tex=xy) 坐标系下的法线函数：
![i(x)=-\frac{1}{f'(x_0)}(x-x_0)+f(x_0)](https://www.zhihu.com/equation?tex=i(x)%3D-%5Cfrac%7B1%7D%7Bf%27(x_0)%7D(x-x_0)%2Bf(x_0))
最新版本可以参看：[dx,dy是什么？](https://www.matongxue.com/madocs/849.html)
