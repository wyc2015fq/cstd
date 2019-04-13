
# 游戏制作之路（26）Camera（摄像机）的清除标志Depth only - 大坡3D软件开发 - CSDN博客

2018年10月17日 17:00:22[caimouse](https://me.csdn.net/caimouse)阅读数：294


在前面学习了天空盒和纯背景颜色的清除方法，这两个方法都比较常用的，一般情况下使用这两个选项，就基本满足了。但是你或许有点好奇心，像一个小学生一样，不断地为什么，比如为什么太阳会发亮，为什么月亮只有晚上才看到，为什么老师要让我背书等等问题，多问为什么，的确是一个好学生的样子。因而，继续来追问下一个清除选项depth only，这个选项从字面上来说，或者从风水师的角度来解释，就是与深度相关的，但它有一个定语only，only就是唯一，经常听英语歌会听到only you这样的歌词。深度与唯一联在一起，就是仅仅清除深度信息了。
因此，depth only就是表示当一帧新画面显示时，它会根据摄像机的深度信息来显示新画面与旧画面。理解这句话，或许不太明确，比如说有两个摄像机，一个摄像机照着大海，一个摄像机照着美女，大海的摄像机的深度是-1， 美女的摄像机的深度是0，根据深度来显示，就是先显示-1的内容，再显示0的内容。这样无论-1里的内容怎么变化，都是当作一个背景，0摄像机就可以设置为深度更新，这样就可以把美女与大海融合到一起了。
为了理解这个深度清除的作用，我们来创建一个例子，当我们使用照相机来拍摄一些花草时，可以让花的背景为模糊不清，花是清楚的，如下图：
![](https://img-blog.csdn.net/20181017164202289?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
像这样的画面，照相机很容易就做到了。但是在游戏里，怎么样做到呢？
其实可以采用双摄像机的方式来实现，并且其中一个摄像机的清除标志为depth only。比如设置主摄像机如下：
![](https://img-blog.csdn.net/20181017164539524?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
你仔细地看到上面场景里的摄像机显示的，只有一个球和地板，没有看到立方体，因为立方体是采用另外一个摄像机Camera1来显示的，两个摄像机组合之下，就可以把里面的东西全部显示了。再为看一下摄像机Camera1的设置：
![](https://img-blog.csdn.net/20181017164927738?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
可以看到摄像机Camera1的设置clear flags为depth only，它的clulling mask为cube层，这个cube层是可以设置游戏对象在那一层里面，cube是层的名称。并且看到depth是0，而主摄像机为-1，所以0摄像机的内容永远显示在前面，-1摄像机的内容永远显示在后面。
从上图也可以看到，当没有执行模糊脚本时，这两个摄像机的内容都是清晰的，接着下来，运行一下游戏，应用模糊的功能到主摄像机，就可以把背景进行模糊处理了，最终的效果如下：
![](https://img-blog.csdn.net/20181017165507792?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
从上图可以看到，背景的地板和圆球都已经模糊不清，但前面的立方体还是非常清楚的，达到虚化背景的效果。因而可以使用多个摄像机来实现不同的特技，也可以显示近景的物体，比如官方解释枪在靠近物体时会导致显示不完整的情况：
如果你想绘制一个玩家的枪，又不让它内部环境被裁剪，你会设置深度为0的相机绘制环境，和另一个深度为1的相机单独绘制武器。武器相机的清除标志（Clear Flags ）应设置 为depth only。这将保持环境的图形显示在屏幕上，但会丢弃所有关于每个对象在三维空间中的位置的信息。当枪被绘制出来，不透明的部分将完全覆盖任何已绘制的事物，而不管枪到墙之间如何接近。
通过本文，你应该学会了不同摄像机的深度作用和概念。
Unity5.x游戏基础入门
[http://edu.csdn.net/course/detail/4810](http://edu.csdn.net/course/detail/4810)
TensorFlow API攻略
[http://edu.csdn.net/course/detail/4495](http://edu.csdn.net/course/detail/4495)
TensorFlow入门基本教程
[http://edu.csdn.net/course/detail/4369](http://edu.csdn.net/course/detail/4369)


