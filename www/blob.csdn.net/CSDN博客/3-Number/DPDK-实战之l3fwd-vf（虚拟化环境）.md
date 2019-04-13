
# DPDK-实战之l3fwd-vf（虚拟化环境） - 3-Number - CSDN博客


2017年10月19日 10:45:34[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：3281


# 0x01 缘由
年初，学习了l3fwd的例子，进行三层转发，现在学习下特定转发，这个在虚拟化设备中常见，也是加强对dpdk的熟悉。
# 0x02 介绍
在虚拟化环境下的L3层转发应用是用DPDK处理数据包的一个简单例子。这个L3层转发应用充分发挥SR-IOV的特征。
这个应用演示如何使用hash和LPM DPDK库去实现数据包转发。这个转发策略是基于输入包的信息。
查询方法既有基于hash的也有基于LPM的，你如何选择在编译的时候决定。当所选择的查找方法是基于hash时，使用hash对象来模拟流分类阶段。 哈希对象与流表相关使用，以便在运行时将每个输入数据包映射到其流。
哈希查询键由DiffServ 5元组表示，它由从输入数据包读取的以下字段组成：源IP地址，目标IP地址，协议，源端口和目标端口。 从识别的流表条目读取输入数据包的输出接口的ID。应用程序使用的一组流在初始化时静态配置并加载到散列中。 当选择的查找方法是基于LPM时，使用LPM对象来模拟IPv4分组的转发阶段。
 LPM对象用作路由表，用于在运行时识别每个输入数据包的下一跳。
LPM查找键由从输入数据包读取的目标IP地址字段表示。 输入数据包的输出接口的ID是LPM查询返回的下一跳。 应用程序使用的一组LPM规则在初始化时被静态配置并加载到LPM对象中。
# 0x03 运行环境
**运行参数样板：**
./build/l3fwd-vf [EAL options] -- -p PORTMASK --config(port,queue,lcore)[,(port,queue,lcore)]
**参数解释：**
[EAL options] EAL选项已经在其他例子中已有说明。
-p [端口掩码 -- 网卡掩码，在部署DPDK运行环境时绑定的网卡]： 十六进制表示
--config: 哪个网口哪个队列绑定到哪个逻辑核
–no-numa： 是否关闭对numa结构的识别
**运行实例：**
./l3fwd-vf -l 1,2 -n 4 -- -p 0x3 --config="(0,0,1),(1,0,2)"
-这个实例占用的逻辑核为1,2
-启动1、2号网卡
-端口映射关系为：
0号网卡0号队列映射到1号逻辑核上；
1号网卡0号队列映射到2号逻辑核上；
运行效果如下（添加了一些提示信息）：
**LPM作为路由查询表：**
```python
Initializing port 0 ... Creating queues: nb_rxq=1 nb_txq=1...  Address:A0:36:9F:03:A8:CA, Allocated mbuf pool on socket 0
LPM: Adding route 0x01010100 / 24 (0)
LPM: Adding route 0x02010100 / 24 (1)
LPM: Adding route 0x03010100 / 24 (2)
LPM: Adding route 0x04010100 / 24 (3)
LPM: Adding route 0x05010100 / 24 (4)
LPM: Adding route 0x06010100 / 24 (5)
LPM: Adding route 0x07010100 / 24 (6)
LPM: Adding route 0x08010100 / 24 (7)
txq=0,0,0 PMD: eth_igb_tx_queue_setup(): sw_ring=0x7ffc3814d2c0 hw_ring=0x7ffc3814f300 dma_addr=0x6734f300
Initializing port 1 ... Creating queues: nb_rxq=1 nb_txq=1...  Address:A0:36:9F:03:A8:CB, txq=1,0,0 PMD: eth_igb_tx_queue_setup(): sw_ring=0x
7ffc3813b040 hw_ring=0x7ffc3813d080 dma_addr=0x6733d080
Initializing rx queues on lcore 1 ... rxq=0,0,0 PMD: eth_igb_rx_queue_setup(): sw_ring=0x7ffc3812aac0 hw_ring=0x7ffc3812af00 dma_addr=0x6732a
f00
Initializing rx queues on lcore 2 ... rxq=1,0,0 PMD: eth_igb_rx_queue_setup(): sw_ring=0x7ffc3811a540 hw_ring=0x7ffc3811a980 dma_addr=0x6731a
980
PMD: eth_igb_start(): <<
done: Port 0
PMD: eth_igb_start(): <<
done: Port 1
L3FWD: entering main loop on lcore 2
L3FWD:  -- lcoreid=2 portid=1 rxqueueid=0
L3FWD: entering main loop on lcore 1
L3FWD:  -- lcoreid=1 portid=0 rxqueueid=0
```
**Hash作为路由查询表：**
```python
Initializing port 0 ... Creating queues: nb_rxq=1 nb_txq=1...  Address:A0:36:9F:03:A8:CA, Allocated mbuf pool on socket 0
Hash: Adding key
IP dst = 640a0001, IP src = c80a0001, port dst = 101, port src = 11, proto = 6
Hash: Adding key
IP dst = 64140002, IP src = c8140002, port dst = 102, port src = 12, proto = 6
Hash: Adding key
IP dst = 641e0003, IP src = c81e0003, port dst = 103, port src = 13, proto = 6
Hash: Adding key
IP dst = 64280004, IP src = c8280004, port dst = 104, port src = 14, proto = 6
txq=0,0,0 PMD: eth_igb_tx_queue_setup(): sw_ring=0x7fd48af7fcc0 hw_ring=0x7fd48af81d00 dma_addr=0x67381d00
Initializing port 1 ... Creating queues: nb_rxq=1 nb_txq=1...  Address:A0:36:9F:03:A8:CB, txq=1,0,0 PMD: eth_igb_tx_queue_setup(): sw_ring=0x
7fd48af6da40 hw_ring=0x7fd48af6fa80 dma_addr=0x6736fa80
Initializing rx queues on lcore 1 ... rxq=0,0,0 PMD: eth_igb_rx_queue_setup(): sw_ring=0x7fd48af5d4c0 hw_ring=0x7fd48af5d900 dma_addr=0x6735d
900
Initializing rx queues on lcore 2 ... rxq=1,0,0 PMD: eth_igb_rx_queue_setup(): sw_ring=0x7fd48af4cf40 hw_ring=0x7fd48af4d380 dma_addr=0x6734d
380
PMD: eth_igb_start(): <<
done: Port 0
PMD: eth_igb_start(): <<
done: Port 1
L3FWD: entering main loop on lcore 2
L3FWD:  -- lcoreid=2 portid=1 rxqueueid=0
L3FWD: entering main loop on lcore 1
L3FWD:  -- lcoreid=1 portid=0 rxqueueid=0
```
**模拟发包去查询**（添加一些实际输出）：
绑定两个网口0，1,连接到另一台服务器，另一台服务器抓包，分析数据包修改的情况，运行效果如下：
![](https://img-blog.csdn.net/20171019104615487?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**数据包分析：**
![](https://img-blog.csdn.net/20171019104632002?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
# 0x04 整体源码分析
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
#include <signal.h>
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
#include <rte_spinlock.h>
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
#include <rte_udp.h>
#include <rte_string_fns.h>
#define APP_LOOKUP_EXACT_MATCH          0     //精确匹配算法 
#define APP_LOOKUP_LPM                  1     //最长匹配算法
#define DO_RFC_1812_CHECKS                    //校验检查 如ip头的合法性
//#define APP_LOOKUP_METHOD             APP_LOOKUP_EXACT_MATCH  //使用何种查询表
#ifndef APP_LOOKUP_METHOD
#define APP_LOOKUP_METHOD             APP_LOOKUP_LPM
#endif
/* 下面是一些相关头文件 */
#if (APP_LOOKUP_METHOD == APP_LOOKUP_EXACT_MATCH)
#include <rte_hash.h>
#elif (APP_LOOKUP_METHOD == APP_LOOKUP_LPM)
#include <rte_lpm.h>
#else
#error "APP_LOOKUP_METHOD set to incorrect value"
#endif
#define RTE_LOGTYPE_L3FWD RTE_LOGTYPE_USER1
/* 内存池缓存大小 */
#define MEMPOOL_CACHE_SIZE 256
/*
 * 该表达式用于根据用户输入计算所需的mbufs数，考虑到rx和tx硬件环的内存，每lcore的缓存和每个端口的mtable .
 * RTE_MAX用于确保NB_MBUF不会低于最小值 8192。
 */
#define NB_MBUF RTE_MAX	(																	\
				(nb_ports*nb_rx_queue*RTE_TEST_RX_DESC_DEFAULT +							\
				nb_ports*nb_lcores*MAX_PKT_BURST +											\
				nb_ports*n_tx_queue*RTE_TEST_TX_DESC_DEFAULT +								\
				nb_lcores*MEMPOOL_CACHE_SIZE),												\
				(unsigned)8192)
/*
 *应优先设置RX和TX预取，主机和写回阈值以获得最佳性能。 请参阅网络控制器的数据表和支持DPDK文档，
 *了解如何设置这些参数的指导。
 */
#define RX_PTHRESH 8 /**< Default values of RX prefetch threshold reg. */
#define RX_HTHRESH 8 /**< Default values of RX host threshold reg. */
#define RX_WTHRESH 4 /**< Default values of RX write-back threshold reg. */
/*
 *这些默认值经过优化，可与Intel（R）82599 10 GbE控制器和DPDK ixgbe PMD配合使用。 考虑为其他网络控
 *制器和/或网络驱动程序使用其他值。
 */
#define TX_PTHRESH 36 /**< Default values of TX prefetch threshold reg. */
#define TX_HTHRESH 0  /**< Default values of TX host threshold reg. */
#define TX_WTHRESH 0  /**< Default values of TX write-back threshold reg. */
#define MAX_PKT_BURST 32
#define BURST_TX_DRAIN_US 100 /* TX drain every ~100us */
#define NB_SOCKETS 8
#define SOCKET0 0
/* 在读取数据包时，配置预取数据包数。 */
#define PREFETCH_OFFSET	3
/*
 * 可配置的RX / TX环形描述符数量。
 */
#define RTE_TEST_RX_DESC_DEFAULT 128
#define RTE_TEST_TX_DESC_DEFAULT 512
static uint16_t nb_rxd = RTE_TEST_RX_DESC_DEFAULT;
static uint16_t nb_txd = RTE_TEST_TX_DESC_DEFAULT;
/* 网口的地址 */
static struct ether_addr ports_eth_addr[RTE_MAX_ETHPORTS];
/* mask of enabled ports */
static uint32_t enabled_port_mask = 0;
static int numa_on = 1; /**< 默认启动numa特性 */
/* mbuf表*/
struct mbuf_table {
	uint16_t len;  //长度
	struct rte_mbuf *m_table[MAX_PKT_BURST];//对应的rte_mbuf结构指针数组。
};
/* 与该逻辑核关联的网口和队列id */
struct lcore_rx_queue {
	uint8_t port_id;  //网口ID
	uint8_t queue_id; //接收队列ID
} __rte_cache_aligned;
#define MAX_RX_QUEUE_PER_LCORE 16  //每个逻辑核最多有16个接收队列
#define MAX_TX_QUEUE_PER_PORT 1  //每个网口一个发送队列
#define MAX_RX_QUEUE_PER_PORT 1  //每个网口一个接受队列
#define MAX_LCORE_PARAMS 1024     //最多1024个lcore参数
struct lcore_params {     
	uint8_t port_id;    //网口id
	uint8_t queue_id;   //队列id
	uint8_t lcore_id;   //逻辑核id
} __rte_cache_aligned;
static struct lcore_params lcore_params_array[MAX_LCORE_PARAMS]; //静态全局逻辑核参数
//当未配置时使用默认值
static struct lcore_params lcore_params_array_default[] = {  
	{0, 0, 2},
	{0, 1, 2},
	{0, 2, 2},
	{1, 0, 2},
	{1, 1, 2},
	{1, 2, 2},
	{2, 0, 2},
	{3, 0, 3},
	{3, 1, 3},
};
static struct lcore_params * lcore_params = lcore_params_array_default; //使用一个全局指针
//配置多少个参数
static uint16_t nb_lcore_params = sizeof(lcore_params_array_default) /
				sizeof(lcore_params_array_default[0]);
//网口的一些属性设置，如模式，如巨型侦的处理。
static struct rte_eth_conf port_conf = {
	.rxmode = {
		.mq_mode	= ETH_MQ_RX_RSS,
		.max_rx_pkt_len = ETHER_MAX_LEN,
		.split_hdr_size = 0,
		.header_split   = 0, /**< Header Split disabled */
		.hw_ip_checksum = 1, /**< IP checksum offload enabled */
		.hw_vlan_filter = 0, /**< VLAN filtering disabled */
		.jumbo_frame    = 0, /**< Jumbo Frame Support disabled */
		.hw_strip_crc   = 0, /**< CRC stripped by hardware */
	},
	.rx_adv_conf = {
		.rss_conf = {
			.rss_key = NULL,
			.rss_hf = ETH_RSS_IP,
		},
	},
	.txmode = {
		.mq_mode = ETH_MQ_TX_NONE,
	},
};
//内存池指针，对应物理CPU颗数。
static struct rte_mempool * pktmbuf_pool[NB_SOCKETS];

//下面是用hash的方式进行精确匹配
#if (APP_LOOKUP_METHOD == APP_LOOKUP_EXACT_MATCH)
#ifdef RTE_MACHINE_CPUFLAG_SSE4_2
#include <rte_hash_crc.h>
#define DEFAULT_HASH_FUNC       rte_hash_crc
#else
#include <rte_jhash.h>
#define DEFAULT_HASH_FUNC       rte_jhash
#endif
//五元组
struct ipv4_5tuple {
	uint32_t ip_dst; //目的IP地址
	uint32_t ip_src; //源IP地址
	uint16_t port_dst; //目的端口
	uint16_t port_src; //源端口
	uint8_t proto;  //协议类型
} __attribute__((__packed__));
//三层转发路由
struct l3fwd_route {
	struct ipv4_5tuple key; //五元组key
	uint8_t if_out;  //出口网口
};
//配置hash 路由参数
static struct l3fwd_route l3fwd_route_array[] = {
	{{IPv4(100,10,0,1), IPv4(200,10,0,1), 101, 11, IPPROTO_TCP}, 0},
	{{IPv4(100,20,0,2), IPv4(200,20,0,2), 102, 12, IPPROTO_TCP}, 1},
	{{IPv4(100,30,0,3), IPv4(200,30,0,3), 103, 13, IPPROTO_TCP}, 2},
	{{IPv4(100,40,0,4), IPv4(200,40,0,4), 104, 14, IPPROTO_TCP}, 3},
};
typedef struct rte_hash lookup_struct_t;  //rte hash结构
static lookup_struct_t *l3fwd_lookup_struct[NB_SOCKETS]; //申请指针数组
#define L3FWD_HASH_ENTRIES	1024
//hash 参数
struct rte_hash_parameters l3fwd_hash_params = {
	.name = "l3fwd_hash_0",
	.entries = L3FWD_HASH_ENTRIES,
	.key_len = sizeof(struct ipv4_5tuple),
	.hash_func = DEFAULT_HASH_FUNC,
	.hash_func_init_val = 0,
	.socket_id = SOCKET0,
};
#define L3FWD_NUM_ROUTES \
	(sizeof(l3fwd_route_array) / sizeof(l3fwd_route_array[0]))
static uint8_t l3fwd_out_if[L3FWD_HASH_ENTRIES] __rte_cache_aligned;
#endif
//LPM 相关结构和初始化
#if (APP_LOOKUP_METHOD == APP_LOOKUP_LPM)
//三层路由结构
struct l3fwd_route {
	uint32_t ip;  //ip
	uint8_t  depth; //深度
	uint8_t  if_out; //出口
};
//相关参数
static struct l3fwd_route l3fwd_route_array[] = {
	{IPv4(1,1,1,0), 24, 0},
	{IPv4(2,1,1,0), 24, 1},
	{IPv4(3,1,1,0), 24, 2},
	{IPv4(4,1,1,0), 24, 3},
	{IPv4(5,1,1,0), 24, 4},
	{IPv4(6,1,1,0), 24, 5},
	{IPv4(7,1,1,0), 24, 6},
	{IPv4(8,1,1,0), 24, 7},
};
//理由参数个数
#define L3FWD_NUM_ROUTES \
	(sizeof(l3fwd_route_array) / sizeof(l3fwd_route_array[0]))
//规则个数
#define L3FWD_LPM_MAX_RULES     1024
typedef struct rte_lpm lookup_struct_t;
static lookup_struct_t *l3fwd_lookup_struct[NB_SOCKETS];
#endif
//逻辑核配置
struct lcore_conf {
	uint16_t n_rx_queue; //接收队列个数
	struct lcore_rx_queue rx_queue_list[MAX_RX_QUEUE_PER_LCORE]; //接收队列配置数组
	uint16_t tx_queue_id; //发送队列id
	struct mbuf_table tx_mbufs[RTE_MAX_ETHPORTS]; //数据包缓存
	lookup_struct_t * lookup_struct; //查询结构
} __rte_cache_aligned;
static struct lcore_conf lcore_conf[RTE_MAX_LCORE]; //逻辑核参数
static rte_spinlock_t spinlock_conf[RTE_MAX_ETHPORTS] = {RTE_SPINLOCK_INITIALIZER}; //自旋锁，来保证对一个网口竞争访问；
/* 在一个发送网口发送数据包 */
static inline int
send_burst(struct lcore_conf *qconf, uint16_t n, uint8_t port)
{
	struct rte_mbuf **m_table; //包结构表
	int ret;
	uint16_t queueid;
	queueid = qconf->tx_queue_id;
	m_table = (struct rte_mbuf **)qconf->tx_mbufs[port].m_table;
	rte_spinlock_lock(&spinlock_conf[port]); //上锁（在这盘旋）
	ret = rte_eth_tx_burst(port, queueid, m_table, n); //发送数据
	rte_spinlock_unlock(&spinlock_conf[port]); //释放锁，其他线程可以访问
	if (unlikely(ret < n)) {
		do {
			rte_pktmbuf_free(m_table[ret]); //释放结构
		} while (++ret < n);
	}
	return 0;
}
/* 将一个包插入队列，当队列满时将数据发送。*/
static inline int
send_single_packet(struct rte_mbuf *m, uint8_t port)
{
	uint32_t lcore_id;
	uint16_t len;
	struct lcore_conf *qconf;
	lcore_id = rte_lcore_id(); //获取当前线程处于哪个逻辑核上
	qconf = &lcore_conf[lcore_id]; //根据对应的逻辑核找对对应的配置
	len = qconf->tx_mbufs[port].len;  //此时发送队列中的包个数
	qconf->tx_mbufs[port].m_table[len] = m; //将对应的数据放到对应的位置
	len++;        //包长度加1
	/* 当队列数据包达到了MAX_PKT_BURST ，则立刻发送 */
	if (unlikely(len == MAX_PKT_BURST)) {
		send_burst(qconf, MAX_PKT_BURST, port); //发送对应的数据包
		len = 0; //长度置0
	}
	qconf->tx_mbufs[port].len = len; //这个网口的发送队列置0
	return 0;
}
#ifdef DO_RFC_1812_CHECKS
//此处做相关校验。
static inline int
is_valid_ipv4_pkt(struct ipv4_hdr *pkt, uint32_t link_len)
{
	/* From http://www.rfc-editor.org/rfc/rfc1812.txt section 5.2.2 */
	/*
	 * 1. The packet length reported by the Link Layer must be large
	 * enough to hold the minimum length legal IP datagram (20 bytes).
	 */
	if (link_len < sizeof(struct ipv4_hdr))
		return -1;
	/* 2. The IP checksum must be correct. */
	/* this is checked in H/W */
	/*
	 * 3. The IP version number must be 4. If the version number is not 4
	 * then the packet may be another version of IP, such as IPng or
	 * ST-II.
	 */
	if (((pkt->version_ihl) >> 4) != 4)
		return -3;
	/*
	 * 4. The IP header length field must be large enough to hold the
	 * minimum length legal IP datagram (20 bytes = 5 words).
	 */
	if ((pkt->version_ihl & 0xf) < 5)
		return -4;
	/*
	 * 5. The IP total length field must be large enough to hold the IP
	 * datagram header, whose length is specified in the IP header length
	 * field.
	 */
	if (rte_cpu_to_be_16(pkt->total_length) < sizeof(struct ipv4_hdr))
		return -5;
	return 0;
}
#endif
#if (APP_LOOKUP_METHOD == APP_LOOKUP_EXACT_MATCH)
static void
print_key(struct ipv4_5tuple key)
{
	//打印5元组
	printf("IP dst = %08x, IP src = %08x, port dst = %d, port src = %d, proto = %d\n",
	       (unsigned)key.ip_dst, (unsigned)key.ip_src, key.port_dst, key.port_src, key.proto);
}
//得到转发网口
static inline uint8_t
get_dst_port(struct ipv4_hdr *ipv4_hdr,  uint8_t portid, lookup_struct_t * l3fwd_lookup_struct)
{
	struct ipv4_5tuple key; //五元组key
	struct tcp_hdr *tcp;  //tcp 头部结构
	struct udp_hdr *udp;  //udp 头部结构
	int ret = 0;
	//将整数值从大端模式转化到cpu序列
	key.ip_dst = rte_be_to_cpu_32(ipv4_hdr->dst_addr);
	key.ip_src = rte_be_to_cpu_32(ipv4_hdr->src_addr);
	key.proto = ipv4_hdr->next_proto_id; //ip层下的协议号，如tcp/udp
	switch (ipv4_hdr->next_proto_id) {
	case IPPROTO_TCP:
		tcp = (struct tcp_hdr *)((unsigned char *) ipv4_hdr +
					sizeof(struct ipv4_hdr));
		key.port_dst = rte_be_to_cpu_16(tcp->dst_port); //目的端口
		key.port_src = rte_be_to_cpu_16(tcp->src_port);  //源端口
		break;
	case IPPROTO_UDP:
		udp = (struct udp_hdr *)((unsigned char *) ipv4_hdr +
					sizeof(struct ipv4_hdr));
		key.port_dst = rte_be_to_cpu_16(udp->dst_port);
		key.port_src = rte_be_to_cpu_16(udp->src_port);
		break;
	default:
		key.port_dst = 0;
		key.port_src = 0;
	}
	/* 查找目的端口 */
	ret = rte_hash_lookup(l3fwd_lookup_struct, (const void *)&key);
	//找不到默认从输入队列出，找到就从l3fwd_out_if[ret]返回；
	return (uint8_t)((ret < 0)? portid : l3fwd_out_if[ret]);
}
#endif
#if (APP_LOOKUP_METHOD == APP_LOOKUP_LPM)
static inline uint8_t
get_dst_port(struct ipv4_hdr *ipv4_hdr,  uint8_t portid, lookup_struct_t * l3fwd_lookup_struct)
{
	uint32_t next_hop;
	//将目的地址传入进行查询下一条信息。
	return (uint8_t) ((rte_lpm_lookup(l3fwd_lookup_struct,
			rte_be_to_cpu_32(ipv4_hdr->dst_addr), &next_hop) == 0)?
			next_hop : portid);
}
#endif
//三层转发
static inline void
l3fwd_simple_forward(struct rte_mbuf *m, uint8_t portid, lookup_struct_t * l3fwd_lookup_struct)
{
	struct ether_hdr *eth_hdr; //以太网头结构
	struct ipv4_hdr *ipv4_hdr; //IP头
	void *tmp;
	uint8_t dst_port;
	//找到以太网头偏移地址
	eth_hdr = rte_pktmbuf_mtod(m, struct ether_hdr *);
  //找到IP头偏移地址 偏移了sizeof(struct ether_hdr)位
	ipv4_hdr = rte_pktmbuf_mtod_offset(m, struct ipv4_hdr *,
					   sizeof(struct ether_hdr));
#ifdef DO_RFC_1812_CHECKS
	//做头部检查
	/* Check to make sure the packet is valid (RFC1812) */
	if (is_valid_ipv4_pkt(ipv4_hdr, m->pkt_len) < 0) {
		rte_pktmbuf_free(m);
		return;
	}
#endif
	//查找路由
	dst_port = get_dst_port(ipv4_hdr, portid, l3fwd_lookup_struct);
	if (dst_port >= RTE_MAX_ETHPORTS || (enabled_port_mask & 1 << dst_port) == 0)
		dst_port = portid; //查出来的dst_port有问题，或者是未启动的port，则用默认的port
	printf("forward to %d port\n", dst_port);
	/* 02:00:00:00:00:xx ，修改目的mac地址*/
	tmp = ð_hdr->d_addr.addr_bytes[0];
	*((uint64_t *)tmp) = 0x000000000002 + ((uint64_t)dst_port << 40);
#ifdef DO_RFC_1812_CHECKS
	/* Update time to live and header checksum */
	--(ipv4_hdr->time_to_live); //更新TTL
	++(ipv4_hdr->hdr_checksum);
#endif
	/* src addr 复制源地址 */
	ether_addr_copy(&ports_eth_addr[dst_port], ð_hdr->s_addr);
	send_single_packet(m, dst_port);
}
/* main processing loop */
static int
main_loop(__attribute__((unused)) void *dummy)
{
	//每个逻辑核都会有一个线程处理
	struct rte_mbuf *pkts_burst[MAX_PKT_BURST]; //数据包发送缓存
	unsigned lcore_id;   
	uint64_t prev_tsc, diff_tsc, cur_tsc;
	int i, j, nb_rx;
	uint8_t portid, queueid;
	struct lcore_conf *qconf;
	const uint64_t drain_tsc = (rte_get_tsc_hz() + US_PER_S - 1) / US_PER_S * BURST_TX_DRAIN_US;
	prev_tsc = 0;
	lcore_id = rte_lcore_id(); //获取逻辑核ID
	qconf = &lcore_conf[lcore_id]; //逻辑核对应的配置，逻辑核可以找到自己处理那些端口，哪些队列等。
	if (qconf->n_rx_queue == 0) {
		//逻辑核为配置任何队列，说明没啥事干，退出
		RTE_LOG(INFO, L3FWD, "lcore %u has nothing to do\n", lcore_id);
		return 0;
	}
	RTE_LOG(INFO, L3FWD, "entering main loop on lcore %u\n", lcore_id);
	for (i = 0; i < qconf->n_rx_queue; i++) {
	
		portid = qconf->rx_queue_list[i].port_id; //对应的网卡
		queueid = qconf->rx_queue_list[i].queue_id;  //对应的接收队列id
		RTE_LOG(INFO, L3FWD, " -- lcoreid=%u portid=%hhu rxqueueid=%hhu\n", lcore_id,
			portid, queueid);
	}
	while (1) {
		//计时
		cur_tsc = rte_rdtsc();
		/*
		 * 发送数据包
		 */
		diff_tsc = cur_tsc - prev_tsc;
		if (unlikely(diff_tsc > drain_tsc)) {
			/*
			 * This could be optimized (use queueid instead of
			 * portid), but it is not called so often
			 */
			for (portid = 0; portid < RTE_MAX_ETHPORTS; portid++) {
				//每个网口就一个发送队列
				if (qconf->tx_mbufs[portid].len == 0)
					continue;
				//发送数据包
				send_burst(&lcore_conf[lcore_id],
					qconf->tx_mbufs[portid].len,
					portid);
				qconf->tx_mbufs[portid].len = 0;
			}
			prev_tsc = cur_tsc;
		}
		/*
		 * 从RX queues 读取数据包
		 */
		for (i = 0; i < qconf->n_rx_queue; ++i) {
			portid = qconf->rx_queue_list[i].port_id; //端口
			queueid = qconf->rx_queue_list[i].queue_id; //队列id
			nb_rx = rte_eth_rx_burst(portid, queueid, pkts_burst, MAX_PKT_BURST);
			/* 第一次逾期数据包 */
			for (j = 0; j < PREFETCH_OFFSET && j < nb_rx; j++) {
				rte_prefetch0(rte_pktmbuf_mtod(
						pkts_burst[j], void *));
			}
			/* 预取并转发已经预取的数据包 */
			for (j = 0; j < (nb_rx - PREFETCH_OFFSET); j++) {
				rte_prefetch0(rte_pktmbuf_mtod(pkts_burst[
						j + PREFETCH_OFFSET], void *));
				l3fwd_simple_forward(pkts_burst[j], portid, qconf->lookup_struct);
			}
			/*  转发剩余的数据包 */
			for (; j < nb_rx; j++) {
				l3fwd_simple_forward(pkts_burst[j], portid, qconf->lookup_struct);
			}
		}
	}
}
//检查逻辑核相关参数
static int
check_lcore_params(void)
{
	uint8_t queue, lcore;
	uint16_t i;
	int socketid;
	for (i = 0; i < nb_lcore_params; ++i) {
		//队列是否在范围内
		queue = lcore_params[i].queue_id;
		if (queue >= MAX_RX_QUEUE_PER_PORT) {
			printf("invalid queue number: %hhu\n", queue);
			return -1;
		}
		//逻辑核是否在启动参数中指定
		lcore = lcore_params[i].lcore_id;
		if (!rte_lcore_is_enabled(lcore)) { 
			printf("error: lcore %hhu is not enabled in lcore mask\n", lcore);
			return -1;
		}
		//numa结构
		if ((socketid = rte_lcore_to_socket_id(lcore) != 0) &&
			(numa_on == 0)) {
			printf("warning: lcore %hhu is on socket %d with numa off \n",
				lcore, socketid);
		}
	}
	return 0;
}
//检查网口配置
static int
check_port_config(const unsigned nb_ports)
{
	unsigned portid;
	uint16_t i;
	for (i = 0; i < nb_lcore_params; ++i) {
		//是否在运行参数中指定
		portid = lcore_params[i].port_id;
		if ((enabled_port_mask & (1 << portid)) == 0) {
			printf("port %u is not enabled in port mask\n", portid);
			return -1;
		}
		if (portid >= nb_ports) {
			printf("port %u is not present on the board\n", portid);
			return -1;
		}
	}
	return 0;
}
//得到网口的接收队列数
static uint8_t
get_port_n_rx_queues(const uint8_t port)
{
	int queue = -1;
	uint16_t i;
	for (i = 0; i < nb_lcore_params; ++i) {
		if (lcore_params[i].port_id == port && lcore_params[i].queue_id > queue)
			queue = lcore_params[i].queue_id;
	}
	return (uint8_t)(++queue);
}
//初始化接收队列逻辑核配置结构
static int
init_lcore_rx_queues(void)
{
	uint16_t i, nb_rx_queue;
	uint8_t lcore;
	for (i = 0; i < nb_lcore_params; ++i) {
		lcore = lcore_params[i].lcore_id; //逻辑核ID
		nb_rx_queue = lcore_conf[lcore].n_rx_queue; //此逻辑核队列数
		if (nb_rx_queue >= MAX_RX_QUEUE_PER_LCORE) { //检查合法性
			printf("error: too many queues (%u) for lcore: %u\n",
				(unsigned)nb_rx_queue + 1, (unsigned)lcore);
			return -1;
		} else {
			lcore_conf[lcore].rx_queue_list[nb_rx_queue].port_id =
				lcore_params[i].port_id; //
			lcore_conf[lcore].rx_queue_list[nb_rx_queue].queue_id =
				lcore_params[i].queue_id;
			lcore_conf[lcore].n_rx_queue++;
		}
	}
	return 0;
}
/* display usage */
static void
print_usage(const char *prgname)
{
	printf ("%s [EAL options] -- -p PORTMASK"
		"  [--config (port,queue,lcore)[,(port,queue,lcore]]\n"
		"  -p PORTMASK: hexadecimal bitmask of ports to configure\n"
		"  --config (port,queue,lcore): rx queues configuration\n"
		"  --no-numa: optional, disable numa awareness\n",
		prgname);
}
/* 捕获从终端获取的信号，进行善后处理。*/
static void
signal_handler(int signum)
{
	uint8_t portid;
	uint8_t nb_ports = rte_eth_dev_count();
	/* When we receive a SIGINT signal */
	if (signum == SIGINT) {
		for (portid = 0; portid < nb_ports; portid++) {
			/* skip ports that are not enabled 检查哪些网卡启动 */
			if ((enabled_port_mask & (1 << portid)) == 0)
				continue;
			rte_eth_dev_close(portid); //关闭相关网卡
		}
	}
	rte_exit(EXIT_SUCCESS, "\n User forced exit\n");
}
//处理网口掩码
static int
parse_portmask(const char *portmask)
{
	char *end = NULL;
	unsigned long pm;
	/* parse hexadecimal string */
	pm = strtoul(portmask, &end, 16);
	/*strtoul()会将参数nptr字符串根据参数base来转换成无符号的长整型数。
		参数base范围从2至36，或0。参数base代表采用的进制方式，如base值为10
		则采用10进制，若base值为16则采用16进制数等。*/
	if ((portmask[0] == '\0') || (end == NULL) || (*end != '\0'))
		return -1;
	if (pm == 0)
		return -1;
	return pm;
}
//解析--config这个参数
static int
parse_config(const char *q_arg)
{
	char s[256];
	const char *p, *p0 = q_arg;
	char *end;
	enum fieldnames {
		FLD_PORT = 0,
		FLD_QUEUE,
		FLD_LCORE,
		_NUM_FLD
	};
	unsigned long int_fld[_NUM_FLD];
	char *str_fld[_NUM_FLD];
	int i;
	unsigned size;
	nb_lcore_params = 0;
	while ((p = strchr(p0,'(')) != NULL) {
		++p;
		if((p0 = strchr(p,')')) == NULL)
			return -1;
		size = p0 - p;
		if(size >= sizeof(s))
			return -1;
		snprintf(s, sizeof(s), "%.*s", size, p);
		if (rte_strsplit(s, sizeof(s), str_fld, _NUM_FLD, ',') != _NUM_FLD)
			return -1;
		for (i = 0; i < _NUM_FLD; i++){
			errno = 0;
			int_fld[i] = strtoul(str_fld[i], &end, 0);
			if (errno != 0 || end == str_fld[i] || int_fld[i] > 255)
				return -1;
		}
		if (nb_lcore_params >= MAX_LCORE_PARAMS) {
			printf("exceeded max number of lcore params: %hu\n",
				nb_lcore_params);
			return -1;
		}
		lcore_params_array[nb_lcore_params].port_id = (uint8_t)int_fld[FLD_PORT];
		lcore_params_array[nb_lcore_params].queue_id = (uint8_t)int_fld[FLD_QUEUE];
		lcore_params_array[nb_lcore_params].lcore_id = (uint8_t)int_fld[FLD_LCORE];
		++nb_lcore_params;
	}
	lcore_params = lcore_params_array;
	return 0;
}
/* Parse the argument given in the command line of the application */
static int
parse_args(int argc, char **argv)
{
	int opt, ret;
	char **argvopt;
	int option_index;
	char *prgname = argv[0];
	static struct option lgopts[] = {
		{"config", 1, 0, 0},
		{"no-numa", 0, 0, 0},
		{NULL, 0, 0, 0}
	};
	argvopt = argv;
	while ((opt = getopt_long(argc, argvopt, "p:",
				lgopts, &option_index)) != EOF) {
		switch (opt) {
		/* portmask */
		case 'p':
			enabled_port_mask = parse_portmask(optarg);
			if (enabled_port_mask == 0) {
				printf("invalid portmask\n");
				print_usage(prgname);
				return -1;
			}
			break;
		/* long options */
		case 0:
			if (!strcmp(lgopts[option_index].name, "config")) {
				ret = parse_config(optarg);
				if (ret) {
					printf("invalid config\n");
					print_usage(prgname);
					return -1;
				}
			}
			if (!strcmp(lgopts[option_index].name, "no-numa")) {
				printf("numa is disabled \n");
				numa_on = 0;
			}
			break;
		default:
			print_usage(prgname);
			return -1;
		}
	}
	if (optind >= 0)
		argv[optind-1] = prgname;
	ret = optind-1;
	optind = 0; /* reset getopt lib */
	return ret;
}
//打印mac地址
static void
print_ethaddr(const char *name, const struct ether_addr *eth_addr)
{
	char buf[ETHER_ADDR_FMT_SIZE];
	ether_format_addr(buf, ETHER_ADDR_FMT_SIZE, eth_addr);
	printf("%s%s", name, buf);
}
#if (APP_LOOKUP_METHOD == APP_LOOKUP_EXACT_MATCH)
//构建hash表 
static void
setup_hash(int socketid)
{
	unsigned i;
	int ret;
	char s[64];
	/* create  hashes */
	snprintf(s, sizeof(s), "l3fwd_hash_%d", socketid);
	l3fwd_hash_params.name = s; //hash表名称
	l3fwd_hash_params.socket_id = socketid; //对应的物理cpu id
	l3fwd_lookup_struct[socketid] = rte_hash_create(&l3fwd_hash_params); //创建相关结构 
	if (l3fwd_lookup_struct[socketid] == NULL)
		rte_exit(EXIT_FAILURE, "Unable to create the l3fwd hash on "
				"socket %d\n", socketid);
	/* 根据配置添加key,计算hash值 */
	for (i = 0; i < L3FWD_NUM_ROUTES; i++) {
		ret = rte_hash_add_key (l3fwd_lookup_struct[socketid],
				(void *) &l3fwd_route_array[i].key);
		if (ret < 0) {
			rte_exit(EXIT_FAILURE, "Unable to add entry %u to the"
				"l3fwd hash on socket %d\n", i, socketid);
		}
		l3fwd_out_if[ret] = l3fwd_route_array[i].if_out; //每个hash条目对应的输入网口
		printf("Hash: Adding key\n");
		print_key(l3fwd_route_array[i].key);
	}
}
#endif
#if (APP_LOOKUP_METHOD == APP_LOOKUP_LPM)
//最长匹配算法
static void
setup_lpm(int socketid)
{
	unsigned i;
	int ret;
	char s[64];
	struct rte_lpm_config lpm_ipv4_config;
	lpm_ipv4_config.max_rules = L3FWD_LPM_MAX_RULES;
	lpm_ipv4_config.number_tbl8s = 256;
	lpm_ipv4_config.flags = 0;
	/* create the LPM table */
	snprintf(s, sizeof(s), "L3FWD_LPM_%d", socketid);
	l3fwd_lookup_struct[socketid] =
			rte_lpm_create(s, socketid, &lpm_ipv4_config);
	if (l3fwd_lookup_struct[socketid] == NULL)
		rte_exit(EXIT_FAILURE, "Unable to create the l3fwd LPM table"
				" on socket %d\n", socketid);
	/* populate the LPM table */
	for (i = 0; i < L3FWD_NUM_ROUTES; i++) {
		ret = rte_lpm_add(l3fwd_lookup_struct[socketid],
			l3fwd_route_array[i].ip,
			l3fwd_route_array[i].depth,
			l3fwd_route_array[i].if_out);
		if (ret < 0) {
			rte_exit(EXIT_FAILURE, "Unable to add entry %u to the "
				"l3fwd LPM table on socket %d\n",
				i, socketid);
		}
		printf("LPM: Adding route 0x%08x / %d (%d)\n",
			(unsigned)l3fwd_route_array[i].ip,
			l3fwd_route_array[i].depth,
			l3fwd_route_array[i].if_out);
	}
}
#endif
//初始化相关内存结构
static int
init_mem(unsigned nb_mbuf)
{
	struct lcore_conf *qconf;
	int socketid;
	unsigned lcore_id;
	char s[64];
	for (lcore_id = 0; lcore_id < RTE_MAX_LCORE; lcore_id++) {
		if (rte_lcore_is_enabled(lcore_id) == 0)
			continue;
		
		//numa结构
		if (numa_on)
			socketid = rte_lcore_to_socket_id(lcore_id);
		else
			socketid = 0;
		
		if (socketid >= NB_SOCKETS) {
			rte_exit(EXIT_FAILURE, "Socket %d of lcore %u is out of range %d\n",
				socketid, lcore_id, NB_SOCKETS);
		}
		if (pktmbuf_pool[socketid] == NULL) {
			//创建内存池
			snprintf(s, sizeof(s), "mbuf_pool_%d", socketid);
			pktmbuf_pool[socketid] = rte_pktmbuf_pool_create(s,
				nb_mbuf, MEMPOOL_CACHE_SIZE, 0,
				RTE_MBUF_DEFAULT_BUF_SIZE, socketid);
			if (pktmbuf_pool[socketid] == NULL)
				rte_exit(EXIT_FAILURE, "Cannot init mbuf pool on socket %d\n", socketid);
			else
				printf("Allocated mbuf pool on socket %d\n", socketid);
#if (APP_LOOKUP_METHOD == APP_LOOKUP_LPM)
			setup_lpm(socketid);
#else
			setup_hash(socketid);
#endif
		}
		qconf = &lcore_conf[lcore_id];
		qconf->lookup_struct = l3fwd_lookup_struct[socketid];
	}
	return 0;
}
int
main(int argc, char **argv)
{
	struct lcore_conf *qconf;
	struct rte_eth_dev_info dev_info;
	struct rte_eth_txconf *txconf;
	int ret;
	unsigned nb_ports;
	uint16_t queueid;
	unsigned lcore_id;
	uint32_t nb_lcores;
	uint16_t n_tx_queue;
	uint8_t portid, nb_rx_queue, queue, socketid;
	//处理信号
	signal(SIGINT, signal_handler);
	/* init EAL */
	ret = rte_eal_init(argc, argv);
	if (ret < 0)
		rte_exit(EXIT_FAILURE, "Invalid EAL parameters\n");
	argc -= ret;
	argv += ret;
	/* parse application arguments (after the EAL ones) */
	ret = parse_args(argc, argv);
	if (ret < 0)
		rte_exit(EXIT_FAILURE, "Invalid L3FWD-VF parameters\n");
	if (check_lcore_params() < 0)
		rte_exit(EXIT_FAILURE, "check_lcore_params failed\n");
	ret = init_lcore_rx_queues();
	if (ret < 0)
		rte_exit(EXIT_FAILURE, "init_lcore_rx_queues failed\n");
	nb_ports = rte_eth_dev_count();
	if (nb_ports > RTE_MAX_ETHPORTS)
		nb_ports = RTE_MAX_ETHPORTS;
	if (check_port_config(nb_ports) < 0)
		rte_exit(EXIT_FAILURE, "check_port_config failed\n");
	nb_lcores = rte_lcore_count();
	/* initialize all ports */
	for (portid = 0; portid < nb_ports; portid++) {
		/* skip ports that are not enabled */
		if ((enabled_port_mask & (1 << portid)) == 0) {
			printf("\nSkipping disabled port %d\n", portid);
			continue;
		}
		/* init port */
		printf("Initializing port %d ... ", portid );
		fflush(stdout);
		/* must always equal(=1) */
		nb_rx_queue = get_port_n_rx_queues(portid);
		n_tx_queue = MAX_TX_QUEUE_PER_PORT;
		printf("Creating queues: nb_rxq=%d nb_txq=%u... ",
			nb_rx_queue, (unsigned)1 );
		//队列配置
		ret = rte_eth_dev_configure(portid, nb_rx_queue, n_tx_queue, &port_conf);
		if (ret < 0)
			rte_exit(EXIT_FAILURE, "Cannot configure device: err=%d, port=%d\n",
				ret, portid);
		rte_eth_macaddr_get(portid, &ports_eth_addr[portid]);
		print_ethaddr(" Address:", &ports_eth_addr[portid]);
		printf(", ");
		ret = init_mem(NB_MBUF);
		if (ret < 0)
			rte_exit(EXIT_FAILURE, "init_mem failed\n");
		/* init one TX queue */
		socketid = (uint8_t)rte_lcore_to_socket_id(rte_get_master_lcore());
		printf("txq=%d,%d,%d ", portid, 0, socketid);
		fflush(stdout);
		
		rte_eth_dev_info_get(portid, &dev_info);
		txconf = &dev_info.default_txconf;
		if (port_conf.rxmode.jumbo_frame)
			txconf->txq_flags = 0;
		//发送队列配置
		ret = rte_eth_tx_queue_setup(portid, 0, nb_txd,
						 socketid, txconf);
		if (ret < 0)
			rte_exit(EXIT_FAILURE, "rte_eth_tx_queue_setup: err=%d, "
				"port=%d\n", ret, portid);
		printf("\n");
	}
	for (lcore_id = 0; lcore_id < RTE_MAX_LCORE; lcore_id++) {
		if (rte_lcore_is_enabled(lcore_id) == 0)
			continue;
		qconf = &lcore_conf[lcore_id];
		qconf->tx_queue_id = 0;
		printf("\nInitializing rx queues on lcore %u ... ", lcore_id );
		fflush(stdout);
		/* init RX queues */
		for(queue = 0; queue < qconf->n_rx_queue; ++queue) {
			portid = qconf->rx_queue_list[queue].port_id;
			queueid = qconf->rx_queue_list[queue].queue_id;
			if (numa_on)
				socketid = (uint8_t)rte_lcore_to_socket_id(lcore_id);
			else
				socketid = 0;
			printf("rxq=%d,%d,%d ", portid, queueid, socketid);
			fflush(stdout);
			//接收队列初始化
			ret = rte_eth_rx_queue_setup(portid, queueid, nb_rxd,
						socketid, NULL,
						pktmbuf_pool[socketid]);
			if (ret < 0)
				rte_exit(EXIT_FAILURE, "rte_eth_rx_queue_setup: err=%d,"
						"port=%d\n", ret, portid);
		}
	}
	printf("\n");
	/* start ports */
	for (portid = 0; portid < nb_ports; portid++) {
		if ((enabled_port_mask & (1 << portid)) == 0) {
			continue;
		}
		/* Start device，开启设备 */
		ret = rte_eth_dev_start(portid);
		if (ret < 0)
			rte_exit(EXIT_FAILURE, "rte_eth_dev_start: err=%d, port=%d\n",
				ret, portid);
		printf("done: Port %d\n", portid);
	}
	/* launch per-lcore init on every lcore */
	rte_eal_mp_remote_launch(main_loop, NULL, CALL_MASTER);
	RTE_LCORE_FOREACH_SLAVE(lcore_id) {
		if (rte_eal_wait_lcore(lcore_id) < 0)
			return -1;
	}
	return 0;
}
```

# 0x05 总结
遇到一个问题，发现这个程序无法处理TCP的数据包，看后期慢慢熟悉能否解决这个问题。


