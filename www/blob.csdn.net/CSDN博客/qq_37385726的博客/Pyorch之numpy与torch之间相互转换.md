# Pyorch之numpy与torch之间相互转换 - qq_37385726的博客 - CSDN博客





2018年08月15日 11:12:18[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：4175
所属专栏：[Pytorch](https://blog.csdn.net/column/details/26172.html)









# **目录**

[代码](#%E4%BB%A3%E7%A0%81)

[输出](#%E8%BE%93%E5%87%BA)

> 
**numpy中的ndarray转化成pytorch中的tensor : torch.from_numpy()**

**pytorch中的tensor转化成numpy中的ndarray : numpy()**


# 代码

```python
import numpy as np
import torch

np_arr = np.array([1,2,3,4])
tor_arr=torch.from_numpy(np_arr)
tor2numpy=tor_arr.numpy()
print('\nnumpy\n',np_arr,'\ntorch\n',tor_arr,'\nnumpy\n',tor2numpy)
```

# 输出

numpy

 [1 2 3 4] 

torch

 tensor([1, 2, 3, 4], dtype=torch.int32) 

numpy

 [1 2 3 4]



