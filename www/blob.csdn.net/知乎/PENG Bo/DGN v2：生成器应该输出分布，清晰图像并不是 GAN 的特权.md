# DGN v2：生成器应该输出分布，清晰图像并不是 GAN 的特权 - 知乎
# 



前几天，我们介绍了一个非常简单的“DGN”：
[PENG Bo：DGN：生成模型，可以有多简单？思考DL的理论细节 (3)​zhuanlan.zhihu.com![图标](https://pic3.zhimg.com/v2-d6037f939fdacaf8e3b5cd50acb45c3e_180x120.jpg)](https://zhuanlan.zhihu.com/p/50952156)
在本文中，我们仍然暂不加入z的训练和z的分布（就是类似VAE的那些事情），而是先实验一个新的事情：

> 让G输出图像的分布，而不是图像。例如，输出每个点的预期像素值 + 预期标准差。

换一个角度，也可以理解为：让G输出图像 + 图像每个点的置信度。**我们将发现，G的确可以很好地给出置信度。**

本文用到的计算，很简单：
![](https://pic2.zhimg.com/v2-3f81f05aceaf350ba1dabb68e4334dbd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='622' height='381'></svg>)
这里我们用 CelebA 数据集，做了简化，只包括年轻女星。比男星简单。
![](https://pic3.zhimg.com/v2-afdbbf1483f8d22bf128248daaaab53a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='512' height='512'></svg>)
核心模型是这样（这里用 log 标准差，简单点）：


```
def make_dcgan_sym(ngf, ndf, nc):
    global fix_gamma
    
    data = mx.sym.Variable('data')
       
    # G
    
    g1 = mx.sym.Deconvolution(data, name='g1', kernel=(4,4), num_filter=ngf*8, no_bias=no_bias) # 增大至4x4
    g1_bn = BatchNorm(g1, name='g1_bn')
    g1_act = PReLU(g1_bn, name='g1_act')
    
    g2 = Upsize2x_BN_Act(g1_act, name='g2', num_filter=ngf*4) # 增大至8x8
    g3 = Upsize2x_BN_Act(g2, name='g3', num_filter=ngf*2) # 增大至16x16
    g4 = Upsize2x_BN_Act(g3, name='g4', num_filter=ngf) # 增大至32x32
    
    g5 = Upsize2x(g4, name='g5', num_filter=nc) # 增大至64x64, mean
    gout = mx.sym.Activation(g5, act_type='sigmoid', name='gout')
    
    g5x = Upsize2x(g4, name='g5x', num_filter=nc) # 增大至64x64, log(variance)
    
    return mx.sym.concat(g5, g5x)
```


注意：这里的采样会溢出 [0,1]，所以这里的效果还不是最佳效果。先这样吧，以后再改。


训练过程，和梯度，是这样：


```
for i in range(len(allData)):
            modG.forward(Batch(allZ[i]), is_train=True)
            outG = modG.get_outputs()[0] # 64 * 6 * 64 * 64
            
            s = outG[:,0:3,:,:]
            t = outG[:,3:6,:,:]
            
            et = mx.nd.exp(-t)
            
            x = allData[i].as_in_context(ctx)
            ds = 2 * et * (s - x)
            dt = 1 - et * (s - x) * (s - x)
            
            modG.backward([mx.nd.concat(ds, dt)])
            modG.update()
```


注意，这就已经比 MSE 更好了，因为模型会自适应了。例如，皮肤的 variance 相对小，背景的 variance 相对大，可以自适应。

训练效果，首先样本是这样的：
![](https://pic3.zhimg.com/v2-2f4f6d99adf97374a2758d2adf817c86_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='512' height='512'></svg>)
然后这是每个点的预期值，当然是有点模糊的，我也没训练到底，就训练了几分钟：
![](https://pic1.zhimg.com/v2-9d8962aa0d198cb471a68a7f6da9c94c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='512' height='512'></svg>)
这是标准差的预期。**如果和原始图像对比，会发现确实就是误差最大（最难还原）的地方**：
![](https://pic2.zhimg.com/v2-369cf31187430bb2e851d862c1a9b485_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='512' height='512'></svg>)
按照标准差，注入噪音：
![](https://pic4.zhimg.com/v2-bf08bad0791343604278404d6a837463_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='512' height='512'></svg>)
真的清晰了一些（**发现知乎压缩图像了，所以可能看不出来**）。下一步应该干什么？你应该可以猜到了：

**降噪。根据【带噪音的图像】+【图像模型】+【之前的标准差预期】降噪。**

显然，这就可以得到又清晰，又有“细节”的图片。

讲真，现在的 GAN 的所谓“细节”，也只不过是“合理的噪音”。还不如用我这里的方法，简单，稳定。

最后，这里有一个统一的看法：
- VAE告诉我们，输入x，输出z的分布。
- 这篇告诉我们，输入z，输出x的分布。

下次的 DGN v3 加入类似 VAE 的部分。最终希望把 GAN 干掉。

当然，这些都是小打小闹，我在想的事情更深，后续继续慢慢写。


