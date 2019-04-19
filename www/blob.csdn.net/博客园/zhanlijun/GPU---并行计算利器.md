# GPU---并行计算利器 - zhanlijun - 博客园
# [GPU---并行计算利器](https://www.cnblogs.com/LBSer/p/4592862.html)
# 转载请引用：[GPU---并行计算利器](http://www.cnblogs.com/LBSer/p/4592862.html)
# [](http://www.cnblogs.com/LBSer/p/4592862.html)源于阿里巴巴CCO《猿来如此》分享
# 1 GPU是什么
      如图1所示，这台PC机与普通PC机不同的是这里插了7张显卡，左下角是显卡，在中间的就是GPU芯片。显卡的处理器称为图形处理器（GPU），它是显卡的“心脏”，与CPU类似，只不过GPU是专为执行复杂的数学和几何计算而设计的。
      GPU计算能力非常强悍，举个例子：现在主流的i7处理器的浮点计算能力是主流的英伟达GPU处理器浮点计算能力的1/12。
![](https://images0.cnblogs.com/blog2015/522490/201506/220745132833777.png)
图1 显卡与GPU
# 2 为什么GPU计算能力如此强悍？
       图2对CPU与GPU中的逻辑架构进行了对比。其中Control是控制器、ALU算术逻辑单元、Cache是cpu内部缓存、DRAM就是内存。可以看到GPU设计者将更多的晶体管用作执行单元，而不是像CPU那样用作复杂的控制单元和缓存。从实际来看，CPU芯片空间的5%是ALU，而GPU空间的40%是ALU。这也是导致GPU计算能力超强的原因。
![](https://images0.cnblogs.com/blog2015/522490/201506/220728431426269.png)
图2 cpu和gpu硬件逻辑结构对比
      那有人讲了，为什么cpu不像gpu那样设计呢，这样计算能力也强悍了！
      为什么？CPU要做得很通用。CPU需要同时很好的支持并行和串行操作，需要很强的通用性来处理各种不同的数据类型，同时又要支持复杂通用的逻辑判断，这样会引入大量的分支跳转和中断的处理。这些都使得CPU的内部结构异常复杂，计算单元的比重被降低了。而GPU面对的则是类型高度统一的、相互无依赖的大规模数据和不需要被打断的纯净的计算环境。因此GPU的芯片比CPU芯片简单很多。
      举个例子，假设有一堆相同的加减乘除计算任务需要处理，那把这个任务交给一堆（几十个）小学生就可以了，这里小学生类似于GPU的计算单元，而对一些复杂的逻辑推理等问题，比如公式推导、科技文章写作等高度逻辑化的任务，交给小学生显然不合适，这时大学教授更适合，这里的大学教授就是CPU的计算单元了，大学教授当然能处理加减乘除的问题，单个教授计算加减乘除比单个小学生计算速度更快，但是成本显然高很多。
# 3 GPU编程库
       GPU计算能力这么强，被广泛使用！比如挖矿（比特币）、图形图像处理、数值模拟、机器学习算法训练等等，那我们怎么发挥GPU超强的计算能力呢？---编程！
       怎么进行GPU编程呢？现在GPU形形色色，比如Nvidia、AMD、Intel都推出了自己的GPU，其中最为流行的就是Nvidia的GPU，其还推出了CUDA并行编程库。然而每个GPU生产公司都推出自己的编程库显然让学习成本上升很多，因此苹果公司就推出了标准OpenCL，说各个生产商都支持我的标准，只要有一套OpenCL的编程库就能对各类型的GPU芯片适用。当然了，OpenCL做到通用不是没有代价的，会带来一定程度的性能损失，在Nvidia的GPU上，CUDA性能明显比OpenCL高出一大截。目前CUDA和OpenCL是最主流的两个GPU编程库。
      从编程语言角度看，CUDA和OpenCL都是原生支持C/C++的，其它语言想要访问还有些麻烦，比如Java，需要通过JNI来访问CUDA或者OpenCL。基于JNI，现今有各种Java版本的GPU编程库，比如JCUDA等。另一种思路就是语言还是由java来编写，通过一种工具将java转换成C。
![](https://images0.cnblogs.com/blog2015/522490/201506/220748532675670.png)
图3 GPU编程库
LWJGL ([http://www.lwjgl.org/](http://www.lwjgl.org/))
JOCL ([http://www.jocl.org](http://www.jocl.org/)/)
JCUDA ([http://www.jcuda.de](http://www.jcuda.de/)[/](http://www.jcuda.de/))
Aparapi ([http://code.google.com/p/aparapi](http://code.google.com/p/aparapi/)[/](http://code.google.com/p/aparapi/))
JavaCL ([http://code.google.com/p/javacl](http://code.google.com/p/javacl/)[/](http://code.google.com/p/javacl/)）
# 4 CUDA程序流程
![](https://images0.cnblogs.com/blog2015/522490/201506/220759089392412.png)
图4 CUDA程序流程
# 5 实践---以图像处理为例
        假设我们有如下图像处理任务，给每个像素值加1。并行方式很简单，为每个像素开一个GPU线程，由其进行加1操作。
![](https://images0.cnblogs.com/blog2015/522490/201506/220802009082042.png)
图5 例子
![](https://images0.cnblogs.com/blog2015/522490/201506/220803453922821.png)
图6 核函数
![](https://images0.cnblogs.com/blog2015/522490/201506/220804225499636.png)
图7 主流程函数
# 6 GPU加速效果
        下图是我实现的基于CUDA的P&D DEM图像预处理算法使用GPU的加速效果，GeForce GT 330是块普通台式机上的显卡，现在价格也就500人民币左右，用它达到了20倍的加速比，Tesla M2075是比较专业的显卡，价格一万左右，用它达到了将近百倍的加速比，这个程序i7 CPU单进程单线程要跑2个小时，而用Tesla M2075 GPU只花了一分多钟就完成计算。
![](https://images0.cnblogs.com/blog2015/522490/201506/220806422058104.png)
图8 P&D DEM图像预处理算法加速效果

