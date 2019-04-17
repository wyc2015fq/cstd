# Pytorch写深度学习 - qq_30006593的博客 - CSDN博客





2017年09月04日 22:18:23[lshiwjx](https://me.csdn.net/qq_30006593)阅读数：373








# 不同学习率

```
params_dict = dict(model.named_parameters())
params = []
for key, value in params_dict.items():
    if key[8:16] == 'conv_off':
        params += [{'params': [value], 'lr': 0.1 * args.lr}]
    else:
        params += [{'params': [value], 'lr': args.lr}]
```

# 数据输入

## torch.utils.data.DataLoader:用来读取数据

[cuda pinned memory](http://blog.csdn.net/dcrmg/article/details/54975432)

## ImageFolder:用来准备数据集

[lmdb方式](https://discuss.pytorch.org/t/whats-the-best-way-to-load-large-data/2977/4)

主要是getitem，init和len函数

# 模型

init和forward 

.train .eval bn,dropout等层不同效果

# Gpu

torch.cuda.is_available() 

~.cuda() 

load函数有个[map_location](https://discuss.pytorch.org/t/on-a-cpu-device-how-to-load-checkpoint-saved-on-gpu-device/349/4)属性用于解决不同gpu的问题。但是使用[cuda_visible_device](https://github.com/pytorch/pytorch/issues/2610)更好
### 多GPU训练：DataParallel

多gpu时第一块显卡会明显占用更多，似乎是存放了[模型](https://github.com/pytorch/pytorch/issues/1499) 和[模型输出](https://github.com/pytorch/pytorch/issues/1893)的问题。 

cuda后可以加device参数，async参数只有variable能加（model不能，与pinned memory机制配合使用） 

data_loader有参数pinned memory，建议使用，固定cpu内存大小，更快复制到gpu。 

所有参数，包含模型，输出，输出需放在同一个gpu上，默认为gpu0，所以gpu0的内存占用更多。 

同时，pytorch的参数更新是发生在gpu上的，所以模型，input，output还必须在gpu上。
# 流程
- for data in data_loader 
[.csv](https://en.wikipedia.org/wiki/Comma-separated_values):下图为基本使用

```
In [17]: a =pd.read_csv('Group1.csv', header=None)
In [19]: a.iloc[x,y]
```

[pandas](https://pandas.pydata.org/pandas-docs/stable/10min.html)

 - variable 

 - forward 

 - loss 
[hinged loss](http://blog.csdn.net/luo123n/article/details/48878759): 不鼓励分类器过度自信 
[criterion = torch.nn.MultiLabelSoftMarginLoss](https://github.com/mratsim/Amazon_Forest_Computer_Vision/blob/master/main_pytorch.py#L61) 处理不平衡样本,多关注少样本 

 - backward 

 - optimizer.zero_grad() and optimizer.step()





