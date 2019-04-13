
# 用梯度下降求解最小二乘线性回归python实现 - bitcarmanlee的博客 - CSDN博客


2018年10月14日 23:43:45[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：243



## 1.前言
最小二乘法线性回归作为最基础的线性回归，在统计和机器学习中都有重要的地位。在机器学习中，线性回归用来从数据中获得启示来帮助预测，因此如何得到最拟合数据的函数和防止过拟合是研究重点。
假设我们的拟合函数是$y = ax + b$，标准的线性最小二乘采用MSE做为loss function。那么在用梯度下降求解的时候，参数a,b对应的梯度分别为:
$$
\frac{\partial}{\partial a} L(a, b) = \frac{\partial}{\partial a} \left(\frac{1}{2m} \sum_{i=1}^m(ax^i + b - y^i)^2 \right) = \frac{1}{m}\sum_{i=1}^m(ax^i + b - y^i) x^i
$$

$$
\frac{\partial}{\partial b} L(a, b) = \frac{\partial}{\partial b} \left(\frac{1}{2m} \sum_{i=1}^m(ax^i + b - y^i)^2 \right) = \frac{1}{m}\sum_{i=1}^m(ax^i + b - y^i)
$$

## 2.梯度下降求解最小二乘
`import numpy as np
                                              
alpha = 0.01                                  
eps = 1e-6                                    
                                              
x = [1., 2., 3., 4., 5., 6., 7., 8., 9.]      
y = [3., 5., 7., 9., 11., 13., 15., 17., 19.] 
def solve_by_gradient():
    # m is sample num
    m = len(x)
    a, b= 0, 0
    sse, sse_new = 0, 0
    grad_a, grad_b = 0, 0
    count = 0
    for step in range(100000):
        count += 1
        for i in range(m):
            base = a * x[i] + b - y[i]
            grad_a += x[i] * base
            grad_b += base
            grad_a = grad_a / m
            grad_b = grad_b / m
            a -= alpha * grad_a
            b -= alpha * grad_b
            # loss function: Mean Squared Error, MSE
            # because 2m is a const, so 1/2m can be ignored
            for j in range(m):
                sse_new += (a * x[j] + b - y[j]) ** 2
        if abs(sse_new - sse) < eps:
            break
        else:
            sse = sse_new
    print('{0} * x + {1}'.format(a, b))
    print "count is: " , count
solve_by_gradient()`上面的代码严格按照梯度进行迭代而来，最后输出的结果为：
`2.00003690546 * x + 0.999758438895
count is:  3386`由结果可知，最后还是比好好的拟合出了数据反应的y=2x+1的规律。
不过问题也比较明显，也比较好的证明了梯度下降的一个缺点：收敛速度很慢。像我们这个简单的例子，用了3386次迭代才最终收敛。
## 3.用矩阵求解的方式直接计算
在参考文献1中，我们给出了最小二乘矩阵求解的直接计算方式：
$$
\theta = (A^TA)^{-1}A^TY
$$
利用这个公式我们来实现一下：
`x = [1., 2., 3., 4., 5., 6., 7., 8., 9.]
y = [3., 5., 7., 9., 11., 13., 15., 17., 19.]

def solve_by_gd_matrix():
    x0 = [1.0 for i in range(9)]
    xarray = np.column_stack((x, x0))
    xmatrix = np.mat(xarray, float)
    yarray = np.array(y)
    ymatrix = np.mat(yarray, float)
    theta = (xmatrix.T * xmatrix).I * xmatrix.T * ymatrix.T
    print(theta)`代码中的x0，就是相当于偏置项b。$\theta$求解公式直接套用上面的公式，最后代码运行的结果为:
`[[2.]
 [1.]]`直接精确求出a=2, b=1。
最小二乘法矩阵求解的推导过程如下，内容来自参考文献2.
![在这里插入图片描述](https://img-blog.csdn.net/20181014234200400?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JpdGNhcm1hbmxlZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![在这里插入图片描述](https://img-blog.csdn.net/20181014234225221?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JpdGNhcm1hbmxlZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![在这里插入图片描述](https://img-blog.csdn.net/20181014234256873?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JpdGNhcm1hbmxlZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
参考文献：
1.[https://mp.csdn.net/mdeditor/51589143\#](https://mp.csdn.net/mdeditor/51589143#)
2.[https://zhuanlan.zhihu.com/p/33899560](https://zhuanlan.zhihu.com/p/33899560)

