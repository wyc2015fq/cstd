# 如何理解拉格朗日乘子法和KKT条件？ - 马同学 - CSDN博客
2018年08月18日 19:13:39[马同学高等数学](https://me.csdn.net/ccnt_2012)阅读数：343
之前简单介绍了拉格朗日乘子法的基本思路：[如何理解拉格朗日乘子法？](https://www.matongxue.com/madocs/939.html)
本文会继续介绍拉格朗日乘子法的细节，以及对其进行适当的推广（也就是所谓的KKT条件）。
**1 无约束下的极值**
**1.1 直观**
根据梯度的意义（参看[如何理解梯度](https://www.matongxue.com/madocs/222.html)）可知，在函数![f(x)](https://www.zhihu.com/equation?tex=f(x)) 的极值点梯度为0：
![](https://img-blog.csdn.net/20180818190857414?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**1.2 代数**
要求（![\text{minimize}](https://www.zhihu.com/equation?tex=%5Ctext%7Bminimize%7D) 的意思是求极小值）：
![\text{minimize}\ f(x)](https://www.zhihu.com/equation?tex=%5Ctext%7Bminimize%7D%5C%20f(x))
只需解如下方程：
![\nabla f=0](https://www.zhihu.com/equation?tex=%5Cnabla%20f%3D0)
**2 单等式约束下的极值**
关于这一节，更详细的请参看：[如何理解拉格朗日乘子法？](https://www.matongxue.com/madocs/939.html)
**2.1 直观**
要求方程![x^2y-3=0](https://www.zhihu.com/equation?tex=x%5E2y-3%3D0) 与原点的最小距离：
![](https://img-blog.csdn.net/20180818190857565?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
问题被转化为了同心圆与![x^2y-3=0](https://www.zhihu.com/equation?tex=x%5E2y-3%3D0) 什么时候相切：
![](https://img-blog.csdn.net/20180818190857717?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
相切就是在极小值点有相同的切线：
![](https://img-blog.csdn.net/20180818190857445?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
只要能通过数学把相切这个条件表示出来，就可以得到解。
我们把同心圆可以看作凸函数![f(x,y)=x^2+y^2](https://www.zhihu.com/equation?tex=f(x%2Cy)%3Dx%5E2%2By%5E2) 的等高线：
![](https://img-blog.csdn.net/20180818190857728?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
把方程![x^2y-3=0](https://www.zhihu.com/equation?tex=x%5E2y-3%3D0) 看作凸函数![g(x,y)=x^2y](https://www.zhihu.com/equation?tex=g(x%2Cy)%3Dx%5E2y) 的等高线中的一条：
![](https://img-blog.csdn.net/20180818190857603?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这样![f](https://www.zhihu.com/equation?tex=f) 的等高线，同心圆，的法线就是![\nabla f](https://www.zhihu.com/equation?tex=%5Cnabla%20f) ：
![](https://img-blog.csdn.net/20180818190857963?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![g](https://www.zhihu.com/equation?tex=g) 的等高线的其中一条，方程![x^2y-3=0](https://www.zhihu.com/equation?tex=x%5E2y-3%3D0) ，的法线就是![\nabla g](https://www.zhihu.com/equation?tex=%5Cnabla%20g)
![](https://img-blog.csdn.net/20180818190857978?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
两者相切就意味着，在切点，两者法线平行：
![](https://img-blog.csdn.net/20180818190858163?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
也就是：
![\nabla f+\lambda\nabla g=0](https://www.zhihu.com/equation?tex=%5Cnabla%20f%2B%5Clambda%5Cnabla%20g%3D0)
**2.2 代数**
上面的问题形式化后，用代数表示为（![\text{subject to}](https://www.zhihu.com/equation?tex=%5Ctext%7Bsubject%20to%7D) 的意思是服从于，约束于的意思）：
![\begin{aligned}    & \text{minimize} & & f(x,y) \\    & \text{subject to} & & g(x,y) = 3\end{aligned}](https://www.zhihu.com/equation?tex=%0A%5Cbegin%7Baligned%7D%0A%20%20%20%20%26%20%5Ctext%7Bminimize%7D%20%26%20%26%20f(x%2Cy)%20%5C%5C%0A%20%20%20%20%26%20%5Ctext%7Bsubject%20to%7D%20%26%20%26%20g(x%2Cy)%20%3D%203%0A%5Cend%7Baligned%7D%0A)
只需解如下方程组：
![\begin{cases}    \nabla f+\lambda\nabla g=0\\    \\    g(x,y)=3\end{cases}](https://www.zhihu.com/equation?tex=%0A%5Cbegin%7Bcases%7D%0A%20%20%20%20%5Cnabla%20f%2B%5Clambda%5Cnabla%20g%3D0%5C%5C%0A%20%20%20%20%5C%5C%0A%20%20%20%20g(x%2Cy)%3D3%0A%5Cend%7Bcases%7D%0A)
**3 多等式约束下的极值**
比如下图：
![](https://img-blog.csdn.net/2018081819085894?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
要求![f](https://www.zhihu.com/equation?tex=f) 被![g_1=0,g_2=0](https://www.zhihu.com/equation?tex=g_1%3D0%2Cg_2%3D0) 约束后的极值，可以证明在极值点![\nabla f](https://www.zhihu.com/equation?tex=%5Cnabla%20f) 必然在![\nabla g_1,\nabla g_2](https://www.zhihu.com/equation?tex=%5Cnabla%20g_1%2C%5Cnabla%20g_2) 张成的空间中。
那么上面的问题形式化后就是：
![\begin{aligned}    & \text{minimize} & & f \\    & \text{subject to} & & g1=0,g_2=0\end{aligned}](https://www.zhihu.com/equation?tex=%0A%5Cbegin%7Baligned%7D%0A%20%20%20%20%26%20%5Ctext%7Bminimize%7D%20%26%20%26%20f%20%5C%5C%0A%20%20%20%20%26%20%5Ctext%7Bsubject%20to%7D%20%26%20%26%20g1%3D0%2Cg_2%3D0%0A%5Cend%7Baligned%7D%0A)
只需解如下方程组：
![\begin{cases}    \nabla f+\lambda_1\nabla g_1+\lambda_2\nabla g_2=0\\    \\    g1=0,g_2=0\end{cases}](https://www.zhihu.com/equation?tex=%0A%5Cbegin%7Bcases%7D%0A%20%20%20%20%5Cnabla%20f%2B%5Clambda_1%5Cnabla%20g_1%2B%5Clambda_2%5Cnabla%20g_2%3D0%5C%5C%0A%20%20%20%20%5C%5C%0A%20%20%20%20g1%3D0%2Cg_2%3D0%0A%5Cend%7Bcases%7D%0A)
更一般的，如果有![n](https://www.zhihu.com/equation?tex=n) 个约束等式：
![\begin{aligned}    & \text{minimize} & & f \\    & \text{subject to} & & g_i=0,i=1,2,\cdots,n\end{aligned}](https://www.zhihu.com/equation?tex=%0A%5Cbegin%7Baligned%7D%0A%20%20%20%20%26%20%5Ctext%7Bminimize%7D%20%26%20%26%20f%20%5C%5C%0A%20%20%20%20%26%20%5Ctext%7Bsubject%20to%7D%20%26%20%26%20g_i%3D0%2Ci%3D1%2C2%2C%5Ccdots%2Cn%0A%5Cend%7Baligned%7D%0A)
只需解如下方程组：
![\begin{cases}    \displaystyle\nabla f+\sum_{i}^{n}\lambda_i\nabla g_i=0    \\    g_i=0,i=1,2,\cdots,n\end{cases}](https://www.zhihu.com/equation?tex=%0A%5Cbegin%7Bcases%7D%0A%20%20%20%20%5Cdisplaystyle%5Cnabla%20f%2B%5Csum_%7Bi%7D%5E%7Bn%7D%5Clambda_i%5Cnabla%20g_i%3D0%0A%20%20%20%20%5C%5C%0A%20%20%20%20g_i%3D0%2Ci%3D1%2C2%2C%5Ccdots%2Cn%0A%5Cend%7Bcases%7D%0A)
**4 不等式约束下的极值**
比如，我们要求刚才同心圆的最小值：
![](https://img-blog.csdn.net/20180818190858380?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
那肯定就是原点啦，半径为0肯定就是最小值了。
从代数上看就是要求：
![\text{minimize}\ f(x,y)=x^2+y^2](https://www.zhihu.com/equation?tex=%5Ctext%7Bminimize%7D%5C%20f(x%2Cy)%3Dx%5E2%2By%5E2)
解：
![\nabla f=0\implies (x,y)=(0,0)](https://www.zhihu.com/equation?tex=%5Cnabla%20f%3D0%5Cimplies%20(x%2Cy)%3D(0%2C0))
**4.1 情况一**
我们给它添加一个不等式约束，也就是求：
![\begin{aligned}    & \text{minimize} & & f(x,y) \\    & \text{subject to} & & h(x,y)=x+y \le 1\end{aligned}](https://www.zhihu.com/equation?tex=%0A%5Cbegin%7Baligned%7D%0A%20%20%20%20%26%20%5Ctext%7Bminimize%7D%20%26%20%26%20f(x%2Cy)%20%5C%5C%0A%20%20%20%20%26%20%5Ctext%7Bsubject%20to%7D%20%26%20%26%20h(x%2Cy)%3Dx%2By%20%5Cle%201%0A%5Cend%7Baligned%7D%0A)
可以看到，这个不等式约束实际上包含了原点：
![](https://img-blog.csdn.net/20180818190858457?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
所以这个约束等于没有，依然求解：
![\nabla f=0\implies (x,y)=(0,0)](https://www.zhihu.com/equation?tex=%5Cnabla%20f%3D0%5Cimplies%20(x%2Cy)%3D(0%2C0))
**4.2 情况二**
换一个不等式约束：
![\begin{aligned}    & \text{minimize} & & f(x,y) \\    & \text{subject to} & & h(x,y)=x+y \le -2\end{aligned}](https://www.zhihu.com/equation?tex=%0A%5Cbegin%7Baligned%7D%0A%20%20%20%20%26%20%5Ctext%7Bminimize%7D%20%26%20%26%20f(x%2Cy)%20%5C%5C%0A%20%20%20%20%26%20%5Ctext%7Bsubject%20to%7D%20%26%20%26%20h(x%2Cy)%3Dx%2By%20%5Cle%20-2%0A%5Cend%7Baligned%7D%0A)
不等式约束看起来是这样的：
![](https://img-blog.csdn.net/20180818190858467?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
因为同心圆是凸函数的等高线，所以等高线的值是这么排列的：
![](https://img-blog.csdn.net/20180818190858534?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
所以，在不等式约束下，最小值是在边缘相切的地方取得：
![](https://img-blog.csdn.net/20180818190858668?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
和用等式![h(x,y)=x+y=-2](https://www.zhihu.com/equation?tex=h(x%2Cy)%3Dx%2By%3D-2) 进行约束效果是一样的：
![](https://img-blog.csdn.net/20180818190858710?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
因此可以通过解方程组求出答案：
![\begin{cases}    \nabla f+\mu\nabla h=0    \\    h(x,y)=x+y=-2\end{cases}](https://www.zhihu.com/equation?tex=%0A%5Cbegin%7Bcases%7D%0A%20%20%20%20%5Cnabla%20f%2B%5Cmu%5Cnabla%20h%3D0%0A%20%20%20%20%5C%5C%0A%20%20%20%20h(x%2Cy)%3Dx%2By%3D-2%0A%5Cend%7Bcases%7D%0A)
**4.3 新增的条件**
仔细研究，不等式实际上带来了新的条件。
同心圆是凸函数的等高线，等高线的值如下排列，所以在相切处，法线也就是![\nabla f](https://www.zhihu.com/equation?tex=%5Cnabla%20f) 的方向如下（法线也就是梯度，指向增长最快的方向，也就是等高线的值变大的方向）：
![](https://img-blog.csdn.net/20180818190858737?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
而凸函数![h(x,y)](https://www.zhihu.com/equation?tex=h(x%2Cy)) 的法线![\nabla h](https://www.zhihu.com/equation?tex=%5Cnabla%20h) 也一样指向![h(x,y)](https://www.zhihu.com/equation?tex=h(x%2Cy)) 增长的方向，这个方向正好和![\nabla f](https://www.zhihu.com/equation?tex=%5Cnabla%20f) 相反：
![](https://img-blog.csdn.net/20180818190858926?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NjbnRfMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
因此：
![\nabla f+\mu\nabla h=0,\mu \ge 0](https://www.zhihu.com/equation?tex=%5Cnabla%20f%2B%5Cmu%5Cnabla%20h%3D0%2C%5Cmu%20%5Cge%200)
其中，![\mu \ge 0](https://www.zhihu.com/equation?tex=%5Cmu%20%5Cge%200) 就表明![\nabla f,\nabla h](https://www.zhihu.com/equation?tex=%5Cnabla%20f%2C%5Cnabla%20h) 方向相反。
因此刚才的方程组可以再增加一个条件：
![\begin{cases}    \nabla f+\mu\nabla h=0    \\    h(x,y)=x+y=-2    \\    \mu \ge 0\end{cases}](https://www.zhihu.com/equation?tex=%0A%5Cbegin%7Bcases%7D%0A%20%20%20%20%5Cnabla%20f%2B%5Cmu%5Cnabla%20h%3D0%0A%20%20%20%20%5C%5C%0A%20%20%20%20h(x%2Cy)%3Dx%2By%3D-2%0A%20%20%20%20%5C%5C%0A%20%20%20%20%5Cmu%20%5Cge%200%0A%5Cend%7Bcases%7D%0A)
**5 KKT条件**
因此，综合上面的所有情况，可以把求如下的极值：
![\begin{aligned}    & \text{minimize} & & f \\    & \text{subject to} & & g_i=0,i=1,2,\cdots,n\\    &                   & & h_i\le 0,i=1,2,\cdots,n\\\end{aligned}](https://www.zhihu.com/equation?tex=%0A%5Cbegin%7Baligned%7D%0A%20%20%20%20%26%20%5Ctext%7Bminimize%7D%20%26%20%26%20f%20%5C%5C%0A%20%20%20%20%26%20%5Ctext%7Bsubject%20to%7D%20%26%20%26%20g_i%3D0%2Ci%3D1%2C2%2C%5Ccdots%2Cn%5C%5C%0A%20%20%20%20%26%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%26%20%26%20h_i%5Cle%200%2Ci%3D1%2C2%2C%5Ccdots%2Cn%5C%5C%0A%5Cend%7Baligned%7D%0A)
通过解下面这个方程组来得到答案：
![\begin{cases}    \displaystyle\nabla f+\sum_{i}^{n}\lambda_i\nabla g_i+\sum_{j}^{m}\mu_j\nabla h_j=0    \\    g_i=0,i=1,2,\cdots,n\\    \\    h_j\le 0,j=1,2,\cdots,m\\    \\    \mu_j \ge 0\\    \\    \mu_j h_j = 0\\\end{cases}](https://www.zhihu.com/equation?tex=%0A%5Cbegin%7Bcases%7D%0A%20%20%20%20%5Cdisplaystyle%5Cnabla%20f%2B%5Csum_%7Bi%7D%5E%7Bn%7D%5Clambda_i%5Cnabla%20g_i%2B%5Csum_%7Bj%7D%5E%7Bm%7D%5Cmu_j%5Cnabla%20h_j%3D0%0A%20%20%20%20%5C%5C%0A%20%20%20%20g_i%3D0%2Ci%3D1%2C2%2C%5Ccdots%2Cn%5C%5C%0A%20%20%20%20%5C%5C%0A%20%20%20%20h_j%5Cle%200%2Cj%3D1%2C2%2C%5Ccdots%2Cm%5C%5C%0A%20%20%20%20%5C%5C%0A%20%20%20%20%5Cmu_j%20%5Cge%200%5C%5C%0A%20%20%20%20%5C%5C%0A%20%20%20%20%5Cmu_j%20h_j%20%3D%200%5C%5C%0A%5Cend%7Bcases%7D%0A)
这个方程组也就是所谓的KKT条件。
进一步解释下方程组的各个项：
![\begin{array}{c|c}    \hline    \\    \quad \displaystyle\nabla f+\sum_{i}^{n}\lambda_i\nabla g_i+\sum_{j}^{m}\mu_j\nabla h_j=0\quad&\quad 等式与不等式约束的梯度的线性组合\quad \\    \quad g_i=0,i=1,2,\cdots,n\quad&\quad等式约束\quad\\    \quad h_j\le 0,j=1,2,\cdots,m\quad&\quad不等式约束\quad\\    \quad \mu_j \ge 0\quad&\quad不等式约束下，法线方向相反\quad\\    \quad \mu_j h_j=0\quad&\quad不等式约束下\begin{cases}情况一：\mu=0,h_j\le 0\\\\情况二：\mu_j \ge 0,h_j=0\end{cases}\quad\\    \\    \hline\end{array}](https://www.zhihu.com/equation?tex=%0A%5Cbegin%7Barray%7D%7Bc%7Cc%7D%0A%20%20%20%20%5Chline%0A%20%20%20%20%5C%5C%0A%20%20%20%20%5Cquad%20%5Cdisplaystyle%5Cnabla%20f%2B%5Csum_%7Bi%7D%5E%7Bn%7D%5Clambda_i%5Cnabla%20g_i%2B%5Csum_%7Bj%7D%5E%7Bm%7D%5Cmu_j%5Cnabla%20h_j%3D0%5Cquad%26%5Cquad%20%E7%AD%89%E5%BC%8F%E4%B8%8E%E4%B8%8D%E7%AD%89%E5%BC%8F%E7%BA%A6%E6%9D%9F%E7%9A%84%E6%A2%AF%E5%BA%A6%E7%9A%84%E7%BA%BF%E6%80%A7%E7%BB%84%E5%90%88%5Cquad%20%5C%5C%0A%20%20%20%20%5Cquad%20g_i%3D0%2Ci%3D1%2C2%2C%5Ccdots%2Cn%5Cquad%26%5Cquad%E7%AD%89%E5%BC%8F%E7%BA%A6%E6%9D%9F%5Cquad%5C%5C%0A%20%20%20%20%5Cquad%20h_j%5Cle%200%2Cj%3D1%2C2%2C%5Ccdots%2Cm%5Cquad%26%5Cquad%E4%B8%8D%E7%AD%89%E5%BC%8F%E7%BA%A6%E6%9D%9F%5Cquad%5C%5C%0A%20%20%20%20%5Cquad%20%5Cmu_j%20%5Cge%200%5Cquad%26%5Cquad%E4%B8%8D%E7%AD%89%E5%BC%8F%E7%BA%A6%E6%9D%9F%E4%B8%8B%EF%BC%8C%E6%B3%95%E7%BA%BF%E6%96%B9%E5%90%91%E7%9B%B8%E5%8F%8D%5Cquad%5C%5C%0A%20%20%20%20%5Cquad%20%5Cmu_j%20h_j%3D0%5Cquad%26%5Cquad%E4%B8%8D%E7%AD%89%E5%BC%8F%E7%BA%A6%E6%9D%9F%E4%B8%8B%5Cbegin%7Bcases%7D%E6%83%85%E5%86%B5%E4%B8%80%EF%BC%9A%5Cmu%3D0%2Ch_j%5Cle%200%5C%5C%5C%5C%E6%83%85%E5%86%B5%E4%BA%8C%EF%BC%9A%5Cmu_j%20%5Cge%200%2Ch_j%3D0%5Cend%7Bcases%7D%5Cquad%5C%5C%0A%20%20%20%20%5C%5C%0A%20%20%20%20%5Chline%0A%5Cend%7Barray%7D%0A)
文章最新版本在（有可能会有后续更新）：[如何理解拉格朗日乘子法与KKT条件？](https://www.matongxue.com/madocs/987.html)
