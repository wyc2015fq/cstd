# OpenCV中几何形状识别与测量-流浪的鱼-51CTO博客
经常看到有学习OpenCV不久的人提问，如何识别一些简单的几何形状与它们的颜色，其实通过OpenCV的轮廓发现与几何分析相关的函数，只需不到100行的代码就可以很好的实现这些简单几何形状识别与对象测量相关操作。本文就会演示给大家如何通过OpenCV 轮廓发现与几何分析相关函数实现如下功能：
- 几何形状识别（识别三角形、四边形/矩形、多边形、圆）
- 计算几何形状面积与周长、中心位置
- 
## 提取几何形状的颜色
在具体代码实现与程序演示之前，我们先要搞清楚一些概念。
### 一：基本概念与函数介绍
**1. 轮廓(contours)**
什么是轮廓，简单说轮廓就是一些列点相连组成形状、它们拥有同样的颜色、轮廓发现在图像的对象分析、对象检测等方面是非常有用的工具，在OpenCV中使用轮廓发现相关函数时候要求输入图像是二值图像，这样便于轮廓提取、边缘提取等操作。轮廓发现的函数与参数解释如下：  
```
findContours(image, mode, method, contours=None, hierarchy=None, offset=None)
- image输入/输出的二值图像
- mode 迒回轮廓的结构、可以是List、Tree、External
- method 轮廓点的编码方式，基本是基于链式编码
- contours 迒回的轮廓集合
- hieracrchy 迒回的轮廓层次关系
- offset 点是否有位移
```
**2. 多边形逼近**
多边形逼近，是通过对轮廓外形无限逼近，删除非关键点、得到轮廓的关键点，不断逼近轮廓真实形状的方法，OpenCV中多边形逼近的函数与参数解释如下：
```
approxPolyDP(curve, epsilon, closed, approxCurve=None)
- curve 表示输入的轮廓点集合
- epsilon 表示逼近曲率，越小表示相似逼近越厉害
- close 是否闭合
```
**3. 几何距计算**
图像几何距是图像的几何特征，高阶几何距中心化之后具有特征不变性，可以产
生Hu距输出，用于形状匹配等操作，这里我们通过计算一阶几何距得到指定轮廓的中心位置，计算几何距的函数与参数解释如下：
```
moments(array, binaryImage=None)
- array表示指定输入轮廓
- binaryImage默认为None
```
### 二：代码实现与演示
整个代码实现分为如下几步完成
- 加载图像，
- 图像二值化
- 轮廓发现
- 几何形状识别
- 测量周长、面积、计算中心
- 颜色提取
完整的源代码如下：
```
####################################################
#   作者:zhigang,
####################################################
import cv2 as cv
import numpy as np
class ShapeAnalysis:
    def __init__(self):
        self.shapes = {'triangle': 0, 'rectangle': 0, 'polygons': 0, 'circles': 0}
    def analysis(self, frame):
        h, w, ch = frame.shape
        result = np.zeros((h, w, ch), dtype=np.uint8)
        # 二值化图像
        print("start to detect lines...\n")
        gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
        ret, binary = cv.threshold(gray, 0, 255, cv.THRESH_BINARY_INV | cv.THRESH_OTSU)
        cv.imshow("input image", frame)
        out_binary, contours, hierarchy = cv.findContours(binary, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)
        for cnt in range(len(contours)):
            # 提取与绘制轮廓
            cv.drawContours(result, contours, cnt, (0, 255, 0), 2)
            # 轮廓逼近
            epsilon = 0.01 * cv.arcLength(contours[cnt], True)
            approx = cv.approxPolyDP(contours[cnt], epsilon, True)
            # 分析几何形状
            corners = len(approx)
            shape_type = ""
            if corners == 3:
                count = self.shapes['triangle']
                count = count+1
                self.shapes['triangle'] = count
                shape_type = "三角形"
            if corners == 4:
                count = self.shapes['rectangle']
                count = count + 1
                self.shapes['rectangle'] = count
                shape_type = "矩形"
            if corners >= 10:
                count = self.shapes['circles']
                count = count + 1
                self.shapes['circles'] = count
                shape_type = "圆形"
            if 4 < corners < 10:
                count = self.shapes['polygons']
                count = count + 1
                self.shapes['polygons'] = count
                shape_type = "多边形"
            # 求解中心位置
            mm = cv.moments(contours[cnt])
            cx = int(mm['m10'] / mm['m00'])
            cy = int(mm['m01'] / mm['m00'])
            cv.circle(result, (cx, cy), 3, (0, 0, 255), -1)
            # 颜色分析
            color = frame[cy][cx]
            color_str = "(" + str(color[0]) + ", " + str(color[1]) + ", " + str(color[2]) + ")"
            # 计算面积与周长
            p = cv.arcLength(contours[cnt], True)
            area = cv.contourArea(contours[cnt])
            print("周长: %.3f, 面积: %.3f 颜色: %s 形状: %s "% (p, area, color_str, shape_type))
        cv.imshow("Analysis Result", self.draw_text_info(result))
        cv.imwrite("D:/test-result.png", self.draw_text_info(result))
        return self.shapes
    def draw_text_info(self, image):
        c1 = self.shapes['triangle']
        c2 = self.shapes['rectangle']
        c3 = self.shapes['polygons']
        c4 = self.shapes['circles']
        cv.putText(image, "triangle: "+str(c1), (10, 20), cv.FONT_HERSHEY_PLAIN, 1.2, (255, 0, 0), 1)
        cv.putText(image, "rectangle: " + str(c2), (10, 40), cv.FONT_HERSHEY_PLAIN, 1.2, (255, 0, 0), 1)
        cv.putText(image, "polygons: " + str(c3), (10, 60), cv.FONT_HERSHEY_PLAIN, 1.2, (255, 0, 0), 1)
        cv.putText(image, "circles: " + str(c4), (10, 80), cv.FONT_HERSHEY_PLAIN, 1.2, (255, 0, 0), 1)
        return image
if __name__ == "__main__":
    src = cv.imread("D:/javaopencv/gem_test.png")
    ld = ShapeAnalysis()
    ld.analysis(src)
    cv.waitKey(0)
    cv.destroyAllWindows()
```
原图
![OpenCV中几何形状识别与测量](https://s1.51cto.com/images/blog/201804/16/8a5760fb07eaae1b7dcc40f09ce22682.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
运行结果：
![OpenCV中几何形状识别与测量](https://s1.51cto.com/images/blog/201804/16/890e0b78436732b2f9c6eee4f4edeced.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
控制台输出：
![OpenCV中几何形状识别与测量](https://s1.51cto.com/images/blog/201804/16/244b29b905b1feae9995c9933d050243.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
**欢迎继续关注本博客，学习OpenCV Python教程**
[Python+OpenCV3.3图像处理视频教程](http://edu.51cto.com/course/11324.html)
