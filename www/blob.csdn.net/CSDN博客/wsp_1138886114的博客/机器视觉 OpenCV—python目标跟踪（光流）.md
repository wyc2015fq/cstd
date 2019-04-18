# 机器视觉 OpenCV—python目标跟踪（光流） - wsp_1138886114的博客 - CSDN博客





2018年12月05日 16:59:11[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：1321











### 文章目录
- [一、运动检测](#_1)
- [1.1 检测思路](#11__2)
- [1.2 代码](#12__14)
- [二、运动方向预测](#_73)
- [2.1 关键点(角点)追踪 goodFeaturesToTrack()](#21__goodFeaturesToTrack_74)
- [2.2 光流法](#22__86)




### 一、运动检测

##### 1.1 检测思路

目标跟踪是对摄像头视频中的移动目标进行定位的过程。实时目标跟踪是许多计算机视觉应用的重要任务，如监控、基于感知的用户界面、增强现实、基于对象的视频压缩以及辅助驾驶等。

好久之前做过一次人脸检测，里面涉及到了目标跟踪。

这次实现一般的运动物体检测，关于实现视频目标跟踪的方法有很多，当跟踪所有移动目标时，帧之间的差异会变的有用；当跟踪视频中移动的手时，基于皮肤颜色的均值漂移方法是最好的解决方案；当知道跟踪对象的一方面时，模板匹配是不错的技术。

常用的目标跟踪方法有：
- 
背景法（本次用的方法）是：将一幅图作为背景，让后和每一帧对比；缺点是一开始存入的背景可能随光照变法而造成错误，但是可以用在光照环境稳定的地方，优点是可以检测之前背景没有的景象；

- 
差帧法是：将前一帧和后一帧进行对比；缺点是**无法对运动后突然又静止**的景象进行识别，优点是光照不影响；


##### 1.2 代码

```python
import cv2
import numpy as np

camera = cv2.VideoCapture(0) # 参数0表示第一个摄像头
if (camera.isOpened()):      # 判断视频是否打开
    print('Open')
else:
    print('摄像头未打开')

#测试用,查看视频size
size = (int(camera.get(cv2.CAP_PROP_FRAME_WIDTH)),
        int(camera.get(cv2.CAP_PROP_FRAME_HEIGHT)))
print('size:'+repr(size))

# 构建椭圆结果
es = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (9, 4))
kernel = np.ones((5, 5), np.uint8)
background = None

while True:
    # 读取视频流
    grabbed, frame_lwpCV = camera.read()

    # 对帧进行预处理，>>转灰度图>>高斯滤波（降噪：摄像头震动、光照变化）。
    gray_lwpCV = cv2.cvtColor(frame_lwpCV, cv2.COLOR_BGR2GRAY)
    gray_lwpCV = cv2.GaussianBlur(gray_lwpCV, (21, 21), 0)

    # 将第一帧设置为整个输入的背景
    if background is None:
        background = gray_lwpCV
        continue

    # 对比背景之后的帧与背景之间的差异，并得到一个差分图（different map）。
    # 阈值（二值化处理）>>膨胀（dilate）得到图像区域块
    diff = cv2.absdiff(background, gray_lwpCV)
    diff = cv2.threshold(diff, 25, 255, cv2.THRESH_BINARY)[1]
    diff = cv2.dilate(diff, es, iterations=2)

    # 显示矩形框：计算一幅图像中目标的轮廓
    image, contours, hierarchy = cv2.findContours(diff.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    for c in contours:
        if cv2.contourArea(c) < 1500:      # 对于矩形区域，只显示大于给定阈值的轮廓（去除微小的变化等噪点）
            continue
        (x, y, w, h) = cv2.boundingRect(c) # 该函数计算矩形的边界框
        cv2.rectangle(frame_lwpCV, (x, y), (x+w, y+h), (0, 255, 0), 2)

    cv2.imshow('contours', frame_lwpCV)
    cv2.imshow('dis', diff)

    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):    # 按'q'健退出循环
        break
# 释放资源并关闭窗口
camera.release()
cv2.destroyAllWindows()
```

### 二、运动方向预测

##### 2.1 关键点(角点)追踪 goodFeaturesToTrack()

```python
cv2.goodFeaturesToTrack(image,         #单通道
                        maxCorners,    #角点数目最大值，若检测的角点超过此值，则只返回前maxCorners个强角点
                        qualityLevel,  #角点的品质因子
                        minDistance,   #如果在其周围minDistance范围内存在其他更强角点，则将此角点删除 
                        corners        #存储所有角点
                        mask,          #指定感兴趣区，若无指定，寻找全图
                        blockSize,     #计算协方差矩阵时的窗口大小
                        useHarrisDetector,  #bool 是否使用Harris角点检测，如不指定，则计算shi-tomasi角点
                        k )           #Harris角点检测需要的k值
```

##### 2.2 光流法

光流法的**工作原理**基于如下假设：
- 连续的两帧图像之间，**目标像素亮度不变**。
- 相邻的像素之间有**相似的运动**。

考虑第一帧的像素 $I(x,y,t)$，表示在时间t时像素 $I(x,y)$ 的值。在经过时间 $d_t$ 后，此像素在下一帧移动了 $(d_x,d_y)$。因为这些像素是相同的，而且亮度不变，表示成，$I(x,y,t)=I(x+d_x,y+d_y,t+d_t)$。

假设移动很小，使用泰勒公式可以表示成：
$$I(x+Δx,y+Δy,t+Δt)=I(x,y,t)+\frac{∂I}{∂x}Δx+\frac{∂I}{∂y}Δy+\frac{∂I}{∂t}Δt+H.O.T$$

H.O.T是高阶无穷小。由第一个假设和使用泰勒公式展开的式子可以得到：
$$\frac{∂I}{∂x}Δx+\frac{∂I}{∂y}Δy+\frac{∂I}{∂t}Δt=0 ~~改写成:~~ \frac{∂I}{∂x}\frac{Δx}{Δt}+\frac{∂I}{∂y}\frac{Δy}{Δt}+\frac{∂I}{∂t}\frac{Δt}{Δt}=0$$

设：$\frac{∂I}{∂x}=f_x$ 同理得 $\frac{∂I}{∂y}=f_y$和 $\frac{∂I}{∂t}=f_t$

令$\frac{Δx}{Δt}=u$ ; $\frac{Δy}{Δt}=v$

**光流方程**: $f_xu+f_yv+f_t=0$ 其中 $f_x$ 和 $f_y$分别是图像的梯度，$f_t$ 是是图像沿着时间的梯度。但是$u$ 和 $v$是未知的，我们没办法用一个方程解两个未知数，那么就有了lucas-kanade这个方法来解决这个问题。

光流是进行视频中运动对象轨迹标记的一种很常用的方法，在OpenCV中实现光流也很容易。
`cv2.calcOpticalFlowPyrLK`函数计算一个稀疏特征集的光流，使用金字塔中的迭代 Lucas-Kanade 方法。
```python
nextPts,status,err = cv2.calcOpticalFlowPyrLK(prevImg,   #上一帧图片
                                              nextImg,   #当前帧图片
                                              prevPts,   #上一帧找到的特征点向量 
                                              nextPts    #与返回值中的nextPtrs相同
                                              [, status[, err[, winSize
                                              [, maxLevel[, criteria
                                              [, flags[, minEigThreshold]]]]]]])
```

返回值：
- nextPtrs 输出一个二维点的向量，这个向量可以是用来作为光流算法的输入特征点，也是光流算法在当前帧找到特征点的新位置（浮点数）
- status 标志，在当前帧当中发现的特征点标志status==1，否则为0
- err 向量中的每个特征对应的错误率

其他输入值：
- status 与返回的status相同
- err 与返回的err相同
- winSize 在计算局部连续运动的窗口尺寸（在图像金字塔中）
- maxLevel 图像金字塔层数，0表示不使用金字塔
- criteria 寻找光流迭代终止的条件
- flags 有两个宏，表示两种计算方法，
`OPTFLOW_USE_INITIAL_FLOW`表示使用估计值作为寻找到的初始光流，
`OPTFLOW_LK_GET_MIN_EIGENVALS`表示使用最小特征值作为误差测量- minEigThreshold 该算法计算光流方程的2×2规范化矩阵的最小特征值，除以窗口中的像素数; 如果此值小于minEigThreshold，则会过滤掉相应的功能并且不会处理该光流，因此它允许删除坏点并获得性能提升。

**实现原理**：

首先选取第一帧，在第一帧图像中检测Shi-Tomasi角点，

然后使用LK算法来迭代的跟踪这些特征点。迭代的方式就是不断向cv2.calcOpticalFlowPyrLK()中传入上一帧图片的特征点以及当前帧的图片。

函数会返回当前帧的点，这些点带有状态1或者0，如果在当前帧找到了上一帧中的点，那么这个点的状态就是1，否则就是0。
**实现流程**：
- 加载视频。
- 调用 GoodFeaturesToTrack 函数寻找兴趣点（关键点）。
- 调用 CalcOpticalFlowPyrLK 函数计算出两帧图像中兴趣点的移动情况。
- 删除未移动的兴趣点。
- 在两次移动的点之间绘制一条线段。

```python
import numpy as np
import cv2

cap = cv2.VideoCapture('./IMG_1521.mp4')

# ShiTomasi corner detection的参数
feature_params = dict(maxCorners=100,
                      qualityLevel=0.3,
                      minDistance=7,
                      blockSize=7)
# 光流法参数
# maxLevel 未使用的图像金字塔层数
lk_params = dict(winSize=(15, 15),
                 maxLevel=2,
                 criteria=(cv2.TERM_CRITERIA_EPS | cv2.TERM_CRITERIA_COUNT, 10, 0.03))

# 创建随机生成的颜色
color = np.random.randint(0, 255, (100, 3))


ret, old_frame = cap.read()                             # 取出视频的第一帧
old_gray = cv2.cvtColor(old_frame, cv2.COLOR_BGR2GRAY)  # 灰度化
p0 = cv2.goodFeaturesToTrack(old_gray, mask=None, **feature_params)
mask = np.zeros_like(old_frame)                         # 为绘制创建掩码图片

while True:
    _, frame = cap.read()
    frame_gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    # 计算光流以获取点的新位置
    p1, st, err = cv2.calcOpticalFlowPyrLK(old_gray, frame_gray, p0, None, **lk_params)
    # 选择good points
    good_new = p1[st == 1]
    good_old = p0[st == 1]
    # 绘制跟踪框
    for i, (new, old) in enumerate(zip(good_new, good_old)):
        a, b = new.ravel()
        c, d = old.ravel()
        mask = cv2.line(mask, (a, b), (c, d), color[i].tolist(), 2)
        frame = cv2.circle(frame, (a, b), 5, color[i].tolist(), -1)
    img = cv2.add(frame, mask)
    cv2.imshow('frame', img)
    k = cv2.waitKey(30)  # & 0xff
    if k == 27:
        break
    old_gray = frame_gray.copy()
    p0 = good_new.reshape(-1, 1, 2)

cv2.destroyAllWindows()
cap.release()
```

这是一个裸眼3D视频动画
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205164431976.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)
鸣谢
[https://blog.csdn.net/lwplwf/article/details/73526831](https://blog.csdn.net/lwplwf/article/details/73526831)
[https://blog.csdn.net/tengfei461807914/article/details/80978947](https://blog.csdn.net/tengfei461807914/article/details/80978947)
[https://blog.csdn.net/github_39611196/article/details/81166057](https://blog.csdn.net/github_39611196/article/details/81166057)








