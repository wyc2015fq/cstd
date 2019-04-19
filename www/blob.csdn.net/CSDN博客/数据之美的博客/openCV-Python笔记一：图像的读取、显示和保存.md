# openCV-Python笔记一：图像的读取、显示和保存 - 数据之美的博客 - CSDN博客
2017年03月17日 17:36:34[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：2142
# 使用cv2.imread()，cv2.imshow()，cv2.imwrite()读取、显示和保存图像
## 一、读入图像
### 使用函数cv2.imread(filepath,flags)读入一副图片
- filepath：要读入图片的完整路径
- flags：读入图片的标志 
- cv2.IMREAD_COLOR：默认参数，读入一副彩色图片，忽略alpha通道
- cv2.IMREAD_GRAYSCALE：读入灰度图片
- cv2.IMREAD_UNCHANGED：顾名思义，读入完整图片，包括alpha通道
```python
import numpy as np
import cv2
img = cv2.imread('1.jpg',cv2.IMREAD_GRAYSCALE)
```
- 1
- 2
- 3
- 1
- 2
- 3
## 二、显示图像
### 使用函数cv2.imshow(wname,img)显示图像，第一个参数是显示图像的窗口的名字，第二个参数是要显示的图像（imread读入的图像），窗口大小自动调整为图片大小
```python
cv2.imshow('image',img)
cv2.waitKey(0)
cv2.destroyAllWindows()
#dv2.destroyWindow(wname)
```
- 1
- 2
- 3
- 4
- 1
- 2
- 3
- 4
- cv2.waitKey顾名思义等待键盘输入，单位为毫秒，即等待指定的毫秒数看是否有键盘输入，若在等待时间内按下任意键则返回按键的ASCII码，程序继续运行。若没有按下任何键，超时后返回-1。参数为0表示无限等待。不调用waitKey的话，窗口会一闪而逝，看不到显示的图片。
- cv2.destroyAllWindow()销毁所有窗口
- cv2.destroyWindow(wname)销毁指定窗口
## 三、保存图像
### 使用函数cv2.imwrite(file，img，num)保存一个图像。第一个参数是要保存的文件名，第二个参数是要保存的图像。可选的第三个参数，它针对特定的格式：对于JPEG，其表示的是图像的质量，用0 - 100的整数表示，默认95;对于png ,第三个参数表示的是压缩级别。默认为3.
### 注意:
- cv2.IMWRITE_JPEG_QUALITY类型为 long ,必须转换成 int
- cv2.IMWRITE_PNG_COMPRESSION, 从0到9 压缩级别越高图像越小。
```python
cv2.imwrite('1.png',img, [int( cv2.IMWRITE_JPEG_QUALITY), 95])
cv2.imwrite('1.png',img, [int(cv2.IMWRITE_PNG_COMPRESSION), 9])
```
- 1
- 2
- 1
- 2
## 四、图片操作
### 1、使用函数cv2.flip(img,flipcode)翻转图像，flipcode控制翻转效果。
- flipcode = 0：沿x轴翻转
- flipcode > 0：沿y轴翻转
- flipcode < 0：x,y轴同时翻转
```python
imgflip = cv2.flip(img,1)
```
- 1
- 1
### 2、复制图像
```python
imgcopy = img.copy()
```
- 1
- 1
### 3、颜色空间转换
```python
#彩色图像转为灰度图像
img2 = cv2.cvtColor(img,cv2.COLOR_RGB2GRAY) 
#灰度图像转为彩色图像
img3 = cv2.cvtColor(img,cv2.COLOR_GRAY2RGB)
# cv2.COLOR_X2Y，其中X,Y = RGB, BGR, GRAY, HSV, YCrCb, XYZ, Lab, Luv, HLS
```
- 1
- 2
- 3
- 4
- 5
- 1
- 2
- 3
- 4
- 5
## 示例
### 读入一副图像，按’s’键保存后退出，其它任意键则直接退出不保存
```python
import numpy as np
import cv2
img = cv2.imread('1.jpg',cv2.IMREAD_UNCHANGED)
cv2.imshow('image',img)
k = cv2.waitKey(0)
if k == ord('s'): # wait for 's' key to save and exit
    cv2.imwrite('1.png',img)
    cv2.destroyAllWindows()
else: 
    cv2.destroyAllWindows()
```
