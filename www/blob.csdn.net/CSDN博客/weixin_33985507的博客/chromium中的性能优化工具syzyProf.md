# chromium中的性能优化工具syzyProf - weixin_33985507的博客 - CSDN博客
2018年10月26日 11:46:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
### 函数性能分析工具SyzyProf
我先开始介绍SyzyProf。这个工具可以捕获每个线程调用每个函数执行的时间，然后把结果生成一个KCacheGrind能够识别的数据格式文件，然后通过KCacheGrind的展示结果。你就可以知道函数哪个函数执行了次数最多，消耗的时间最多，哪个线程在读写文件，哪个线程在创建窗口界面，而且KCacheGrind以图形的方式显示出函数调用链等信息，非常直观。如图这是我生成CEF自带demo程序的函数调用信息。
![20141215225109](http://blog1.gclxry.com/wp-content/uploads/2014/12/20141215225109-1.png)
其实能够函数性能分析工具已经够多了，比如xperf,AQTime,还有visual studio自带的性能分析功能。但是这些工具都是重量级的，有的需要调试的模式运行目标程序，导致对目标程序运行效率有很大的影响。
相比而言SyzyProf是轻量级的函数性能分析工具，配置简单，对目标程序单运行效率影响小。说起来这么先进，其实SyzyProf的工作原理很简单，就是给编译器加上/profile的开关，然后编译出的pe文件有一些额外的空间，SyzyProf在这些额外的空间里插入指令，修改编译后的pe文件，hook住每个函数的调用，然后记录下函数的执行信息。/profile是vs编译器为性能优化预留的操作空间，vs本身性能优化也用到了这个接口。
### 不可思议的山寨机(syzygy)
如果chromium的性能优化仅限于SyzyProf那就太普通了，它还有一个杀手锏——syzygy。syzygy利用SyzyProf生成的函数性能分析数据，重新优化函数在pe二进制文件中的分布位置，比如调用的早的函数放到pe文件的前头，函数之间调用频繁紧密的函数放到一起，减少程序运行时的缺页中断错误，从而减少程序的冷启动时间和内存占用。因为syzygy这个词的发音太拗口，我一般称它为山寨机。
听起来有些不可思议，但确实是这样的，这也是为什么chromium浏览器启动的这么快的原因。chromium自己宣称syzygy能够减少了80%的冷启动时间，还有40%的内存占用。
事实上我们用visual studio的编译器生成的pe文件函数的分布相对它在执行时的顺序而言几乎是随机的，这就导致了程序在执行的时候有一些非必要的缺页中断，导致冷启动时间增加。还有不同分页中代码和数据性能很差的访问，导致内存工作集变大。这些正是syzygy优化的地方。
微软自己早就对windows系统某些模块才用了同样的技术对pe文件的函数重分布，比如 ntoskrnl.exe，根本不是编译器默认生成的样子。微软很早就开始了这方面的研究，[Binary Technologies Projects](http://www.microsoft.com/windows/cse/bit_projects.mspx)的[Binary Transformation Infrastructure](http://research.microsoft.com/pubs/69850/tr-2001-50.pdf)。这种性能优化的思路并非chromium首创，但发挥的淋漓尽致的确是chromium。
syzygy做优化的需要改变pe文件，这将导致pe文件跟原来的pdb文件对应不上，不能再使用pdb文件来调试了，为此syzygy改变pe文件的同时也会修改对应的pdb文件。
总结syzygy所用到的技术：
- 使用编译器提供的/profile接口，在生成的pe文件中插入指令hook住函数。
- 通过ETW(Event Tracing for Windows)机制获得目标程序的函数性能数据。
- 根据函数性能数据生成了重排布策略，调用编译器接口去重排布pe文件，调用的DIA(Debug Interface Access)接口去修改pdb文件。
看起来也并不复杂，但是背后分解pe文件，还有编译器生成某些符号不能重排布，底下有很多有脏话累活，这都要试错躺雷才能明白。
