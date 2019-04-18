# 机器视觉 OpenCV—python 图像数据集获取工具（视频取帧） - wsp_1138886114的博客 - CSDN博客





2018年10月11日 15:47:55[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：783








### 一、前言

之前在做图像分类的时候，人脸识别（开源代码）的练手，数据集获取麻烦（没人愿意将自己照片给人家做数据集），于是就用自己造数据集，但是拍照拍几百张训练效果不好，也嫌麻烦，干脆就是视频取帧的方式，在这之前使用专门的软件。不过opencv自带了视频处理的API，详细代码如下：

### 二、视频取帧代码

```python
import cv2
import os
import sys


input_path = sys.argv[1]            # 第一个输入参数是包含视频片段的路径
frame_interval = int(sys.argv[2])   # 第二个输入参数是设定每隔多少帧截取一帧
filenames = os.listdir(input_path)  # 列出文件夹下所有的视频文件


video_prefix = input_path.split(os.sep)[-1]   # 获取文件夹名称
frame_path = '{}_frames'.format(input_path)   # 新建文件夹，名称为原名加上_frames
if not os.path.exists(frame_path):
    os.mkdir(frame_path)

cap = cv2.VideoCapture()                     # 初始化一个VideoCapture对象

for filename in filenames:
    filepath = os.sep.join([input_path, filename])
    cap.open(filepath)                       # VideoCapture::open函数可以从文件获取视频
    n_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))     # 获取视频帧数
    
    for i in range(42):                     # 为了避免视频头几帧质量低下，黑屏或者无关等
        cap.read()
    for i in range(n_frames):
        ret, frame = cap.read()
        
        # 每隔frame_interval帧进行一次截屏操作
        if i % frame_interval == 0:  
            imagename = '{}_{}_{:0>6d}.jpg'.format(video_prefix, filename.split('.')[0], i)
            imagepath = os.sep.join([frame_path, imagename])
            print('exported {}!'.format(imagepath))
            cv2.imwrite(imagepath, frame)
cap.release()   # 执行结束释放资源
```



