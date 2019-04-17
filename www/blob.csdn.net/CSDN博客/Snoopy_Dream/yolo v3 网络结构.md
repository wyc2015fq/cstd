# yolo v3 网络结构 - Snoopy_Dream - CSDN博客





2018年11月27日 15:56:28[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：520
所属专栏：[pytorch yolov3](https://blog.csdn.net/column/details/30519.html)









首先先看一下13*13如何与26*26相concat的。整个过程相当于第二张图里的右侧紫色部分【第一张图】。

其次看一下整个大规模的模型图【第二张图】

更详细的图【第三章图】

![](https://img-blog.csdnimg.cn/20181127153224436.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

9个clusters在分配时深层的feature map应该得到最大的3个clusters，浅层的feature map应该得到最小的3个clusters。这是因为深层的feature map是用来检测大目标的，而浅层的feature map是用来检测小目标的。  

1 2 8 8 4

![](https://img-blog.csdnimg.cn/20181127153757638.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181127153810725.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181127155610830.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181127155754336.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181127155816711.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/201811271558585.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)



