
# python安装dlib，关键点检测 - 小花生的博客 - CSDN博客


2018年12月16日 21:37:13[Peanut_范](https://me.csdn.net/u013841196)阅读数：91



###### python 安装dlib Face Landmark Detection
dlib是人脸识别比较有名的库，有c++、Python的接口。使用dlib可以大大简化开发，比如人脸识别，特征点检测之类的工作都可以很轻松实现。
**关于dlib的安装，直接运行pip install dlib**即可。
e.g.：pip install dlib-19.6.0-cp36-cp36m-win_amd64.whl命令，即可安装成功。
**参考代码（关键点检测）：**
`# -*- coding:utf-8 -*- 
import cv2
import dlib
import numpy as np 
def key_points(src,img):
    points_keys = []
    PREDICTOR_PATH = "shape_predictor_68_face_landmarks.dat"
    detector = dlib.get_frontal_face_detector()
    predictor = dlib.shape_predictor(PREDICTOR_PATH)
    rects = detector(img,1)
    for i in range(len(rects)):
        landmarks = np.matrix([[p.x,p.y] for p in predictor(img,rects[i]).parts()])
        img = img.copy()
        for idx,point in enumerate(landmarks):
            pos = (point[0,0],point[0,1])
            points_keys.append([point[0,0],point[0,1]])
            cv2.circle(src,pos,2,(255,0,0),-1)
    print(points_keys)
    return src
if __name__ == '__main__':
    frame = cv2.imread("XZQ.jpg")
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    face_key = key_points(frame,gray)
    cv2.imshow('frame',face_key)
    cv2.imwrite('xzq_lanmark.jpg',face_key)
    cv2.waitKey(0)`**实验结果：**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181216213436524.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
---

###### 注：博众家之所长，集群英之荟萃。

