# Opencv之给图片加水印 - qq_37385726的博客 - CSDN博客





2018年08月23日 10:09:59[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：419
所属专栏：[Opencv](https://blog.csdn.net/column/details/26515.html)









# **目录**

[一、水印为英文](#%E4%B8%80%E3%80%81%E6%B0%B4%E5%8D%B0%E4%B8%BA%E8%8B%B1%E6%96%87)

[二、水印为中文](#%E4%BA%8C%E3%80%81%E6%B0%B4%E5%8D%B0%E4%B8%BA%E4%B8%AD%E6%96%87)

# 一、水印为英文

直接使用cv2中的putText

```python
import cv2

img1 = cv2.imread('./Image/reba_color.jpg',cv2.IMREAD_COLOR)

text = 'Python Opencv'
pos = (10,150)
font_type = 4
font_size = 2
color = (255,255,255)
bold = 1

#图片，文字，位置，字体，字号，颜色，厚度
cv2.putText(img1,text,pos, font_type, font_size, color,bold)
cv2.imshow('img',img1)
cv2.waitKey(0)
```



# 二、水印为中文

这种情况下处理较为复杂，因为cv2的putText直接输出中文会出现乱码的情况。

一般有两种解决办法：
- freetype模块，但这种方法及其麻烦，完全不建议尝试
- 利用PIL对象和opencv对象进行数据类型转换实现

> 
**opencv对象转PIL对象，注意将opencv的BGR通道转成RGB通道**


```python
Image.fromarray(cv2.cvtColor(img1, cv2.COLOR_BGR2RGB))
```

> 
**PIL对象转opencv对象，注意将PIL的RGB通道转成BGR通道**


```python
cv_img = cv2.cvtColor(np.asarray(pil_image),cv2.COLOR_RGB2BGR)
```



```python
import cv2
from PIL import Image,ImageFont,ImageDraw
import numpy as np

img1 = cv2.imread('./Image/reba_color.jpg',cv2.IMREAD_COLOR)

pil_image = Image.fromarray(cv2.cvtColor(img1, cv2.COLOR_BGR2RGB))

font = ImageFont.truetype('sarasa-mono-sc-regular.ttf', 40)
color = (255,255,255)
pos = (10,150)
text = u"我是水印"

draw = ImageDraw.Draw(pil_image)
draw.text(pos,text,font=font,fill=color)

cv_img = cv2.cvtColor(np.asarray(pil_image),cv2.COLOR_RGB2BGR)

cv2.imshow('imgg',cv_img)

cv2.waitKey(0)
```

![](https://img-blog.csdn.net/20180823100928981?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



