# Pytorch之浅入torchvision.transforms.ToTensor与ToPILImage - qq_37385726的博客 - CSDN博客





2018年08月18日 17:33:08[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：2423
所属专栏：[Pytorch](https://blog.csdn.net/column/details/26172.html)









> 
**torchvision.transforms**中定义了一系列数据转换形式，有PILImage,numpy,Tensor间相互转换，还能对数据进行处理。

在torchvision.datasets下载数据的时候，作为一个参数传入，对下载的数据进行处理

（关于数据集下载，具体可参见[https://blog.csdn.net/qq_37385726/article/details/81771943](https://blog.csdn.net/qq_37385726/article/details/81771943)）


# **目录**



[1.torchvision.transforms.ToTensor()](#1.torchvision.transforms.ToTensor())

[把一个取值范围是[0,255]的PIL.Image 转换成 Tensor](#%E6%8A%8A%E4%B8%80%E4%B8%AA%E5%8F%96%E5%80%BC%E8%8C%83%E5%9B%B4%E6%98%AF%5B0%2C255%5D%E7%9A%84PIL.Image%20%E8%BD%AC%E6%8D%A2%E6%88%90%20Tensor)

[shape为(H,W,C)的numpy.ndarray，转换成形状为[C,H,W]，取值范围是[0,1.0]的Tensor](#shape%E4%B8%BA(H%2CW%2CC)%E7%9A%84numpy.ndarray%EF%BC%8C%E8%BD%AC%E6%8D%A2%E6%88%90%E5%BD%A2%E7%8A%B6%E4%B8%BA%5BC%2CH%2CW%5D%EF%BC%8C%E5%8F%96%E5%80%BC%E8%8C%83%E5%9B%B4%E6%98%AF%5B0%2C1.0%5D%E7%9A%84Tensor)

[2.torchvision.transforms.ToPILImage()](#2.torchvision.transforms.ToPILImage())

[将ndarray转化成PILImage](#%E5%B0%86ndarray%E8%BD%AC%E5%8C%96%E6%88%90PILImage)

[将Tensor转化成PILImage](#%E5%B0%86Tensor%E8%BD%AC%E5%8C%96%E6%88%90PILImage)

[3.综合处理，ndarray->Tensor->PILImage](#3.%E7%BB%BC%E5%90%88%E5%A4%84%E7%90%86%EF%BC%8Cndarray-%3ETensor-%3EPILImage)

[深入理解Ptorchvision.transforms.ToTensor与ToPILImage](#%E6%B7%B1%E5%85%A5%E7%90%86%E8%A7%A3Ptorchvision.transforms.ToTensor%E4%B8%8EToPILImage)







# 1.torchvision.transforms.ToTensor()

> 
将PILImage或者numpy的ndarray转化成Tensor
- 对于PILImage转化的Tensor，其数据类型是torch.FloatTensor
- **对于ndarray的数据类型没有限制，但转化成的Tensor的数据类型是由ndarray的数据类型决定的。**



- 
### **把一个取值范围是[0,255]的PIL.Image 转换成 Tensor**


```python
img1 = Image.open('./Image/use_Crop.jpg')

t_out = transforms.ToTensor()(img1)
```
- 
### **shape为(H,W,C)的numpy.ndarray，转换成形状为[C,H,W]，取值范围是[0,1.0]的Tensor**


```python
n_out = np.random.rand(100,100,3)
print(n_out.dtype)

t_out = transforms.ToTensor()(n_out)
print(t_out.type())
```

**输出**

float64

torch.DoubleTensor



# 2.torchvision.transforms.ToPILImage()

> 
将Numpy的ndarray或者Tensor转化成PILImage类型**【在数据类型上，两者都有明确的要求】**
- ndarray的**数据类型要求dtype=uint8**, range[0, 255] and shape H x W x C
- Tensor 的shape为 C x H x W **要求是FloadTensor的，不允许DoubleTensor或者其他类型**

                # to a PIL.Image of range [0, 255]

- 
### **将ndarray转化成PILImage**


```python
#初始化随机数种子
np.random.seed(0)

data = np.random.randint(0, 255, 300)
print(data.dtype)
n_out = data.reshape(10,10,3)

#强制类型转换
n_out = n_out.astype(np.uint8)
print(n_out.dtype)

img2 = transforms.ToPILImage()(n_out)
img2.show()
```
- 
### **将Tensor转化成PILImage**


```python
t_out = torch.randn(3,10,10)
img1 = transforms.ToPILImage()(t_out)
img1.show()
```

> 
**因为要求是FloatTensor类型，所以最好是不管此时的tensor是不是FloatTensor类型，都加一个强制转换再传进去。**


```python
t_out = torch.randn(3,10,10)
img1 = transforms.ToPILImage()(t_out.float())
img1.show()
```



# 3.综合处理，ndarray->Tensor->PILImage

```python
n_out = np.random.rand(100,100,3)
t_out = transforms.ToTensor()(n_out)
img2 = transforms.ToPILImage()(t_out.float())  #强制类型转换
img2.show()
```



# 深入理解Ptorchvision.transforms.ToTensor与ToPILImage

[https://blog.csdn.net/qq_37385726/article/details/81811466](https://blog.csdn.net/qq_37385726/article/details/81811466)

































