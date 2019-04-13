
# Unity基本物体和临时对象 - 大坡3D软件开发 - CSDN博客

2019年02月20日 10:00:10[caimouse](https://me.csdn.net/caimouse)阅读数：100


我们知道unity可以使用任何建模软件构造的3D模型，但是有一些简单的模型，或者临时测试对象，就不必要使用建模软件来构造了，因为那样效率非常低下。比如你正在测试一个算法，只需要一个物体就可以代表一辆汽车在运动了。因此，unity提供了一些基本的模型创建，命名为Cube（立方体），Sphere（球体）， Capsule（胶囊体）， Cylinder（圆柱体）， Plane（平面）和 Quad（四方形）。这些基本对象看起来没有什么用处，但是使用到正确的位置还是大有用途的，比如平面可以用来表示一个大的平坦的地面，并且这些对象也可以用来做游戏里临时物品，比如开发一个赛车游戏，由于美工还没有把车的模型做出来之前，但是又想测试游戏的算法，就可以使用一个立方体来做一个临时车的对象，或者想验证一个游戏的想法，也可以使用这些模型做测试。这些基本模型都可以通过菜单GameObject>3D Object里添加。
立方体（Cube）
这是一个非常简单的立方体，每条边上都是一个单位的长度，六个表面都可以使用相同的纹理图片。简单的立方体在游戏里不是经常使用的对象，但是把它进行变换大小之后，就可以构造成墙壁、柱子、盒子、台阶和其它相似的物品。或者开发人员使用它来测试游戏的代码，特别美工的模型还构造出来之前，都可以使用它临时替换，等美工完成之后，就可以换回真实的模型。由于这个立方体的边长是标准的一个单元的长度，可以使用它来比对导入的网格或模型是否符合unity的场景尺寸。
![](https://img-blog.csdnimg.cn/20190220095835777.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
球体（Sphere)
默认创建的是单位长度直径的球体（半径是0.5），纹理可以使用两个极点球形映射的纹理。这个球体可以用来表示足球、星球和炮弹。不过，半透明的球体，也可以用来制作一个漂亮的GUI界面部件，比如像生命值的表示。
![](https://img-blog.csdnimg.cn/20190220095924355.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
[https://blog.csdn.net/caimouse/article/details/51749579](https://blog.csdn.net/caimouse/article/details/51749579)

