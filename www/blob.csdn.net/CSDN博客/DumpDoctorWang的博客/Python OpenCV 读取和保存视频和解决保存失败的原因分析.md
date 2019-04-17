# Python OpenCV 读取和保存视频和解决保存失败的原因分析 - DumpDoctorWang的博客 - CSDN博客





2018年05月30日 21:23:27[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：2561








### 一、安装OpenCV

```python
# 打开终端输入，没有装pip的话装上pip
pip install opencv-contrib-python
# 打开Python交互环境，打印opencv版本
import cv2
print(cv2.__version__)
```

### 二、读取和保存视频

```python
# coding=utf-8
import cv2

if __name__ == '__main__':
	# 打开视频
	reader = cv2.VideoCapture("./Videos/patio.mp4")
	print reader.isOpened()
	# 视频的宽度
	width = int(reader.get(cv2.CAP_PROP_FRAME_WIDTH))
	# 视频的高度
	height = int(reader.get(cv2.CAP_PROP_FRAME_HEIGHT))
	# 视频的帧率
	fps = reader.get(cv2.CAP_PROP_FPS)
	# 视频的编码
	fourcc = int(reader.get(cv2.CAP_PROP_FOURCC))

	# 定义视频输出
	writer = cv2.VideoWriter("./Videos/out.mp4", fourcc, fps, (width, height))

	have_more_frame = True
	while have_more_frame:
	have_more_frame, frame = reader.read()
	if have_more_frame:
	# 显示
	cv2.imshow('视频', frame)
	# 延时
	cv2.waitKey(24)
	# 写入新的视频
	writer.write(frame)
	reader.release()
	writer.release()
```

运行上一段代码，可能会遇到下面的错误

```
True

Failed to load OpenH264 library: openh264-1.7.0-win64.dll
Please check environment and/or download library: https://github.com/cisco/openh264/releases

[libopenh264 @ 000000001e84abe0] Incorrect library version loaded
Could not open codec 'libopenh264': Unspecified error

Process finished with exit code 0
```

这是因为没有编码库造成的原因，去它提示的链接，也就是https://github.com/cisco/openh264/releases，下载对应版的openh264，注意位数不要选错了，比如我的就是64位的Python，Windows，就下载[**openh264-1.7.0-win64.dll.bz2这个压缩包。下载完之后，解压到你的py文件所在目录，再次运行就不会报错。**](https://github.com/cisco/openh264/releases/download/v1.7.0/openh264-1.7.0-win64.dll.bz2)

### 三、视频保存失败的原因

多数是因为编码或者视频的宽度和写入的frame宽度不同，导致写入失败。因此，最好使用输入视频的编码(fourcc)，视频的宽度也要设置对。不知道处理后的图片的大小，那就打印下图片的大小就行了，保存视频时，视频的大小要和图片的大小一致。



