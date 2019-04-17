# Opencv之加图片边框 - qq_37385726的博客 - CSDN博客





2018年08月24日 10:03:54[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：136
所属专栏：[Opencv](https://blog.csdn.net/column/details/26515.html)









# **目录**

[一、copyMakeBorder](#%E4%B8%80%E3%80%81copyMakeBoarder)

[二、代码](#%E4%BA%8C%E3%80%81%E4%BB%A3%E7%A0%81)

# 一、copyMakeBorder

cv2.copyMakeBoarder(img,up,down,left,right,parameter)

其中parameter有：
** * BORDER_REPLICATE:     aaaaaa|abcdefgh|hhhhhhh**
** * BORDER_REFLECT:       fedcba|abcdefgh|hgfedcb * BORDER_REFLECT_101:   gfedcb|abcdefgh|gfedcba * BORDER_WRAP:          cdefgh|abcdefgh|abcdefg * BORDER_CONSTANT:      iiiiii|abcdefgh|iiiiiii  with some specified 'i'**





# 二、代码

```python
import cv2
import numpy as np



img1 = cv2.imread('./Image/reba_color.jpg')


#image,up,down,left,right,parameter
replicate = cv2.copyMakeBorder(img1,100,100,10,10,cv2.BORDER_REPLICATE)  
reflect = cv2.copyMakeBorder(img1,100,100,10,10,cv2.BORDER_REFLECT)
reflect101 = cv2.copyMakeBorder(img1,100,100,10,10,cv2.BORDER_REFLECT_101)
wrap = cv2.copyMakeBorder(img1,100,100,10,10,cv2.BORDER_WRAP)
constant= cv2.copyMakeBorder(img1,100,100,10,10,cv2.BORDER_CONSTANT,value=(255,255,255))


cv2.imshow('1',replicate)
cv2.imshow('2',reflect)
cv2.imshow('3',reflect101)
cv2.imshow('4',wrap)
cv2.imshow('5',constant)



cv2.waitKey(0)
```

> 
**cv2.BORDER_REPLICATE:     aaaaaa|abcdefgh|hhhhhhh**


![](https://img-blog.csdn.net/20180824095444190?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

> 
**cv2.BORDER_REFLECT:    aaaaaa|abcdefgh|hhhhhhh**


![](https://img-blog.csdn.net/20180824095642106?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

> 
**cv2.BORDER_REFLECT_101:   gfedcb|abcdefgh|gfedcba**


![](https://img-blog.csdn.net/20180824095737840?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

> 
**cv2.BORDER_WRAP:          cdefgh|abcdefgh|abcdefg**


![](https://img-blog.csdn.net/20180824095825942?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

> 
**cv2.BORDER_CONSTANT:      iiiiii|abcdefgh|iiiiiii  with some specified 'i'**


![](https://img-blog.csdn.net/20180824095901776?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)































