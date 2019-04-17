# 64位Win10安装Pytorch - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年09月06日 09:40:39[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：108
个人分类：[Windows](https://blog.csdn.net/fjssharpsword/article/category/6480443)









![](https://img-blog.csdn.net/20180906093751926?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zqc3NoYXJwc3dvcmQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180906093801669?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zqc3NoYXJwc3dvcmQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

参考：[https://zhuanlan.zhihu.com/p/26871672](https://zhuanlan.zhihu.com/p/26871672)

加清华源，然后使用conda进行安装。

Pytorch示例：

```python
9:39:49
浪墨 2018/9/6 9:39:49
# -*- coding: utf-8 -*-
'''
Created on 2018年9月6日

@author: user
'''
import torch
import numpy as np

if __name__ == "__main__":  
    # 创建一个np array
    a = np.array([[1,2], [3,4]])
    b = torch.from_numpy(a)      # 根据np array创建torch 张量
    c = b.numpy()                # 根据张量, 导出np array
    print (c)
```





