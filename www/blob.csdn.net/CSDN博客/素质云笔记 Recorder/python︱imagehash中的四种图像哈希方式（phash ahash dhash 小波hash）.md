# python︱imagehash中的四种图像哈希方式（phash/ahash/dhash/小波hash） - 素质云笔记/Recorder... - CSDN博客





2017年11月20日 14:36:58[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：7020








code来源：[https://github.com/JohannesBuchner/imagehash](https://github.com/JohannesBuchner/imagehash)

外文原文：[https://fullstackml.com/wavelet-image-hash-in-python-3504fdd282b5](https://fullstackml.com/wavelet-image-hash-in-python-3504fdd282b5)

![这里写图片描述](https://camo.githubusercontent.com/5216e9008861ed84aa38ae92d78bda1516cca118/68747470733a2f2f6a656e737365676572732e636f6d2f75706c6f6164732f696d616765732f66696e6765727072696e742e706e67)

可以直接pip：
`pip install imagehash`
# 1 perception hashing

感知哈希，不同于aHash，但首先它确实是离散余弦变换和频域。 

主函数：`def phash(image, hash_size=8, highfreq_factor=4):`
- 两个参数，一起决定了图片resize的大小，最适合的才最好，按照公式： 

img_size = hash_size * highfreq_factor
- hash_size代表最终返回hash数值长度
- highfreq_factor，代表resize的尺度

**案例：**

```bash
highfreq_factor = 1
hash_size = 8
img_size = hash_size * highfreq_factor

hash1 = imagehash.phash(Image.open('1_1.jpg'),hash_size=hash_size,highfreq_factor=highfreq_factor)
print(hash1)
# > 354adab5054af0b7

hash2 = imagehash.phash(Image.open('5_1.jpg'),hash_size=hash_size,highfreq_factor=highfreq_factor)
print(hash2)
# > 5b7724c8bb364551

1 - (hash1 - hash2)/len(hash1.hash)**2 # 相似性
```

# 2  average hashing

平均散列，对于每个像素输出1，如果该像素是大于或等于平均值，否则为0。 
**主函数：**

```bash
average_hash(image, hash_size=8)
```

**案例：**

```bash
hash_size = 6
hash1 = imagehash.average_hash(Image.open('1_1.jpg'),hash_size=hash_size)
print(hash1)
# > 354adab5054af0b7

hash2 = imagehash.average_hash(Image.open('5_1.jpg'),hash_size=hash_size)
print(hash2)
# > 5b7724c8bb364551

1 - (hash1 - hash2)/len(hash1.hash)**2 # 相似性
```

# 3 difference hashing

梯度散列，计算每个像素的差值，并与平均差异的差异进行比较。

```python
def dhash(image, hash_size=8)
```

**案例：**

```bash
hash_size = 10
hash1 = imagehash.dhash(Image.open('5_1.jpg'),hash_size=hash_size)
print(hash1)
# > 354adab5054af0b7

hash2 = imagehash.dhash(Image.open('1_1.jpg'),hash_size=hash_size)
print(hash2)
# > 5b7724c8bb364551

1 - (hash1 - hash2)/len(hash1.hash)**2 # 相似性
```

# 4 wavelet hashing

离散小波变换（DWT）是频表示的另一种形式。流行的DCT和傅立叶变换使用余弦函数作为sin\cos的基础：sin(x)，sin(2x)，sin(3x)等等。与此相反，DWT使用一个单一的功能作为基础，但在不同的形式：缩放和移动。基础功能是可以改变的，这就是为什么我们可以有Haar小波，Daubechie-4小波等，这尺度效应给我们很大“时频表示”的时候，低频部分类似于原始信号。

小波散列，几天前我把它添加到库里。它的工作原理在频域中作为pHash但它使用DWT代替DCT变换。 
**主函数：**
`def whash(image, hash_size = 8, image_scale = None, mode = 'haar', remove_max_haar_ll = True)`
**参数：**
- mode: 

       ‘haar’ - Haar wavelets, by default 

       ‘db4’ - Daubechies wavelets- remove_max_haar_ll:是否去掉低频段位，low level (LL) frequency
- image_scale:图像重新resize成多大，一定是2的倍数

**案例：**

```bash
hash_size = 8
mode = 'db4'
image_scale = 64
hash1 = imagehash.whash(Image.open('1_1.jpg'),image_scale=image_scale,hash_size=hash_size,mode = mode)
print(hash1)
# > 354adab5054af0b7

hash2 = imagehash.whash(Image.open('5_1.jpg'),image_scale=image_scale,hash_size=hash_size,mode = mode)
print(hash2)
# > 5b7724c8bb364551

1 - (hash1 - hash2)/len(hash1.hash)**2 # 相似性
```


**公众号“素质云笔记”定期更新博客内容：**
![这里写图片描述](https://img-blog.csdn.net/20180226155348545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





