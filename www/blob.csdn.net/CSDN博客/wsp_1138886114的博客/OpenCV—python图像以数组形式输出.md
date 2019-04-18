# OpenCV—python图像以数组形式输出 - wsp_1138886114的博客 - CSDN博客





2018年09月21日 15:26:33[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：788








```python
```python
from PIL import Image
import numpy as np
import os


# 将图像（灰度图）以矩阵（数字）的的形式输出
img = Image.open("./cat02.png")
img_array = np.asarray(img)
print("输出图像数组：",img_array)
print("数组形状：",img_array.shape)


# 批量：图像（灰度图）以矩阵（数字）的的形式输出
img_array_list = []
list_dir = os.listdir("./image_dir/")
for image in list_dir:
    im = Image.open("./image_dir/"+image)
    print("输出图像数组：",np.asarray(im))
```
```

numpy 矩阵操作

```python
```python
img = np.zeros([3,3],np.float32)
print("img:\n",img)

img_out = np.zeros(img.shape + (3,))  #以img矩阵为基准生成增维的新矩阵
print("img_out:\n",img_out)

"""
img:
 [[0. 0. 0.]
 [0. 0. 0.]
 [0. 0. 0.]]
 
img_out:
 [[[0. 0. 0.]
  [0. 0. 0.]
  [0. 0. 0.]]

 [[0. 0. 0.]
  [0. 0. 0.]
  [0. 0. 0.]]

 [[0. 0. 0.]
  [0. 0. 0.]
  [0. 0. 0.]]]
"""
```
```



