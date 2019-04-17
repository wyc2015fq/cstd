# python读取image - Keith - CSDN博客





2016年12月19日 14:13:22[ke1th](https://me.csdn.net/u012436149)阅读数：11539








# python 读取image

在python中我们有两个库可以处理图像文件，`scipy`和`matplotlib`.

## 安装库
`pip install matplotlib pillow scipy`
## 用法

```python
from scipy.misc import imread
data = imread(image_root)
#data是 ndarray对象
```

```python
import matplotlib.image as mpimg
data = mpimg.imread(image_root)
#data是 ndarray对象
```

## skimage

安装 `pip install -U scikit-image`

```python
from skimage.io import imread
img = imread(file_path) # 返回的是 ndarray
# 这里需要注意的是
# imread 读取 8-bit png 的时候莫名奇妙的读出个 3-channel 的图片
# from scipy.misc import imread 这个 imread 也是一个尿性
```

## PIL

安装 `pip install pillow`

```python
from PIL import Image
import numpy as np
img_obj = Image.open(file_path)
img_array = np.array(img_obj, dtype=np.uint8)

# 无论是 jpg 还是 png 都能正确读取
```

## matplotlib

安装 `pip install matplotlib`

```python
from matplotlib.image import imread
img = imread(img_path) # 返回 ndarray
# 这个imread 读 png 的时候，返回ndarray 的类型是 uint8
# 读 png 的时候，返回 ndarray 是 float32， 8-bit png 也能读出 3-channel，活在梦里
```



