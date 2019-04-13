
# 平面（Plane)和四方形（Quad） - 大坡3D软件开发 - CSDN博客

2019年02月20日 11:26:14[caimouse](https://me.csdn.net/caimouse)阅读数：147标签：[unity](https://so.csdn.net/so/search/s.do?q=unity&t=blog)个人分类：[6.游戏引擎开发](https://blog.csdn.net/caimouse/article/category/7094872)



平面（Plane)
![](https://img-blog.csdnimg.cn/20190220112456406.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
这是一个10个单位长度的正方形平面，坐标是XZ轴平面里，可以使用整个四方形的纹理来贴图。平面常用来表示地面和墙，也可以在GUI用来显示一些图片或者视频，或者一些特别的效果，不过使用quad四方形来做这些事情更加方便、合理。
四方形（Quad）
![](https://img-blog.csdnimg.cn/20190220112527206.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
四方形跟平面有点像，但它四个边组成，只有两个三角形，平面是两百个三角形组成，因此从效率上来说，四方形比平面更好。比如要在游戏里显示图片或者视频，这个非常合适。还有在GUI里显示一些图片，以及在粒子系统的粒子显示，精灵和伪图片等等。
[https://blog.csdn.net/caimouse/article/details/51749579](https://blog.csdn.net/caimouse/article/details/51749579)

