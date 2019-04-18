# OpenCV-Python教程（11、轮廓检测） - YZXnuaa的博客 - CSDN博客
2018年02月01日 16:06:46[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：153
个人分类：[Python库																[OpenCV](https://blog.csdn.net/YZXnuaa/article/category/7390843)](https://blog.csdn.net/YZXnuaa/article/category/7389269)
相比C++而言，Python适合做原型。本系列的文章介绍如何在Python中用OpenCV图形库，以及与C++调用相应OpenCV函数的不同之处。这篇文章介绍在Python中使用OpenCV检测并绘制轮廓。
**提示：**
- 转载请详细注明原作者及出处，谢谢！
- 本文介绍在OpenCV-Python中检测并绘制轮廓的方法。
- 本文不介详细的理论知识，读者可从其他资料中获取相应的背景知识。笔者推荐清华大学出版社的《[图像处理与计算机视觉算法及应用(第2版)](http://www.amazon.cn/%E5%9B%BE%E5%83%8F%E5%A4%84%E7%90%86%E4%B8%8E%E8%AE%A1%E7%AE%97%E6%9C%BA%E8%A7%86%E8%A7%89%E7%AE%97%E6%B3%95%E5%8F%8A%E5%BA%94%E7%94%A8-%E5%B8%95%E7%A7%91%E5%B0%94/dp/B0083PY050/ref=sr_1_1?ie=UTF8&qid=1373614562&sr=8-1&keywords=%E5%9B%BE%E5%83%8F%E5%A4%84%E7%90%86%E4%B8%8E%E8%AE%A1%E7%AE%97%E6%9C%BA%E8%A7%86%E8%A7%89%E7%AE%97%E6%B3%95%E5%8F%8A%E5%BA%94%E7%94%A8%28%E7%AC%AC2%E7%89%88%29)
 》。
# 轮廓检测
轮廓检测也是图像处理中经常用到的。OpenCV-Python接口中使用cv2.findContours()函数来查找检测物体的轮廓。
## 实现
使用方式如下：
**[python]**[view plain](http://blog.csdn.net/sunny2038/article/details/12889059#)[copy](http://blog.csdn.net/sunny2038/article/details/12889059#)
- import cv2  
- 
- img = cv2.imread('D:\\test\\contour.jpg')  
- gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)  
- ret, binary = cv2.threshold(gray,127,255,cv2.THRESH_BINARY)  
- 
- contours, hierarchy = cv2.findContours(binary,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)  
- cv2.drawContours(img,contours,-1,(0,0,255),3)  
- 
- cv2.imshow("img", img)  
- cv2.waitKey(0)  
需要注意的是cv2.findContours()函数接受的参数为二值图，即黑白的（不是灰度图），所以读取的图像要先转成灰度的，再转成二值图，参见4、5两行。第六行是检测轮廓，第七行是绘制轮廓。
## 结果
原图如下：
![](https://img-blog.csdn.net/20131030153346984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3VubnkyMDM4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
检测结果如下：
![](https://img-blog.csdn.net/20131030153441656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3VubnkyMDM4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**注意**，findcontours函数会“原地”修改输入的图像。这一点可通过下面的语句验证：
**[python]**[view plain](http://blog.csdn.net/sunny2038/article/details/12889059#)[copy](http://blog.csdn.net/sunny2038/article/details/12889059#)
- cv2.imshow("binary", binary)  
- contours, hierarchy = cv2.findContours(binary,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)  
- cv2.imshow("binary2", binary)  
执行这些语句后会发现原图被修改了。
# cv2.findContours()函数
函数的原型为
**[python]**[view plain](http://blog.csdn.net/sunny2038/article/details/12889059#)[copy](http://blog.csdn.net/sunny2038/article/details/12889059#)
- cv2.findContours(image, mode, method[, contours[, hierarchy[, offset ]]])  
返回两个值：contours：hierarchy。
## 参数
第一个参数是寻找轮廓的图像；
第二个参数表示轮廓的检索模式，有四种（本文介绍的都是新的cv2接口）：
    cv2.RETR_EXTERNAL表示只检测外轮廓
    cv2.RETR_LIST检测的轮廓不建立等级关系
    cv2.RETR_CCOMP建立两个等级的轮廓，上面的一层为外边界，里面的一层为内孔的边界信息。如果内孔内还有一个连通物体，这个物体的边界也在顶层。
    cv2.RETR_TREE建立一个等级树结构的轮廓。
第三个参数method为轮廓的近似办法
    cv2.CHAIN_APPROX_NONE存储所有的轮廓点，相邻的两个点的像素位置差不超过1，即max（abs（x1-x2），abs（y2-y1））==1
    cv2.CHAIN_APPROX_SIMPLE压缩水平方向，垂直方向，对角线方向的元素，只保留该方向的终点坐标，例如一个矩形轮廓只需4个点来保存轮廓信息
    cv2.CHAIN_APPROX_TC89_L1，CV_CHAIN_APPROX_TC89_KCOS使用teh-Chinl chain 近似算法
## 返回值
cv2.findContours()函数返回两个值，一个是轮廓本身，还有一个是每条轮廓对应的属性。
## contour返回值
cv2.findContours()函数首先返回一个list，list中每个元素都是图像中的一个轮廓，用numpy中的ndarray表示。这个概念非常重要。在下面drawContours中会看见。通过
**[python]**[view plain](http://blog.csdn.net/sunny2038/article/details/12889059#)[copy](http://blog.csdn.net/sunny2038/article/details/12889059#)
- print (type(contours))  
- print (type(contours[0]))  
- print (len(contours))  
可以验证上述信息。会看到本例中有两条轮廓，一个是五角星的，一个是矩形的。每个轮廓是一个ndarray，每个ndarray是轮廓上的点的集合。
由于我们知道返回的轮廓有两个，因此可通过
**[python]**[view plain](http://blog.csdn.net/sunny2038/article/details/12889059#)[copy](http://blog.csdn.net/sunny2038/article/details/12889059#)
- cv2.drawContours(img,contours,0,(0,0,255),3)  
和
**[python]**[view plain](http://blog.csdn.net/sunny2038/article/details/12889059#)[copy](http://blog.csdn.net/sunny2038/article/details/12889059#)
- cv2.drawContours(img,contours,1,(0,255,0),3)  
分别绘制两个轮廓，关于该参数可参见下面一节的内容。同时通过
**[python]**[view plain](http://blog.csdn.net/sunny2038/article/details/12889059#)[copy](http://blog.csdn.net/sunny2038/article/details/12889059#)
- print (len(contours[0]))  
- print (len(contours[1]))  
输出两个轮廓中存储的点的个数，可以看到，第一个轮廓中只有4个元素，这是因为轮廓中并不是存储轮廓上所有的点，而是只存储可以用直线描述轮廓的点的个数，比如一个“正立”的矩形，只需4个顶点就能描述轮廓了。
## hierarchy返回值
此外，该函数还可返回一个可选的hiararchy结果，这是一个ndarray，其中的元素个数和轮廓个数相同，每个轮廓contours[i]对应4个hierarchy元素hierarchy[i][0] ~hierarchy[i][3]，分别表示后一个轮廓、前一个轮廓、父轮廓、内嵌轮廓的索引编号，如果没有对应项，则该值为负数。
通过
**[python]**[view plain](http://blog.csdn.net/sunny2038/article/details/12889059#)[copy](http://blog.csdn.net/sunny2038/article/details/12889059#)
- print (type(hierarchy))  
- print (hierarchy.ndim)  
- print (hierarchy[0].ndim)  
- print (hierarchy.shape)  
得到
**[python]**[view plain](http://blog.csdn.net/sunny2038/article/details/12889059#)[copy](http://blog.csdn.net/sunny2038/article/details/12889059#)
- 3
- 2
- (1, 2, 4)  
可以看出，hierarchy本身包含两个ndarray，每个ndarray对应一个轮廓，每个轮廓有四个属性。
# 轮廓的绘制
OpenCV中通过cv2.drawContours在图像上绘制轮廓。  
## cv2.drawContours()函数
**[python]**[view plain](http://blog.csdn.net/sunny2038/article/details/12889059#)[copy](http://blog.csdn.net/sunny2038/article/details/12889059#)
- cv2.drawContours(image, contours, contourIdx, color[, thickness[, lineType[, hierarchy[, maxLevel[, offset ]]]]])  
- 第一个参数是指明在哪幅图像上绘制轮廓；
- 第二个参数是轮廓本身，在Python中是一个list。
- 第三个参数指定绘制轮廓list中的哪条轮廓，如果是-1，则绘制其中的所有轮廓。后面的参数很简单。其中thickness表明轮廓线的宽度，如果是-1（cv2.FILLED），则为填充模式。绘制参数将在以后独立详细介绍。
# 补充：
写着写着发现一篇文章介绍不完，所以这里先作为入门的。更多关于轮廓的信息有机会再开一篇文章介绍。
但有朋友提出计算轮廓的极值点。可用下面的方式计算得到，如下
**[python]**[view plain](http://blog.csdn.net/sunny2038/article/details/12889059#)[copy](http://blog.csdn.net/sunny2038/article/details/12889059#)
- pentagram = contours[1] #第二条轮廓是五角星
- 
- leftmost = tuple(pentagram[:,0][pentagram[:,:,0].argmin()])  
- rightmost = tuple(pentagram[:,0][pentagram[:,:,0].argmin()])  
- 
- cv2.circle(img, leftmost, 2, (0,255,0),3)   
- cv2.circle(img, rightmost, 2, (0,0,255),3)   
**注意！假设轮廓有100个点，OpenCV返回的ndarray的维数是(100, 1, 2)！！！而不是我们认为的(100, 2)。切记！！！**人民邮电出版社出版了一本《NumPy攻略：Python科学计算与数据分析》，推荐去看一下。
**更新：关于pentagram[:,0]的意思**
在numpy的数组中，用逗号分隔的是轴的索引。举个例子，假设有如下的数组：
**[python]**[view plain](http://blog.csdn.net/sunny2038/article/details/12889059#)[copy](http://blog.csdn.net/sunny2038/article/details/12889059#)
- a = np.array([[[3,4]], [[1,2]],[[5,7]],[[3,7]],[[1,8]]])  
其shape是(5, 1, 2)。与我们的轮廓是相同的。那么a[:,0]的结果就是：
**[python]**[view plain](http://blog.csdn.net/sunny2038/article/details/12889059#)[copy](http://blog.csdn.net/sunny2038/article/details/12889059#)
- [3,4], [1,2], [5,7], [3,7], [1,8]  
这里a[:,0]的意思就是a[0:5,0]，也就是a[0:5,0:0:2]，这三者是等价的。
回头看一下，a的shape是(5,1,2)，表明是三个轴的。在numpy的数组中，轴的索引是通过逗号分隔的。同时冒号索引“:”表示的是该轴的所有元素。因此a[:, 0]表示的是第一个轴的所有元素和第二个轴的第一个元素。在这里既等价于a[0:5, 0]。
再者，若给出的索引数少于数组中总索引数，则将已给出的索引树默认按顺序指派到轴上。比如a[0:5,0]只给出了两个轴的索引，则第一个索引就是第一个轴的，第二个索引是第二个轴的，而第三个索引没有，则默认为[:]，即该轴的所有内容。因此a[0:5,0]也等价于a[0:5,0:0:2]。
再详细一点，a的全体内容为：[[[3,4]], [[1,2]],[[5,7]],[[3,7]],[[1,8]]]。去掉第一层方括号，其中有五个元素，每个元素为[[3,4]]这样的，所以第一个索引的范围为[0:5]。**注意OpenCV函数返回的多维数组和常见的numpy数组的不同之处！**
观察[[3,4]]，我们发现其中只有一个元素，即[3, 4]，第二个索引为[0:1]。
再去掉一层方括号，我们面对的是[3,4]，有两个元素，所以第三个索引的范围为[0:2]。
再次强调一下OpenCVPython接口函数返回的NumPy数组和普通的NumPy数组在组织上的不同之处。
**PS：OpenCV-Python讨论群——219962286，欢迎大家加入互相探讨学习。**
得到的结果为如下：
![](https://img-blog.csdn.net/20131030171817578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3VubnkyMDM4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
