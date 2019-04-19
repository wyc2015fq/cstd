# Nebula3资源子系统 - 逍遥剑客 - CSDN博客
2008年10月30日 00:44:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2969
所属专栏：[Nebula引擎研究](https://blog.csdn.net/column/details/thenebuladevice.html)
跟N2比起来, N3的资源子系统更加开放, 让程序员对资源的创建和管理更多的控制.  
Nebula3的资源有下面几个特性:
- 包装了一些其它Nebula子系统需要的数据
- 可以用ResourceId共享
- 可以在任何时候加载(初始化)和卸载
- 可以同步或异步加载 
例如典型的图形资源有网格和纹理, 但资源子系统并不局限于图形资源.  
资源子系统有两个操作层次( 可能以后会把他们放入两个不同的命名空间, 现在他们都是在Resources命名空间下 ): 
低层提供了真正的资源对象, 处理资源的共享, 加载和(次要的)保存. 低层的资源类有:
- ResourceId
- Resource 
- ResourceLoader 
- ResourceSaver 
- SharedResourceServer.  
高层资源子系统提供了资源管理, 这意味着根据用户的反馈动态的加载和卸载资源. 高层资源子系统的类有:
- ResourceProxy (又名: ManagedResource)
- ResourceProxyServer (又名: ResourceManager)
- ResourceMapper 
下面说明一下资源子系统的各个类是怎么协同工作的: 
一个ResourceId是一个唯一的资源标识符, 用来共享和定位磁盘上的数据(或者资源保存在哪). ResouceId是一个原子字符串(string atoms), 而Atom是一个常量字符串唯一的32-bit标识符, 这可以大大加快拷贝和比较, 并且可以减少内存占用, 因为标识符字符串只保存一份. 为了定位磁盘上的数据, ResourceId常常表示成一个合法的URL(例如一个ResourceId “texture:materials/granite.dds”, 会在运行时被解析成”file:///C:/Programme/[AppName]/export/textures/materials/granite.dds”.  
一个Resource对象实际上是资源数据的容器. 像纹理和网格这样特定的资源类型都是Resource类的子类, 并且实现了特定的接口. Resource的子类通常都是平台相关的(如D3D9Texture), 但是通过条件编译使其变成平台无关的. 并不像Nebula2那样, 资源对象并不知道怎样去组织, 加载或保存自己. 取而代之的是, 附属于它的一个合适的ResourceLoader或ResourceSaver. 因为Nebula程序很少输出数据, ResourceSaver只是为了完整性而存在. 换句话说, ResourceLoader是必须的, 因为他们是启用Resource对象的唯一途径. ResourceLoader具有整个资源装载过程的完全控制权. 它们可以是平台相关的, 而且也许会依赖于相关联的特定平台的Resource类. 这使得程序员可以对资源的装载过程相比Nebula2有更多的控制. 典型的资源加载类有StreamTextureLoader, MemoryVertexBufferLoader和MemoryIndexBufferLoader(从内存中加载顶点缓存和索引缓存). 
Resource类也提供了共同的接口用来同步和异步的资源加载. 同步加载可以这样做: 
res-> SetResourceId("tex:system/white.dds");
res-> SetLoader(StreamTextureLoader::Create());
res-> SetAsyncEnabled(false)
res-> Load()
if (res-> IsValid()) ... 这时资源加载已经成功了, 否则LoadFailed会返回true.
异步资源加载也很相似:
res->SetResourceId("tex:system/white.dds"); 
res->SetLoader(StreamTextureLoader::Create());
res->SetAsyncEnabled(true); 
res->Load(); 
资源这时进入等待状态...
只要 IsPending() return true, 就要重复地调用Load()... 当然真正的程序会在这时做一些其他的事情 
接下来的某个调用Load()后时刻, 资源的状态要么是Valid(资源已经准备好了), Failed(资源加载失败)或者Cancelled(等待中的资源被取消加载了) 
一个应用程序甚至是Nebula3的渲染代码通常都不需要关心这些, 因为资源管理层会处理他们, 并把异步加载的这些细节隐藏到资源代理后面.  
SharedResourceServer单件通过ResourceId来共享资源. 通过SharedResourceServer创建资源确保了每个资源只在内存中加载了一份, 而不管客户端的数目. 如果客户端的数目降低到了0, 资源会被自动卸载(这并不是合适的资源管理, 而应该是ResourceProxyServer应该关心的). 资源共享完全可以直接通过标准的Nebula3的创建机制来绕过这个问题.  
ResourceProxy(或ManagedResource)是对于实际资源对象的资源管理包装. 它的思想是包含的资源对象会受资源用途反馈的控制. 例如, 一个纹理代理会在被请求的纹理在后台加载时提供一个占位纹理, 屏幕上所有使用这个资源的物体都很小的话会被提供一张低分辨率的纹理, 一个X帧没有被绘制的纹理会被卸载, 等等.  
ResourceProxyServer(或ResourceManager)单件是资源管理系统的前端. 除了管理附属于它的ResourceMapper的工作外, 它还是ResourceProxy的工厂, 并且把ResourceMapper跟Resource类型联系到了一起.  
ResourceMapper是一个有趣的东西. 一个ResourceMapper跟一种资源类型(如纹理或网格)相关联, 并被应用程序依附到ResourceProxyServer. 它负责从渲染代码的使用反馈来加载/卸载资源. ResourceMapper的子类可以实现不同的资源管理策略, 也可以通过派生特定的ResourceMapper和ResourceLoader来创建一个完全定制的平台和应用相关的资源管理方案. 目标是显而易见的, Nebula3提供了一些好用的ResourceMapper来加载需要的任何东西.  
资源使用反馈是由渲染代码写入ResourceProxy对象的, 而且包含了这个资源的一些信息:是否会在不久后用到, 是否可见, 并估计物体占用的屏幕空间大小. 特定的反馈依赖于特定的ResourceProxy子类, ResourceProxy中没有公共的反馈方法.  
基于资源的使用反馈, 一个ResourceMapper应该实现下面的一些操作(这取决于具体的mapper): 
- Load: 根据level-of-detail异步加载资源(如跳过不需要的高分辨率mipmap层次)
- Unload: 完全卸载资源, 释放珍贵的内存
- Upgrade: 提高已加载资源的level-of-detail(如加载高分辨率的mipmap层次纹理)
- Degrade: 降低已加载资源的level-of-detail(如跟上面相反的情况)
