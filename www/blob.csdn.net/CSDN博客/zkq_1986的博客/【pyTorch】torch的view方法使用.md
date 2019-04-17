# 【pyTorch】torch的view方法使用 - zkq_1986的博客 - CSDN博客





2018年11月15日 16:15:38[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：283








view，将Tensor转成特定维数空间。

```python
x = torch.zeros(2,2,3)
x.view(3,4) #等价于x:view(3, -1)
    #-1 表示将剩余元素全部看成这一维度
```

输出：

tensor([[ 0.,  0.,  0.,  0.],

        [ 0.,  0.,  0.,  0.],

        [ 0.,  0.,  0.,  0.]])



