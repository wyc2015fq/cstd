# 图像增强︱window7+opencv3.2+keras/theano简单应用(函数解读) - 素质云笔记/Recorder... - CSDN博客





2017年03月06日 17:13:05[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：4920








在服务器上安装opencv遇到跟CUDA8.0不适配的问题，于是不得不看看其他机器是否可以预装并使用。 

.

# 一、python+opencv3.2安装

opencv在windows安装为啥这么简单？ 

安装流程： 

1、下载opencv文件[opencv-3.2.0-vc14.exe](https://sourceforge.net/projects/opencvlibrary/files/opencv-win/3.2.0/)

2、点击下载，其实就是解压过程，随便放在一个盘里面。 

3、python部署阶段， 

进入OpenCV的安装目录下找到+复制：\build\python\2.7\x64\cv2.pyd 

将cv2.pyd复制到python的子目录：\Lib\site-packages\ 

4、即可直接调用：
```java
import cv2
```

.

# 二、windows+keras/theano

> 
Keras深度学习框架是基于Theano或Tensorflow框架安装的，所以首先要准备底层框架的搭建，用tensorflow比较麻烦，所以选用Theano安装即可。


## 1、tensorflow/keras框架

同时如果要使用tensorflow0.12版本+python3.5及以上，也可以使用Anaconda 3.5. 

一种比较好的方式使用docker： 

参考：[TensorFlow 官方文档中文版、下载与安装](http://wiki.jikexueyuan.com/project/tensorflow-zh/get_started/os_setup.html)
如果要使用原生的window安装：
- （1）前提：现有了python3.5或Anaconda 3.5
- （2）下载：tensorflow-0.12.0rc0-cp35-cp35m-win_amd64.whl，下载的东西放在某文件夹中
- （3）在Power Shell中输入下述命令实现本地安装：
`pip install F:\DevResources\tensorflow_gpu-0.12.0rc0-cp35-cp35m-win_amd64.whl`- （4）验证安装

到“所有程序”下找到”Python 3.5 64bit”，出现命令窗口，输入测试代码：

```
>>>import tensorflow as tf
>>>sess = tf.Session()
>>>a = tf.constant(10)
>>>b = tf.constant(22)
>>>print(sess.run(a + b))
32
```

正确输出32则为成功安装。

报错无法正常下载numpy 1.11.0：参考博客：[原生Windows安装TensorFlow 0.12方法](http://www.itdadao.com/articles/c15a844161p0.html)

.

## 2、theano/keras框架

安装过程：
- （1）安装theano，Power Shell中输入：
`pip install theano -U --pre`- （2）安装keras：
`pip install keras -U --pre`- （3）修改默认后端：很关键，不然会一直报错：ImportError: No module named tensorflow 

因为，keras默认后端是给tensorflow， 

打开C:\Users\当前用户名.keras,修改文件夹内的keras.json文件如下：
```
{
"image_dim_ordering":"th",
"epsilon":1e-07,
"floatx":"float32",
"backend":"theano"
}
```
- （4）验证安装

```java
>>>import keras
Using Theano(Tensorflow) backend.
>>>
```

当然，还有theano的加速模式，可参考： [Keras安装和配置指南(Windows)](https://keras-cn.readthedocs.io/en/latest/getting_started/keras_windows/)

.

# 三、用python+keras/theano进行图像增强（Data Augmentation）

## 1、图像增强的方式

**以下一共有8中图像变换的方式：**
- 旋转 | 反射变换(Rotation/reflection): 随机旋转图像一定角度; 改变图像内容的朝向;
- 翻转变换(flip): 沿着水平或者垂直方向翻转图像;
- 缩放变换(zoom): 按照一定的比例放大或者缩小图像;
- 平移变换(shift): 在图像平面上对图像以一定方式进行平移; 可以采用随机或人为定义的方式指定平移范围和平移步长, 

沿水平或竖直方向进行平移. 改变图像内容的位置;
- 尺度变换(scale): 对图像按照指定的尺度因子, 进行放大或缩小; 或者参照SIFT特征提取思想, 

利用指定的尺度因子对图像滤波构造尺度空间. 改变图像内容的大小或模糊程度;
- 对比度变换(contrast): 在图像的HSV颜色空间，改变饱和度S和V亮度分量，保持色调H不变. 

对每个像素的S和V分量进行指数运算(指数因子在0.25到4之间), 增加光照变化;
- 噪声扰动(noise): 对图像的每个像素RGB进行随机扰动, 常用的噪声模式是椒盐噪声和高斯噪声;
- 颜色变换(color): 在训练集像素值的RGB颜色空间进行PCA, 得到RGB空间的3个主方向向量,3个特征值 

.

## 2、图像增强的案例

网上有一个极为广泛的套路,参考博客《[深度学习中的Data Augmentation方法和代码实现](https://absentm.github.io/2016/06/14/%E6%B7%B1%E5%BA%A6%E5%AD%A6%E4%B9%A0%E4%B8%AD%E7%9A%84Data-Augmentation%E6%96%B9%E6%B3%95%E5%92%8C%E4%BB%A3%E7%A0%81%E5%AE%9E%E7%8E%B0/)》、《[深度学习中的数据增强实现（Data Augmentation）](https://github.com/manutdzou/manutdzou.github.io/blob/master/_posts/%E7%A7%91%E7%A0%94/2016-10-26-data%20augmentation.md)》、[《keras中文文档-图片预处理》](https://keras-cn.readthedocs.io/en/latest/preprocessing/image/)：

```python
from keras.preprocessing.image import ImageDataGenerator, array_to_img, img_to_array, load_img

# 主要的增强函数
datagen = ImageDataGenerator(
        rotation_range=0.2,
        # 整数，旋转范围, 随机旋转(0-180)度
        width_shift_range=0.2,
        # 浮点数，以图像的长宽小部分百分比为变化范围进行横向平移
        height_shift_range=0.2,
        # 浮点数，以图像的长宽小部分百分比为变化范围进行竖直平移
        shear_range=0.2,
        # 浮点数，水平或垂直投影变换
        zoom_range=0.2,
        # 浮点数，随机缩放的幅度，[lower,upper] = [1 - zoom_range, 1+zoom_range]
        horizontal_flip=True,
        # 布尔值，进行随机水平翻转
        fill_mode='nearest')
        # 填充像素，超出边界时，有四种方式：‘constant’、‘nearest’、‘reflect’、‘wrap’
# featurewise_center=True  # 使输入数据集去中心化（均值为0）
# featurewise_std_normalization=True #将输入除以数据集的标准差以完成标准化
# rescale=1./255,#重放缩因子,默认为None. 如果为None或0则不进行放缩,否则会将该数值乘到数据上(在应用其他变换之前)
# zca_whitening=True #对输入数据施加ZCA白化
# channel_shift_range=0.2 #随机通道偏移的幅度
# vertical_flip=True #布尔值，进行随机竖直翻转



#数据导入
img = load_img('C:\\Users\\Desktop\\003.jpg')  
x = img_to_array(img)  
x = x.reshape((1,) + x.shape)  
# the .flow() command below generates batches of randomly transformed images
# and saves the results to the `preview/` directory
i = 0
for batch in datagen.flow(x, 
                          batch_size=1,
                          save_to_dir='C:\\Users\\Desktop',  
                          #存放文件夹
                          save_prefix='lena', 
                          #存放文件名字
                          save_format='jpg'):
    i += 1
    if i > 20:
        break
```

其中： 

ImageDataGenerator是图像增强的主要函数，里面包含了很多类型的增强方法 

load_img、img_to_array、x.reshape图像载入函数 

datagen.flow，增强执行函数
其中：
- load_img函数：

```
load_img(path, grayscale=False, target_size=None)
#path:图像载入的路径
#grayscale:是否只载入灰度，默认为false
#target_size:是否需要重新框定大小，默认是原图大小，其中如果要修改，则类似：
image.load_img(img_path, target_size=(224, 224))
```
- img_to_array函数：

```
img_to_array(img, dim_ordering='default')
#img，load_img之后的内容
#dim_ordering，图像的格式是否更改，一般是default，不做任何更改
```

函数源码来源： 
[https://github.com/fchollet/keras/blob/master/keras/preprocessing/image.py](https://github.com/fchollet/keras/blob/master/keras/preprocessing/image.py)

.

## 延伸一：win下theano安装报错
`ImportError: cannot import name gof`
网上的[解决方案](https://github.com/Theano/Theano/issues/2406)为：
`pip install --upgrade --no-deps git+git://github.com/Theano/Theano.git`
但是windows里面没有git，于是我[安装了git](https://git-scm.com/download/win)之后，执行上述内容，的确安装完成，但是就是Import不了…  无解了….

也有人说重新[安装theano](http://deeplearning.net/software/theano/install.html#bleeding-edge-install-instructions)

[http://blog.csdn.net/zhouyongsdzh/article/details/24449645](http://blog.csdn.net/zhouyongsdzh/article/details/24449645)

.

## 延伸二：python中常用数据增强的library

下面这个Github的作者做了一个library, 实现了N种 data augmentation， 还嫌你的数据太少吗。。。试试吧！! 输入一只小老鼠，输出一窝小老鼠。 

github地址：[https://github.com/aleju/imgaug](https://github.com/aleju/imgaug)
![这里写图片描述](https://img-blog.csdn.net/20170416172241652?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







