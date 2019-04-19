# 被FMOD的内存管理坑了一把 - 逍遥剑客 - CSDN博客
2012年12月15日 19:00:56[xoyojank](https://me.csdn.net/xoyojank)阅读数：5204
最近遇到个内存泄露, 查来查去竟然在FMOD模块里
要不是别的组件全部定制了内存分配函数, 还真不会怀疑到它头上
使用FMOD::EventSystem::getMemoryInfo()输出内存信息一看, 直接崩溃了, 100MB~200MB
之前我是用XACT的, FMOD本以为是差不多的东西, 就没怎么在意, 结果摔一跟头
细细的去读了一下文档, 发现有3点:
- FMOD::Memory_Initialize
- 可以把FMOD的内存分配器换成自己的, 方便统计各模块的内在使用情况
- 这个函数需要链接fmodex的lib, 只有fmod event的lib不行的
- 这步可选, 但是我们自己的内存分析工具对于自己内存模块分配的比较好分析, 所以还是定制了一下
- FMOD::Event
- 播放声音会分配一些内存, 如果你不进行回收的话, FMOD是不会释放的, 除非你把整个工程(.fev)unload掉
- 一般关卡式的游戏才会使用切换工程的方式管理, 对于MMOG不是很适用, 所以还是需要定期使用FMOD::EventGroup::freeEventData()进行内存回收
- 原有的代码实在是乱, 受不了重写了个. 改成根据引用计数和播放状态进行回收
- SoundBank- 这个跟XACT差不多, 一般都是音效直接加载到内存(Memory), 背景音乐使用流式加载(Stream)
- 为了节省磁盘和内存占用, 一般会进行编码压缩, 音效我习惯使用ADPCM, 音乐使用MP3/xWMA, CPU解码(一般能硬件直接支持)消耗可以忽略
- 坑就在FMOD多了一种载入方式: "Decompress into memory". 把压缩过的音频数据解压到内存再播放. 要知道一首背景音乐解压完有几十MB...
- 检查发现我们的SoundBank竟然都设置成了"Decompress into memory", 汗死. 音效工程还是由程序建好再让制作人员填数据比较好-_-!
- 跟音效师做了个约定: 音效使用"Load into memory" + "ADPCM", 音乐使用"Stream from disk"+ "MP3"
经过以上改动, 正常游戏FMOD占用的内存一般在3MB左右, 激烈战斗高峰也不会超过10MB, 再说过一会儿就会被回收回去了
