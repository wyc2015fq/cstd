
# 游戏制作之路（4）Unity添加游戏对象 - 大坡3D软件开发 - CSDN博客

2018年08月29日 15:39:57[caimouse](https://me.csdn.net/caimouse)阅读数：1110


在前面学习会使用blender制作游戏的模型或动画，那么这些动画和模型可以放到游戏里了，但是怎么样制作成游戏呢？一般制作游戏有两种方法，一种就是从头开始，所有代码全部自己造；另外一种就是使用现有游戏引擎。在本文里使用最后一种方法，使用现成的游戏引擎，前面那种方法以后再考虑吧。
在选择不同的游戏引擎，又有很多选择的，在这里主要选择unity引擎，你也许问为什么？其实，我之所以选择它，就是因为王者荣耀使用它来开发的,因此，如果想开发游戏跟这样的游戏竞争，必须站在同样的起点上，才有机会PK一下。
好吧，这样就选择了unity这个工具了，别的就放下。要使用这个工具之前，必须进行下载和安装，这个具体过程，我就不说了，如果不懂，可以参考我的课程：
### Unity5.x游戏基础入门
### [http://edu.csdn.net/course/detail/4810](http://edu.csdn.net/course/detail/4810)
接着，安装好了，就可以进行下面的操作了。
双击桌面的unity图标：
![](https://img-blog.csdn.net/20180829143756225?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
就打开像下面的窗口：
![](https://img-blog.csdn.net/20180829143938232?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
可以看到前面有new、open这两项，new就是创建新项目，open就是打开以前的项目。在这里选择new这项，就会出现下面界面：
![](https://img-blog.csdn.net/2018082914500329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这个就是创建新项目的界面，在project name这里输入项目的名称，这里叫作UnitySeries1，在3D、2D选项里，选择3D。后面使用默认的即可，继续点击create project下一步：
![](https://img-blog.csdn.net/20180829145603436?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Unity有5个主要区域：Scene, Game, Hierarchy, Project, 和Inspector。Scene窗口是编辑游戏世界所有相关物体，可以通过鼠标进行操作这里的物体。右键点下时，可以使用WASD来查看整个世界，使用QE按键来进行升降查看。如果没有显示这个界面，可以通过下面的菜单来选择：
![](https://img-blog.csdn.net/20180829150456336?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这样选择之后，肯定会出现上面界面布局了。
接着下来，要在游戏世界里添加物体，这样怎么样操作呢？
首先在菜单里选择GameObject->3D Object->Capsule：
![](https://img-blog.csdn.net/20180829150707348?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这样就可以创建一个3D的物体：小容器，如下图：
![](https://img-blog.csdn.net/20180829150919917?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在这里可以选择capsule，它是这个物体的名称，就可以右边查看它的属性，如下图：
![](https://img-blog.csdn.net/20180829151205517?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在这里，可以看到inspector里就是物体的属性，第一个物体的名称，接着下面是它的相关属性。比如Transform对象，就表示这个物体在游戏世界里的位置、大小、方向。
接着下来，要对这个物体进行一些动作，比如让这个物体运动起来，因此要添加脚本，以便响应不同的事件，如下添加：
![](https://img-blog.csdn.net/2018082915172834?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
先要选中capsule，然后在inspector里，找到add component按钮，点击之后选择new script，就会出来这个窗口，然后在最上面输入脚本的名称为：BasicMovement，最后一步点击create and add，就可以创建脚本，并关联到这个物体。接着就可以看到脚本代码在项目里显示了：
![](https://img-blog.csdn.net/20180829152117160?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
到这里，终于看到代码了。在这脚本代码里，主要有两个方法：一个是Start ()，这个方法只是运行一次；另一个是Update ()，每一个游戏帧里运行一次，一帧是表示什么意思呢？其实这帧的概念是来自于电影，电影每秒播放24个图片，一帧就是一个图片。在游戏里，每秒钟更新多少个图片，取决于电脑性能。
我们来在Update ()方法里添加一行代码，如下：
```python
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
public class BasicMovement : MonoBehaviour {
	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
        transform.position += Vector3.right * Time.deltaTime;
    }
}
```
这行代码里，transform表示物体小容器，transform.position是表示小容器的位置不断地改变，Vector3.right是表示游戏世界右边方向，即是Vector3(1, 0, 0)；Time.deltaTime是表示游戏里最后一帧所使用的时间，这里Vector3.right * Time.deltaTime，就是相当于每秒移动多少距离，如果Time.deltaTime越大，表示每帧时间越大，移动的距离就要大一些，如果时间小，那更新的速度很快，那么就移动距离小一些。
最后点击下面的按钮，就可以运行起来了：
![](https://img-blog.csdn.net/20180829153905478?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
到这里，就完成unity的第一个项目了。
### 五子棋游戏开发
### [http://edu.csdn.net/course/detail/5487](http://edu.csdn.net/course/detail/5487)
### RPG游戏从入门到精通
### [http://edu.csdn.net/course/detail/5246](http://edu.csdn.net/course/detail/5246)
### WiX安装工具的使用
### [http://edu.csdn.net/course/detail/5207](http://edu.csdn.net/course/detail/5207)
### 俄罗斯方块游戏开发
### [http://edu.csdn.net/course/detail/5110](http://edu.csdn.net/course/detail/5110)
### boost库入门基础
### [http://edu.csdn.net/course/detail/5029](http://edu.csdn.net/course/detail/5029)

