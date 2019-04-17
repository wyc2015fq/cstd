# 利用Caffe训练模型(solver、deploy、train_val)+python使用已训练模型 - 素质云笔记/Recorder... - CSDN博客





2017年01月06日 19:17:49[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：10781标签：[python																[caffe																[训练																[solver																[识别](https://so.csdn.net/so/search/s.do?q=识别&t=blog)
个人分类：[图像︱caffe](https://blog.csdn.net/sinat_26917383/article/category/6666555)





*本文部分内容来源于CDA深度学习实战课堂，由唐宇迪老师授课*
- 如果你企图用CPU来训练模型，那么你就疯了…
- 训练模型中，最耗时的因素是图像大小size，一般227*227用CPU来训练的话，训练1万次可能要超过1周的时间。
- 不同的网络结构，可能会有不同图片尺寸的需求，所以训练之前需要了解一下，在生成LMDB环节就直接符合上模型的数据要求。
- 如果你自己DIY了框架，那么不知道如何检验框架与通用框架比较，是否优质，可以去benchmarks网站，跟别人的PK一下：[http://human-pose.mpi-inf.mpg.de/](http://human-pose.mpi-inf.mpg.de/)
- Caffe官网： Examples：主要集中在训练模型 Notebook Examples：集中在fun-tuning模型

# 一、训练文件配置详情

## 1、参数文件solver.prototxt

以caffeNet为例，参数解读：

```
net: "/caffe/examples/lmdb_test/train/bvlc_reference_caffenet/train_val.prototxt"
    # 训练的prototxt在哪，路径
test_iter: 1000
    # 测试要迭代多少个Batch  test_iter * batchsize(测试集的)＝ 测试集的大小
test_interval: 500
    # 每500次迭代，就在用测试集进行测试
base_lr: 0.01
    # 设置初始化的学习率为0.01
lr_policy: "step"
    # 权重衰减策略。
gamma: 0.1
stepsize: 100000
    # 初始的学习率为0.01，并且每100000次迭代中进行学习率下降
display: 20
    # 每20次epoch就显示出一些数据信息
max_iter: 50000
    # 迭代次数
momentum: 0.9
    # 一直都是0.9，固定不变；迭代的数据更快，步伐更快

weight_decay: 0.0005
    # 权重衰减因子为0.0005
snapshot: 10000
    # 每10000次迭代中，就生成当前状态的快照
snapshot_prefix: "/caffe/examples/lmdb_test/train/bvlc_reference_caffenet"
    # 模型快照保存

solver_mode: CPU
    # 可以设定GPU还是cpu
```

**快照的大用途：**如果出了什么意外中断了训练，那真是天都要塌了，所以快照存储了训练的中间结果，这个设计真是人性化，当再次训练时，就可以从快照中恢复数据了。直接在最后的执行文件，调用已经训练的快照就行，用-snapshot

## 2、框架文件,train_val.prototxt

> 
框架文件每一层参数具体含义是什么？可参考caffe官方链接：[http://caffe.berkeleyvision.org/tutorial/layers.html](http://caffe.berkeleyvision.org/tutorial/layers.html)


训练文件:配置训练阶段的图片数据集、配置训练阶段的标签数据集、配置测试阶段的图片数据集 、配置测试阶段的标签数据集 、 多标签损失函数(博客：[Caffe中LMDB接口实现多标签数据准备及训练](http://blog.csdn.net/hyman_yx/article/details/51791136))

网络的配置文件——定义网络

```
Name:””  #随便写
Layer
{
Name:””  #名称
Type:””  #很严格
Top:”label” # 接最后的内容，最后全连接层会出现"bottom:"label""
}

Transform.param
# 1/256，归一化,多少大小一般都需要进行归一化处理

Batch_size:64  
#一次迭代多少个样本

Layer
{data}
# 两个数据层，一个训练层，一个验证层

Layer 
{
Conv1
}
```

需要注意的是：

**1. 最后全连接输出，时候你是几分类，就要填几：**

多分类数量就看训练时候的分类个数。

```bash
inner_product_param {
    num_output: 2
    weight_filler {
      type: "gaussian"
      std: 0.01
```

**2. 自己定义层的时候**

 需要自己写不同的层，c++自己写，挺麻烦的

**3. 图片尺寸**

图片尺寸是根据网络来定义的，大型网络都是227*227（224*224），VGG，ALEX都是这样，譬如lenet小网络就可以用28*28

**4. batch的作用**

Batch越大越好，一般64.小的话，可以更好地显示，小步迭代

## 3、模型执行文件train.sh

执行文件就是训练好之后，你要在linux下运行的文件。 

```
./build/tools/caffe train \
  # caffe这个工具在哪，一般就在tool之下
-gpu 0 \
  # 不必须，是否用GPU，譬如你有很多块GPU，每块都有编号，那么就可以直接选择某一块GPU。其中如果你有四块GPU，那么就可以-gpu all
-model path/to/trainval.prototxt \
  # 不必须，因为后面有solver参数文件，而参数文件里面有trainval.prototxt文件，一般都不用写
-solver path/to/solver.prototxt \
  # 必须，prototxt文件内容在哪
-weights path/to/pretrained_weights.caffemodel
  # 不必须，-weight用来做fine-tuning，参数拿来学习。这个在fine-tuning时候用得上
--snapshot=examples/imagenet/myself/model/caffenet_train_1000.solverstate
  # 如果断了，这时候就可以用快照继续训练啦~  只要在执行文件中加入snapshot的路径
```

其中snapshot可是大杀器，笔者觉得有两个用途：
- 1、临时停机了…机器训练中断了… 

训练的时候是遵循snapshot每10000次生成一次快照，如果停机就可以继续延续上次的内容继续训练，再写一遍这个文件即可。
- 2、要fine-tuning别人的模型，则需要先down他们的模型快照，然后继续训练，继续训练的时候可以让学习率降低到很小，把全连接层可以稍微分一下。

## 4、验证集文件：deploy.prototxt

这个文件是预测的时候需要用到的，跟参数trian_val.prototxt训练文件很相似。

> 
train_val.prototxt文件=数据输入+卷积层+全连接层+loss/accuracy 

  Deploy.prototxt文件=简化版数据输入+卷积层+全连接层+prob预测层


其他真的不用改动。train_val的ImageData输入层，需要修改为input层。

就拿**AlexNet来说，不同之处在数据输入部分+最后链接层**
- Deploy.prototxt的数据输入部分：

```
layer {
  name: "data"
  type: "Input"
  top: "data"
  input_param { shape: { dim: 10 dim: 3 dim: 227 dim: 227 } }
}
```

其中input_param中的参数意义为： 

**第一个：**dim，对待识别样本图片进行数据增广的数量，一个图片会变成10个，之后输入到网络进行识别。如果不进行数据增广，可以设置成1。  
**第二个：**图片的通道数，一般灰度图片为单通道，则值为1，如果为非灰度图3通道图片则为3。  
**第三个：**图片的高度，单位像素。  
**第四个：**图片的宽度，单位像素。  

内容来源于：[caffe生成lenet-5的deploy.prototxt文件](http://blog.csdn.net/lanxuecc/article/details/52474476)- Deploy.prototxt的全连接prob部分：

```
layer {
  name: "prob"
  type: "Softmax"
  bottom: "fc8"
  top: "prob"
}
```

输出的是概率值，参数trian_val.prototxt集合中全连接层之后链接的是loss/accuracy。可以看出训练集要求的主要内容是输出loss/accuracy，衡量训练精度；而验证集文件，主要就是将图片分类输出出来。

# 二、训练完之后如何测试新数据

官方案例案例链接ipynb格式:[打开链接](http://localhost:8888/notebooks/Python%20Scripts/00-classification.ipynb)

## 1、如何将mean.binaryproto转mean.npy

> 
由于验证的时候需要在python下，验证新图片的时候，是先读入然后减去均值，这时候均值就需要一个python可以理解的版本。 

  网络上有博客总结两种方法（博客:[Caffe均值文件mean.binaryproto转mean.npy](http://blog.csdn.net/hyman_yx/article/details/51732656)）：mean.binaryproto转mean.npy、已知均值情况下用均值创建


**（1）mean.binaryproto 转化**

使用Caffe的C++接口进行操作时，需要的图像均值文件是pb格式，例如常见的均值文件名为mean.binaryproto；但在使用Python接口进行操作时，需要的图像均值文件是numpy格式，例如mean.npy。所以在跨语言进行操作时，需要将mean.binaryproto转换成mean.npy，转换代码如下：

```php
import caffe
import numpy as np

MEAN_PROTO_PATH = 'mean.binaryproto'               # 待转换的pb格式图像均值文件路径
MEAN_NPY_PATH = 'mean.npy'                         # 转换后的numpy格式图像均值文件路径

blob = caffe.proto.caffe_pb2.BlobProto()           # 创建protobuf blob
data = open(MEAN_PROTO_PATH, 'rb' ).read()         # 读入mean.binaryproto文件内容
blob.ParseFromString(data)                         # 解析文件内容到blob

array = np.array(caffe.io.blobproto_to_array(blob))# 将blob中的均值转换成numpy格式，array的shape （mean_number，channel, hight, width）
mean_npy = array[0]                                # 一个array中可以有多组均值存在，故需要通过下标选择其中一组均值
np.save(MEAN_NPY_PATH ,mean_npy)
```

**（2）已知图像均值，构造mean.npy**

如果已知图像中每个通道的均值，例如3通道图像每个通道的均值分别为104，117，123，我们也可以通过其构造mean.npy。代码如下：

```
import numpy as np

MEAN_NPY_PATH = 'mean.npy'

mean = np.ones([3,256, 256], dtype=np.float)
mean[0,:,:] = 104
mean[1,:,:] = 117
mean[2,:,:] = 123

np.save(MEAN_NPY, mean)
```

（3）如何加载mean.npy文件 

上面我们用两种方式构造了均值文件mean.npy，在使用时载入mean.npy的代码如下：

```
import numpy as np

mean_npy = np.load(MEAN_NPY_PATH)
mean = mean_npy.mean(1).mean(1)
```

## 2、利用python做预测

**（1）模块加载与设置环境**

```python
#加载模块与图像参数设置
import numpy as np
import matplotlib.pyplot as plt
plt.rcParams['figure.figsize'] = (10, 10)        # large images
plt.rcParams['image.interpolation'] = 'nearest'  # don't interpolate: show square pixels
plt.rcParams['image.cmap'] = 'gray' 

#模型路径deploy
import caffe
import os

caffe.set_mode_cpu()

model_def = caffe_root + 'examples/faceDetech/deploy.prototxt'
model_weights = caffe_root + 'examples/faceDetech/alexnet_iter_50000_full_conv.caffemodel'


#模型加载
net = caffe.Net(model_def,      # defines the structure of the model
                model_weights,  # contains the trained weights
                caffe.TEST)
```

其中如何你没有训练好的模型那么caffe官方有一套，利用imagenet图片和caffenet模型训练好了一个caffemodel,  供大家下载。要进行图片的分类，这个caffemodel是最好不过的了。下载地址为：[http://dl.caffe.berkeleyvision.org/bvlc_reference_caffenet.caffemodel](http://dl.caffe.berkeleyvision.org/bvlc_reference_caffenet.caffemodel)

或者命令行下载：
`# sudo ./scripts/download_model_binary.py models/bvlc_reference_caffenet`
**（2）模型预处理阶段-不处理均值的情况**

```
transformer = caffe.io.Transformer({'data': net.blobs['data'].data.shape})
  # 那么reshape操作，就是自动将验证图片进行放缩
transformer.set_transpose('data', (2,0,1))  # move image channels to outermost dimension
  # transpose将RGB变为BGR,都要做transpose
  # BGR谁放在前面，譬如3*300*100，这里设定3在前面
transformer.set_raw_scale('data', 255)      # rescale from [0, 1] to [0, 255]
  # 像素点rescale操作，将数据的像素点集中在[0,255]区间内
transformer.set_channel_swap('data', (2,1,0))  

  # CPU classification              
 net.blobs['data'].reshape(50,        # batch size
                          3,         # 3-channel (BGR) images
                          227, 227)
```

**（3）单张新图片处理与识别**

```
image = caffe.io.load_image("/caffe/data/trainlmdb/val/test_female/image_00010.jpg") 
# 导入图片         
transformed_image = transformer.preprocess('data', image)        
# 预处理图片
output = net.forward()            
# 前向传播一次，找出参数
net.blobs['data'].data[...] = transformed_image         
output_prob = output['prob'][0]                
# 输出概率
print 'predicted class is:', output_prob.argmax() 
# 输出最大可能性
```

笔者训练的一个二分类结果是：
`array([ 0.34624347,  0.65375656], dtype=float32)`
# 转载：三、多张训练循环读取

本节主要参考博客：[Caffe学习系列(20)：用训练好的caffemodel来进行分类](http://www.cnblogs.com/denny402/p/5111018.html)

> 
在caffe根目录下的 examples/cpp-classification/ 

  文件夹下面，有个classification.cpp文件，就是用来分类的。当然编译后，放在/build/examples/cpp_classification/ 

  下面
数据准备好了，我们就可以开始分类了，我们给大家提供两个版本的分类方法：

## **一、c++方法**

在caffe根目录下的 examples/cpp-classification/ 文件夹下面，有个classification.cpp文件，就是用来分类的。当然编译后，放在/build/examples/cpp_classification/ 下面

我们就直接运行命令：

```
# sudo ./build/examples/cpp_classification/classification.bin \
  models/bvlc_reference_caffenet/deploy.prototxt \
  models/bvlc_reference_caffenet/bvlc_reference_caffenet.caffemodel \
  data/ilsvrc12/imagenet_mean.binaryproto \
  data/ilsvrc12/synset_words.txt \
  examples/images/cat.jpg
```

命令很长，用了很多的\符号来换行。可以看出，从第二行开始就是参数，每行一个，共需要4个参数

运行成功后，输出top-5结果：

```bash
---------- Prediction for examples/images/cat.jpg ----------
0.3134 - "n02123045 tabby, tabby cat"
0.2380 - "n02123159 tiger cat"
0.1235 - "n02124075 Egyptian cat"
0.1003 - "n02119022 red fox, Vulpes vulpes"
0.0715 - "n02127052 lynx, catamount"
```

即有0.3134的概率为tabby cat, 有0.2380的概率为tiger cat ……

## **二、python方法**

python接口可以使用jupyter notebook来进行可视化操作，因此推荐使用这种方法。

在这里我就不用可视化了，编写一个py文件，命名为py-classify.py

```
#coding=utf-8
#加载必要的库
import numpy as np

import sys,os

#设置当前目录
caffe_root = '/home/xxx/caffe/' 
sys.path.insert(0, caffe_root + 'python')
import caffe
os.chdir(caffe_root)

net_file=caffe_root + 'models/bvlc_reference_caffenet/deploy.prototxt'
caffe_model=caffe_root + 'models/bvlc_reference_caffenet/bvlc_reference_caffenet.caffemodel'
mean_file=caffe_root + 'python/caffe/imagenet/ilsvrc_2012_mean.npy'

net = caffe.Net(net_file,caffe_model,caffe.TEST)
transformer = caffe.io.Transformer({'data': net.blobs['data'].data.shape})
transformer.set_transpose('data', (2,0,1))
transformer.set_mean('data', np.load(mean_file).mean(1).mean(1))
transformer.set_raw_scale('data', 255) 
transformer.set_channel_swap('data', (2,1,0))

im=caffe.io.load_image(caffe_root+'examples/images/cat.jpg')
net.blobs['data'].data[...] = transformer.preprocess('data',im)
out = net.forward()


imagenet_labels_filename = caffe_root + 'data/ilsvrc12/synset_words.txt'
labels = np.loadtxt(imagenet_labels_filename, str, delimiter='\t')

top_k = net.blobs['prob'].data[0].flatten().argsort()[-1:-6:-1]
for i in np.arange(top_k.size):
    print top_k[i], labels[top_k[i]]
```

执行这个文件，输出：

```matlab
281 n02123045 tabby, tabby cat
282 n02123159 tiger cat
285 n02124075 Egyptian cat
277 n02119022 red fox, Vulpes vulpes
287 n02127052 lynx, catamount
```

caffe开发团队实际上也编写了一个python版本的分类文件，路径为 python/classify.py

运行这个文件必需两个参数，一个输入图片文件，一个输出结果文件。而且运行必须在python目录下。假设当前目录是caffe根目录，则运行：

```
# cd python
# sudo python classify.py ../examples/images/cat.jpg result.npy
```

分类的结果保存为当前目录下的result.npy文件里面，是看不见的。而且这个文件有错误，运行的时候，会提示

```java
Mean shape incompatible with input shape
```

的错误。因此，要使用这个文件，我们还得进行修改：

**1、修改均值计算：**

定位到 
`mean = np.load(args.mean_file)`
这一行，在下面加上一行：
`mean=mean.mean(1).mean(1)`
则可以解决报错的问题。

**2、修改文件，使得结果显示在命令行下：**

定位到

```
# Classify.
    start = time.time()
    predictions = classifier.predict(inputs, not args.center_only)
    print("Done in %.2f s." % (time.time() - start))
```

这个地方，在后面加上几行，如下所示：

```
# Classify.
    start = time.time()
    predictions = classifier.predict(inputs, not args.center_only)
    print("Done in %.2f s." % (time.time() - start))
    imagenet_labels_filename = '../data/ilsvrc12/synset_words.txt'
    labels = np.loadtxt(imagenet_labels_filename, str, delimiter='\t')
    top_k = predictions.flatten().argsort()[-1:-6:-1]
    for i in np.arange(top_k.size):
        print top_k[i], labels[top_k[i]]
```

就样就可以了。运行不会报错，而且结果会显示在命令行下面。 

.

## 延伸一：caffe中网络结构可视化

还有一个是python下面的接口，draw_net.py可以根据.prototxt文件将模式用图示的方法表示出来，博文开始的模型图即用该接口所绘

```
./python/draw_net.py ./examples/siamese/mnist_siamese.prototxt   ./examples/siamese/mnist_siamese.png
#使用该接口进行网络的绘制示例化
```

第一个参数为模型文件，第二个参数为所绘模型图的保存地址。参考博客：[Caffe使用step by step：caffe框架下的基本操作和分析](http://www.cnblogs.com/empty16/p/4878164.html)](https://so.csdn.net/so/search/s.do?q=solver&t=blog)](https://so.csdn.net/so/search/s.do?q=训练&t=blog)](https://so.csdn.net/so/search/s.do?q=caffe&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)




