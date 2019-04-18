# OpenCV—Python 轮廓检测 绘出矩形框（findContours\ boundingRect\rectangle） - wsp_1138886114的博客 - CSDN博客





2018年10月05日 16:52:48[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：6403











### 文章目录
- [1 获取轮廓](#1__1)
- [1.1 返回值：image, contours, hierarchy](#11_image_contours_hierarchy_53)
- [2 绘出轮廓](#2__60)
- [3 获取轮廓区域](#3__88)
- [4 获取物体最小外界矩阵](#4__98)




### 1 获取轮廓

OpenCV2获取轮廓主要是用 cv2.findContours()

```python
import cv2

img = cv2.imread('wujiaoxing.png')
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
ret,binary = cv2.threshold(gray, 127, 255, cv2.THRESH_BINARY)

_,contours, hierarchy = cv2.findContours(binary, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

draw_img0 = cv2.drawContours(img.copy(),contours,0,(0,255,255),3)
draw_img1 = cv2.drawContours(img.copy(),contours,1,(255,0,255),3)
draw_img2 = cv2.drawContours(img.copy(),contours,2,(255,255,0),3)
draw_img3 = cv2.drawContours(img.copy(), contours, -1, (0, 0, 255), 3)


print ("contours:类型：",type(contours))
print ("第0 个contours:",type(contours[0]))
print ("contours 数量：",len(contours))

print ("contours[0]点的个数：",len(contours[0]))
print ("contours[1]点的个数：",len(contours[1]))

cv2.imshow("img", img)
cv2.imshow("draw_img0", draw_img0)
cv2.imshow("draw_img1", draw_img1)
cv2.imshow("draw_img2", draw_img2)
cv2.imshow("draw_img3", draw_img3)

cv2.waitKey(0)
cv2.destroyAllWindows()

输出：
contours:类型： <class 'list'>
第0 个contours: <class 'numpy.ndarray'>
contours 数量： 3
contours[0]点的个数： 6
contours[1]点的个数： 74
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181029160913559.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_27,color_FFFFFF,t_70)

其中，cv2.findContours() 的第二个参数主要有- cv2.RETR_LIST：检测的轮廓不建立等级关系
- cv2.RETR_TREE：L建立一个等级树结构的轮廓。
- cv2.RETR_CCOMP：建立两个等级的轮廓，上面的一层为外边界，里面的一层为内孔的边界信息。
- cv2.RETR_EXTERNAL：表示只检测外轮廓

cv2.findContours() 的第三个参数 method为轮廓的近似办法
- cv2.CHAIN_APPROX_NONE存储所有的轮廓点，相邻的两个点的像素位置差不超过1，即max（abs（x1-x2），abs（y2-y1））==1
- cv2.CHAIN_APPROX_SIMPLE压缩水平方向，垂直方向，对角线方向的元素，只保留该方向的终点坐标，例如一个矩形轮廓只需4个点来保存轮廓信息
- cv2.CHAIN_APPROX_TC89_L1，CV_CHAIN_APPROX_TC89_KCOS使用teh-Chinl chain 近似算法

##### 1.1 返回值：image, contours, hierarchy
- contour返回值

cv2.findContours()函数首先返回一个list，list中每个元素都是图像中的一个轮廓，用numpy中的ndarray表示。
- hierarchy返回值

该函数还可返回一个可选的hiararchy结果，这是一个ndarray，其中的元素个数和轮廓个数相同，每个轮廓contours[i]对应4个hierarchy元素hierarchy[i][0] ~hierarchy[i][3]，分别表示后一个轮廓、前一个轮廓、父轮廓、内嵌轮廓的索引编号，如果没有对应项，则该值为负数。

### 2 绘出轮廓

cv2.drawContours()函数

cv2.drawContours(image, contours, contourIdx, color[, thickness[, lineType[, hierarchy[, maxLevel[, offset ]]]]])
- 第一个参数是指明在哪幅图像上绘制轮廓；
- 第二个参数是轮廓本身，在Python中是一个list。
- 第三个参数指定绘制轮廓list中的哪条轮廓，如果是-1，则绘制其中的所有轮廓。后面的参数很简单。其中thickness表明轮廓线的宽度，如果是-1（cv2.FILLED），则为填充模式。绘制参数将在以后独立详细介绍。

为了看到自己画了哪些轮廓，可以使用 `cv2.boundingRect()`函数获取轮廓的范围，即左上角原点，以及他的高和宽。然后用`cv2.rectangle()`方法画出矩形轮廓

```python
"""
x, y, w, h = cv2.boundingRect(img)   
    参数：
    img  是一个二值图
    x，y 是矩阵左上点的坐标，
    w，h 是矩阵的宽和高

cv2.rectangle(img, (x,y), (x+w,y+h), (0,255,0), 2)
    img：       原图
    (x，y）：   矩阵的左上点坐标
    (x+w，y+h)：是矩阵的右下点坐标
    (0,255,0)： 是画线对应的rgb颜色
    2：         线宽
"""
for i in range(0,len(contours)):  
    x, y, w, h = cv2.boundingRect(contours[i])   
    cv2.rectangle(image, (x,y), (x+w,y+h), (153,153,0), 5)
```

### 3 获取轮廓区域

```python
new_image=image[y+2:y+h-2,x+2:x+w-2]    # 先用y确定高，再用x确定宽
input_dir=("E:/cut_image/")
if not os.path.isdir(input_dir):
    os.makedirs(input_dir)
cv2.imwrite( nrootdir+str(i)+".jpg",newimage) 
print (i)
```

### 4 获取物体最小外界矩阵

使用 cv2.minAreaRect(cnt) ，返回点集cnt的最小外接矩形，cnt是所要求最小外接矩形的点集数组或向量，这个点集不定个数。

其中：cnt = np.array([[x1,y1],[x2,y2],[x3,y3],[x4,y4]])  # 必须是array数组的形式

rect = cv2.minAreaRect(cnt)    # 得到最小外接矩形的（中心(x,y), (宽,高), 旋转角度）

box = np.int0(cv2.boxPoints(rect))  #通过box会出矩形框
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181028164701590.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_27,color_FFFFFF,t_70)
鸣谢
[https://blog.csdn.net/loovelj/article/details/78739790](https://blog.csdn.net/loovelj/article/details/78739790)
[https://blog.csdn.net/hjxu2016/article/details/77833336](https://blog.csdn.net/hjxu2016/article/details/77833336)
[https://blog.csdn.net/sunny2038/article/details/12889059](https://blog.csdn.net/sunny2038/article/details/12889059)





