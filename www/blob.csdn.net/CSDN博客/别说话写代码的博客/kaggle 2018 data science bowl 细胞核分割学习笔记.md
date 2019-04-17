# kaggle 2018 data science bowl 细胞核分割学习笔记 - 别说话写代码的博客 - CSDN博客





2018年10月20日 14:48:14[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：657








# 一、 获奖者解决方案

## 1. [第一名解决方案](https://www.kaggle.com/c/data-science-bowl-2018/discussion/54741#319142)（Unet 0.631）

### 主要的贡献
- targets: 预测touching borders，将问题作为instance分割
- loss function：组合交叉熵跟soft dice loss，避免pixel imbalance问题

> - binary_crossentropy有类平衡问题，每个像素作为单独的一个来考虑。This makes predictions a bit fuzzy.
- soft dice (and Jaccard) 对图像中的所有像素点计算，因此预测结果有较好的shapes并且not fuzzy。 不足是它们太confident了，probability is close to 0 or 1 even for wrong pixels.
- 两者的组合是为了克服它们的不足。最简单的组合是：`loss = binary_crossentropy + (1 - dice)`; 也可以根据自己的数据加权： `loss = w1 * binary_crossentropy + w2 * (1 - dice)`

- very deep encoder-decoder architectures that also achieve state-of-the-art results in other binary segmentation problems (SpaceNet, Inria and others)
- 后处理技巧：分水岭，morphological features and second level model with Gradient Boosted Trees
- 基于任务的数据增广（Clahe, Sharpen, Emboss，高斯噪声，color2gray，模糊，中值模糊，高斯模糊，调整对比度和亮度，随机scale,roation and flips）。比较有效的两个增广方法

> - Channel shuffle - I guess this one was very important due to the nature of the data
- Nucleus copying on images. That created a lot of overlapping nuclei. It seemed to help networks to learn better borders for overlapping nuclei.
**Whenever we reduced augmentations we got better validation results and worse score on public leaderboard.**


## **Unet or Mask-RCNN？** 我们也参加了之前的一些分割比赛，选择清晰了——Unet!

## 实验历程
- 首先尝试最简单的方法并增加watershed line for binary masks(i.e. modified GT masks to always have gaps between nuclei)，这个方法在public LB得到0.5分。

> 
不是用的plain Unets，而是**预训练**的非常深的编码器（Resnet50不够）。That makes a huge difference when you don't have enough data, which is clearly the case。


**使用预训练模型（只用来初始化，然后端到端训练）非常重要，如果直接训练一个plain Unet，结果会差很多。**

> 
Encoders were initialized with pretrained weights from ImageNet. Then we trained models end to end. From my experience with a frozen encoder it is usually not possible to achieve good segmentation results even on datasets that are more or less similar to ImageNet.


> 
分水岭后处理：取两个阈值，high for seeds, low for masks (something like 0.6 and 0.3 for a binary mask).

- 然后增加第二个带contours的通道，contours的宽度依赖于核的大小，这些masks + 简单的分水岭后处理得到0.525分，不是很大的突破，但是a hint to the right directino.

> 
In my initial experiments, i actually tried Unet but i gave it up because i have problem in the post processing of unet outputs. Watershed is very sensitive to input marker and energy function (distance transform). That is why I switch to mask-rcnn.

@topcoders solution use unet to learn the "border of nuclei", which is made from watershed line of the ground truth. In this way they can control the accuracy of the watershed post processing.


> 
create ground truth contours from the individual binary masks, then combine them in a single image channel. You can then use these as ground truth along with binary masks to teach U-Net. To output a second channel just increase the number of kernels in the final U-Net layer from 1 to 2, and make sure your inputs are consistent with this format.


> 
contours是所有细胞核的轮廓吗？它的宽度跟细胞核大小怎么个相关法？具体怎么得到。A better approach is to use labels, dilation, watershed with watershed_line=True etc. Watershed line will be a border between the nuclei.

- 观察结果发现，网络很容易的预测出non ambiguous地方的contours，因此我们决定仅仅预测细胞之间的边界。这很容易的让我们得到0.55分，只用了一个网络的结果+分水岭后处理。
- 如果我们在一个通道有一个full mask，另一个通道有a border，有时候用于分水岭方法的种子点不足够好。一个更好的方法是change nuclei masks and make pixels empty on the borders. This also allows to use softmax as the target activation instead of sigmoid. 这可以较好的分离细胞核，但是actually decreases MaP because of high thresholds for IoU. 为解决这个问题，我们train additional networks trained on full masks and combined the results in the postprocessing step.
**最后的方法：**
- 2 channels full masks i.e. (mask, border)
- 2 channels masks for networks with sigmoid activation i.e. (mask - border, border) or 3 channels masks for networks with softmax activation i.e. (mask - border, border , 1 - mask - border)

### 网络架构

使用Unet类型的编解码结构，编码部分在ImageNet上预训练。令人惊讶的是，类似于VGG16这样的简单编码器在这个数据集上不work，这让我们决定go deeper，As a result - **top performing encoders in this competition were : DPN-92, Resnet-152, InceptionResnetV2, Resnet101.**

### 2nd Level model / Postprocessing

在预测出来的nucleus candidates上面训练LightGBM模型。。。看不懂啥意思

lgb just trained as regressor to predict IoU. So, we have nucleus candidate after Neural Networks prediction. We extracting some features and predicting IoU.

在公开LB上，这个带自适应阈值和FP rejection的后处理提升了~15。

### 集成

后处理前平均所有预测出来的mask。

### 训练参数
- RGB channels
- Random Crops: 256x256
- Random scale: [0.55, 1.45]
- Batch Size: 16
- optimizer: Adam
- learning rate: initial 1e-4 with decay (we had different LR policies, but mostly small LR no more than 1e-4)
- preprocessing: same as on ImageNet depending on network

### Test Time Augmentations (TTA)

Standard Flips/Rotations(0,90,180,270).

### 讨论区集锦
- 是否用dropout

> 
普通的dropout通常对卷积层是有害的，但从经验来讲，SpatialDropout2D可以用于分割任务中并得到好一些的结果，I used SpatialDropout2D just before the classification layer。

- 如何让编解码过程中维数匹配

> 
I simply changed encoders and replaced `valid padding` with `same padding`. 常用的解码器有两种：1） U-Net like with standard upsampling-conv approach 2） a custom FPN like decoder 。两者的性能相当

- [Applying Deep Watershed Transform to Kaggle Data Science Bowl 2018](https://spark-in.me/post/playing-with-dwt-and-ds-bowl-2018)

	分水岭方法主要围绕着以下三个问题

i. 如何选择种子点

ii. 如何决定分水岭边界

iii. 如何决定地形高度

[Deep Watershed Transform](https://arxiv.org/abs/1611.08303)（DWT）方法帮助我们解决了上述一些问题。主要的思想是让CNN学习两件事情：**unit vectors pointing to (against) boundary** 和 **energy (mountain height) levels**。实践中如果仅仅用分水岭，结果很可能会过分割（因为有噪声干扰，所以建立的landscape会有很多局部极小），这就是DWT的思想，让CNN学习mountain landscape。原始作者用2个VGG-like CNN来学习
- The watershed energy itself (energy, elevation - whatever you may call it);
- The unit vectors points to (away from) the borders - to help CNN learn the boundaries);

实践中不一定要用一个分离的CNN，仅仅用Unet并且 feed several masks to it（Unet）:
- The merged nuclei masks (gt_mask);
- Several thinned / eroded merged nuclei masks (3 levels of eroson - 1 pixel, 3 pixels, 5 or 7 pixels);
- Centers of the nuclei (did not help in my case);
- Unit vectors (helped a little bit, locally);
- Borders (helped a bit, locally);

对我来说， [最好的后处理方法](https://github.com/snakers4/ds_bowl_2018/blob/master/src/utils/watershed.py)(energy_baseline function) 很简单:
- Sum the predicted mask and 3 levels of eroded masks;
- Apply a threshold of 0.4 to produce watershed seeds;
- Flood the original thresholded masks from these markers;
- Use distance transform as "landscape height" measure;

个人看来，我想把解决细胞核分割问题的方法分成4类：

i. Unet-like 架构+基于[分水岭变换](http://cmm.ensmp.fr/~beucher/wtshed.html)的后处理。

ii. Recurrent架构。I found only this more or less relevant [paper](http://arxiv.org/abs/1712.00617)(even followed by code [release)](https://imatge-upc.github.io/rsis/);

iii. 基于proposal的模型，比如Mask-RCNN。

## 2. [第三名解决方案]()（Mask-RCNN，0.614）

## 3. [第四名解决方案](https://www.kaggle.com/c/data-science-bowl-2018/discussion/55118#321050)（Unet，0.610）

我们队伍评测了Unet和Mask-RCNN，结果是Unet效果显著的好于mask-Rcnn。

受[Deep Watershed Transform paper](https://arxiv.org/abs/1611.08303)启发，对每个pixel，我们预测了the x,y components of vector pointing from the instance border和mask，watershed levels and nuclei centers using the second connected UNet.

For unet encoders we used Conv2d - BN - Relu - Conv2d - Relu Decoders: Upsample/concatenate - Conv2d - Relu

## 4. [第五名解决方案](https://www.kaggle.com/c/data-science-bowl-2018/discussion/56326)（Mask-RCNN，0.609）
- [Mask-RCNN keras, tensorflow 代码](https://github.com/matterport/Mask_RCNN)

# 二、优秀教程
- [Keras U-Net starter tutorial](https://www.kaggle.com/keegil/keras-u-net-starter-lb-0-277?scriptVersionId=2164855) by Kjetil Amdal-Saevik with 739 upvotes

### Get the data

首先导入所有图像和对应的mask，下采样训练集图像和测试集图像来保持things light and manageable. 但是我们应该对测试集的原始图像大小记录下来，从而将predicted masks上采样到原始大小以及创建正确的run-length encoding。

> 
There are definitely better ways to handle this, but it works fine for now! 更好的处理方法有哪些呢？？


### 搭建并训练神经网络

搭建Unet，如果那小数据实验的话建议使用checkpointing and early stopping.

### 进行预测

在训练集、验证集（合理性检查）以及测试集上做预测。注意：如果使用了early stopping 和checkpointing，记得加载保存的性能最好的模型。

### 编码结果并提交

最终的结果是0.277 LB
- [Teaching notebook for total imaging newbies tutorial](https://www.kaggle.com/stkbailey/teaching-notebook-for-total-imaging-newbies) by Stephen Bailey with 354 upvotes

	新手入门教程，教如何分析图像，并不是要得高分。

### A. 处理颜色

数据集中的图像可能是RGB、RGBA以及灰度图像。为了简化处理，通过`rgb2gray`函数都转到灰度图像。

### B. 去除背景

最简单的假设是图像中只用前景和背景两类，它们灰度服从bimodal distribution。如果能发现最好的分离阈值，我们可以mask out背景数据，仅仅处理剩下的前景目标。可以尝试采用大津阈值方法，因为它将图像建模为一个bimodal分布并且找到最优的分离值。

```
thresh_val = threshold_otsu(im_gray)
mask = np.where(im_gray > thresh_val, 1, 0)
```

### C. Deriving individual masks for each object

我们需要对每个细胞核得到separate mask，遍历mask中所有的目标，通过`ndimage.label`给每个目标一个label。

```
from scipy import ndimage
labels, nlabels = ndimage.label(mask)
label_array = []
for label_num in range(1, nlabels+1):
     label_mask = np.where(labels== label_num, 1, 0)
     label_arrays.append(label_mask)

print('共计 {} 个分离的components 被检测到'.format(nlabels))
```

这样简单的处理有两个很直接的问题
- 有一些像素点是孤立的（右上角）
- 细胞重叠（中间靠右）

	[图片上传失败...(image-42cef7-1530348310622)]

利用`ndimage.find_objects`, 可以迭代的遍历mask，zooming in单个的细胞核并施加额外的后处理。

```
for label_ind, label_coords in enumerate(ndimage.find_objects(labels)):
    cell = im_gray[label_coords]
    
    # Check if the label size is too small
    if np.product(cell.shape) < 10: 
        print('Label {} is too small! Setting to 0.'.format(label_ind))
        mask = np.where(labels==label_ind+1, 0, mask)
```

对于adjacent 细胞(不同于重叠细胞)，One thing we can do here is to see whether we can shrink the mask to "open up" the differences between the cells. This is called mask erosion. We can then re-dilate it to to recover the original proportions.

```
# Get the object indices, and perform a binary opening procedure
two_cell_indices = ndimage.find_objects(labels)[1]
cell_mask = mask[two_cell_indices]
cell_mask_opened = ndimage.binary_opening(cell_mask, iterations=8)
```

> 
本次比赛的目标是细胞核检测，因此需要提交单个细胞核的结果。

- [Step-By-Step Explanation of Scoring Metric](https://www.kaggle.com/stkbailey/step-by-step-explanation-of-scoring-metric)
- [Nuclei overview to submission tutorial](https://www.kaggle.com/kmader/nuclei-overview-to-submission) by Kevin Mader with 240 upvotes

### A. the preprocessing steps to load the data

### B. a quick visualization of the color-space

### C. training a simple CNN

搭建一个简单的CNN网络，用扩张卷积

```
from keras.models import Sequential
from keras.layers import BatchNormalization, Conv2D, UpSampling2D, Lambda
simple_cnn = Sequential()
simple_cnn.add(BatchNormalization(input_shape = (None, None, IMG_CHANNELS), 
                                  name = 'NormalizeInput'))
simple_cnn.add(Conv2D(8, kernel_size = (3,3), padding = 'same'))
simple_cnn.add(Conv2D(8, kernel_size = (3,3), padding = 'same'))
# use dilations to get a slightly larger field of view
simple_cnn.add(Conv2D(16, kernel_size = (3,3), dilation_rate = 2, padding = 'same'))
simple_cnn.add(Conv2D(16, kernel_size = (3,3), dilation_rate = 2, padding = 'same'))
simple_cnn.add(Conv2D(32, kernel_size = (3,3), dilation_rate = 3, padding = 'same'))

# the final processing
simple_cnn.add(Conv2D(16, kernel_size = (1,1), padding = 'same'))
simple_cnn.add(Conv2D(1, kernel_size = (1,1), padding = 'same', activation = 'sigmoid'))
simple_cnn.summary()
```

**Loss function：Dice**

```
from keras import backend as K
smooth = 1.
def dice_coef(y_true, y_pred):
    y_true_f = K.flatten(y_true)
    y_pred_f = K.flatten(y_pred)
    intersection = K.sum(y_true_f * y_pred_f)
    return (2. * intersection + smooth) / (K.sum(y_true_f) + K.sum(y_pred_f) + smooth)
def dice_coef_loss(y_true, y_pred):
    return -dice_coef(y_true, y_pred)
simple_cnn.compile(optimizer = 'adam', 
                   loss = dice_coef_loss, 
                   metrics = [dice_coef, 'acc', 'mse'])
```

**训练网络**

```
def simple_gen():
    while True:
        for _, c_row in train_img_df.iterrows():
            yield np.expand_dims(c_row['images'],0), np.expand_dims(np.expand_dims(c_row['masks'],-1),0)

simple_cnn.fit_generator(simple_gen(), 
                         steps_per_epoch=train_img_df.shape[0],
                        epochs = 3)
```

### D. applying the model to the test data
- [用RCNN分割的Keras 教程](https://www.kaggle.com/kmader/keras-rcnn-based-overview-wip)

forked from [Nuclei Overview to Submission](https://www.kaggle.com/kmader/nuclei-overview-to-submission) by [Kevin Mader](https://www.kaggle.com/kmader) ([+0/–0](https://www.kaggle.com/kmader/keras-rcnn-based-overview-wip/versions#base=2165003&new=2343113))

## 优质讨论集锦
- 
[UNet Vs Mask-rcnn, which is better?](https://www.kaggle.com/c/data-science-bowl-2018/discussion/54700)

	Unet!

- 
[mask-rcnn in pytorch from scratch](https://www.kaggle.com/c/data-science-bowl-2018/discussion/49692)

- 
是否需要padding？

- 原始Unet作者使用`VALID` padding，不是`SAME`;
- 比赛第一名[Selim Seferbekov](https://www.kaggle.com/selimsef)：认为VALID要比SAME padding弱很多。实际上，Unet的贡献不是no padding和cropping，而是**skip connection**。所以我认为VALID仅仅是作者的preference，不是thorough study的结果。使用zero padding有如下好处：保留空间分辨率，更容易的让编解码架构使用，可以被用于很深的网络以及提升了borders分割的性能。cs231n 助教Karpathy讲义中也提到，如果每次卷积都执行VALID padding，volume的大小会减小，进而导致边界处的信息被“washed away”的很快。
- Karphathy在twitter上也说到："Zero padding in ConvNets is highly suspicious/wrong. Input distribution stats are off on each border differently yet params are all shared"。Reflection or periodic or symetric padding has the benefit to not introduce new freqencies in the image signal. （Unet作者就是用的reflection 预处理）

## 其它
- Heng CherKeng 分享了很多有价值的insight
- Waleed 写了非常好的mask_rnn 代码
- [MPWare 队](https://www.kaggle.com/mpware/kernels)
- [第41名Unet代码](https://github.com/ildoonet/data-science-bowl-2018)

> 
原始Unet作者使用`VALID` padding，不是`SAME`; 使用加权的cross entropy。对细胞间small gaps增加更多的权重。我们在预处理阶段对细胞做了腐蚀，确保每个细胞是分离开的，然后对最终的结果做膨胀。使用scale-augmentation使得网络对各种大小的细胞都有预测能力。没有任何集成，最终得分是0.504.
[kaggle discussion](https://www.kaggle.com/c/data-science-bowl-2018/discussion/54426)

- [Kaggle求生：亚马逊热带雨林篇](https://www.leiphone.com/news/201707/LAh9ISNjgzni2Zat.html)




作者：EdwardMa

链接：https://www.jianshu.com/p/4b6ccb52af2e

來源：简书

简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。



