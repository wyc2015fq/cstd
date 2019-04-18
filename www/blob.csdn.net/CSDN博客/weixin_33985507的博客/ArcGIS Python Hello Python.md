# ArcGIS Python Hello Python - weixin_33985507的博客 - CSDN博客
2017年06月16日 11:36:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
今年写下ArcGIS[Python](https://link.jianshu.com?t=http://lib.csdn.net/base/python)的第一章，本来也是从零开始学习[python](https://link.jianshu.com?t=http://lib.csdn.net/base/python)，也就是记录下自己学习Python的过程，万丈高楼平地起，只要心中有梦，脚下有行动，自然最终攀高峰！自勉
1、为什么要用Python呢？在arcgis里面为什么要用Python？
2、从什么地方开始学习Python？
百度百科里面的解释：Python（英语发音：/ˈpaɪθən/）, 是一种[面向对象](https://link.jianshu.com?t=http://baike.baidu.com/view/125370.htm)、解释型[计算机程序设计语言](https://link.jianshu.com?t=http://baike.baidu.com/view/2561555.htm)，由[Guido van Rossum](https://link.jianshu.com?t=http://baike.baidu.com/view/2975166.htm)于1989年底发明，第一个公开发行版发行于1991年。Python语法简洁而清晰，具有丰富和强大的类库。它常被昵称为[胶水语言](https://link.jianshu.com?t=http://baike.baidu.com/view/2993364.htm)，它能够把用其他语言制作的各种模块（尤其是[C](https://link.jianshu.com?t=http://baike.baidu.com/view/10075.htm)/[C++](https://link.jianshu.com?t=http://baike.baidu.com/view/824.htm)）很轻松地联结在一起。常见的一种应用情形是，使用Python快速生成[程序](https://link.jianshu.com?t=http://baike.baidu.com/subview/17674/17674.htm)的原型（有时甚至是程序的最终界面），然后对其中有特别要求的部分，用更合适的语言改写，比如[3D游戏](https://link.jianshu.com?t=http://baike.baidu.com/view/96860.htm)中的图形渲染模块，性能要求特别高，就可以用C++重写。
官网：https://www.python.org/
对于使用ArcGIS的用户来说，ArcGIS提供了三种使用地理处理的功能，Toolbox、model Builder以及Python，同时在arcgis的字段计算器中也有Python的使用，可以看出来arcgis还是比较在乎Python的。那么在这三种地理处理中只有model builder和Python可以实现批处理和流程化操作，而在model builder中有一个缺点使得model builder在做处理时候有不尽人意，只能进行一次迭代过程，这是做批处理无法实现有效控制的。
那么接下来我们看如何在arcgis desktop中如何使用python
很简单吧，还有更好的呢，在arcgis的Python环境中能够有[智能](https://link.jianshu.com?t=http://lib.csdn.net/base/aiplanning)提示。
这是不是很帅，但是也发现了问题，只能按照顺序执行。那么我们就需要一个比较好的IDE来做工程项目，在这里我比较喜欢jetbrain的pycharm
智能提示和调试起来都是做的比较好的。另外安装完pycharm会自动或者可以手动扫描Python环境，不怎么需要另行配置arcpy环境 直接可以使用这才是对我们GISer最好的事情。
好了，环境做了，我们首先开始一个Hello Arcpy吧，我们首先要设置地理处理工作空间，然后对其中的数据进行遍历。
__author__=**'Administrator'****import**arcpyarcpy.env.workspace=**"D:\GP IDW\GP.gdb"**db=arcpy.ListDatasets()**print**(db)
结果就是打印处理数据集的名称，至于获得数据集后我们可以继续遍历要素类，最后得到自己想要数据进行处理
__author__=**'Administrator'****import**arcpyarcpy.env.workspace=**"D:\GP IDW\GP.gdb"**dbs=arcpy.ListDatasets()**for**db**in**dbs:**print**(db)**print**(dbs)
今天的Hello Arcpy 就先到这里
