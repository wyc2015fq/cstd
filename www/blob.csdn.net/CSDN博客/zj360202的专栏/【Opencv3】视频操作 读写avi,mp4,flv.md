# 【Opencv3】视频操作 读写avi,mp4,flv - zj360202的专栏 - CSDN博客





2018年01月10日 18:37:13[zj360202](https://me.csdn.net/zj360202)阅读数：8086








### 1. 读视频文件



```python
import cv2

# This is a demo of running face recognition on a video file and saving the results to a new video file.
#
# PLEASE NOTE: This example requires OpenCV (the `cv2` library) to be installed only to read from your webcam.
# OpenCV is *not* required to use the face_recognition library. It's only required if you want to run this
# specific demo. If you have trouble installing it, try any of the other demos that don't require it instead.

# Open the input movie file
input_movie = cv2.VideoCapture("D:/work_test/object_predict.avi")
length = int(input_movie.get(cv2.CAP_PROP_FRAME_COUNT))

pre_file = '12_28_62_'
f_index = 0

while True:
    # Grab a single frame of video
    ret, frame = input_movie.read()

    # Quit when the input video file ends
    if not ret:
        break
    
    frame_resize = cv2.resize(frame, (640, 360), interpolation=cv2.INTER_AREA)
    # Write the resulting image to the output video file
    #print("Writing frame {} / {}".format(frame_number, length))
    cv2.imwrite("D:/work_test/op/"+pre_file+str(f_index)+".jpg", frame_resize)
    f_index += 1

# All done!
input_movie.release()
cv2.destroyAllWindows()
```
都文件avi和mp4没有任何区别，这里就不多说



读完的文件保存到一个目录下面



### 2. 写avi文件



```python
import os
import cv2

# This is a demo of running face recognition on a video file and saving the results to a new video file.
#
# PLEASE NOTE: This example requires OpenCV (the `cv2` library) to be installed only to read from your webcam.
# OpenCV is *not* required to use the face_recognition library. It's only required if you want to run this
# specific demo. If you have trouble installing it, try any of the other demos that don't require it instead.
length = 30

# Create an output movie file (make sure resolution/frame rate matches input video!)
fourcc = cv2.VideoWriter_fourcc(*'XVID')
output_movie = cv2.VideoWriter('output2_8.avi', fourcc, length, (1920, 1080))

frame_number = 0

file_path = 'D:/work_test/face_recognition-master/examples/2_8/'

for i in range(3127):
    # Grab a single frame of video
    frame = cv2.imread(file_path+"12_28_62_"+str(i)+".jpg")
    frame_number += 1

    # Write the resulting image to the output video file
    print("Writing frame {} / {}".format(frame_number, length))
    output_movie.write(frame)

# All done!
cv2.destroyAllWindows()
```
将一个目录下面的写到avi目录文件中，我们注意到





```python
fourcc = cv2.VideoWriter_fourcc(*'XVID')
```
用来指定格式的



opencv3支持的avi格式有

I420:  未压缩YUV颜色编码

PIMI:  MPEG-1编码

XVID: MPEG-4编码

### 3. 写mp4



```python
import os
import cv2

# This is a demo of running face recognition on a video file and saving the results to a new video file.
#
# PLEASE NOTE: This example requires OpenCV (the `cv2` library) to be installed only to read from your webcam.
# OpenCV is *not* required to use the face_recognition library. It's only required if you want to run this
# specific demo. If you have trouble installing it, try any of the other demos that don't require it instead.
length = 20

# Create an output movie file (make sure resolution/frame rate matches input video!)
#fourcc = cv2.VideoWriter_fourcc(*'MP4V')
fourcc = cv2.VideoWriter_fourcc('m', 'p', '4', 'v')
output_movie = cv2.VideoWriter('output2_9.mp4', fourcc, length, (640, 360))

frame_number = 0

file_path = 'D:/work_test/face_recognition-master/examples/2_1/'

for i in range(3127):
    # Grab a single frame of video
    frame = cv2.imread(file_path+"12_28_62_"+str(i)+".jpg")
    frame_number += 1

    # Write the resulting image to the output video file
    print("Writing frame {} / {}".format(frame_number, length))
    output_movie.write(frame)

# All done!
cv2.destroyAllWindows()
```



这里我们看到



```python
fourcc = cv2.VideoWriter_fourcc('m', 'p', '4', 'v')
```
其实以下三种写法是等价的





```python
fourcc = cv2.VideoWriter_fourcc('m', 'p', '4', 'v')
fourcc = cv2.VideoWriter_fourcc('M', 'P', '4', 'V')
fourcc = cv2.VideoWriter_fourcc(*'MP4V')
fourcc = cv2.VideoWriter_fourcc(*'mp4v')
```
由于mp4对于avi的压缩比比较好，所有常用mp4,这里我们实验的数据是avi 80.7M, mp4是50.4M，效果非常明显



### 4. 写flv文件，这里我们没有实验相应的代码，直接列出格式:FLV1



