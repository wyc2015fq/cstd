
# OpenCV对象检测实例 - 喜欢打酱油的老鸟 - CSDN博客


2019年01月28日 08:56:37[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：167


[https://www.toutiao.com/a6644490462834983438/](https://www.toutiao.com/a6644490462834983438/)

2019-01-09 21:36:31
在本文中，我将演示如何跟踪传送带上的对象。这可以用于在传送带上定位对象，或者可以进一步用于执行质量控制和检查。我将在Windows上使用Python OpenCV库进行此演示。原始视频如下：
![OpenCV对象检测实例](http://p1.pstatp.com/large/pgc-image/3fac0bbefe994ae3b3c6d4bb16ea5cbc)
假设在视频的00:00:24和00:00:36之间，有我们要检测的对象。让我们从剪切我们最感兴趣视频部分开始。
```python
#import tools
from moviepy.video.io.ffmpeg_tools import ffmpeg_extract_subclip
#t1 and t2 correspond to beginning and end time in seconds
#target name corresponds to the output file name
ffmpeg_extract_subclip("Conveyor for Packaged Food.mp4", t1 = 24, t2=36, targetname="test.mp4")
```
![OpenCV对象检测实例](http://p3.pstatp.com/large/pgc-image/c0cae24e6e4d4299be481778d3215197)
一旦我们剪切后，我们现在可以开始编辑和操作我们的小视频，以生成精确的检测和跟踪运动中的对象。首先导入Python库
```python
# Libraries needed
import numpy as np
import cv2
import time
```
![OpenCV对象检测实例](http://p1.pstatp.com/large/pgc-image/6d86174afce045aaa7a256c396e3150a)
我们可以导入剪切视频并运行它。
```python
import numpy as np
import cv2
import time
cap = cv2.VideoCapture("test.mp4")
fgbg = cv2.createBackgroundSubtractorMOG2()
while(1):
 time.sleep(1/25) #This slows down the video
 ret, frame = cap.read()
 cv2.imshow("frame",frame)
 k = cv2.waitKey(1) & 0xff
 if k == 27:
 break
cap.release()
cv2.destroyAllWindows()
```
![OpenCV对象检测实例](http://p1.pstatp.com/large/pgc-image/81eba9c6baf444e7a452189a3d6e2432)
首先，我们可以提取移动对象的前景和背景（fgbg）。在下面的Python代码中，fgbg应用于main frame。它检测任何变化的像素并将它们的颜色设置为白色，同时保持所有静态像素为黑色。然后我们可以根据它们的强度和大小提取运动物体的轮廓。然后，提取位置（x，y）以及高度（h）和宽度（w）以围绕这些轮廓绘制矩形。请注意，这些框的位置和尺寸是从fgmask中提取的，并且实际上是在main frame上绘制的。
```python
fgmask = fgbg.apply(frame)
 (im2, contours, hierarchy) = cv2.findContours(fgmask.copy(),
 cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
 for c in contours:
 if cv2.contourArea(c) <= 1500 :
 continue 
 x,y,w,h = cv2.boundingRect(c)
 cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255,0), 2)
 cv2.imshow("fgmask",fgmask)
```
![OpenCV对象检测实例](http://p3.pstatp.com/large/pgc-image/fefe4861622c4f07a1de4ee07a5f7eb5)
如果我们包含上面的代码，我们会看到类似下面的图片：
![OpenCV对象检测实例](http://p1.pstatp.com/large/pgc-image/96df92260f704e7e9ee0b8073cf77ab3)
fgmask
然而，移动的像素不仅仅是输送带上的物体，输送带本身也有微小的运动，在背景中有框架上的反射，横条和设备。这将导致检测所有这些运动作为单独的对象。
![OpenCV对象检测实例](http://p1.pstatp.com/large/pgc-image/88f0ec0da460455597fd44a9bd2faf81)
初始噪声检测
要解决这个问题，我们可以做的第一件事是指定一个感兴趣的区域(ROI)，并引入一些去噪函数。这是通过包含以下Python代码行来实现的。
```python
pts = np.array([[700,5],[950,5],[730,600],[100,500]], np.int32)
 pts = pts.reshape((-1,1,2))
 new = cv2.polylines(frame,pts[:3],True,(0,0,0))
 mask = np.zeros((new.shape[0], new.shape[1]))
 cv2.fillConvexPoly(mask, pts, 1)
 mask = mask.astype(np.bool)
 roi = np.zeros_like(new)
 roi[mask] = new[mask] 
 
 blurred_frame = cv2.GaussianBlur(roi, (5,5), 0)
 fgmask = fgbg.apply(blurred_frame)
```
![OpenCV对象检测实例](http://p9.pstatp.com/large/pgc-image/81d9cd1a37eb4b89acca82a7403c72dc)
更改fgbg mask的参数以从提取的ROI中获取其图像也很重要。当物体在它们下方通过时，我们仍然存在横杆反射的问题。以下Python代码在这些区域上绘制线条以避免混淆。
```python
cv2.line(roi,(670,35),(920,55),(0,0,0),10)
 cv2.line(roi,(630,75),(920,100),(0,0,0),10) 
 cv2.line(roi,(540,130),(900,160),(0,0,0),13)
 cv2.line(roi,(465,210),(850,250),(0,0,0),13)
 cv2.line(roi,(320,320),(800,385),(0,0,0),18)
```
![OpenCV对象检测实例](http://p3.pstatp.com/large/pgc-image/ab188cb55b134b5eae347b42a2c8e18e)
最后，这将为我们提供干净的物体检测，噪音可以忽略不计。
![OpenCV对象检测实例](http://p3.pstatp.com/large/pgc-image/135be73c29a84bad8507197287fb6906)
应用ROI和去噪后的fgbg
![OpenCV对象检测实例](http://p3.pstatp.com/large/pgc-image/a01567a56ce6469d8d53cdeea8b1276b)
ROI and cross bars blackout
![OpenCV对象检测实例](http://p3.pstatp.com/large/pgc-image/3986d5e381ee4c06b6d9656b017d6c5c)
最终检测
总结一下，这是最终的Python代码：
```python
import numpy as np
import cv2
import time
cap = cv2.VideoCapture("test.mp4")
fgbg = cv2.createBackgroundSubtractorMOG2()
while(1):
 time.sleep(1/25)
 ret, frame = cap.read()
 pts = np.array([[700,5],[950,5],[730,600],[100,500]], np.int32)
 pts = pts.reshape((-1,1,2))
 new = cv2.polylines(frame,pts[:3],True,(0,0,0))
 mask = np.zeros((new.shape[0], new.shape[1]))
 cv2.fillConvexPoly(mask, pts, 1)
 mask = mask.astype(np.bool)
 roi = np.zeros_like(new)
 roi[mask] = new[mask] 
 cv2.line(roi,(670,35),(920,55),(0,0,0),10)
 cv2.line(roi,(630,75),(920,100),(0,0,0),10) 
 cv2.line(roi,(540,130),(900,160),(0,0,0),13)
 cv2.line(roi,(465,210),(850,250),(0,0,0),13)
 cv2.line(roi,(320,320),(800,385),(0,0,0),18)
 
 blurred_frame = cv2.GaussianBlur(roi, (5,5), 0)
 fgmask = fgbg.apply(blurred_frame)
 (im2, contours, hierarchy) = cv2.findContours(fgmask.copy(), cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
for c in contours:
 if cv2.contourArea(c) <= 1500 :
 continue 
 x,y,w,h = cv2.boundingRect(c)
 cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255,0), 2)
 cv2.imshow("frame",frame)
 cv2.imshow("fgmask",fgmask)
 cv2.imshow("ROI",roi)
 k = cv2.waitKey(1) & 0xff
 if k == 27:
 break
cap.release()
cv2.destroyAllWindows()
```
![OpenCV对象检测实例](http://p3.pstatp.com/large/pgc-image/0c5a53baaed24d8f937f2d104cf5009c)

