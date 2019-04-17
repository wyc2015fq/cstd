# 【opencv】Opencv（Python） 教程-轮廓（4）凹缺陷（凸缺陷）/点与轮廓关系/形状匹配 - zj360202的专栏 - CSDN博客





2018年01月26日 11:21:55[zj360202](https://me.csdn.net/zj360202)阅读数：1057








## 凹缺陷/凸缺陷


前面我们已经学习了轮廓的凸包，对象上的任何凹陷都被成为凸缺陷。OpenCV 中有一个函数 cv.convexityDefect() 可以帮助我们找到凸缺陷。函数调用如下：





**[python]**[view
 plain](http://blog.csdn.net/jjddss/article/details/73548564#)[copy](http://blog.csdn.net/jjddss/article/details/73548564#)



- import cv2  
- import numpy as np  
- img = cv2.imread('star.jpg')  
- img_gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)  
- ret, thresh = cv2.threshold(img_gray, 127, 255,0)  
- contours,hierarchy = cv2.findContours(thresh,2,1)  
- cnt = contours[0]  
- hull = cv2.convexHull(cnt,returnPoints = False)  ###返回凸包的点的坐标，returnPoints = False时反馈的坐标点在轮廓描述集合中点的编号，




**[python]**[view
 plain](http://blog.csdn.net/jjddss/article/details/73548564#)[copy](http://blog.csdn.net/jjddss/article/details/73548564#)



- #returnPoints = True时反馈的坐标点位置。为了后面cv2.convexityDefects的使用，此处必须为False

defects = cv2.convexityDefects(cnt,hull) ##反馈的是Nx4的数组，第一列表示的是起点（轮廓集合中点的编号）、第二列表示的是终点（轮廓集合中点的编号）
##第三列表示的是最远点（轮廓集合中点的编号），第四列表示的是最远点到凸轮廓的最短距离for i in range(defects.shape[0]): s,e,f,d = defects[i,0] start = tuple(cnt[s][0]) end = tuple(cnt[e][0]) far = tuple(cnt[f][0]) cv2.line(img,start,end,[0,255,0],2)
 cv2.circle(img,far,5,[0,0,255],-1)cv2.imshow('img',img)cv2.waitKey(0)cv2.destroyAllWindows()


![](https://img-blog.csdn.net/20160715144429194?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


## 点与轮廓关系


求解图像中的一个点到一个对象轮廓的最短距离。如果点在轮廓的外部，返回值为负。如果在轮廓上，返回值为 0。如果在轮廓内部，返回值为正。



下面我们以点（50，50）为例：





**[python]**[view
 plain](http://blog.csdn.net/jjddss/article/details/73548564#)[copy](http://blog.csdn.net/jjddss/article/details/73548564#)



- dist = cv2.pointPolygonTest(cnt,(50,50),True)  

此函数的第三个参数是 measureDist。如果设置为 True，就会计算最短距离。如果是 False，只会判断这个点与轮廓之间的位置关系（返回值为+1，-1，0）。

## 形状匹配


函数 cv2.matchShape() 可以帮我们比较两个形状或轮廓的相似度。如果返回值越小，匹配越好。它是根据 Hu 矩来计算的。文档中对不同的方法都有解释。





**[python]**[view
 plain](http://blog.csdn.net/jjddss/article/details/73548564#)[copy](http://blog.csdn.net/jjddss/article/details/73548564#)



- import cv2  
- import numpy as np  
- img1 = cv2.imread('star.jpg',0)  
- img2 = cv2.imread('star2.jpg',0)  
- ret, thresh = cv2.threshold(img1, 127, 255,0)  
- ret, thresh2 = cv2.threshold(img2, 127, 255,0)  
- contours,hierarchy = cv2.findContours(thresh,2,1)  
- cnt1 = contours[0]  
- contours,hierarchy = cv2.findContours(thresh2,2,1)  
- cnt2 = contours[0]  
- ret = cv2.matchShapes(cnt1,cnt2,1,0.0)  
- print ret  





