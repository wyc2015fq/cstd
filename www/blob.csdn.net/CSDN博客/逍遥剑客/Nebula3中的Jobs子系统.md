# Nebula3中的Jobs子系统 - 逍遥剑客 - CSDN博客
2010年05月09日 21:31:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2112
所属专栏：[Nebula引擎研究](https://blog.csdn.net/column/details/thenebuladevice.html)
Jobs子系统是Nov2009这版本新加入的. 最初的背景可能是官方在把引擎移植到PS3上时, 为了充分利用SPU而做的优化. 参考[KILLZONE2](http://blog.csdn.net/xoyojank/archive/2009/09/23/4586322.aspx), SPU上可以计算的部分有:
·动画
·危险预测 AI 
·弹道回避 AI 
·障碍物规避 AI 
·冲撞判定
·物理演算
·粒子计算
·粒子渲染
·场景图（ Scene graph ）
·生成绘图列表
·光源基准图像渲染扫描计算 (IBL Probe) 
·图像后处理
·动态音乐播放系统的控制
·关节平滑处理
·MP3 解压
·Playstation Edge 内的解压处理 (Zlib) 
其中红色的是N3中已经采用的Job运算. 当然, 并不是所有的任务都并行化了才好, 对于Win32平台, GPU不是最大瓶颈, 而CPU的核心数目前并没有那么多, 所以骨骼/粒子/场景部分并行化是一个不错的选择. 当然, AI这种上层逻辑也是有其必要性的. 物理部分, 一般是采用物理引擎了, 至于内部怎么实现是它们的事.
Jobs系统结构一览:
![](http://hi.csdn.net/attachment/201005/9/0_12734118878ZA2.gif)
由于公开的SDK只包含Win32平台, 所以PS3不做讨论.
Win32平台上, Jobs系统是基于ThreadPool(线程池)实现的. 先来明确一下各个类的功能:
l**Job**: 可以称之为”任务”, 进行异步工作的基本单位. 它由以下部分组成:
n输入数据缓冲
n输出数据缓冲
n统一(uniform)数据缓冲
n任务函数
任务函数对输入数据缓冲进行并行处理, 把产生的结果写入输出数据缓冲. 输入数据缓冲会被分割到一些Slice(片)中, 而Slice则会并行地处理. Uniform数据缓冲对于各个Slice而言是唯一的, 通常是用于给任务函数传输一些额外的参数.
l**JobPort**: 同MessagePort一样, JobPort是一个管道的”端口”, 接收要执行的Job对象, 并处理有依赖关系的Job之间的同步和等待. 对于普通的Job, 它只是简单地拆成JobSlice扔给ThreadPool, 而对于有依赖关系的JobChain, 它会保证每个Job的第一个Slice在同一个工作线程(WorkerThread)中, 这样可以改善负荷. 同时, 为了保证后面的Job在前一个Job完成时才执行, JobPort会把前面Job的完成事件(CompletionEvent)告诉下一个Job, 让其等待.
l**JobSystem**: 这只是一个外观类, 真正的实现是JobTheadPool
l**JobThreadPool**: 这里面包含几个工作线程(目前是4个), 每次增加Job时, 它会把Job的各个Slice分散到不同的工作线程中去, 让它们尽可能地并行化执行
l**WorkerThread**: 这是从Thread类派生的, 就是一个实际的线程. 它的基本执行单位是JobCommand. JobCommand有两种类型:
n**Run**: 执行JobSlice
n**Sync**: 等待完成事件
WorkerThread里只有一个JobCommand的队列, 按次序执行. 如果是Run类型, 则执行;如是是Sync类型, 则等待. 这就是JobChain互相依赖的关键!
在CPU频率止步不前, 核心数不断增加的今天, 并行编程已经成为一种趋势. 就是连主机这样的平台, 也免不了多个”SPU”. N3中的”Fat Thread”和”Job System”, 可以说是其两大精髓, 给大家做多线程化引擎编程提供了一个不错的参考. 
