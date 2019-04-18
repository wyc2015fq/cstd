# Python OpenCV cv2.xfeatures2d.SIFT_create detectAndCompute方法 - YZXnuaa的博客 - CSDN博客
2018年01月31日 19:20:50[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：3503
1、OpenCV 也提供了绘制关键点的函数: cv2.drawKeyPoints(),它可以在关键点的部位绘制一个小圆圈。如果你设置参数为 cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_就会绘制代表关键点大小的圆圈甚至可以绘制除关键点的方向。
第一个参数image：原始图像，可以使三通道或单通道图像；
第二个参数keypoints：特征点向量，向量内每一个元素是一个KeyPoint对象，包含了特征点的各种属性信息；
第三个参数outImage：特征点绘制的画布图像，可以是原图像；
第四个参数color：绘制的特征点的颜色信息，默认绘制的是随机彩色；
第五个参数flags：特征点的绘制模式，其实就是设置特征点的那些信息需要绘制，那些不需要绘制，有以下几种模式可选：
　　DEFAULT：只绘制特征点的坐标点,显示在图像上就是一个个小圆点,每个小圆点的圆心坐标都是特征点的坐标。 
　　DRAW_OVER_OUTIMG：函数不创建输出的图像,而是直接在输出图像变量空间绘制,要求本身输出图像变量就 是一个初始化好了的,size与type都是已经初始化好的变量
　　NOT_DRAW_SINGLE_POINTS：单点的特征点不被绘制 
　　DRAW_RICH_KEYPOINTS：绘制特征点的时候绘制的是一个个带有方向的圆,这种方法同时显示图像的坐 标,size，和方向,是最能显示特征的一种绘制方式。
2、这里的kps就是关键点。它所包含的信息有： 
angle：角度，表示关键点的方向，通过Lowe大神的论文可以知道，为了保证方向不变形，SIFT算法通过对关键点周围邻域进行梯度运算，求得该点方向。-1为初值。
class_id：当要对图片进行分类时，我们可以用class_id对每个特征点进行区分，未设定时为-1，需要靠自己设定
octave：代表是从金字塔哪一层提取的得到的数据。
pt：关键点点的坐标
response：响应程度，代表该点强壮大小，更确切的说，是该点角点的程度。
size：该点直径的大小
二、代码
import cv2
import numpy as np
def SIFT(img):
    I = cv2.imread(img)
    gray = cv2.cvtColor(I, cv2.COLOR_BGR2GRAY)
    descriptor = cv2.xfeatures2d.SIFT_create()
    kps, features = descriptor.detectAndCompute(gray, None)
    cv2.drawKeypoints(I,kps,I,(0,255,255),flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
    cv2.imshow('Vikings Detected!!', I)
    cv2.imwrite('sift_keypoints.jpg',I)
    cv2.waitKey(0)
if __name__ == '__main__':
    img = '/home/ubuntu/图片/webwxgetmsgimg.jpeg'
SIFT(img)![](https://img-blog.csdn.net/20180131191846306?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvWVpYbnVhYQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
我的女神～
