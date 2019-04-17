# Opencv之python下车牌识别 - qq_37385726的博客 - CSDN博客





2018年09月03日 11:48:24[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：3831








# **目录**

[一、读入原始图片，灰度处理](#%E4%B8%80%E3%80%81%E8%AF%BB%E5%85%A5%E5%8E%9F%E5%A7%8B%E5%9B%BE%E7%89%87%EF%BC%8C%E7%81%B0%E5%BA%A6%E5%A4%84%E7%90%86)

[二、高斯模糊处理，去噪](#%E4%BA%8C%E3%80%81%E9%AB%98%E6%96%AF%E6%A8%A1%E7%B3%8A%E5%A4%84%E7%90%86%EF%BC%8C%E5%8E%BB%E5%99%AA)

[三、Sobel滤波，边缘检测](#%E4%B8%89%E3%80%81Sobel%E6%BB%A4%E6%B3%A2%EF%BC%8C%E8%BE%B9%E7%BC%98%E6%A3%80%E6%B5%8B)

[四、 Otsu大津算法自适应阈值二值化处理](#%E5%9B%9B%E3%80%81%20Otsu%E5%A4%A7%E6%B4%A5%E7%AE%97%E6%B3%95%E8%87%AA%E9%80%82%E5%BA%94%E9%98%88%E5%80%BC%E4%BA%8C%E5%80%BC%E5%8C%96%E5%A4%84%E7%90%86)

[五、形态学操作，闭操作](#%E4%BA%94%E3%80%81%E5%BD%A2%E6%80%81%E5%AD%A6%E6%93%8D%E4%BD%9C%EF%BC%8C%E9%97%AD%E6%93%8D%E4%BD%9C)

[六、轮廓提取](#%E5%85%AD%E3%80%81%E8%BD%AE%E5%BB%93%E6%8F%90%E5%8F%96)

[七、根据contours信息，构建外界矩形，并判断该轮廓是否合理](#%C2%A0%E4%B8%83%E3%80%81%E6%A0%B9%E6%8D%AEcontours%E4%BF%A1%E6%81%AF%EF%BC%8C%E6%9E%84%E5%BB%BA%E5%A4%96%E7%95%8C%E7%9F%A9%E5%BD%A2%EF%BC%8C%E5%B9%B6%E5%88%A4%E6%96%AD%E8%AF%A5%E8%BD%AE%E5%BB%93%E6%98%AF%E5%90%A6%E5%90%88%E7%90%86)

[八、对合理矩形（区域），进行floodFill泛洪处理  &  综合后续](#%E5%85%AB%E3%80%81%E5%AF%B9%E5%90%88%E7%90%86%E7%9F%A9%E5%BD%A2%EF%BC%88%E5%8C%BA%E5%9F%9F%EF%BC%89%EF%BC%8C%E8%BF%9B%E8%A1%8CfloodFill%E6%B3%9B%E6%B4%AA%E5%A4%84%E7%90%86%C2%A0%20%26%C2%A0%20%E7%BB%BC%E5%90%88%E5%90%8E%E7%BB%AD)

[九、完整代码](#%E4%B9%9D%E3%80%81%E5%AE%8C%E6%95%B4%E4%BB%A3%E7%A0%81)



基于[https://blog.csdn.net/jinshengtao/article/details/17883075](https://blog.csdn.net/jinshengtao/article/details/17883075)该博客下的Python实现

![](https://images0.cnblogs.com/blog/673793/201410/241156034496061.jpg)



## 一、读入原始图片，灰度处理

```python
# Step1  读入灰度图

initial_car = cv2.imread(r'F:\ml_summer\Opencv\Image\car.jpg')   #(600, 800, 3)  行，列，通道数


gray_car = cv2.cvtColor(initial_car,cv2.COLOR_BGR2GRAY)
```

原始图像

![](https://img-blog.csdn.net/20180903104930631?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 灰度图像

![](https://img-blog.csdn.net/2018090310495035?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)







## 二、高斯模糊处理，去噪

采用5*5模版对图像进行高斯模糊来退出由照相机或其他环境噪声（如果不这么做，我们会得到很多垂直边缘，导致错误检测。）

```python
# Step2  高斯模糊处理
blur_car = cv2.GaussianBlur(gray_car,(5,5),0)
```

关于
`cv2.GaussianBlur(img,kernel_size,sigMax)`
函数的具体情况，参见[https://blog.csdn.net/qq_37385726/article/details/82020214](https://blog.csdn.net/qq_37385726/article/details/82020214)

![](https://img-blog.csdn.net/2018090310410657?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



## 三、Sobel滤波，边缘检测

为了识别出车牌这个信息，我们有效利用车牌矩形的特征，边缘信息明显，故我们使用Sobel边缘检测的方法进行边缘的识别
`cv2.Sobel(img,dtype,dx,dy)    【dx是进行垂直边缘检测，dy是对于水平边缘检测】`
关于Sobel边缘检测，参见[https://blog.csdn.net/qq_37385726/article/details/82020725](https://blog.csdn.net/qq_37385726/article/details/82020725)

```python
#Step3  Sobel计算水平导数
sobel_car = cv2.Sobel(blur_car,cv2.CV_16S,1,0)
sobel_car = cv2.convertScaleAbs(sobel_car)  #转回uint8
```

![](https://img-blog.csdn.net/20180824161948781?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





## 四、 Otsu大津算法自适应阈值二值化处理

为去除掉背景的噪声，单独的处理车牌这一目标对象，我们将原始的灰度图像进行二值化处理。

利用Otus算法进行二值处理，利用
`cv2.threshold(img,threshold, maxval,type)`
实现，具体参见[https://blog.csdn.net/qq_37385726/article/details/82015545](https://blog.csdn.net/qq_37385726/article/details/82015545) （固定阈值的二值化处理可以实现大津算法）

更多过于二值化处理（自适应阈值二值化处理）参见[https://blog.csdn.net/qq_37385726/article/details/82017177](https://blog.csdn.net/qq_37385726/article/details/82017177)

```python
#Step4  Otsu大津算法自适应阈值二值化
_, otsu_car = cv2.threshold(sobel_car,0,255,cv2.THRESH_OTSU|cv2.THRESH_BINARY)
```

![](https://img-blog.csdn.net/20180903105037734?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



## 五、形态学操作，闭操作

利用形态学下的闭操作，将刚得到的二值化图像进行闭操作，**消除黑色小块，填充闭合区域**，将车牌区域连接起来，将车牌区域变成连通的区域，以便之后在轮廓提取的时候能将车牌作为一个区域提取出来

关于闭操作，参见[https://blog.csdn.net/qq_37385726/article/details/82021970](https://blog.csdn.net/qq_37385726/article/details/82021970)

```python
#Step5  闭操作
kernel = cv2.getStructuringElement(cv2.MORPH_RECT,(8,8))
close_car = cv2.morphologyEx(otsu_car,cv2.MORPH_CLOSE,kernel)
```

![](https://img-blog.csdn.net/2018090310575191?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



## 六、轮廓提取

从闭操作得到的结果图像，提取图像中的轮廓信息（点集），该点集是有一个list包裹的，list中的每一个元素都是一个numpy.ndarray 点的集合。该点的集合就是我们提取到的一个区域的轮廓信息。

关于

`cv2.findContours()`

参见

```python
#Step6  提取外部轮廓
img, contours, hierarchy = cv2.findContours(close_car,cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
```

其中contours就是我们提取到的点集list



##  七、根据contours信息，构建外界矩形，并判断该轮廓是否合理

因为车牌是规则的矩形，有其长宽的信息，我们可以对构建的外接矩形的长宽信息进行处理来判断该外接矩形是否和车牌接近。

【阈值：长宽比为4.727272，允许误差范围正负40%，面积范围15*15至125*125】

故定义verifySizes函数

```python
# 对minAreaRect获得的最小外接矩形，用纵横比进行判断
def verifySizes(RotatedRect):
    error = 0.4
    aspect = 4.7272
    min = 15 * aspect * 15
    max = 125 * aspect * 125
    rmin = aspect - aspect * error
    rmax = aspect + aspect * error
    height,width = RotatedRect[1]
    if height==0 or width==0:
        return False
    area = height * width
    r = width/height
    if r < 1:
        r = height/width
    if (area < min or area > max) or (r < rmin or r > rmax):
        return False
    else:
        return True
```

依据定义的验证函数，使用for循环一次遍历轮廓大点集中的点集集合

```python
# 对minAreaRect获得的最小外接矩形，用纵横比进行判断
save = []  #存储合理轮廓
rectall = []  #存储对应的在最小面积矩形

for contour in contours:
    rect = cv2.minAreaRect(contour)
    if verifySizes(rect):
        save.append(contour)
        rectall.append(rect)
```



下图为经过验证函数处理后得到的合理轮廓的点集，画成的轮廓图
`cv2.drawContours(initial_car,save,-1,(0,0,255),2)`
![](https://img-blog.csdn.net/20180903111448453?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





## 八、对合理矩形（区域），进行floodFill泛洪处理  &  综合后续

为了进一步提高效果，因为刚得到的合理的矩形所包括的车牌区域可能并不完整，所以我们使用泛洪处理来将得到的区域更为完整

step 1 : 利用合理矩形的中心点（rect[0]）为中心，生成十个周围的随机种子点

Step 2：对生成的十个随机种子点，依次使用cv2.floodFill泛洪算法进行处理

生成的随机种子点图片（种子点：黄色，以矩形中心作圆：红色）

![](https://img-blog.csdn.net/2018090311223792?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

关于
`cv2.floodFill(img,mask,(seed_x,seed_y),newvalue(b,g,r),(loDiff,loDiff,loDiff),(upDiff,upDiff,upDiff),flag)`
的详细信息，参见[https://blog.csdn.net/qq_37385726/article/details/82313004](https://blog.csdn.net/qq_37385726/article/details/82313004)


- 我们使用floodFill算法，对掩码层进行处理，则我们可以通过掩码层中被标记为255的像素点来判断是不是目标区域。
- 再将目标区域的点都保存成一个新点集，使用minAreaRect函数来提取最小面积矩形。
- 对最小面积矩形再次应用verifySize函数进行处理，得到合理的矩形
- 对得到的合理矩形的区域，在原图上进行图像切割
- 对切割后的图像，进行高斯模糊，去噪，直方图均衡化处理
- 保存

```python
#Step7  得到矩形中心附近随机数点
for step,rect in enumerate(rectall):
    x,y = rect[0]   #x:列数，y:行数
    x = int(x)
    y = int(y)

    cv2.circle(initial_car,(x,y),3,(0,255,0),2)

    width, height = rect[1]
    minimum = width if width<height else height
    minimum = 0.5*minimum

    h,w=initial_car.shape[:2]  #600,


    mask = np.zeros((h + 2, w + 2), dtype=np.uint8)

    for i in range(10):
        seed_x = int(x+0.5*(np.random.random_integers(0,100000)%int(minimum)-(minimum/2)))
        seed_y = int(y+0.5*(np.random.random_integers(0,100000)%int(minimum)-(minimum/2)))

        cv2.circle(initial_car,(seed_x,seed_y),1,(0,255,255))

        loDiff = 7.95
        upDiff = 30
        Connectivity = 4
        flag = Connectivity + (255<<8) + cv2.FLOODFILL_MASK_ONLY
        cv2.floodFill(initial_car,mask,(seed_x,seed_y),(255,0,0),(loDiff,loDiff,loDiff),(upDiff,upDiff,upDiff),flag)

    # cv2.imshow(str(step),mask)

    points = []
    row,column = mask.shape

    for i in range(row):
        for j in range(column):
            if mask[i][j]==255:
                points.append((j,i))   #点应该输入点坐标（列，行）
    points = np.asarray(points)
    new_rect = cv2.minAreaRect(points)

    if verifySizes(new_rect):
        # 宽,高
        x,y = new_rect[0]
        new_width, new_height = new_rect[1]
        angel = new_rect[2]
        point1 = cv2.boxPoints(new_rect)[0]
        point2 = cv2.boxPoints(new_rect)[1]
        point3 = cv2.boxPoints(new_rect)[2]
        point4 = cv2.boxPoints(new_rect)[3]

        # cv2.line(initial_car,tuple(point1),tuple(point2),(255,255,255),2)
        # cv2.line(initial_car, tuple(point2), tuple(point3), (255, 255, 255), 2)
        # cv2.line(initial_car, tuple(point3), tuple(point4), (255, 255, 255), 2)
        # cv2.line(initial_car, tuple(point4), tuple(point1), (255, 255, 255), 2)  #width
        rotate = cv2.getRotationMatrix2D((x,y),90+angel,1)
        res = cv2.warpAffine(initial_car,rotate,initial_car.shape[:2])

        #img,(列，行)，（中心）
        res = cv2.getRectSubPix(res,(int(new_height),int(new_width)),(x,y))

        #img,(列，行)
        res = cv2.resize(res,(105,25),interpolation=cv2.INTER_AREA)
        res = cv2.cvtColor(res,cv2.COLOR_BGR2GRAY)
        res = cv2.GaussianBlur(res,(3,3),0)
        res = cv2.equalizeHist(res)
        path = './Image/Sample/sample_'+str(step)+'.jpg'
        cv2.imwrite(path,res)
```

![](https://img-blog.csdn.net/20180903114652358?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180903114703951?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)







## 九、完整代码

```python
import cv2
import numpy as np
from matplotlib import pyplot as plt


# Step1  读入灰度图

initial_car = cv2.imread(r'F:\ml_summer\Opencv\Image\car.jpg')   #(600, 800, 3)  行，列，通道数


gray_car = cv2.cvtColor(initial_car,cv2.COLOR_BGR2GRAY)

# Step2  高斯模糊处理
blur_car = cv2.GaussianBlur(gray_car,(5,5),0)



#Step3  Sobel计算水平导数
sobel_car = cv2.Sobel(blur_car,cv2.CV_16S,1,0)
sobel_car = cv2.convertScaleAbs(sobel_car)  #转回uint8

#Step4  Otsu大津算法自适应阈值二值化
_, otsu_car = cv2.threshold(sobel_car,0,255,cv2.THRESH_OTSU|cv2.THRESH_BINARY)



#Step5  闭操作
kernel = cv2.getStructuringElement(cv2.MORPH_RECT,(8,8))
close_car = cv2.morphologyEx(otsu_car,cv2.MORPH_CLOSE,kernel)
# cv2.imshow('sss',close_car)

#Step6  提取外部轮廓
img, contours, hierarchy = cv2.findContours(close_car,cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)





save = []  #存储合理轮廓
rectall = []  #存储对应的在最小面积矩形

# 对minAreaRect获得的最小外接矩形，用纵横比进行判断
def verifySizes(RotatedRect):
    error = 0.4
    aspect = 4.7272
    min = 15 * aspect * 15
    max = 125 * aspect * 125
    rmin = aspect - aspect * error
    rmax = aspect + aspect * error
    height,width = RotatedRect[1]
    if height==0 or width==0:
        return False
    area = height * width
    r = width/height
    if r < 1:
        r = height/width
    if (area < min or area > max) or (r < rmin or r > rmax):
        return False
    else:
        return True



for contour in contours:
    rect = cv2.minAreaRect(contour)
    if verifySizes(rect):
        save.append(contour)
        rectall.append(rect)

# cv2.drawContours(initial_car,save,-1,(0,0,255),2)


#Step7  得到矩形中心附近随机数点
for step,rect in enumerate(rectall):
    x,y = rect[0]   #x:列数，y:行数
    x = int(x)
    y = int(y)

    cv2.circle(initial_car,(x,y),3,(0,255,0),2)

    width, height = rect[1]
    minimum = width if width<height else height
    minimum = 0.5*minimum

    h,w=initial_car.shape[:2]  #600,


    mask = np.zeros((h + 2, w + 2), dtype=np.uint8)

    for i in range(10):
        seed_x = int(x+0.5*(np.random.random_integers(0,100000)%int(minimum)-(minimum/2)))
        seed_y = int(y+0.5*(np.random.random_integers(0,100000)%int(minimum)-(minimum/2)))

        cv2.circle(initial_car,(seed_x,seed_y),1,(0,255,255))

        loDiff = 7.95
        upDiff = 30
        Connectivity = 4
        flag = Connectivity + (255<<8) + cv2.FLOODFILL_MASK_ONLY
        cv2.floodFill(initial_car,mask,(seed_x,seed_y),(255,0,0),(loDiff,loDiff,loDiff),(upDiff,upDiff,upDiff),flag)

    # cv2.imshow(str(step),mask)

    points = []
    row,column = mask.shape

    for i in range(row):
        for j in range(column):
            if mask[i][j]==255:
                points.append((j,i))   #点应该输入点坐标（列，行）
    points = np.asarray(points)
    new_rect = cv2.minAreaRect(points)

    if verifySizes(new_rect):
        # 宽,高
        x,y = new_rect[0]
        new_width, new_height = new_rect[1]
        angel = new_rect[2]
        point1 = cv2.boxPoints(new_rect)[0]
        point2 = cv2.boxPoints(new_rect)[1]
        point3 = cv2.boxPoints(new_rect)[2]
        point4 = cv2.boxPoints(new_rect)[3]

        # cv2.line(initial_car,tuple(point1),tuple(point2),(255,255,255),2)
        # cv2.line(initial_car, tuple(point2), tuple(point3), (255, 255, 255), 2)
        # cv2.line(initial_car, tuple(point3), tuple(point4), (255, 255, 255), 2)
        # cv2.line(initial_car, tuple(point4), tuple(point1), (255, 255, 255), 2)  #width
        rotate = cv2.getRotationMatrix2D((x,y),90+angel,1)
        res = cv2.warpAffine(initial_car,rotate,initial_car.shape[:2])

        #img,(列，行)，（中心）
        res = cv2.getRectSubPix(res,(int(new_height),int(new_width)),(x,y))

        #img,(列，行)
        res = cv2.resize(res,(105,25),interpolation=cv2.INTER_AREA)
        res = cv2.cvtColor(res,cv2.COLOR_BGR2GRAY)
        res = cv2.GaussianBlur(res,(3,3),0)
        res = cv2.equalizeHist(res)

        # 绘制直方图
        # hist = cv2.calcHist([res],[0],None,[256],[0,255])
        # plt.plot(hist,'r')
        # plt.hist(res.ravel(), 256, [0, 256],color='r')
        # plt.show()

        path = './Image/Sample/sample_'+str(step)+'.jpg'
        cv2.imwrite(path,res)

# cv2.imshow('now',initial_car)
cv2.waitKey(0)
```























