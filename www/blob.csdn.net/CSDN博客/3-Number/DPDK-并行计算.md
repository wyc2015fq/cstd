
# DPDK-并行计算 - 3-Number - CSDN博客


2017年11月14日 16:33:44[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：245


### 0x01 缘由
继续学习DPDK在并行计算上的优化。对于DPDK的主要应用领域--数据包处理。资源局部优化、避免跨核共享、减少临界区碰撞、加快临界区皖苏完成速率，都不同程度地降低了不可并行部分和并发干扰部分的占比。
### 0x02 慨念
**多核处理器：**在一个处理器中集成两个或者多个完整的内核（及计算引擎）。
**超线程（Hyper-Threading）:**在一个处理器中提供两个逻辑执行现场，逻辑线程共享流水线、执行单元和缓存。DPDK是一种I/O集中的负载，对于这类负载，IPC相对不是特别高，所以超线程技术会有一定程度的帮助。
**处理器核数（cpu cores）：**每个物理CPU中core个数。cat /proc/cpuinfo | grep "cpu cores" | uniq
**逻辑处理器核心数：**sibling 是内核认为的单个物理处理器所有的超线程个数，也就是一个物理封装中的逻辑核的个数。如果sibling等于实际物理核数的话，就说明没有启动超线程。反之，则启用。
**系统物理处理器封装ID: Socket(插槽)**，也就是所谓的物理处理器封装个数。physical id。cat /proc/cpuinfo | grep "physical id" | sort | uniq | wc -l 编号从0开始。
**系统逻辑处理器ID:**cat /proc/cpuinfo | grep "processor" | wc -l
**亲和性（Core affinity）：**一个特定的任务要在某个给定的CPU上尽量长时间地运行而不被迁移到其他处理上的倾向性。好处就是能够提高了CPU Cache的命中率，从而减少内存访问损耗，提高程序的速度。
**线程独占：**要将某个逻辑核从内核调度系统剥离，采用方法为isolcpus。
**线程模型：**抢占式线程模型，受内核调度只配。
**指令并发：**现代多核处理器几乎都采用了超标量的体系结构来提高指令的并发度，并进一步地允许对无依赖关系的指令乱序执行。
**单指令多数据(SIMD Single-Instruction Multiple-Data)：**多数据指以特定宽度为一个数据单元，多单元数据独立操作。对于I/O密集的负载，如DPDK，最大的抵用访存带宽，减少处理流水线后端因I/O访问造成的CPU失速。Intel SSE\AVX指令集。
**资源管理（cgroup --control group）**：Linux内核提供的一种可以限制、记录、隔离进程组所用的物理资源的机制。
### 0x03 DPDK中的具体过程
#### 3.1 DPDK的多线程
DPDK的线程基于pthread接口创建，属于抢占式线程模型，受内核调度支配。DPDK的线程可以作为控制线程，也可以作为数据线程。下面是DPDK的线程模型：
![](https://img-blog.csdn.net/20171114163020183?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

注意事项，如果有用户自己创建一些一些线程，而不按DPDK的开发模式，则要注意一些事项？
1、在普通用户创建的线程中，lcore id的值是LCORE_ID_ANY，以此确定这个thread是一个有效普通用户创建的pthread。
2、客户创建的线程可能无法发挥DPDK库的最高性能。
#### 3.1 DPDK的指令并发
主要在一些内存拷贝上的用法，如DPDK中memcpy就利用到了SSE/AVX的特点。
### 0x04 总结
可以不对细节有很深的理解，也不需要再次开发。学习的目的是，更加了解DPDK的设计思想。


