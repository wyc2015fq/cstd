# 深度学习AI美颜系列---AI美颜磨皮算法二

2018年11月28日 13:10:41

 

Trent1985

 

阅读数：762

更多

所属专栏： [SF图像滤镜/美颜/美妆算法详解与实战](https://blog.csdn.net/column/details/25028.html)



版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/Trent1985/article/details/84582522

这里先放两张对比结果图，原图来自网络：

![img](https://img-blog.csdnimg.cn/20181128130611611.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==,size_16,color_FFFFFF,t_70)

![img](https://img-blog.csdnimg.cn/20181128130629883.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==,size_16,color_FFFFFF,t_70)

算法的流程如下：

1，皮肤分割算法；

目前主流的语义分割等都可以实现，准确度要求较高。本人使用U-net和PSPNet。

2，人脸关键点识别算法；

这一步可以使用商汤/Face++/虹软人脸SDK，这里使用的是虹软101点人脸SDK。

也可以自己训练人脸SDK，推荐使用mtcnn或者opencv人脸检测+cnn关键点网络；

3，基于皮肤区域的祛斑算法；

这里祛斑算法如下：

①使用Sobel粗略提取斑点信息；

②使用连通域判断得到最终斑点区域；

③结合高斯滤波+泊松融合来实现祛斑效果；

4，基于皮肤区域的磨皮美白调色算法；

磨皮部分：通过高反磨皮或双曲线磨皮算法对人像磨皮，结合皮肤区域得到精确磨皮效果；

美白调色部分：这一步通过PS中的曲线调节来得到美白调色的LUT，结合肤色区域进行Blend得到调色结果；

5，大眼瘦脸算法；

基于人脸特征点+MLS变形或IDW反距离加权变形来实现大眼瘦脸算法；

6，五官立体+腮红+唇彩+美瞳算法；

这一步通过人脸关键点来得到嘴唇和腮红区域，实现唇彩和腮红；

通过美瞳模板来实现美瞳算法；

通过双曲线调色来实现五官立体算法；

关键点：

1，精准的皮肤识别分割；

2，精准的人脸关键点；

本算法使用opengl可在手机端实时处理；

 