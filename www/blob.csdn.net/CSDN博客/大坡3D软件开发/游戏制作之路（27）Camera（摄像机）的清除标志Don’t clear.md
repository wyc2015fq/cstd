
# 游戏制作之路（27）Camera（摄像机）的清除标志Don’t clear - 大坡3D软件开发 - CSDN博客

2018年10月21日 14:59:16[caimouse](https://me.csdn.net/caimouse)阅读数：4021


前面学习了按摄像头的深度来进行清除，到这里已经学习了三个选项，摄像机的清除就只剩下一个最简单的选项：Don’t clear。这个选项应该是最简单的，为什么这样说呢，因为它什么也不干，保留原来的画面，原汁原味，原封不动。你也许感觉到奇怪，什么情况之下还会有不清除的呢？其实在普通的游戏里是不出现这个要求的，因为普通的游戏都是一帧帧地绘出来的，如果不清除，再绘新的内容上去，就相当于在一个没有擦除的黑板上再写字，这是看不清楚的。因此，只有在特效的场合才会使用，比如shader编写。我们来假定有这样一个需求，把游戏的背景进行模糊处理，那么这帧画面就要清除了，只要在这帧画面进行加工处理即可。到这里应该明白这个选项的使用了，为了加深理解，下面来学习一个简单的例子，把摄像机按如下设置：
![](https://img-blog.csdn.net/20181021145306764?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这里设置为don't clear的选项，看到这个圆球是圆形的，当运行这个游戏时，就会让立方体移动过去，它会把这个球的下半部切掉，并且不再恢复为圆形。如下图：
![](https://img-blog.csdn.net/20181021145639340?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
从上图可以看到球真的被这个“天狗食月“了。
到这里，就把摄像机的四个清除选项学习完成了。下一次，继续来学习摄像机的其它内容。
TensorFlow API攻略
[http://edu.csdn.net/course/detail/4495](http://edu.csdn.net/course/detail/4495)
TensorFlow入门基本教程
[http://edu.csdn.net/course/detail/4369](http://edu.csdn.net/course/detail/4369)
C++标准模板库从入门到精通
[http://edu.csdn.net/course/detail/3324](http://edu.csdn.net/course/detail/3324)


