# Python金融大数据分析-回归分析 - lyx的专栏 - CSDN博客





2016年12月12日 23:47:04[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：10839








## 1.pandas的线性回归        

回归分析是金融中一个绕不过的话题，其实最好的工具应该是R语言，但是pandas其实也是能够胜任绝大部分工作的。

        这里我们就简单介绍一下。



```python
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
noise = np.random.normal(0,12,100)
x= np.array(range(100))
y = 0.7*x + noise
pp = pd.DataFrame({'xvalue':x,'yvalue':y})
model = pd.ols(y=pp['yvalue'],x=pp['xvalue'])
print model
plt.plot(x,y,'r.')
plt.plot(x,model.beta[1]+model.beta[0]*x,'b')
```

        代码很简单，不用做过多的解释

         我们可以看一下程序的输出，以及图形上的表示。

![](https://img-blog.csdn.net/20161212234139026?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


        这里，pandas的回归给出了上图的分析。决策系数是0.7621，调整后的是0.7597，不过笔者这里有一个疑问，一元线性回归的调整系数有意义吗？

        p-value很小，越小越拒绝，所以，我们拒绝原假设，换句话说，这里的水平是显著的。我们可以看到，我们程序中x和y的关系是没有intercept项的，但是在回归的时候却产生了。但是我们可以看到，他的p-value是很大的，所以，讲道理我们是可以拒绝原假设，换句话说，截距项是不显著的，也就是说，我们不能承认这一截距项是对的。

![](https://img-blog.csdn.net/20161212234043992?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


        然后绘制一个图，直观的看一下回归的过程，总的来说，效果还是很好的。当然，如果我们用的是真实世界的数据，恐怕就不会那么好了吧。

## 2.numpy的回归拟合



```python
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
#定义一个函数，用于回归
def f(x):
    return np.sin(x) + 0.5*x
#对x抽样
x = np.linspace(-2*np.pi,2*np.pi,50)
#利用一阶线性进行回归
reg = np.polyfit(x,f(x),deg = 1)
#rg就是回归之后的y的取值
ry = np.polyval(reg,x)
plt.plot(x,f(x),'b')
plt.plot(x,ry,'r.')
```
        numpy中的拟合更加具有实用意义，其实我们可以改变deg来进行不同阶的多项式的拟合。

既然我们可以进行多项式拟合，那么也就可以给出不同的拟合基函数。

        上面的例子中，我们的曲线实际上是由sin函数和x组合的，所以，假设我们知道了这样的情况，然后，选好这样的两个基，然后进行回归拟合，应该会得到更加好的效果，而实际上也确实是这样的。



```python
matrix = np.zeros((2,len(x)))
matrix[1,:] = np.sin(x)
matrix[0,:] = x
reg = np.linalg.lstsq(matrix.T,f(x))[0]
print reg
ry = np.dot(reg,matrix)
plt.plot(x,f(x),'b')
plt.plot(x,ry,'r.')
```
        在上面的代码中，我们首先初始化了一个matrix，这个matrix就是用来存储我们的基的。一个是sin一个是x。然后用linalg，线性模拟函数，最小化平方和的方法获取reg，然后用dot方法或者拟合后的y值，如果对矩阵或者线性代数很了解的话，这一过程应该是可以很容易就理解的。

         我们绘制出来后是这样的结果。

![](https://img-blog.csdn.net/20161213151428238?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


        发现拟合的非常完美，然后拟合系数打印出来后时候0.5与1，这和我们设置的完全一样。








