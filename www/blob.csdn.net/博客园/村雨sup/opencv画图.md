# opencv画图 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
#coding=utf-8
import cv2
import numpy as np

img = cv2.imread("2.png",cv2.IMREAD_COLOR)
cv2.line(img,(0,0),(150,150),(255,0,0),15)
#画线段，起点，终点，颜色（BGR），线段粗细
cv2.rectangle(img,(10,10),(50,50),(0,255,0))
#长方形，长方形左上角，右下角
cv2.circle(img,(100,100),50,(0,0,255),-1)
#圆，圆心，半径，-1代表完全填充
pts = np.array([[10,5],[20,30],[70,20],[50,10]],np.int64)
#记录四个点的位置
cv2.polylines(img,[pts],True,(0,255,255),5)
#四个点连起来的一个折现圈
font = cv2.FONT_HERSHEY_SIMPLEX
cv2.putText(img,'Fuck you',(210,190),font,1,(255,255,255),5,cv2.LINE_AA)
#字的位置，字的大小，字的粗细
cv2.imshow('image',img)
cv2.waitKey (0)
cv2.destroyAllWindows()
```

![](https://images2018.cnblogs.com/blog/1196023/201803/1196023-20180310004949480-229133661.png)



```
#coding=utf-8
import cv2
import numpy as np

img = cv2.imread("2.png",cv2.IMREAD_COLOR)

img[55,55] = [255,255,255]
img[180:240,180:240] = [255,255,255]
#直接对图像的某一位置进行修改,修改成白色


watch_face = img[50:200,100:300]
img[0:150,0:200] = watch_face
#直接将图像从原图中扣下来

cv2.imshow('image',img)
cv2.waitKey (0)
cv2.destroyAllWindows()
```

![](https://images2018.cnblogs.com/blog/1196023/201803/1196023-20180310004926519-1268690907.png)

 ——













