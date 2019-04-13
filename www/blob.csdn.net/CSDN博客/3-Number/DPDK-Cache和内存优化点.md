
# DPDK-Cache和内存优化点 - 3-Number - CSDN博客


2017年11月13日 15:49:51[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：453


# 0x01 缘由
在一篇博文中看到false-sharing问题（多核编程中伪共享问题），引发了我对DPDK内存和Cache的思考和学习。看看这些知识点，大部分都是计算机组成原理和操作系统的知识点，如果做系统编程，这些知识点不能还给大学老师。
# 0x02 概念
**南桥：**系统I/O芯片（SI/O）：主要管理中低速外部设备；集成了中断控制器、DMA控制器、PCI网卡等。
**北桥：**系统控制芯片，主要负责CPU与内存、CPU与PCI-E之间的通信。掌控项目多为高速设备，如：CPU、Host Bus。后期北桥集成了内存控制器、Cache高速控制器。
**NUMA系统（Non-Uniform Memory Architecture，非一致性内存架构）**：一种用于多处理器的电脑记忆体设计，内存访问时间取决于处理器的内存位置。 在NUMA下，处理器访问它自己的本地存储器的速度比非本地存储器（存储器的地方到另一个处理器之间共享的处理器或存储器）快一些。下图是至强E5-2400处理器架构图。
![](https://img-blog.csdn.net/20171113154555228?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**Cache:**位于CPU和主存之间的一个高速小存储容量的存储器。目的是为了匹配处理器和内存之间存在的巨大的速度鸿沟。
**TLB(Translation Lookaside Buffer)**：转换检测缓冲区是一个内存管理单元,用于改进虚拟地址到物理地址转换速度的缓存。
**Cache预取**：分为硬件预取、软件预取。将数据和指令提前放入到Cache行中。
**Cache 一致性：**多个核共享数据结构时，缓存到Cache中的内容不一致。
**MESI协议：**解决Cache line一致性问题，修改（Modified）\独占（Exclusive）\共享（Shared）\失效（Invaild）状态，通过状态机的变换控制缓存的一致性；
大页：内存的分配和管理是按页进行分配，一般常规页为4KB，但是4KB的页会带来TLB不命中的问题，调整页的大小；
# 0x03 DPDK Cache和内存的优化
DPDK在上述内存访问上做了协议优化，如预取、Cache line对齐、Cache一致性、大页、NUMA系统结构等。
## 3.1 DPDK预取
DPDK一个处理器核每秒钟大概能够处理33M个报文，大概每30纳秒需要处理一个报文，假设处理器的主频是2.7GHz，那么大概每80个处理器时钟周期就需要处理一个报文。
接收报文的代码段：
```python
nmb = rte_rxmbuf_alloc(rxq->mb_pool);
        if (nmb == NULL) {
            PMD_RX_LOG(DEBUG, "RX mbuf alloc failed port_id=%u "
                   "queue_id=%u", (unsigned) rxq->port_id,
                   (unsigned) rxq->queue_id);
            rte_eth_devices[rxq->port_id].data->rx_mbuf_alloc_failed++;
            break;
        }
        nb_hold++;
        rxe = &sw_ring[rx_id];
        rx_id++;
        if (rx_id == rxq->nb_rx_desc)
            rx_id = 0;
        /* Prefetch next mbuf while processing current one. */
        rte_igb_prefetch(sw_ring[rx_id].mbuf);
        /*
         * When next RX descriptor is on a cache-line boundary,
         * prefetch the next 4 RX descriptors and the next 8 pointers
         * to mbufs.
         */
        if ((rx_id & 0x3) == 0) {
            rte_igb_prefetch(&rx_ring[rx_id]);
            rte_igb_prefetch(&sw_ring[rx_id]);
    .....
```

## 3.2 Cache Line 对齐
```python
/**
* A structure that stores the ring statistics (per-lcore).
*/
struct rte_ring_debug_stats {
    uint64_t enq_success_bulk; /**< Successful enqueues number. */
    uint64_t enq_success_objs; /**< Objects successfully enqueued. */
    uint64_t enq_quota_bulk;   /**< Successful enqueues above watermark. */
    uint64_t enq_quota_objs;   /**< Objects enqueued above watermark. */
    uint64_t enq_fail_bulk;    /**< Failed enqueues number. */
    uint64_t enq_fail_objs;    /**< Objects that failed to be enqueued. */
    uint64_t deq_success_bulk; /**< Successful dequeues number. */
    uint64_t deq_success_objs; /**< Objects successfully dequeued. */
    uint64_t deq_fail_bulk;    /**< Failed dequeues number. */
    uint64_t deq_fail_objs;    /**< Objects that failed to be dequeued. */
} __rte_cache_aligned;
```
**__attribute__**((align(64)))
关于这个缓存行对齐问题，参考如下博文：
http://www.cpper.cn/2016/10/03/develop/false-sharing/?utm_source=tuicool&utm_medium=referral
## 3.3 DPDK如何保证Cache一致性
1避免多个核访问同一个内存地址或者数据结构。这样每个核尽量都避免与其他核共享数据，从而减少因为错误的数据共享导致的Cache一致性的开销。
如下例子访问同样的配置：
```python
#define MAX_RX_QUEUE_PER_LCORE 16
#define MAX_TX_QUEUE_PER_PORT 16
struct lcore_queue_conf {
    unsigned n_rx_port;
    unsigned rx_port_list[MAX_RX_QUEUE_PER_LCORE];
} __rte_cache_aligned; //对齐
struct lcore_queue_conf lcore_queue_conf[RTE_MAX_LCORE]; //每个核一个配置结构 RTE_MAX_LCORE
```
对网络端口的访问，访问同一个网卡：
通过为每个核都准备一个单独的接收和发送队列。
# 3.4 NUMA 系统结构
1Per-core memory。一个处理器上有多个核（core），per-core memory是指每个核都有属于自己的内存，即对于经常要访问的数据结构，每个核都有自己的备份。
2本地设备本地处理。即用本地的处理器、本地的内存来处理本地的设备上产生的数据。如果一个PCI设备在node0上，就用node0上的核来处理该设备，处理该设备用到的数据机构和数据缓冲区都从node0上分配。
# 0x04 总结
DPDK有较高的性能，利用了相关的优化技能，在大量的工作实践测试中提炼出来。后续继续学习先关知识点，也是回顾。


