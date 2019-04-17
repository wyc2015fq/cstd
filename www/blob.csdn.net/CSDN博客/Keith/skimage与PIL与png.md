# skimage与PIL与png - Keith - CSDN博客





2017年07月26日 12:15:41[ke1th](https://me.csdn.net/u012436149)阅读数：1132








# skimage 与 png

png 图像 有 png-8，png-24， png-32 之分，后面的数字代表一个 颜色用几位来存储，png-24就是我们常见的 RGB 图像啦。

使用 skimage.io 中 `imread(file_name)` 来读取 `png-8` 是不明智的，因为 `png-8` 能被读出来 3-channel。

对于 `png-8` 图像的正确读取方式是

```python
from PIL import Image
import numpy as np
file_path = '...' 
img_obj = Image.open(file_path)
img_arr = np.array(img_obj).astype(np.unit8)
```

## skimage.transform.resize

```python
resize(im, output_shape=(20, 20), order=0, mode='constant', preserve_range=True).astype(np.uint8)

# order: 0 代表最近邻插值，1代表双线性插值。。。
# preserve_range: True的话表示保持原有的 取值范围，false 的话就成 0-1 了
# 返回的是 float，有需要的可以强转一下类型
```

## 参考资料

[http://pillow.readthedocs.io/en/3.4.x/handbook/image-file-formats.html](http://pillow.readthedocs.io/en/3.4.x/handbook/image-file-formats.html)
[https://stackoverflow.com/questions/10965417/how-to-convert-numpy-array-to-pil-image-applying-matplotlib-colormap](https://stackoverflow.com/questions/10965417/how-to-convert-numpy-array-to-pil-image-applying-matplotlib-colormap)



