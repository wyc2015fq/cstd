# Nebula3 SDK Nov 2009 更新内容 - 逍遥剑客 - CSDN博客
2009年11月06日 22:03:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2617标签：[http服务器																[ps3																[平台																[animation																[shader																[velocity](https://so.csdn.net/so/search/s.do?q=velocity&t=blog)](https://so.csdn.net/so/search/s.do?q=shader&t=blog)](https://so.csdn.net/so/search/s.do?q=animation&t=blog)](https://so.csdn.net/so/search/s.do?q=平台&t=blog)](https://so.csdn.net/so/search/s.do?q=ps3&t=blog)](https://so.csdn.net/so/search/s.do?q=http服务器&t=blog)
个人分类：[引擎研究](https://blog.csdn.net/xoyojank/article/category/337622)
所属专栏：[Nebula引擎研究](https://blog.csdn.net/column/details/thenebuladevice.html)
这是新版的 N3 SDK:[下载链接](http://213.191.63.156/n3_sdks/N3SDK_Nov2009.exe)
跟以往一样, 只包含了Win32平台的源码. Console平台特定代码 (Xbox360, PS3 还有 Wii) 介于法律问题没有包括进去.
这是Apr2009 SDK之后大体的改变:
**== ****主要新特性**
- XNAMath 在Win32 和Xbox360 平台上的统一支持
- PS3 支持(不属于公开的SDK, 但是针对GCC 4.x做了一些代码修正) HTTP 文件系统包装现在可以正常工作了, 这允许创建从HTTP服务器加载所有资源的N3应用程序(见testhttpviewer.exe 这个程序示例) 
- 支持“二进制XML” 以快速载入大的XML文件(克服TinyXML的缺点) 
- 新的“FrameSync” 系统, 适用于主线程和渲染线程以锁定步进(lock-step)的方式运行
- 新的“Jobs” 系统, 用于实现并行化工作(Win32 和Xbox360上的CPU线程池PS3上的SPU) 
- 父窗体支持, 现在能够以另一个窗口子窗口的形式进行渲染, 以便把N3嵌入到其它Windows应用程序中去
- 整合FMOD 
**==****基础层**
**Core**
- 新的Debug::StringAtomPageHandler , 可以在浏览器中查看string atom 表
- 优化Ptr<>中的类型转换方法
- RefCounted 对象支持从内存池分配(现在还不稳定!) 
**Util**
- Win32StringConverter: UTF-8 和宽字符字符串的转换(现在只有Win32 平台) 
- Util::Array::InsertSorted() 现在返回插入的元素索引
- 新的Util::BitField<> 类允许多于32位的掩码操作
- 移除了老的string atom系统中的 Util::Atom<>, Util::Proxy<> 
- 新方法Util::FixedArray<>::Resize() 
- String atom系统的新类: Util::StringAtom, Util::StringBuffer, Util::LocalStringAtomTable, Util::GlobalStringAtomTable 
- 新方法Util::Queue<>::Reserve() 
- Util::RingBuffer<> 中增加直接访问的新方法
- 新方法Util::Round::RoundUp() 
- 新类Util::SparseTable, 用于有很多空格子的2D表格
- Util::String: - 新方法CopyToBuffer() 
- 新的经过优化的Tokenize() , 以分隔符填充一个字符串数组, 允许重用一个已经存在的数组对象
- 新的静态包装方法: IsDigit(), IsAlpha(), IsAlNum(), IsLower(), IsUpper(), StrCmp(), StrLen(), StrChr() 
- 新的有助于解决“类型双关(type punning)” 的工具函数
**IO**
- BXmlReader: “二进制XML 文件”的流读取器(用新的工具binaryxmlconverter3.exe 创建) 
- 一些ZIP 文件系统包装的底层优化
- 应用程序根目录之类的东西从AssetRegistry移到Core::CoreServer 
- 新类GameContentServer, 用在一些console平台上预备游戏数据
- 加入http:和httpnz: scheme的支持,可以通过N3文件系统从HTTP服务器读取数据
**Math**
- Xbox360 和 Win32 数学类统一成XNAMath类
- 底层的性能调整
**Memory**
- 在Win32平台上试验性的支持内存池
- 在Win32 平台上, 动态分配的内存现在是16字节对齐的(注意: 可能在Realloc()中有一个很难重现的严重BUG , 有时HeapSize() 会返回一个错误值) 
- 新的内存池HTML调试输出Debug::MemoryPageHandler 
**Threading**
- Threading::CriticalSection 根据"[Fast critical sections with timeout](http://www.codeproject.com/KB/threads/CritSectEx.aspx)" (作者Vladislav Gelfer)重写(Win32 平台) 
- Threading::Event 现在支持“手动重置(manual reset)” 行为
- Threading::Interlocked 类现在在Win32和Xbox360平台上使用编译器指令(compiler intrinsics) 
- 新类Threading::ThreadBarrier: 在其它所有线程到达障碍(barrier)时才会停止
- 优化Threading::SafeQueue 
**System**
- 新类System::Win32Environment用于访问环境变量(只有Win32平台) 
- Win32Registry 类现在以宽字符读取并转换为UTF-8 
- System::ByteOrder 中对于类型双关的修正
**Timing**
- 移除MasterTime/SlaveTime 系统, 全局计时(global timing)现在由FrameSync子系统提供
**Messaging**
- Message::SetHandled() 原本不是线程安全的, 现在使用Interlocked::Exchange()更新它的状态
- 为了更好地控制HandlerThreadBase子类的消息处理行为, Messaging::AsyncPort进行了重写
- 新的同步消息处理线程类BlockingHandlerThread, RunThroughHandlerThread 
**Net**
- debugpacket.cc中一些类型双关的修正
**Http**
- 新类HttpClientRegistry, HttpStream, HttpNzStream 用于实现一个透明的HTTP文件系统, HttpNzStream在客户端使用zlib解压缩以加速下载
**Debug**
- 没有值得一提的改变
**App**
- 新的标识符字符串AppTitle 和AppID, 这对于一些console平台是必需的
**Jobs**
- 这是一个用来在线程池(Win32和Xbox360)或PS3的SPU之间分发任务的新子系统
**FrameSync**
- 这是一个新的子系统, 在游戏线程与渲染线程之间实现了更好的同步
**== ****渲染层**
**CoreGraphics**
- DisplayDevice (Win32 平台)中父窗口相关的东西
- 现在可以在渲染目标(render target)之间共享深度/模板缓冲
- ShaderServer改为解析一个词典文件( 由shaderbatcher3.exe工具创建), 而不是像之前那样枚举shader目录下的内容
- shader变量中移除数组的支持(shader参数数组不是很好移植) 
- 针对PS3平台做了一些结构调整(一些类分成一个基类和一个平台特定的派生类) 
- 新的私有方法D3D9RenderDevice: SyncGPU() , 它在Present()中被调用, 用来防止GPU比CPU运行得过于超前(这是驱动内部的”优化”, 某些情况下会导致帧率卡顿) 
- 通过清除标记(clear flag)对渲染目标的清空进行更好的控制
- RenderDevice::SaveScreenshot()方法现需要设置输出流的MIME类型, 这是因为实际的MIME类型可能跟请求的类型不一致
- 加载网格(mesh)文件时不再需要进行字节顺序的转换, 这个操作现在改为在制作流程中进行
- 新类MemoryMeshLoader, 从已有的VertexBuffer 和 IndexBuffer对象创建一个网格(mesh) 
**CoreAudio ****和**** Audio**
- CoreAudio 和 Audio子系统已经废弃, 替换成基于FMOD的Audio2子系统, 自然而然地可以”自动”跨所有平台. 不过对于商业项目, 请检查一下FMOD的许可! 
**CoreAnimation**
- 以下类已经从CoreAnimation移除: AnimDrivenMotionSampler, AnimMixer, AnimSampler 
- 新的动画数据文件格式: nax3 
- 新的动画曲线类型: Velocity, 由AnimDrivenMotion特性使用
**Input**
- 没有值得一提的改变
**Frame**
- 针对[Light Pre-pass Rendering](http://blog.csdn.net/xoyojank/archive/2009/08/18/4460953.aspx)做了一些小的改动
- FramePass 中可以更好的控制渲染目标的清空
- FramePostEffect:全屏幕矩形的绘制已经移到一个新的辅助类RenderUtil::DrawFullScreenQuad中
- frame shader现在根据需要加载
- LightServer类: LightPrePassServer实现了light pre-pass rendering (延迟着色的一种变体, 现在只在PS3平台上实现) 
**Animation**
- 动作评估(anim evaluation)已经“任务化(jobified)” 
- Animation 子系统中没有一点AnimDrivenMotion 特定的代码了 (现在这个通过一个包含速度键(velocity key)的新动画曲线类型进行处理) 
**Audio2**
- 新的基于FMOD的跨平台音频子系统
**Characters**
- 骨骼评估(skeleton evaluation)已经“任务化” 
- 对于PS3, 蒙皮运行在SPU上
- 整个character子系统已经优化(差不多重写了) 
**InternalGraphics**
- 使用子系统使渲染线程和游戏线程以锁定步进的方式运行(这基本上解决了所有卡帧的问题) 
- 浏览器中通过GraphicsPageHandler显示更多的调试信息
- attachment系统的很多修正(角色关节附属物: 剑, 盾等…) 
- 对更新/渲染循环(Update/Render-Loop)进行重构, 改善并行化支持. 基本思想是在更新对象和渲染对象之间创造更多的空间, 使得异步工作有更多的机会按时完成渲染所需数据
**Graphics**
- 增加一些在主线程和渲染线程间通信的新消息(见graphicshandler.cc) 
**Models**
- 没有值得一提的改变
**Particles**
- 一些方便移植所做的结构调整
- 粒子的更新已经“任务化” 
**RenderModules**
- 没有值得一提的改变
**RenderUtil**
- 新的辅助类RenderFullScreenQuad 
- 新的辅助类 NodeLookupUtil 用于在层级结构中查找一个 ModelNodeInstance 
**Resources**
- 没有值得一提的改变
**== ****移进****Addon****的****:**
- fx 
- network 
- locale 
- posteffect 
- ui 
- vegetation 
- vibration 
- video 
**== ExtLib****中的新东西****:**
- FMOD 
- RakNet 
尽情享受吧!
