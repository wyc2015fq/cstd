# 图像处理--传统的手工特征之Haar特征 - wydbyxr的博客 - CSDN博客
2018年10月30日 10:19:53[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：158
# Haar特征
  Haar特征反映了局部区域之间的相对明暗关系，能够为人脸和非人脸的区分提供有效的信息，例如眼睛区域比周围的皮肤区域要暗，通过Haar特征就可以将这一特点表示出来。
## 特征计算
  1）先在窗口的某个位置取一个矩形的小块，
  2）然后将这个矩形小块划分为黑色和白色两部分，并分别对两部分所覆盖的像素点（图像上的每个点称为一个像素）的灰度值求和，
  3）最后用白色部分像素点灰度值的和减去黑色部分像素点灰度值的和，得到一个Haar特征的值（即一个数）
**关键点是矩形的形状和划分黑白块的方法。**
## 分矩形的方式
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181030101925350.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181030101933844.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
- 
环形的黑白区域划分模式，而不仅仅是上下或者左右型；
- 
旋转的Haar的特征，即将原来提取Haar特征的局部小块顺时针或逆时针旋转45度；
- 
分离的Haar特征，即允许一个Haar特征由多个互相分离的黑白区域来计算，而不要求黑白区域必须处于一个矩形小块中；
- 
对角型Haar特征；
- 
组合Haar特征，即对多个不同的Haar特征进行组合和二值编码；
- 
局部组合二值特征，即在局部对特定的Haar特征按照一定的结构进行组合和二值编码；
- 
带权多通道Haar特征，即一个Haar特征不再只包含黑白两种块，而允许有多种不同形状和不同颜色的块，其中不同的颜色对应着不同的权值，表示像素点上求和之后所占的比重——原来只有1和-1两种，多通道指的是在像素点上求和不仅仅是在灰度这一个通道上计算，而是同时在其它通道上计算（如RGB三个颜色通道；事实上，基于原图计算而来和原图同样大小的任何一张图都可以是图像的一个通道）。
