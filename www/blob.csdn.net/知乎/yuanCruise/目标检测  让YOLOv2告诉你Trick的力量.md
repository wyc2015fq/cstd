# 目标检测 | 让YOLOv2告诉你Trick的力量 - 知乎
# 

### 0.简介

在前一篇文章[目标检测 | 让YOLOv1算法告诉你回归网络的能力](https://link.zhihu.com/?target=https%3A//www.jianshu.com/p/d1a8ebabfb8b)中我们见证了回归网络的能力，本文就利用YOLOv2的整体实现，来告诉大家一些在目标检测领域中的Trick的力量。YOLOv2论文中其实讲到了两个算法：YOLOv2和YOLO9000。本文因为主要介绍的是检测领域的Trick，因此着重介绍YOLOv2算法原理，及其在YOLOv1基础下的各个改进点。本文先介绍YOLOv2的各种改进点，最后再介绍YOLOv2的训练过程（不具体介绍网络的测试过程，因为其和YOLOv1中的前向测试类似）。

### 1.改进点

YOLOv2的整体结构和YOLOv1是类似的，都是用回归网络解决目标检测问题，由于YOLOv1过于在意网络的速度，因此其网络召回率很一般。当然这主要原因是YOLOv1算法中每个cell只能预测两个bounding box，不像类Faster Rcnn系列算法有很多的anchor可以用来预测目标，导致YOLOv1算法在这方面存在缺陷，基于此YOLOv2提出了如下几个改进点。

不会改变网络结构 - High Resolution Classifier（高分辨率的预训练） - Multi-Scale Training（不同size训练策略） - Dimension Clusters（引入聚类策略） - Direct location prediction（坐标直接预测）

会改变网络结构 - Batch Normalization（加入BN层） - Convolutional With Anchor Boxes（将v1中的bounding box替换为anchor box） - Fine-Grained Features（多尺度检测，reorg层） - Darknet-19（更轻量级的特征提取网络）

### 2.改进点详解

（1）引入多尺度策略，预训练和训练

在引入多尺度策略上，YOLOv2一共用了三种trick，分别在预训练参数产生过程中，训练检测网络过程中，训练网络结构的调整这三个过程中进行多尺度的尝试。 - High Resolution Classifier（高分辨率的预训练） - Multi-Scale Training（不同size训练策略）

这两个策略在这种一起介绍了，省的大家会搞混，因为这两个策略有点类似。 High Resolution 主要是针对于预训练的，用High Resolution策略可以得到更加适应于检测任务的预训练参数。作者先用将ImageNet数据集中的图片resize成224*224并在网络对应的backbone（特征提取器）上进行训练，大概迭代160代后（表示将所有训练数据循环跑160次），然后再将输入调整到448*448，再训练10代。至此就得到了YOLOv2网络会用到的预训练参数。这样的策略相比于直接拿网络用单一尺度的预训练参数，可以提高大约4%的MAP。 Multi-Scale主要针对于YOLOv2的训练过程，用该策略可以使得网络在最终应用的时候对于不同尺度的输入能够有较好的鲁棒性。作者每训练10个batch，随机改变一种网络的输入size，这些size从{320, 352, ..., 608}这几个值中选择，原因是YOLOv2网络的整体降采样倍数为32，所以输入分辨率最好为32的倍数。
- Fine-Grained Features（多尺度检测，reorg层） 该部分的多尺度策略和上述的两种略有不一样，正如SSD网络会将不同层级也就是不同大小的特征图用于最后的detect，YOLOv2中以相同的思想但不完全一致的策略实现这个过程。 
![](https://pic1.zhimg.com/v2-b28f25157440fcb82c52efddeecf5ddc_b.png)
 如上图所示，这个层的作用就是将前面一层的26*26的feature map和本层的13*13的feature map进行连接，当然concat操作时需要特征图的大小是一致的，因此对于26*26的特征图首先做一个reorg操作，将其变成13*13但又不破坏其大特征图的特性，然后进行concat操作。接下来详细介绍下reorg操作的含义。 在caffe中reorg在做前向计算时的源码如下：

```
void reorg_cpu(float *x, int w, int h, int c, int batch, int stride, int forward, float *out)
{
    int b,i,j,k;
    int out_c = c/(stride*stride);

    for(b = 0; b < batch; ++b){
        for(k = 0; k < c; ++k){
            for(j = 0; j < h; ++j){
                for(i = 0; i < w; ++i){
                    int in_index  = i + w*(j + h*(k + c*b));
                    int c2 = k % out_c;
                    int offset = k / out_c;
                    int w2 = i*stride + offset % stride;
                    int h2 = j*stride + offset / stride;
                    int out_index = w2 + w*stride*(h2 + h*stride*(c2 + out_c*b));
                    if(forward) out[out_index] = x[in_index];
                    else out[in_index] = x[out_index];
                }
            }
        }
    }
}
```

仔细阅读上述源码，我们利用一张网络上的图来直观的解释上述源码的含义：如下图所示，将双倍的特征图中的特征，按照层级进行排列，即减小了特征图尺寸，又能够保留大特征图的特性。 
![](https://pic2.zhimg.com/v2-3333aca71e4ff843df0806d0f668a7fd_b.jpg)

（2）引入Anchor机制

引入了在Faster Rcnn中提出的anchor机制，并结合聚类策略，和直接坐标预测策略更好的应用anchor，使得网络效果提升。
- Convolutional With Anchor Boxes（将v1中的bounding box替换为anchor box）

目标检测领域另一类召回率很高的Faster Rcnn网络中有一个很好的策略就是Anchor，正是因为anchor的存在使得网络最终能够预测更多的框，召回率能够高也是自然的，因此YOLOv2为了提升算法的召回率，特此引入了Anchor机制。

>  用416*416大小的输入代替原来448*448。这样做的原因在于希望得到的特征图都有奇数大小的宽和高，奇数大小的宽和高会使得每个特征图在划分cell的时候就只有一个center cell（比如可以划分成7*7或9*9个cell，center cell只有一个，如果划分成8*8或10*10的，center cell就有4个）。为什么希望只有一个center cell呢？因为大的object一般会占据图像的中心，所以希望用一个center cell去预测，而不是4个center cell去预测。网络最终将416*416的输入变成13*13大小的feature map输出，也就是缩小比例为32。 （来自网络）

- Dimension Clusters（引入聚类策略）

引入了anchor机制之后，为了能够更好的利用anchor，YOLOv2中引入了聚类机制，利用kmean算法对训练集中的目标框尺寸做聚类，找到更加适应于当前数据集的先验anchor尺寸。 需要注意的是，对目标框进行聚类操作和上图5-8中对样本点进行聚类操作有些不同。对目标框进行聚类操作时由于k-means算法的特性需要计算目标框和Anchor框之间的距离，如果直接采用欧式距离就会导致较大的目标框会比较小的目标框产生更大的训练误差，最终使得训练效果不理想。因此针对当前样本空间的距离，YOLOv2中使用如下公式中的距离定义方式。             
![](https://pic4.zhimg.com/v2-d08f6c1959fb910b7fe67e87d18798b7_b.png)

根据下图可知IOU与目标框的尺寸无关，因此利用公式中定义的距离能够规避掉由于尺寸带来的误差不均问题。 
![](https://pic2.zhimg.com/v2-7d2cbb98dab51b733af919a0e4b207d9_b.jpg)
 作者采用的5个anchor（Cluster IOU）的平均IOU是61，而采用9种Anchor 的Faster RCNN的平均IOU是60.9，也就是说本文仅选取5种box就能达到Faster RCNN的9中box的效果。 
![](https://pic3.zhimg.com/v2-dbb59e8cef2c920eeaba6e87e7afd6be_b.jpg)

- Direct location prediction（坐标直接预测）

还有需要注意的一点，YOLOv2中虽然引入了和Faster Rcnn中类似的anchor，但在训练预测的过程中，作者参考Faster Rcnn的回归偏置的方式但发现训练不稳定，因此作者使用了YOLOv1中回归bounding box的策略。接下来先介绍下在Faster Rcnn中是如何对anchor进行回归预测的。

![](https://pic3.zhimg.com/v2-bdbb5c4309ae1a363766c465ffaea1ae_b.jpg)

如上图所示，在Faster Rcnn中对box的中心坐标的预测是基于anchor坐标的偏移量得到的。这个策略之所以会导致最终的训练不稳定，其原因在于：

此公式对预测结果没有过多约束，box可能出现的区域比较广： - 当tx=1时，box将向右偏移一个anchor box的宽度； - 当tx=-1时，box将向左偏移一个anchor box的宽度；

正如上述的，由于训练不稳定的问题，YOLOv2中没有采用预测偏置的策略，而是采用了YOLOv1中提出的方法，就是预测anchor（v1中是bounding box）中心点相对于对应cell左上角位置的相对偏移值，利用这种方法的好处就是能够将anchor的偏移固定在某个范围内，使得网络训练更加稳定。如下图所示，每一个anchor最后预测的bounding box都会预测如下的五个值，并且中心点坐标会通过sigmoid函数约束在0-1之间，并且相对于cell的位置，使得最后网络更加稳定。 
![](https://pic2.zhimg.com/v2-329cdc5df1c12ef2891b6f680aa88bf5_b.jpg)

![](https://pic2.zhimg.com/v2-18f1c4026cf49d2f978e5ebf76ee5b0d_b.jpg)

（3）全新的特征提取模块（darknet13） - Darknet-19（更轻量级的特征提取网络） 作者提出的Darknet-19网络包含19个卷积层和5个max pooling层，要比YOLOv1中使用的GoogleNet的参数要少。darknet-19的结构示意图如下。 
![](https://pic4.zhimg.com/v2-734a2973fc8abd8dea3277bef04b6533_b.jpg)

最终展示一下YOLOv2在darknet-13下的整体网络结构图。 
![](https://pic2.zhimg.com/v2-27178c83ee6c1abf6ef2481b5cc3629d_b.jpg)

### 2.网络训练

和上一篇介绍YOLOv1中的方式类似，介绍网络训练过程主要就是介绍标签和损失函数。而YOLOv2网络的标签和YOLOv1中的基本一致，所以接下来就主要介绍下YOLOv2中的损失函数。

如下代码为YOLOv2的caffe实现中最后部分的代码。

```
layer {
  name: "conv22"
  type: "Convolution"
  bottom: "scale21"
  top: "conv22"
  convolution_param {
    num_output: 425
    kernel_size: 1
    pad: 0
    stride: 1
  }
}
layer {
  name: "region1"
  type: "RegionLoss"
  bottom: "conv22"
  bottom: "label"
  top: "region1"
  region_loss_param {
    side: 13
    num_class: 80
    coords: 4
    num: 5
  }
}
```

如下图所示，上述代码中在损失函数前面一层，输出的滤波器个数425，而特征图的大小为13*13.其中13*13一共有169个cell，而425代表的含义如下，也就是一共5个anchor，且每个anchor会预测（4+1+80）个值，所以维度综述为5*85=425。 
![](https://pic3.zhimg.com/v2-9c3841ff1d7ec3e98b0d2bb62fdd00e2_b.jpg)

接下来结合YOLOv2官方的cfg文件中的region层参数和源代码，来解释后续的region层的功能。YOLOv2的cfg文件的region层部分如下：

```
[region]
anchors = 1.08,1.19,  3.42,4.41,  6.63,11.38,  9.42,5.11,  16.62,10.52
bias_match=1  
rescore=1  
classes=20
coords=4
num=5

//使用softmax分类，以及数据抖动
softmax=1   
jitter=.2

//四个权重
object_scale=5
noobject_scale=1
class_scale=1
coord_scale=1

absolute=1
thresh = .6  
//thresh：决定是否需要计算IOU误差的参数，大于thresh，IOU误差不会夹在cost function中 
random=1  
//random：如果为1每次迭代图片大小随机从320到608，步长为32，如果为0，每次训练大小与输入大小一致
bias_match功能：
若该值为1：计算best iou时，使用anchors相对于truth的宽高
若该值为0：计算best iou时，使用anchors的绝对宽高

for(n = 0; n < l.n; ++n){ // 遍历对应的cell预测出的n个anchor
    // 即通过该cell对应的anchors与truth的iou来判断使用哪一个anchor产生的predict来回归
    int box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
    box pred = get_region_box(l.output, l.biases, n, box_index, i, j, l.w, l.h, l.w*l.h);
    // 预测box，归一化的值
    //下面这几句是将truth与anchor中心对齐后，计算anchor与truch的iou

    //*********************bias_match功能**************************
    if(l.bias_match){ 
        pred.w = l.biases[2*n]/l.w; // 因为是和anchor比较，所以直接使用anchor的相对大小
        pred.h = l.biases[2*n+1]/l.h;
    }
    //***********************************************

    //printf("pred: (%f, %f) %f x %f\n", pred.x, pred.y, pred.w, pred.h);
    pred.x = 0;
    pred.y = 0;
    float iou = box_iou(pred, truth_shift);
    if (iou > best_iou){
        best_iou = iou;
        best_n = n;
    // 最优iou对应的anchor索引，然后使用该anchor预测的predict box计算与真实box的误差
    }
}
rescore功能：
若该值为1：计算best iou时，使用anchors相对于truth的宽高
若该值为0：计算best iou时，使用anchors的绝对宽高

l.outputs = h*w*n*(classes + coords + 1); //输出tensor的存储空间大小 13*13*5*(80+4+1)
l.output = calloc(batch*l.outputs, sizeof(float));// 批量输出tensor的存储空间
l.delta = calloc(batch*l.outputs, sizeof(float)); // 批量梯度

 l.delta[obj_index] = l.noobject_scale * (1- l.output[obj_index]);//有目标时的损失

if (l.rescore) { //定义了rescore表示同时对confidence score进行回归
    l.delta[obj_index] = l.object_scale * (iou - l.output[obj_index]); 
    }
if(l.background){//不执行
    l.delta[obj_index] = l.object_scale * (0 - l.output[obj_index]);
    }
```

最终的损失函数：公式中的h乘以w表示最后特征图的大小，也就是cell的个数（比如特征图大小为13*13，那么最终有169个cell）而n表示的是每个cell中anchor的个数，本例中为5。所以下面的损失函数，和YOLOv1中的损失函数基本类似。主要的区别有两点：
- 第一点：在计算类概率误差时，YOLOv1中仅对每个cell计算；而在YOLOv2中对每一个anchor box都会计算类概率误差。
- 第二点：YOLOv1中使用w和h的开方缓和box尺寸不平衡问题，而在YOLOv2中则通过赋值一个和w，h相关的权重函数达到该目的。
- 第三点：在源码中，在12800样本之前计算未预测到target的anchors的梯度，且将系数强制为0.01. 
![](https://pic3.zhimg.com/v2-ab5016e3aac2e719641288513f445846_b.jpg)

