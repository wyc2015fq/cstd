# python绘图：matplotlib绘图（2） - Keith - CSDN博客





2017年02月10日 09:26:13[ke1th](https://me.csdn.net/u012436149)阅读数：718








# matplotlib绘图（2）

## plt.bar

```python
colors=["#348ABD","#A60628"]
plt.bar([0,.7], prior, alpha=0.70, width=0.25, color=colors[0],
label="prior distribution", lw="3", edgecolor="#348ABD")
plt.bar([.25, .7+.25], posterior, alpha=.7, width=0.25,
color=colors[1],label="posterior dis", lw="3",edgecolor="#A60628")
#第一个参数：x轴位置，第二个参数：位置对应的值，alpha：透明度，width：bar的宽度。
#以上代码在一个axes上，画了两个bar
#plt.plot, 类似
```

```python
#x轴对应的位置加上对应的标签
plt.xticks([0.10,0.95], ["Librarian", "Farmer"])


#设置label
plt.set_xlabel("string")
plt.set_ylabel("string")
```

## 实例

```python
%matplotlib inline
import numpy as np
from matplotlib import pyplot as plt
colors=["#348ABD","#A60628"]
prior = [1/21., 20/21.]
posterior = [0.087, 1-0.087]
plt.bar([0,.7], prior, alpha=0.70, width=0.25, color=colors[0],label="prior distribution", lw="3", edgecolor="#348ABD")
plt.bar([.25, .7+.25], posterior, alpha=.7, width=0.25, color=colors[1],label="posterior dis", lw="3",edgecolor="#A60628")
plt.xticks([0.10,0.95], ["Librarian", "Farmer"])
plt.title(".......")
plt.ylabel("Prob")
plt.legend(loc="upper left")
```

```
<matplotlib.legend.Legend at 0x7f952983dac8>
```

![这里写图片描述](https://img-blog.csdn.net/20170210180008448?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjQzNjE0OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



