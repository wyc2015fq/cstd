# CUDA知识普及 - wishchinYang的专栏 - CSDN博客
2013年12月31日 16:43:39[wishchin](https://me.csdn.net/wishchin)阅读数：809
IT168 CUDA专题: [http://www.it168.com/tag/3263_1.shtml](http://www.it168.com/tag/3263_1.shtml)
CUDA 深入浅出： [http://www.opengpu.org/forum.php?mod=viewthread&tid=2109](http://www.opengpu.org/forum.php?mod=viewthread&tid=2109)    很详细的解释！
异构技术构建云计算平台：[http://tech.it168.com/a2011/1215/1289/000001289157.shtml](http://tech.it168.com/a2011/1215/1289/000001289157.shtml)
Vs13添加CUDA支持 ：[http://blog.csdn.net/wishchin/article/details/51883142](http://blog.csdn.net/wishchin/article/details/51883142)
GPU优化与实例分析：[http://tech.it168.com/a2011/1215/1289/000001289225.shtml](http://tech.it168.com/a2011/1215/1289/000001289225.shtml)
[](http://tech.it168.com/a2011/1215/1289/000001289225.shtml)
[](http://tech.it168.com/a2011/1215/1289/000001289225.shtml)
**（一）：兼容库**
thrust快速入门教程：算法：[http://tech.it168.com/a2011/0714/1218/000001218181.shtml](http://tech.it168.com/a2011/0714/1218/000001218181.shtml)
      Thrust是一个类似于STL的针对CUDA的C++模板库，能够使程序更简洁易读。Thrust提供与CUDA C完全兼容的接口，可以使我们高效地编写高性能并行程序。通过利用Thrust，程序员得以快速构建CUDA程序，并能够获得极高的稳定性和性能与精度，并行排序等例程的速度可提升5至100倍。
　　Thrust提供了大量的常用并行算法。这些算法与STL的算法非常相似，于是我们使用了相同的名称(例如thrust::sort 与std::sort)。
　　所有的Thrust算法可以在主机端和设备端上使用。尤其是，当Thrust算法转入主机端迭代器时，将会调度主机端路径，同样，当使用设备端迭代器时将使用设备端实现。
　　thrust::copy是一个例外，他可以任意的拷贝主机端和设备端的数据。但是所有的迭代器参数必须符合Thrust算法的要求，要么都在主机端，要么都在设备端。当不能满足要求的时候，编译器会报错。
**Transformations**
　　Transformations算法作用是用来将目标容器赋上特定值(例如零)或者特定数列。之前的例子我们已经使用过thrust::fill，可以向所有元素赋特定值。此外transformations算法还包括thrust::sequence、thrust::replace、thrust::transform。完整的列表请参考文档。
　　下面的代码演示了几个transformation算法的用法。注意类似于C++中拥有的thrust::negate和thrust::modulus，Thrust在thrust/functional.h中也提供了，此外还有plus与multiplies等。
# include <thrust/ device_vector .h>
# include <thrust/ transform .h>
# include <thrust/ sequence .h>
# include <thrust/ copy .h>
# include <thrust/ fill .h>
# include <thrust/ replace .h>
# include <thrust/ functional .h>
# include <iostream>
int main ( void )
{
// allocate three device_vectors with 10 elements
thrust :: device_vector <int> X (10)
 ;
thrust :: device_vector <int> Y (10)
 ;
thrust :: device_vector <int> Z (10)
 ;
// initialize X to 0,1,2,3, ....
thrust :: sequence (X. begin () , X. end ());
// compute Y = -X
thrust :: transform (X. begin () , X.end () , Y. begin () , thrust :: negate<int>()
 );
// fill Z with twos
thrust :: fill (Z. begin () , Z. end () , 2);
// compute Y = X mod 2
thrust :: transform (X. begin () , X.end () , Z. begin () , Y. begin () , thrust :: modulus<int-
>() );
// replace all the ones in Y with tens
thrust :: replace (Y. begin () , Y. end () , 1, 10) ;
// print Y
thrust :: copy (Y. begin () , Y. end () , std :: ostream_iterator<int>(
 std :: cout ,"\n"));
return0;
}
　　thrust/fuctuional.h中的函数提供了大部分内置代数和比较运算，但是我们想提供更多出色的功能。比如，运算y < - a * x + y，x、y为向量，a为常数标量。这其实就是我们所熟知的由BLAS提供的SAXPY运算。
　　如果我们在thrust中实现SAXPY我们有几个选择。一个是，我们需要使用两个transformations(一个加和一个乘法)还有一个临时数则用于存储a乘后的值。另一更佳选择是使用一个单独的由用户自己定义函数的transformation，这才是我们真正先要的。我下面用源代码解释说明这两种方法。
struct saxpy_functor
{
constfloat a;
saxpy_functor ( float _a) : a(_a) {}
__host__ __device__
floatoperator ()( constfloat& x,constfloat&
 y)const {
return a * x + y;
}
};
void saxpy_fast (float A, thrust :: device_vector<float>&
 X, thrust :: device_vector<-
float>& Y)
{
// Y <- A * X + Y
thrust :: transform (X. begin () , X.end () , Y. begin () , Y. begin () , saxpy_functor (A));
}
void saxpy_slow (float A, thrust :: device_vector<float>&
 X, thrust :: device_vector<-
float>& Y)
{
thrust :: device_vector <float> temp (X. size ());
// temp <- A
thrust :: fill ( temp . begin () , temp . end () , A);
// temp <- A * X
thrust :: transform (X. begin () , X.end () , temp . begin () , temp . begin () , thrust ::-
multiplies <float>() );
// Y <- A * X + Y
thrust :: transform ( temp . begin () , temp . end () , Y. begin () , Y. begin () , thrust :: plus<-
float>() );
}
　　Saxpy_fast和saxpy_slow都是有效的SAXPY实现，尽管saxpy_fast会比saxpy_slow更快。忽略临时向量分配与代数运算的花费，其开销如下：
　　fast_saxpy：2N次读取和N次写入
　　slow_saxpy：4N次读取和3N写入
　　因为SAXPY受到[内存](http://product.it168.com/list/b/0205_1.shtml)约束(它的性能受限于[内存](http://product.pcpop.com/Memory/10734_1.html)的带宽，而不是浮点性能)更大量的读写操作使得saxpy_slow开销更加昂贵。而saxpy_fast执行速度与优化的BLAS实现中的SAXPY一样快。在类似SAXPY内存约束算法通常值得使用kernel融合(合并多个计算于单独的kernel)的方法以最小化内存的读写交换。
[（二）：基于gpu的异构程序开发流程](http://www.cnblogs.com/viviman/archive/2012/11/23/2784424.html)：
[http://www.cnblogs.com/viviman/archive/2012/11/23/2784424.html](http://www.cnblogs.com/viviman/archive/2012/11/23/2784424.html)
异构程序开发的特点决定了开发与传统开发手段还是有一些列不同的，通俗的说有一定的讲究。针对本项目，本章列出几条重要的值得注意的地方，从开发过程上指导整个流程，以保证程序的质量+和优化。对异构程序的开发，针对自己的开发业务，简单的叙述整个开发流程如图：![](http://pic002.cnblogs.com/images/2012/356510/2012112314211817.jpg)
这个过程文字叙述为：
过程1：数据准备
准备需要处理的业务的原始数据，比如你的数据来源为mysql或者app或者mongodb或者其他的，通常为了测试，我都会写一个生产随即浮点数的函数来模拟自己的项目。
过程2：业务逻辑设计
更加业务需求的功能，设计业务层的类，投资组合类大体上有4个功能，每个功能直接又是有依赖关系的，这个过程产生的软件产品是——类图。
过程3：业务逻辑实现
指的是在CPU中实现，可以被其他APP调用的接口；我建议将可并行和非可并行的事物逻辑都封装在这个业务类中，如果有并行的处理模块，在下一个软件过程中处理。这个过程产生的软件产品是——类的.h和.cpp。我时刻提醒自己——不急于求成并行模块的内核程序书写。
过程4：数据词典设计
为什么把这个过程放在这个地方，其实是不对的，因为数据从数据库中出来，到最后计算出来的数据存入数据库，这个整个过程都涉及到的东西，不应该放到这个地方。就像图中表示的一样，数据词典贯穿始终。
但是，放在这个过程中是有一定意义的，因为一个数据块放到一个gpu中进行并行计算，并且要从设备中拷贝出来，一个好的数据类型，对设备与主机传递的带宽和设备内存的使用都是具有重要意义的。很简单的说，谁都不愿意把一群没计算意义的string类型的字符串拷贝到gpu中，只作为标示一个计算结果的id来用，对吧。所以，数据词典设计也是一个迭代的过程，开发过程中发现数据词典可以优化，尽量优化！
数据词典的设计，原则是以设备gpu为服务对象，青睐设备原则。
何谓轻重之分，数据词典的设计，在异构开发中显得很重要。我们不求一步到位，但求改进。
过程5：内核程序设计
内核程序就是在gpu上开发的并行计算程序。
过程3中如果某个功能模块发现并行粒度很大，那么，我们要开始做真正有意义的事情了。
为了强调程序架构的清晰——
我们先建立.cuh文件，声明要并行计算的功能模块，注意，我们过程3中的业务功能只需要包含这个.cuh文件，就可以调用封装好的并行计算模块啦。
接着我们建立.cu文件，注意，所有内核操作的符号，必须在cu文件中实现。我们在cu文件中实现内核函数，处理并行的数据。
是的，内核程序我们不要嫌多写个头文件，多一个头文件，对于程序的架构是很有帮助的，工程化！
过程6：迭代优化
这里有两个意义上的优化：
第一，我们始终不能忘记业务上是否还可以进一步的优化；
第二，我们最期待的内核程序算法上的优化。
也许第二个是我们会更加有挑战，很简单的例子，我的排序算法就是比你的排序算法高级、快速；或者我的程序就是比你的程序使用的内存少、指令吐量大。我们在开发内核程序时，尽量不要浪费内核的资源，更杜绝可能的情况下内存越界和显露，这个后果不是软件崩溃，而是系统蓝屏！
最后一条，图中没有加，就是每一个版本，我们都保存记录，分析效率，作为自己优化程序的阶段性产品。我试图从cpu到gpu的每个版本都进行了版本定稿，这样，整个过程将是有趣的，我看到每个版本都在提升，会很得意。是的，原来——
我提醒自己注意，异构程序，步步为营，一步一个版本，逐个比较，稳定提高效率和质量！
