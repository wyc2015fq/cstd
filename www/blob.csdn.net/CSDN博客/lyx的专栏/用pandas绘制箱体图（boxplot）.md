# 用pandas绘制箱体图（boxplot） - lyx的专栏 - CSDN博客





2016年10月18日 20:06:45[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：14802








箱体图是一种用于表示分布的图像，由五个分位数组成。很好用的图，但是excel要生成这个可就得曲线救国了，然而如果用python加上pandas的话就很easy啦。

话不多说，上代码：



```python
# -*- coding: utf-8 -*-
"""
Created on Tue Sep 13 16:41:47 2016

@author: Luyixiao
"""
import numpy as np  
import matplotlib.pyplot as plt  
import pandas as pd
def list_generator(mean,dis,number):#封装一下这个函数，用来后面生成数据
    return np.random.normal(mean,dis*dis,number)#normal分布，输入的参数是均值、标准差以及生成的数量
#我们生成四组数据用来做实验，我们都只生成100个数据
list1=list_generator(0.8531,0.0956,100)
list2=list_generator(0.8631,0.0656,100)
list3=list_generator(0.8731,0.1056,100)
list4=list_generator(0.8831,0.0756,100)
#把四个list导入到pandas的数据结构中，dataframe
data = pd.DataFrame({"Hausdorff":list1,
                     "City-block":list2,
                     "Wasserstein":list3,
                     "KL-divergence":list4})
data.boxplot()#这里，pandas自己有处理的过程，很方便哦。
plt.ylabel("ARI")
plt.xlabel("Dissimilarity Measures")#我们设置横纵坐标的标题。
plt.show()
```
然后我们的输出结果是这样的一个boxplot。
![](https://img-blog.csdn.net/20161018200540029?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

这就是箱体图了。很easy，但是表现力很强哦。





