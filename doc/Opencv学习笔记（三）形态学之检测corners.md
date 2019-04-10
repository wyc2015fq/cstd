# Opencv学习笔记（三）形态学之检测corners

2012年03月14日 21:39:23

crzy_sparrow

阅读数：3945

 								标签： 																[c																](https://so.csdn.net/so/search/s.do?q=c&t=blog) 							更多

 								个人分类： 																[OpenCV																](https://blog.csdn.net/crzy_sparrow/article/category/1091158) 							

 								所属专栏： 																[Opencv学习笔记](https://blog.csdn.net/column/details/opencv.html) 																 							

 									



​       没什么自己的东西，大致翻译了opencv cook第 五章Detecting edges and corners using  morphological filters部分的一些段落，并且做一个简略的概括。

​       首先创建四个5x5的不同形状的kernel；

​         1）kernel a（十字）：

​                             0,0,1,0,0,

​                             0,0,1,0,0,

​                             1,1,1,1,1,

​                             0,0,1,0,0,

​                             0,0,1,0,0

​       2）kernel b（菱形）；



​                             0,0,1,0,0,

​                             0,1,1,1,0,

​                             1,1,1,1,1,

​                             0,1,1,1,0,

​                             0,0,1,0,0



​       3）kernel c（X形）；



​                             1,0,0,0,1,

​                             0,1,0,1,0,

​                             0,0,1,0,0,

​                             0,1,0,1,0,

​                             1,0,0,0,1

​      

​       4）kernel d（方形）；



​                             1,1,1,1,1,

​                             1,1,1,1,1,

​                             1,1,1,1,1,

​                             1,1,1,1,1,

​                             1,1,1,1,1,

​    便于理解用下图式例：

  ![img](http://hi.csdn.net/attachment/201203/14/0_1331732651lVsV.gif)

​    步骤为：1）对源图用kernel a进行膨胀，

![img](http://hi.csdn.net/attachment/201203/14/0_1331732692P1WW.gif)









​                   2

）对1）步后的图用kernel b进行腐蚀得到result1：



![img](http://hi.csdn.net/attachment/201203/14/0_1331732686AJS1.gif)

​                3）对源图用kernel c进行膨胀

​                4）对3）步后的图用kernel d进行腐蚀得到result2

​                5）abs(result1-result2)就是corners了

​    其中，3）、4）步原理同1）、2）步骤，只是其的一个旋转版本，用于检测45度的corners

​    下图是实际检测corner的结果图：

![img](http://hi.csdn.net/attachment/201203/14/0_1331732699z9vz.gif)



@相关论文：The article A modified regulated morphological corner detector by F.Y. Shih, C.-F. Chuang, V. 
 Gaddipati, Pattern Recognition Letters, volume 26, issue 7, May 2005, for more information 
 on morphological corner detection. 