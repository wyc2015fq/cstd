# 【Pytorch】RuntimeError: arguments are located on different GPUs - CD's Coding - CSDN博客





2017年11月07日 17:59:58[糖果天王](https://me.csdn.net/okcd00)阅读数：2628








## 0x00 前言

Pytorch里使用optimizer的时候，由于其会记录step等信息， 

有时会希望将optimizer的内容记录下来，以备之后继续使用， 

那么自然而然的会想到使用API中自带的 
`torch.save(object, path)`
`torch.load(path)`
再配合上 
`optimizer.state_dict()`
`optimizer.load_state_dict(obj)`

来实现这一需求了~
于是，大家自然而然地会自信满满敲出如下这样的语句——

```
torch.save(optimizer.state_dict(), path)
optimizer.load_state_dict(torch.load(path))
```

并收获如下的Error——

```python
RuntimeError                              Traceback (most recent call last)
<ipython-input-160-19f8d61b5e53> in <module>()
     37         optimizer.zero_grad()
     38         loss.backward()
---> 39         optimizer.step()
     40         print(model.state_dict()['linear_layer.weight'])
     41 

/usr/local/anaconda2/lib/python2.7/site-packages/torch/optim/adam.pyc in step(self, closure)
     63 
     64                 # Decay the first and second moment running average coefficient
---> 65                 exp_avg.mul_(beta1).add_(1 - beta1, grad)
     66                 exp_avg_sq.mul_(beta2).addcmul_(1 - beta2, grad, grad)
     67 

RuntimeError: arguments are located on different GPUs at /opt/conda/conda-bld/pytorch_1503966894950/work/torch/lib/THC/generated/../generic/THCTensorMathPointwise.cu:215
```

## 0x01 解决方案

二话不说上解决方案是我的习惯

```python
# Load from dict
optimizer.load_state_dict(check_point['optim'])

# Load from file
optimizer.load_state_dict(torch.load(optim_path))

# Add this
for state in optimizer.state.values():
    for k, v in state.items():
        print (type(v))
        if torch.is_tensor(v):
            state[k] = v.cuda(cuda_id)
```

## 0x02 原理解释

然后在慢慢的讲为啥子~ 

首先，这个方案是我在Issue中翻看到的： 

Thanks to [`pytorch/issues/2830`](https://github.com/pytorch/pytorch/issues/2830)
可以这么理解，举例说明，虽说你之前是放在GPU3上的，数据类型叫做 cuda.Tensor(GPU 3)， 

但是天晓得你这个GPU3是哪台机器上的GPU3哦，机器问了一下GPU3：是不是你家的啊， 

GPU3看了一眼计算完被打扫干净的战场，已经空无一物——“不是吧，我家没人啊”， 

然后就委婉的拒绝了它。
所以，我们可以对load完毕的optimizer逐个询问，只要是个tensor，我们就再把它介绍给GPU3一次~







