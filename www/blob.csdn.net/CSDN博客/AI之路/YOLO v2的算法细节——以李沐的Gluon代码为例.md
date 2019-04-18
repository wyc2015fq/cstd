# YOLO v2的算法细节——以李沐的Gluon代码为例 - AI之路 - CSDN博客





2018年02月25日 11:02:26[AI之路](https://me.csdn.net/u014380165)阅读数：5509








YOLO算法在object detection领域算是比较有意思的一个分支，2017年CVPR上的YOLO v2对原来的YOLO算法进行了升级，论文本身包含较多的算法细节，可以先参考博客：[YOLO9000算法详解](http://blog.csdn.net/u014380165/article/details/77961414)，这里借助李沐的深度学习公开课中的代码（通过MXNet框架下的Gluon接口实现）来详细了解YOLO v2算法的细节。 

参考链接：[https://zh.gluon.ai/chapter_computer-vision/yolo.html](https://zh.gluon.ai/chapter_computer-vision/yolo.html)

**在参考链接中完整地给出了实现YOLO v2算法的代码，主要包括数据读取、模型加载、训练模型、测试模型四个部分，最重要的是模型加载部分的YOLO2Output类、训练模型部分的yolo2_forward函数和yolo2_target函数。接下来按这四个部分依次介绍。**

**1、数据读取部分**的实现代码主要在get_iteratirs函数中，在该函数中主要采用mxnet.image.ImageDetIter接口来读取，需要提前准备好train.rec和val.rec文件，class_names就是object名称的列表。另外这里定义了输入图像的大小是3*256*256，后面的代码都基于这个定义。**两个参数的含义：1、min_object_covered** (float, default=0.1) – The cropped area of the image must contain at least this fraction of any bounding box supplied. The value of this parameter should be non-negative. In the case of 0, the cropped area does not need to overlap any of the bounding boxes supplied. **2、max_attempts** (int, default=50) – Number of attempts at generating a cropped/padded region of the image of the specified constraints. After max_attempts failures, return the original image.

```python
from mxnet import image
from mxnet import nd

data_shape = 256
batch_size = 32
rgb_mean = nd.array([123, 117, 104])
rgb_std = nd.array([58.395, 57.12, 57.375])

def get_iterators(data_shape, batch_size):
    class_names = ['pikachu', 'dummy']
    num_class = len(class_names)
    train_iter = image.ImageDetIter(
        batch_size=batch_size,
        data_shape=(3, data_shape, data_shape),
        path_imgrec=data_dir+'train.rec',
        path_imgidx=data_dir+'train.idx',
        shuffle=True,
        mean=True,
        std=True,
        rand_crop=1,
        min_object_covered=0.95,
        max_attempts=200)
    val_iter = image.ImageDetIter(
        batch_size=batch_size,
        data_shape=(3, data_shape, data_shape),
        path_imgrec=data_dir+'val.rec',
        shuffle=False,
        mean=True,
        std=True)
    return train_iter, val_iter, class_names, num_class

train_data, test_data, class_names, num_class = get_iterators(
    data_shape, batch_size)
```

**2、模型加载部分**，先通过mxnet.gluon.model_zoo.vision.get_model接口导入模型，这个用法和PyTorch很像。在导入的时候注意到最后的.features，这个features是ResNetV1类的初始化函数中的变量，通过mxnet.gluon.nn.HybridSequential接口初始化，mxnet.gluon.nn.HybridSequential是mxnet.gluon.nn.Sequential的特例，mxnet.gluon.nn.Sequential将添加进来的层按先后顺序执行。这里通过HybridSequential类的add方法添加层（更抽象点就是层或者网络都是通过block实现的），最后包含网络除最后的全连接层以外的所有层。也就是说net存放pretrained这个网络中除了最后两层的网络结构，是用来构造主网络的，所以net是包含7*7卷积加3个block（pretrained中包含7*7卷积加4个block加pooling层），net最后一层输出feature map大小就变成256/16=16。scales变量用来存放anchor的尺寸信息，是一个二维列表，每一行表示一个anchor，第一列表示width，第二列表示height。这个scales里面的值根据主网络最后一层的输出feature map大小来定，比如这个net最后一层输出是16*16，那么这里的scale里面的值是3或者9这样大小是比较正常的。**YOLO2Output这个类用来构造预测层，最后的net.add(predictor)就完成了主网络和预测层的连接，后面会详细介绍YOLO2Output类**。predictor.initialize()是调用了mxnet.gluon.Block类（YOLO2Output类是基于HybridBlock类实现的，HybridBlock的底层是通过Block基类实现的）的initialize方法，是用来初始化网络参数的，这一步是必须的，否则构造的网络结构没有参数就跑前向会报错。initialize方法的两个主要参数是初始化方式（一般默认即可）和ctx（也就是指定的cpu或gpu，比如ctx=[mx.gpu(0),mx.gpu(1)]）。另外initialize方法和block.collect_params().initialize()结果一样，block类的collect_params()方法是block中比较常用的，返回的是block及其children的参数，官网中有个关于collect_params()的例子：假如你要用dense0层的参数初始化dense1层，可以这样实现：dense0 = nn.Dense(20)；dense1 = nn.Dense(20, params=dense0.collect_params())。至于net为什么没有运行initialize方法，是因为在得到pretrained的时候设置了pretrained=True，也就是用了预训练模型进行参数初始化了。

```
from mxnet.gluon.model_zoo import vision
pretrained = vision.get_model('resnet18_v1', pretrained=True).features
net = nn.HybridSequential()
for i in range(len(pretrained) - 2):
    net.add(pretrained[i])

# anchor scales, try adjust it yourself
scales = [[3.3004, 3.59034],
          [9.84923, 8.23783]]

# use 2 classes, 1 as dummy class, otherwise softmax won't work
predictor = YOLO2Output(2, scales)
predictor.initialize()
net.add(predictor)
```

**YOLO2Output类用来构造预测层，代码如下**。几个assert语句用来确保输入数据的格式符合要求，比较**重要的一个是out_channels = len(anchor_scales) * (num_class + 1 + 4)，首先len(anchor_scales)表示anchor的数量， (num_class + 1 + 4)中的num_class标object的数量，1表示score，4表示框的中心点坐标和宽高信息**。self.output = nn.Conv2D(out_channels, 1, 1)这一行点明了用1*1的卷积来完成预测层。

```python
class YOLO2Output(HybridBlock):
    def __init__(self, num_class, anchor_scales, **kwargs):
        super(YOLO2Output, self).__init__(**kwargs)
        assert num_class > 0, "number of classes should > 0, given {}".format(num_class)
        self._num_class = num_class
        assert isinstance(anchor_scales, (list, tuple)), "list or tuple of anchor scales required"
        assert len(anchor_scales) > 0, "at least one anchor scale required"
        for anchor in anchor_scales:
            assert len(anchor) == 2, "expected each anchor scale to be (width, height), provided {}".format(anchor)
        self._anchor_scales = anchor_scales
        out_channels = len(anchor_scales) * (num_class + 1 + 4)
        with self.name_scope():
            self.output = nn.Conv2D(out_channels, 1, 1)

    def hybrid_forward(self, F, x, *args):
        return self.output(x)
```

**3、训练代码部分**。首先通过gluon.Trainer接口初始化一个训练器，然后是训练的循环（例子中是20个epoch），循环的一开始先将几个损失函数的值重置。 for i, batch in enumerate(train_data)每次循环都读取一个batch的数据。x = net(x)是数据从输入到输出预测结果，比如当anchor数量为2、输入图像大小为256*256，batch size为32时，该行代码的输入是32*3*256*256，输出是32*14*16*16，其中14是2*(2+1+4)，括号中的三个值分别表示类别数、score和坐标信息。output, cls_pred, score, xywh = yolo2_forward(x, 2, scales)一行调用**yolo2_forward函数**将net输出结果进行处理，后面会详细介绍这个函数。tid, tscore, tbox, sample_weight = yolo2_target(score, xywh, y, scales, thresh=0.5)一行是调用**yolo2_target函数**得到模型训练目标相关的信息，后面会详细介绍该函数。 loss1 = sce_loss(cls_pred, tid, sample_weight * class_weight)一行是计算分类的损失，输入的cls_pred表示每个box的每个类别的预测概率，tid表示和真实框的IOU最大的box的标签，sample_weight是只有和真实框的IOU最大的box为1，其余为0，**可以看出在YOLO算法中每个object都是由这个object的中心所在的grid cell中的一个box来预测的。score_weight是计算正负样本在回传损失时候的权重**，这里nd.where函数的第一个输入矩阵的一些位置的数值满足不等式，那么对应位置的数值就用第二个输入来替代，相反就用第三个输入来替代，**最后得到的这个score_weight，除了和真实框的IOU最大的box的权重是positive_weight以外，其他都是negative_weight**。 loss2 = l1_loss(score, tscore, score_weight)一行是计算score的损失，这个score也就是我们常见的显示在框上的置信度，范围是0到1的小数。loss3 = l1_loss(xywh, tbox, sample_weight * box_weight)一行是计算box回归的损失，**这里sample_weight*box_weight也是只回传和真实框的IOU最大的box的损失，而乘以box_weight是为了增加loss3在中loss中的权重**。最后回传的loss是这3个loss的和。trainer.step(batch_size)是更新网络参数，之所以要输入batch_size，是因为梯度要归一化成1/batch_size。cls_loss.update(loss1)是更新loss数值，obj_loss和box_loss同理，这里的3个值都只是为了打印在显示界面上，和回传的损失没关系。整体流程是这样的，接下来详细介绍各函数。

```
from mxnet import init
from mxnet import gpu

positive_weight = 5.0
negative_weight = 0.1
class_weight = 1.0
box_weight = 5.0

ctx = gpu(0)
net.collect_params().reset_ctx(ctx)
trainer = gluon.Trainer(net.collect_params(), 'sgd', {'learning_rate': 1, 'wd': 5e-4})



import time
from mxnet import autograd
for epoch in range(20):
    # reset data iterators and metrics
    train_data.reset()
    cls_loss.reset()
    obj_loss.reset()
    box_loss.reset()
    tic = time.time()
    for i, batch in enumerate(train_data):
        x = batch.data[0].as_in_context(ctx)
        y = batch.label[0].as_in_context(ctx)
        with autograd.record():
            x = net(x)
            output, cls_pred, score, xywh = yolo2_forward(x, 2, scales)
            with autograd.pause():
                tid, tscore, tbox, sample_weight = yolo2_target(score, xywh, y, scales, thresh=0.5)
            # losses
            loss1 = sce_loss(cls_pred, tid, sample_weight * class_weight)
            score_weight = nd.where(sample_weight > 0,
                                    nd.ones_like(sample_weight) * positive_weight,
                                    nd.ones_like(sample_weight) * negative_weight)
            loss2 = l1_loss(score, tscore, score_weight)
            loss3 = l1_loss(xywh, tbox, sample_weight * box_weight)
            loss = loss1 + loss2 + loss3
        loss.backward()
        trainer.step(batch_size)
        # update metrics
        cls_loss.update(loss1)
        obj_loss.update(loss2)
        box_loss.update(loss3)

    print('Epoch %2d, train %s %.5f, %s %.5f, %s %.5f time %.1f sec' % (
        epoch, *cls_loss.get(), *obj_loss.get(), *box_loss.get(), time.time()-tic))
```

**yolo2_forward函数**用来将网络输出进行整理和转换。以这份代码的网络结构以及输入尺寸是3*256*256，batch_size=32为例，yolo2_forward函数的输入x是32*14*16*16。stride = num_class + 5这里的5是一个score加上四个坐标相关的值。x = x.transpose((0, 2, 3, 1))是将输出channel移到最后一个维度，然后通过x = x.reshape((0, 0, 0, -1, stride))得到5维的输出，前面3维不变，分别是batch size，weight，height，第4维是anchor的数量，第5维就是每个anchor对应的参数（2个类别数+1个score+4个坐标值），所以得到的x是32*16*16*2*7。cls_pred = x.slice_axis(begin=0, end=num_class, axis=-1)是取x的最后一维的前num_class个矩阵（这里是2）作为类别预测结果。 score_pred = x.slice_axis(begin=num_class, end=num_class + 1, axis=-1)是取x的最后一维的接下来1个矩阵作为score的预测结果。 xy_pred = x.slice_axis(begin=num_class + 1, end=num_class + 3, axis=-1)是取x的最后一维的再接下来的2个矩阵作为box的中心点坐标预测结果。 wh = x.slice_axis(begin=num_class + 3, end=num_class + 5, axis=-1)是取x的最后一维的再接下来的2个矩阵作为box的宽高预测结果。这样长度为7的最后一维就分清楚了。这里score = nd.sigmoid(score_pred)和 xy = nd.sigmoid(xy_pred)都是做归一化，前者是因为score的范围在0到1之间，后者是因为要用到grid cell的相对坐标，所以需要0到1范围（可以看原文Figure3的bx和by计算，这里模型预测得到的xy对应Figure3中的tx和ty）。transform_center是用来将每个grid cell里面的相对坐标转换成图片上的相对坐标。transform_size函数是将模型输出的宽高处理成实际的宽高。cid是预测的每个box的类别。left、top、right、bottom是预测的box的边界。

```python
def yolo2_forward(x, num_class, anchor_scales):
    """Transpose/reshape/organize convolution outputs."""
    stride = num_class + 5
    # transpose and reshape, 4th dim is the number of anchors
    x = x.transpose((0, 2, 3, 1))
    x = x.reshape((0, 0, 0, -1, stride))
    # now x is (batch, m, n, stride), stride = num_class + 1(object score) + 4(coordinates)
    # class probs
    cls_pred = x.slice_axis(begin=0, end=num_class, axis=-1)
    # object score
    score_pred = x.slice_axis(begin=num_class, end=num_class + 1, axis=-1)
    score = nd.sigmoid(score_pred)
    # center prediction, in range(0, 1) for each grid
    xy_pred = x.slice_axis(begin=num_class + 1, end=num_class + 3, axis=-1)
    xy = nd.sigmoid(xy_pred)
    # width/height prediction
    wh = x.slice_axis(begin=num_class + 3, end=num_class + 5, axis=-1)
    # convert x, y to positions relative to image
    x, y = transform_center(xy)
    # convert w, h to width/height relative to image
    w, h = transform_size(wh, anchor_scales)
    # cid is the argmax channel
    cid = nd.argmax(cls_pred, axis=-1, keepdims=True)
    # convert to corner format boxes
    half_w = w / 2
    half_h = h / 2
    left = nd.clip(x - half_w, 0, 1)
    top = nd.clip(y - half_h, 0, 1)
    right = nd.clip(x + half_w, 0, 1)
    bottom = nd.clip(y + half_h, 0, 1)
    output = nd.concat(*[cid, score, left, top, right, bottom], dim=4)
    return output, cls_pred, score, nd.concat(*[xy, wh], dim=4)
```

**在yolo2_forward函数中有两个重要的函数：transform_center函数和transform_sizer函数**。这两个函数是用来做坐标和长宽转换的。YOLO v2算法的一大亮点就是预测目标不是中心坐标或长宽的offset，而是offset的简单变换，具体可以看论文，接下来详细介绍。 
**transform_center函数**是用来将每个grid cell里面的相对坐标转换成图片上的相对坐标。首先输入xy是32*16*16*2*2大小，那么xy[0,1,1,0,:]就表示第一个输入的16*16的feature map上的(1,1)位置的第0个anchor的weight和height，feature map上的每个点代表一个grid cell，这个weight和height就是这个grid cell中某个点相对于grid cell的左上角的距离，如果weight=height=1，那么这个点就是grid cell的右下角点。offset_y是32*16*16*2*1大小，其中16*16是第一行为0，第二行为1…最后一行为15的二维矩阵，其他维度都是直接broadcast过去的，offset_x同理。这样在执行x + offset_x操作时，对于x[b,h,2,n,0]就是加上2，x[b,h,4,n,0]就是加上4。最后除以w或者除以h也是归一化的操作，使得最后得到的x和y范围在0到1之间。因此这个函数的作用就是实现论文中Figure3的加号这一步。 
![这里写图片描述](https://img-blog.csdn.net/20180225105857493?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```python
def transform_center(xy):
    """Given x, y prediction after sigmoid(), convert to relative coordinates (0, 1) on image."""
    b, h, w, n, s = xy.shape
    offset_y = nd.tile(nd.arange(0, h, repeat=(w * n * 1), ctx=xy.context).reshape((1, h, w, n, 1)), (b, 1, 1, 1, 1))
    # print(offset_y[0].asnumpy()[:, :, 0, 0])
    offset_x = nd.tile(nd.arange(0, w, repeat=(n * 1), ctx=xy.context).reshape((1, 1, w, n, 1)), (b, h, 1, 1, 1))
    # print(offset_x[0].asnumpy()[:, :, 0, 0])
    x, y = xy.split(num_outputs=2, axis=-1)
    x = (x + offset_x) / w
    y = (y + offset_y) / h
    return x, y
```

**transform_size函数**和transform_center函数类似。实现的是论文中Figure3的这一步（如下图公式）。输入wh对应tw和th。aw和ah就是box的宽高信息。 
![这里写图片描述](https://img-blog.csdn.net/2018022510593923?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```python
def transform_size(wh, anchors):
    """Given w, h prediction after exp() and anchor sizes, convert to relative width/height (0, 1) on image"""
    b, h, w, n, s = wh.shape
    aw, ah = nd.tile(nd.array(anchors, ctx=wh.context).reshape((1, 1, 1, -1, 2)), (b, h, w, 1, 1)).split(num_outputs=2, axis=-1)
    w_pred, h_pred = nd.exp(wh).split(num_outputs=2, axis=-1)
    w_out = w_pred * aw / w
    h_out = h_pred * ah / h
    return w_out, h_out
```

**yolo2_target函数**构造模型训练目标。这里输入labels就是ground truth，尺寸是32*1*5，1表示只有1个object，5包含1个class标签和4个坐标信息。for b in range(output.shape[0])是遍历batch中的每个输入，label是k*5大小的numpy array，k就是object数量，一般正常的object标签都是大于0的，所以这里valid_label是为了过滤掉那些错误的标签。输入scores的尺寸中n表示anchor的数量，h和w针对输入图像是256*256的情况分别是16和16。for l in valid_label就遍历一张图中所有有效的object标注信息，因为标注数据的坐标是采取框的左上角和右下角坐标（还是相对坐标，也就是值在0到1），所以通过简单的加减可得到gx、gy、gw和gh；ind_x和ind_y则是对应于输入的坐标，比如你的输入feature map是16*16，换句话说ind_x和ind_y就是16*16的feature map上的某个grid cell的坐标。因此重点来了：tx = gx * w - ind_x和ty = gy * h - ind_y，tx和ty是模型回归的目标值。intersect是计算每个anchor和ground truth的交集面积，因此是一个1*n的numpy array，n是anchor的数量；ovps是计算交集面积占并集面积的比例，也就是IOU，也是1*n大小。best_match是选择IOU最大的那个anchor的index。**接下来的几行赋值语句中都用到了ind_x和ind_y，这就是为什么说在YOLO算法中是以object的ground truth框的中心所在的box来预测该object，实际上所谓的box都是隐式的，从这里的介绍也可以看出，先是按照box的尺寸去匹配目前的这个object，找IOU最大的box，然后再一个ndarray中将ground truth的信息赋给该box，包括socre、坐标、类别标签、哪个box以及中心点坐标**。target_id[b, ind_y, ind_x, best_match, :] = l[0]是将IOU最大的anchor的标签赋值为ground truth的标签，只要没进行这个赋值的点的anchor的标签都是-1，表示背景。target_score[b, ind_y, ind_x, best_match, :] = 1.0是将best_match的box的score赋值为1，也就是置信度为1，其他都为0。tw和th的计算是论文中Figure3的公式的相反过程。因此最后的target_box放的就是模型训练的目标，符号和论文中的公式符号都是一一对应的。sample_weight表示权重。最后得到的tx和ty对应论文中sigmoid函数处理过的结果。

```
def yolo2_target(scores, boxes, labels, anchors, ignore_label=-1, thresh=0.5):
    """Generate training targets given predictions and labels."""
    b, h, w, n, _ = scores.shape
    anchors = np.reshape(np.array(anchors), (-1, 2))
    #scores = nd.slice_axis(outputs, begin=1, end=2, axis=-1)
    #boxes = nd.slice_axis(outputs, begin=2, end=6, axis=-1)
    gt_boxes = nd.slice_axis(labels, begin=1, end=5, axis=-1)
    target_score = nd.zeros((b, h, w, n, 1), ctx=scores.context)
    target_id = nd.ones_like(target_score, ctx=scores.context) * ignore_label
    target_box = nd.zeros((b, h, w, n, 4), ctx=scores.context)
    sample_weight = nd.zeros((b, h, w, n, 1), ctx=scores.context)
    for b in range(output.shape[0]):
        # find the best match for each ground-truth
        label = labels[b].asnumpy()
        valid_label = label[np.where(label[:, 0] > -0.5)[0], :]
        # shuffle because multi gt could possibly match to one anchor, we keep the last match randomly
        np.random.shuffle(valid_label)
        for l in valid_label:
            gx, gy, gw, gh = (l[1] + l[3]) / 2, (l[2] + l[4]) / 2, l[3] - l[1], l[4] - l[2]
            ind_x = int(gx * w)
            ind_y = int(gy * h)
            tx = gx * w - ind_x
            ty = gy * h - ind_y
            gw = gw * w
            gh = gh * h
            # find the best match using width and height only, assuming centers are identical
            intersect = np.minimum(anchors[:, 0], gw) * np.minimum(anchors[:, 1], gh)
            ovps = intersect / (gw * gh + anchors[:, 0] * anchors[:, 1] - intersect)
            best_match = int(np.argmax(ovps))
            target_id[b, ind_y, ind_x, best_match, :] = l[0]
            target_score[b, ind_y, ind_x, best_match, :] = 1.0
            tw = np.log(gw / anchors[best_match, 0])
            th = np.log(gh / anchors[best_match, 1])
            target_box[b, ind_y, ind_x, best_match, :] = mx.nd.array([tx, ty, tw, th])
            sample_weight[b, ind_y, ind_x, best_match, :] = 1.0
            # print('ind_y', ind_y, 'ind_x', ind_x, 'best_match', best_match, 't', tx, ty, tw, th, 'ovp', ovps[best_match], 'gt', gx, gy, gw/w, gh/h, 'anchor', anchors[best_match, 0], anchors[best_match, 1])
    return target_id, target_score, target_box, sample_weight
```

关于**损失函数**，这里针对分类和回归分别定义了两个损失函数：分类的交叉熵损失函数sce_loss和回归的L1损失函数l1_loss。obj_loss、cls_loss、box_loss是通过继承mx.metric.EvalMetric类来实现的，这个类原本是用来实现评价标准的，但是如果用这个类来输出loss的情况也是可以的，因为这里的这三个变量是用来输出到显示界面的，方便查看训练进度。

```python
sce_loss = gluon.loss.SoftmaxCrossEntropyLoss(from_logits=False)
l1_loss = gluon.loss.L1Loss()

from mxnet import metric

class LossRecorder(mx.metric.EvalMetric):
    """LossRecorder is used to record raw loss so we can observe loss directly
    """
    def __init__(self, name):
        super(LossRecorder, self).__init__(name)

    def update(self, labels, preds=0):
        """Update metric with pure loss
        """
        for loss in labels:
            if isinstance(loss, mx.nd.NDArray):
                loss = loss.asnumpy()
            self.sum_metric += loss.sum()
            self.num_inst += 1

obj_loss = LossRecorder('objectness_loss')
cls_loss = LossRecorder('classification_loss')
box_loss = LossRecorder('box_refine_loss')
```

**4、测试部分**主要包含两个步骤，先是读取数据并做预处理，然后才是过模型得到输出结果。数据读取和预处理通过process_image函数进行，调用mxnet.image.imdecode接口将打开的图像decode成height*width*3的NDArray，然后通过mxnet.image.imresize接口resize到指定尺寸，再将数据格式转成float32并做归一化，最后交换第0和第2个channel变成3*height*width的形式并增加一个维度成为1*3*height*width，这样就模拟了batch操作。predict函数用来对处理好的数据做预测，直接过训练好的模型net得到结果，然后通过yolo2_forward函数得到详细的结果。output.reshape((0, -1, 6))操作将输出reshape到3个维度，分别表示batch size，box数量，box的6个指标信息（类别、score、4个坐标信息），最后经过nms操作去掉重复框。

```python
def process_image(fname):
    with open(fname, 'rb') as f:
        im = image.imdecode(f.read())
    # resize to data_shape
    data = image.imresize(im, data_shape, data_shape)
    # minus rgb mean, divide std
    data = (data.astype('float32') - rgb_mean) / rgb_std
    # convert to batch x channel x height xwidth
    return data.transpose((2,0,1)).expand_dims(axis=0), im

def predict(x):
    x = net(x)
    output, cls_prob, score, xywh = yolo2_forward(x, 2, scales)
    return nd.contrib.box_nms(output.reshape((0, -1, 6)))

x, im = process_image('../img/pikachu.jpg')
out = predict(x.as_in_context(ctx))
```

显示检测结果，每调用一个display都传入一张图像的预测结果（out[0]）。display函数中plt.imshow(im.asnumpy())是用来显示图像，输入可以是numpy array，如果是RGB的话应该是(n,m,3)这样的维度，数值可以是uint8或者float。display函数中的for循环是遍历经过nms得到的框， if class_id < 0 or score < threshold语句跳过背景类以及score小于某个阈值的box，box = row[2:6] * np.array([im.shape[0],im.shape[1]]*2)是将预测的框边界映射成原图中的边界。rect是从box边界信息生成矩形框，并通过add_patch添加到plt中，这里plt.gca()相当于获取当前的plt内容，然后在此基础上再调add_patch方法添加新的内容比如框或者文字信息。

```python
mpl.rcParams['figure.figsize'] = (6,6)

colors = ['blue', 'green', 'red', 'black', 'magenta']

def display(im, out, threshold=0.5):
    plt.imshow(im.asnumpy())
    for row in out:
        row = row.asnumpy()
        class_id, score = int(row[0]), row[1]
        if class_id < 0 or score < threshold:
            continue
        color = colors[class_id%len(colors)]
        box = row[2:6] * np.array([im.shape[0],im.shape[1]]*2)
        rect = box_to_rect(nd.array(box), color, 2)
        plt.gca().add_patch(rect)
        text = class_names[class_id]
        plt.gca().text(box[0], box[1],
                       '{:s} {:.2f}'.format(text, score),
                       bbox=dict(facecolor=color, alpha=0.5),
                       fontsize=10, color='white')
    plt.show()

display(im, out[0], threshold=0.5)
```





