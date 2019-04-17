# Pytoch之torchvision.transforms图像变换 - qq_37385726的博客 - CSDN博客





2018年08月18日 21:11:34[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：986
所属专栏：[Pytorch](https://blog.csdn.net/column/details/26172.html)









# **目录**

[transforms.CenterCrop(size)](#transforms.CenterCrop(size))

[transforms.RandomCrop(size,padding=0)](#transforms.RandomCrop(size%2Cpadding%3D0))

[transforms.RandomHorizontalFlip](#class-torchvisiontransformsrandomhorizontalflip)

[transforms.RandomSizedCrop(size, interpolation=2)](#class-torchvisiontransformsrandomsizedcropsize-interpolation2)

[transforms.Pad(padding, fill=0)](#class-torchvisiontransformspadpadding-fill0)

# transforms.CenterCrop(size)

> 
将给定的`PIL.Image`进行中心切割，得到给定的`size`，`size`可以是`tuple`，`(target_height, target_width)`。`size`也可以是一个`Integer`，在这种情况下，切出来的图片的形状是正方形。

**size可以为int，也可以为float**


```python
#定义中心切割
centerCrop = transforms.CenterCrop((img.size[0]/2,img.size[1]/2))
imgccrop = centerCrop(img)
```

# transforms.RandomCrop(size,padding=0)

> 
切割中心点的位置随机选取。`size`可以是`tuple`也可以是`Integer`。

但是如果是Tuple，只能是int型的不能是float


```python
#要求目标size必须为整数
randomCrop1 = transforms.RandomCrop((int(img.size[0]/2),int(img.size[1]/2)))  #padding默认为False
randomCrop2 = transforms.RandomCrop((int(img.size[0]/2),int(img.size[1]/2)),padding=10)


imgrcrop1 = randomCrop1(img)
imgrcrop2 = randomCrop2(img)
```





# transforms.RandomHorizontalFlip

> 
随机水平翻转给定的`PIL.Image`,概率为`0.5`。即：一半的概率翻转，一半的概率不翻转。


```python
#随机将图片旋转180°
randomFlip = transforms.RandomHorizontalFlip()

imgf = randomFlip(img)
```





# transforms.RandomSizedCrop(size, interpolation=2)

> 
先将给定的`PIL.Image`随机切，然后再`resize`成给定的`size`大小。

size只能是Int，不能是float或tuple


```python
#参数需为一个整数，不能是float或者tuple
randomcut = transforms.RandomResizedCrop(100)
imgc = randomcut(img)
```





# transforms.Pad(padding, fill=0)

> 
将给定的`PIL.Image`的所有边用给定的`pad value`填充。 `padding：`要填充多少像素`fill`

可以用于给图片加边框2333


```python
#加边界框
pad = transforms.Pad(padding=10,fill=0)
pad_img = pad(img)
pad_img.save("block.jpg")
```











