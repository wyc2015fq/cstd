# 梳理 Opengl ES 3.0 （五）shader运行原理 - DesignYourDream - 博客园
# [梳理 Opengl ES 3.0 （五）shader运行原理](https://www.cnblogs.com/designyourdream/p/6773228.html)
先来看看一张图
![](https://images2015.cnblogs.com/blog/698097/201704/698097-20170427105737100-196985822.png)
shader都是在运行时编译和执行的，每个shader都有一个main函数作为它的入口。
vertex shader的功能有两个：一个是计算顶点坐标变换，另一个就是为片元shader计算逐顶点信息。
fragment shader在每个像素片上会执行，如上图的4和5，它的功能是计算片元即fragment的颜色值，一般来说，这类shader会计算颜色插值，纹理坐标计算，纹理提取，法线插值，光照，雾效等等。上图里的6是光栅化处理后的图像，framebuffer即帧缓存里的一个pixel是由一个或者任意多个fragment的信息组合而成，至于如何组合，就可以用程序来决定了，每个fragment都是可以用程序控制的对象。
现代的多核GPU针对大数据量的多边形转换计算以及浮点型运算进行了优化，能够并行处理大量数据。处理过程如下所示：
![](https://images2015.cnblogs.com/blog/698097/201704/698097-20170427111110990-662839508.png)

