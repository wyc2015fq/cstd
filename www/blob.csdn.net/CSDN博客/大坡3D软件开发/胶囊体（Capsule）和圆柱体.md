
# 胶囊体（Capsule）和圆柱体 - 大坡3D软件开发 - CSDN博客

2019年02月20日 10:32:29[caimouse](https://me.csdn.net/caimouse)阅读数：151


胶囊体（Capsule）
![](https://img-blog.csdnimg.cn/20190220103057328.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
胶囊体是由一个单位长度的圆柱和两个半单位的半球组成。纹理是从两个极点边缘线开始贴图。不过在实际环境里使用这个外形的物体不多见，经常使用它来做碰撞测试，比如一个游戏里人物就可以使用它来代替测试地图里碰撞是否合理，是否会卡住人物出不来。它比使用立方体来测试要好很多，并且更加接近人体的结构。
圆柱体
![](https://img-blog.csdnimg.cn/20190220103120295.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
这是一个简单的圆柱体，它的直径是一个单位，高是两个单位。它的纹理包括柱表面，也包含上底和下底的两个表面。圆柱体很适合用来创建柱子、杆和轮子，不过你要注意的是它没有圆柱体的碰撞体，而是使用胶囊体的碰撞体。如果实在需要圆柱体的碰撞体，就要自己来做了，创建一个合适的圆柱网格，导入物理引擎里。
[https://blog.csdn.net/caimouse/article/details/51749579](https://blog.csdn.net/caimouse/article/details/51749579)

