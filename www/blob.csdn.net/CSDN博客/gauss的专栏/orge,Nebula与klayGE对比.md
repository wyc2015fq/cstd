# orge,Nebula与klayGE对比 - gauss的专栏 - CSDN博客
2011年12月04日 09:07:20[gauss](https://me.csdn.net/mathlmx)阅读数：8934
1.       orge([**鬼怪**](http://imgcache.qq.com/qzone/qzfl/editor/assets/blank.htm#))
2.       [**Irrlicht**](http://imgcache.qq.com/qzone/qzfl/editor/assets/blank.htm#)（[**鬼火**](http://imgcache.qq.com/qzone/qzfl/editor/assets/blank.htm#)），
3.       [**Nebula**](http://imgcache.qq.com/qzone/qzfl/editor/assets/blank.htm#)([**星云**](http://imgcache.qq.com/qzone/qzfl/editor/assets/blank.htm#))
4.       klayGE（。。）
前言：目前，开源3d游戏引擎已经趋近于商业化引擎标准，许多引擎在图形渲染，内存分配，[**网络**](http://imgcache.qq.com/qzone/qzfl/editor/assets/blank.htm#)通讯方面甚超越了许多商业引擎。这些[**开源**](http://imgcache.qq.com/qzone/qzfl/editor/assets/blank.htm#)引擎的共同特点是这样的：
（1）引擎[**框架**](http://imgcache.qq.com/qzone/qzfl/editor/assets/blank.htm#)：多数的3d引擎框架为树形结构。框架最为清晰、经典的游戏引擎是Nebula星云引擎。父子节点构成了整个框架的层次，首先nebula有一个叫nRoot的类,它是所有对象类的基类,nRoot中有一个名字变量,有一个parent指针指向它的父结点,
　　还有一个childlist链表保存它所拥有的子结点.因为结点也是nRoot类,所有结点的子结点中也可能有子结点,
　　这样就形成了一个树形目录结构.nRoot中有一个nRoot* Find( const char* name )方法,功能很简单,就是遍历自已的
　　childlist链表对比每一个nRoot结点对象的名字,如果相同就返回些对象的指针.。其他引擎也有类似的框架，比如orge的root根节点下面子节点代表每一个功能[**模块**](http://imgcache.qq.com/qzone/qzfl/editor/assets/blank.htm#)。
（2）内存[**管理**](http://imgcache.qq.com/qzone/qzfl/editor/assets/blank.htm#)：多数采取引用计数[**模式**](http://imgcache.qq.com/qzone/qzfl/editor/assets/blank.htm#)（引用计数为每当初始化一个指针，在函数运行完毕后自动释放，并且把这个过程进行计数）这样的好处在于对于[**项目**](http://imgcache.qq.com/qzone/qzfl/editor/assets/blank.htm#)的可控性很有保障，避免内存泄漏，重命名对象等。但是这种
（3）图形处理：多采取三个经典模块：场景管理器，资源管理器，渲染器。这种划分模式以orge鬼怪引擎和irrlicht鬼火引擎最为典型。先进的引擎还配有一套专门从事对于图形编程的脚本语言。比如orge内置一套脚本，可以控制场景管理器，资源管理器的图形参数及图形算法进行编辑，这样可以跳过c++繁冗的[**程序**](http://imgcache.qq.com/qzone/qzfl/editor/assets/blank.htm#)编写过程和漫长的项目编译时间而直接运行察看修改后程序的效果。当然，这种脚本语言模式有些效仿目前最著名的商业游戏引擎unreal3虚幻3引擎。关于场景的图形算法大多数采用8叉树与bsp（二叉空间分割）方法，支持大游戏场景的局部渲染功能。
（4）跨[**平台**](http://imgcache.qq.com/qzone/qzfl/editor/assets/blank.htm#)性：
本文要介绍的4 个3d引擎都具有跨平台[**开发**](http://imgcache.qq.com/qzone/qzfl/editor/assets/blank.htm#)性能，一般可以支持多种操作[**系统**](http://imgcache.qq.com/qzone/qzfl/editor/assets/blank.htm#)和开发[**工具**](http://imgcache.qq.com/qzone/qzfl/editor/assets/blank.htm#)，比如windous下的vs.net，linux下的codewarrior等。而且像irrilicht鬼火这种引擎还支持多种语言，vc，vb等，但是目前版本其本身并无脚本可用。这些引擎还支持多种底层图形api，比如
 dx，opengl等。开发者可以随意[**选择**](http://imgcache.qq.com/qzone/qzfl/editor/assets/blank.htm#)熟悉的3d渲染api来驱动游戏程序。注意：KlayGE是本文唯一入选的国产引擎，但该引擎没有支持多api 渲染[**机制**](http://imgcache.qq.com/qzone/qzfl/editor/assets/blank.htm#)，底层图形方面只是用opengl实现。
（5）网络部分功能较差：目前说到的几个引擎在网络方面都不是很强大，比如著名的orge引擎就是专注的图形引擎，不具备服务器-客户端的网络工作模式。目前，只有北京涂鸦产的起点引擎在网络方面在国产引擎中可以算是比较出色。值得一提的是在网络方面比较强大的商业引擎目前最火的就是bigworld，如果利用适当的渲染算法该引擎可以轻松支撑上百人
的在线pk。
（6）可接收多种 3d美术资源，比如鬼火引擎可以接受Maya，3DStudio Max，COLLADA，DeleD，Milkshape，Quake 3 levels，Quake2 models，DirectX，Pulsar，My3DTools，FSRad以及Cartography Shop工具导出的资源。但是orge可接受的种类要更多。
支持骨骼动画和关键贞动画两种经典动画形式。
好了，目前我们基本了解了市面上著名的3d引擎的基本共同点，那么下面就列举以下他们的[**特性**](http://imgcache.qq.com/qzone/qzfl/editor/assets/blank.htm#)及优缺点。
1．Orge鬼怪：
优点：（1）强悍的图形渲染质量。该引擎的粒子系统很强，[**制作**](http://imgcache.qq.com/qzone/qzfl/editor/assets/blank.htm#)的光影，雾化效果极佳，图形方面全面超过torque,朱比特等普通商业引擎的能力。
（2）高效的图形编程机制。内置的特殊脚本语言，并且具有可视化的编辑工具，可以直接进行外部[**数据**](http://imgcache.qq.com/qzone/qzfl/editor/assets/blank.htm#)修改，内部调试。
缺点：（1）网络部分的空白
（2）高度计算量和高精度的渲染模式，使得内存和显存的压力都很大，做出的程序整体效率不高。没有相关随着硬件加速调整渲染算法和参数的api。
2．Irrlicht鬼火:
优点：（1）精悍的引擎框架，和orge及其相似。
（2）高质量的图形渲染，虽然没有orge的质量优越，但是比起一般的商业引擎仍然略胜一筹。
（3）清晰单一的c++语言实现使得引擎没有繁冗庞大的外部脚本编译器。
（4）支持根据硬件加速调整渲染模块的参数和算法的机制。整体程序会跑得很快流畅，但是画质降低。牺牲画质来保证性能。
缺点：（1）缺乏高效开发的脚本。无法像orge那样高质量高效率的进行图形编程。
（2）相对其它游戏引擎鬼火的框架可扩展性不强，[**设计**](http://imgcache.qq.com/qzone/qzfl/editor/assets/blank.htm#)模式相对单调而简洁。
（3）支持图形算法单一，比如骨骼动画多贴图需要很麻烦的过程。
