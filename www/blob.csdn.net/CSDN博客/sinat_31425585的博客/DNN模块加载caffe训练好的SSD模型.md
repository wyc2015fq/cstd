# DNN模块加载caffe训练好的SSD模型 - sinat_31425585的博客 - CSDN博客
2018年09月12日 18:24:45[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：963
    opencv越来越强大了，可以直接对训练好的caffe、tensorflow等框架训练好的模型进行加载，进而完成识别、检测等任务。
    opencv加载caffe训练好的模型，采用readNetFromCaffe(arg1,arg2)，第一个参数对应定义模型结构的prototxt文件，第二个参数对应于训练好的model，加载完之后，使用blobFromImage函数，将图片转换成blob格式，网络接收输入数据后，通过forward()函数进行前向传播，即可得到网络输出的结果，检测视频其实也差不多，视频其实对应于一帧一帧的图像，我们只需要对视频中每一帧进行检测，即可得到对视频的检测结果。
```python
blobFromImage() 
Mat cv::dnn::blobFromImage	(InputArray 	image,
double 	scalefactor = 1.0,
const Size & 	size = Size(),
const Scalar & 	mean = Scalar(),
bool 	swapRB = true,
bool 	crop = true 
)		
Python:
retval	=	cv.dnn.blobFromImage(image[, scalefactor[, size[, mean[, swapRB[, crop]]]]]	)
```
   对应第一个参数为图像数据，第二个为缩放因子，第三个为缩放后图像大小，第四个为均值，第五个为交换颜色通道标志，第六个为是否进行裁剪操作。
    代码如下： 
```python
from imutils.video import VideoStream
from imutils.video import FPS
import numpy as np
import argparse
import imutils
import time
import cv2
import time
ap = argparse.ArgumentParser()
ap.add_argument("-p", "--prototxt", required=True, help="path to Caffe 'deploy' prototxt file")
ap.add_argument("-m", "--model", required=True, help="path to Caffe pre-trained model")
ap.add_argument("-c", "--confidence", type=float, default=0.2, help="min probability. to filter weak detections")
args = vars(ap.parse_args())
# initialize the list of class labels MobileNet SSD was trained to
# detect, then generate a set of bounding box colors for each class
CLASSES = ["background", "aeroplane", "bicycle", "bird", "boat",
	"bottle", "bus", "car", "cat", "chair", "cow", "diningtable",
	"dog", "horse", "motorbike", "person", "pottedplant", "sheep",
	"sofa", "train", "tvmonitor"]
COLORS = np.random.uniform(0, 255, size=(len(CLASSES), 3))
# load the serialized model from disk
print("[INFO] loading model...")
net = cv2.dnn.readNetFromCaffe(args["prototxt"], args["model"])
# initialize video stream, allow camera sensor to warmup and initialize FPS counter
video_stream = VideoStream(src=0).start()
time.sleep(2.0)
fps = FPS().start()
while True:
    # grab the frame from threaded video stream and resize it to a max width of 400
    frame = video_stream.read()
    frame = imutils.resize(frame, width=400)
    # grab the frame dimensions and convert it to blob
    (h, w) = frame.shape[:2]
    blob = cv2.dnn.blobFromImage(cv2.resize(frame, (300, 300)), 0.007843, (300, 300), 127.5)
    # pass the blob through network
    net.setInput(blob)
    detections = net.forward()
    # loop over the detections
    for i in np.arange(0, detections.shape[2]):
        # extract the confidence
        confidence = detections[0, 0, i, 2]
        # filter weak detections
        if confidence > args["confidence"]:
            # extract index of class label
            idx = int(detections[0, 0, i, 1])
            box = detections[0, 0, i, 3:7] * np.array([w, h, w, h])
            (startX, startY, endX, endY) = box.astype("int")
            # draw predictions in the frame
            label = "{}: {:.2f}%".format(CLASSES[idx], confidence* 100)
            cv2.rectangle(frame, (startX, startY), (endX, endY), COLORS[idx], 2)
            y = startY - 15 if startY - 15 > 15 else startY + 15
            cv2.putText(frame, label, (startX, y), cv2.FONT_HERSHEY_SIMPLEX, 0.5, COLORS[idx], 2)
    # show the output frame
    cv2.imshow("frame", frame)
    key = cv2.waitKey(1) & 0xFF
    # if `q` is pressed, break from loop
    if key == ord('q'):
        break
    # update the fps counter
    fps.update()
# stop the timer and display FPS information
fps.stop()
print("[INFO] elapsed time: {:.2f}".format(fps.elapsed()))
print("[INFO] approx. FPS: {:.2f}".format(fps.fps()))
# cleanup
cv2.destroyAllWindows()
video_stream.stop()
```
    输入命令：  
```python
python object_detection.py -p MobileNetSSD_deploy.prototxt.txt -m MobileNetSSD_deploy.caffemodel
```
   效果如下：
![](https://img-blog.csdn.net/20180912182415524?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
代码来源：[https://www.pyimagesearch.com/](https://www.pyimagesearch.com/)
