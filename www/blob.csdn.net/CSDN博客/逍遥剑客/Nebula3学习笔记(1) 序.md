# Nebula3学习笔记(1): 序 - 逍遥剑客 - CSDN博客
2008年07月01日 15:20:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：6122标签：[操作系统相关																[http服务器																[scripting																[application																[sqlite																[脚本](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)](https://so.csdn.net/so/search/s.do?q=sqlite&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=scripting&t=blog)](https://so.csdn.net/so/search/s.do?q=http服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=操作系统相关&t=blog)
个人分类：[引擎研究](https://blog.csdn.net/xoyojank/article/category/337622)
所属专栏：[Nebula引擎研究](https://blog.csdn.net/column/details/thenebuladevice.html)

**前言**
目前这个引擎只是一个预览版, 很多功能仍在开发当中
硬件需求: 显卡支持ShaderModel3.0
作者Blog: [http://flohofwoe.blogspot.com](http://flohofwoe.blogspot.com/)
**用到的开源工程**
为了避免版本之间的不兼容, 已经在SDK中包含
- LUA ([http://www.lua.org](http://www.lua.org/)) 
- SQLite ([http://www.sqlite.org](http://www.sqlite.org/)) 
- TinyXML ([http://www.grinninglizard.com/tinyxml](http://www.grinninglizard.com/tinyxml)) 
- ZLib ([http://www.zlib.net](http://www.zlib.net/))
**体系结构一览**
- Nebula3 分成三层, 每一层都是建立在另一层之上的: - 基础层: 最底层, 提供了一个图形和音频之下基本的平台抽象. 基础层可以用作任意类型的程序开发平台, 而不仅仅是实时3D程序. 
- 渲染层: 这是中间层, 它在基础层之上另加了许多特性, 像3D渲染, 音频, 物理和场景管理等. 
- 应用程序层: 这是最高的一层, 提供了一个完整的游戏框架, 这使得开发人员可以集中精力在游戏逻辑上, 而不用对关心各种细节实现.
- Nebula3 会跟Mangalore 合为一个整体, Mangalore的各种子系统会集成到Nebula3的适合它们的层中去. 
- Nebula3 比 Nebula2更趋向于使用C++. 
- Nebula3通过引用计数和智能指针实现了对象生命周期的管理. 
- Nebula3的新对象模型使用一个4 byte的基类来代替Nebula2中70+ bytes的. 
- RTTI更高效, 更易用. 
- Nebula3仍然不使用C++异常, RTTI和STL(所有这些不是降低性能就是降低便携性).
- 根据类名来创建对象更快更易用.
- Nebula3 避免使用C Lib, 去除了附加的代码层. 
- Nebula3 使用LUA 代替TCL作为标准的脚本语言(当然也可以增加其它脚本语言的支持)
**基础层**
- App包含各种Application类
- Attr属性注册
- Core                Nebula3的对象模型
- Debug程序内存转储和基于HTTP的调试信息(这个真是个好东西, 第一次听说)
- Http实现了HTTP服务器和客户端, 这样可以在浏览器中实时查看调试信息
- IO输入输出系统, 很方便
- Math基于C++的数学库, 没什么特别的
- Memory实现了定制的内存分配器
- Messaging消息定义
- Net最基本的基于TCP协议的C/S架构, 更多的功能会在高层的网络扩展中给出
- Scripting脚本系统
- System硬件平台和特定操作系统相关的一些特性
- Threading多线程
- Timing时间/定时器管理
- Util工具库, 包括各种容器, 数据结构, 还有一个强大的string类(抛弃STL吧!)
**渲染层**
- CoreGraphics图形库核心类
- Frame基于帧的处理, 如RenderTarget, PostProcess等
- Graphics一些实体类型定义, 如Camera之类
- Input输入设备, 就是鼠标键盘手柄…..
- Lighting光照处理, 包括阴影算法(竟然要SM3.0@_@)
- Models骨骼模型&粒子系统
- RenderUtil目前就只有一个Maya的摄像机…..
- Resources资源管理器
**应用程序层**
application_win32工程就是一个简单的游戏, 框架, 但是已经包含了许多特性, 如果物理, 数据库等. 不过好像还缺少GUI系统.
**插件**
目前有三个, 基于ODE的物理引擎, 基于SQLite的本地数据库, 基于nebula2模型的骨骼模型系统
**编译**
没什么好说的, 打开.sln直接编译就成了, 没见过这么简单的开源工程^_^
