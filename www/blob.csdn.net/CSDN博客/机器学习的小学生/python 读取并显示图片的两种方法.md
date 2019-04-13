
# python 读取并显示图片的两种方法 - 机器学习的小学生 - CSDN博客


2017年04月23日 11:10:02[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：2339


转自：[http://www.cnblogs.com/yinxiangnan-charles/p/5928689.html](http://www.cnblogs.com/yinxiangnan-charles/p/5928689.html)
在 python 中除了用 opencv，也可以用 matplotlib 和 PIL 这两个库操作图片。本人偏爱 matpoltlib，因为它的语法更像 matlab。
## 一、matplotlib
**1. 显示图片**
```python
import
```
```python
matplotlib.pyplot
```
```python
as
```
```python
plt
```
```python
# plt 用于显示图片
```
```python
import
```
```python
matplotlib.image
```
```python
as
```
```python
mpimg
```
```python
# mpimg 用于读取图片
```
```python
import
```
```python
numpy
```
```python
as
```
```python
np
lena = mpimg.imread(
```
```python
'lena.png'
```
```python
)
```
```python
# 读取和代码处于同一目录下的 lena.png
```
```python
# 此时 lena 就已经是一个 np.array 了，可以对它进行任意处理
```
```python
lena.shape
```
```python
#(512, 512, 3)
```
```python
plt.imshow(lena)
```
```python
# 显示图片
```
```python
plt.axis(
```
```python
'off'
```
```python
)
```
```python
# 不显示坐标轴
```
```python
plt.show()
```
**2. 显示某个通道**
```python
# 显示图片的第一个通道
```
```python
lena_1 = lena[:,:,
```
```python
0
```
```python
]
plt.imshow(
```
```python
'lena_1'
```
```python
)
plt.show()
```
```python
# 此时会发现显示的是热量图，不是我们预想的灰度图，可以添加 cmap 参数，有如下几种添加方法：
```
```python
plt.imshow(
```
```python
'lena_1'
```
```python
, cmap=
```
```python
'Greys_r'
```
```python
)
plt.show()
img = plt.imshow(
```
```python
'lena_1'
```
```python
)
img.set_cmap(
```
```python
'gray'
```
```python
) #
```
```python
'hot'
```
```python
是热量图
plt.show()
```
**3. 将 RGB 转为灰度图**
```python
def
```
```python
rgb2gray
```
```python
(rgb)
```
```python
:
```
```python
return
```
```python
np.dot(rgb[...,:
```
```python
3
```
```python
], [
```
```python
0.299
```
```python
,
```
```python
0.587
```
```python
,
```
```python
0.114
```
```python
])
gray = rgb2gray(lena)
```
```python
# 也可以用 plt.imshow(gray, cmap = plt.get_cmap('gray'))
```
```python
plt.imshow(gray, cmap=
```
```python
'Greys_r'
```
```python
)
plt.axis(
```
```python
'off'
```
```python
)
plt.show()
```
**4. 对图像进行放缩**
```python
from
```
```python
scipy
```
```python
import
```
```python
misc
lena_new_sz = misc.imresize(lena,
```
```python
0.5
```
```python
)
```
```python
# 第二个参数如果是整数，则为百分比，如果是tuple，则为输出图像的尺寸
```
```python
plt.imshow(lena_new_sz)
plt.axis(
```
```python
'off'
```
```python
)
plt.show()
```
**5. 保存图像**
5.1 保存 matplotlib 画出的图像
该方法适用于保存任何 matplotlib 画出的图像，相当于一个 screencapture。
```python
plt.imshow(lena_new_sz)
plt.axis(
```
```python
'off'
```
```python
)
plt.savefig(
```
```python
'lena_new_sz.png'
```
```python
)
```
5.2 将 array 保存为图像
```python
from
```
```python
scipy
```
```python
import
```
```python
misc
misc.imsave(
```
```python
'lena_new_sz.png'
```
```python
, lena_new_sz)
```
5.3 直接保存 array
读取之后还是可以按照前面显示数组的方法对图像进行显示，这种方法完全不会对图像质量造成损失
```python
np.save(
```
```python
'lena_new_sz'
```
```python
, lena_new_sz)
```
```python
# 会在保存的名字后面自动加上.npy
```
```python
img = np.
```
```python
load
```
```python
(
```
```python
'lena_new_sz.npy'
```
```python
)
```
```python
# 读取前面保存的数组
```
## 二、PIL
**1. 显示图片**
```python
from PIL import
```
```python
Image
```
```python
im =
```
```python
Image
```
```python
.open(
```
```python
'lena.png'
```
```python
)
im.show()
```
**2. 将 PIL Image 图片转换为 numpy 数组**
```python
im_array = np.
```
```python
array
```
```python
(im)
```
```python
# 也可以用 np.asarray(im) 区别是 np.array() 是深拷贝，np.asarray() 是浅拷贝
```
**3. 保存 PIL 图片**
直接调用 Image 类的 save 方法
```python
from PIL import
```
```python
Image
```
```python
I
```
```python
=
```
```python
Image
```
```python
.open(
```
```python
'lena.png'
```
```python
)
```
```python
I
```
```python
.save(
```
```python
'new_lena.png'
```
```python
)
```
**4. 将 numpy 数组转换为 PIL 图片**
这里采用 matplotlib.image 读入图片数组，注意这里读入的数组是 float32 型的，范围是 0-1，而 PIL.Image 数据是 uinit8 型的，范围是0-255，所以要进行转换：
```python
import
```
```python
matplotlib.image
```
```python
as
```
```python
mpimg
```
```python
from
```
```python
PIL
```
```python
import
```
```python
Image
lena = mpimg.imread(
```
```python
'lena.png'
```
```python
)
```
```python
# 这里读入的数据是 float32 型的，范围是0-1
```
```python
im = Image.fromarray(np.uinit8(lena*
```
```python
255
```
```python
))
im.show()
```
**5. RGB 转换为灰度图**
```python
from PIL import
```
```python
Image
```
```python
I
```
```python
=
```
```python
Image
```
```python
.open(
```
```python
'lena.png'
```
```python
)
```
```python
I
```
```python
.show()
L =
```
```python
I
```
```python
.convert(
```
```python
'L'
```
```python
)
L.show()
```

