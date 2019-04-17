# 语义分割 各种loss实现 python - 别说话写代码的博客 - CSDN博客





2019年02月19日 11:43:38[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：346








文章转自：[https://blog.csdn.net/wangdongwei0/article/details/84576044](https://blog.csdn.net/wangdongwei0/article/details/84576044)，侵删

## 前言：

在参加Kaggle的比赛中，有时遇到的分割任务是那种背景所占比例很大，但是物体所占比例很小的那种严重不平衡的数据集，这时需要谨慎的挑选loss函数。

## Loss：

### 1.Log loss 

log loss其实就是TensorFlow中的 [tf.losses.sigmoid_cross_entropy](https://tensorflow.google.cn/api_docs/python/tf/losses/sigmoid_cross_entropy) 或者Keras的 [keras.losses.binary_crossentropy(y_true, y_pred)](https://keras.io/losses/#binary_crossentropy)

![](https://img-blog.csdnimg.cn/2019021911270886.png)

乍看上去难以理解loss函数的意义，也就是说不明白为什么这个函数可以当做损失函数。

其实我们可以对上面的公式进行拆分：

![](https://img-blog.csdnimg.cn/20190219112732630.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

### 2、Dice Loss

首先定义两个轮廓区域的相似程度，用A、B表示两个轮廓区域所包含的点集，定义为：

![](https://img-blog.csdnimg.cn/20190219112800163.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)



```python
def dice_coef(y_true, y_pred, smooth=1):
    intersection = K.sum(y_true * y_pred, axis=[1,2,3])
    union = K.sum(y_true, axis=[1,2,3]) + K.sum(y_pred, axis=[1,2,3])
    return K.mean( (2. * intersection + smooth) / (union + smooth), axis=0)
def dice_coef_loss(y_true, y_pred):
	1 - dice_coef(y_true, y_pred, smooth=1)
```

多分类Dice loss

```python
# y_true and y_pred should be one-hot
# y_true.shape = (None,Width,Height,Channel)
# y_pred.shape = (None,Width,Height,Channel)
def dice_coef(y_true, y_pred, smooth=1):
    mean_loss = 0;
    for i in range(y_pred.shape(-1)):
        intersection = K.sum(y_true[:,:,:,i] * y_pred[:,:,:,i], axis=[1,2,3])
        union = K.sum(y_true[:,:,:,i], axis=[1,2,3]) + K.sum(y_pred[:,:,:,i], axis=[1,2,3])
    mean_loss += (2. * intersection + smooth) / (union + smooth)
    return K.mean(mean_loss, axis=0)
def dice_coef_loss(y_true, y_pred):
	1 - dice_coef(y_true, y_pred, smooth=1)
```

### 3、FOCAL LOSS

![](https://img-blog.csdnimg.cn/2019021911313437.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

```python
def focal_loss(y_true, y_pred):
    gamma=0.75
    alpha=0.25
    pt_1 = tf.where(tf.equal(y_true, 1), y_pred, tf.ones_like(y_pred))
    pt_0 = tf.where(tf.equal(y_true, 0), y_pred, tf.zeros_like(y_pred))
 
    pt_1 = K.clip(pt_1, 1e-3, .999)
    pt_0 = K.clip(pt_0, 1e-3, .999)
 
    return -K.sum(alpha * K.pow(1. - pt_1, gamma) * K.log(pt_1))-K.sum((1-alpha) * K.pow( pt_0, gamma) * K.log(1. - pt_0))
```

 在实际应用中，我们可以使用多种loss函数相互组合得到更好的结果

### **4、Focal Loss + DICE LOSS**

Focal Loss 和 Dice Loss的结合需要注意把两者缩放至相同的数量级，Focal Loss非常大，可以成千上百，但是Dice Loss一般小于1，所以得想办法结合两者，我觉的有一个比较好的办法，参考Kaggle的一个项目：

使用-log放大Dice Loss，使用alpha缩小Focal Loss


```python
def mixedLoss(y_ture,y_pred,alpha)
    return alpha * focal_loss(y_ture,y_pred) - K.log(diceA_loss(y_true,y_pred))
```

### 5.BCE + DICE LOSS

```python
def bce_logdice_loss(y_true, y_pred):
    return binary_crossentropy(y_true, y_pred) - K.log(1. - dice_loss(y_true, y_pred))
```

### 6、WEIGHTED BCE LOSS

相比dice loss增加了参数 weight，没看明白怎么个意思，我以为weight会是个数组呢，包含w1和w2，分别与y_pred和y_true相乘呢....待改日查查论文

```python
def weighted_dice_loss(y_true, y_pred, weight):
    smooth = 1.
    w, m1, m2 = weight, y_true, y_pred
    intersection = (m1 * m2)
    score = (2. * K.sum(w * intersection) + smooth) / (K.sum(w * m1) + K.sum(w * m2) + smooth)
    loss = 1. - K.sum(score)
    return loss
```

### 7、WEIGHTED BCE DICE LOSS

```python
def weighted_bce_dice_loss(y_true, y_pred):
    y_true = K.cast(y_true, 'float32')
    y_pred = K.cast(y_pred, 'float32')
    # if we want to get same size of output, kernel size must be odd
    averaged_mask = K.pool2d(
            y_true, pool_size=(50, 50), strides=(1, 1), padding='same', pool_mode='avg')
    weight = K.ones_like(averaged_mask)
    w0 = K.sum(weight)
    weight = 5. * K.exp(-5. * K.abs(averaged_mask - 0.5))
    w1 = K.sum(weight)
    weight *= (w0 / w1)
    loss = weighted_bce_loss(y_true, y_pred, weight) + dice_loss(y_true, y_pred)
    return loss
```

### 8、 Mean IOU

mean IOU字面理解就是平均的IOU，不过计算时需要设置多个阈值(0.5, 1.0, 0.05)，是指在阈值范围内分别计算IOU，取均值。

```python
def mean_iou(y_true, y_pred):
    prec = []
    for t in np.arange(0.5, 1.0, 0.05):
        y_pred_ = tf.to_int32(y_pred > t)
        score, up_opt = tf.metrics.mean_iou(y_true, y_pred_, 2)
        K.get_session().run(tf.local_variables_initializer())
        with tf.control_dependencies([up_opt]):
            score = tf.identity(score)
        prec.append(score)
    return K.mean(K.stack(prec), axis=0)
```

2019/1/16补充：

一般在分割模型中我们有时会用intersection over union去衡量模型的表现，按照IOU的定义，比如对predicted instance和actual instance的IOU大于0.5算一个positive，在这个基础上再做一些F1,F2之类其他的更宏观的metric。

所以如果能优化IOU就能提高分数啦，那么怎么优化呢？

一般直接使用BCE去训练，但是优化BCE并不等于优化IOU，可以参考 [http://cn.arxiv.org/pdf/1705.08790v2](http://cn.arxiv.org/pdf/1705.08790v2)，直观上来说一个minibatch中的没有pixel的权重其实是不一样的。两张图片，一张正样本有1000个pixels，另一张只有4个，那么第二张1个pixel带来的IOU得损失就能顶的上第一张中250pixel的损失。

如果直接使用IOU作为Loss Function，也不是最好的，因为训练过程是不稳定的。一个模型从坏到好，我们希望监督它的loss的过渡是平滑的。

所以推荐使用[Lovasz-Softmax loss](http://cn.arxiv.org/pdf/1705.08790v2)，效果不错，github上有源码。




