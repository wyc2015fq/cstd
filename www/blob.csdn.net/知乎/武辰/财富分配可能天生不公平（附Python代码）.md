# 财富分配可能天生不公平（附Python代码） - 知乎
# 

收入分配是福利经济学中的重要话题。假设有5个人的收入分别是[99.5，101，100.5，100.2，98.6]，那么可以直观地看出这5个人的收入比较均匀。如果有另外5个人的收入分别是[100，520，13，26，46540]，那么可以直观地看出这5个人的收入不均匀。

基尼系数是衡量一个地区居民收入差距的常用指标。它的取值范围是[0,1]。基尼系数越接近0表明收入分配越趋向平等。国际惯例把基尼系数为0.3以下的情况视为收入比较平均，0.3-0.4视为收入相对合理，0.4-0.5视为收入差距较大，0.5以上表示收入悬殊。

接下来我通过一个简单的游戏来模拟财富分配（在本文中，财富分配等同于收入分配）。假设有一根特别长（可以假设为100公里）的面包，上帝随机在这个面包切99999刀（具体来说，如果把面包固定在数轴上，面包的左端点对应零点，那么每一刀所在的坐标值服从于(0,100公里)的均匀分布）。切完之后面包可以分为十万份。接下来把这十万份小面包分给十万个人。假定面包量意味着收入，那么这十万个人的收入分配是公平的吗？

直观来说，这个分配规则十分公平，因为每个人被分配到的面包量应该是差不多的。最直接的想法是每个人的收入服从均匀分布。如果不是均匀分布，那么正态分布也是一个不错的答案，因为大多数人分到的面包量应该差不多，且只有少数人获得非常大或者非常小的面包量，这两条性质几乎完整地刻画了正态分布的规律。然而理论计算以及程序模拟的结果都与均匀分布或者正态分布大相径庭。

我做了多次Python模拟。下图是十万人获得的面包量的直方图。可以看出它服从幂律分布（可以理论证明这一点）。绝大多数人的面包量都很小，只有少数人的面包量很大。而且它几乎是严格下降的，这一点完全不同于正态分布的先上升后下降的图像。
![](https://pic4.zhimg.com/v2-96d8da788af7f859bac646069da01b7f_b.jpg)
幂律分布的结果也是符合现实生活的。大量的研究显示，各个国家的财富分配服从于幂律分布。幂律分布能导出著名的二八法则：20%的人掌握了80%的财富。

幂律分布还有一个很有趣的性质：它具有自相似性（是不是想到了神奇的分形？）。假如说你是一个点，你不管站在这个分布曲线的哪个位置，都无法通过向前看或者向后看来判断出你所在的坐标。直白地说，穷人遇到的困境与富人遇到的困境是一样的（当然这里所说的困境是指因财富间比较而引起的困境）。在理想情况下，假如你是个穷人，那么总有比你更穷的；假如你是个富人，那么总有比你更富的。

经过计算得出基尼系数为 ![\frac{1}{2}](https://www.zhihu.com/equation?tex=%5Cfrac%7B1%7D%7B2%7D) 。这在国际通用标准中已经几乎到了“收入悬殊”的地步！**一个看似十分公平的分配方式，竟然会导致悬殊的财富分配**，不得不说这是一个反直觉的数学结论。

不过这个结论可能并非那么反直觉。哪怕所有人的财富分布都严格服从均匀分布，最后计算的基尼系数也会到达 ![\frac{1}{3}](https://www.zhihu.com/equation?tex=%5Cfrac%7B1%7D%7B3%7D) （这个例子不是那么准确，因为均匀分布不代表所有人的财富是均匀的，均匀分布只是表示每个财富值的概率都是相等的；事实上，如果财富分配绝对均匀，那么财富的分布应该是退化分布，对应地，退化分布的基尼系数为0）。这是因为即使是均匀分布也会有不小的方差存在，而方差会使得大家的收入分配不均。

我在本文用一个简单的模型去模拟财富分配，仅是本人的一家之言。现实生活中的财富分配情况远远比这个模型复杂。但尽管如此，我认为这个结论也是具有一定的启发性的。

以下是Python代码：

```python
# -*- coding: utf-8 -*-
"""
@author: 武辰
"""
import matplotlib.pyplot as plt
import random
import numpy as np 
#打算要在面包上切n刀
n=
#cut_list的元素是每一刀对应的数轴上的值，它服从均匀分布
#alocate_list的元素是每个人得到的面包量，它服从幂律分布
cut_list=[]
alocate_list=[]
#生成cut_list，并排序
for i in range(n):
    x=random.random()
    cut_list.append(x)
cut_list.sort()
#生成alocate_list
for i in range(n):
    if i == :
     alocate_list.append(cut_list[i])
    else:
        temp=cut_list[i]-cut_list[i-]
        alocate_list.append(temp)
#画出每一刀的位置的直方图
brr=cut_list
plt.figure("cut_list")
n, bins, patches = plt.hist(brr, bins=,facecolor='green', alpha=0.75)  
plt.show()
#画出每一个人得到的面包量的直方图
arr=alocate_list
plt.figure("alocate_list")
n, bins, patches = plt.hist(arr, bins=,facecolor='green', alpha=0.75)  
plt.show()
#解决matplotlib显示中文乱码问题
plt.rcParams['font.sans-serif'] = ['SimHei']
plt.rcParams['axes.unicode_minus'] = False
fig, ax = plt.subplots()
#画出洛伦兹曲线并计算基尼系数
def Gini():
    #计算数组累计值,从 0 开始
    wealths = alocate_list
    cum_wealths = np.cumsum(sorted(np.append(wealths, )))
    sum_wealths = cum_wealths[-]
    #累积占比
    xarray = np.array(range(, len(cum_wealths))) / np.float(len(cum_wealths) - )
    upper = xarray
    yarray = cum_wealths / sum_wealths
    #绘制基尼系数对应的洛伦兹曲线
    ax.plot(xarray, yarray)
    ax.plot(xarray, upper)
    ax.set_xlabel(u'人数累积占比')
    ax.set_ylabel(u'面包量累积占比')
    plt.show()
    #计算曲线下面积的通用方法
    B = np.trapz(yarray, x=xarray)
    #总面积 0.5
    A = 0.5 - B
    G = A / (A + B)
    return G
a=Gini()
print(a)
```

