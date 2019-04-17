# keras系列︱人脸表情分类与识别：opencv人脸检测+Keras情绪分类（四） - 素质云笔记/Recorder... - CSDN博客





2017年06月06日 18:42:44[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：23844








> 
人脸识别热门，表情识别更加。但是表情识别很难，因为人脸的微表情很多，本节介绍一种比较粗线条的表情分类与识别的办法。


### Keras系列：

1、[keras系列︱Sequential与Model模型、keras基本结构功能（一）](http://blog.csdn.net/sinat_26917383/article/details/72857454)

2、[keras系列︱Application中五款已训练模型、VGG16框架（Sequential式、Model式）解读（二）](http://blog.csdn.net/sinat_26917383/article/details/72859145)

3、[keras系列︱图像多分类训练与利用bottleneck features进行微调（三）](http://blog.csdn.net/sinat_26917383/article/details/72861152)

 4、[keras系列︱人脸表情分类与识别：opencv人脸检测+Keras情绪分类（四）](http://blog.csdn.net/sinat_26917383/article/details/72885715)

 5、[keras系列︱迁移学习：利用InceptionV3进行fine-tuning及预测、完整案例（五）](http://blog.csdn.net/sinat_26917383/article/details/72982230)
#### 本次讲述的表情分类是识别的分析流程分为：
- 1、加载pre-model网络与权重；
- 2、利用opencv的函数进行简单的人脸检测；
- 3、抠出人脸的图并灰化；
- 4、表情分类器检测

.

# 一、表情数据集

主要来源于kaggle比赛，[下载地址](https://www.kaggle.com/c/challenges-in-representation-learning-facial-expression-recognition-challenge/data)。 

有七种表情类别： (0=Angry, 1=Disgust, 2=Fear, 3=Happy, 4=Sad, 5=Surprise, 6=Neutral). 

数据是48x48 灰度图，格式比较奇葩。 

第一列是情绪分类，第二列是图像的numpy，第三列是train or test。  
![这里写图片描述](https://img-blog.csdn.net/20170606171044342?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

.
# 二、opencv的人脸识别

参考[《opencv+Recorder︱OpenCV 中使用 Haar 分类器进行面部检测》](http://blog.csdn.net/sinat_26917383/article/details/69831495)

理论略过，直接来看重点： 

（1）加载人脸检测器，haarcascade_frontalface_default.xml； 

（2）图片加载并灰化，cvtColor，可参考： [opencv︱图像的色彩空間cvtColor（HSV、HSL、HSB 、BGR）](http://blog.csdn.net/sinat_26917383/article/details/70860910)

（2）人脸探测，detectMultiScale.
```
# （1）加载人脸检测器
cascPath = '/.../haarcascade_frontalface_default.xml'
faceCascade = cv2.CascadeClassifier(cascPath)

# （2）图片加载并灰化
jpg_file = '/home/ubuntu/keras/image/8c80abb4gw1f3b5hxd3aaj20jg0cx411.jpg'
img_gray = cv2.imread(jpg_file)
img_gray = cv2.cvtColor(img_gray, cv2.COLOR_BGR2GRAY)

# 人脸探测
faces = faceCascade.detectMultiScale(
        img_gray,
        scaleFactor=1.1,
        minNeighbors=1,# minNeighbors=5比较难检测
        minSize=(30, 30),
        flags=cv2.cv.CV_HAAR_SCALE_IMAGE
    )
```

其中minNeighbors设置小一些，容易检测出来。这个检测器还是有点粗糙。 

.

# 三、表情分类与识别

本节源自github的[mememoji](https://github.com/JostineHo/mememoji)。 

网络结构： 
![这里写图片描述](https://img-blog.csdn.net/20170606183551463?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://github.com/JostineHo/mememoji/raw/master/figures/mynetarch.png)

> 
opencv中的人脸检测的pre-model文件（haarcascade_frontalface_default.xml）和表情识别pre-model文件（model.h5）都在[作者的github](https://github.com/JostineHo/real-time_emotion_analyzer)下载。


是利用Keras实现的。直接来看完整的代码：

```python
import cv2
import sys
import json
import time
import numpy as np
from keras.models import model_from_json


emotion_labels = ['angry', 'fear', 'happy', 'sad', 'surprise', 'neutral']

# load json and create model arch
json_file = open('/.../model.json','r')
loaded_model_json = json_file.read()
json_file.close()
model = model_from_json(loaded_model_json)

# load weights into new model
model.load_weights('/.../model.h5')

def predict_emotion(face_image_gray): # a single cropped face
    resized_img = cv2.resize(face_image_gray, (48,48), interpolation = cv2.INTER_AREA)
    # cv2.imwrite(str(index)+'.png', resized_img)
    image = resized_img.reshape(1, 1, 48, 48)
    list_of_list = model.predict(image, batch_size=1, verbose=1)
    angry, fear, happy, sad, surprise, neutral = [prob for lst in list_of_list for prob in lst]
    return [angry, fear, happy, sad, surprise, neutral]


# -------------------直接预测-----------------------
img_gray = cv2.imread('/.../real-time_emotion_analyzer-master/meme_faces/angry-angry.png')
img_gray = cv2.cvtColor(img_gray, cv2.COLOR_BGR2GRAY)
angry, fear, happy, sad, surprise, neutral = predict_emotion(img_gray)


# -------------------人脸预测-----------------------
# 加载检测器
cascPath = '/.../real-time_emotion_analyzer-master/haarcascade_frontalface_default.xml'
faceCascade = cv2.CascadeClassifier(cascPath)

# 图像灰化
jpg_file = '/.../001.jpg'
img_gray = cv2.imread(jpg_file)
img_gray = cv2.cvtColor(img_gray, cv2.COLOR_BGR2GRAY)

# 人脸检测
faces = faceCascade.detectMultiScale(
        img_gray,
        scaleFactor=1.1,
        minNeighbors=1,# minNeighbors=5比较难检测
        minSize=(30, 30),
        flags=cv2.cv.CV_HAAR_SCALE_IMAGE
    )

# 表情画框
for (x, y, w, h) in faces:
    face_image_gray = img_gray[y:y+h, x:x+w]
    cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)
    angry, fear, happy, sad, surprise, neutral = predict_emotion(face_image_gray)
```







