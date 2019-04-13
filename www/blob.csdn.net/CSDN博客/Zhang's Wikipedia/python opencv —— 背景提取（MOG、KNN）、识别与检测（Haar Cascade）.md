
# python opencv —— 背景提取（MOG、KNN）、识别与检测（Haar Cascade） - Zhang's Wikipedia - CSDN博客


2018年05月01日 23:01:05[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：733


注意 opencv 的坐标轴，x 轴向右，和 width 对应，y 轴向下，和 height 对应；
## 1. MOG2 与 KNN
MOG：Mixture of Gaussian
importcv2
cap = cv2.VideoCapture('./data/video/768x576.avi')
knn_sub = cv2.createBackgroundSubtractorKNN()
mog2_sub = cv2.createBackgroundSubtractorMOG2()whileTrue:
    ret, frame = cap.read()ifnotret:breakmog_sub_mask = mog2_sub.apply(frame)
    knn_sub_mask = knn_sub.apply(frame)
    cv2.imshow('original', frame)
    cv2.imshow('MOG2', mog_sub_mask)
    cv2.imshow('KNN', knn_sub_mask)
    key = cv2.waitKey(30) &0xffifkey ==27orkey == ord('q'):breakcap.release()
cv2.destroyAllWindows()
## 2. 识别与检测
Haar Cascade
importcv2importnumpyasnp\# haar 级联分类器，opencv 源文件中data文件夹下会有\# https://github.com/opencv/opencv/tree/master/data/haarcascadesface_cascade = cv2.CascadeClassifier('./data/classifiers/haarcascade_frontalface_default.xml')
eye_cascade = cv2.CascadeClassifier('./data/classifiers/haarcascade_eye.xml')
cap = cv2.VideoCapture(0)whileTrue:
    ret, img = cap.read()ifnotret:breakgray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    faces = face_cascade.detectMultiScale(gray,1.3,5)for(x, y, w, h)infaces:
        cv2.rectangle(img, (x, y), (x+w, y+h), color=(255,0,0), thickness=2)
        roi_gray = gray[y:y+h, x:x+w]
        roi_color = img[y:y+h, x:x+w]
        eyes = eye_cascade.detectMultiScale(roi_gray)try:
            ex, ey, ew, eh = eyes[0]
            cv2.rectangle(roi_color, (ex, ey), (ex+ew, ey+eh), 
                color=(0,255,0), thickness=2)
            ex1, ey1, ew1, eh1 = eyes[1]
            cv2.rectangle(roi_color, (ex1, ey1), (ex1+ew1, ey1+eh1), 
                color=(0,255,0), thickness=2)exceptIndexError:
            print('')
    cv2.imshow('me', img)
    key = cv2.waitKey(30)ifkey ==27orkey == ord('q'):breakcap.release()
cv2.destroyAllWindows()

