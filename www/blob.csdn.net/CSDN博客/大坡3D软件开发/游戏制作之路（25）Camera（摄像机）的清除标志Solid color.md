
# 游戏制作之路（25）Camera（摄像机）的清除标志Solid color - 大坡3D软件开发 - CSDN博客

2018年10月16日 14:45:15[caimouse](https://me.csdn.net/caimouse)阅读数：221


前面学习了清除标志skybox的使用，接着下来，来学习一下Solid color的使用。当一个游戏画面没有背景图时，可以使用一个纯颜色的背景来显示。比如想开发一个演示动画的时候，在我们上大学的时候，很多老师都想把课堂做得最好，所以常常使用电脑作为辅助手段。特别在上物理课时，很多实验现象是需要动画来演示的，这时可能背景往往是空白，或者某一种颜色，会比较好一些，因为这样可以形成巨大的反差。这时，就可以使用清除标志Solid color，**这个标志的作用是把一帧画面先清空，再使用设定的颜色来显示背景，然后再在上面从远到近来进行投影游戏对象。**
比如下面的例子，就是使用清除标志Solid color，来显示一个粒子海洋的效果：
![](https://img-blog.csdn.net/20181016144225723?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这个例子里，右边就选择黑色的夜空，以便把粒子海洋显示出来，有一个强烈的对比效果。
俄罗斯方块游戏开发
[http://edu.csdn.net/course/detail/5110](http://edu.csdn.net/course/detail/5110)
boost库入门基础
[http://edu.csdn.net/course/detail/5029](http://edu.csdn.net/course/detail/5029)
Arduino入门基础
[http://edu.csdn.net/course/detail/4931](http://edu.csdn.net/course/detail/4931)


