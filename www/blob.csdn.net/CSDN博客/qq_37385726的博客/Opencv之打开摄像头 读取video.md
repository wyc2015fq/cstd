# Opencv之打开摄像头/读取video - qq_37385726的博客 - CSDN博客





2018年08月24日 15:23:46[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：221
所属专栏：[Opencv](https://blog.csdn.net/column/details/26515.html)









# **目录**

[一、代码](#%E4%B8%80%E3%80%81%E4%BB%A3%E7%A0%81)

[二、程序解释](#%E4%BA%8C%E3%80%81%E7%A8%8B%E5%BA%8F%E8%A7%A3%E9%87%8A)

# 一、代码

```python
import cv2

cap = cv2.VideoCapture(r"C:\Users\Administrator\Desktop\ml_summer\DSHfile\Image\video.mp4")   #0打开内置摄像头，路径打开系统视频

while True:
    _,frame = cap.read()
    cv2.imshow('now',frame)
    if cv2.waitKey(1)==ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
```



# 二、程序解释

1、cap = cv2.VideoCapture(0)

VideoCapture()中参数是0，表示打开笔记本的内置摄像头，参数是视频文件路径则打开视频，如cap = cv2.VideoCapture("../test.avi")

2、ret,frame = cap.read()

 cap.read()按帧读取视频，ret,frame是获cap.read()方法的两个返回值。其中`ret`是布尔值，如果读取帧是正确的则返回True，如果文件读取到结尾，它的返回值就为False。`frame`就是每一帧的图像，是个三维矩阵。

3、cv2.waitKey(1)，waitKey（）方法本身表示等待键盘输入，

参数是1，表示延时1ms切换到下一帧图像，对于视频而言；

参数为0，如cv2.waitKey(0)只显示当前帧图像，相当于视频暂停,；

参数过大如cv2.waitKey(1000)，会因为延时过久而卡顿感觉到卡顿。

c得到的是键盘输入的ASCII码，esc键对应的ASCII码是27，即当按esc键是if条件句成立

4、调用release()释放摄像头，调用destroyAllWindows()关闭所有图像窗口。



