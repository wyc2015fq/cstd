# PIL之ImageGrab下的基本操作 - qq_37385726的博客 - CSDN博客





2018年08月18日 11:43:57[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：699








# **目录**



[1.屏幕截图](#1.%E5%B1%8F%E5%B9%95%E6%88%AA%E5%9B%BE)

[​](#%E8%BE%93%E5%87%BA)



# 1.屏幕截图

```python
from  PIL import Image
from PIL import ImageGrab

size = (300,300,400,400)
img = ImageGrab.grab(size)
img.save("cut.jpg")
img.show()
```

# ![](https://img-blog.csdn.net/20180818114318716?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



