
# 基于图的图像分割（Graph-Based Image Segmentation） - 郭云飞的专栏 - CSDN博客


2017年12月06日 12:34:49[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：15055


**一、介绍**
基于图的图像分割（Graph-Based Image Segmentation），论文《Efficient Graph-Based Image Segmentation》，P. Felzenszwalb, D. Huttenlocher，International Journal of Computer Vision, Vol. 59, No. 2, September 2004
论文下载和论文提供的C++代码在[这里](http://cs.brown.edu/~pff/segment/)。
Graph-Based Segmentation是经典的图像分割算法，其作者Felzenszwalb也是提出DPM（Deformable Parts Model）算法的大牛。
Graph-Based Segmentation算法是基于图的贪心聚类算法，实现简单，速度比较快，精度也还行。不过，目前直接用它做分割的应该比较少，很多算法用它作垫脚石，比如Object Propose的开山之作《Segmentation as Selective Search for Object Recognition》就用它来产生过分割（over segmentation）。
**二、图的基本概念**
因为该算法是将图像用加权图抽象化表示，所以补充图的一些基本概念。
1、图
是由顶点集V（vertices）和边集E（edges）组成，表示为*G=(V, E)*，顶点*v∈V*，在论文即为单个的像素点，连接一对顶点的边*(vi, vj)*具有权重*w(vi, vj)*，本文中的意义为顶点之间的不相似度（dissimilarity），所用的是无向图。
2、树
特殊的图，图中任意两个顶点，都有路径相连接，但是没有回路。如下图中加粗的边所连接而成的图。如果看成一团乱连的珠子，只保留树中的珠子和连线，那么随便选个珠子，都能把这棵树中所有的珠子都提起来。
如果顶点i和h这条边也保留下来，那么顶点h,i,c,f,g就构成了一个回路。
![](https://img-blog.csdn.net/20171206104959304?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
3、最小生成树（minimum spanning tree）
特殊的树，给定需要连接的顶点，选择边权之和最小的树。
论文中，初始化时每一个像素点都是一个顶点，然后逐渐合并得到一个区域，确切地说是连接这个区域中的像素点的一个MST。如下图，棕色圆圈为顶点，线段为边，合并棕色顶点所生成的MST，对应的就是一个分割区域。分割后的结果其实就是森林。
![](https://img-blog.csdn.net/20171206105413116?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**三、相似性**
既然是聚类算法，那应该依据何种规则判定何时该合二为一，何时该继续划清界限呢？对于孤立的两个像素点，所不同的是灰度值，自然就用灰度的距离来衡量两点的相似性，本文中是使用RGB的距离，即
![](http://images.cnitblog.com/blog/460184/201407/212139044793634.png)
当然也可以用perceptually uniform的Luv或者Lab色彩空间，对于灰度图像就只能使用亮度值了，此外，还可以先使用纹理特征滤波，再计算距离，比如先做Census Transform再计算Hamming distance距离。
**四、全局阈值 >> 自适应阈值，区域的类内差异、类间差异**
上面提到应该用亮度值之差来衡量两个像素点之间的差异性。对于两个区域（子图）或者一个区域和一个像素点的相似性，最简单的方法即只考虑连接二者的边的不相似度。如下图，已经形成了棕色和绿色两个区域，现在通过紫色边来判断这两个区域是否合并。那么我们就可以设定一个阈值，当两个像素之间的差异（即不相似度）小于该值时，合二为一。迭代合并，最终就会合并成一个个区域，效果类似于区域生长：星星之火，可以燎原。
![](https://img-blog.csdn.net/20171206110045764?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
举例说明：
![](https://img-blog.csdn.net/20171206111022215?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
对于上右图，显然应该聚成上左图所示的3类：高频区h,斜坡区s,平坦区p。
如果我们设置一个全局阈值，那么如果h区要合并成一块的话，那么该阈值要选很大，但是那样就会把p和s区域也包含进来，分割结果太粗。如果以p为参考，那么阈值应该选特别小的值，那样的话p区是会合并成一块，但是h区就会合并成特别特别多的小块，如同一面支离破碎的镜子，分割结果太细。显然，全局阈值并不合适，那么自然就得用自适应阈值。对于p区该阈值要特别小，s区稍大，h区巨大。
先来两个定义，原文依据这两个附加信息来得到自适应阈值。
一个区域内的类内差异*Int(C)*：
![](http://images.cnitblog.com/blog/460184/201407/212139059799375.png)
可以近似理解为一个区域内部最大的亮度差异值，定义是MST中不相似度最大的一条边。
俩个区域的类间差异*Diff(C1, C2)*：
![](http://images.cnitblog.com/blog/460184/201407/212139078857059.png)
即连接两个区域所有边中，不相似度最小的边的不相似度，也就是两个区域最相似的地方的不相似度。
直观的判断，当：
![](http://images.cnitblog.com/blog/460184/201407/212139082135961.png)
时，两个区域应当合并！
**五、算法步骤**
1、计算每一个像素点与其8邻域或4邻域的不相似度。
![](https://img-blog.csdn.net/20171206112900548?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
如上图，实线为只计算4领域，加上虚线就是计算8邻域，由于是无向图，按照从左到右，从上到下的顺序计算的话，只需要计算右图中灰色的线即可。
2、将边按照不相似度non-decreasing排列（从小到大）排序得到*e1, e2, ..., en*。
3、选择*ei*
4、对当前选择的边**ej**（vi和vj不属于一个区域）进行合并判断。设其所连接的顶点为*(vi, vj)*，
if 不相似度小于二者内部不相似度：
5、更新阈值以及类标号
else：
6、如果i < n，则按照排好的顺序，选择下一条边转到Step 4，否则结束。

**六、论文提供的代码**
打开本博文最开始的连接，进入论文网站，下载C++代码。下载后，make编译程序。命令行运行格式：

```python
/********************************************
sigma  对原图像进行高斯滤波去噪
k      控制合并后的区域的数量
min:   后处理参数，分割后会有很多小区域，当区域像素点的个数小于min时，选择与其差异最小的区域合并
input  输入图像（PPM格式）
output 输出图像（PPM格式）
sigma: Used to smooth the input image before segmenting it.
k:     Value for the threshold function.
min:   Minimum component size enforced by post-processing.
input: Input image.
output:Output image.
Typical parameters are sigma = 0.5, k = 500, min = 20.
Larger values for k result in larger components in the result.
*/
./segment sigma k min input output
```
**七、OpenCV3.3 cv::ximgproc::segmentation::GraphSegmentation类**

```python
/opencv_contrib/modules/ximgproc/include/opencv2/ximgproc/segmentation.hpp
```





