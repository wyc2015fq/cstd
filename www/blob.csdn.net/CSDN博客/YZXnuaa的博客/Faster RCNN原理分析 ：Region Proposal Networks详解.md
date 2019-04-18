# Faster RCNN原理分析 ：Region Proposal Networks详解 - YZXnuaa的博客 - CSDN博客
2018年01月31日 19:23:34[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：3671
缩进Region Proposal Networks是Faster RCNN出新提出来的proposal生成网络。其替代了之前RCNN和Fast RCNN中的selective search方法，将所有内容整合在一个网络中，大大提高了检测速度（语文水平差，所以历史科普请看其他文章T_T）。
缩进在正文前，还要多解释几句基础知识，已经懂的看官老爷跳过就好。
- 对于单通道图像+单卷积核做卷积，上一篇文章已经介绍了（参考上文图4）；
- 对于多通道图像+多卷积核做卷积，计算方式如下：
![](https://img-blog.csdn.net/20170318112222765)
*图1 多通道+多卷积核做卷积示意图（摘自Theano教程）*
缩进如图2，输入图像layer m-1有4个通道，同时有2个卷积核w1和w2。对于卷积核w1，先在输入图像4个通道分别作卷积，再将4个通道结果加起来得到w1的卷积输出；卷积核w2类似。所以对于某个卷积层，无论输入图像有多少个通道，输出图像通道数总是等于卷积核数量！特殊的，对于多通道做1x1卷积，其实就是将输入图像于每个通道乘以卷积系数后加在一起。这样做的结果就是把原图像中本来各个独立的通道“联通”在了一起。
## （一）anchors
缩进提到RPN网络，就不能不说anchors。所谓anchors，实际上就是一组由rpn/generate_anchors.py生成的矩形。直接运行generate_anchors.py得到以下输出：
```python
[[ -84.  -40.   99.   55.]
 [-176.  -88.  191.  103.]
 [-360. -184.  375.  199.]
 [ -56.  -56.   71.   71.]
 [-120. -120.  135.  135.]
 [-248. -248.  263.  263.]
 [ -36.  -80.   51.   95.]
 [ -80. -168.   95.  183.]
 [-168. -344.  183.  359.]]
```
其中每行的4个值[x1,y1,x2,y2]代表矩形左上和右下角点坐标。9个矩形共有3种形状，长宽比为大约为：width:height = [1:1, 1:2, 2:1]三种，如图2。实际上通过anchors就引入了检测中常用到的多尺度方法。
![](https://img-blog.csdn.net/20170322103823615)
*图2 anchors示意图*
注：关于上面的anchors size，其实是根据检测图像设置的。在python demo中，会把任意大小的输入图像reshape成800x600（即上文中提到的M=800，N=600）。再回头来看anchors的大小，anchors中长宽1:2中最大为352x704，长宽2:1中最大736x384，基本是cover了800x600的各个尺度和形状。
那么这9个anchors是做什么的呢？借用Faster RCNN论文中的原图，如图3，遍历Conv layers计算获得的feature maps，为每一个点都配备这9种anchors作为初始的检测框。这样做获得检测框很不准确，不用担心，后面还有2次bounding box regression可以修正检测框位置。
![](https://img-blog.csdn.net/20170322103903632)
*图3*
解释一下上面这张图的数字。
- 在原文中使用的是ZF model中，其Conv Layers中最后的conv5层num_output=256，对应生成256张特征图，所以相当于feature map每个点都是256-d
- 由于输入图像M=800，N=600，且Conv Layers做了4次Pooling，feature map的长宽为[M/16, N/16]=[50, 38]
- 在conv5之后，做了rpn_conv/3x3卷积，num_output=256，相当于每个点使用了周围3x3的空间信息，同时256-d不变，如图3红框，同时对应图4中的红框中的3x3卷积
- 假设一共有k个anchor，而每个anhcor要分foreground和background，所以cls=2k scores；而每个anchor都有[x, y, w, h]对应4个偏移量，所以reg=4k coordinates
![](https://img-blog.csdn.net/20170328113414055)
*图4 *
注意，在本文讲解中使用的VGG conv5 num_output=512，所以是512d，其他类似.....
## （二）softmax判定foreground与background
缩进在上一篇文章中提到：一副MxN大小的矩阵送入Faster RCNN网络后，到RPN网络变为(M/16)x(N/16)，不妨设W=M/16，H=N/16。在进入reshape与softmax之前，先做了1x1卷积，如图5：
![](https://img-blog.csdn.net/20170319220636315)
*图5 RPN中判定fg/bg网络结构*
该1x1卷积的caffe prototxt定义如下：
```cpp
layer {
  name: "rpn_cls_score"
  type: "Convolution"
  bottom: "rpn/output"
  top: "rpn_cls_score"
  convolution_param {
    num_output: 18   # 2(bg/fg) * 9(anchors)
    kernel_size: 1 pad: 0 stride: 1
  }
}
```
可以看到其num_output=18，也就是经过该卷积的输出图像为WxHx18大小（注意文章开头提到的卷积计算方式）。这也就刚好对应了feature maps每一个点都有9个anchors，同时每个anchors又有可能是foreground和background，所有这些信息都保存WxHx(9x2)大小的矩阵。为何这样做？后面接softmax分类获得foreground anchors，也就相当于初步提取了检测目标候选区域box（一般认为目标在foreground
 anchors中）。
缩进那么为何要在softmax前后都接一个reshape layer？其实只是为了便于softmax分类，至于具体原因这就要从caffe的实现形式说起了。在caffe基本数据结构blob中以如下形式保存数据：
**blob=[batch_size, channel，height，width]**
对应至上面的保存bg/fg anchors的矩阵，其在caffe blob中的存储形式为[1, 18, H, W]。而在softmax分类时需要进行fg/bg二分类，所以reshape layer会将其变为[1, 2, 9*H, W]大小，即单独“腾空”出来一个维度以便softmax分类，之后再reshape回复原状。贴一段caffe softmax_loss_layer.cpp的reshape函数的解释，非常精辟：
```cpp
"Number of labels must match number of predictions; "
"e.g., if softmax axis == 1 and prediction shape is (N, C, H, W), "
"label count (number of labels) must be N*H*W, "
"with integer values in {0, 1, ..., C-1}.";
```
综上所述，RPN网络中利用anchors和softmax初步提取出foreground anchors作为候选区域。
## （三）bounding box regression原理
缩进先介绍bounding box regression数学模型及原理。如图5所示绿色框为飞机的Ground Truth(GT)，红色为提取的foreground anchors，那么即便红色的框被分类器识别为飞机，但是由于红色的框定位不准，这张图相当于没有正确的检测出飞机。所以我们希望采用一种方法对红色的框进行微调，使得foreground anchors和GT更加接近。
![](https://img-blog.csdn.net/20170321000420426)
*图6*
缩进对于窗口一般使用四维向量(x, y, w, h)表示，分别表示窗口的中心点坐标和宽高。对于图 7，红色的框A代表原始的Foreground Anchors，绿色的框G代表目标的GT，我们的目标是寻找一种关系，使得输入原始的anchor A经过映射得到一个跟真实窗口G更接近的回归窗口G'，即：给定A=(Ax,
 Ay, Aw, Ah)，寻找一种映射f，使得f(Ax, Ay, Aw, Ah)=(G'x,
 G'y, G'w, G'h)，其中(G'x, G'y, G'w, G'h)≈(Gx,
 Gy, Gw, Gh)。
![](https://img-blog.csdn.net/20170321221228658)
*图7*
那么经过何种变换才能从图6中的A变为G'呢？ 比较简单的思路就是:
缩进 1. 先做平移
![](https://img-blog.csdn.net/20170322104630982)
缩进 2. 再做缩放
![](https://img-blog.csdn.net/20170322104634390)
缩进观察上面4个公式发现，需要学习的是dx(A)，dy(A)，dw(A)，dh(A)这四个变换。当输入的anchor与GT相差较小时，可以认为这种变换是一种线性变换，
 那么就可以用线性回归来建模对窗口进行微调（注意，只有当anchors和GT比较接近时，才能使用线性回归模型，否则就是复杂的非线性问题了）。对应于Faster RCNN原文，平移量(tx, ty)与尺度因子(tw, th)如下：
![](https://img-blog.csdn.net/20170322104637857)
缩进接下来的问题就是如何通过线性回归获得dx(A)，dy(A)，dw(A)，dh(A)了。线性回归就是给定输入的特征向量X,
 学习一组参数W, 使得经过线性回归后的值跟真实值Y（即GT）非常接近，即Y=WX。对于该问题，输入X是一张经过num_output=1的1x1卷积获得的feature map，定义为Φ；同时还有训练传入的GT，即(tx, ty,
 tw, th)。输出是dx(A)，dy(A)，dw(A)，dh(A)四个变换。那么目标函数可以表示为：
![](https://img-blog.csdn.net/20170322110627709)
其中Φ(A)是对应anchor的feature map组成的特征向量，w是需要学习的参数，d(A)是得到的预测值（*表示
 x，y，w，h，也就是每一个变换对应一个上述目标函数）。为了让预测值(tx, ty,tw,th)与真实值最小，得到损失函数：
![](https://img-blog.csdn.net/20170322113250485)
函数优化目标为：
![](https://img-blog.csdn.net/20170322113253485)
## （四）对proposals进行bounding box regression
缩进在了解bounding box regression后，再回头来看RPN网络第二条线路，如图8。
![](https://img-blog.csdn.net/20170321151019146)
*图8 RPN中的bbox reg*
先来看一看上图7中1x1卷积的caffe prototxt定义：
```cpp
layer {
  name: "rpn_bbox_pred"
  type: "Convolution"
  bottom: "rpn/output"
  top: "rpn_bbox_pred"
  convolution_param {
    num_output: 36   # 4 * 9(anchors)
    kernel_size: 1 pad: 0 stride: 1
  }
}
```
可以看到其num_output=36，即经过该卷积输出图像为PxQx36，在caffe blob存储为[1, 4x9, Q, P]。与上文中fg/bg anchors存储为[1, 18, Q, P]类似，这里相当于feature maps每个点都有9个anchors，每个anchors又都有4个用于回归的[dx(A)，dy(A)，dw(A)，dh(A)]变换量。利用上面的的计算公式即可从foreground
 anchors回归出proposals。
## （五）Proposal Layer
缩进Proposal Layer负责综合所有[dx(A)，dy(A)，dw(A)，dh(A)]变换量和foreground anchors，计算出精准的proposal，送入后续RoI
 Pooling Layer。还是先来看看Proposal Layer的caffe prototxt定义：
```cpp
layer {
  name: 'proposal'
  type: 'Python'
  bottom: 'rpn_cls_prob_reshape'
  bottom: 'rpn_bbox_pred'
  bottom: 'im_info'
  top: 'rois'
  python_param {
    module: 'rpn.proposal_layer'
    layer: 'ProposalLayer'
    param_str: "'feat_stride': 16"
  }
}
```
Proposal Layer有3个输入：fg/bg anchors分类器结果rpn_cls_prob_reshape，对应的bbox reg的[dx(A)，dy(A)，dw(A)，dh(A)]变换量rpn_bbox_pred，以及im_info；另外还有参数feat_stride=16。
缩进首先解释im_info。对于一副任意大小PxQ图像，传入Faster RCNN前首先reshape到固定MxN，im_info=[M, N, scale_factor]则保存了此次缩放的所有信息。然后经过Conv Layers，经过4次pooling变为(M/16)x(N/16)大小，其中feature_stride=16则保存了该信息。所有这些数值都是为了将proposal映射回原图而设置的，如图8，毕竟检测就是为了在原图上画一个框而已~
![](https://img-blog.csdn.net/20170323200941596)
*图8*
缩进Proposal Layer forward（caffe layer的前传函数）按照以下顺序依次处理：
- 再次生成anchors，并对所有的anchors做bbox reg位置回归（注意这里的anchors生成顺序和之前是即完全一致的）
- 按照输入的foreground softmax scores由大到小排序anchors，提取前pre_nms_topN(e.g. 6000)个anchors。即提取修正位置后的foreground anchors
- 利用feat_stride和im_info将anchors映射回原图，判断fg anchors是否大范围超过边界，剔除严重超出边界fg anchors。
- 进行nms（nonmaximum suppression，非极大值抑制）
- 再次按照nms后的foreground softmax scores由大到小排序fg anchors，提取前post_nms_topN(e.g. 300)结果作为proposal输出。
之后输出proposal=[x1, y1, x2, y2]，注意，由于在第三步中将anchors映射回原图判断是否超出边界，所以这里输出的proposal是对应MxN输入图像尺度的，这点在后续网络中有用。另外我认为，严格意义上的检测应该到此就结束了，后续部分应该属于识别了~
RPN网络结构就介绍到这里，总结起来就是：
**生成anchors -> softmax分类器提取fg anchors -> bbox reg回归fg anchors -> Proposal Layer生成proposals**
