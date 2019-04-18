# OpenCV—Python 图像加减乘除-逻辑运算 - wsp_1138886114的博客 - CSDN博客





2018年09月27日 14:10:53[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：840








### 一、图像逻辑运算

##### 1.1 add—subtract—图像矩阵加减运算

```python
```python
"""
cv2.add(src1, src2, dst=None, mask=None, dtype=None)        加运算函数
cv2.subtract(src1, src2, dst=None, mask=None, dtype=None)   减运算函数
	src1：图像矩阵1
	src1：图像矩阵2
	dst： 默认选项
	mask：默认选项
	dtype：默认选项
"""

import cv2
import numpy as np

image = cv2.imread("F:\\img\\cat.jpg")
"""
图像image各像素加50
与image大小一样的矩阵
"""
M = np.ones(image.shape,dtype="uint8")*50 
added = cv2.add(image,M)             # 将图像image与M相减
subtracted = cv2.subtract(image,M)   # 将图像image与M相减

cv2.imshow("Original_img",image)     # 展示原图
cv2.imshow("Added",added)            # 加运算图  
cv2.imshow("subtracted",subtracted)  # 减运算图     
cv2.waitKey(0)
cv2.destroyAllWindows()
```
```

![在这里插入图片描述](https://img-blog.csdn.net/20180927084515617?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 1.2 bitwise_and ，bitwise_or 图像（与、或）运算

```python
```python
"""
cv2.bitwise_and(src1, src2, dst=None, mask=None) 取交集
cv2.bitwise_or(src1, src2, dst=None, mask=None)  取并集
	src1：图像矩阵1
	src1：图像矩阵2
	dst： 默认选项
	mask：默认选项
"""
```
```

##### 1.3、bitwise_xor、bitwise_not 图像（异或、非）运算

```python
```python
"""
cv2.bitwise_xor(src1, src2, dst=None, mask=None) 取不重叠的区域
cv2.bitwise_not(src1, src2, dst=None, mask=None) 取反
	src1：图像矩阵1
	src1：图像矩阵2
	dst： 默认选项
	mask：默认选项
"""
import cv2

img = cv2.imread("original_fapiao.png")
img2 = cv2.imread("extract_fapiao.png")

bitwiseNot = cv2.bitwise_not(img2)
bitwiseXor = cv2.bitwise_xor(img,bitwiseNot)

cv2.imshow("extract_fapiao_Not取反运算：",bitwiseNot)
cv2.imshow("bitwiseXor异或运算：",bitwiseXor)
cv2.imwrite("./extract_fapiao_Not.png",bitwiseNot)
cv2.imwrite("./fapiao_Not_Xor.png",bitwiseXor)
cv2.waitKey(0)
cv2.destroyAllWindows()
```
```

![在这里插入图片描述](https://img-blog.csdn.net/20180928082708169?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 将两张图片直接相减（异或）运算

```
import cv2
img = cv2.imread("original_fapiao.png")
img2 = cv2.imread("extract_fapiao.png")

bitwiseXor = cv2.bitwise_xor(img,img2 )

cv2.imshow("bitwiseXor异或运算：",bitwiseXor)
cv2.imwrite("./img_xor.png",bitwiseXor)
cv2.waitKey(0)
cv2.destroyAllWindows()
```

![在这里插入图片描述](https://img-blog.csdn.net/20180928083241973?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



