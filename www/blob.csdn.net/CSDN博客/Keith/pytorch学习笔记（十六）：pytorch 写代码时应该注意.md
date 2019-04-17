# pytorch学习笔记（十六）：pytorch 写代码时应该注意 - Keith - CSDN博客





2017年11月24日 13:14:08[ke1th](https://me.csdn.net/u012436149)阅读数：1589
所属专栏：[pytorch学习笔记](https://blog.csdn.net/column/details/15023.html)








- 当网络中有 `dropout，bn` 的时候。训练的要记得 `net.train()`, 测试 要记得 `net.eval()`
- 在测试的时候 创建输入 `Variable` 的时候 要记得 `volatile=True`
- `torch.sum(Tensor), torch.mean(Tensor)` 返回的是 `python` 浮点数，不是 `Tensor`。
- 在不需要 `bp` 的地方用 `Tensor` 运算。



