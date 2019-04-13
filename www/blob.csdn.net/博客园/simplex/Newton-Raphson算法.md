
# Newton-Raphson算法 - simplex - 博客园|[simplex](https://www.cnblogs.com/simplex/)
|
|[博客园](https://www.cnblogs.com/)|::|[首页](https://www.cnblogs.com/simplex/)|::|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|::|[联系](https://msg.cnblogs.com/send/simplex)|::|[订阅](https://www.cnblogs.com/simplex/rss)![订阅](//www.cnblogs.com/images/xml.gif)|::|[管理](https://i.cnblogs.com/)|17 				Posts ::				0 Stories				::				5 Comments				::				0 Trackbacks|
|

|公告


|简介
|牛顿法又叫做牛顿-拉裴森（Newton-Raphson）方法，是一维求根方法中最著名的一种。其特点是在计算时需要同时计算函数值与其一阶导数值，从几何上解释，牛顿法是将当前点处的切线延长，使之与横轴相交，然后把交点处值作为下一估值点。
![](https://img2018.cnblogs.com/blog/1006242/201810/1006242-20181030222733448-1247752563.png)
|图1
|从数学上解释，牛顿法可以从函数的泰勒展开得到。\(f(x)\)的泰勒展开可以表示为：
|\(f(x+\delta)=f(x)+f’(x)\delta+\frac{f’’(x)}{2}\delta^2+O(\delta^3)\)
|对于足够小的\(\delta\)，可以将只保留上式右端关于的一阶项，得到：
|\(\delta=-\frac{f(x)}{f’(x)}\)
|于是得到由到的递推公式：
|\(x_{i+1}=x_{i}+\delta=x_i-\frac{f(x_i)}{f’(x_i)}\)
|可见牛顿法是让\(x\)沿着\(f(x)\)梯度的方向下降，类似于最优化方法中的梯度下降法。牛顿法也可以作为最优化算法，只不过那时需要求函数的二阶导数。
|牛顿法相比二分法、截弦法的优点是收敛速度可以达到二阶，在根附近没迭代一次，结果的有效数字几乎可以翻倍。当然牛顿法也可能可能失败，比如收敛到一个局部极值，其切线方向与横轴水平，从而无法计算下一个迭代值。
|另外，牛顿法的实现需要用户提供一个函数用于计算函数值\(f(x)\)与其一阶导数值\(f'(x)\)，因此比较适合函数的导数可以解析求出的情况，如果需要求数值导数，则牛顿法的收敛速度和精度都会受影响。
|我们可以将牛顿法和二分法综合起来形成一个混合算法，一旦牛顿法在运行过程中出现解跳出给定区间或者猜测值远离实际根导致收敛速度较慢时，就采取一步二分法。
|实现一：利用预先求出的一阶导函数
|import|numpy as np|import|matplotlib.pyplot as plt|def|f(FV, PMT, r, n):|return|PMT * (1 + r) * (((1 + r)**n - 1)) / r +|FV|def|df(FV, PMT, r, n):
    r_plus_1_power_n|= (1 + r)**|n
    p1|= N * PMT * r_plus_1_power_n /|r
    p2|= -PMT * (r + 1) * (r_plus_1_power_n - 1) / r /|r
    p3|= PMT * (r_plus_1_power_n - 1) /|r|return|p1 + p2 +|p3|def|newtonRaphson2(FV,
                   PMT,
                   n,
                   f,
                   df,
                   xmin,
                   xmax,
                   maxit,
                   shift|=0.0001|,
                   tol|=1.0e-9|):|'''|函数作用说明：计算组合收益率
    FV：目标金额
    PMT：每期投资金额
    n:定投期数
    f：函数值（根据要求的方程自定义）
    df：导数值（根据要求的方程自定义）
    xmin：根的下限
    xmax：根的上限
    maxit：最大迭代次数
    tol：计算精度|'''|import|math
    fxmin|=|f(FV, PMT, xmin, n)|if|fxmin == 0.0|:|return|xmin
    fxmax|=|f(FV, PMT, xmax, n)|if|fxmax == 0.0|:|return|xmax|if|fxmin * fxmax > 0.0|:|print|(|'|Root is not bracketed|'|)|\#|在[xmin, xmax]内函数不变号（没根），或者是变了偶数次号（多个根）|return|1|if|fxmin < 0.0:|\#|确定搜索方向使f(xl)<0|xl =|xmin
        xh|=|xmax|else|:
        xl|=|xmax
        xh|=|xmin
    x|= 0.5 * (xmin + xmax)|\#|根的预测值|if|x ==|0:
        x|+=|shift
    fx, dfx|= f(FV, PMT, x, n), df(FV, PMT, x, n)|\#|求f(x)和其一阶导数|dxold = math.fabs(xmax - xmin)|\#|储存步长|dx =|dxold|for|ii|in|range(maxit):|\#|牛顿法的解跳出解区间或者收敛速度太慢，则下一步改用二分法|if|((x - xh) * dfx - fx) * ((x - xl) * dfx - fx) > 0.0|or|(
                math.fabs(|2 * fx) > math.fabs(dxold *|dfx)):|\#|二分法|dxold =|dx
            dx|= 0.5 * (xh -|xl)
            x|= xl +|dx|else|:|\#|牛顿法|dxold =|dx
            dx|= fx /|dfx
            temp|=|x
            x|-=|dx|if|temp ==|x:|print|(|"|total iterate time:%s|"|%|ii)|return|x|if|math.fabs(dx) < tol:|\#|达到要求精度，返回找到的根|print|(|"|total iterate time:%s|"|%|ii)|return|x|if|x ==|0:
            x|+=|shift
        fx, dfx|= f(FV, PMT, x, n), df(FV, PMT, x, n)|\#|否则继续迭代，求f(x)和其一阶导数|if|fx < 0.0:|\#|使根保持在解区间内|xl =|x|else|:
            xh|=|x|print|(|'|Maximum number of iterations exceeded|'|)|return|1|\#|\#\# 测试用例：首先给定PMT，n，r_analytical，计算FV，然后利用PMT，n，FV计算r_numerical，两者应该相等|\#|\#给定r_analytical计算FV|R=0.1|r_analytical|= R / 12|PMT|= -4e3|N|= 30|n|= N * 12|FV|= -PMT * (1 + r_analytical) * (((1 + r_analytical)**n - 1)) /|r_analytical|\#|\#给定FV反解r_numerical|r_numerical = newtonRaphson2(FV, PMT, n, f, df, -1, 1, 100, tol=1.0e-8|)|print|(|'|\nr_analytical=%s,\nr_numerical=%s\n|'|% (r_analytical, r_numerical))
|实现二：利用TensorFlow提供的自动微分计算导函数
|import|numpy as np|import|math|import|pandas as pd|import|tensorflow as tf|import|matplotlib.pyplot as plt|\#|\#一个利用tensorflow的自动微分功能实现牛顿法解方程的小程序|class|NewtonRaphson:|def|__init__|(self, y, x, session):
        self.y|=|y
        self.x|=|x
        self.grad|=|tf.gradients(y, x)
        self.sess|=|session
        sess.run(tf.global_variables_initializer())|def|_fx(self, x_value):|\#|尽量避免出现f(x)不能计算的情况，比如函数试图计算a/0，log(0)等，如果计算结果为inf则x+0.0001再进行计算|temp|= self.sess.run(y, feed_dict=|{x: [x_value]})[0]|if|np.isinf(temp):|return|self.sess.run(y, feed_dict={x: [x_value + 0.0001|]})[0]|else|:|return|temp|def|_dfx(self, x_value):|return|self.sess.run(self.grad, feed_dict=|{x: [x_value]})[0][0]|def|solve(self, xmin, xmax, maxiter, tol):
        fmin|=|self._fx(xmin)
        fmax|=|self._fx(xmax)|if|fmin ==|0:|return|xmin|if|fmax ==|0:|return|xmax|if|fmin * fmax > 0.0|:|raise|ValueError(|'|Root is not brackted!!|'|)|if|fmin <|0:
            xl|=|xmin
            xh|=|xmax|else|:
            xl|=|xmax
            xh|=|xmin
        x|= (xmin + xmax) / 2|fx, dfx|=|self._fx(x), self._dfx(x)
        dxold|= math.fabs(xmax -|xmin)
        dx|=|dxold|for|ii|in|range(maxiter):|if|((x - xh) * dfx - fx) * ((x - xl) * dfx - fx) > 0.0|or|(
                    math.fabs(|2 * fx) > math.fabs(dxold *|dfx)):
                dxold|=|dx
                dx|= 0.5 * (xh -|xl)
                x|= xl +|dx|else|:
                dxold|=|dx
                dx|= fx /|dfx
                temp|=|x
                x|-=|dx|\#|newton|if|temp ==|x:|print|(|"|total iterate time:%s|"|%|ii)|return|x
            fx, dfx|=|self._fx(x), self._dfx(x)|if|fx < 0.0|:
                xl|=|x|else|:
                xh|=|x|print|(|'|Maximum number of iterations exceeded|'|)|return|1|PV|= 1e4|FV|= 3e6|N|= 20|cpi|= 0.018|RATE|= 0.15|r|= RATE / 12|PMT|= 10000|x|= tf.placeholder(shape=[1], dtype=|tf.float32)
y|=r * (FV * (1 + cpi)**(N) - PV * (r + 1)**x) / ((r + 1)**x - 1 - r) -|PMT
sess|=|tf.InteractiveSession()
solver|=|NewtonRaphson(y,x,sess)

nmin|= 2|nmax|= 300|solver.solve(nmin,nmax,|100,1e-9)






|posted on|2018-10-30 22:39|[simplex](https://www.cnblogs.com/simplex/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=9880131)|[收藏](#)


|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
Copyright @
	simplex
Powered by:[.Text](http://scottwater.com/blog)and[ASP.NET](http://asp.net)
Theme by:[.NET Monster](http://www.DotNetMonster.com)
