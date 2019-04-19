# September Nebula3 SDK 中的新东西 - 逍遥剑客 - CSDN博客
2008年09月29日 17:25:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2222标签：[平台																[工具																[animation																[shader																[代码分析																[resources](https://so.csdn.net/so/search/s.do?q=resources&t=blog)](https://so.csdn.net/so/search/s.do?q=代码分析&t=blog)](https://so.csdn.net/so/search/s.do?q=shader&t=blog)](https://so.csdn.net/so/search/s.do?q=animation&t=blog)](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=平台&t=blog)
个人分类：[引擎研究](https://blog.csdn.net/xoyojank/article/category/337622)
所属专栏：[Nebula引擎研究](https://blog.csdn.net/column/details/thenebuladevice.html)
今天国庆放假的第一天, 饿得无聊就出来找东西吃, 转着转着就来到公司了. 就我一人, 真爽
想想这几天看看N3吧, 工作上的事情还是按计划来
上作者blog一看, 又有新东西啦, 大体翻译一下:
------------------------------------------------------------------------------------------------------------------------
我最后还是抽出时间打包了一个新的N3 SDK. 我会在星期一回到办公室时上传上去,  同时这里有一个大体的列表.  很多底层的东西改变了, 所以我不得不移除一些不错的前台特性(如在实现了多线程的渲染器后N2的角色渲染必须删除, 并且届时shader光照的代码会出错). 我会在下一个release版本中去考虑这些前台的特性. 
概述
- 
- 一些为了兼容Nebula2的代码所做的修改, 主要是一些宏的名字受到影响(DeclareClass -> __DeclareClass, ImplementSingleton -> __ImplementSingleton etc...)
- 着手删除#ifndef/#define/#endif 这些防止重复include的宏, 因为几乎所有的编译器(VStudio, GCC, Codewarrior) 都支持#pragma once 
- 把同的样Win32 和Xbox360 代码移动到一个共同的Win360 命名空间来消除代码冗余 
- 加入了一个新的Toolkit层, 它包含了一些导出工具和辅助类 
- 加入和整理了一些 Doxygen(文档) 页面
编译系统
- 重新组织了 VStudio解决方案的结构, 让所有的依赖工程都在一个解决方案中, 这样就不用再同时打开多个VStudio了
- 现在可以通过.epk编译脚本来导入VStudio工程(对于不在Nebula3 SDK目录下的工程很有用)
- 新的"projectinfo.xml" 文件为一些有用的导出工具定义了工程和平台特有的属性
- 把 export.zip 档案文件分割到一个独立的平台无关文件和几个特定平台的文件 (export.zip 包含所有平台无关的文件, export_win32.zip, export_xbox360.zip, export_wii.zip 包含特定平台的文件)
- 加入一个统一的多平台支持到 asset-pipeline (如 "msbuild /p:Platform=xbox360" 来生成XBOX360的东西)
- 一个新的命令行生成工具 (有代码):
- audiobatcher3.exe (包装了音频导出) 
- texturebatcher3.exe (包装了纹理导出) 
- shaderbatcher3.exe (包装了 shader 编译) 
- buildresdict.exe (生成资源词典文件) 
- 这些工具大部分只是调用其它的生成工具(像xactbld3.exe, nvdxt.exe, 还有其它命令下的生成工具)
- 注意公开的N3-SDK因为法律原因只包含Win32平台的支持 
基础层
- 修正Core::RefCounted 和Util::Proxy 引用计数线程不安全的BUG 
- 加入 WeakPtr<> 类用于更好地处理环形引用
- 在 Ptr<>中加入类型转换的方法
- 简化System::ByteOrder 类接口
- 加入平台相关的面向任务的"virtual CPU core id" (如 MainThreadCode, RenderThreadCore, 等等...)
- 加入一个 System::SystemInfo 类
- 加入 Threading::ThreadId 类型和 Threading::Thread::GetMyThreadId()静态方法
- 现在可以在VStudio调试器和其它的高度工具中看到线程的固有名称了 
- SetThreadIdealProcessor() 现在用于在Win32平台上把线程分配给可用CPU核心 
- 新的线程子系统的HTTP 调试页面(现在只列出Nebula3的活动线程) 
- MiniDump支持: 崩溃, n_assert()和 n_error() 现在在Win32平台上会生成 MiniDump 文件 
- 新的 Debug 子系统用于代码分析: 
- 提供 DebugTimer 和 DebugCounter 对象 
- HTTP 调试页面允许在运行时检查DebugTimers和 DebugCounters
- 新的Memory::MemoryPool 类来分配同样大小的内存块(加快分配速度和减少内存碎片) 
- Math::matrix44在中的一些新的和改名的方法
- Http 子系统现在运行在它自己的线程里 
- 把 SVG 支持加入到 Http 子系统(Http::SvgPageWriter 和Http::SvgLineChartWriter) (xoyojank:难道是Scalable Vector Graphics?这样的话可以输出图表了) 
- 加入 IO::ExcelXMLReader 流读取类, 允许读取XML模式的MS Excel电子表格文件 
- 在Messaging::AsyncPort加入行为方式, 定义了处理线程怎样去等待新的消息: 
- WaitForMessage: 在消息到达前一直阻塞 
- WaitForMessageOrTimeOut: 在消息到达或超时前一直阻塞 
- DoNotWait: 不等待消息
- 加入 Remote 子系统, 允许通过TCP/IP连接远程控制N3应用程序
渲染层
- 把渲染移动了它自己的线程 (InternalGraphics子系统在渲染线程这边,  Graphics 前端子系统在主线程这边) 
- 加入了 CoreAnimation 和 Animation 子系统 (构造中) 
- 为简单的用户界面加入了UI子系统 (构造中) (xoyojank: 这个不错^_^) 
- 加入CoreAudio和 Audio 子系统(构造中): 
- CoreAudio 是后台的, 运行在自己的线程里 
- Audio 是前台的"客户端", 运行在主线程里 (或者其它任何线程) 
- 围绕XACT的概念设计 
- 提供 XACT 的包装实现
- 加入 CoreGraphics::TextRenderer 和 CoreGraphics::ShapeRenderer 类, 打算用于渲染调试信息 
- 加入调试渲染子系统(现在在Debug命名空间下) 
- Frame 子系统: FramePostEffect 现也也许会包含 FrameBatch 
- Input 子系统: 断开 XInput 游戏手柄接口现在对于连接中的设备每隔0.5秒才检测一次 
- Resources 子系统: 加入 ResourceAllocator/ResourceLump 系统为Console平台真正的资源流做准备
应用层和插件:
- 删除了 CoreFeature (这东西不得不进入GameApplication类来阻止鸡生蛋问题) 
- 加入 NetworkFeature (构造中) 
- 加入 UIFeature (构造中) 
- 加入 CoreNetwork 和 Multiplayer 插件(RakNet的包装)
