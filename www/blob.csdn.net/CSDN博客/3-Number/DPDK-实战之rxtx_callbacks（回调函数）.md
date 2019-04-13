
# DPDK-实战之rxtx_callbacks（回调函数） - 3-Number - CSDN博客


2017年10月16日 14:21:04[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：514


# 0x01 缘由
继续学习DPDK相关例子。此例子为用用户定义的回调函数来为所有接收的数据包添加时间戳，计算每个数据包转发的时间。
# 0x02 源码
此例子与上篇《DPDK-实战之skeleton(basicfwd)》有一点点不同，此处仅列出不同之处。
```python
static inline int
port_init(uint8_t port, struct rte_mempool *mbuf_pool)
{
    /*.....*/
    //此处添加连个回调函数，一个用户接收数据包（add_timestamps 添加时间戳），一个用于发送数据包（calc_latency 计算延迟）
    rte_eth_add_rx_callback(port, 0, add_timestamps, NULL);
    rte_eth_add_tx_callback(port, 0, calc_latency, NULL);
    return 0;
}
static uint16_t
add_timestamps(uint8_t port __rte_unused, uint16_t qidx __rte_unused,
        struct rte_mbuf **pkts, uint16_t nb_pkts,
        uint16_t max_pkts __rte_unused, void *_ __rte_unused)
{
    unsigned i;
    uint64_t now = rte_rdtsc();
    for (i = 0; i < nb_pkts; i++)
        pkts[i]->udata64 = now;
    return nb_pkts;
}
static uint16_t
calc_latency(uint8_t port __rte_unused, uint16_t qidx __rte_unused,
        struct rte_mbuf **pkts, uint16_t nb_pkts, void *_ __rte_unused)
{
    uint64_t cycles = 0;
    uint64_t now = rte_rdtsc();
    unsigned i;
    for (i = 0; i < nb_pkts; i++)
        cycles += now - pkts[i]->udata64;
    latency_numbers.total_cycles += cycles;
    latency_numbers.total_pkts += nb_pkts;
    if (latency_numbers.total_pkts > (100 * 1000 * 1000ULL)) {
        printf("Latency = %"PRIu64" cycles\n", /* PRIu64 为一个宏 打印64位整形值*/
        latency_numbers.total_cycles / latency_numbers.total_pkts);
        latency_numbers.total_cycles = latency_numbers.total_pkts = 0;
    }
    return nb_pkts;
}
```

# 0x03 总结
例子比较简单，但是所有的基础知识都是由简单到难循序渐进的。

