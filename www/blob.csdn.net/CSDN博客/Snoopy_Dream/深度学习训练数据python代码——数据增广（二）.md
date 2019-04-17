# 深度学习训练数据python代码——数据增广（二） - Snoopy_Dream - CSDN博客





2018年09月27日 21:20:31[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：799
所属专栏：[深度学习caffe常用python代码分享](https://blog.csdn.net/column/details/22599.html)[数据生成器+数据增广+常用工具](https://blog.csdn.net/column/details/29905.html)









### python图像数据增强库



Augmentor使用比较简单,只有一些简单的操作。 imgaug实现的功能更多，可以对keypoint, bounding box同步处理，比如你现在由一些标记好的数据，只有同时对原始图片和标记信息同步处理，才能有更多的标记数据进行训练。我在segmentation和detection任务经常使用imgaug这个库。



# Augmentor

[http://augmentor.readthedocs.io/en/master/index.html](http://augmentor.readthedocs.io/en/master/index.html)

[直接点开，谷歌翻译，文档很全]

Augmentor 是管道化的图像增强库，每一个增强操作都是逐步叠加在图像上。此外对于输入的图像，可以选择按照一定的概率进行增强，比如只**随机**对一半图像进行旋转。



可以实现的操作有, rotate, crop, perspective skew(**视角倾斜**), elastic distortions**(弹性变换【OCR非刚体中可以利用随机扭曲】**), sheering(**坐标轴倾斜**), mirroring(**镜像**)、**随机改变图像亮度**`Augmentor.Operations.RandomBrightness、随机改变对比度/饱和度、**随机擦除**某一矩形框里的像素Augmentor.Operations.``RandomErasing、随机裁剪`

**/*注意倾斜后，旋转后都有多余的边界，从倾斜图像中裁剪原始图像的相同纵横比的最大可能区域，然后调整大小以匹配原始图像尺寸*/**

## **安装**
`sudo -H pip install Augmentor`
## 升级：`  `

```python
pip install Augmentor --upgrade
```

# **用法**

### **第1步：创建新的管道**

必须首先初始化一个[`Pipeline`](https://augmentor.readthedocs.io/en/master/code.html#Augmentor.Pipeline.Pipeline)对象，该对象指向**存储原始图像数据集的目录**

```python
>>> import Augmentor
>>> p = Augmentor.Pipeline("/path/to/images")
Initialised with 100 images found in selected directory.
```

### **第2步：向管道添加操作**

```python
>>> p.rotate(probability=0.7, max_left_rotation=10, max_right_rotation=10)
```

添加进一步的操作，在这种情况下是一个[`zoom()`](https://augmentor.readthedocs.io/en/master/code.html#Augmentor.Pipeline.Pipeline.zoom)操作

```python
>>> p.zoom(probability=0.3, min_factor=1.1, max_factor=1.6)
```

### **第3步：从流水线执行和采样**

并指定所需的图像数量，在本例中为10,000：

```python
>>> p.sample(10000)
```

默认情况下，新生成的增强图像将保存到名为**output的目录中**，相对于包含初始图像数据集的目录。

可以使用Augumentor.Pipeline()创建一个实例,调用各种方法向pipeline添加方法, status()可以显示当前pipeline的状态，在status中每个操作都有一个对应的index, remove_operation(index)移除一个操作, 最后调用**sample(nums)得到nums个augument**后的图像。

```python
import Augmentor
p = Augmentor.Pipeline("/path/to/images/")
p.status()
p.remove_operation(0)
```









参考：[https://www.cnblogs.com/vincentcheng/p/9186540.html](https://www.cnblogs.com/vincentcheng/p/9186540.html)





