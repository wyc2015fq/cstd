
# DPDK-实战之load_balancer（负载均衡） - 3-Number - CSDN博客


2017年10月24日 15:19:07[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：2967


# 0x01 缘由
“负载均衡”这个名词经常听，实际实践中也有过尝试。利用DPDK来做数据流量的负载均衡。我现在负责的产品中，有一个教智能分流设备，可以根据一些规则对流量进行转发。用DPDK可以适合这种场景不？显然，只要专注其中，可以为公司节约很大的成本。
# 0x02 介绍
简单的负载均衡器应用表明一个将数据包IO操作与应用处理过程隔离。依据执行目标，将一些逻辑核专门做数据包的接收和转发，一部分逻辑核专门做一些业务操作。处理业务的逻辑核和处理数据包I/O的逻辑核通过ring缓存队列交互数据。
如下相关逻辑图：
![](https://img-blog.csdn.net/20171024151930082?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
负载hash算法为：
**worker_id = packet[load_balancing_field] % n_workers**
# 0x03 运行
运行前创建大页面大于64M，否则相关表会创建失败。
**./load_balancer -l 3-7 -n 4 -- --rx "(0,0,3)" --tx "(0,3),(1,3)" --w "4,5,6,7" --lpm "172.16.1.0/24=>0****; 172.16.3.0/24=>1;" --pos-lb 29**
**参数解释：**
-l 列出此次要使用的逻辑核序号
-n 内存相关参数
--rx 接收逻辑核角色配置 （网卡，队列，逻辑核）
--tx 转发队列逻辑核配置（网卡，逻辑核）
--w  业务处理逻辑核序号
--lpm配置，子网表示方式 =>转发口
--pos-lb 表示转发网口的位置，此处要求在包前64字节前。
**运行输出：**
```python
Creating the mbuf pool for socket 0 ...
Creating the LPM table for socket 0 ...
Creating ring to connect I/O lcore 3 (socket 0) with worker lcore 4 ...
Creating ring to connect I/O lcore 3 (socket 0) with worker lcore 5 ...
Creating ring to connect I/O lcore 3 (socket 0) with worker lcore 6 ...
Creating ring to connect I/O lcore 3 (socket 0) with worker lcore 7 ...
Creating ring to connect worker lcore 4 with TX port 0 (through I/O lcore 3) (socket 0) ...
Creating ring to connect worker lcore 4 with TX port 1 (through I/O lcore 3) (socket 0) ...
Creating ring to connect worker lcore 5 with TX port 0 (through I/O lcore 3) (socket 0) ...
Creating ring to connect worker lcore 5 with TX port 1 (through I/O lcore 3) (socket 0) ...
Creating ring to connect worker lcore 6 with TX port 0 (through I/O lcore 3) (socket 0) ...
Creating ring to connect worker lcore 6 with TX port 1 (through I/O lcore 3) (socket 0) ...
Creating ring to connect worker lcore 7 with TX port 0 (through I/O lcore 3) (socket 0) ...
Creating ring to connect worker lcore 7 with TX port 1 (through I/O lcore 3) (socket 0) ...
Initializing NIC port 0 ...
Initializing NIC port 0 RX queue 0 ...
PMD: eth_igb_rx_queue_setup(): sw_ring=0x7f27382d43c0 hw_ring=0x7f27382d6400 dma_addr=0x672d6400
Initializing NIC port 0 TX queue 0 ...
PMD: eth_igb_tx_queue_setup(): sw_ring=0x7f27382c0240 hw_ring=0x7f27382c4280 dma_addr=0x672c4280
PMD: eth_igb_start(): <<
Initializing NIC port 1 ...
Initializing NIC port 1 TX queue 0 ...
PMD: eth_igb_tx_queue_setup(): sw_ring=0x7f27382ac040 hw_ring=0x7f27382b0080 dma_addr=0x672b0080
PMD: eth_igb_start(): <<
Checking link status....................................done
Port 0 Link Up - speed 1000 Mbps - full-duplex
Port 1 Link Up - speed 1000 Mbps - full-duplex
Initialization completed.
NIC RX ports: 0 (0 )  ; //接收网口0
I/O lcore 3 (socket 0): RX ports  (0, 0)  ; Output rings  0x7f27382fd400  0x7f27382fb2c0  0x7f27382f9180  0x7f27382f7040  ;
Worker lcore 4 (socket 0) ID 0: Input rings  0x7f27382fd400  ;
Worker lcore 5 (socket 0) ID 1: Input rings  0x7f27382fb2c0  ;
Worker lcore 6 (socket 0) ID 2: Input rings  0x7f27382f9180  ;
Worker lcore 7 (socket 0) ID 3: Input rings  0x7f27382f7040  ;
NIC TX ports:  0  1  ; //转发网口0 1
I/O lcore 3 (socket 0): Input rings per TX port  0 (0x7f27382f4f00  0x7f27382f0c80  0x7f27382eca00  0x7f27382e8780  )  1 (0x7f27382f2dc0  0x7
f27382eeb40  0x7f27382ea8c0  0x7f27382e6640  )  ;
Worker lcore 4 (socket 0) ID 0: //工作队列0
Output rings per TX port  0 (0x7f27382f4f00)  1 (0x7f27382f2dc0)  ;
Worker lcore 5 (socket 0) ID 1: //工作队列1
Output rings per TX port  0 (0x7f27382f0c80)  1 (0x7f27382eeb40)  ;
Worker lcore 6 (socket 0) ID 2://工作队列2
Output rings per TX port  0 (0x7f27382eca00)  1 (0x7f27382ea8c0)  ;
Worker lcore 7 (socket 0) ID 3://工作队列3
Output rings per TX port  0 (0x7f27382e8780)  1 (0x7f27382e6640)  ;
LPM rules: //LPM规则
    0: 172.16.1.0/24 => 0;
    1: 172.16.3.0/24 => 1;
Ring sizes: NIC RX = 1024; Worker in = 1024; Worker out = 1024; NIC TX = 1024;
Burst sizes: I/O RX (rd = 144, wr = 144); Worker (rd = 144, wr = 144); I/O TX (rd = 144, wr = 144)
Logical core 4 (worker 0) main loop.
Logical core 5 (worker 1) main loop.
Logical core 6 (worker 2) main loop.
Logical core 7 (worker 3) main loop.
Logical core 3 (I/O) main loop
```
**实例测试：**
向网口0发包，在网口1抓包分析，结果再网口1抓的包目的地址都是172.16.3.0网段。说明将自网段负载到了1网口。
**结果分析：**
![](https://img-blog.csdn.net/20171024152011366?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
# 0x04 程序设计思路
**第一步：**一些网卡接收数据包，然后接收线程根据数据包算出hash值，根据hash值放入对应的环形队列；
**第二步：**工作线程从环形队列中取出数据包，对数据包做lpm查询，找到对应的出口网口，然后放入对应的环形队列中；
**第三步：**发送线程从环形队列中取出数据包，然后将数据从网口中转发出去；
每个线程尽量专注做一件事，将线程绑定到不同的逻辑核上。
程序结构设计也是根据线程角色进行结构设计，通过物理cpu与逻辑核、队列、配置进行关联。
# 0x05 源码分析
下面做一些具体的源码分析，这块代码叫多，但是逻辑相对较简单。
**main函数逻辑：**
```python
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/types.h>
#include <string.h>
#include <sys/queue.h>
#include <stdarg.h>
#include <errno.h>
#include <getopt.h>
#include <unistd.h>
//以上是glibc的头文件
#include <rte_common.h>
#include <rte_byteorder.h>
#include <rte_log.h>
#include <rte_memory.h>
#include <rte_memcpy.h>
#include <rte_memzone.h>
#include <rte_eal.h>
#include <rte_per_lcore.h>
#include <rte_launch.h>
#include <rte_atomic.h>
#include <rte_cycles.h>
#include <rte_prefetch.h>
#include <rte_lcore.h>
#include <rte_per_lcore.h>
#include <rte_branch_prediction.h>
#include <rte_interrupts.h>
#include <rte_pci.h>
#include <rte_random.h>
#include <rte_debug.h>
#include <rte_ether.h>
#include <rte_ethdev.h>
#include <rte_ring.h>
#include <rte_mempool.h>
#include <rte_mbuf.h>
#include <rte_ip.h>
#include <rte_tcp.h>
#include <rte_lpm.h>
//以上是dpdk的头文件
#include "main.h" //自定义的相关结构
int
main(int argc, char **argv)
{
    uint32_t lcore;
    int ret;
    /* 初始化抽象环境层 EAL */
    ret = rte_eal_init(argc, argv);
    if (ret < 0)
        return -1;
    argc -= ret;
    argv += ret;
    /* 解析运行时命令含参数 */
    ret = app_parse_args(argc, argv);
    if (ret < 0) {
        app_print_usage(); //出错时打印相关帮助信息
        return -1;
    }
    /* 初始化工作 */
    app_init();
    /* 打印相关参数 */
    app_print_params();
    /* Launch per-lcore init on every lcore */
    rte_eal_mp_remote_launch(app_lcore_main_loop, NULL, CALL_MASTER);
    RTE_LCORE_FOREACH_SLAVE(lcore) {
        if (rte_eal_wait_lcore(lcore) < 0) {
            return -1;
        }
    }
    return 0;
}
```
**相关结构说明：**
```python
#ifndef _MAIN_H_
#define _MAIN_H_
/* Logical cores */
#ifndef APP_MAX_SOCKETS
#define APP_MAX_SOCKETS 2  //最多两颗CPU核，实际在大量服务器集群或核心机房中，可能有相当多的CPU
#endif
#ifndef APP_MAX_LCORES
#define APP_MAX_LCORES       RTE_MAX_LCORE  //逻辑核最大数
#endif
#ifndef APP_MAX_NIC_PORTS
#define APP_MAX_NIC_PORTS    RTE_MAX_ETHPORTS //网卡最大数
#endif
#ifndef APP_MAX_RX_QUEUES_PER_NIC_PORT
#define APP_MAX_RX_QUEUES_PER_NIC_PORT 128 //每个网卡接收队列最大数
#endif
#ifndef APP_MAX_TX_QUEUES_PER_NIC_PORT
#define APP_MAX_TX_QUEUES_PER_NIC_PORT 128 //每个网卡发送队列最大数
#endif
#ifndef APP_MAX_IO_LCORES
#define APP_MAX_IO_LCORES 16            //定义APP最大IO逻辑核
#endif
#if (APP_MAX_IO_LCORES > APP_MAX_LCORES)  //异常检测
#error "APP_MAX_IO_LCORES is too big"
#endif
#ifndef APP_MAX_NIC_RX_QUEUES_PER_IO_LCORE
#define APP_MAX_NIC_RX_QUEUES_PER_IO_LCORE 16  //每个IO逻辑核最大网卡接收队列数
#endif
#ifndef APP_MAX_NIC_TX_PORTS_PER_IO_LCORE
#define APP_MAX_NIC_TX_PORTS_PER_IO_LCORE 16  //每个IO逻辑核最大网卡发送队列数
#endif
#if (APP_MAX_NIC_TX_PORTS_PER_IO_LCORE > APP_MAX_NIC_PORTS)
#error "APP_MAX_NIC_TX_PORTS_PER_IO_LCORE too big"
#endif
#ifndef APP_MAX_WORKER_LCORES
#define APP_MAX_WORKER_LCORES 16      //最大的工作线程逻辑核数
#endif
#if (APP_MAX_WORKER_LCORES > APP_MAX_LCORES)
#error "APP_MAX_WORKER_LCORES is too big"
#endif
/* Mempools */
#ifndef APP_DEFAULT_MBUF_DATA_SIZE
#define APP_DEFAULT_MBUF_DATA_SIZE  RTE_MBUF_DEFAULT_BUF_SIZE  //默认内存池大少
#endif
#ifndef APP_DEFAULT_MEMPOOL_BUFFERS
#define APP_DEFAULT_MEMPOOL_BUFFERS   8192 * 4       //内存缓存多小
#endif
#ifndef APP_DEFAULT_MEMPOOL_CACHE_SIZE
#define APP_DEFAULT_MEMPOOL_CACHE_SIZE  256   //CACHE大小
#endif
/* LPM Tables */
#ifndef APP_MAX_LPM_RULES
#define APP_MAX_LPM_RULES 1024     //LPM规则大小
#endif
/* NIC RX */
#ifndef APP_DEFAULT_NIC_RX_RING_SIZE
#define APP_DEFAULT_NIC_RX_RING_SIZE 1024   //网卡接收环大小
#endif
/* 之前说网卡的配置
* RX and TX Prefetch, Host, and Write-back threshold values should be
* carefully set for optimal performance. Consult the network
* controller's datasheet and supporting DPDK documentation for guidance
* on how these parameters should be set.
*/
#ifndef APP_DEFAULT_NIC_RX_PTHRESH
#define APP_DEFAULT_NIC_RX_PTHRESH  8
#endif
#ifndef APP_DEFAULT_NIC_RX_HTHRESH
#define APP_DEFAULT_NIC_RX_HTHRESH  8
#endif
#ifndef APP_DEFAULT_NIC_RX_WTHRESH
#define APP_DEFAULT_NIC_RX_WTHRESH  4
#endif
#ifndef APP_DEFAULT_NIC_RX_FREE_THRESH
#define APP_DEFAULT_NIC_RX_FREE_THRESH  64
#endif
#ifndef APP_DEFAULT_NIC_RX_DROP_EN
#define APP_DEFAULT_NIC_RX_DROP_EN 0
#endif
/* NIC TX */
#ifndef APP_DEFAULT_NIC_TX_RING_SIZE
#define APP_DEFAULT_NIC_TX_RING_SIZE 1024
#endif
/*
* These default values are optimized for use with the Intel(R) 82599 10 GbE
* Controller and the DPDK ixgbe PMD. Consider using other values for other
* network controllers and/or network drivers.
*/
#ifndef APP_DEFAULT_NIC_TX_PTHRESH
#define APP_DEFAULT_NIC_TX_PTHRESH  36
#endif
#ifndef APP_DEFAULT_NIC_TX_HTHRESH
#define APP_DEFAULT_NIC_TX_HTHRESH  0
#endif
#ifndef APP_DEFAULT_NIC_TX_WTHRESH
#define APP_DEFAULT_NIC_TX_WTHRESH  0
#endif
#ifndef APP_DEFAULT_NIC_TX_FREE_THRESH
#define APP_DEFAULT_NIC_TX_FREE_THRESH  0
#endif
#ifndef APP_DEFAULT_NIC_TX_RS_THRESH
#define APP_DEFAULT_NIC_TX_RS_THRESH  0
#endif
/* 软件环大小 */
#ifndef APP_DEFAULT_RING_RX_SIZE
#define APP_DEFAULT_RING_RX_SIZE 1024  //默认接收环缓存大小
#endif
#ifndef APP_DEFAULT_RING_TX_SIZE
#define APP_DEFAULT_RING_TX_SIZE 1024 //默认发送环缓存大小
#endif
/* Bursts */
#ifndef APP_MBUF_ARRAY_SIZE
#define APP_MBUF_ARRAY_SIZE   512  //发送队列的512
#endif
#ifndef APP_DEFAULT_BURST_SIZE_IO_RX_READ
#define APP_DEFAULT_BURST_SIZE_IO_RX_READ  144  //接收读大小
#endif
#if (APP_DEFAULT_BURST_SIZE_IO_RX_READ > APP_MBUF_ARRAY_SIZE)
#error "APP_DEFAULT_BURST_SIZE_IO_RX_READ is too big"
#endif
#ifndef APP_DEFAULT_BURST_SIZE_IO_RX_WRITE
#define APP_DEFAULT_BURST_SIZE_IO_RX_WRITE  144
#endif
#if (APP_DEFAULT_BURST_SIZE_IO_RX_WRITE > APP_MBUF_ARRAY_SIZE)
#error "APP_DEFAULT_BURST_SIZE_IO_RX_WRITE is too big"
#endif
#ifndef APP_DEFAULT_BURST_SIZE_IO_TX_READ
#define APP_DEFAULT_BURST_SIZE_IO_TX_READ  144
#endif
#if (APP_DEFAULT_BURST_SIZE_IO_TX_READ > APP_MBUF_ARRAY_SIZE)
#error "APP_DEFAULT_BURST_SIZE_IO_TX_READ is too big"
#endif
#ifndef APP_DEFAULT_BURST_SIZE_IO_TX_WRITE
#define APP_DEFAULT_BURST_SIZE_IO_TX_WRITE  144
#endif
#if (APP_DEFAULT_BURST_SIZE_IO_TX_WRITE > APP_MBUF_ARRAY_SIZE)
#error "APP_DEFAULT_BURST_SIZE_IO_TX_WRITE is too big"
#endif
#ifndef APP_DEFAULT_BURST_SIZE_WORKER_READ
#define APP_DEFAULT_BURST_SIZE_WORKER_READ  144
#endif
#if ((2 * APP_DEFAULT_BURST_SIZE_WORKER_READ) > APP_MBUF_ARRAY_SIZE)
#error "APP_DEFAULT_BURST_SIZE_WORKER_READ is too big"
#endif
#ifndef APP_DEFAULT_BURST_SIZE_WORKER_WRITE
#define APP_DEFAULT_BURST_SIZE_WORKER_WRITE  144
#endif
#if (APP_DEFAULT_BURST_SIZE_WORKER_WRITE > APP_MBUF_ARRAY_SIZE)
#error "APP_DEFAULT_BURST_SIZE_WORKER_WRITE is too big"
#endif
/* Load balancing logic 加载均衡逻辑 */
#ifndef APP_DEFAULT_IO_RX_LB_POS
#define APP_DEFAULT_IO_RX_LB_POS 29
#endif
#if (APP_DEFAULT_IO_RX_LB_POS >= 64)
#error "APP_DEFAULT_IO_RX_LB_POS is too big"
#endif
//数组结构
struct app_mbuf_array {
    struct rte_mbuf *array[APP_MBUF_ARRAY_SIZE];
    uint32_t n_mbufs; //数据
};
//app逻辑核类型
enum app_lcore_type {
    e_APP_LCORE_DISABLED = 0,
    e_APP_LCORE_IO,
    e_APP_LCORE_WORKER
};
//做io的逻辑核参数结构体
struct app_lcore_params_io {
    /* I/O RX 接收队列*/
    struct {
        /* NIC */
        struct {
            uint8_t port;
            uint8_t queue;
        } nic_queues[APP_MAX_NIC_RX_QUEUES_PER_IO_LCORE]; //网卡可以对多个队列
        uint32_t n_nic_queues; //队列数
        /* Rings */
        struct rte_ring *rings[APP_MAX_WORKER_LCORES]; //没个工作逻辑核对应多个环形队列
        uint32_t n_rings; //环形队列数
        /* Internal buffers 内部缓存*/
        struct app_mbuf_array mbuf_in; //进入的缓存数据包
        struct app_mbuf_array mbuf_out[APP_MAX_WORKER_LCORES]; //出去的换粗你数据包
        uint8_t mbuf_out_flush[APP_MAX_WORKER_LCORES]; //直接转发
        /* Stats */
        uint32_t nic_queues_count[APP_MAX_NIC_RX_QUEUES_PER_IO_LCORE]; //接收队列统计
        uint32_t nic_queues_iters[APP_MAX_NIC_RX_QUEUES_PER_IO_LCORE];//接收队列次数
        uint32_t rings_count[APP_MAX_WORKER_LCORES];
        uint32_t rings_iters[APP_MAX_WORKER_LCORES];
    } rx;
    /* I/O TX 发送 */
    struct {
        /* Rings 环 */
        struct rte_ring *rings[APP_MAX_NIC_PORTS][APP_MAX_WORKER_LCORES];
        /* NIC 转发网卡列表*/
        uint8_t nic_ports[APP_MAX_NIC_TX_PORTS_PER_IO_LCORE];
        uint32_t n_nic_ports;
        /* Internal buffers */
        struct app_mbuf_array mbuf_out[APP_MAX_NIC_TX_PORTS_PER_IO_LCORE]; //发送缓存
        uint8_t mbuf_out_flush[APP_MAX_NIC_TX_PORTS_PER_IO_LCORE]; //直接
        /* Stats */
        uint32_t rings_count[APP_MAX_NIC_PORTS][APP_MAX_WORKER_LCORES];
        uint32_t rings_iters[APP_MAX_NIC_PORTS][APP_MAX_WORKER_LCORES];
        uint32_t nic_ports_count[APP_MAX_NIC_TX_PORTS_PER_IO_LCORE];
        uint32_t nic_ports_iters[APP_MAX_NIC_TX_PORTS_PER_IO_LCORE];
    } tx;
};
//工作者 相关参数
struct app_lcore_params_worker {
    /* Rings 环*/
    struct rte_ring *rings_in[APP_MAX_IO_LCORES];
    uint32_t n_rings_in; //多少个接收环
    struct rte_ring *rings_out[APP_MAX_NIC_PORTS]; 发出换队列
    /* LPM table */
    struct rte_lpm *lpm_table; //LPM结构
    uint32_t worker_id; //逻辑核编号
    /* Internal buffers */
    struct app_mbuf_array mbuf_in; //进入缓存
    struct app_mbuf_array mbuf_out[APP_MAX_NIC_PORTS]; //发送缓存
    uint8_t mbuf_out_flush[APP_MAX_NIC_PORTS]; 直接缓存
    /* Stats */
    uint32_t rings_in_count[APP_MAX_IO_LCORES];
    uint32_t rings_in_iters[APP_MAX_IO_LCORES];
    uint32_t rings_out_count[APP_MAX_NIC_PORTS];
    uint32_t rings_out_iters[APP_MAX_NIC_PORTS];
};
//app运行参数
struct app_lcore_params {
    //连接类型
    union {
        struct app_lcore_params_io io;
        struct app_lcore_params_worker worker;
    };
    //逻辑核类型
    enum app_lcore_type type;
    //内存池
    struct rte_mempool *pool;
} __rte_cache_aligned;
//lpm规则
struct app_lpm_rule {
    uint32_t ip; //ip
    uint8_t depth; //网络号位数
    uint8_t if_out; //出口网卡
};
//整体配置
struct app_params {
    /* 逻辑核配置 */
    struct app_lcore_params lcore_params[APP_MAX_LCORES];
    /* NIC 相关掩码 */
    uint8_t nic_rx_queue_mask[APP_MAX_NIC_PORTS][APP_MAX_RX_QUEUES_PER_NIC_PORT];
    uint8_t nic_tx_port_mask[APP_MAX_NIC_PORTS];
    /* mbuf pools ，每个socket一个内存池*/
    struct rte_mempool *pools[APP_MAX_SOCKETS];
    /* LPM tables 每个socket一个lpm*/
    struct rte_lpm *lpm_tables[APP_MAX_SOCKETS];
    struct app_lpm_rule lpm_rules[APP_MAX_LPM_RULES];
    uint32_t n_lpm_rules;
    /* rings */
    uint32_t nic_rx_ring_size;
    uint32_t nic_tx_ring_size;
    uint32_t ring_rx_size;
    uint32_t ring_tx_size;
    /* burst size */
    uint32_t burst_size_io_rx_read;
    uint32_t burst_size_io_rx_write;
    uint32_t burst_size_io_tx_read;
    uint32_t burst_size_io_tx_write;
    uint32_t burst_size_worker_read;
    uint32_t burst_size_worker_write;
    /* load balancing */
    uint8_t pos_lb;  //这个参数后面详细分析
} __rte_cache_aligned;
extern struct app_params app; //提前声明
//下面是相关API
int app_parse_args(int argc, char **argv); //解析参数
void app_print_usage(void); //打印帮助
void app_init(void); //初始化
int app_lcore_main_loop(void *arg);
int app_get_nic_rx_queues_per_port(uint8_t port);
int app_get_lcore_for_nic_rx(uint8_t port, uint8_t queue, uint32_t *lcore_out);
int app_get_lcore_for_nic_tx(uint8_t port, uint32_t *lcore_out);
int app_is_socket_used(uint32_t socket);
uint32_t app_get_lcores_io_rx(void);
uint32_t app_get_lcores_worker(void);
void app_print_params(void);
#endif /* _MAIN_H_ */
```
**重要的初始化：**
```python
void
app_init(void)
{
    //为每个worker分配 ID
    app_assign_worker_ids();
    //初始化相关内存
    app_init_mbuf_pools();
    //初始化lpm结构
    app_init_lpm_tables();
    //初始化接收\发送环
    app_init_rings_rx();
    app_init_rings_tx();
    //初始化网卡
    app_init_nics();
    printf("Initialization completed.\n");
}
```
**环初始化：**
```python
static void
app_init_rings_rx(void)
{
    unsigned lcore; //无符号类型整形
    /* Initialize the rings for the RX side */
    for (lcore = 0; lcore < APP_MAX_LCORES; lcore ++) {
        struct app_lcore_params_io *lp_io = &app.lcore_params[lcore].io;
        unsigned socket_io, lcore_worker;
        if ((app.lcore_params[lcore].type != e_APP_LCORE_IO) ||
            (lp_io->rx.n_nic_queues == 0)) {
            continue;
        }
        socket_io = rte_lcore_to_socket_id(lcore); //此逻辑核在哪个物理cpu核上
        for (lcore_worker = 0; lcore_worker < APP_MAX_LCORES; lcore_worker ++) {
            char name[32];
            struct app_lcore_params_worker *lp_worker = &app.lcore_params[lcore_worker].worker;
            struct rte_ring *ring = NULL;
            if (app.lcore_params[lcore_worker].type != e_APP_LCORE_WORKER) {
                continue;
            }
            //创建环去连接io逻辑核与工作者逻辑核
            printf("Creating ring to connect I/O lcore %u (socket %u) with worker lcore %u ...\n",
                lcore,
                socket_io,
                lcore_worker);
            snprintf(name, sizeof(name), "app_ring_rx_s%u_io%u_w%u",
                socket_io,
                lcore,
                lcore_worker);
            //创建环RING_F_SP_ENQ | RING_F_SC_DEQ 单生产者单消费者模式
            ring = rte_ring_create(
                name,
                app.ring_rx_size,
                socket_io,
                RING_F_SP_ENQ | RING_F_SC_DEQ);
            if (ring == NULL) {
                rte_panic("Cannot create ring to connect I/O core %u with worker core %u\n",
                    lcore,
                    lcore_worker);
            }
            lp_io->rx.rings[lp_io->rx.n_rings] = ring;
            lp_io->rx.n_rings ++;
            lp_worker->rings_in[lp_worker->n_rings_in] = ring;
            lp_worker->n_rings_in ++;
        }
    }
    for (lcore = 0; lcore < APP_MAX_LCORES; lcore ++) {
        //检查io逻辑核队列数是否与工作者逻辑核匹配
        struct app_lcore_params_io *lp_io = &app.lcore_params[lcore].io;
        if ((app.lcore_params[lcore].type != e_APP_LCORE_IO) ||
            (lp_io->rx.n_nic_queues == 0)) {
            continue;
        }
        if (lp_io->rx.n_rings != app_get_lcores_worker()) {
            rte_panic("Algorithmic error (I/O RX rings)\n");
        }
    }
    for (lcore = 0; lcore < APP_MAX_LCORES; lcore ++) {
        //做相关检查
        struct app_lcore_params_worker *lp_worker = &app.lcore_params[lcore].worker;
        if (app.lcore_params[lcore].type != e_APP_LCORE_WORKER) {
            continue;
        }
        if (lp_worker->n_rings_in != app_get_lcores_io_rx()) {
            rte_panic("Algorithmic error (worker input rings)\n");
        }
    }
}
```
**I/O逻辑：**
```python
static void
app_lcore_main_loop_io(void)
{
    uint32_t lcore = rte_lcore_id(); //获取逻辑核id
    struct app_lcore_params_io *lp = &app.lcore_params[lcore].io; //根据逻辑核找到对应的io结构参数
    uint32_t n_workers = app_get_lcores_worker(); //得到工作队列总数
    uint64_t i = 0;
    uint32_t bsz_rx_rd = app.burst_size_io_rx_read; //相关阀值
    uint32_t bsz_rx_wr = app.burst_size_io_rx_write;
    uint32_t bsz_tx_rd = app.burst_size_io_tx_read;
    uint32_t bsz_tx_wr = app.burst_size_io_tx_write;
    uint8_t pos_lb = app.pos_lb; //负载均衡位置
    for ( ; ; ) {
        //当IO 等于APP_LCORE_IO_FLUSH时，直接io。
        if (APP_LCORE_IO_FLUSH && (unlikely(i == APP_LCORE_IO_FLUSH))) {
            //网卡接收队列大于0时，就可以直接收包
            if (likely(lp->rx.n_nic_queues > 0)) {
                app_lcore_io_rx_flush(lp, n_workers);
            }
            //网卡发送队列大于0时，就可以考虑直接发包
            if (likely(lp->tx.n_nic_ports > 0)) {
                app_lcore_io_tx_flush(lp);
            }
            i = 0;
        }
        //网卡接收队列大于0时，就可以考虑收包
        if (likely(lp->rx.n_nic_queues > 0)) {
            app_lcore_io_rx(lp, n_workers, bsz_rx_rd, bsz_rx_wr, pos_lb);
        }
        //网卡发送队列大于0时，就可以考虑发包
        if (likely(lp->tx.n_nic_ports > 0)) {
            app_lcore_io_tx(lp, n_workers, bsz_tx_rd, bsz_tx_wr);
        }
        i ++;
    }
}
```
**处理1：**
```python
static inline void
app_lcore_io_rx(
    struct app_lcore_params_io *lp,
    uint32_t n_workers,
    uint32_t bsz_rd,
    uint32_t bsz_wr,
    uint8_t pos_lb)
{
    struct rte_mbuf *mbuf_1_0, *mbuf_1_1, *mbuf_2_0, *mbuf_2_1;
    uint8_t *data_1_0, *data_1_1 = NULL;
    uint32_t i;
    //遍历接收队列，处理数据包
    for (i = 0; i < lp->rx.n_nic_queues; i ++) {
        uint8_t port = lp->rx.nic_queues[i].port; //网口
        uint8_t queue = lp->rx.nic_queues[i].queue; //队列
        uint32_t n_mbufs, j;
        
        //接收多少包
        n_mbufs = rte_eth_rx_burst(
            port, //端口
            queue, //队列
            lp->rx.mbuf_in.array,
            (uint16_t) bsz_rd);
        //等于0  没数据包
        if (unlikely(n_mbufs == 0)) {
            continue;
        }
        //相关统计
#if APP_STATS
        lp->rx.nic_queues_iters[i] ++; //多少次收到
        lp->rx.nic_queues_count[i] += n_mbufs; //收了多少包
        if (unlikely(lp->rx.nic_queues_iters[i] == APP_STATS)) {
            struct rte_eth_stats stats; //统计
            unsigned lcore = rte_lcore_id(); //逻辑核
            rte_eth_stats_get(port, &stats); //调用接口读网卡相关统计
            printf("I/O RX %u in (NIC port %u): NIC drop ratio = %.2f avg burst size = %.2f\n",
                lcore,
                (unsigned) port,
                (double) stats.imissed / (double) (stats.imissed + stats.ipackets),
                ((double) lp->rx.nic_queues_count[i]) / ((double) lp->rx.nic_queues_iters[i]));
            lp->rx.nic_queues_iters[i] = 0;
            lp->rx.nic_queues_count[i] = 0;
        }
#endif
        //删除所有包
#if APP_IO_RX_DROP_ALL_PACKETS
        for (j = 0; j < n_mbufs; j ++) {
            struct rte_mbuf *pkt = lp->rx.mbuf_in.array[j];
            rte_pktmbuf_free(pkt);
        }
        continue;
#endif
        mbuf_1_0 = lp->rx.mbuf_in.array[0]; //第一个包指针
        mbuf_1_1 = lp->rx.mbuf_in.array[1]; //第二个包指针
        data_1_0 = rte_pktmbuf_mtod(mbuf_1_0, uint8_t *); //第一个包数据部分
        if (likely(n_mbufs > 1)) { ///有第二个包
            data_1_1 = rte_pktmbuf_mtod(mbuf_1_1, uint8_t *);
        }
        mbuf_2_0 = lp->rx.mbuf_in.array[2]; //第三个包指针
        mbuf_2_1 = lp->rx.mbuf_in.array[3]; //第四个包指针
        APP_IO_RX_PREFETCH0(mbuf_2_0);     //预取
        APP_IO_RX_PREFETCH0(mbuf_2_1);         //预取
        for (j = 0; j + 3 < n_mbufs; j += 2) {
            struct rte_mbuf *mbuf_0_0, *mbuf_0_1;
            uint8_t *data_0_0, *data_0_1;
            uint32_t worker_0, worker_1;
            mbuf_0_0 = mbuf_1_0; //第一个数据包
            mbuf_0_1 = mbuf_1_1; //第二个数据包
            data_0_0 = data_1_0;  //第一个包数据
            data_0_1 = data_1_1;    //第二个包数据
            mbuf_1_0 = mbuf_2_0;
            mbuf_1_1 = mbuf_2_1;
            data_1_0 = rte_pktmbuf_mtod(mbuf_2_0, uint8_t *); //取数据
            data_1_1 = rte_pktmbuf_mtod(mbuf_2_1, uint8_t *); //取数据
            APP_IO_RX_PREFETCH0(data_1_0);
            APP_IO_RX_PREFETCH0(data_1_1);
            mbuf_2_0 = lp->rx.mbuf_in.array[j+4]; //第四个包
            mbuf_2_1 = lp->rx.mbuf_in.array[j+5]; //第五个包
            APP_IO_RX_PREFETCH0(mbuf_2_0);
            APP_IO_RX_PREFETCH0(mbuf_2_1);
            
            //pos_lb根据数据包这位进行与运算，这个种算法进行负载
            worker_0 = data_0_0[pos_lb] & (n_workers - 1);
            worker_1 = data_0_1[pos_lb] & (n_workers - 1);
            app_lcore_io_rx_buffer_to_send(lp, worker_0, mbuf_0_0, bsz_wr);
            app_lcore_io_rx_buffer_to_send(lp, worker_1, mbuf_0_1, bsz_wr);
        }
        /* Handle the last 1, 2 (when n_mbufs is even) or 3 (when n_mbufs is odd) packets  当n_mbufs为偶数或者为3时。*/
        for ( ; j < n_mbufs; j += 1) {
            struct rte_mbuf *mbuf;
            uint8_t *data;
            uint32_t worker;
            mbuf = mbuf_1_0;
            mbuf_1_0 = mbuf_1_1;
            mbuf_1_1 = mbuf_2_0;
            mbuf_2_0 = mbuf_2_1;
            data = rte_pktmbuf_mtod(mbuf, uint8_t *);
            APP_IO_RX_PREFETCH0(mbuf_1_0);
            worker = data[pos_lb] & (n_workers - 1);
            app_lcore_io_rx_buffer_to_send(lp, worker, mbuf, bsz_wr);
        }
    }
}
```
**处理2：**
```python
static inline void
app_lcore_io_rx_buffer_to_send (
    struct app_lcore_params_io *lp,
    uint32_t worker,
    struct rte_mbuf *mbuf,
    uint32_t bsz)
{
    uint32_t pos;
    int ret;
    pos = lp->rx.mbuf_out[worker].n_mbufs; //多少个包
    lp->rx.mbuf_out[worker].array[pos ++] = mbuf;
    if (likely(pos < bsz)) {
        lp->rx.mbuf_out[worker].n_mbufs = pos;
        return;
    }
    //将数据包放入worker环中
    ret = rte_ring_sp_enqueue_bulk(
        lp->rx.rings[worker],
        (void **) lp->rx.mbuf_out[worker].array,
        bsz);
    if (unlikely(ret == -ENOBUFS)) {
        //没内存
        uint32_t k;
        for (k = 0; k < bsz; k ++) {
            struct rte_mbuf *m = lp->rx.mbuf_out[worker].array[k];
            rte_pktmbuf_free(m);
        }
    }
    //放入队列后清空
    lp->rx.mbuf_out[worker].n_mbufs = 0;
    lp->rx.mbuf_out_flush[worker] = 0;
#if APP_STATS
    //统计
    lp->rx.rings_iters[worker] ++;
    if (likely(ret == 0)) {
        lp->rx.rings_count[worker] ++;
    }
    if (unlikely(lp->rx.rings_iters[worker] == APP_STATS)) {
        unsigned lcore = rte_lcore_id();
        printf("\tI/O RX %u out (worker %u): enq success rate = %.2f\n",
            lcore,
            (unsigned)worker,
            ((double) lp->rx.rings_count[worker]) / ((double) lp->rx.rings_iters[worker]));
        lp->rx.rings_iters[worker] = 0;
        lp->rx.rings_count[worker] = 0;
    }
#endif
}
```
**处理3：**
```python
static inline void
app_lcore_io_tx(
    struct app_lcore_params_io *lp,
    uint32_t n_workers,
    uint32_t bsz_rd,
    uint32_t bsz_wr)
{
    uint32_t worker;
    //遍历工作线程，每个工作缓存情况
    for (worker = 0; worker < n_workers; worker ++) {
        uint32_t i;
        //这个转发网卡队列数
        for (i = 0; i < lp->tx.n_nic_ports; i ++) {
            uint8_t port = lp->tx.nic_ports[i]; //获取此端口的转发
            struct rte_ring *ring = lp->tx.rings[port][worker];//这个网口与工作线程的ring buffer
            uint32_t n_mbufs, n_pkts;
            int ret;
            n_mbufs = lp->tx.mbuf_out[port].n_mbufs;//将环形队列出队到发送缓存中
            ret = rte_ring_sc_dequeue_bulk(
                ring,  
                (void **) &lp->tx.mbuf_out[port].array[n_mbufs],
                bsz_rd);
            if (unlikely(ret == -ENOENT)) {
                continue;
            }
            n_mbufs += bsz_rd;
#if APP_IO_TX_DROP_ALL_PACKETS
    删除所有包
            {
                uint32_t j;
                APP_IO_TX_PREFETCH0(lp->tx.mbuf_out[port].array[0]);
                APP_IO_TX_PREFETCH0(lp->tx.mbuf_out[port].array[1]);
                for (j = 0; j < n_mbufs; j ++) {
                    if (likely(j < n_mbufs - 2)) {
                        APP_IO_TX_PREFETCH0(lp->tx.mbuf_out[port].array[j + 2]);
                    }
                    rte_pktmbuf_free(lp->tx.mbuf_out[port].array[j]);
                }
                lp->tx.mbuf_out[port].n_mbufs = 0;
                continue;
            }
#endif
            if (unlikely(n_mbufs < bsz_wr)) {
                lp->tx.mbuf_out[port].n_mbufs = n_mbufs;
                continue;
            }
            //发送数据包
            n_pkts = rte_eth_tx_burst(
                port,
                0,
                lp->tx.mbuf_out[port].array,
                (uint16_t) n_mbufs);
#if APP_STATS
            lp->tx.nic_ports_iters[port] ++;
            lp->tx.nic_ports_count[port] += n_pkts;
            if (unlikely(lp->tx.nic_ports_iters[port] == APP_STATS)) {
                unsigned lcore = rte_lcore_id();
                printf("\t\t\tI/O TX %u out (port %u): avg burst size = %.2f\n",
                    lcore,
                    (unsigned) port,
                    ((double) lp->tx.nic_ports_count[port]) / ((double) lp->tx.nic_ports_iters[port]));
                lp->tx.nic_ports_iters[port] = 0;
                lp->tx.nic_ports_count[port] = 0;
            }
#endif
            //删除
            if (unlikely(n_pkts < n_mbufs)) {
                uint32_t k;
                for (k = n_pkts; k < n_mbufs; k ++) {
                    struct rte_mbuf *pkt_to_free = lp->tx.mbuf_out[port].array[k];
                    rte_pktmbuf_free(pkt_to_free);
                }
            }
            lp->tx.mbuf_out[port].n_mbufs = 0;
            lp->tx.mbuf_out_flush[port] = 0;
        }
    }
}
```
**Worker逻辑：**
**处理1：**
```python
static void
app_lcore_main_loop_worker(void) {
    uint32_t lcore = rte_lcore_id(); //逻辑号
    struct app_lcore_params_worker *lp = &app.lcore_params[lcore].worker; //工作者逻辑核参数
    uint64_t i = 0;
    uint32_t bsz_rd = app.burst_size_worker_read;
    uint32_t bsz_wr = app.burst_size_worker_write;
    for ( ; ; ) {
        if (APP_LCORE_WORKER_FLUSH && (unlikely(i == APP_LCORE_WORKER_FLUSH))) {
            app_lcore_worker_flush(lp); //至二级发送
            i = 0;
        }
        app_lcore_worker(lp, bsz_rd, bsz_wr); /
        i ++;
    }
}
```
**处理2：**
```python
static inline void
app_lcore_worker(
    struct app_lcore_params_worker *lp,
    uint32_t bsz_rd,
    uint32_t bsz_wr)
{
    uint32_t i;
    //包进入缓冲环，遍历此逻辑核负责的环形缓冲
    for (i = 0; i < lp->n_rings_in; i ++) {
        struct rte_ring *ring_in = lp->rings_in[i];
        uint32_t j;
        int ret;
        //将环形缓冲中的包出队到缓冲区
        ret = rte_ring_sc_dequeue_bulk(
            ring_in, //环形缓冲
            (void **) lp->mbuf_in.array, //数据包缓存
            bsz_rd);
        if (unlikely(ret == -ENOENT)) {
            continue;
        }
#if APP_WORKER_DROP_ALL_PACKETS
        //删除包
        for (j = 0; j < bsz_rd; j ++) {
            struct rte_mbuf *pkt = lp->mbuf_in.array[j];
            rte_pktmbuf_free(pkt);
        }
        continue;
#endif
        APP_WORKER_PREFETCH1(rte_pktmbuf_mtod(lp->mbuf_in.array[0], unsigned char *));
        APP_WORKER_PREFETCH0(lp->mbuf_in.array[1]);
        for (j = 0; j < bsz_rd; j ++) {
            struct rte_mbuf *pkt; // 包
            struct ipv4_hdr *ipv4_hdr; //ip头部结构
            uint32_t ipv4_dst, pos;
            uint32_t port;
            if (likely(j < bsz_rd - 1)) {
                APP_WORKER_PREFETCH1(rte_pktmbuf_mtod(lp->mbuf_in.array[j+1], unsigned char *));
            }
            if (likely(j < bsz_rd - 2)) {
                APP_WORKER_PREFETCH0(lp->mbuf_in.array[j+2]);
            }
            pkt = lp->mbuf_in.array[j];
            ipv4_hdr = rte_pktmbuf_mtod_offset(pkt,
                               struct ipv4_hdr *,
                               sizeof(struct ether_hdr));
            ipv4_dst = rte_be_to_cpu_32(ipv4_hdr->dst_addr); //目的ip地址
            //查表
            if (unlikely(rte_lpm_lookup(lp->lpm_table, ipv4_dst, &port) != 0)) {
                port = pkt->port; //出口网口
            }
            pos = lp->mbuf_out[port].n_mbufs;
            //放入发送缓存
            lp->mbuf_out[port].array[pos ++] = pkt;
            if (likely(pos < bsz_wr)) {
                lp->mbuf_out[port].n_mbufs = pos;
                continue;
            }
            //将发送缓存中的数据包放入环形队列
            ret = rte_ring_sp_enqueue_bulk(
                lp->rings_out[port],
                (void **) lp->mbuf_out[port].array,
                bsz_wr);
#if APP_STATS
            //统计
            lp->rings_out_iters[port] ++;
            if (ret == 0) {
                lp->rings_out_count[port] += 1;
            }
            if (lp->rings_out_iters[port] == APP_STATS){
                printf("\t\tWorker %u out (NIC port %u): enq success rate = %.2f\n",
                    (unsigned) lp->worker_id,
                    (unsigned) port,
                    ((double) lp->rings_out_count[port]) / ((double) lp->rings_out_iters[port]));
                lp->rings_out_iters[port] = 0;
                lp->rings_out_count[port] = 0;
            }
#endif
            if (unlikely(ret == -ENOBUFS)) {
                uint32_t k;
                for (k = 0; k < bsz_wr; k ++) {
                    struct rte_mbuf *pkt_to_free = lp->mbuf_out[port].array[k];
                    rte_pktmbuf_free(pkt_to_free);
                }
            }
            lp->mbuf_out[port].n_mbufs = 0;
            lp->mbuf_out_flush[port] = 0;
        }
    }
}
```
0x05 总结
此例子使用了环形缓存队列，进行个个环境的缓存，这个环形队列的大小等参数比较关键。



