# Opencv—python 基于投影的字符分割 - wsp_1138886114的博客 - CSDN博客





2018年10月07日 09:47:22[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：1721标签：[Opencv—python 基于投影的字符分割](https://so.csdn.net/so/search/s.do?q=Opencv—python 基于投影的字符分割&t=blog)
个人分类：[OpenCV](https://blog.csdn.net/wsp_1138886114/article/category/8068053)









### 一、前言
- 字符分割有很多方法，根据自己的需要来分析，那种方法更加适合自己。大致适而言有两种方法：**投影分割法**和**连通域分割法**。
- 投影法的原理：利用二值化图片的像素的分布直方图进行分析，从而找出相邻字符的分界点进行分割。
- 投影有: 水平投影 和 垂直投影

### 二、代码

```python
```python
import cv2
import numpy as np
img = cv2.imread('./original_img.png', 0)

height, width = img.shape[:2]
(_, thresh) = cv2.threshold(img, 150, 255, cv2.THRESH_BINARY) 
kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (10, 10)) # 形态学处理:定义矩形结构
closed = cv2.erode(thresh, kernel, iterations = 5)           # 闭运算：迭代5次

height, width = closed.shape[:2]
v = [0]*width
z = [0]*height
a = 0

#垂直投影：统计并存储每一列的黑点数
for x in range(0, width):               
    for y in range(0, height):
        if closed[y,x][0] == 0:
            a = a + 1
        else :
            continue
    v[x] = a
    a = 0
    
#创建空白图片，绘制垂直投影图
l = len(v)
emptyImage = numpy.zeros((height, width, 3), numpy.uint8) 
for x in range(0,width):
    for y in range(0, v[x]):
        b = (255,255,255)
        emptyImage[y,x] = b


#水平投影  #统计每一行的黑点数
a = 0
emptyImage1 = numpy.zeros((height, width, 3), numpy.uint8) 
for y in range(0, height):
    for x in range(0, width):
        if closed[y,x][0] == 0:
            a = a + 1
        else :
            continue
    z[y] = a
    a = 0
    
#绘制水平投影图
l = len(z)
for y in range(0,height):
    for x in range(0, z[y]):
        b = (255,255,255)
        emptyImage1[y,x] = b

cv2.imshow('original_img',img)
cv2.imshow('erode',closed)
cv2.imshow('chuizhi', emptyImage)
cv2.imshow('shuipin', emptyImage1)
cv2.waitKey(0)
cv2.destroyAllWindows()
```
```

代码运行效果：投影
![在这里插入图片描述](https://img-blog.csdn.net/20181007094648119?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```python
```python
import cv2
import numpy
img = cv2.imread('D:/0.jpg',cv2.COLOR_BGR2GRAY)
height, width = img.shape[:2]

gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
(_, thresh) = cv2.threshold(gray, 140, 255, cv2.THRESH_BINARY) 
closed = cv2.erode(thresh, None, iterations = 7)
height, width = closed.shape[:2]

z = [0]*height
v = [0]*width
hfg = [[0 for col in range(2)] for row in range(height)]
lfg = [[0 for col in range(2)] for row in range(width)]
box = [0,0,0,0]
#水平投影
a = 0
emptyImage1 = numpy.zeros((height, width, 3), numpy.uint8) 
for y in range(0, height):
    for x in range(0, width):
        cp = closed[y,x]
        #if np.any(closed[y,x]):
        if cp == 0:
            a = a + 1
        else :
            continue
    z[y] = a
    #print z[y]
    a = 0
#根据水平投影值选定行分割点
inline = 1
start = 0
j = 0
for i in range(0,height):
    if inline == 1 and z[i] >= 150 :  #从空白区进入文字区
        start = i  #记录起始行分割点
        #print i
        inline = 0
    elif (i - start > 3) and z[i] < 150 and inline == 0 :  #从文字区进入空白区
        inline = 1
        hfg[j][0] = start - 2  #保存行分割位置
        hfg[j][1] = i + 2
        j = j + 1
 
#对每一行垂直投影、分割
a = 0
for p in range(0, j):
    for x in range(0, width):
        for y in range(hfg[p][0], hfg[p][1]):
            cp1 = closed[y,x]
            if cp1 == 0:
                a = a + 1
            else :
                continue
        v[x] = a  #保存每一列像素值
        a = 0
    #print width
    #垂直分割点
    incol = 1
    start1 = 0
    j1 = 0
    z1 = hfg[p][0]
    z2 = hfg[p][1]
    for i1 in range(0,width):
        if incol == 1 and v[i1] >= 20 :  #从空白区进入文字区
            start1 = i1  #记录起始列分割点
            incol = 0
        elif (i1 - start1 > 3) and v[i1] < 20 and incol == 0 :  #从文字区进入空白区
            incol = 1
            lfg[j1][0] = start1 - 2   #保存列分割位置
            lfg[j1][1] = i1 + 2
            l1 = start1 - 2
            l2 = i1 + 2
            j1 = j1 + 1
            cv2.rectangle(img, (l1, z1), (l2, z2), (255,0,0), 2)            
cv2.imshow('result', img)
cv2.waitKey(0)
cv2.destroyAllWindows()
```
```

分割效果不佳（有待优化）

```python
```python
import cv2
import numpy as np
img = cv2.imread('./0.png', cv2.COLOR_BGR2GRAY)

height, width = img.shape[:2]
data = np.array(img)
min_val = 10     #设置最小的文字像素高度，防止切分噪音字符

start_i = -1
end_i = -1
rowPairs = []    #存放每行的起止坐标

#行分割
for i in range(height):
    if(not data[i].all() and start_i < 0): 
        start_i = i
    elif(not data[i].all()):
        end_i = i
    elif (data[i].all() and start_i >= 0):
        if(end_i - start_i >= min_val):
            rowPairs.append((start_i, end_i))
        start_i, end_i = -1, -1

#列分割
start_j = -1
end_j = -1
min_val_word = 5  #最小文字像素长度
number = 0        #分割后保存编号

for start, end in rowPairs:
    for j in range(width):
        if(not data[start: end, j].all() and start_j < 0):
            start_j = j
        elif(not data[start: end, j].all()):
             end_j = j
        elif(data[start: end, j].all() and start_j >= 0):
            if(end_j - start_j >= min_val_word):
                tmp = data[start:end, start_j: end_j]
                cv2.imwrite("./" + '%d.png' % number, tmp) 
                number += 1
            start_j, end_j = -1, -1
```
```

鸣谢：
[https://blog.csdn.net/TIME_LEAF/article/details/79373162](https://blog.csdn.net/TIME_LEAF/article/details/79373162)
[https://blog.csdn.net/Tuzi294/article/details/79959199](https://blog.csdn.net/Tuzi294/article/details/79959199)




