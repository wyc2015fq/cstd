# pytorch: 如何优雅的将 int list 转成 one-hot形式 - Keith - CSDN博客





2017年08月09日 22:23:50[ke1th](https://me.csdn.net/u012436149)阅读数：8516
所属专栏：[pytorch学习笔记](https://blog.csdn.net/column/details/15023.html)









虽然 pytorch 已经升级到 0.2.0 了，但是，貌似依旧没有简单的 api 来帮助我们快速将 int list 转成 one-hot。那么，如何优雅的实现 one-hot 代码呢？

```python
def one_hot(ids, out_tensor):
    """
    ids: (list, ndarray) shape:[batch_size]
    out_tensor:FloatTensor shape:[batch_size, depth]
    """
    if not isinstance(ids, (list, np.ndarray)):
        raise ValueError("ids must be 1-D list or array")
    ids = torch.LongTensor(ids).view(-1,1)
    out_tensor.zero_()
    out_tensor.scatter_(dim=1, index=ids, src=1.)
    # out_tensor.scatter_(1, ids, 1.0)
```

`scatter_` 是什么鬼？

> 
从 value 中拿值，然后根据 dim 和 index 给自己的相应位置填上值


```python
Tensor.scatter_(dim, index, src)
# index: LongTensor
# out[index[i, j], j] = value[i, j] dim=0
# out[i,index[i, j]] = value[i, j]] dim=1 
# index 的 shape 可以不和 out 的 shape 一致
# value 也可以是一个 float 值, 也可以是一个 FloatTensor 
# 如果 value 是 FloatTensor 的话，那么shape 需要和 index 保持一致
```

## 参考资料

[https://discuss.pytorch.org/t/convert-int-into-one-hot-format/507/3](https://discuss.pytorch.org/t/convert-int-into-one-hot-format/507/3)



