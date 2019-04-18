# PCL的学习必要性、重要性、意义及最初——持续修改中 - wishchinYang的专栏 - CSDN博客
2013年07月25日 13:22:29[wishchin](https://me.csdn.net/wishchin)阅读数：2764
                
        为什么学习PCL：[图像描述：各种维度图像的逻辑描述形式  ^-^](http://blog.csdn.net/wishchin/article/details/49903115)
        且点云库是机器人学领域的必备基础库，ICRA和IROS的计算机视觉相关一般都使用了PCL库，PCL库也成为ROS的基础部分，与机器人操作系统有了很好的对接。
        自此，开始了点云库里被淹没的苦X生涯。不停地重建、分割、识别、解析语义，分割、抓取..........![大哭](http://static.blog.csdn.net/xheditor/xheditor_emot/default/wail.gif)
         什么是机器人，天晓得！我都没有见过一个完整功能的机器人！！！Fuck了的世界，对我真是太不公平了![大哭](http://static.blog.csdn.net/xheditor/xheditor_emot/default/wail.gif)
**前言：PCL 简介**
        官方主页：[http://pointclouds.org/](http://pointclouds.org/)
        PCL中国：[http://www.pclcn.org/](http://www.pclcn.org/) （对于英语语言运用不好的同学，可以参考此网站的资料，比较详细，英文版的就只是下载一些软件包）
[PCL入门文档 :http://blog.csdn.net/vbskj/article/details/7556269](http://blog.csdn.net/vbskj/article/details/7556269)
**官方介绍：**
## What is PCL?
         The **P**oint **C**loud **L**ibrary (or**PCL**) is a**large scale, open project[[1]](http://pointclouds.org/about/#open)** for 2D/3D image
 and point cloud processing. The PCL framework contains numerous state-of-the art algorithms including filtering, feature estimation, surface reconstruction, registration, model fitting and segmentation. These algorithms can be used, for example, to filter
 outliers from noisy data, stitch 3D point clouds together, segment relevant parts of a scene, extract keypoints and compute descriptors to recognize objects in the world based on their geometric appearance, and create surfaces from point clouds and visualize
 them -- to name a few.
         点云库是一个大规模，开放工程的2D,3D处理库。骨架包括......（更多信息请参考中文网站.....）
         对于PCL的详细介绍，可参见 ICRA2011的一篇论文：icra2011   3D is here： Point Cloud Library (PCL).此paper里面有详细的介绍及大致的使用规则.
         我在IEEE上检索到的一篇paper：Point Cloud Library：Three-DimensionalObject Recognition and 6 DoF Pose Estimation（[Robotics & Automation Magazine, IEEE](http://ieeexplore.ieee.org/xpl/RecentIssue.jsp?punumber=100)
Volume: 19,[Issue: 3](http://ieeexplore.ieee.org/xpl/tocresult.jsp?isnumber=6299141)） 有一个详细的使用规则。   
**（一）：从基础数据开始**
**转自官方网站：：**[http://www.pclcn.org/study/shownews.php?lang=cn&id=54](http://www.pclcn.org/study/shownews.php?lang=cn&id=54)
# PCD文件（点云数据）格式
       本节描述PCD（点云数据）文件格式。
### 为什么用一种新的文件格式？
         PCD文件格式并非白费力气地做重复工作，现有的文件结构因本身组成的原因不支持由PCL库引进n维点类型机制处理过程中的某些扩展，而PCD文件格式能够很好地补足这一点。PCD不是第一个支持3D点云数据的文件类型，尤其是计算机图形学和计算几何学领域，已经创建了很多格式来描述任意多边形和激光扫描仪获取的点云。包括下面几种格式：
      l   PLY是一种多边形文件格式，由Stanford大学的Turk等人设计开发；
      l STL是3D Systems公司创建的模型文件格式，主要应用于CAD、CAM领域；
      l  OBJ是从几何学上定义的文件格式，首先由Wavefront Technologies开发；
      l  X3D是符合ISO标准的基于XML的文件格式，表示3D计算机图形数据；
      l  其他许多种格式。
以上所有的文件格式都有缺点，在下一节会讲到。这是很自然的，因为它们是在不同时间为了不同的使用目的所创建的，那时今天的新的传感器技术和算法都还没有发明出来。
**我的理解** : [各种维度图像的逻辑描述形式——我为什么偏爱点云](http://blog.csdn.net/wishchin/article/details/49903115)
### PCD版本
        在点云库（PCL）1.0版本发布之前，PCD文件格式有不同的修订号。这些修订号用PCD_Vx来编号（例如，PCD_V5、PCD_V6、PCD_V7等等），代表PCD文件的0.x版本号。然而PCL中PCD文件格式的正式发布是0.7版本（PCD_V7）。
### 文件头格式
       每一个PCD文件包含一个文件头，它确定和声明文件中存储的点云数据的某种特性。PCD文件头必须用ASCII码来编码。PCD文件中指定的每一个文件头字段以及ascii点数据都用一个新行（\n）分开了，从0.7版本开始，PCD文件头包含下面的字段：
```python
·VERSION –指定PCD文件版本
·FIELDS    –指定一个点可以有的每一个维度和字段的名字。例如：
FIELDS x y z                                   # XYZ data
FIELDS x y z rgb                               # XYZ + colors
FIELDS x y z normal_xnormal_y normal_z         # XYZ + surface normals
FIELDS j1 j2 j3                                # moment invariants
```
...
·SIZE –用字节数指定每一个维度的大小。例如：
```python
unsigned char/char has 1 byte
unsigned short/short has 2 bytes
unsignedint/int/float has 4 bytes
double has 8 bytes
```
·TYPE –用一个字符指定每一个维度的类型。现在被接受的类型有：
**I** –表示有符号类型int8（char）、int16（short）和int32（int）；
**U** – 表示无符号类型uint8（unsigned char）、uint16（unsigned short）和uint32（unsigned int）；
**F** –表示浮点类型。
·COUNT –指定每一个维度包含的元素数目。例如，x这个数据通常有一个元素，但是像VFH这样的特征描述子就有308个。实际上这是在给每一点引入n维直方图描述符的方法，把它们当做单个的连续存储块。默认情况下，如果没有COUNT，所有维度的数目被设置成1。
·WIDTH –用点的数量表示点云数据集的宽度。根据是有序点云还是无序点云，WIDTH有两层解释：
1)它能确定无序数据集的点云中点的个数（和下面的POINTS一样）；
2)它能确定有序点云数据集的宽度（一行中点的数目）。
（注意：有序点云数据集，意味着点云是类似于图像（或者矩阵）的结构，数据分为行和列。这种点云的实例包括立体摄像机和时间飞行摄像机生成的数据。有序数据集的优势在于，预先了解相邻点（和像素点类似）的关系，邻域操作更加高效，这样就加速了计算并降低了PCL中某些算法的成本。
例如：
```python
WIDTH 640       # 每行有640个点
·HEIGHT –用点的数目表示点云数据集的高度。类似于WIDTH ，HEIGHT也有两层解释：
```
1)它表示有序点云数据集的高度（行的总数）；
2)对于无序数据集它被设置成1（被用来检查一个数据集是有序还是无序）。
有序点云例子：
```python
WIDTH 640       # 像图像一样的有序结构，有640行和480列，
HEIGHT 480      # 这样该数据集中共有640*480=307200个点）
```
无序点云例子：
```python
WIDTH 307200
HEIGHT 1        # 有307200个点的无序点云数据集
```
·VIEWPOINT–指定数据集中点云的获取视点。VIEWPOINT有可能在不同坐标系之间转换的时候应用，在辅助获取其他特征时也比较有用，例如曲面法线，在判断方向一致性时，需要知道视点的方位，
视点信息被指定为平移（txtytz）+四元数（qwqxqyqz）。默认值是：
```python
VIEWPOINT 0 0 0 1 0 0 0
·POINTS–指定点云中点的总数。从0.7版本开始，该字段就有点多余了，因此有可能在将来的版本中将它移除。
```
例子：
```python
POINTS 307200   #点云中点的总数为307200
·DATA –指定存储点云数据的数据类型。从0.7版本开始，支持两种数据类型：ascii和二进制。查看下一节可以获得更多细节。
```
注意：文件头最后一行（DATA）的下一个字节就被看成是点云的数据部分了，它会被解释为点云数据。
警告：PCD文件的文件头部分必须以上面的顺序精确指定，也就是如下顺序：
VERSION、FIELDS、SIZE、TYPE、COUNT、WIDTH、HEIGHT、VIEWPOINT、POINTS、DATA
之间用换行隔开。
### 数据存储类型
在0.7版本中，.PCD文件格式用两种模式存储数据：
        如果以ASCII形式，每一点占据一个新行：
```python
p_1
p_2
...
p_n
```
注意：从PCL 1.0.1版本开始，用字符串“nan”表示NaN，此字符表示该点的值不存在或非法等。
       如果以二进制形式，这里数据是数组（向量）pcl::PointCloud.points的一份完整拷贝，在Linux系统上，我们用mmap/munmap操作来尽可能快的读写数据，存储点云数据可以用简单的ascii形式，每点占据一行，用空格键或Tab键分开，没有其他任何字符。也可以用二进制存储格式，它既简单又快速，当然这依赖于用户应用。ascii格式允许用户打开点云文件，使用例如gunplot这样的标准软件工具更改点云文件数据，或者用sed、awk等工具来对它们进行操作。
### 相对其他文件格式的优势
       用PCD作为（另一种）文件格式可能被看成是没有必要的一项工作。但实际中，情况不是这样的，因为上面提到的文件格式无一能提高PCD文件的适用性和速度。PCD文件格式包括以下几个明显的优势：
        l    存储和处理有序点云数据集的能力——这一点对于实时应用，例如增强现实、机器人学等领域十分重要；
       l    二进制mmap/munmap数据类型是把数据下载和存储到磁盘上最快的方法；
        l    存储不同的数据类型（支持所有的基本类型：char，short，int，float，double）——使得点云数据在存储和处理过程中适应性强并且高效，其中无效的点的通常存储为NAN类型；
        l    特征描述子的n维直方图——对于3D识别和计算机视觉应用十分重要。
       另一个优势是通过控制文件格式，我们能够使其最大程度上适应PCL，这样能获得PCL应用程序的最好性能，而不用把一种不同的文件格式改变成PCL的内部格式，这样的话通过转换函数会引起额外的延时。
注意：尽管PCD（点云数据）是PCL中的内部文件格式，pcl_io库也提供在前面提到的所有其他文件格式中保存和加载数据。
### 例子
```cpp
下面贴出了PCD文件的一个片段。把它留给读者以解析这些数据，看看它的组成，玩的愉快！
# .PCD v.7 - Point Cloud Data file format
VERSION .7
FIELDS x y z rgb
SIZE 4 4 4 4
TYPE F FFF
COUNT 1 1 1 1
WIDTH 213
HEIGHT 1   //设置为1，标记为无序点云
VIEWPOINT 0 0 0 1 0 0 0
POINTS 213  //点云中点的总数为213
DATA ascii
0.93773 0.33763 0 4.2108e+06
0.90805 0.35641 0 4.2108e+06
```
# Example：格式内容.
A snippet of a PCD file is attached below. It is left to the reader tointerpret the data and see what it means. :)   Have fun!:
```cpp
# .PCD v.7 - Point Cloud Data file format
VERSION .7
FIELDS x y z rgb
SIZE 4 4 4 4
TYPE F F F F
COUNT 1 1 1 1
WIDTH 213
HEIGHT 1
VIEWPOINT 0 0 0 1 0 0 0
POINTS 213
DATA ascii
0.93773 0.33763 0 4.2108e+06
0.90805 0.35641 0 4.2108e+06
0.81915 0.32 0 4.2108e+06
0.97192 0.278 0 4.2108e+06
0.944 0.29474 0 4.2108e+06
0.98111 0.24247 0 4.2108e+06
0.93655 0.26143 0 4.2108e+06
0.91631 0.27442 0 4.2108e+06
0.81921 0.29315 0 4.2108e+06
0.90701 0.24109 0 4.2108e+06
0.83239 0.23398 0 4.2108e+06
0.99185 0.2116 0 4.2108e+06
0.89264 0.21174 0 4.2108e+06
0.85082 0.21212 0 4.2108e+06
0.81044 0.32222 0 4.2108e+06
0.74459 0.32192 0 4.2108e+06
0.69927 0.32278 0 4.2108e+06
0.8102 0.29315 0 4.2108e+06
0.75504 0.29765 0 4.2108e+06
0.8102 0.24399 0 4.2108e+06
0.74995 0.24723 0 4.2108e+06
0.68049 0.29768 0 4.2108e+06
0.66509 0.29002 0 4.2108e+06
0.69441 0.2526 0 4.2108e+06
0.62807 0.22187 0 4.2108e+06
0.58706 0.32199 0 4.2108e+06
0.52125 0.31955 0 4.2108e+06
0.49351 0.32282 0 4.2108e+06
0.44313 0.32169 0 4.2108e+06
0.58678 0.2929 0 4.2108e+06
0.53436 0.29164 0 4.2108e+06
0.59308 0.24134 0 4.2108e+06
0.5357 0.2444 0 4.2108e+06
0.50043 0.31235 0 4.2108e+06
0.44107 0.29711 0 4.2108e+06
0.50727 0.22193 0 4.2108e+06
0.43957 0.23976 0 4.2108e+06
0.8105 0.21112 0 4.2108e+06
0.73555 0.2114 0 4.2108e+06
-0.85618 0.29497 0 4.2108e+06
-0.79975 0.24326 0 4.2108e+06
-0.8521 0.24246 0 4.2108e+06
-0.91157 0.31224 0 4.2108e+06
-0.95031 0.29572 0 4.2108e+06
-0.92223 0.2213 0 4.2108e+06
-0.94979 0.24354 0 4.2108e+06
-0.78641 0.21505 0 4.2108e+06
-0.87094 0.21237 0 4.2108e+06
-0.90637 0.20934 0 4.2108e+06
-0.93777 0.21481 0 4.2108e+06
0.22244 -0.0296 0 4.808e+06
0.2704 -0.078167 0 4.808e+06
```
 敬请关注PCL（Point Cloud Learning）中国更多的点云库PCL（Point Cloud Library）相关官方教程。
**（二）PCL库的模块**
简单介绍：[http://qing.blog.sina.com.cn/1455173150/56bc2e1e330005pk.html](http://qing.blog.sina.com.cn/1455173150/56bc2e1e330005pk.html)
        PCL is released under the terms of the [3-clause BSD license](http://en.wikipedia.org/wiki/BSD_licenses#3-clause_license_.28.22New_BSD_License.22_or_.22Modified_BSD_License.22.29) and is open source software. **It is free for commercial and research use.**
       PCL is **cross-platform**, and has been successfully compiled and deployed on Linux, MacOS, Windows, and[Android/iOS](http://www.pointclouds.org/news/2012/05/29/pcl-goes-mobile-with-ves-and-kiwi/).
 To simplify development, PCL is split into a series of smaller code libraries, that can be compiled separately. This modularity is important for distributing PCL on platforms with reduced computational or size constraints (for more information about each module
 see the [documentation](http://www.pointclouds.org/documentation/) page). Another way to think about PCL is as a graph of code libraries, similar to the[Boost](http://www.boost.org) set of C++ libraries. Here's an example:![](http://www.pointclouds.org/assets/images/about/pcl_dependency_graph2.png)
**pcl的子模块们**
pcl_filters                     去除噪点
pcl_features                三维特征提取
pcl_registration           将点云融合到一个全局模型中
pcl_kdtree                   基于FLANN的kdtree实现最近邻搜索
pcl_octree                   基于八叉树实现最近邻搜索
pcl_segmentation       分割点云
pcl_sample_consensus  对点云进行拟合（线、面、园、球、柱面、平行线），拟合算法包括：
    * SAC_RANSAC - RANdom SAmple Consensus
    * SAC_LMEDS - Least Median of Squares
    * SAC_MSAC - M-Estimator SAmple Consensus
    * SAC_RRANSAC - Randomized RANSAC
    * SAC_RMSAC - Randomized MSAC
    * SAC_MLESAC - Maximum LikeLihood Estimation SAmple Consensus
    * SAC_PROSAC - PROgressive SAmple Consensus
pcl_surface                             从三维扫描重建表面，可以是mesh/convex hull/concave hull
pcl_range_image                    深度图，由kinect获取后可转换为点云
pcl_io                                       输入输出，PCD (Point Cloud Data) 文件的读写，OpenNI的接口（没有实现微软SDK的接口）
pcl_visualization                      类似于OpenCV中highgui的作用，基于VTK实现三维可视化（VTK是一个极其庞大的库）
因此有了库：
必需的库有：
       boost        C++的高级版本库
       Eigen       矩阵计算库，主要用于位姿计算
       FLANN     近似最近邻查询，用于匹配和模式识别
可选项有：
        VTK，用于pcl_visualization      PCL的显示库，可是实时显示点云
        QHull，用于pcl_surface           多边形计算库，可以计算PLY格式点云
        OpenNI，用于pcl_io                  I/O关系，得到PLY格式点云，可以自行转化为PCD格式
        CUDA                                        加速计算库
后记：使用PCL库时，要选取一块好的NVIDIA显卡.....................
**转载了...**
