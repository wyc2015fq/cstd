# monoSLAM - KinboSong的博客 - CSDN博客
2017年03月20日 16:51:51[songjinbo3](https://me.csdn.net/KinboSong)阅读数：956
**单目相机的优点（和双目相机相比）：双目相机测得的深度距离收到基线长度的限制，单目相机就可以解决这个问题。**
参考博客：[http://blog.csdn.net/heyijia0327/article/details/50758944](http://blog.csdn.net/heyijia0327/article/details/50758944)  （一定要看，解决了所有疑惑,包括尺度问题，5点法，8点法的由来）
视觉里程计、SFM、运动估计
本质矩阵：essential matrix
基础矩阵：Fundamental matrix
参考文献：[http://www.cnblogs.com/gemstone/archive/2011/12/20/2294805.html](http://www.cnblogs.com/gemstone/archive/2011/12/20/2294805.html)
[http://blog.csdn.net/xiao4399/article/details/48037287](http://blog.csdn.net/xiao4399/article/details/48037287)
[http://sparklinglee.blog.163.com/blog/static/23009012920145183315488/](http://sparklinglee.blog.163.com/blog/static/23009012920145183315488/)
[https://wenku.baidu.com/view/5b963c00ba1aa8114431d94a.html](https://wenku.baidu.com/view/5b963c00ba1aa8114431d94a.html)
[https://en.wikipedia.org/wiki/Essential_matrix](https://en.wikipedia.org/wiki/Essential_matrix)
**注意**：最后恢复的旋转矩阵是可以确定的，但是恢复的平移矩阵则不一定是正确的，但是和真正的平移矩阵相差一个scale factor
同理，恢复的三维点也相差一个scale factor。下面是摘自其他作者的原话。
（1）The scale issue is normal in Mono camera, because for mono camera (or said sfm), the reconstruction matrix can only be back to scale level.
（2）Visual odometry (also called structure from motion) is the problem of recovering the motion of a camera from the visual input alone.
This can be done by using single cameras(perspective or omnidirectional), stereo cameras,or multi-camera systems.
The advantage of using more than one camera is that both the motion and the 3D structure can be computed directly in the absolute scale when the distance between the cameras is known. 
—— 《Absolute Scale in Structure from Motion from a Single Vehicle Mounted Camera by Exploiting Nonholonomic Constraints》
（3）It is well known that the shape and motion geometry in SfM problems such as this are subject to arbitrary scaling and that this scale factor cannot be recovered. 
(The imaging geometry $\beta$ and the rotation are recoverable and not subject to this scaling). 
—— http://www1.cs.columbia.edu/~jebara/htmlpapers/SFM/node26.html
**1、相机坐标系、图像坐标系、像素坐标系**
![](https://img-blog.csdn.net/20170320101628044?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvS2luYm9Tb25n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
相机坐标系：OsXsYsZs
图像坐标系：O1x1y1
像素坐标系：与图像坐标系的方向相同；不同的是：
x方向的单位尺度为像素点的宽度，y方向的单位尺度为像素点的宽度。
**2、相机内参矩阵和外参矩阵**
内参矩阵：
外参矩阵：包括旋转矩阵和平移矩阵
**3、摄像机矩阵**
![](https://img-blog.csdn.net/20170320105204687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvS2luYm9Tb25n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**4、本质矩阵**
![](https://img-blog.csdn.net/20170320095759987?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvS2luYm9Tb25n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
注：这里的Pr、Pl代表在两个**相机坐标系**下的**三维坐标**。
下面过程将外极线方程中的三维坐标化为成像平面的齐次坐标：
![](https://img-blog.csdn.net/20170320100926846?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvS2luYm9Tb25n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
注意：pr、pl代表三维点p在左右两个**图像坐标系**下的**齐次坐标。**S代表平移向量T的斜对称矩阵（skew symmetric matrix）
**5、基础矩阵**
本质矩阵指明了在**图像坐标系**下左右成像平面对应点的关系。
但是我们在使用时使用的是像素坐标系（比如在特征检测和匹配都是用像素坐标系），基础矩阵则指明了在**像素坐标系**下左右成像平面对应点的关系。
![](https://img-blog.csdn.net/20170320104224080?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvS2luYm9Tb25n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**6、本质矩阵和基础矩阵的关系**
![](https://img-blog.csdn.net/20170320121901195?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvS2luYm9Tb25n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**7、本质矩阵的性质：**
（1）本质矩阵仅仅由旋转矩阵和平移矩阵确定，与相机的参数无关。
（2）外极线方程：
![](https://img-blog.csdn.net/20170320102739013?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvS2luYm9Tb25n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
（3）极线与本质矩阵和像素点的对应关系：
![](https://img-blog.csdn.net/20170320102829389?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvS2luYm9Tb25n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
（4）极点与本质矩阵的关系：
![](https://img-blog.csdn.net/20170320103001969?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvS2luYm9Tb25n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
（5）本质矩阵的秩和行列式
rank（E） = 2
det（E） = 0
**8、基础矩阵性质：**
同本质矩阵一样。
**9、求解基础矩阵、本质矩阵**
本质矩阵5个自由度，可已从两个层面理解：
（1）旋转矩阵3个、平移矩阵2个（因为平移矩阵乘以一个系数不影响外极线方程的成立）
（2）体现在数学公式上可以增加如下4个约束，将E的自由度控制在5个：
（1）det（E）=0
（2）||E|| = 1（即E乘以一个系数，外极线方程依然成立，可以通过||E||=1限制系数）
（3）E的一个奇异值为0
（4）E的另外两个奇异值相等
基础矩阵的自由度7个：
（1）det（F） = 0
（2）||F|| = 1（即F乘以一个系数，外极线方程依然成立，可以通过||F||=1限制系数）
八点法：
用八点法求解本质矩阵E的过程可以参考：[https://en.wikipedia.org/wiki/Eight-point_algorithm](https://en.wikipedia.org/wiki/Eight-point_algorithm) 
 （the basic algorithm 部分）
注意：为什么很多文献说最少用8个点？
这里只考虑E和F的第二个约束，即E和F乘以一个系数，外极线方程依然成立；在求出来之后要做后续处理（近似），使得E和F满足其他的约束条件。
**10、本质矩阵求旋转矩阵R和平移矩阵t**
参考：《从本质矩阵恢复运动的方法简单证明》——郁钱
对本质矩阵E做SVD分解，再根据下面的定理即可得到结果。
**![](https://img-blog.csdn.net/20170421210230402?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvS2luYm9Tb25n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**
这里S表示平移向量T的斜对成矩阵（skew symmetric matrix）。
![](https://img-blog.csdn.net/20170421211105124?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvS2luYm9Tb25n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
综合定理1.2和推论1.3，可以得出：由本质矩阵即可恢复出2个旋转矩阵和带有一个尺度因子的平移矩阵（如果加上限制条件||T||=1，那么平移矩阵会产生两个）。
**如何从四个R，T的组合中恢找出真正的那个R，T组合？**
把所有特征点的深度计算出来，看深度值是不是都大于0，深度都大于0的那组R,T就是正确的。
**11、求解场景点的三维坐标**
**参考：[http://blog.csdn.net/heyijia0327/article/details/50774104](http://blog.csdn.net/heyijia0327/article/details/50774104)**
**参考：[http://sparklinglee.blog.163.com/blog/static/23009012920145183315488/](http://sparklinglee.blog.163.com/blog/static/23009012920145183315488/)**
注意：如果求出旋转矩阵R和平移矩阵t，并且知道两个图像坐标系中对应点的坐标（x1，y1）和（x2，y2）;那么可以根据下面的公式求出对应像点的三维点的坐标。
![](https://img-blog.csdn.net/20170320122829958?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvS2luYm9Tb25n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**12、确定scale的方法**
The absolute scale can then be determined from direct measurements(e.g., measuring the size of an element in the scene),motion constraints,or from the integration with other sensors.
