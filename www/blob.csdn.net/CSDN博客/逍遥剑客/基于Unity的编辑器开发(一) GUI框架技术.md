# 基于Unity的编辑器开发(一): GUI框架技术 - 逍遥剑客 - CSDN博客
2018年04月26日 10:13:54[xoyojank](https://me.csdn.net/xoyojank)阅读数：4950

- [需求背景](#需求背景)
- [技术挑战](#技术挑战)
- [GUI框架技术](#gui框架技术)- [Unity渲染窗口](#unity渲染窗口)
- [GUI](#gui)
- [Docking](#docking)
- [Property Editor](#property-editor)
- [小结](#小结)
# 需求背景
用Unity开发的游戏, 需要做个关卡编辑器. 如果只是内部用用, 其实直接扩展Unity就够了; 但问题是需要发布给外部的用户使用, 那么总不能把全套资源和Unity发给他们, 所以就诞生了这么个奇葩需求. 
# 技术挑战
- 既然用了Unity引擎, 那么渲染肯定还是用Unity, 怎么在外面套一堆UI面板?
- UI需要使用传统的方式来开发, 什么Qt/Winforms/WPF/Web等, 用Unity内置的UI也是一个思路
- 游戏内的逻辑基本都在mono虚拟机上跑, 外面再搞一套UI逻辑的话, 怎么做到公共代码的复用?
- Unity在不改源码的情况下没法直接跟外部的UI框架逻辑互调, 所以需要进程间通信
- 在跨进程的情况下, 怎么做到比较方便的方法调用和属性编辑?
# GUI框架技术
## Unity渲染窗口
问题就是怎么把Unity嵌入到现有的一些GUI窗口中? 搜索一下已经有人问过了: 
[https://answers.unity.com/questions/871077/how-to-display-unity-scene-in-a-qt-window.html](https://answers.unity.com/questions/871077/how-to-display-unity-scene-in-a-qt-window.html)
[https://stackoverflow.com/questions/44059182/embed-unity3d-app-inside-wpf-application](https://stackoverflow.com/questions/44059182/embed-unity3d-app-inside-wpf-application)
都是利用了Unity的一个命令行参数”-parentHWND”, 游戏启动后会自动做为子窗口嵌入到指定的HWND窗口中.  
还有另外一个思路, 就是使用类似ActiveX的Web控件技术, 不过这种对于游戏的改动太大, 使用上也有比较多限制, 所认不推荐.  
Unity安装完后在Editor\Data\Documentation\en\uploads\Examples\EmbeddedWindow.zip下面有一个嵌入Winforms的示例, 算是一个很好的开始: 
![这里写图片描述](https://img-blog.csdn.net/20180425125349108?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## GUI
本来想尝试一下使用Web前端技术做UI的, 比如像[https://threejs.org/editor/](https://threejs.org/editor/), [https://victhorlopez.github.io/editor/](https://victhorlopez.github.io/editor/), [https://webglstudio.org/demo/](https://webglstudio.org/demo/), [https://www.mixamo.com](https://www.mixamo.com) 等, 看起来挺酷, 但是考虑到Unity在Web端的技术限制和游戏代码的共享, 放弃了这个方案, 留给下次做工具时再尝试吧.  
如果是C++项目, 那我会首选Qt, 之前折腾CLI的interop给自己留了下心理阴影…因为Unity游戏的主力语言还是C#, 所以只要选了Qt, 就绕不过interop, 放弃好了. 
然后是WPF, 之前也折腾过一阵子, 虽然看起来很酷, 但是自己想做出那么酷的效果还是要花比较多的精力, 上手成本也是个问题, 编辑器做到差不多的时候总要有其他人来接手维护, 像我几年没用过WPF都忘了怎么写XAML了-_-, 算了, 还是选个最简单的Winforms吧, 完全不需要什么学习成本. 
## Docking
对于一个面板很多的编辑器来说, Docking应该是必备的一个特性, 要不然就像以前的MFC开发的编辑器一样, 一堆Tabs挤在一起…本来WPF那边的选择有很多, 不过放弃WPF后, Winforms这边, 如果不选择商用控件库, 那么最好用的应该就是[DockPanel Suite](https://github.com/dockpanelsuite/dockpanelsuite). 不过在找Theme Style相关的方案过程中, 意外发现了个[DarkUI](https://github.com/RobinPerris/DarkUI), 跟Unity整合一下看起来还不错: 
![这里写图片描述](https://img-blog.csdn.net/20180426093109921?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
不过里面的控件类型不是很全, 考虑到后期造轮子的成本, 选择暂时放弃, 后续有需要可以参考它的控件风格实现. 如果只是Docking的话, 还是[DockPanel Suite](https://github.com/dockpanelsuite/dockpanelsuite)做得更好. 但是对于编辑器来说, 黑色风格的显然看起来更加高大上. 
## Property Editor
每次做编辑器都躲不开做个通用属性编辑, 选择C#也是有这方面的考虑. 想当年用MFC开发编辑器时, 每个参数都加一个控件, 那体验…维护起来简直想死. 后来我就入了.Net Interop的坑, 想来可能仅仅是因为这个PropertyGrid: 
![这里写图片描述](https://img-blog.csdn.net/20180426095634282?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
再后来放弃了C#, 参考[HeliumProject](https://github.com/HeliumProject)基于C++搞了一整套反射序列化, 用Qt实现了一个通用的属性编辑器.  
![这里写图片描述](https://img-blog.csdn.net/20180426094542175?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
不过这东西想要做到体验好, 还是需要非常多的定制化控件, 而3D游戏的基础数据类型每个引擎的定义都不一样, 很难做成通用的. 有空的还是想基于C++搞一套反射+序列化+GC+属性编辑+脚本绑定的通用库, 这样再做其它的编辑器就很轻松了.  
跑题了, 回到C#这边. WPF能找到的比较不错的属性编辑器大多是商用的: 
![这里写图片描述](https://img-blog.csdn.net/2018042610235370?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
开源的做得还不错的有[Property Tools](https://github.com/objorke/PropertyTools), 以前我干过把它嵌入到MFC编辑器里的奇葩事, 这次差一点又想把它嵌入Winforms里用. 之所以没选择它, 那是因为我发现了个更好的: [AdamsLair.WinForms](https://github.com/AdamsLair/winforms), 开源2D引擎[Duality](https://github.com/AdamsLair/duality)背后的一个控件库. 
![这里写图片描述](https://img-blog.csdn.net/20180426095352676?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
它自带的编辑器Dualitor已经证明了这个属性编辑控件的强大, 针对不同的游戏引擎的数据类型也已经有一些扩展控件参考: 
![这里写图片描述](https://img-blog.csdn.net/20180426100317612?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![这里写图片描述](https://img-blog.csdn.net/20180426100055940?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![这里写图片描述](https://img-blog.csdn.net/2018042610011257?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 小结
Dualitor中除了属性编辑, 其它特性的实现也非常不错, 值得拿来学习. 因为2D引擎没有3D引擎那么复杂, 所以其中的代码逻辑还是比较清晰条理的, 本身的设计理念又跟Unity非常类似, 所以照着它的结构跟Unity又杂交了一下: 
![这里写图片描述](https://img-blog.csdn.net/20180426100742851?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
看起来还不错, 结合Winforms的所见即所得的GUI编辑器, 再加上这次也不用折腾Interop了, 算是一个挺不错的开始.
腾讯云自媒体分享计划: [https://cloud.tencent.com/developer/support-plan?invite_code=1lss32a6pqj5w](https://cloud.tencent.com/developer/support-plan?invite_code=1lss32a6pqj5w)
