# Non-local算法代码解析 - AI之路 - CSDN博客





2018年04月19日 22:07:17[AI之路](https://me.csdn.net/u014380165)阅读数：2660








论文：Non-local Neural Networks for Video Classification 

论文链接：[https://arxiv.org/abs/1711.07971](https://arxiv.org/abs/1711.07971)

代码链接：[https://github.com/facebookresearch/video-nonlocal-net](https://github.com/facebookresearch/video-nonlocal-net)
官方代码是基于Caffe2实现，这篇博客介绍该项目的主要代码，通过代码加深对该算法的理解。 

假设~video-nonlocal-net是从[https://github.com/facebookresearch/video-nonlocal-net](https://github.com/facebookresearch/video-nonlocal-net)拉下来的项目目录。因为代码是以video分类为例，所以网络结构上和图像分类采用的ResNet在维度上有些差异，但是整体网络结构还是和ResNet一样包含conv1到conv5_x和一些池化及全连接层，对于ResNet-50而言，conv2_x、conv3_x、conv4_x、conv5_x的block数量分别是3,4,6,3，对于ResNet-101而言则分别是3,4,23,3，这两个网络是该代码中的主要例子。

**训练启动脚本都在~video-nonlocal-net/scripts/目录下，从训练脚本可以看出non-local的操作只在conv3_x和conv4_x两个stage中引入，conv2_x和conv5_x这两个stage不包含non-local操作。而在一个stage的多个block结构中，包含non-local操作的block和不包含non-local操作的block是交替叠加的，后面会详细介绍。**

配置相关的文件目录：~video-nonlocal-net/lib/core/config.py

**文章的主要特点就是在网络结构上的修改，而网络结构的构造是在~video-nonlocal- net/lib/models/resnet_video.py该脚本中的create_model函数，所以就从这个函数开始看起。**

```bash
def create_model():
...
# resnet网络的第一个卷积层，也就是conv1
conv_blob = model.ConvNd(
        data, 'conv1', 3, 64, [1 + use_temp_convs_set[0][0] * 2, 7, 7], strides=[temp_strides_set[0][0], 2, 2],
        pads=[use_temp_convs_set[0][0], 3, 3] * 2,
        weight_init=('MSRAFill', {}),
        bias_init=('ConstantFill', {'value': 0.}), no_bias=1
    )

# BN层、Relu层和池化层
bn_blob = model.SpatialBN(
            conv_blob, 'res_conv1_bn', 64, epsilon=cfg.MODEL.BN_EPSILON,
            momentum=cfg.MODEL.BN_MOMENTUM, is_test=test_mode,
        )
relu_blob = model.Relu(bn_blob, bn_blob)
max_pool = model.MaxPool(relu_blob, 'pool1', kernels=[1, 3, 3], strides=[1, 2, 2], pads=[0, 0, 0] * 2)

# conv2_x。该stage的所有block都不引入non local操作，通过resnet_helper.py脚本中
# 的res_stage_nonlocal函数来实现。group默认配置为1，也就是常规的卷积操作。
blob_in, dim_in = resnet_helper.res_stage_nonlocal(
            model, res_block, max_pool, 64, 256, stride=1, num_blocks=n1,
            prefix='res2', dim_inner=dim_inner, group=group,
            use_temp_convs=use_temp_convs_set[1], temp_strides=temp_strides_set[1])

blob_in = model.MaxPool(blob_in, 'pool2', kernels=[2, 1, 1], strides=[2, 1, 1], pads=[0, 0, 0] * 2)

# conv3_x。该stage有non local操作。几个重要的参数：nonloca_mod对于ResNet-50和
# ResNet-101而言都是2，该参数决定了该stage中哪些block要引入non local操作。
blob_in, dim_in = resnet_helper.res_stage_nonlocal(
                model, res_block, blob_in, dim_in, 512, stride=2, num_blocks=n2,
                prefix='res3', dim_inner=dim_inner * 2, group=group,
                use_temp_convs=use_temp_convs_set[2], temp_strides=temp_strides_set[2],
                batch_size=batch_size, nonlocal_name='nonlocal_conv3', nonlocal_mod=layer_mod)

# conv4_x。该stage有non local操作。几个重要的参数：nonloca_mod对于ResNet-50
# 而言是2，对于ResNet-101而言是7，这个主要是因为ResNet-50的conv4_x有6个block，
# 而ResNet-101的conv4_x有23个lock。
blob_in, dim_in = resnet_helper.res_stage_nonlocal(
            model, res_block, blob_in, dim_in, 1024, stride=2, num_blocks=n3,
            prefix='res4', dim_inner=dim_inner * 4, group=group,
            use_temp_convs=use_temp_convs_set[3], temp_strides=temp_strides_set[3],
            batch_size=batch_size, nonlocal_name='nonlocal_conv4', nonlocal_mod=layer_mod)

# conv5_x。该stage的所有block都不引入non local操作。
blob_in, dim_in = resnet_helper.res_stage_nonlocal(
            model, res_block, blob_in, dim_in, 2048, stride=2, num_blocks=n4,
            prefix='res5', dim_inner=dim_inner * 8, group=group,
            use_temp_convs=use_temp_convs_set[4], temp_strides=temp_strides_set[4])

# 最后的池化层
blob_out = model.AveragePool(blob_in, 'pool5', kernels=[pool_stride, 7, 7], strides=[1, 1, 1], pads=[0, 0, 0] * 2)

# 最后的全连接层
blob_out = model.FC(
            blob_out, 'pred', dim_in, cfg.MODEL.NUM_CLASSES,
            weight_init=('GaussianFill', {'std': cfg.MODEL.FC_INIT_STD}),
            bias_init=('ConstantFill', {'value': 0.})
        )

# 最后的softmax损失函数层
softmax, loss = model.SoftmaxWithLoss( [blob_out, labels], ['softmax', 'loss'], scale=scale)
```

以上就是宏观上整个网络的结构，重点当然在于4个stage结构的构造。可以看出4个stage都是通过resnet_helper.py脚本中的res_stage_nonlocal函数来构造，只不过当要引入non local操作时需要传入non local相关的参数。因此resnet_helper.py脚本中的**res_stage_nonlocal函数涉及non local操作的细节内容**，接下来看看。

脚本所在目录：~video-nonlocal- net/lib/models/resnet_helper.py

```python
def res_stage_nonlocal():
...
# 该函数主要代码如下。这个大的for循环的一个循环就是构建一个block。
# 主要包含两个部分，一个是_generic_residual_block_3d函数，
# 该函数就是一个常规的block结构构造代码，一个block主要包含3个卷积层和
# 一个residual connection，不再细讲。而non local操作是通过后面
# 的if条件的内容来实现的，可以看这个条件if idx % nonlocal_mod == nonlocal_mod - 1
# 语句，对于ResNet-50的conv3_x和conv4_x而言，nonlocal_mod都是2，
# 因此当num_blocks==4时（例如ResNet-50的conv3_x），
# 那么idx==1或3时就会执行non local操作，而且non local操作得到的输出blob_in就会
# 覆盖常规block构造得到的blob_in。因此效果上就是每隔一个block就会采用non local方
# 式进行该block的构造。强调下add_nonlocal函数的最后一个参数是int(dim_in / 2)，
# 也就是输入channel数量的一半，和Figure2中channel数量关系是相等的。
for idx in range(num_blocks):
        block_prefix = "{}_{}".format(prefix, idx)
        block_stride = 2 if (idx == 0 and stride == 2) else 1
        blob_in = _generic_residual_block_3d(
            model, blob_in, dim_in, dim_out, block_stride, block_prefix,
            dim_inner, group, use_temp_convs[idx], temp_strides[idx])
        dim_in = dim_out

        if idx % nonlocal_mod == nonlocal_mod - 1:
            blob_in = nonlocal_helper.add_nonlocal(
                model, blob_in, dim_in, dim_in, batch_size,
                nonlocal_name + '_{}'.format(idx), int(dim_in / 2))


return blob_in, dim_in
```

现在我们知道了non local操作会在哪个stage的哪些block结构中引入，但是non local的具体操作还未涉及，接下来就要揭开庐山真面目了。通过上一段代码可以看出non local的具体操作是通过nonlocal_helper.py脚本的add_nonlocal函数来实现。

脚本所在目录：~video-nonlocal- net/lib/models/nonlocal_helper.py 
**spacetime_nonlocal函数代码如下，这部分是论文的核心。**

```
def spacetime_nonlocal()
...
# 对应论文Figure2中的θ操作，是用1*1*1卷积实现的。
# 这里卷积核数量dim_inner是输入feature map数量dim_in的一半。
theta = model.ConvNd(
        cur, prefix + '_theta',
        dim_in,
        dim_inner,
        [1, 1, 1],
        strides=[1, 1, 1],
        pads=[0, 0, 0] * 2,
        weight_init=('GaussianFill', {'std': cfg.NONLOCAL.CONV_INIT_STD}),
        bias_init=('ConstantFill', {'value': 0.}), no_bias=cfg.NONLOCAL.NO_BIAS)

# 如果配置中有池化操作，那么实现的效果类似：(8, 1024, 4, 14, 14) => (8, 1024, 4, 7, 7)，
# 这部分也就是文中说的采取pooling方式实现subsampling，从而降低计算量。
if cfg.NONLOCAL.USE_MAXPOOL is True:
        max_pool = model.MaxPool(
            cur, prefix + '_pool',
            kernels=[1, max_pool_stride, max_pool_stride],
            strides=[1, max_pool_stride, max_pool_stride],
            pads=[0, 0, 0] * 2,
        )
    else:
        max_pool = cur

# 对应论文Figure2中的φ操作，是用1*1*1卷积实现的。
phi = model.ConvNd(
        max_pool, prefix + '_phi',
        dim_in,
        dim_inner,
        [1, 1, 1],
        strides=[1, 1, 1],
        pads=[0, 0, 0] * 2,
        weight_init=('GaussianFill', {'std': cfg.NONLOCAL.CONV_INIT_STD}),
        bias_init=('ConstantFill', {'value': 0.}), no_bias=cfg.NONLOCAL.NO_BIAS)


# 对应论文Figure2中的g操作，是用1*1*1卷积实现的。
g = model.ConvNd(
        max_pool, prefix + '_g',
        dim_in,
        dim_inner,
        [1, 1, 1],
        strides=[1, 1, 1],
        pads=[0, 0, 0] * 2,
        weight_init=('GaussianFill', {'std': cfg.NONLOCAL.CONV_INIT_STD}),
        bias_init=('ConstantFill', {'value': 0.}), no_bias=cfg.NONLOCAL.NO_BIAS)

# reshape操作是将T*H*W三个维度合并成THW一个维度，这是因为接下来要做矩阵乘法。
theta, theta_shape_5d = model.Reshape(
        theta, [theta + '_re' if not cfg.MODEL.ALLOW_INPLACE_RESHAPE else theta,
            theta + '_shape5d'],
        shape=(batch_size, dim_inner, -1))
phi, phi_shape_5d = model.Reshape(
        phi, [phi + '_re' if not cfg.MODEL.ALLOW_INPLACE_RESHAPE else phi,
            phi + '_shape5d'],
        shape=(batch_size, dim_inner, -1))
g, g_shape_5d = model.Reshape(
        g, [g + '_re' if not cfg.MODEL.ALLOW_INPLACE_RESHAPE else g,
            g + '_shape5d'],
        shape=(batch_size, dim_inner, -1))

# θ和φ的输出的矩阵乘法，维度变化例子：(8, 1024, 784) * (8, 1024, 784) => (8, 784, 784)
theta_phi = model.net.BatchMatMul([theta, phi], prefix + '_affinity', trans_a=1)

# 是否采用softmax，对应论文中的Gaussian和Embedded Gaussian两种f()函数
if cfg.NONLOCAL.USE_SOFTMAX is True:
        if cfg.NONLOCAL.USE_SCALE is True:
            theta_phi_sc = model.Scale(theta_phi, theta_phi, scale=dim_inner**-.5)
        else:
            theta_phi_sc = theta_phi
        # softmax
        # sum(p[i, j, :]) == 1, for any i, j
        p = model.Softmax(theta_phi_sc, theta_phi + '_prob', engine='CUDNN', axis=2)
else:
        ones = model.net.ConstantFill([theta_phi], [theta_phi + '_ones'], value=1.)
        ones = model.net.ReduceBackSum([ones], [theta_phi + '_const'])

        zeros = model.net.ConstantFill([theta_phi], [theta_phi + '_zeros'], value=0.)
        denom = model.net.Add(
            [zeros, ones], [theta_phi + '_denom'], broadcast=1, axis=0)

        model.StopGradient(denom, denom)
        p = model.net.Div([theta_phi, denom], [theta_phi + '_sc'])

# 矩阵相乘，这一步得到的就是论文中公式1对应的y。
t = model.net.BatchMatMul([g, p], prefix + '_y', trans_b=1)

# 将THW维度reshape回T*H*W，否则不能进行后面的卷积操作。
t_re, t_shape = model.Reshape(
        [t, theta_shape_5d],
        [t + '_re' if not cfg.MODEL.ALLOW_INPLACE_RESHAPE else t,
            t + '_shape3d'])

# 对应论文中Figure2右上角的1*1*1卷积，也就是论文中公式6的W*y。
blob_out = t_re
blob_out = model.ConvNd(
        blob_out, prefix + '_out',
        dim_inner,
        dim_out,
        [1, 1, 1],
        strides=[1, 1, 1],
        pads=[0, 0, 0] * 2,
        weight_init=('GaussianFill', {'std': cfg.NONLOCAL.CONV_INIT_STD})
        if not cfg.NONLOCAL.USE_ZERO_INIT_CONV else
        ('ConstantFill', {'value': 0.}),
        bias_init=('ConstantFill', {'value': 0.}), no_bias=cfg.NONLOCAL.NO_BIAS)

# 最后如果配置文件中有配置BN层，则再加上一个BN层
blob_out = model.SpatialBN(
            blob_out, prefix + "_bn", dim_out,
            epsilon=cfg.NONLOCAL.BN_EPSILON, momentum=cfg.NONLOCAL.BN_MOMENTUM,
            is_test=is_test
        )
```

至此，网络结构构造代码就介绍完了。




