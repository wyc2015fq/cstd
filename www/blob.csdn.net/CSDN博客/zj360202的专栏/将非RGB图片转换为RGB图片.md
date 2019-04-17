# 将非RGB图片转换为RGB图片 - zj360202的专栏 - CSDN博客





2017年11月15日 11:34:06[zj360202](https://me.csdn.net/zj360202)阅读数：1745








有很多场景对图片的要求是RGB 3通道的，但是在网上抓的图片或是微信得到的图片，很多不是，这样就需要将他们转换一下

在caffe的应用中，可能会报



```java
OpenCV Error: Assertion failed ((scn == 3 || scn == 4) && (depth == CV_8U || depth == CV_32F)) in cvtColor, file /ssda/software/opencv-3.1.0/modules/imgproc/src/color.cpp, line 8141
```

在tensorflow中，应该就是报shape问题了


下面是解决问题的python 代码



```python
from PIL import Image
import os
path = "/home/hortatech/data/VOCdevkit/VOC2007/JPEGImages/"
for file_name in os.listdir(path):
    img =Image.open(path + file_name)
    if img.mode != "RGB" :
        print(file_name)
        img_rgb = img.convert("RGB")
        os.remove(path + file_name)
        img_rgb.save(path + file_name)
```





