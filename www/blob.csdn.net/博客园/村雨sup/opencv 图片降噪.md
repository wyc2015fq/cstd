# opencv 图片降噪 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





——

```
# -*- coding: utf-8 -*
import numpy as np
import cv2

cap = cv2.VideoCapture(0)

while True:
    _ , frame = cap.read()
    hsv = cv2.cvtColor(frame,cv2.COLOR_BGR2HSV)
    lower_red = np.array([150,150,50])
    upper_red = np.array([180,255,255])

    dark_red = np.uint8([[[12,22,121]]])
    dark_red = cv2.cvtColor(dark_red,cv2.COLOR_BGR2HSV)

    mask = cv2.inRange(hsv,lower_red,upper_red)
    res = cv2.bitwise_and(frame,frame,mask=mask)

    kernel = np.ones((15,15),np.float32)/225
    smoothed = cv2.filter2D(res,-1,kernel)

    blur = cv2.GaussianBlur(res,(15,15),0)

    median = cv2.medianBlur(res,15)

    bilateral = cv2.bilateralFilter(res,15,75,75)

    cv2.imshow('frame',frame)
    cv2.imshow('res', res)
    cv2.imshow('smoothed', smoothed)
    cv2.imshow('blur', blur)
    cv2.imshow('median', median)
    cv2.imshow('bilateral', bilateral)
    
    k = cv2.waitKey(5) & 0xff
    if k == 27:
        break

cv2.destroyAllWindows()
cap.release()
```

![](https://images2018.cnblogs.com/blog/1196023/201803/1196023-20180316000642680-1752727256.png)

这么多钟smooth消除噪声











