# Neubla3中的骨骼动画: CoreAnimation子系统 - 逍遥剑客 - CSDN博客
2009年04月10日 23:50:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2271
所属专栏：[Nebula引擎研究](https://blog.csdn.net/column/details/thenebuladevice.html)
最早的一个设计是把动作代码分成两个子系统. CoreAnimation作为低层系统, 只为更复杂的高层动作系统提供简单而又高性能的”砖瓦”. 高层Animation子系统以CoreAnimation为基础, 提供了像抽像动作名和实际剪辑(clip)名的映射, 还有一个动作序列发生器(sequencer), 可以方便地处理复杂的动作融合情形. 
CoreAnimation主要关注于高性能的基础操作, 如采样和动作数据的混合. CoreAnimation可能会包含特定平台的优化(尽管现在哪个都没做, 但是它是基于Nebula3数学库的). CoreAnimation还假设动作捕捉是动作数据的主要来源, 就像采样的音频MIDI. 动作捕捉的数据由大量均匀分布的关键帧组成，而不是在时间轴上任意分布．使用这种数据的优势是可以简化成简单的流操作，这比较适合SSE, GPU或者Cell SPU. 相对于基于曲线的动作系统, 缺点是显而易见的: 数据量更大. 
曲线动画会在Nebula3的其它部分得到应用, 而且对它的支持应该放到数学库中, 而不是动作自身这个子系统里. 
尽管没有限制, CoreAnimation假设蒙皮的角色为主要的动作对象. 这不会影响其它类型物体的使用, 但是全面的设计和优化会比较”偏爱”角色类型的动作数据(也就是许多剪辑, 每个剪辑许多动作曲线, 还有几百到几千的动作帧). 
另外一个设计局限是, 新的动作系统需要兼容现有的数据. 动作导出代码写了在Maya的插件中, 并且批处理时进行进一步的优化和打包并不是没有意义的. 虽然工具这边的许多代码可以通过清理来改善(就像通常开发环境中的代码), 可我并不想重写这些东西, 特别是工具这边的工作比运行时那边多很多. 
不再废话, 给出CoreAnimation的类J: 
l**AnimResouce: **这个类包括了一个目标对象的所有动作数据(例如, 一个角色的所有动作数据), 也就是AnimClip对象的数组, 还有一个AnimKeyBuffer. AnimResource也是Nebula3的资源对象, 所以可以通过ResouceId进行共享, 也可以进行异步装载. 
l**StreamAnimationLoader: **这是一个标准的流加载器的子类, 通过读取流数据来初始化一个AnimResource对象. 现在它只支持Nebula2的.nax文件(xoyojank: 08-Nov-2008). 
l**AnimKeyBuffer: **这是一个AnimResource的所有动作帧存在的地方. 这仅仅是一块float4帧的内存区域, 帧中没有任何关于与其相关的动作剪辑和曲线信息. 动作导出工具负责保证这些帧以一种缓存友好的方式组织起来(帧在内存中交错存储, 这样一个简单的采样操作所需要的帧在内存中都是彼此靠近的)
l**AnimClip: **一个AnimClip通过一个常用的名字(如”walk”, “run”, “idle”, 等等)把AnimCurve的集合组织到一起. 在使用动作子系统时剪辑名通常就是需要关心的最底层部分了. 剪辑有很多属性和限制: 
n一个人类可读的名字, 以StringAtom的方式保存和传递, 所以没有字符串的拷贝发生
n一个剪辑包含一些AnimCurve(如一个典型的角色动作剪辑每个骨骼接合处有3个曲线, 移动, 旋转和缩放各一个)
n一个剪辑中的所有AnimCurve必须相同的帧周期和帧数
n剪辑有一个周期(帧周期*帧数)
npre-infinity-type和post-infinity-type定义了在采样时间超出剪辑时间范围后的采样方式(夹取还是循环)
l**AnimCurve: **AnimCurve组织了所有的帧, 它们描述一段时间内一个4D值的变化. 例如, 一个剪辑中的角色骨骼连接点的动画位移是由这个剪辑中的动作曲线(animation curve)来描述的. AnimCurve实际上并不包含动作帧, 而是描述了这些帧在父级AnimResource的AnimKeyBuffer中的什么位置. AnimCurve有以下几个属性: 
n**Active/Inactive: **非活动的AnimCurve是不会对最终结果有所贡献的, 例如, 一个AnimClip只影响一个角色骨骼的一部分(如上半身), 那么一些曲线就设置为非活动的. 非活动的曲线在帧缓冲(key buffer)是没有任何帧的. 
n**Static/Dynamic: **一个不随时间变化的AnimCurve被导出工具标记为静态的, 而且不占用帧缓冲的空间. 
n**CurveType: **这是一个对高层动画代码的关于这个动作曲线中包含的是什么类型数据的一个暗示, 例如, AnimCurve描述了一个旋转, 那么这帧就必须做为四元数来解析, 采样和混合也要使用特殊的操作. 
l**Animation****帧(key): **在CoreAnimation系统中并没有”AnimKey”这个类, 取而代之的是, 帧的原子数据类型是float4, 它在动作系统的高层可以被解析成点, 向量, 四元数或者颜色. 这里并没有提供标量的支持, 因为大多数3D引擎的动作数据都是向量数据, 并且硬件处理向量时更喜欢它的数据大小是128位的. 
l**AnimEvent: **动作事件在”播放光标”经过它时触发. 相同的东西在Nebula2中叫作”HotSpots”. AnimEvent还现在没有实现, 不过它是很多东西与动作进行同步必不可少的东西(例如, 一个走路动作应该在脚底着地时触发事件, 这样脚步声和灰尘粒子才可以在正确的时间和位置创建, 并且结束事件对于同步开始播放一个新的动作也非常有用, 比如在左脚着地后开始一个”左转”动作剪辑等等. 
l**AnimSampler: **这个类只有一个静态方法叫Sample(). 它在一个特定的采样时间点从AnimClip里采样数据到目标AnimSamplerBuffer. 这是CoreAnimation系统提供的两个”front-end-features”(采样和混合)之一. AnimSampler被高层的Animation系统所使用. 
l**AnimMixer: **像AnimSampler类一样, AnimMixer类只提供了一个简单的静态方法叫Mix(). 这个方法接收两个AnimSampleBuffer跟一个插值参数(通常在0到1之间), 把它们混合后输出到另一个缓冲(k = k0 + l * (k1 – k0)). AnimMixer被高一级的Animation子系统用于做优先级的动作剪辑混合. 
l**AnimSampleBuffer: **AnimSampleBuffer中保存了AnimSampler::Sample()的采样结果, 做为AnimMixer::Mix()的输入和输出. 它跟AnimKeyBuffer相比有一个重要的不同: AnimSampleBuffer有一个额外的”SampleCounts”数组. 这个数组记录了每次采样和混合动作剪辑到最终结果时的采样操作数目. 这对于混合局部剪辑(只影响角色骨骼一部分的剪辑)是很有必要的. AnimSampler::Sample()方法会为每个从活动动作曲线中采样出来的帧设置采样数目为1, 非活动的则设为0(代表活动采样值为无效的). 之后当AnimMixer:Mix()混合两个采样缓冲(sample buffer)时会检查输入的采样数目, 只有在两个采样都有效时才进行混合操作. 如果一方值无效则不会做任何混合, 另一方(有效的)会直接写入结果. 如果双方都无效, 输出的采样也是无效的. 因此, 如果在一次完整的采样和混合操作的的最后, 某个采样的采样数目为0, 这意味没有一个动作剪辑为它做出了贡献(这可能出现BUG了-_-). 
这就是CoreAnimation子系统了, 接下来会是架构在CoreAnimation之上的Animation子系统. 之后会再介绍一个新的Character子系统, 而它是架构在Animation系统之上的. 
