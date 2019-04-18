# OpenCV—Python视频的读取及保存 - wsp_1138886114的博客 - CSDN博客





2018年12月05日 10:32:14[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：309








运行环境 `Anaconda=5.3 | python=3.7`

### 一、从摄像头中获取视频

创建一个VideoCapture对象。它的参数可以是设备索引或视频文件的名称（下面会讲到）。设备索引只是指定哪台摄像机的号码。0代表第一台摄像机、1代表第二台摄像机。之后，可以逐帧捕捉视频。最后释放捕获。

```python
import cv2

cap = cv2.VideoCapture(0)

while(True):
    ret, frame = cap.read()                        #读取帧
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY) #灰度化展示
    cv2.imshow('frame',gray)
    if cv2.waitKey(1) & 0xFF == ord('q'):          #按‘q’退出
        break

#释放资源并关闭窗口
cap.release()
cv2.destroyAllWindows()
```

### 二、读取视频文件

从文件中播放视频，更改相机索引与视频文件名。 在显示帧时，选择适当的cv2.waitKey（）时间，如果该值太小，视频会非常快，如果它太大，视频会很慢（这可以用来慢动作显示视频）。 正常情况下，25毫秒即可。

```python
import cv2

cap = cv2.VideoCapture('IMG_1521.mp4')

while(cap.isOpened()):
    ret, frame = cap.read()

    cv2.imshow('frame',frame)
    if cv2.waitKey(25) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
```

##### 2.1 获取相机/视频的各种属性–常见函数
- **`cv2.VideoCapture.get(propId)`** 访问视频的某些功能，其中propId是一个从0到18的数字，每个数字表示视频的属性（Property Identifier）。
- **`retval = cv2.VideoCapture.set(propId,value)`**

其中一些值可以使用  cap.set(propId，value) 进行修改，value是修改后的值。

例如：通过cap.get（3）和cap.get（4）来检查帧的宽度和高度，默认的值是640x480。现修改为320x240，使用`ret = cap.set（3, 320）`和`ret = cap.set（4, 240）`。- **`retval,image= cv2.VideoCapture.read([,image])`** 抓取，解码并返回下一个视频帧。返回值为true表明抓取成功。该函数是组合了grab()和retrieve()，这是最方便的方法。如果没有帧，该函数返回false，并输出空图像。
- **`retval, image = cv2.VideoCapture.retrieve([, image[, flag]])`**  解码并返回抓取的视频帧
- **`retval = cv2.VideoCapture.grab()`** 从视频文件或相机中抓取下一帧。true为抓取成功。该函数主要用于多摄像头时。
- **`cv2.VideoCapture.release()`** 关闭视频文件或相机设备。

```python
retval = cv2.VideoCapture.get(propId)   #打开视频文件或者相机设备进行视频捕获。

propId 常见取值如下:
"""
cv2.CAP_PROP_POS_MSEC：     视频文件的当前位置（ms）
cv2.CAP_PROP_POS_FRAMES：   从0开始索引帧，帧位置。
cv2.CAP_PROP_POS_AVI_RATIO：视频文件的相对位置（0表示开始，1表示结束）
cv2.CAP_PROP_FRAME_WIDTH：  视频流的帧宽度。
cv2.CAP_PROP_FRAME_HEIGHT： 视频流的帧高度。
cv2.CAP_PROP_FPS：          帧率
cv2.CAP_PROP_FOURCC：       编解码器四字符代码
cv2.CAP_PROP_FRAME_COUNT：  视频文件的帧数
cv2.CAP_PROP_FORMAT：       retrieve()返回的Mat对象的格式。
cv2.CAP_PROP_MODE：         后端专用的值，指示当前捕获模式

cv2.CAP_PROP_BRIGHTNESS：图像的亮度，仅适用于支持的相机
cv2.CAP_PROP_CONTRAST：  图像对比度，仅适用于相机
cv2.CAP_PROP_SATURATION：图像饱和度，仅适用于相机
cv2.CAP_PROP_HUE：       图像色调，仅适用于相机
cv2.CAP_PROP_GAIN：      图像增益，仅适用于支持的相机
cv2.CAP_PROP_EXPOSURE：    曝光，仅适用于支持的相机
cv2.CAP_PROP_CONVERT_RGB：布尔标志，指示是否应将图像转换为RGB。
"""
```

### 三、视频保存

OpenCV提供了接口VideoWriter 用于视频的保存，
- **`<VideoWriter object> = cv.VideoWriter( filename, fourcc, fps, frameSize[, isColor] )`**
- 函数参数：
filename：给要保存的视频起个名字

fourcc：指定视频编解码器的4字节代码

【（‘P’，‘I’，‘M’，‘1’）是MPEG-1编解码器】

【（‘M’，‘J’，‘P’，'G '）是一个运动jpeg编解码器】

fps：帧率

frameSize：帧大小- **`retval = cv2.VideoWriter_fourcc( c1, c2, c3, c4 )`** 将4字符串接为fourcc代码。
- **`cv.VideoWriter.write( image )`** 将帧图像保存为视频文件。

isColor：如果为true，则视频为彩色，否则为灰度视频，默认为true

```python
import cv2

cap = cv2.VideoCapture(0)				#打开相机

#创建VideoWriter类对象
fourcc = cv2.VideoWriter_fourcc(*'XVID')
out = cv2.VideoWriter('output.avi',fourcc, 20.0, (640,480))

while(cap.isOpened()):
    ret, frame = cap.read()				#捕获一帧图像
    if ret==True:
        frame = cv2.flip(frame,0)			#沿x轴翻转
        out.write(frame)					#保存帧

        cv2.imshow('frame',frame)  		#显示帧
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    else:
        break

cap.release() #关闭相机
out.release()
cv2.destroyAllWindows()
```

```python
import cv2

cap = cv2.VideoCapture(0)

fourcc = cv2.VideoWriter_fourcc(*'XVID')
out = cv2.VideoWriter('output.avi',fourcc, 20.0, (640,480))

while(cap.isOpened()):
    ret, frame = cap.read()
    out.write(frame)
    cv2.imshow('frame',frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break


cap.release()
out.release()
cv2.destroyAllWindows()
```

鸣谢
[https://blog.csdn.net/qq_18995069/article/details/82772944](https://blog.csdn.net/qq_18995069/article/details/82772944)




