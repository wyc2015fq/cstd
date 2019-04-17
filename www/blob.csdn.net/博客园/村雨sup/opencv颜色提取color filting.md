# opencv颜色提取color filting - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
# -*- coding: utf-8 -*
import numpy as np
import cv2

cap = cv2.VideoCapture(0)

while True:
    _ , frame = cap.read()
    hsv = cv2.cvtColor(frame,cv2.COLOR_BGR2HSV)
    lower_red = np.array([150,150,50])#在此调参
    upper_red = np.array([180,255,255])

    dark_red = np.uint8([[[12,22,121]]])
    dark_red = cv2.cvtColor(dark_red,cv2.COLOR_BGR2HSV)

    mask = cv2.inRange(hsv,lower_red,upper_red)
    res = cv2.bitwise_and(frame,frame,mask=mask)

    cv2.imshow('frame',frame)
    cv2.imshow('mask', mask)
    cv2.imshow('res', res)
    k = cv2.waitKey(5) & 0xff
    if k == 27:
        break

cv2.destroyAllWindows()
cap.release()
```

![](https://images2018.cnblogs.com/blog/1196023/201803/1196023-20180315235039304-175450277.png)

把红色提取了出来，有点模糊











