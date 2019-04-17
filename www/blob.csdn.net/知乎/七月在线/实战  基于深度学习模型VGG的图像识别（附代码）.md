# 实战 | 基于深度学习模型VGG的图像识别（附代码） - 知乎
# 



> 本文演示了如何使用百度公司的PaddlePaddle实现基于深度学习模型VGG的图像识别。

## **准备工作**

## **VGG简介**

牛津大学VGG(Visual Geometry Group)组在2014年ILSVRC提出的模型被称作VGG模型 。该模型相比以往模型进一步加宽和加深了网络结构，它的核心是五组卷积操作，每两组之间做Max-Pooling空间降维。同一组内采用多次连续的3X3卷积，卷积核的数目由较浅组的64增多到最深组的512，同一组内的卷积核数目是一样的。卷积之后接两层全连接层，之后是分类层。由于每组内卷积层的不同，有11、13、16、19层这几种模型，下图展示一个16层的网络结构。VGG模型结构相对简洁，提出之后也有很多文章基于此模型进行研究，如在ImageNet上首次公开超过人眼识别的模型就是借鉴VGG模型的结构。
![](https://pic1.zhimg.com/v2-5129c6a4d73f97209e03e24791f20064_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='299'></svg>)
## **数据准备**

通用图像分类公开的标准数据集常用的有CIFAR、ImageNet、COCO等，常用的细粒度图像分类数据集包括CUB-200-2011、Stanford Dog、Oxford-flowers等。其中ImageNet数据集规模相对较大，如模型概览一章所讲，大量研究成果基于ImageNet。ImageNet数据从2010年来稍有变化，常用的是ImageNet-2012数据集，该数据集包含1000个类别：训练集包含1,281,167张图片，每个类别数据732至1300张不等，验证集包含50,000张图片，平均每个类别50张图片。

由于ImageNet数据集较大，下载和训练较慢，为了方便大家学习，我们使用CIFAR10数据集。CIFAR10数据集包含60,000张32x32的彩色图片，10个类别，每个类包含6,000张。其中50,000张图片作为训练集，10000张作为测试集。图11从每个类别中随机抽取了10张图片，展示了所有的类别。

Paddle API提供了自动加载cifar数据集模块 paddle.dataset.cifar。

## **代码实现思路**

通过输入python train.py，就可以开始训练模型了，主要包括三个函数：
- def vgg_bn_drop(input_data):
- def event_handler(event):
- def train():




**第一步：vgg_bn_drop**

首先介绍VGG模型结构，由于CIFAR10图片大小和数量相比ImageNet数据小很多，因此这里的模型针对CIFAR10数据做了一定的适配。卷积部分引入了BN和Dropout操作。

## **函数完整代码**

VGG核心模块的输入是数据层，vgg_bn_drop 定义了16层VGG结构，每层卷积后面引入BN层和Dropout层，详细的定义如下：


```
def vgg_bn_drop(input):
    def conv_block(ipt, num_filter, groups, dropouts, num_channels=None):
        return paddle.networks.img_conv_group(
            input=ipt,
            num_channels=num_channels,
            pool_size=2,
            pool_stride=2,
            conv_num_filter=[num_filter] * groups,
            conv_filter_size=3,
            conv_act=paddle.activation.Relu(),
            conv_with_batchnorm=True,
            conv_batchnorm_drop_rate=dropouts,
            pool_type=paddle.pooling.Max())
    conv1 = conv_block(input, 64, 2, [0.3, 0], 3)
    conv2 = conv_block(conv1, 128, 2, [0.4, 0])
    conv3 = conv_block(conv2, 256, 3, [0.4, 0.4, 0])
    conv4 = conv_block(conv3, 512, 3, [0.4, 0.4, 0])
    conv5 = conv_block(conv4, 512, 3, [0.4, 0.4, 0])
    drop = paddle.layer.dropout(input=conv5, dropout_rate=0.5)
    fc1 = paddle.layer.fc(input=drop, size=512, act=paddle.activation.Linear())
    bn = paddle.layer.batch_norm(
        input=fc1,
        act=paddle.activation.Relu(),
        layer_attr=paddle.attr.Extra(drop_rate=0.5))
    fc2 = paddle.layer.fc(input=bn, size=512, act=paddle.activation.Linear())
    return fc2
```


## **VGG构造思路**

（1） 首先定义了一组卷积网络，即conv_block。卷积核大小为3x3，池化窗口大小为2x2，窗口滑动大小为2，groups决定每组VGG模块是几次连续的卷积操作，dropouts指定Dropout操作的概率。所使用的img_conv_group是在paddle.networks中预定义的模块，由若干组 Conv->BN->ReLu->Dropout 和 一组 Pooling 组成。

（2）五组卷积操作，即 5个conv_block。 第一、二组采用两次连续的卷积操作。第三、四、五组采用三次连续的卷积操作。每组最后一个卷积后面Dropout概率为0，即不使用Dropout操作。

（3）最后接两层512维的全连接。




## **第二步：event_handler**

## **函数完整代码**


```
# End batch and end pass event handler
def event_handler(event):
    if isinstance(event, paddle.event.EndIteration):
        if event.batch_id % 100 == 0:
            print "\nPass %d, Batch %d, Cost %f, %s" % (
                event.pass_id, event.batch_id, event.cost, event.metrics)
        else:
            sys.stdout.write('.')
            sys.stdout.flush()
    if isinstance(event, paddle.event.EndPass):
        # save parameters
        with open('params_pass_%d.tar' % event.pass_id, 'w') as f:
            trainer.save_parameter_to_tar(f)
        result = trainer.test(
            reader=paddle.batch(
                paddle.dataset.cifar.test10(), batch_size=128),
            feeding=feeding)
        print "\nTest with Pass %d, %s" % (event.pass_id, result.metrics)
```


## **函数解析**

event_handler主要功能：
- 观察训练过程： print()
- 模型参数保存：trainer.save_parameter_to_tar(f)
- 进行测试：trainer.test()

该回调函数是trainer.train函数里设定,event_handler_plot可以用来利用回调数据来打点画图,也可以输出日志。输出日志的示例图：


```
Pass 0, Batch 0, Cost 2.473182, {'classification_error_evaluator': 0.9140625}
...................................................................................................
Pass 0, Batch 100, Cost 1.913076, {'classification_error_evaluator': 0.78125}
...................................................................................................
Pass 0, Batch 200, Cost 1.783041, {'classification_error_evaluator': 0.7421875}
...................................................................................................
Pass 0, Batch 300, Cost 1.668833, {'classification_error_evaluator': 0.6875}
..........................................................................................
Test with Pass 0, {'classification_error_evaluator': 0.885200023651123}
```


## **第三步：train函数**

## 函数完整代码示例


```
def train():
    data_dim = 3 * 32 * 32
    class_dim = 10
    image = paddle.layer.data(
        name="image", type=paddle.data_type.dense_vector(data_dim))
    net = vgg_bn_drop(image)
    out = paddle.layer.fc(input=net,
                          size=class_dim,
                          act=paddle.activation.Softmax())
    lbl = paddle.layer.data(
        name="label", type=paddle.data_type.integer_value(class_dim))
    cost = paddle.layer.classification_cost(input=out, label=lbl)
    parameters = paddle.parameters.create(cost)
    print(parameters.keys())
    momentum_optimizer = paddle.optimizer.Momentum(
        momentum=0.9,
        regularization=paddle.optimizer.L2Regularization(rate=0.0002 * 128),
        learning_rate=0.1 / 128.0,
        learning_rate_decay_a=0.1,
        learning_rate_decay_b=50000 * 100,
        learning_rate_schedule='discexp')
    # Create trainer
    trainer = paddle.trainer.SGD(cost=cost,
                                 parameters=parameters,
                                 update_equation=momentum_optimizer)
    reader = paddle.batch(
        paddle.reader.shuffle(
            paddle.dataset.cifar.train10(), buf_size=50000),
        batch_size=128)
    feeding = {'image': 0,
               'label': 1}
    trainer.train(
        reader=reader,
        num_passes=200,
        event_handler=event_handler,
        feeding=feeding)
```


## **函数解析**

## **1. 定义数据输入及其维度**

网络输入定义为 data_layer (数据层)，在图像分类中即为图像像素信息。CIFRAR10是RGB 3通道32x32大小的彩色图，因此输入数据大小为3072(3x32x32)，类别大小为10，即10分类。


```
datadim = 3 * 32 * 32
classdim = 10
image = paddle.layer.data(
    name="image", type=paddle.data_type.dense_vector(datadim))
```


## **2. 定义VGG网络核心模块**


```
net = vgg_bn_drop(image)
```


## **3. 定义分类器**

通过上面VGG网络提取高层特征，然后经过全连接层映射到类别维度大小的向量，再通过Softmax归一化得到每个类别的概率，也可称作分类器。


```
out = paddle.layer.fc(input=net,
                      size=classdim,
                      act=paddle.activation.Softmax())
```


## **4. 定义网络输出和损失函数**

在有监督训练中需要输入图像对应的类别信息，同样通过paddle.layer.data来定义。训练中采用多类交叉熵作为损失函数，并作为网络的输出，预测阶段定义网络的输出为分类器得到的概率信息。


```
lbl = paddle.layer.data(
    name="label", type=paddle.data_type.integer_value(classdim))
cost = paddle.layer.classification_cost(input=out, label=lbl)
```


## **5. 定义参数**

首先依据模型配置的cost定义模型参数。


```
# Create parameters
parameters = paddle.parameters.create(cost)
```


可以打印参数名字，如果在网络配置中没有指定名字，则默认生成。


```
print parameters.keys()
```


## **6. 构造优化器**

根据网络拓扑结构和模型参数来构造出trainer用来训练，在构造时还需指定优化方法，这里使用最基本的Momentum方法，同时设定了学习率、正则等。


```
# Create optimizer
momentum_optimizer = paddle.optimizer.Momentum(
    momentum=0.9,
    regularization=paddle.optimizer.L2Regularization(rate=0.0002 * 128),
    learning_rate=0.1 / 128.0,
    learning_rate_decay_a=0.1,
    learning_rate_decay_b=50000 * 100,
    learning_rate_schedule='discexp')
#####7. 创建训练器
# Create trainer
trainer = paddle.trainer.SGD(cost=cost,
                             parameters=parameters,
                             update_equation=momentum_optimizer)
```


通过 learning_rate_decay_a、learning_rate_decay_b 和 learning_rate_schedule 指定学习率调整策略，这里采用离散指数的方式调节学习率，计算公式如下，n 代表已经处理过的累计总样本数，lr0 即为 settings 里设置的 learning_rate。

## **7. 启动训练器**

cifar.train10()每次产生一条样本，在完成shuffle和batch之后，作为训练的输入。


```
reader=paddle.batch(
    paddle.reader.shuffle(
        paddle.dataset.cifar.train10(), buf_size=50000),
        batch_size=128)
```


通过feeding来指定每一个数据和paddle.layer.data的对应关系。例如: cifar.train10()产生数据的第0列对应image层的特征。


```
feeding={'image': 0,
         'label': 1}
```


通过trainer.train函数训练:


```
trainer.train(
        reader=reader,
        num_passes=200,
        event_handler=event_handler,
        feeding=feeding)
```


## **第四步：头部和尾部**

## 头部：模型初始化

通过 paddle.init，初始化Paddle是否使用GPU，trainer的数目等等。


```
import sys
import paddle.v2 as paddle
from vgg import vgg_bn_drop
from resnet import resnet_cifar10
# PaddlePaddle init
paddle.init(use_gpu=False, trainer_count=1)
```


## 尾部：主函数


```
if __name__ == '__main__':
    train()
```





**推断模型（测试）**

可以使用训练好的模型对图片进行分类，下面程序展示了如何使用paddle.infer接口进行推断，可以打开注释，更改加载的模型。


```
from PIL import Image
import numpy as np
import os
def load_image(file):
    im = Image.open(file)
    im = im.resize((32, 32), Image.ANTIALIAS)
    im = np.array(im).astype(np.float32)
    # PIL打开图片存储顺序为H(高度)，W(宽度)，C(通道)。
    # PaddlePaddle要求数据顺序为CHW，所以需要转换顺序。
    im = im.transpose((2, 0, 1)) # CHW
    # CIFAR训练图片通道顺序为B(蓝),G(绿),R(红),
    # 而PIL打开图片默认通道顺序为RGB,因为需要交换通道。
    im = im[(2, 1, 0),:,:] # BGR
    im = im.flatten()
    im = im / 255.0
    return im
test_data = []
cur_dir = os.getcwd()
test_data.append((load_image(cur_dir + '/image/dog.png'),))
# with open('params_pass_50.tar', 'r') as f:
#    parameters = paddle.parameters.Parameters.from_tar(f)
probs = paddle.infer(
    output_layer=out, parameters=parameters, input=test_data)
lab = np.argsort(-probs) # probs and lab are the results of one batch data
print "Label of image/dog.png is: %d" % lab[0][0]
```


[http://geek.csdn.net/news/detail/239674](https://link.zhihu.com/?target=http%3A//geek.csdn.net/news/detail/239674)

对深度学习感兴趣的同学可以加我微信：julyedukefu01 **免费领深度学习干货大礼包**，


含以下资料：

一、6门深度学习课程

二、1本《深度学习》“圣经”

三、10G深度学习训练数据集

四、20+深度学习论文集合

五、kaggle挑战赛完整源码

六、《一天搞懂深度学习》课件


