# opencv图像处理 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
#coding=utf-8
import cv2
import numpy as np

img1 = cv2.imread("3.jpg")
img2 = cv2.imread("4.jpg")
#图像大小不同不能合并，因此裁剪一下
img1 = img1[0:180,0:180]
img2 = img2[0:180,0:180]

img = img1 + img2
#这种图像加法与前面的一种不同之处在于，如果所加的和大于255，那就按255处理，所以看起来会白一些
add = cv2.add(img1,img2)
#带权值的加法
weight = cv2.addWeighted(img1,0.6,img2,0.4,0)


cv2.imshow('image',img)
cv2.imshow('add',add)
cv2.imshow('weight',weight)
cv2.waitKey (0)
cv2.destroyAllWindows()
```

![](https://images2018.cnblogs.com/blog/1196023/201803/1196023-20180310011920114-437723841.png)



——

![](https://images2018.cnblogs.com/blog/1196023/201803/1196023-20180310011834777-1188943227.png)

——



```
#coding=utf-8
import cv2
import numpy as np

img1 = cv2.imread("0.jpg")
img2 = cv2.imread("1.jpg")

# img2 = img2[0:180,0:180]
#在图一中找到和图二大小一样的那块区域
rows,cols,channels = img2.shape
roi = img1[0:rows,0:cols]
#转换为灰度图像
img2gray = cv2.cvtColor(img2,cv2.COLOR_BGR2GRAY)
#隔离图像上像素的边缘，仅与图像有关（即图像的白色区域剔除），下面函数将大于220像素的值置为0,小于的置为255
ret,mask = cv2.threshold(img2gray,220,255,cv2.THRESH_BINARY_INV)
#通过反转上面的图像创建掩码（因为不希望背景影响叠加）
mask_inv = cv2.bitwise_not(mask)
#使用位“与”运算来提取面具精确的边界
#取ROI中与mask中不为零的值对应的像素的值，其让值为0
img1_bg = cv2.bitwise_and(roi,roi,mask=mask_inv)
#取roi中与mask_inv中不为零的值对应的像素的值，其他值为0
img2_fg = cv2.bitwise_and(img2,img2,mask=mask)
#重合图像
dst = cv2.add(img1_bg,img2_fg)
img1[0:rows,0:cols] = dst


cv2.imshow('img2gry',img2gray)
cv2.imshow('mask',mask)
cv2.imshow('mask_inv',mask_inv)
cv2.imshow('img1_bg',img1_bg)
cv2.imshow('img2_fg',img2_fg)
cv2.imshow('dst',dst)
cv2.imshow('res',img1)


cv2.waitKey (0)
cv2.destroyAllWindows()
```

![](https://images2018.cnblogs.com/blog/1196023/201803/1196023-20180310233624587-1711742886.png)

原图：

![](https://images2018.cnblogs.com/blog/1196023/201803/1196023-20180310233700963-73710829.png)

最终效果图：

![](https://images2018.cnblogs.com/blog/1196023/201803/1196023-20180310233728270-1421702567.png)

https://www.kancloud.cn/aollo/aolloopencv/263215

http://blog.csdn.net/u011028345/article/details/77278467













