# 人脸检测模块测试

## 测试数据: dt_1(一秒一帧，2228帧)
## 机器配置: i5-4590
## 操作系统: win 7 旗舰版

## 测试结果:

### 测试:1  参数 min_object_width = 30
|结果|误检|时间|来源|
|---|---|---|---|---|
|518|300+|40ms|opencv lbpcascade_frontalface.xml|
|368|6|30ms|my|
|444|21|60ms|于仕琪 https://github.com/ShiqiYu/libfacedetection |
|548|12|180ms|mtcnn https://github.com/foreverYoungGitHub/MTCNN|

### 测试:2  参数 min_object_width = 60
|结果|误检|时间|来源|
|---|---|---|---|---|
|281|200+|18ms|opencv lbpcascade_frontalface.xml |
|115|1|12ms|my|
|218|3|28ms|于仕琪 https://github.com/ShiqiYu/libfacedetection |
|393|2|40ms|mtcnn https://github.com/foreverYoungGitHub/MTCNN|
