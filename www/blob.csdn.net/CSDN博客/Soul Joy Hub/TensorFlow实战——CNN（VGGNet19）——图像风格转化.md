# TensorFlow实战——CNN（VGGNet19）——图像风格转化 - Soul Joy Hub - CSDN博客

2017年06月25日 20:27:15[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：17587


[http://blog.csdn.net/u011239443/article/details/73721903](http://blog.csdn.net/u011239443/article/details/73721903)

这次我们要做一件比较有趣的事——讲图像风格转化。

如何将一张杭州西湖图片：

![这里写图片描述](https://img-blog.csdn.net/20170625180947265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

将其风格转化为和梵高的《星夜》一样具有鲜明艺术的风格呢？

![这里写图片描述](https://img-blog.csdn.net/20170625181219311?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

先给出完整的代码：[https://github.com/xiaoyesoso/TensorFlowinAction/blob/master/InActionB2/chapter4/tranImage.py](https://github.com/xiaoyesoso/TensorFlowinAction/blob/master/InActionB2/chapter4/tranImage.py)

接下来我们来讲解它。

# 图像数据

```python
if __name__ == '__main__':
    style = Image.open('star.jpg')
    style = np.array(style).astype(np.float32) - 128.0
    content = Image.open('me.jpg')
    content = np.array(content).astype(np.float32) - 128.0
stylize(style,content,0.5,500)
```

上述`style`读取的是《星夜》，将其转为浮点数组，并且减去128.0，这样就以0为中心，可以加快收敛。`content`读取的是《西湖》，操作相同。

要注意的是，`style`，`content`图片大小要相同。我在之前用“美图秀秀”将《星夜》和《西湖》的大小转为来$224×224$。

```python
print(content.shape)
    print(style.shape)
```

可见：

```
(224, 224, 3)
(224, 224, 3)
```

# 风格转化

```python
def stylize(style_image,content_image,learning_rate=0.1,epochs=500):
    # 结果图片
    target = tf.Variable(tf.random_normal(content_image.shape),dtype=tf.float32)
    style_input = tf.constant(style_image,dtype=tf.float32)
    content_input = tf.constant(content_image, dtype=tf.float32)
    cost = loss_function(style_input,content_input,target)
    train_op = tf.train.AdamOptimizer(learning_rate).minimize(cost)
    with tf.Session(config=tf.ConfigProto(log_device_placement=True)) as sess:
        tf.initialize_all_variables().run()
        for i in range(epochs):
            _,loss,target_image = sess.run([train_op,cost,target])
            print("iter:%d,loss:%.9f" % (i, loss))
            if (i+1) % 100 == 0:
                image = np.clip(target_image + 128,0,255).astype(np.uint8)
                Image.fromarray(image).save("./neural_me_%d.jpg" % (i + 1))
```

这里比较有趣的是，以前我们通常调优的是参数矩阵，然后配合测试数据预测出结果。而这边，我们没有测试数据，而所要调优的`target`矩阵，就是我们要得结果。

# VGGNet19模型

VGGNet模型：

![这里写图片描述](https://img-blog.csdn.net/20170625184235817?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

要注意的是，在这里我们并没有想试图训练`VGGNet19`模型，我们只是想获得图片在`VGGNet19`模型中的某些隐藏层上的特征矩阵。所以我们这里使用已经训练好的`VGGNet19`模型的参数，下载地址：[http://www.vlfeat.org/matconvnet/models/beta16/imagenet-vgg-verydeep-19.mat](http://www.vlfeat.org/matconvnet/models/beta16/imagenet-vgg-verydeep-19.mat)

然后只需要加载它据可以了：

```python
_vgg_params = None

def vgg_params():
    global _vgg_params
    if _vgg_params is None:
        _vgg_params = sio.loadmat('imagenet-vgg-verydeep-19.mat')
    return _vgg_params
```

```python
def vgg19(input_image):
    layers = (
        'conv1_1', 'relu1_1', 'conv1_2', 'relu1_2', 'pool1',
        'conv2_1', 'relu2_1', 'conv2_2', 'relu2_2', 'pool2',
        'conv3_1', 'relu3_1', 'conv3_2', 'relu3_2', 'conv3_3', 'relu3_3', 'conv3_4', 'relu3_4','pool3',
        'conv4_1', 'relu4_1', 'conv4_2', 'relu4_2', 'conv4_3', 'relu4_3', 'conv4_4', 'relu4_4', 'pool4',
        'conv5_1', 'relu5_1', 'conv5_2', 'relu5_2', 'conv5_3', 'relu5_3', 'conv5_4', 'relu5_4', 'pool5'
    )

    weights = vgg_params()['layers'][0]
    net = input_image
    network = {}
    for i,name in enumerate(layers):
        layer_type = name[:4]
        # 若是卷积层
        if layer_type == 'conv':
            kernels,bias = weights[i][0][0][0][0]
            # 由于 imagenet-vgg-verydeep-19.mat 中的参数矩阵和我们定义的长宽位置颠倒了，所以需要交换
            kernels = np.transpose(kernels,(1,0,2,3))
            conv = tf.nn.conv2d(net,tf.constant(kernels),strides=(1,1,1,1),padding='SAME',name=name)
            net = tf.nn.bias_add(conv,bias.reshape(-1))
            net = tf.nn.relu(net)
        # 若是池化层
        elif layer_type == 'pool':
            net = tf.nn.max_pool(net,ksize=(1,2,2,1),strides=(1,2,2,1),padding='SAME')
        # 将隐藏层加入到集合中
        # 若为`激活函数`直接加入集合
        network[name] = net

    return network
```

我们将`weights`打印出来看看：

```python
print(weights.shape)
```

得到：

```
(43,)
```

可知`imagenet-vgg-verydeep-19.mat`还可以支持比`VGGNet19`更多层的`VGGNet`模型。

若是卷积层，如`conv1_1`：

```python
print(weights[0][0][0][0][0])
```

得到的是参数矩阵和偏置：

```
[ array([[[[ 0.39416704, -0.08419707, -0.03631314, ..., -0.10720515,
          -0.03804016,  0.04690642],
         [ 0.46418372,  0.03355668,  0.10245045, ..., -0.06945956,
          -0.04020201,  0.04048637],
         [ 0.34119523,  0.09563112,  0.0177449 , ..., -0.11436455,
          -0.05099866, -0.00299793]],

        [[ 0.37740308, -0.07876257, -0.04775979, ..., -0.11827433,
          -0.19008617, -0.01889699],
         [ 0.41810837,  0.05260524,  0.09755926, ..., -0.09385028,
          -0.20492788, -0.0573062 ],
         [ 0.33999205,  0.13363543,  0.02129423, ..., -0.13025227,
          -0.16508926, -0.06969624]],

        [[-0.04594866, -0.11583115, -0.14462094, ..., -0.12290562,
          -0.35782176, -0.27979308],
         [-0.04806903, -0.00658076, -0.02234544, ..., -0.0878844 ,
          -0.3915486 , -0.34632796],
         [-0.04484424,  0.06471398, -0.07631404, ..., -0.12629718,
          -0.29905206, -0.28253639]]],

       [[[ 0.2671299 , -0.07969447,  0.05988706, ..., -0.09225675,
           0.31764674,  0.42209673],
         [ 0.30511212,  0.05677647,  0.21688674, ..., -0.06828708,
           0.3440761 ,  0.44033417],
         [ 0.23215917,  0.13365699,  0.12134422, ..., -0.1063385 ,
           0.28406844,  0.35949969]],

        [[ 0.09986369, -0.06240906,  0.07442063, ..., -0.02214639,
           0.25912452,  0.42349899],
         [ 0.10385381,  0.08851637,  0.2392226 , ..., -0.01210995,
           0.27064082,  0.40848857],
         [ 0.08978214,  0.18505956,  0.15264879, ..., -0.04266965,
           0.25779948,  0.35873157]],

        [[-0.34100872, -0.13399366, -0.11510294, ..., -0.11911335,
          -0.23109646, -0.19202407],
         [-0.37314063, -0.00698938,  0.02153259, ..., -0.09827439,
          -0.2535741 , -0.25541356],
         [-0.30331427,  0.08002605, -0.03926321, ..., -0.12958746,
          -0.19778992, -0.21510386]]],

       [[[-0.07573577, -0.07806503, -0.03540679, ..., -0.1208065 ,
           0.20088433,  0.09790061],
         [-0.07646758,  0.03879711,  0.09974211, ..., -0.08732687,
           0.2247974 ,  0.10158388],
         [-0.07260918,  0.10084777,  0.01313597, ..., -0.12594968,
           0.14647409,  0.05009392]],

        [[-0.28034249, -0.07094654, -0.0387974 , ..., -0.08843154,
           0.18996507,  0.07766484],
         [-0.31070709,  0.06031388,  0.10412455, ..., -0.06832542,
           0.20279962,  0.05222717],
         [-0.246675  ,  0.1414054 ,  0.02605635, ..., -0.10128672,
           0.16340195,  0.02832468]],

        [[-0.41602272, -0.11491341, -0.14672887, ..., -0.13079506,
          -0.1379628 , -0.26588449],
         [-0.46453714, -0.00576723, -0.02660675, ..., -0.10017379,
          -0.15603794, -0.32566148],
         [-0.33683276,  0.06601517, -0.08144748, ..., -0.13460518,
          -0.1342358 , -0.27096185]]]], dtype=float32)
 array([[ 0.73017758,  0.06493629,  0.03428847,  0.8260386 ,  0.2578029 ,
         0.54867655, -0.01243854,  0.34789944,  0.55108708,  0.06297145,
         0.60699058,  0.26703122,  0.649414  ,  0.17073655,  0.47723091,
         0.38250586,  0.46373144,  0.21496128,  0.46911287,  0.23825859,
         0.47519219,  0.70606434,  0.27007523,  0.68552732,  0.03216552,
         0.60252881,  0.35034859,  0.446798  ,  0.77326518,  0.58191687,
         0.39083108,  1.75193536,  0.66117406,  0.30213955,  0.53059655,
         0.67737472,  0.33273223,  0.49127793,  0.26548928,  0.18805602,
         0.07412001,  1.10810876,  0.28224325,  0.86755145,  0.19422948,
         0.810332  ,  0.36062282,  0.50720042,  0.42472315,  0.49632648,
         0.15117475,  0.79454446,  0.33494323,  0.47283995,  0.41552398,
         0.08496041,  0.37947032,  0.60067391,  0.47174454,  0.81309211,
         0.45521152,  1.08920074,  0.47757268,  0.4072122 ]], dtype=float32)]
```

若是激活函数，如`relu1_1`：

```python
print(weights[1][0][0][0][0])
```

输出：

```
relu
```

若是池化层，如`pool1`：

```python
print(weights[4][0][0][0][0])
```

输出：

```
pool1
```

# 损失函数

```python
STYLE_WEIGHT = 1
CONTENT_WEIGHT = 1
STYLE_LAYERS = ['relu1_2','relu2_2','relu3_2']
CONTENT_LAYERS = ['relu1_2']

def loss_function(style_image,content_image,target_image):
    style_features = vgg19([style_image])
    content_features = vgg19([content_image])
    target_features = vgg19([target_image])
    loss = 0.0
    for layer in CONTENT_LAYERS:
        loss += CONTENT_WEIGHT * content_loss(target_features[layer],content_features[layer])

    for layer in STYLE_LAYERS:
        loss += STYLE_WEIGHT * style_loss(target_features[layer],style_features[layer])

    return loss
```

可以看到权重`STYLE_WEIGHT`和`CONTENT_WEIGHT`可以控制优化更

取趋于风格还是趋于内容。而`STYLE_LAYERS`的层数越多，就能挖掘出《星夜》越多样的风格特征，而`CONTENT_LAYERS`中的越深的掩藏层得到的特征越抽象。

建议`STYLE_LAYERS`中的层数尽可能的多，这样更加能挖掘出《星夜》风格特征。当然层数多了或者深了，所需要的迭代次数需要很大才能得到比较好的效果。由于机子不给力，我只迭代了500轮，只选了三个隐藏层`'relu1_2','relu2_2','relu3_2'`。而`CONTENT_LAYERS`中的隐藏层的越浅就表达了目标图中原内容就越具像。
- 内容损失函数很简单，就是特征值误差：

```python
def content_loss(target_features,content_features):
    _,height,width,channel = map(lambda i:i.value,content_features.get_shape())
    content_size = height * width * channel
    return tf.nn.l2_loss(target_features - content_features) / content_size
```
- 风格损失函数。我们现将三维特征矩阵`(-1,channel)`重塑为二维矩阵，即一行代表一个特征值，三列分别是RGB。使用其格拉姆矩阵（$A^TA$）误差作为返回结果。

```python
def style_loss(target_features,style_features):
    _,height,width,channel = map(lambda i:i.value,target_features.get_shape())
    size = height * width * channel
    target_features = tf.reshape(target_features,(-1,channel))
    target_gram = tf.matmul(tf.transpose(target_features),target_features) / size

    style_features = tf.reshape(style_features,(-1,channel))
    style_gram = tf.matmul(tf.transpose(style_features),style_features) / size

    return tf.nn.l2_loss(target_gram - style_gram) / size
```

# 结果图

好啦，接下来看看我们的作品吧。

迭代100轮：

![这里写图片描述](https://img-blog.csdn.net/20170625201252402?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

迭代200轮：

![这里写图片描述](https://img-blog.csdn.net/20170625201312154?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

迭代300轮：

![这里写图片描述](https://img-blog.csdn.net/20170625201336742?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

迭代400轮：

![这里写图片描述](https://img-blog.csdn.net/20170625201401446?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

迭代500轮：

![这里写图片描述](https://img-blog.csdn.net/20170625201421777?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

可见结果图的内容的原来越清晰的，但是过于清晰反而不具有艺术感，可以适当加深加多`CONTENT_LAYERS`中的隐藏层。注意图片的右上部分，也可以发现越发具有类似于《星夜》的旋转的风格纹路细节。

# 赠送一只胖子～

原：

![这里写图片描述](https://img-blog.csdn.net/20170625201552137?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

100：

![这里写图片描述](https://img-blog.csdn.net/20170625201629448?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

200：

![这里写图片描述](https://img-blog.csdn.net/20170625201706836?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

300：

![这里写图片描述](https://img-blog.csdn.net/20170625201726231?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

400：

![这里写图片描述](https://img-blog.csdn.net/20170625201745855?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

500：

![这里写图片描述](https://img-blog.csdn.net/20170625201804489?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20170625202806924?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

