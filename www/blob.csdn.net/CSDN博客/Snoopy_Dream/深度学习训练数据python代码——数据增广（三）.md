# 深度学习训练数据python代码——数据增广（三） - Snoopy_Dream - CSDN博客





2018年09月27日 22:51:04[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：548
所属专栏：[深度学习caffe常用python代码分享](https://blog.csdn.net/column/details/22599.html)[数据生成器+数据增广+常用工具](https://blog.csdn.net/column/details/29905.html)









# imgaug

[http://imgaug.readthedocs.io/en/latest/index.html](http://imgaug.readthedocs.io/en/latest/index.html)

## 安装

### 　 依赖
- numpy
- scipy
- scikit-image (pip install -U + scikit-image)
- six (pip install -U six)
- OpenCV (i.e. cv2 must be available in python). The library is mainly tested in OpenCV 2, but seems to also work in OpenCV 3.
`pip install　git+https://github.com/aleju/imgaug`
或者
`pip install imgaug`
前者安装github最新版本，后者安装pypi版本。　



```python
import imgaug as ia
from imgaug import augmenters as iaa
import numpy as np
import cv2

images = cv2.imread("./125573.png")

images = np.expand_dims(images, axis=0)#N C H W


plt.imshow(images[0])
plt.show()
```

```python
seq = iaa.Sequential([
#     iaa.Fliplr(0.5), # horizontal flips

#     iaa.Crop(percent=(0, 0.1)), # random crops

    # Small gaussian blur with random sigma between 0 and 0.5.
    # But we only blur about 50% of all images.
    iaa.Sometimes(0.5,
        iaa.GaussianBlur(sigma=(0.25, 0.75))
    ),
    
    # Strengthen or weaken the contrast in each image.
#     iaa.ContrastNormalization((0.75, 1.5)),
    
    # Add gaussian noise.
    # For 50% of all images, we sample the noise once per pixel.
    # For the other 50% of all images, we sample the noise per pixel AND
    # channel. This can change the color (not only brightness) of the
    # pixels.
    iaa.AdditiveGaussianNoise(loc=0, scale=(0.0, 0.05*255), per_channel=0.5),
    
    # Make some images brighter and some darker.
    # In 20% of all cases, we sample the multiplier once per channel,
    # which can end up changing the color of the images.
    iaa.Multiply((0.8, 1.2), per_channel=0.2),
    
    # Apply affine transformations to each image.
    # Scale/zoom them, translate/move them, rotate them and shear them.
    iaa.Affine(
        scale={"x": (0.9, 1.1), "y": (0.9, 1.1)},
        translate_percent={"x": (-0.1, 0.1), "y": (-0.1, 0.1)},
        rotate=(-6, 6),
        shear=(-4, 4)
    )
], random_order=True) # apply augmenters in random order
```

```python
images_aug = seq.augment_images(image)
```





