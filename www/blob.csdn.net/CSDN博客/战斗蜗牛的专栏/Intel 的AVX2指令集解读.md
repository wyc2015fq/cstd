# Intel 的AVX2指令集解读 - 战斗蜗牛的专栏 - CSDN博客





2014年08月06日 22:48:01[vbskj](https://me.csdn.net/vbskj)阅读数：30351
个人分类：[SSE并行](https://blog.csdn.net/vbskj/article/category/2169499)









在Intel Sandy Bridge微架构中，Intel引入了256位SIMD扩展AVX，这套指令集在兼容原MMX、SSE、SSE2对128位整点SIMD支持的基础上，把支持的总向量数据宽度扩展成了256位。新增了若干条256位浮点SIMD指令。

昨天，Intel刚刚发布了AVX2指令集，这套指令集在AVX基础上做了扩展，不过要在2013年发布的Haswell处理器上才能支持。参考1给出了AVX2的详细特性。




## AVX2指令集概述

相比AVX，AVX2在如下方面做了扩展。
- 支持的整点SIMD数据宽度从128位扩展到256位。Sandy Bridge虽然已经将支持的SIMD数据宽度增加到了256位，但仅仅增加了对256位的浮点SIMD支持，整点SIMD数据的宽度还停留在128位上，
- 增强广播、置换指令支持的数据元素类型、移位操作对各个数据元素可变移位数的支持、跨距访存支持。

## 跨距访存支持

跨距访存支持即访存时，每个SIMD数据的向量数据元素可以来自不相邻的内存地址。AVX2的跨距访存指令称为”gather”指令，该指令的操作数是一个基地址加一个向量寄存器，向量寄存器中存放着SIMD数据中各个元素相对基地址的偏移量是多少。有了这条指令，CPU可以轻松用一条指令实现若干不连续数据”聚集”到一个SIMD寄存器中。这会对编译器和虚拟机充分利用向量指令带来很大便利，尤其是自动向量化。另外，参考2中对跨距访存指令的功能描述中可以看到，当该指令的偏移地址向量寄存器中任何两个值相同时，都会出GP错。这意味着编译器还是需要些特殊处理才能利用好这条指令。
![跨距访存指令](http://www.lingcc.com/wp-content/uploads/2011/06/img3-300x183.png)跨距访存指令

但跨距访存指令仅仅支持32位整点、64位整点、单精度浮点、双精度浮点的跨距访存操作。从参考4可以猜测其实gather指令只是在硬件上分解成若干条32位或64位的微访存指令实现。这就移位着其实一条32×8的SIMD访存其实就是8次32位普通数据访存，其访存延时和延时不确定性会非常大，聊剩于无。

## 拓宽原有整点SIMD指令

理论上从128位到256位的成倍SIMD宽度扩展能带来一倍的加速。


![](http://www.lingcc.com/wp-content/uploads/2011/06/img1-300x181.png)从128位扩展到256位的整点SIMD指令

## 位操作指令支持

这些指令在加速数据库压缩、哈希，大数的算术计算方面会有帮助。


![](http://www.lingcc.com/wp-content/uploads/2011/06/img2-300x166.png)新增的位访存操作指令

## 任意位置的SIMD数据置换支持

这一支持将使编译器可以更灵活的使用这条指令协助自动向量化。像参考5这类工作就能实施在Intel的芯片中。


![](http://www.lingcc.com/wp-content/uploads/2011/06/img4-300x134.png)任意位置数据置换指令支持

## 向量-向量移位支持

之前Intel上的所有SIMD扩展指令的移位操作仅支持所有SIMD数据同时移位相同的位数，有了向量-向量移位支持，就可以为每个SIMD数据做不同的移位操作。


![](http://www.lingcc.com/wp-content/uploads/2011/06/img5-300x151.png)向量-向量移位操作支持

## 浮点乘法累积操作

之前的X86处理器上的累积操作多数针对整点数据，这次针对浮点数据增加的60条SIMD操作会给Intel跑浮点Benchmark，比如linpack之类的带来很多加速。

## 《[编译点滴](http://www.lingcc.com/)》评论

之前几乎所有在通用微处理器上的SIMD指令，都倾向于一刀切策略，即所有的SIMD操作都针对SIMD数据实施完全相同的操作，并不存在特性化的指令。比如访存都是从一块连续的地址空间直接访存、移位时所有的数据都移动相同的位数，数据置换指令的支持也是最近才出现在通用CPU中的。AVX2的这些支持再次肯定了个性化的趋势，CPU中的SIMD支持朝着GPU的方向大踏步前进，并最终赶上并超越向量机。不得不称赞一下IBM的超前思想。AVX2中的这些特性支持在几年前的Power处理器中就已经出现了。

虽然AVX号称可以扩展到512位甚至1024位的SIMD支持，但是所带来的问题怎么解决，期待AVX3或者AMD的小宇宙爆发，或者威盛的黑马：
- 理论上，增加1倍的向量数据宽度，将带来2倍的晶体管数量提升。目前，Intel的AVX指令集只实现在片上每个core里，作为core中的一个功能部件，若扩展到1024位，将增加4倍的晶体管。虽然制造工艺也会改进，但功耗还是会很大，怎么解决？
- 在自动向量化仍然不好用的前提下，普通程序很难利用到这些功能做加速，白白的浪费这些晶体管吗？
- 这么大的数据计算能力，访存怎么供数？对齐貌似还是没有好的方法。还是要程序员自己吭哧吭哧写向量化代码，调试吗？芯片厂商可以每几年升级一次宽度，但兼容性如何保证，原来有64位、128位、256位分别对应MMX、XMM、YMM寄存器，以后呢？


## 参考
- [http://software.intel.com/en-us/blogs/2011/06/13/haswell-new-instruction-descriptions-now-available/](http://software.intel.com/en-us/blogs/2011/06/13/haswell-new-instruction-descriptions-now-available/)
- [http://software.intel.com/file/36945](http://software.intel.com/file/36945)
- [http://en.wikipedia.org/wiki/Haswell_%28microarchitecture%29](http://en.wikipedia.org/wiki/Haswell_%28microarchitecture%29)
- [http://software.intel.com/en-us/forums/showthread.php?t=83459&o=a&s=lr](http://software.intel.com/en-us/forums/showthread.php?t=83459&o=a&s=lr)
- [http://portal.acm.org/citation.cfm?id=1133996](http://portal.acm.org/citation.cfm?id=1133996)



