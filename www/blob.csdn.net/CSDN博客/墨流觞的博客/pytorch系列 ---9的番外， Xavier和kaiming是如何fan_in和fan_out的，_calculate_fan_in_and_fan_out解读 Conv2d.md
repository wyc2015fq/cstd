# pytorch系列 ---9的番外， Xavier和kaiming是如何fan_in和fan_out的，_calculate_fan_in_and_fan_out解读 Conv2d - 墨流觞的博客 - CSDN博客





2018年11月12日 15:25:25[墨氲](https://me.csdn.net/dss_dssssd)阅读数：228标签：[pytorch																[weight_initialization](https://so.csdn.net/so/search/s.do?q=weight_initialization&t=blog)](https://so.csdn.net/so/search/s.do?q=pytorch&t=blog)
个人分类：[pytorch 记录](https://blog.csdn.net/dss_dssssd/article/category/8109673)

所属专栏：[python3 pytorch0.4系列教程](https://blog.csdn.net/column/details/29714.html)








本文主要借助代码讲解Xavier和kaiming是如何借助`_calculate_fan_in_and_fan_out`函数来计算当前网络层的fan_in（输入神经元个数）和fan_out（输出神经元个数的），先针对Linear和Conv2d两种。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181112153029570.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

```python
m_c = nn.Conv2d(16, 33, 3, stride=2)
m_l = nn.Linear(1, 10)
m_c.weight.size()
m_l.weight.size()
```

out:

> 
torch.Size([33, 16, 3, 3])

torch.Size([10, 1])


注意看Linear weight的维度为2，而Conv2d的维度为4.

首先判断tensor的维度，如果是二维，则是Linear，

```python
if dimensions == 2:  # Linear
        fan_in = tensor.size(1)
        fan_out = tensor.size(0)
```

此时：$$fan\_in = in\_channels $$
$$fan\_out = out\_channels $$

而如果大于2维，Conv2d.weight的第一维为`out_channels`, 第二维为`in_channels`第三维和第四维维kernal_size,

代码else，先取出前两个维度，然后tensor[0][0].numel得到的是tensor[0][0]中元素的数目，也就是:$$w\_c.weight.size(2) \times w\_c.weight.size(3)\times {\ldots}_{\rm } \times w\_c.weight.size(-1)$$,在m_c中就是$3*3=9$

再将此值乘以 `num_input_fmaps`和`num_output_fmaps`就得到`fan_in`和`fan_out`了
$$fan\_in = in\_channels * kernal\_size[0]* kernal\_size[0]$$
$$fan\_out = out\_channels * kernal\_size[0]* kernal\_size[0]$$

```python
else:
        num_input_fmaps = tensor.size(1)
        num_output_fmaps = tensor.size(0)
        receptive_field_size = 1
        if tensor.dim() > 2:
            receptive_field_size = tensor[0][0].numel()
        fan_in = num_input_fmaps * receptive_field_size
        fan_out = num_output_fmaps * receptive_field_size
```

这是测试代码

```python
m_c = nn.Conv2d(16, 33, 3, stride=2)

m_l = nn.Linear(1, 10)

m_c.weight.size()
Out[30]: torch.Size([33, 16, 3, 3])

m_l.weight.size()
Out[31]: torch.Size([10, 1])

m_c.weight[0][0]
Out[32]: 
tensor([[-0.0667,  0.0241,  0.0701],
        [-0.0209,  0.0364,  0.0826],
        [ 0.0803, -0.0535,  0.0316]], grad_fn=<SelectBackward>)

m_c.weight[0][0].numel()
Out[33]: 9
```





