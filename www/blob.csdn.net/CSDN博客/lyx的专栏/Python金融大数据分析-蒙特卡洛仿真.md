# Python金融大数据分析-蒙特卡洛仿真 - lyx的专栏 - CSDN博客





2016年12月14日 17:06:50[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：12714








## 1.简单的例子

![](https://img-blog.csdn.net/20161213211547907?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


    了解一点金融工程的对这个公式都不会太陌生，是用现在股价预测T时间股价的公式，其背后是股价符合几何布朗运动，也就是大名鼎鼎的BSM期权定价模型的基础。

    我们假设现在一个股票的价值是100，那么两年后是多少呢？



```python
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
S0 = 100
r = 0.05
sigma = 0.25
T = 2.0
I = 10000
ST1 = S0*np.exp((r - 0.5*sigma**2)*T+sigma*np.sqrt(T)*np.random.standard_normal(I))
plt.hist(ST1,bins = 50)
plt.xlabel('price')
plt.ylabel('ferquency')
```
    运行的结果如下所示：


![](https://img-blog.csdn.net/20161213212014882?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


    很明显，是一个lognormal分布，因为这样的假设下，价格符合lognormal分布，收益率符合正态分布。

## 2.简单的蒙特卡洛路径

    上面是一步到位的，那么如果我们中间分很多个小时间段来仿真呢？可以知道，物理问题是一样的，结果也不会有差异。



```python
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import scipy.stats as scs
S0 = 100
r = 0.05
sigma = 0.25
T = 2.0
I = 10000
#ST1 = S0*np.exp((r - 0.5*sigma**2)*T+sigma*np.sqrt(T)*np.random.standard_normal(I))
#plt.hist(ST1,bins = 50)
#plt.xlabel('price')
#plt.ylabel('ferquency')

M = 50
dt = T/M
S = np.zeros((M + 1,I))
S[0] = S0
print S[0]
for t in range(1,M+1):
    S[t] = S[t-1]*np.exp((r-0.5*sigma**2)*dt+sigma*np.sqrt(dt)*np.random.standard_normal(I))
plt.hist(S[-1],bins = 50)
plt.xlabel('price')
plt.ylabel('frequency')
plt.show()
plt.plot(S[:,:],lw = 1.5)
plt.xlabel('time')
plt.ylabel('price')
plt.show()
```


![](https://img-blog.csdn.net/20161214170401180?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20161214170404867?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

    我们不仅可以得到最终的分布，也可以知道价格路径，而这一价格路径，才是真正代表了蒙特卡洛的精髓。

如果我们绘制得路径更加多一点，就是这样的一个效果：

![](https://img-blog.csdn.net/20161214170526269?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


    从侧面看，其实就是一个lognormal分布。



























