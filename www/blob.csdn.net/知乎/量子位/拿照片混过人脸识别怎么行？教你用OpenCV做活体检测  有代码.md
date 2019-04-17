# 拿照片混过人脸识别怎么行？教你用OpenCV做活体检测 | 有代码 - 知乎
# 



> 圆栗子 发自 凹非寺
量子位 报道 | 公众号 QbitAI
![](https://pic3.zhimg.com/v2-43a53a9d85707f1b543528ae717b05c6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1040' height='500'></svg>)△ 来自虾米妈咪
小朋友用妈妈的一寸照片通过了人脸识别，打击了小度音箱的家长监督机制。

活体检测没做好。

公交车身广告上的董明珠头像，被宁波交警系统拍了照，判定成“违法闯红灯”。

活体检测没做好。

所以，**活体检测**要怎么做？

名叫Adrian Rosebrock的程序猿，写了份事无巨细的教程，从构建数据集开始，一步步教大家用AI分辨真人和照片，精细到每行代码的用途。
![](https://pic3.zhimg.com/v2-e0b94290133088cc16cc1aa1c6c599a2_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='564' height='331'></svg>)△ 川川是假的，光头是真的
这个识别方法，用到了**OpenCV**和**Keras**，打开摄像头就可以**实时检测**。

重要的是，有**源码**提供，受到了推特用户的踊跃比心。

## **活体检测，可以检测些什么？**

AI可以用哪些技巧，来区分真人和照片？

> 一是**纹理分析** (Texture Analysis) 。皮肤的纹理特征是重要的依据，给2D照片拍照，比起给3D真人拍照，会损失一些纹理。

二是**频率分析** (Frequency Analysis) 。照片脸部的频率组成，也不像真人那样丰富。

三是**可变聚焦分析** (Variable focusing Analysis) 。连拍两张照片，聚焦在不同位置，查看像素值 (Pixel Value) 的变化。

四是**启发式算法** (Heuristic-Based Algorithms) 。眼动、唇动、眨眼这些动作，照片是不会有的。

五是**光流算法** (Optical Flow Algorithms) 。在相邻两帧之间，检测物体运动的方向和幅度，查出2D和3D物体之间的差别。

……
![](https://pic4.zhimg.com/v2-4af3a54245726158209f50aa0fda1bef_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='538'></svg>)
不过这里，就把活体检测看成一个粗暴的**二分类问题**，这些复杂的分析先抛到一边。

## **自制数据集**

程序猿把问题又简化了一下：这里说的“假脸”，只是“屏幕里的人脸”而已。

现在，可以开始造数据集了。

他用手机的前摄像头拍了一段25秒的视频；又举着手机、对着电脑摄像头，把视频播了一遍。
![](https://pic2.zhimg.com/v2-311af7e55ff561eba446c4bdacf5b0ad_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='350' height='185'></svg>)
这样，一段真人视频，和一段“假脸”视频，就准备好了。

程序猿拍下的这两条视频，都提供下载。不过，他还是建议大家多收集一些数据，不同的人脸，甚至不同的人种，帮助算法茁壮成长。

下一步，要用**OpenCV**的人脸检测算法处理两段视频，把有用的区域 (ROI) ，就是人脸部分框出来。

这步用了80多行代码，每一行在做些什么，教程里都写清了。
![](https://pic1.zhimg.com/v2-b2f77e31792124aa16d156c6fd43554c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='264'></svg>)
最后，按照真假两个类别，把框好的脸部提取出来，就有了数据集。

温馨提示，需要**平衡**一下真图和假图的数量。比如，程序猿发现真实视频比假视频长，就用更大的间隔来提取。于是，他收获了真图161张，假图150张。

这数据集还是显得有些贫乏，所以后面需要**扩增**：旋转、翻转之类的操作，可以增加图片数量。

## **活体检测模型**

数据集做好了，就要喂给做活体检测的神经网络。

程序猿给网络起名**LivenessNet**，大体长这样：
![](https://pic4.zhimg.com/v2-9058aa692de6ed1df7781551c2bf2513_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='501'></svg>)
他说，这其实只是一个简单的CNN。而且，已经努力让网络保持在**最浅**、**参数最少**的状态。

这样做有两个原因：一是为避免模型在小数据集上发生过拟合，二是为保证模型快到可以实时推理，就算在树莓派上也能运行。

## **搭个网络**

现在，就来实现一下这个网络。打开livenessnet.py，再写这一段代码：


```python
# import the necessary packages
  from keras.models import Sequential
  from keras.layers.normalization import BatchNormalization
  from keras.layers.convolutional import Conv2D
  from keras.layers.convolutional import MaxPooling2D
  from keras.layers.core import Activation
  from keras.layers.core import Flatten
  from keras.layers.core import Dropout
  from keras.layers.core import Dense
 from keras import backend as K
 
 class LivenessNet:
     @staticmethod
     def build(width, height, depth, classes):
         # initialize the model along with the input shape to be
         # "channels last" and the channels dimension itself
         model = Sequential()
         inputShape = (height, width, depth)
         chanDim = -
 
         # if we are using "channels first", update the input shape
         # and channels dimension
         if K.image_data_format() == "channels_first":
             inputShape = (depth, height, width)
             chanDim =
```

![](https://pic4.zhimg.com/v2-7e11dbbeb72fcab3072723feb4d764eb_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='270'></svg>)
然后，一层一层加上去：


```python
# first CONV => RELU => CONV => RELU => POOL layer set
         model.add(Conv2D(, (, ), padding="same",
             input_shape=inputShape))
         model.add(Activation("relu"))
         model.add(BatchNormalization(axis=chanDim))
         model.add(Conv2D(, (, ), padding="same"))
         model.add(Activation("relu"))
         model.add(BatchNormalization(axis=chanDim))
         model.add(MaxPooling2D(pool_size=(, )))
        model.add(Dropout(0.25))

        # second CONV => RELU => CONV => RELU => POOL layer set
        model.add(Conv2D(, (, ), padding="same"))
        model.add(Activation("relu"))
        model.add(BatchNormalization(axis=chanDim))
        model.add(Conv2D(, (, ), padding="same"))
        model.add(Activation("relu"))
        model.add(BatchNormalization(axis=chanDim))
        model.add(MaxPooling2D(pool_size=(, )))
        model.add(Dropout(0.25))
```


程序猿说，这个网络有点像VGGNet，很浅，过滤器 (Filter) 很少。只是判断真假，不用深度网络。

最后，再加一个FC层→RELU层的组合。


```python
# first (and only) set of FC => RELU layers
         model.add(Flatten())
         model.add(Dense())
         model.add(Activation("relu"))
         model.add(BatchNormalization())
         model.add(Dropout(0.5))
 
         # softmax classifier
         model.add(Dense(classes))
        model.add(Activation("softmax"))

        # return the constructed network architecture
        return model
```


CNN搭好了，该训练了。

## **训练脚本长这样**

大致的训练过程，就像这张图：
![](https://pic1.zhimg.com/v2-1eed60ddfe225ca9ac3a913182dab1d8_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='603'></svg>)
打开train_liveness.py，写下这段代码：


```python
# set the matplotlib backend so figures can be saved in the background
  import matplotlib
  matplotlib.use("Agg")
  
  # import the necessary packages
  from pyimagesearch.livenessnet import LivenessNet
  from sklearn.preprocessing import LabelEncoder
  from sklearn.model_selection import train_test_split
  from sklearn.metrics import classification_report
 from keras.preprocessing.image import ImageDataGenerator
 from keras.optimizers import Adam
 from keras.utils import np_utils
 from imutils import paths
 import matplotlib.pyplot as plt
 import numpy as np
 import argparse
 import pickle
 import cv2
 import os

 # construct the argument parser and parse the arguments
 ap = argparse.ArgumentParser()
 ap.add_argument("-d", "--dataset", required=True,
     help="path to input dataset")
 ap.add_argument("-m", "--model", type=str, required=True,
     help="path to trained model")
 ap.add_argument("-l", "--le", type=str, required=True,
     help="path to label encoder")
 ap.add_argument("-p", "--plot", type=str, default="plot.png",
     help="path to output loss/accuracy plot")
 args = vars(ap.parse_args())
```




![](https://pic4.zhimg.com/v2-878f90655009eca469066a94005dcf3f_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='255'></svg>)△ 导入，不停地导入
里面包含了许多的**导入**：

> 有matplotlib，是可视化工具；
有LivenessNet，就是刚才搭好的CNN；
有train_test_split，这是scikit-learn里的函数，把数据集拆成训练集和测试集；
有classification_report，也是scikit-learn里面的工具，用来生成简短统计报告的；
有ImageDataGenerator，做数据扩增用的；
有Adam，适合这个任务的优化器，当然也可以用SGD、RMSprop等等代替；
有paths，这个模块是用来收集图片路径的；
有pyplot，也是美丽的可视化工具；
有numpy，是Python数学库，也是OpenCV必需品；
有argparse，用来处理命令行参数；
有pickle，可以把标签编码器序列化到盘上；
有cv2，这是一组OpenCV Binding；
还有os，这个模块用处很多，但这里只用到了它的操作系统路径分隔符而已。
![](https://pic4.zhimg.com/v2-7bc0dbbf8f55bd2033689f9c1264e2ff_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='278'></svg>)
梳理好这些，再看余下的代码，就会清晰很多了。

后面，是一系列的初始化，以及训练前的各种准备活动。此处略去，详见教程原文。

## **训练正式启动**

准备就绪，运行这段命令，就可以训练了：


```python
$ python train.py --dataset dataset --model liveness.model --le le.pickle
  [INFO] loading images...
  [INFO] compiling model...
  [INFO] training network for  epochs...
  Epoch /
  / [==============================] - s ms/step - loss: 1.0113 - acc: 0.5862 - val_loss: 0.4749 - val_acc: 0.7436
  Epoch /
  / [==============================] - s ms/step - loss: 0.9418 - acc: 0.6127 - val_loss: 0.4436 - val_acc: 0.7949
  Epoch /
 / [==============================] - s ms/step - loss: 0.8926 - acc: 0.6472 - val_loss: 0.3837 - val_acc: 0.8077
 ...
 Epoch /
 / [==============================] - s ms/step - loss: 0.2796 - acc: 0.9094 - val_loss: 0.0299 - val_acc: 1.0000
 Epoch /
 / [==============================] - s ms/step - loss: 0.3733 - acc: 0.8792 - val_loss: 0.0346 - val_acc: 0.9872
 Epoch /
 / [==============================] - s ms/step - loss: 0.2660 - acc: 0.9008 - val_loss: 0.0322 - val_acc: 0.9872
 [INFO] evaluating network...
               precision    recall  f1-score   support
 
         fake       0.97      1.00      0.99        
         real       1.00      0.98      0.99        
 
    micro avg       0.99      0.99      0.99        
    macro avg       0.99      0.99      0.99        
 weighted avg       0.99      0.99      0.99        
 
 [INFO] serializing network to 'liveness.model'...
```


## **成果斐然**

训练完结，LivenessNet在验证集上拿到了**99%**的准确度。

当然，验证集只是热身，后面还要**打开摄像头**，让AI去看更多没见过的人，和没见过的“假人”。

(这一部分代码，也有详尽的解读，参见教程原文。)
![](https://pic4.zhimg.com/v2-f9be6ca4614487d5a6ab4393c167a0af_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='238'></svg>)
就像开头展示的那样，AI能清楚地判断，它眼前的川川不是真人，而程序猿是真人。

那么，你也可以训练一只这样的AI了。

不过，不用局限于简单的二分类，可以用上前面讲到的那些复杂的分析方法，比如频率分析，比如光流法，大有可为。

教程原文传送门：
[https://www.pyimagesearch.com/2019/03/11/liveness-detection-with-opencv/](https://link.zhihu.com/?target=https%3A//www.pyimagesearch.com/2019/03/11/liveness-detection-with-opencv/)

源码下载入口，和无微不至的代码解析，都在里面了。
![](https://pic1.zhimg.com/v2-b2650de37c68168330faff617a80f3d0_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='565' height='329'></svg>)
— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤


