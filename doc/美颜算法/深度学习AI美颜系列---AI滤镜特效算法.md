# 深度学习AI美颜系列---AI滤镜特效算法

2018年05月25日 17:53:02

 

Trent1985

 

阅读数：4937

更多

所属专栏： [SF图像滤镜/美颜/美妆算法详解与实战](https://blog.csdn.net/column/details/25028.html)



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/Trent1985/article/details/80454295

滤镜是图像美化中必不可少的步骤， 所谓滤镜，最初是指安装在相机镜头前过滤自然光的附加镜头，用来实现调色和添加效果。我们做的滤镜算法又叫做软件滤镜，是对大部分镜头滤镜进行的模拟，当然，误差也就再所难免，我们的宗旨只是无限逼近。也是这个原因，我们无法再现真实的拍摄场景，无法复原照片中未包含的信息，进而也难以实现某些特殊滤镜效果，诸如偏光镜和紫外线滤色镜（UV）的效果等等。

目前各种图像处理软件无论是PC软件还是手机app，都有着各种各样的滤镜特效，Instagram也正因为自己独特的滤镜风格，风靡一时；

针对传统的滤镜特效，我这里将它分类为LUT滤镜和几何滤镜两种。

LUT滤镜是指基于调色表和纹理组合的滤镜特效；

几何滤镜是指一些基于空间位置变换得到的特效，比如哈哈镜等；

随着深度学习的兴起，各种风格滤镜层出不穷，给人们带来了惊艳的效果，谷歌的HDRNET更是将AI应用到了照片智能调色，今天我们不讲HDRNET，我们以一种最简单的方式，来讲一下如何使用深度学习来做传统的LUT滤镜；

有人说LUT滤镜已经很成熟了，用深度学习是多此一举，这里我们不探讨意义，只探讨方法论，实际上LUT可以实现的是一类特效而已，可以说只是深度学习中的一个子集；

我们以一个特殊的黑白滤镜为例，先看下面几张图：

![img](https://img-blog.csdn.net/20180525172330717)![img](https://img-blog.csdn.net/20180525172359922)![img](https://img-blog.csdn.net/20180525172412667)

上面的三列图分别为原图+PS灰度+论文算法的效果对比，我们可以看到一般的灰度化算法(第二列所示)直接导致了一些图像信息的消逝，至少我们无法分辨，而论文算法的效果在一定程度上增强了不同信息之间的对比度，让我们依旧可以看到图像本身的内容；

这种算法实际上就是对比度保留的去色算法，相关论文如下：

[Real-time Contrast Preserving Decolorization](http://appsrv.cse.cuhk.edu.hk/~xuli/mypapers/siga12t_color2gray.pdf)

[Contrast Preserving Decolorization](http://www.cse.cuhk.edu.hk/~leojia/projects/color2gray/)

[Color2Gray: Salience-Preserving Color Removal  ](http://www.cs.northwestern.edu/~jet/docs/2005color2grayFINAL.pdf)

上述论文都是使用传统的图像算法来实现对比度保留的灰度计算的，而且计算复杂，速度差异化也比较大，现在我们可以将这个算法效果理解为一种特殊的黑白滤镜，然后用深度学习的方法来实现这种效果：

我这里使用最简单的CNN网络，我们构建一个四层卷积层的网络，如下所示：

![img](https://img-blog.csdn.net/20180525174004999)





网络输入为256X256大小三通道的彩色图，输出为256X256大小三通道彩色图；使用Keras来实现该网络；

该网络有四个卷积层，相关参数分别如下：





第一层：kSize=9,Strides=1,Padding=same,输出通道channels=32,激活函数为relu；





第二层：kSize=3,Strides=1,Padding=same,输出通道channels=64,激活函数为relu；

第三层：kSize=3,Strides=1,Padding=same,输出通道channels=64,激活函数为relu；

第四层：kSize=3,Strides=1,Padding=same,输出通道channels=3,无激活函数；

第四层卷积后，图像以256X256X3大小输出；

loss计算采用均方误差，adm下降算法，minbatch设置为32，迭代次数为500次；

数据来源：使用论文算法处理原图得到对应的效果图，分别作为训练的样本；

代码如下：

```python
import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt
from tensorflow.python.framework import ops
import math
import os
from keras import layers
from keras.layers import Dropout,Input, Add, Dense, Activation, ZeroPadding2D, BatchNormalization, Flatten, Conv2D, AveragePooling2D, MaxPooling2D, GlobalMaxPooling2D,Lambda,Convolution2D,Conv2DTranspose,UpSampling2D
from keras.models import Model, load_model,Sequential
from keras.preprocessing import image
from keras.utils import layer_utils
from keras.utils.data_utils import get_file
from keras.applications.imagenet_utils import preprocess_input
from keras.optimizers import SGD
import pydot
from IPython.display import SVG
from keras.utils.vis_utils import model_to_dot
from keras.utils import plot_model
from keras.initializers import glorot_uniform
from keras.utils import to_categorical
import scipy.misc
import h5py
import keras.backend as K
K.set_image_data_format('channels_last')
K.set_learning_phase(1)
K.set_image_dim_ordering('tf')
```

```python
def input_data():  
      
    path_x = "data/train/X/" 
    path_y = "data/train/Y3/"
    train_num = sum([len(x) for _, _, x in os.walk(os.path.dirname(path_x))])  
    image_train = np.zeros((train_num,256,256,3)) 
    label_train = np.zeros((train_num,256,256,3)) 
    count = 0
    for i in range(train_num):
        img = image.load_img(path_x + str(i) + ".bmp", target_size = (256, 256))
        x = image.img_to_array(img)
        x = np.expand_dims(x, axis = 0)                   
        image_train[count] = x
        
        img = image.load_img(path_y + str(i) + ".bmp", target_size = (256, 256))
        x = image.img_to_array(img)
        x = np.expand_dims(x, axis = 0)                   
        label_train[count] = x
        count = count + 1
    return image_train, label_train
def FilterModel(input_shape = (256,256,3)):
    # Define the input as a tensor with shape input_shape
    X_input = Input(input_shape)
    X = ZeroPadding2D((0,0))(X_input)
    # frist convolution layer
    X = Conv2D(32, (9, 9), strides = (1, 1),padding='same', name = 'conv1', kernel_initializer = glorot_uniform(seed=0))(X)
    #X = BatchNormalization(axis = 3, name = 'bn_conv1')(X)
    X = Activation('relu')(X)
    print("conv1--X:",str(X.shape))
    # second convolution layer
    X = Conv2D(64, (3, 3), strides = (1, 1),padding='same', name = 'conv2', kernel_initializer = glorot_uniform(seed=0))(X)
    #X = BatchNormalization(axis = 3, name = 'bn_conv2')(X)
    X = Activation('relu')(X)
    X_shortcut = X
    print("conv2--X:",str(X.shape))
    # third convolution layer
    X = Conv2D(128, (3, 3), strides = (1, 1),padding='same', name = 'conv3', kernel_initializer = glorot_uniform(seed=0))(X)
    #X = BatchNormalization(axis = 3, name = 'bn_conv3')(X)
    X = Activation('relu')(X)
    #five convolution layer
    output = Conv2D(3, (5, 5), strides = (1, 1),padding='same', name = 'conv5', kernel_initializer = glorot_uniform(seed=0))(X)
    print("conv5--X:",str(X.shape))   
    model = Model(inputs=X_input, outputs=output, name='FilterModel')
    return model
#训练代码  
image_train, label_train = input_data()  
image_train = image_train.astype('float32')
label_train = label_train.astype('float32')
image_train /= 255
label_train /= 255
print("image_train:", str(image_train.shape))
print("label_train:", str(label_train.shape))
K.clear_session()
model = FilterModel((256,256,3))
model.compile(optimizer = "adam", loss = "mean_squared_error", metrics = ["accuracy"])
print("image_train:", str(image_train.shape))
print("label_train:", str(label_train.shape))
model.fit(image_train, label_train,epochs=500,batch_size=32)#训练500次，数据量为500张，有点少，没关系，只介绍方法与思路
model.save("model/filterModel.h5")
print("Model save done!")
```

```python
#测试代码如下
import matplotlib
import scipy.misc
np.random.seed(1)  
from keras.models import load_model  
image = img.imread("data/t1.bmp")  
print("image: ", str(image.shape))
image_test = image[:,:,:]  
print("image_test shape:", str(image_test.shape))  
image = image_test.reshape(1,image_test.shape[0],image_test.shape[1],3)  
image = image / 255.  
# imaget = tf.image.convert_image_dtype(image, tf.float32)  
print("image shape: %", str(image.shape)) 
model = load_model('model/filterModel.h5')
res = model.predict(image)
print("res:",str(res.shape))
# image = res[0,...].eval()
amin, amax = res.min(),res.max()
image = (res - amin) / (amax - amin) * 255.0
image = image[0,...]
print(str(image.shape))
matplotlib.image.imsave('data/result5.png', np.uint8(image))
```



最后效果图如下：

![img](https://img-blog.csdn.net/20180525174901275)![img](https://img-blog.csdn.net/2018052517491622)![img](https://img-blog.csdn.net/20180525174931747)



从效果上看，思路是没有问题的，效果也基本达到了预期；

本文话题是AI美颜系列，因此这里我用同样的网络，训练了一个阿宝色美颜滤镜，我们以传统阿宝色算法处理10000样本，得到对应的效果图，以此作为训练数据，当然，理论上可以使用专业的PS设计人员调色精修得到更好的阿宝色效果图作为训练样本，这样效果才更好，这里为了说明方法，简化如此，最后训练模型的效果如下：

![img](https://img-blog.csdn.net/20180525181006887)![img](https://img-blog.csdn.net/2018052518103462)

原图                                               阿宝色效果图

大家可以测试一些其他彩色滤镜，当然，对于美颜这个话题，我们可以用这种思路去实现一些美颜特效滤镜，比如美颜相机中的肤色调节滤镜，肤色粉嫩滤镜等等，相信深度学习不会让你失望的！这里的网络比较简单，当然大家可以使用比较复杂的，以达到更好的效果！

本人QQ1358009172