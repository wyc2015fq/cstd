# OpenMP、MPICH与OpenMPI - Koma Hub - CSDN博客
2017年11月24日 12:01:37[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：3063
[原文网址](http://blog.csdn.net/ddreaming/article/details/53319501)
openmp比较简单，修改现有的大段代码也容易。基本上openmp只要在已有程序基础上根据需要加并行语句即可。而mpi有时甚至需要从基本设计思路上重写整个程序，调试也困难得多，涉及到局域网通信这一不确定的因素。不过，openmp虽然简单却只能用于单机多CPU/多核并行，mpi才是用于多主机超级计算机集群的强悍工具，当然复杂。
**（1）MPI＝message passing interface：**
在分布式内存（distributed-memory）之间实现信息通讯的一种 规范/标准/协议（standard）。它是一个库，不是一门语言。可以被fortran，c，c++等调用。MPI 允许静态任务调度，显示并行提供了良好的性能和移植性，用 MPI 编写的程序可直接在多核集群上运行。在集群系统中，集群的各节点之间可以采用 MPI 编程模型进行程序设计，每个节点都有自己的内存，可以对本地的指令和数据直接进行访问，各节点之间通过互联网络进行消息传递，这样设计具有很好的可移植性，完备的异步通信功能，较强的可扩展性等优点。MPI
 模型存在一些不足，包括：程序的分解、开发和调试相对困难，而且通常要求对代码做大量的改动；通信会造成很大的开销，为了最小化延迟，通常需要大的代码粒度；细粒度的并行会引发大量的通信；动态负载平衡困难；并行化改进需要大量地修改原有的串行代码，调试难度比较大
**（2）MPICH和OpenMPI：**
它们都是采用MPI标准，在并行计算中，实现节点间通信的开源软件。各自有各自的函数，指令和库。
Reference:
They are two implementations of the MPI standard. In the late 90s and early 2000s, there were many different MPI implementations, and the implementors started to realize they were all re-inventing the wheel; there was something of a consolidation. The LAM/MPI
 team joined with the LA/MPI, FT-MPI, and eventually PACX-MPI teams to develop OpenMPI. LAM MPI stopped being developed in 2007. The code base for OpenMPI was completely new, but it brought in ideas and techniques from all the different teams.
Currently, the two major open-source MPI implementation code-bases are OpenMPI andMPICH2.
而MPICH2是MPICH的一个版本。
有的计算机厂商，也会针对旗下机型特点，自主开发基于MPICH的MPI软件，从而使机器的并行计算效率得以提高。
**（3）OpenMP：**
在节点内（多核 SMP）执行的基于共享内存的编程模型。
OpenMP是针对单主机上多核/多CPU并行计算而设计的工具，换句话说，OpenMP更适合单台计算机共享内存结构上的并行计算。由于使用线程间共享内存的方式协调并行计算，它在多核/多CPU结构上的效率很高、内存开销小、编程语句简洁直观，因此编程容易、编译器实现也容易（现在最新版的C、C++、Fortran编译器基本上都内置OpenMP支持）。不过OpenMP最大的缺点是只能在单台主机上工作，不能用于多台主机间的并行计算
