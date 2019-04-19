# 【深度学习】caffe 中的一些参数介绍 - 数据之美的博客 - CSDN博客
2017年03月04日 21:53:42[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：129
个人分类：[caffe&tensorflow&keras																[深度学习/机器学习](https://blog.csdn.net/oppo62258801/article/category/6739087)](https://blog.csdn.net/oppo62258801/article/category/6739085)
# caffe 参数介绍
# solver.prototxt
```
net: "models/bvlc_alexnet/train_val.prototxt" 
test_iter: 1000       # 
test_interval: 1000   # 
base_lr: 0.01         # 开始的学习率
lr_policy: "step"     # 学习率的drop是以gamma在每一次迭代中
gamma: 0.1
stepsize: 100000      # 每stepsize的迭代降低学习率：乘以gamma
display: 20           # 没display次打印显示loss
max_iter: 450000      # train 最大迭代max_iter 
momentum: 0.9         #
weight_decay: 0.0005  #
snapshot: 10000       # 没迭代snapshot次，保存一次快照
snapshot_prefix:   "models/bvlc_reference_caffenet/caffenet_train"
solver_mode: GPU      # 使用的模式是GPU
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 
test_iter
在[测试](http://lib.csdn.net/base/softwaretest)的时候，需要迭代的次数，即test_iter* batchsize（测试集的）=测试集的大小，测试集的 batchsize可以在prototx文件里设置。
- 
test_interval
训练的时候，每迭代test_interval次就进行一次测试。
- 
momentum
灵感来自于牛顿第一定律，基本思路是为寻优加入了“惯性”的影响，这样一来，当误差曲面中存在平坦区的时候，SGD可以更快的速度学习。 
# train_val.prototxt
```bash
layer { # 数据层
    name: "data"
    type: "Data"
    top: "data"
    top: "label"
    include {
        phase: TRAIN # 表明这是在训练阶段才包括进去
    }
    transform_param { # 对数据进行预处理
        mirror: true # 是否做镜像
        crop_size: 227
        # 减去均值文件
        mean_file: "data/ilsvrc12/imagenet_mean.binaryproto"
    }
    data_param { # 设定数据的来源
        source: "examples/imagenet/ilsvrc12_train_lmdb"
        batch_size: 256
        backend: LMDB
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
```
layer {
    name: "data"
    type: "Data"
    top: "data"
    top: "label"
    include {
        phase: TEST # 测试阶段
    }
    transform_param {
        mirror: false # 是否做镜像
        crop_size: 227
        # 减去均值文件
        mean_file: "data/ilsvrc12/imagenet_mean.binaryproto"
    }
    data_param {
        source: "examples/imagenet/ilsvrc12_val_lmdb"
        batch_size: 50
        backend: LMDB
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 
lr_mult
学习率，但是最终的学习率需要乘以 solver.prototxt 配置文件中的 base_lr .
> 
如果有两个 lr_mult, 则第一个表示 weight 的学习率，第二个表示 bias 的学习率 
一般 bias 的学习率是 weight 学习率的2倍’
- 
decay_mult
权值衰减，为了避免模型的over-fitting，需要对cost function加入规范项。 
- 
num_output
卷积核（filter）的个数
- 
kernel_size
卷积核的大小。
> 
如果卷积核的长和宽不等，需要用 kernel_h 和 kernel_w 分别设定
- 
stride
卷积核的步长，默认为1。也可以用stride_h和stride_w来设置。
- 
pad
扩充边缘，默认为0，不扩充。
> 
扩充的时候是左右、上下对称的，比如卷积核的大小为5*5，那么pad设置为2，则四个边缘都扩充2个像素，即宽度和高度都扩充了4个像素，这样卷积运算之后的特征图就不会变小。 
也可以通过pad_h和pad_w来分别设定。
- 
weight_filler
权值初始化。 默认为“constant”,值全为0. 
很多时候我们用”xavier”[算法](http://lib.csdn.net/base/datastructure)来进行初始化，也可以设置为”gaussian”
```
weight_filler {
    type: "gaussian"
    std: 0.01
}
```
- 1
- 2
- 3
- 4
- 1
- 2
- 3
- 4
- bias_filler
偏置项的初始化。一般设置为”constant”, 值全为0。
```
bias_filler {
    type: "constant"
    value: 0
}
```
- 1
- 2
- 3
- 4
- 1
- 2
- 3
- 4
- 
bias_term
是否开启偏置项，默认为true, 开启
- 
group
分组，默认为1组。如果大于1，我们限制卷积的连接操作在一个子集内。 
卷积分组可以减少网络的参数，至于是否还有其他的作用就不清楚了。
> 
每个input是需要和每一个kernel都进行连接的，但是由于分组的原因其只是与部分的kernel进行连接的 
如: 我们根据图像的通道来分组，那么第i个输出分组只能与第i个输入分组进行连接。
- 
pool
池化方法，默认为MAX。目前可用的方法有 MAX, AVE, 或 [STOCHASTIC](http://www.cnblogs.com/tornadomeet/p/3432093.html)
- 
dropout_ratio
丢弃数据的概率
