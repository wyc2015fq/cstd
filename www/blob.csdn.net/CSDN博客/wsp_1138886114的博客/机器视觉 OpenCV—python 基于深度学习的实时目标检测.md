# 机器视觉 OpenCV—python 基于深度学习的实时目标检测 - wsp_1138886114的博客 - CSDN博客





2018年11月24日 08:49:46[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：602











### 文章目录
- [一、目标检测原理](#_1)
- [1.1 检测方法](#11__4)
- [1.2 背景模型](#12__13)
- [1.3 目标检测](#13__19)
- [1.4 后处理](#14__21)
- [二、基于深度学习实时目标检测](#_24)




### 一、目标检测原理

运动目标在工厂，监控，自动驾驶中有着举足轻重的地位。在做实时目标检测之前，我做过OpenCV的目标检测和运动轨迹及运动方向预判等，但这些都是基于OpenCV的，有一定的局限性。为此，从这篇博客开始将给大家带来一系列的实时目标检测，与大家一起学习。

##### 1.1 检测方法

**基于统计背景模型**的运动目标检测方法

（1） 背景获取：需要在场景存在运动目标的情况下获得背景图像

（2） 背景扰动：背景中可以含有轻微扰动的对象，如树枝、树叶的摇动，扰动部分不应该被看做是前景运动目标

（3） 外界光照变化：一天中不同时间段光线、天气等的变化对检测结果的影响

（4） 背景中固定对象的移动：背景里的固定对象可能移动，如场景中的一辆车开走、一把椅子移走，对象移走后的区域在一段时间内可能被误认为是运动目标，但不应该永远被看做是前景运动目标

（5） 背景的更新：背景中固定对象的移动和外界光照条件的变化会使背景图像发生变化，需要及时对背景模型进行更新，以适应这种变化

（6） 阴影的影响：通常前景目标的阴影也被检测为运动目标的一部分，这样将影响对运动目标的进一步处理和分析
##### 1.2 背景模型

**背景模型提取（建立模型背景）**

前提假设 在背景模型提取阶段，运动目标在场景区域中运动，不会长时间停留在某一位置

视频流中某一像素点只有在前景运动目标通过时，它的亮度值才发生大的变化，在一段时间内，亮度值主要集中在很小的一个区域中，可以用这个区域内的平均值作为该点的背景值。

具体实现过程：在YUV颜色空间下，Y值的变化范围为0~255，将该范围划分成若干区间[0,T][T,2T]…[Nt,255],n=255/T，对于每个像素点，统计一段时间内每个区间内亮度值的出现的次数。找出出现次数最多的那个区间，将该区间内所有值的平均值作为背景模型在该点的亮度值。这种方法不受前景运动目标的影响。
##### 1.3 目标检测

检测当前图像和背景图像中对应像素点的差异，如果差值大于一定阈值，则判定该像素为前景运动目标

##### 1.4 后处理

噪声的影响，会使检测结果中出现一些本身背景的区域像素点被检测成运动区域，也可能是运动目标内的部分区域被漏检。另外，背景的扰动，如树枝、树叶的轻微摇动，会使这部分也被误判断为运动目标。

为了消除这些影响：**首先**对上一步的检测结果用形态学的方法进行处理，**再**找出经过形态学处理的后的连通域，计算每个连通域中的面积，**然后**对于面积小于一定值的区域，将其抛弃，不看做是前景运动目标。

### 二、基于深度学习实时目标检测

为了确保运行不出错建议 环境 `Anaconda|python ==3.6.x` 或以上版本集成下述安装包

```python
from imutils.video import VideoStream
from imutils.video import FPS
import numpy as np
import argparse
import imutils
import time
import cv2


ap = argparse.ArgumentParser()
ap.add_argument("-p", "--prototxt", required=True,   # Caffe prototxt 文件路径。
	help="path to Caffe 'deploy' prototxt file")
ap.add_argument("-m", "--model", required=True,     # 预训练模型的路径。
	help="path to Caffe pre-trained model")
ap.add_argument("-c", "--confidence", type=float, default=0.2,
	help="minimum probability to filter weak detections")
args = vars(ap.parse_args())

# 初始化被 MobileNet SSD 用来训练检测的类标签列表，然后为每个类生成一组边框颜色
CLASSES = ["background", "aeroplane", "bicycle", "bird", "boat",
	"bottle", "bus", "car", "cat", "chair", "cow", "diningtable",
	"dog", "horse", "motorbike", "person", "pottedplant", "sheep",
	"sofa", "train", "tvmonitor"]
COLORS = np.random.uniform(0, 255, size=(len(CLASSES), 3))

# 加载序列化模型
print("[INFO] loading model...")
net = cv2.dnn.readNetFromCaffe(args["prototxt"], args["model"])

# 初始化视频流，允许CMAMER传感器预热，并初始化FPS计数器
print("[INFO] starting video stream...")
vs = VideoStream(src=0).start()
time.sleep(2.0)
fps = FPS().start()

# 从视频流循环帧
while True:
	# 从线程视频流抓取框架并调整其大小，使其最大宽度为400像素。
	frame = vs.read()
	frame = imutils.resize(frame, width=400)

	# 抓取框架尺寸并将其转换成块
	(h, w) = frame.shape[:2]
	blob = cv2.dnn.blobFromImage(cv2.resize(frame, (300, 300)),
		0.007843, (300, 300), 127.5)

	# 将blob通过网络并获得检测和预测
	net.setInput(blob)
	detections = net.forward()

	# 循环检测
	for i in np.arange(0, detections.shape[2]):
		# 提取与预测相关联的置信度（即概率）
		confidence = detections[0, 0, i, 2]

		# 通过保证“置信度”大于最小置信度来滤除弱检测，过滤小的干扰区域
		if confidence > args["confidence"]:
			# 从“detections”中提取类标签的索引，然后计算对象的边界框的(x，y)坐标 
			idx = int(detections[0, 0, i, 1])
			box = detections[0, 0, i, 3:7] * np.array([w, h, w, h])
			(startX, startY, endX, endY) = box.astype("int")

			# 画出帧上的目标
			label = "{}: {:.2f}%".format(CLASSES[idx],
				confidence * 100)
			cv2.rectangle(frame, (startX, startY), (endX, endY),
				COLORS[idx], 2)
			y = startY - 15 if startY - 15 > 15 else startY + 15
			cv2.putText(frame, label, (startX, y),
				cv2.FONT_HERSHEY_SIMPLEX, 0.5, COLORS[idx], 2)

	# 展示 frame
	cv2.imshow("Frame", frame)
	key = cv2.waitKey(1) & 0xFF   #会不会太快了
	if key == ord("q"):
		break
	fps.update()

# 停止计时器并显示FPS信息
fps.stop()
print("[INFO] elapsed time: {:.2f}".format(fps.elapsed()))
print("[INFO] approx. FPS: {:.2f}".format(fps.fps()))

cv2.destroyAllWindows()
vs.stop()

执行命令：
python real_time_object_detection.py \
	--prototxt MobileNetSSD_deploy.prototxt.txt \
	--model MobileNetSSD_deploy.caffemodel
```





