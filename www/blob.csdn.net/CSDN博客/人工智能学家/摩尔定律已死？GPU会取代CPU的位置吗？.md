# 摩尔定律已死？GPU会取代CPU的位置吗？ - 人工智能学家 - CSDN博客
2017年10月22日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：224
![640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBWXbC9dyTsvLfqiakWFAxpWzoia3w9t8xTGAg7qD88oJBCuU8p4iaz5GCQX7ib3PHgqPjCcvcw5sIBgIA/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)
*来源：全球人工智能*
*概要：**CPU和GPU之所以大不相同，是由于其设计目标的不同，它们分别针对了两种不同的应用场景。*
京举办的NVIDIA GTC China会议中，无论是AI智能运算，还是服务器数据中心、智能城市，甚至还有去年很火热但是已经很多人已经支撑不下去的虚拟现实，看起来在很多内心中依然是属于图形行业代表的NVIDIA已经变得越来越丰满，不过在这些新闻的背后，似乎还有更大胆的预言：摩尔定律已死，GPU最终会取代CPU。
摩尔定律是由英特尔（Intel）创始人之一戈登·摩尔（Gordon Moore）提出来的。其内容为：当价格不变时，集成电路上可容纳的元器件的数目，约每隔18-24个月便会增加一倍，性能也将提升一倍。换言之，每一美元所能买到的电脑性能，将每隔18-24个月翻一倍以上。这一定律揭示了信息技术进步的速度。
尽管这种趋势已经持续了超过半个世纪，摩尔定律仍应该被认为是观测或推测，而不是一个物理或自然法。预计定律将持续到至少2015年或2020年 。然而，2010年国际半导体技术发展路线图的更新增长已经放缓在2013年年底，之后的时间里晶体管数量密度预计只会每三年翻一番。
CPU和GPU之所以大不相同，是由于其设计目标的不同，它们分别针对了两种不同的应用场景。CPU需要很强的通用性来处理各种不同的数据类型，同时又要逻辑判断又会引入大量的分支跳转和中断的处理。这些都使得CPU的内部结构异常复杂。而GPU面对的则是类型高度统一的、相互无依赖的大规模数据和不需要被打断的纯净的计算环境。于是CPU和GPU就呈现出非常不同的架构。
GPU采用了数量众多的计算单元和超长的流水线，但只有非常简单的控制逻辑并省去了Cache。而CPU不仅被Cache占据了大量空间，而且还有有复杂的控制逻辑和诸多优化电路，相比之下计算能力只是CPU很小的一部分 
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/X8OUs1wOMWHEccicw7Wxv0nNOMbAsQTLhADTZYCibFqMicjx85JjJkPf9HictFWzcYykeLB851Reckqpw0FMzUMhSQ/640?wx_fmt=png)
从上图可以看出：Cache, local memory： CPU > GPU Threads(线程数): GPU > CPURegisters: GPU > CPU 多寄存器可以支持非常多的Thread,thread需要用到register,thread数目大，register也必须得跟着很大才行。SIMD Unit(单指令多数据流,以同步方式，在同一时间内执行同一条指令):
 GPU > CPU。 CPU 基于低延时的设计：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/X8OUs1wOMWHEccicw7Wxv0nNOMbAsQTLhTwXz3MichbjWVj9mKu3tdEh9OiceghZXT3fbAJGibJ93b6FlDP7V7LicUA/640?wx_fmt=png)
CPU有强大的ALU（算术运算单元）,它可以在很少的时钟周期内完成算术计算。当今的CPU可以达到64bit 双精度。执行双精度浮点源算的加法和乘法只需要1～3个时钟周期。CPU的时钟周期的频率是非常高的，达到1.532～3gigahertz(千兆HZ, 10的9次方).大的缓存也可以降低延时。保存很多的数据放在缓存里面，当需要访问的这些数据，只要在之前访问过的，如今直接在缓存里面取即可。复杂的逻辑控制单元。当程序含有多个分支的时候，它通过提供分支预测的能力来降低延时。数据转发。
 当一些指令依赖前面的指令结果时，数据转发的逻辑控制单元决定这些指令在pipeline中的位置并且尽可能快的转发一个指令的结果给后续的指令。这些动作需要很多的对比电路单元和转发电路单元。 
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/X8OUs1wOMWHEccicw7Wxv0nNOMbAsQTLhEWJh4ia2dK1WCOXlKHvbsbjLyqqtwt78zQhI9szdjYmY1Lu05zXIAibw/640?wx_fmt=png)
GPU是基于大的吞吐量设计。GPU的特点是有很多的ALU和很少的cache. 缓存的目的不是保存后面需要访问的数据的，这点和CPU不同，而是为thread提高服务的。如果有很多线程需要访问同一个相同的数据，缓存会合并这些访问，然后再去访问dram（因为需要访问的数据保存在dram中而不是cache里面），获取数据后cache会转发这个数据给对应的线程，这个时候是数据转发的角色。但是由于需要访问dram，自然会带来延时的问题。GPU的控制单元（左边黄色区域块）可以把多个的访问合并成少的访问。
GPU的虽然有dram延时，却有非常多的ALU和非常多的thread. 为啦平衡内存延时的问题，我们可以中充分利用多的ALU的特性达到一个非常大的吞吐量的效果。尽可能多的分配多的Threads.通常来看GPU ALU会有非常重的pipeline就是因为这样。所以与CPU擅长逻辑控制，串行的运算。和通用类型数据运算不同，GPU擅长的是大规模并发计算，这也正是密码破解等所需要的。所以GPU除了图像处理，也越来越多的参与到计算当中来。
**什么类型的程序适合在GPU上运行？　**
（1）**计算密集型的程序**。所谓计算密集型(Compute-intensive)的程序，就是其大部分运行时间花在了寄存器运算上，寄存器的速度和处理器的速度相当，从寄存器读写数据几乎没有延时。可以做一下对比，读内存的延迟大概是几百个时钟周期；读硬盘的速度就不说了，即便是SSD,
 也实在是太慢了。　
（2）**易于并行的程序**。GPU其实是一种SIMD(Single Instruction Multiple Data)架构， 他有成百上千个核，每一个核在同一时间最好能做同样的事情。
**讨论**：您是否了解GPU呢？您认为CPU会被GPU取代吗？您了解CPU现在最大的瓶颈吗？
