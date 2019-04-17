# pytorch学习笔记(十):learning rate decay(学习率衰减) - Keith - CSDN博客





2017年04月24日 22:06:45[ke1th](https://me.csdn.net/u012436149)阅读数：16095
所属专栏：[pytorch学习笔记](https://blog.csdn.net/column/details/15023.html)









# pytorch learning rate decay

本文主要是介绍在`pytorch`中如何使用`learning rate decay`. 

先上代码:

```python
def adjust_learning_rate(optimizer, decay_rate=.9):
    for param_group in optimizer.param_groups:
        param_group['lr'] = param_group['lr'] * decay_rate
```

**什么是`param_groups`?**
`optimizer`通过`param_group`来管理参数组.`param_group`中保存了参数组及其对应的学习率,动量等等.所以我们可以通过更改`param_group['lr']`的值来更改对应参数组的学习率.

```python
# 有两个`param_group`即,len(optim.param_groups)==2
optim.SGD([
                {'params': model.base.parameters()},
                {'params': model.classifier.parameters(), 'lr': 1e-3}
            ], lr=1e-2, momentum=0.9)

#一个参数组
optim.SGD(model.parameters(), lr=1e-2, momentum=.9)
```

## 参考资料

[https://discuss.pytorch.org/t/adaptive-learning-rate/320/4](https://discuss.pytorch.org/t/adaptive-learning-rate/320/4)



