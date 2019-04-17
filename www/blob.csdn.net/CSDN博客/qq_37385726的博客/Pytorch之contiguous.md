# Pytorch之contiguous - qq_37385726的博客 - CSDN博客





2018年08月18日 17:52:36[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：434
所属专栏：[Pytorch](https://blog.csdn.net/column/details/26172.html)









# **目录**

[contiguous](#contiguous)

[is_contiguous](#is_contiguous)

# contiguous



### tensor变量调用contiguous()函数会使tensor变量在内存中的存储变得连续。

contiguous()：**view只能用在contiguous的variable上**。如果在view之前用了transpose, permute等，需要用contiguous()来返回一个contiguous copy。 

> 
**一种可能的解释是： **

有些tensor并不是占用一整块内存，而是由不同的数据块组成，而tensor的`view()`操作依赖于内存是整块的，这时只需要执行`contiguous()`这个函数，把tensor变成在内存中连续分布的形式。 




# is_contiguous

### 判断是否contiguous用`torch.Tensor.is_contiguous()`函数。

```
import torch
x = torch.ones(10, 10)
x.is_contiguous()  # True
x.transpose(0, 1).is_contiguous()  # False
x.transpose(0, 1).contiguous().is_contiguous()  # True
```

在pytorch的最新版本0.4版本中，增加了torch.reshape(), 这与 numpy.reshape 的功能类似。**它大致相当于 tensor.contiguous().view()**



