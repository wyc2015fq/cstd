
# DPDK-实战之skeleton(basicfwd) - 3-Number - CSDN博客


2017年10月13日 15:17:32[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：1607


# 0x01 缘由
继续学习DPDK示例，主要熟悉每个环节和设计理念，今天要学习的是一个相对简答的服务，仅仅从一个网口抓取数据包转发到另外一个网口，这样做双向转发，相当于桥的功能。其他基础业务都不做。
# 0x02 直接上源码分析
```python
#include <stdint.h>
#include <inttypes.h>
#include <rte_eal.h>
#include <rte_ethdev.h>
#include <rte_cycles.h>
#include <rte_lcore.h>
#include <rte_mbuf.h>
#define RX_RING_SIZE 128
#define TX_RING_SIZE 512
#define NUM_MBUFS 8191
#define MBUF_CACHE_SIZE 250
#define BURST_SIZE 32
static const struct rte_eth_conf port_conf_default = {
    .rxmode = { .max_rx_pkt_len = ETHER_MAX_LEN }
};
/* basicfwd.c: Basic DPDK skeleton forwarding example. */
/*
 * 用全局默认的配置来初始化网口。在内存池中分配接收队列和发送队列。
 */
static inline int
port_init(uint8_t port, struct rte_mempool *mbuf_pool)
{
    struct rte_eth_conf port_conf = port_conf_default; //全部配置模式
    const uint16_t rx_rings = 1, tx_rings = 1;   //每个网口队列多少，这里都为1
    int retval;
    uint16_t q;
    if (port >= rte_eth_dev_count())  //检查网口编号
        return -1;
    /* 配置以太网设备 */
    retval = rte_eth_dev_configure(port, rx_rings, tx_rings, &port_conf);
    if (retval != 0)
        return retval;
    /* 为每个以太网网口分配和设置1个接收队列 */
    for (q = 0; q < rx_rings; q++) {
        retval = rte_eth_rx_queue_setup(port, q, RX_RING_SIZE,
                rte_eth_dev_socket_id(port), NULL, mbuf_pool);
        if (retval < 0)
            return retval;
    }
    /* 为每个以太网网口分配和设置1个发送队列 */
    for (q = 0; q < tx_rings; q++) {
        retval = rte_eth_tx_queue_setup(port, q, TX_RING_SIZE,
                rte_eth_dev_socket_id(port), NULL);
        if (retval < 0)
            return retval;
    }
    /* 开启以太网网口 */
    retval = rte_eth_dev_start(port);
    if (retval < 0)
        return retval;
    /* 打印网卡地址 MAC address. */
    struct ether_addr addr;
    rte_eth_macaddr_get(port, &addr);
    printf("Port %u MAC: %02" PRIx8 " %02" PRIx8 " %02" PRIx8
               " %02" PRIx8 " %02" PRIx8 " %02" PRIx8 "\n",
            (unsigned)port,
            addr.addr_bytes[0], addr.addr_bytes[1],
            addr.addr_bytes[2], addr.addr_bytes[3],
            addr.addr_bytes[4], addr.addr_bytes[5]);
    /* 网口开启混杂模式 */
    rte_eth_promiscuous_enable(port);
    return 0;
}
/*
 * 一个线程处理一对网口，数据包一进一出。
 */
static __attribute__((noreturn)) void
lcore_main(void)
{
    const uint8_t nb_ports = rte_eth_dev_count(); //网口数
    uint8_t port;
    /*
     * 当有NUMA结构时，检查网口是否在同一个NUMA node节点上，只有在一个NUMA node上时轮询线程效率最好。
     */
    for (port = 0; port < nb_ports; port++)
        if (rte_eth_dev_socket_id(port) > 0 &&
                rte_eth_dev_socket_id(port) !=
                        (int)rte_socket_id())
            printf("WARNING, port %u is on remote NUMA node to "
                    "polling thread.\n\tPerformance will "
                    "not be optimal.\n", port);
    printf("\nCore %u forwarding packets. [Ctrl+C to quit]\n",
            rte_lcore_id());
    /* 死循环处理数据包的搬运 */
    for (;;) {
        /*
         * 一个端口接收包，然后直接转发到这对数据包上。
         * 如 0 -> 1, 1 -> 0, 2 -> 3, 3 -> 2 等。
         */
        for (port = 0; port < nb_ports; port++) {
            /* 从一个端口接收数据包 */
            struct rte_mbuf *bufs[BURST_SIZE];
            const uint16_t nb_rx = rte_eth_rx_burst(port, 0,
                    bufs, BURST_SIZE);
            if (unlikely(nb_rx == 0))
                continue;
            /* Send burst of TX packets, to second port of pair. */
            const uint16_t nb_tx = rte_eth_tx_burst(port ^ 1, 0,
                    bufs, nb_rx);
            /* 释放没有发送的数据包。 */
            if (unlikely(nb_tx < nb_rx)) {
                uint16_t buf;
                for (buf = nb_tx; buf < nb_rx; buf++)
                    rte_pktmbuf_free(bufs[buf]);
            }
        }
    }
}
/*
 * The main function, which does initialization and calls the per-lcore
 * functions.
 */
int
main(int argc, char *argv[])
{
    struct rte_mempool *mbuf_pool; //使用内存池空间来容纳ring队列，接收和发送数据包
    unsigned nb_ports;             //网卡数
    uint8_t portid;                //网卡编号
    /* Initialize the Environment Abstraction Layer (EAL). */
    int ret = rte_eal_init(argc, argv);
    if (ret < 0)
        rte_exit(EXIT_FAILURE, "Error with EAL initialization\n");
    argc -= ret;
    argv += ret;
    /* 检查网卡是否为偶数 send/receive */
    nb_ports = rte_eth_dev_count();
    if (nb_ports < 2 || (nb_ports & 1))
        rte_exit(EXIT_FAILURE, "Error: number of ports must be even\n");
    /*  创建一个内存池来容纳mbufs。大小按网口多少来分配。相当于每个网口都要有一个接受和发送队列。*/
    mbuf_pool = rte_pktmbuf_pool_create("MBUF_POOL", NUM_MBUFS * nb_ports,
        MBUF_CACHE_SIZE, 0, RTE_MBUF_DEFAULT_BUF_SIZE, rte_socket_id());
    if (mbuf_pool == NULL)
        rte_exit(EXIT_FAILURE, "Cannot create mbuf pool\n");
    /* 初始化所有网口配置 */
    for (portid = 0; portid < nb_ports; portid++)
        if (port_init(portid, mbuf_pool) != 0)
            rte_exit(EXIT_FAILURE, "Cannot init port %"PRIu8 "\n",
                    portid);
    if (rte_lcore_count() > 1)
        printf("\nWARNING: Too many lcores enabled. Only 1 used.\n");
    /* 仅仅用一个主线程调用，相当与一个线程处理一对网口的数据包。*/
    lcore_main();
    return 0;
}
```

# 0x03 运行实例
```python
[root@Huawei x86_64-native-linuxapp-gcc]# ./basicfwd -l 1 -n 4
EAL: Detected lcore 0 as core 0 on socket 0
EAL: Detected lcore 1 as core 1 on socket 0
EAL: Detected lcore 2 as core 2 on socket 0
EAL: Detected lcore 3 as core 3 on socket 0
EAL: Detected lcore 4 as core 4 on socket 0
EAL: Detected lcore 5 as core 5 on socket 0
EAL: Detected lcore 6 as core 6 on socket 0
EAL: Detected lcore 7 as core 7 on socket 0
EAL: Detected lcore 8 as core 0 on socket 1
EAL: Detected lcore 9 as core 1 on socket 1
EAL: Detected lcore 10 as core 2 on socket 1
EAL: Detected lcore 11 as core 3 on socket 1
EAL: Detected lcore 12 as core 4 on socket 1
EAL: Detected lcore 13 as core 5 on socket 1
EAL: Detected lcore 14 as core 6 on socket 1
EAL: Detected lcore 15 as core 7 on socket 1
EAL: Detected lcore 16 as core 0 on socket 0
EAL: Detected lcore 17 as core 1 on socket 0
EAL: Detected lcore 18 as core 2 on socket 0
EAL: Detected lcore 19 as core 3 on socket 0
EAL: Detected lcore 20 as core 4 on socket 0
EAL: Detected lcore 21 as core 5 on socket 0
EAL: Detected lcore 22 as core 6 on socket 0
EAL: Detected lcore 23 as core 7 on socket 0
EAL: Detected lcore 24 as core 0 on socket 1
EAL: Detected lcore 25 as core 1 on socket 1
EAL: Detected lcore 26 as core 2 on socket 1
EAL: Detected lcore 27 as core 3 on socket 1
EAL: Detected lcore 28 as core 4 on socket 1
EAL: Detected lcore 29 as core 5 on socket 1
EAL: Detected lcore 30 as core 6 on socket 1
EAL: Detected lcore 31 as core 7 on socket 1
EAL: Support maximum 128 logical core(s) by configuration.
EAL: Detected 32 lcore(s)
EAL: Setting up physically contiguous memory...
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7f45f8000000 (size = 0x200000)
EAL: Ask a virtual area of 0x7c00000 bytes
EAL: Virtual area found at 0x7f45f0200000 (size = 0x7c00000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7f45efe00000 (size = 0x200000)
EAL: Ask a virtual area of 0x7c00000 bytes
EAL: Virtual area found at 0x7f45e8000000 (size = 0x7c00000)
EAL: Ask a virtual area of 0x400000 bytes
EAL: Virtual area found at 0x7f45e7a00000 (size = 0x400000)
EAL: Requesting 64 pages of size 2MB from socket 0
EAL: Requesting 64 pages of size 2MB from socket 1
EAL: TSC frequency is ~2593994 KHz
EAL: Master lcore 1 is ready (tid=f83f2880;cpuset=[1])
EAL: PCI device 0000:04:00.0 on NUMA socket 0
EAL:   probe driver: 8086:1521 rte_igb_pmd
EAL:   Not managed by a supported kernel driver, skipped
EAL: PCI device 0000:04:00.1 on NUMA socket 0
EAL:   probe driver: 8086:1521 rte_igb_pmd
EAL:   Not managed by a supported kernel driver, skipped
EAL: PCI device 0000:04:00.2 on NUMA socket 0
EAL:   probe driver: 8086:1521 rte_igb_pmd
EAL:   PCI memory mapped at 0x7f45f7f00000
EAL:   PCI memory mapped at 0x7f45f7efc000
PMD: eth_igb_dev_init(): port_id 0 vendorID=0x8086 deviceID=0x1521
EAL: PCI device 0000:04:00.3 on NUMA socket 0
EAL:   probe driver: 8086:1521 rte_igb_pmd
EAL:   PCI memory mapped at 0x7f45f0100000
EAL:   PCI memory mapped at 0x7f45f8408000
PMD: eth_igb_dev_init(): port_id 1 vendorID=0x8086 deviceID=0x1521
EAL: PCI device 0000:05:00.0 on NUMA socket 0
EAL:   probe driver: 8086:1522 rte_igb_pmd
EAL:   Not managed by a supported kernel driver, skipped
EAL: PCI device 0000:05:00.1 on NUMA socket 0
EAL:   probe driver: 8086:1522 rte_igb_pmd
EAL:   Not managed by a supported kernel driver, skipped
EAL: PCI device 0000:05:00.2 on NUMA socket 0
EAL:   probe driver: 8086:1522 rte_igb_pmd
EAL:   Not managed by a supported kernel driver, skipped
EAL: PCI device 0000:05:00.3 on NUMA socket 0
EAL:   probe driver: 8086:1522 rte_igb_pmd
EAL:   Not managed by a supported kernel driver, skipped
PMD: eth_igb_rx_queue_setup(): sw_ring=0x7f45eff90fc0 hw_ring=0x7f45eff91400 dma_addr=0x67391400
PMD: eth_igb_tx_queue_setup(): sw_ring=0x7f45eff7ee40 hw_ring=0x7f45eff80e80 dma_addr=0x67380e80
PMD: eth_igb_start(): <<
Port 0 MAC: a0 36 9f 03 a8 ca
PMD: eth_igb_rx_queue_setup(): sw_ring=0x7f45eff6e7c0 hw_ring=0x7f45eff6ec00 dma_addr=0x6736ec00
PMD: eth_igb_tx_queue_setup(): sw_ring=0x7f45eff5c640 hw_ring=0x7f45eff5e680 dma_addr=0x6735e680
PMD: eth_igb_start(): <<
Port 1 MAC: a0 36 9f 03 a8 cb
Core 1 forwarding packets. [Ctrl+C to quit]
```

# 0x04 总结
思路比较简单，一个网口用两个缓存来缓存数据包，然后将数据包转发出去。此处用到了内存池结构。

