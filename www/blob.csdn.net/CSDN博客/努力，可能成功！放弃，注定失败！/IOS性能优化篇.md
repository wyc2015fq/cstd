# IOS性能优化篇 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年12月07日 10:41:26[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：116
![](https://upload-images.jianshu.io/upload_images/3067538-827f83acb7c4bde9?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
图片
好的应用应该有好的性能流畅度，本篇文章就大概讲一下IOS性能优化。
### 先来谈谈CPU和GPU
- 在屏幕成像的过程中，CPU和GPU起着至关重要的作用
- CPU（Central Processing Unit，中央处理器）就是机器的“大脑”，也是布局谋略，发号施令，控制行动的“总司令官”。
- CPU的结构主要包括运算器（ALU，算术逻辑单元），控制单元（CU，控制单元），寄存器（寄存器），高速缓存器（缓存）和它们之间通讯的数据，控制及状态的总线。
- GPU全称为图形处理单元，中文为图形处理器，就如它的名字一样，GPU最初是用在个人电脑，工作站，游戏机和一些移动设备（如平板电脑，智能手机等）上运行绘图运算工作的微处理器。
- 为什么GPU特别擅长处理图像数据呢？这是因为图像上的每一个像素点都有被处理的需要，而且每个像素点处理的过程和方式都十分相似，也就成了GPU的天然温床。
![](https://upload-images.jianshu.io/upload_images/3067538-b67499e59d74b80a?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
图片
在的iOS中是双缓冲机制，有前帧缓存，后帧缓存，即GPU会预先渲染好一帧放入一个缓冲区内（前帧缓存），让视频控制器读取，当下一帧渲染好后，GPU会直接把视频控制器的指针指向第二个缓冲器（后帧缓存）。当你视频控制器已经读完一帧，准备读下一帧的时候，GPU会等待显示器的垂直同步信号发出后，前帧缓存和后帧缓存会瞬间切换，后帧缓存会变成新的前帧缓存，同时旧的前帧缓存会变成新的后帧缓存。
### 屏幕成像原理
![](https://upload-images.jianshu.io/upload_images/3067538-6e03f67e4aa64a9c?imageMogr2/auto-orient/strip%7CimageView2/2/w/974/format/webp)
图片
### 卡顿产生的原因
![](https://upload-images.jianshu.io/upload_images/3067538-42451f7e9628e976?imageMogr2/auto-orient/strip%7CimageView2/2/w/962/format/webp)
图片
在同步信号到来后，系统图形服务会通过CADisplayLink等机制通知应用程序，应用程序主线程开始在CPU中计算显示内容，比如视图的创建，布局计算，图片解码，文本绘制等。随后CPU会将计算好的内容提交到GPU去，由GPU进行交换，合成，渲染。随后GPU会把渲染结果提交到帧缓冲区，等待下一次的VSync信号（垂直同步信号）到来时显示到屏幕上。由于垂直同步机制，如果在一个垂直同步时间内，CPU或者GPU没有完成内容提交，则那一帧就会被丢弃，等待下一次机会再显示，而这时显示屏因为没有新的刷新，会保留之前的内容不变。这就造成了卡顿。
- 按照60FPS的刷帧率，每隔16毫秒就会有一次垂直同步信号
### 卡顿优化-CPU
- 尽量用轻量级的对象，比如用不到事件处理的地方，可以考虑使用的CALayer取代的UIView
- 不要频繁地调用的UIView的相关属性，比如帧，边界，变换等属性，尽量减少不必要的修改
- 尽量提前计算好布局，在有需要时一次性调整对应的属性，不要多次修改属性
- 自动布局会比直接设置帧消耗更多的CPU资源
- 图片的大小最好刚好跟的UIImageView的大小保持一致
- 控制一下线程的最大并发数量
- 尽量把耗时的操作放到子线程
### 卡顿优化-GPU
- 尽量避免短时间内大量图片的显示，尽可能将多张图片合成一张进行显示
- 尽量减少视图数量和层次
- 减少透明的视图（阿尔法<1），不透明的就设置不透明为YES
- 尽量避免出现离屏渲染
### 离屏渲染
- 在OpenGL中，GPU有2种渲染方式1.On 
-Screen Rendering：当前屏幕渲染，在当前用于显示的屏幕缓冲区进行渲染操作
2.Off-Screen Rendering：离屏渲染，在当前屏幕缓冲区以外新开辟一个缓冲区进行渲染操作
- 离屏渲染消耗性能的原因
1.需要创建新的缓冲区
2.离屏渲染的整个过程，需要多次切换上下文环境，先是从当前屏幕（屏幕）切换到离屏（屏幕外）;等到离屏渲染结束以后，将离屏缓冲区的渲染结果显示到屏幕上，又需要将上下文环境从离屏切换到当前屏幕
- 哪些操作会触发离屏渲染？
1.光栅化，layer.shouldRasterize = YES 
2.遮罩，layer.mask 
3.圆角，同时设置layer.masksToBounds = YES，layer.cornerRadius大于0 
考虑通过CoreGraphics绘制裁剪圆角，或者叫美工提供圆角图片
4.阴影，layer.shadowXXX 
如果设置了layer.shadowPath就不会产生离屏渲染
### 耗电优化
耗电的主要来源？
![](https://upload-images.jianshu.io/upload_images/3067538-0c20d906ec02881f?imageMogr2/auto-orient/strip%7CimageView2/2/w/886/format/webp)
图片
- CPU处理，处理
- 网络，网络
- 定位，定位
- 图像，图形
1.定位优化
- 如果只是需要快速确定用户位置，最好用CLLocationManager的requestLocation方法。定位完成后，会自动让定位硬件断电
- 如果不是导航应用，尽量不要实时更新位置，定位完毕就关掉定位服务
- 尽量降低定位精度，比如尽量不要使用精度最高的kCLLocationAccuracyBest
- 需要后台定位时，尽量设置pausesLocationUpdatesAutomatically为YES，如果用户不太可能移动的时候系统会自动暂停位置更新
- 尽量不要使用startMonitoringSignificantLocationChanges，优先考虑startMonitoringForRegion：
### APP启动优化
先来看应用程序启动流程
APP的启动可以分为2种
1，冷启动（Cold Launch）：从零开始启动APP 
2，热启动（Warm Launch）：APP已经在内存中，在后台存活着，再次点击图标启动APP
APP启动时间的优化，主要是针对冷启动进行优化
- 通过添加环境变量可以打印出APP的启动时间分析（编辑方案 - >运行 - >参数）
1，DYLD_PRINT_STATISTICS设置为1 
2，如果需要更详细的信息，那就将DYLD_PRINT_STATISTICS_DETAILS设置为1
APP的冷启动概括为三大阶段
- 
`dyld`，Apple的动态链接器，可以用来装载Mach-O文件（可执行文件，动态库等）
`启动APP时，dyld所做的事情有`
1。装载APP的可执行文件，同时会递归加载所有依赖的动态库
2.当dyld把可执行文件，动态库都装载完毕后，会通知运行时进行下一步的处理
- 
`runtime`
`启动APP时，runtime所做的事情有`
1.调用map_images进行可执行文件内容的解析和处理
2.在load_images中调用call_load_methods，调用所有类和类的+加载方法
3.进行各种objc结构的初始化（注册Objc类，初始化类对象等等）
4.调用C ++静态初始化器和**属性**（（构造函数））修饰的函数
到此为止，可执行文件和动态库中所有的符号（Class，Protocol，Selector，IMP，...）都已经按格式成功加载到内存中，被运动所所管理
- 
`main`
1.APP的启动由dyld的主导，将可执行文件加载到内存，顺便加载所有依赖的动态库
2.并由运行时负责加载成objc定义的结构
3.所有初始化工作结束后，使dyld就会调用主函数
4.接下来就是UIApplicationMain函数，AppDelegate中的应用中：didFinishLaunchingWithOptions：方法
![](https://upload-images.jianshu.io/upload_images/3067538-1662f47d3e728ec5?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
图片
优化方案
一，使dyld
- 减少动态库，合并一些动态库（定期清理不必要的动态库）
- 减少Objc类，分类的数量，减少选择器数量（定期清理不必要的类，分类）
- 减少C ++虚函数数量
- 斯威夫特尽量使用结构
二，运行时间
用+初始化方法和dispatch_once取代所有的**属性**（（构造函数）），C ++静态构造器，ObjC的+加载
三，主
- 在不影响用户体验的前提下，尽可能将一些操作延迟，不要全部都放在finishLaunching方法中
- 按需加载
后面会分享自己的优化过程
> - 请关注：[https](https://github.com/sallenhandong)：[//github.com/sallenhandong](https://github.com/sallenhandong)
- 资料来源：slimsallen.com/#/detail/iosperformance.md
作者：slimsallen 
链接：HTTPS：//www.jianshu.com/p/46da426b130c 
来源：书繁简
繁简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
