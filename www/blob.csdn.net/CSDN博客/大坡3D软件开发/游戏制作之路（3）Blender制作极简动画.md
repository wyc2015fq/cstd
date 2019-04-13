
# 游戏制作之路（3）Blender制作极简动画 - 大坡3D软件开发 - CSDN博客

2018年08月28日 12:02:00[caimouse](https://me.csdn.net/caimouse)阅读数：2098


在前面已经把Blender安装好，并且修改为中文显示了，接着下来，先不要做全面的了解，先来学习做一个简单的动画，来体验一下工具的威力，也体验自己辛苦安装之后的成果体现。你不用担心不会做动画，这里制作的都是极简单的动画，就像编程学习一样，都从hello world开始，学习制作动画也作这个开始。其实只要点几下按钮，就可创建一个动画出来，非常方便。
在创建之前先记住下面几个快捷键：
打开blender可以通过“shift+a”调出创建菜单。
通过“rgs”这三个按键，可以分别对模型进行旋转移动缩放。
“shift+d”可以实现复制功能。
使用“z”键可以切换到线框模式，再按一次切换回来。
“tab”按键可以切换到“编辑模式”，以及回到当前模式。
在“编辑模式”可以通过“ctrl+tab”切换到“点线面”选择模式。
选择模型部分后可以使用“e”键挤出。
使用“x”键可以调出删除菜单，选择要删除的数据。
当你打开blender就会显示如下缺省界面：
![](https://img-blog.csdn.net/20180828111531769?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
通过这个界面介绍，就基本了解这个软件的基本功能分布了，现在就来进入创建动画的过程。
第一步，先来到底部的时间轴这个区域，设置第一帧的动画是什么样的如下：
![](https://img-blog.csdn.net/20180828112028321?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
找到设置帧的位置了，接着下来，就是要把物体改变位置，改变大小，或者改变转动方向，做了这三个动作之后，就要插入关键帧了，那么怎么样来插入关键帧呢？这是通过快捷键的方式：按下i键，就会弹出如下菜单：
![](https://img-blog.csdn.net/20180828112456153?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
当你选择菜单里的一个选项之后，就可以插入关键帧了，这样就定义了动画的起始状态，相当于小孩子出生长得怎么样，后面动画，就看他长得怎么样了。blender这时就会记住场景里所有信息，接着下来所有变化，都会记录到动画的变化里。接着下来，我们来把这个四方体进行变大变小地变化，看看动画是否也跟着改变。因此，接着下来就创建第二个关键帧，这样软件就可以计算从第一个状态到第二个状态，它需要怎么样进行插值。第二个关键如下操作：
![](https://img-blog.csdn.net/20180828113420474?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
第二个操作是一样的，只是在中间添加了一个缩放物体的动作，这个缩放就是实现动画的功能：变大变小。当然，你也可以进行其它操作，比如位移，旋转，或者变成其它物体。接着下来创建第三个关键帧，同样把时间轴设置为100帧位置，可以在50的位置进行手动输入，也可以使用鼠标进行拖动绿色的滑块：
![](https://img-blog.csdn.net/20180828114034742?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
最后一步跟前面是一样的，只是按下s键把物体变小了，再按i键插入关键帧，最后把动画总帧数限制为100帧，这样动画就创建完成了。接着下来，就进观察这个动画是否符合我们设计的效果，按下底部播放键：
![](https://img-blog.csdn.net/20180828114416923?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这样就可以预览动画的效果，可见制作动画就是这么简单的一个过程。不过，这个查看是没有使用摄像头观看的，如果要那样查看，需要从菜单里选择这个：
![](https://img-blog.csdn.net/20180828115055133?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
当然，你也可以按下快捷键：Ctrl + F11,就会显示如下窗口：
![](https://img-blog.csdn.net/201808281153491?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
可以看到动画的结果了，这样就完成动画的制作过程了。接着下来是保存文件，如下图：
![](https://img-blog.csdn.net/20180828115526394?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
最后一步，也是最关键的一步，就是导出动画给游戏开发人员使用，有很多文件格式可以导出，最常用的使用是fbx格式，采用下面的菜单来导出：
![](https://img-blog.csdn.net/2018082811574346?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
好吧，这样就可以拿到动画数据了，我们来把这个动画保存名称为Test007.fbx，以便后面使用。
![](https://img-blog.csdn.net/20180828120032821?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
可见，通过blender制作一个动画是非常简单，快捷，导出功能也强大。
### Unity5.x游戏基础入门
### [http://edu.csdn.net/course/detail/4810](http://edu.csdn.net/course/detail/4810)
### TensorFlow API攻略
### [http://edu.csdn.net/course/detail/4495](http://edu.csdn.net/course/detail/4495)
### TensorFlow入门基本教程
### [http://edu.csdn.net/course/detail/4369](http://edu.csdn.net/course/detail/4369)
### C++标准模板库从入门到精通
### [http://edu.csdn.net/course/detail/3324](http://edu.csdn.net/course/detail/3324)

