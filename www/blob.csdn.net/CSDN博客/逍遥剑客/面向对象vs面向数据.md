# 面向对象vs面向数据 - 逍遥剑客 - CSDN博客
2013年03月30日 14:11:40[xoyojank](https://me.csdn.net/xoyojank)阅读数：5905
最近看了[OGRE2.0的一个PPT](http://www.mediafire.com/view/?3xkuka8jjoaoohc), 触动挺大的
其实OGRE一直以来所为人诟病的性能问题, 何偿不也是我们引擎存在的问题
虽然很多时候我们都拿OGRE和GameBryo的效率当反面教材, 但是自己也没有做到极致
相对于GPU的性能优化来说, CPU的性能优化难得多
就好比游戏开发的书籍, 讲API/渲染的多, 讲架构&逻辑的少
可能很多人以为, 做引擎开发就是做图形开发, 对于国内的游戏来说好像没错
但是如果真正做下来, 资源管理, 场景管理, 动画, 物理, AI, UI, 音效, 脚本, 甚至技能系统等都是非常有深度的
这也就是导致了, 很多自研引擎能够把GPU的性能优化得很好, 玩起来却仍然不流畅
究其原因, 大多数是对数据的处理存在瓶颈
最早让人觉得"毁三观"的是[Battlefield3的一个PPT](http://dice.se/publications/culling-the-battlefield-data-oriented-design-in-practice/), 打破了传统基于树/图的场景管理模式
15000+的物体, 并行Bruce force的一个线性数组做Culling, 比基于树形结构的管理方式快了3倍, 代码量只有1/5
为什么呢? 是因为当前的硬件架构决定了, 大多数的瓶颈是在数据访问上面
CPU与内存之间有速度非常快的Cache, 如果数据可以在Cache中直接找到, 会比从内存中Load过来快很多
具体快多少呢? 我只能说不是一个数量级的
之所以说是"毁三观", 是因为这跟学校教的完全不一样
大学里的"数据结构", "算法导论", 拿到当前领域&当前硬件体系结构上, 无疑是有一定误导性的
各种所谓的O(n)算法, 都是"brach heavy"的, 会引起非常多的"cache miss"
举个例子, 在某些情况下, 二分查找还不如线性遍历来得快
再加上各种面向对象理论的洗脑, 完全就走上了一条邪路
当然, Battlefield3里还利用了parallel(并行)的思想, 并不仅仅是"cache frieldly"就可以把效率提上去的
说了这么多, 其实就是想阐明一句话(OGRE2.0 PPT里的):
SIMD, parallel, cache-friendly algorithms are the industry standard today!
当VTune的热点函数看不出什么来, 当GPA的GPU柱状图都很平均, 但是性能仍然不够好, 是不是有些抓狂?
想想上面这句话, 也就有了优化的方向!
- SIMD, Cache friendly
- - 其实大多数做引擎的人都有考虑, 好多人都会说"SSE我很熟"
- 我去, 看看他们写的代码, 连数据结构内存都没对齐, 还好意思说"SSE我很熟"......
- 另外, 尽量把相同类型的数据存放在连续的内存空间里, 并且进行顺序访问
- 如果有需要, 甚至可以使用prefetch指令把数据加载到Cache中去
- 吐槽一句: 喜欢用if-else的程序员都不是好程序员
- SOA vs AOS
- - 很多时候SOA(struct of array)比AOS(array of struct)是快的, 因为多数情况下我们遍历一个结构体数组, 只是访问其中的一个字段而已
- SOA与AOS的区别, 就是面向对象与面向数据程序设计的区别之一
- class vs struct
- - 这才是面向对象与面向数据在语言层面的差异
- 最早认识到class性能会出问题的, 是从N3的代码里. floh有说为什么自己的引擎平台抽象层没有使用抽象类, 是因为虚函数对于主机平面的硬件架构性能很差. 本质上来说, 还是cache miss的问题
- 刚入行时看到有人说, 一开始写程序用struct, 后来用class, 最后还是用struct. 想想这也是从入门->改善设计->改善性能的一个过程吧
- parallel
- - 现在CPU核越来越多, 甚至手机都4核8核了, 我们游戏已经把双核定义为入门配置了
- 打开任务管理器, 看看CPU占用率, 除一个核跑满, 其它都是闲置的-_-
- 所以, 从引擎架构上, 性能优化必需要做的一步就是并行化
- 一般来说, 可以有两种方式
- - 模块划分: IO一个线程, 渲染一个线程, 物理一个线程, 逻辑一个线程等
- 任务划分: 动画计算, 场景剔除, AI寻路计算, 粒子计算等可以拆分成一个个的小任务, 扔到任务系统(本质上是一个线程池, PS3可以是SPU)里进行计算
- 很多顿卡问题其实就是某些API调用时间过长引起的, 可以放入后台线程调用. 如磁盘IO, Shader编译, DirectX API调用等
- memory, bandwidth
- - 另一个优化方向, 其实就是尽量减少内存占用
- 一是从数量上减少, 这样数据处理次数就减少了
- 一是从单位占用上, 可以提高内存访问效率
- 再就是内存对齐了, 参考SIMD
- 带宽的考虑, 更多的是GPU端. 如Vertex, Texture的缩小, FrameBuffer的UpSampling, PixelFormat的压缩, UV的运算(影响Cache), Overdraw的减少(Fillrate优化), Shader branch的减少(影响Cache)
- 从ForwardShading到DeferredShading是算法复杂度上的考虑, 从DeferredShading到DeferredLighting就是带宽和灵活性上的考虑了. 现在又出来个TileBasedRendering, 都是因为硬件的变化带来算法/架构上的变化
总得来说, 硬件在不断升级换代, 我们头脑也需要升级换代才能跟得上潮流
面向对象虽然加快了开发效率,但是并不是对机器友好的. 在性能至上的领域, 不是很适用
说到底, 还是人与机器的博弈
