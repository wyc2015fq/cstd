# 【Python】np.log - YZXnuaa的博客 - CSDN博客
2018年03月11日 21:22:17[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：7691
此处先介绍log常用的两个底数计算（以10为底和以e为底），以后再补充完毕 
1以10为底：
```
import numpy as np
np.log10(x)
```
如：
```python
>>> np.log10(100)
2.0
>>>
```
2，e为底 
log下什么都不写默认是自然对数 
如：
```
>>> np.log(np.e)
1.0
>>> np.log(10)
2.3025850929940459
```
