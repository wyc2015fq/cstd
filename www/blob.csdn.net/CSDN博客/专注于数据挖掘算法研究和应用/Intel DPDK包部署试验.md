# Intel DPDK包部署试验 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年03月21日 14:14:06[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：7275








inteldpdk(Intel® Data Plane Development Kit)  是intel开发的一个关于网络数据包处理转发的套件。官网：[http://dpdk.org](http://dpdk.org/)






### 1、系统环境要求



1）intel网卡

$lspci |grep Ethernet    //查看网卡，配置2张网卡

2）CPU核数

$cat /proc/cpuinfo     //查看系统有多少个cpu，支持多少硬件线程，配置2个

3）系统内存

$cat /proc/meminfo   //系统内存支持hugepage技术

4）查看hpet

$grep hpet/proc/timer_list   //需支持hpet

如无显示，需设置BIOS：Advanced -> PCH-IO Configuration -> High Precision Timer ->(Change from Disabled to Enabled if necessary)

Virtualbox上虚拟机Ubuntu如何设置BIOS？

5）系统内核

$uname –a   //虚拟机 Ubunut（主机名c） GNU/Linux x86_64 

//4.2.0-16.generic（要求Kernelversion >= 2.6.34，支持hugepage）

// Kernel需支持UIO、HUGETLBFS、PROC_PAGE_MONITOR

6）组件库

$gcc –verison   //要求gcc4.5.x以上，本机gcc5.2.1；

$sudo apt-get installglibc++

$ldd –version  //要求glibc >= 2.7 ，本机2.2.1

如何编译安装glibc（下载地址：[http://ftp.gnu.org/gnu/glibc/](http://ftp.gnu.org/gnu/glibc/)）升级？

$sudo apt-get install libpcap-dev   //安装pcap

若要使用libpcap驱动分析，修改配置文件：$sudo gedit config/common_linuxapp

CONFIG_RTE_LIBRTE_PMD_PCAP=n 修改为：CONFIG_RTE_LIBRTE_PMD_PCAP=y

$sudo apt-get installpython  //安装python

$sudo apt-get installpython-dev

### 2、DPDK编译安装

1）GIT下载

 $sudo apt-get install git  //安装git下载

 $git clone git://dpdk.org/dpdk  //git到/home/c/dpdk目录

配置环境变量：$sudo gedit/etc/profile

export  RTE_SDK=/home/c/dpdk

export  RTE_TARGET= x86_64-native-linuxapp-gcc

2）脚本安装：

$cd dpdk

$./tools/setup.sh    

//step1:14  x86_64-native-linuxapp-gcc

  RTE_SDK=/home/c/dpdk

  RTE_TARGET= x86_64-native-linuxapp-gcc

//step2:17Insert IGB UIO module

//step3:20Setup hugepage mappings for non-NUMA systems 输入64或128

//step4:23Bind Ethernet device to IGB UIO module 

提前关闭网卡：$ifconfig enp0s8 down //enp0s8网卡接口名，地址00.08.0

查看PCI地址；$lspci   //找网卡地址

输入PCI地址：00.08.0

//step5:22Display current Ethernet device settings

//step6:26Run test application($RTE_TARGET/app/test)

输入bitmask:0x3  

设置CPU的掩码，根据CPU的个数来设置，比如如果只有2个cpu，按照16进制掩码就选择 0x3

提示HPET不可用，需要BIOS设置。

~~3）手动安装：~~

~~——编译~~

~~$makeinstall T=x86_64-native-linuxapp-gcc~~

~~——配置大页内存（非NUMA）~~

~~$echo 128> /sys/kernel/mm/hugepages/hugepages-2048kB/nr_hugepages~~

~~$mkdir/mnt/huge~~

~~$mount -thugetlbfs nodev /mnt/huge~~

~~$cat/proc/meminfo | grep Huge   //查看大页内存状态~~

~~——安装igb_uio驱动~~

~~$modprobeuio~~

~~$insmodx86_64-native-linuxapp-gcc/kmod/igb_uio.ko~~

~~——绑定网卡~~

~~$./tools/dpdk_nic_bind.py--status~~

~~$ ./tools/dpdk_nic_bind.py -b igb_uio 00.03.0  //网卡1的PCI地址，可用eth1接口名~~

~~$./tools/dpdk_nic_bind.py-b igb_uio 00.08.0   //网卡2的PCI地址，可用eth2接口名~~

~~——运行testpmd测试程序~~

~~$ ./x86_64-native-linuxapp-gcc/app/testpmd -c 0x3 -n 2 -- -i~~

4）运行示例

$cdexamples/helloworld

$make

$./build/helloworld-c 0xf -n 2

参数解释：c代表用几个core， 采用bit位设置，如 f 代码 二进制 1111 相当于 从0到3这4个core都采用；n 表示设置内存的通道数。






### 3、DPDK示例代码


```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/queue.h>
#include <netinet/in.h>
#include <setjmp.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <unistd.h>
 
#include <rte_common.h>
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
 
#define RTE_LOGTYPE_L2FWD RTE_LOGTYPE_USER1
 
#define MBUF_SIZE (2048 + sizeof(struct rte_mbuf) + RTE_PKTMBUF_HEADROOM)
#define NB_MBUF   8192
 
#define MAX_PKT_BURST 32
#define BURST_TX_DRAIN_US 100 /* TX drain every ~100us */
 
 /*
  * Configurable number of RX/TX ring descriptors
  */
#define RTE_TEST_RX_DESC_DEFAULT 128
#define RTE_TEST_TX_DESC_DEFAULT 512
 static uint16_t nb_rxd = RTE_TEST_RX_DESC_DEFAULT;
 static uint16_t nb_txd = RTE_TEST_TX_DESC_DEFAULT;
 
 /* ethernet addresses of ports */
 static struct ether_addr l2fwd_ports_eth_addr[RTE_MAX_ETHPORTS];
 
 /* mask of enabled ports */
 static uint32_t l2fwd_enabled_port_mask = 0;
 
 /* list of enabled ports */
 static uint32_t iweb_dst_ports[RTE_MAX_ETHPORTS];
 
 static unsigned int l2fwd_rx_queue_per_lcore = 1;
 
 struct mbuf_table {
     unsigned len;
     struct rte_mbuf *m_table[MAX_PKT_BURST];
 };
 
#define MAX_RX_QUEUE_PER_LCORE 16
#define MAX_TX_QUEUE_PER_PORT 16
 struct lcore_queue_conf {
     unsigned n_rx_port;
     unsigned rx_port_list[MAX_RX_QUEUE_PER_LCORE];
     struct mbuf_table tx_mbufs[RTE_MAX_ETHPORTS];
 
 } __rte_cache_aligned;
 struct lcore_queue_conf lcore_queue_conf[RTE_MAX_LCORE];
 
 static const struct rte_eth_conf port_conf = {
     .rxmode = {
         .split_hdr_size = 0,
         .header_split   = 0, /**< Header Split disabled */
         .hw_ip_checksum = 0, /**< IP checksum offload disabled */
         .hw_vlan_filter = 0, /**< VLAN filtering disabled */
         .jumbo_frame    = 0, /**< Jumbo Frame Support disabled */
         .hw_strip_crc   = 0, /**< CRC stripped by hardware */
     },
     .txmode = {
         .mq_mode = ETH_MQ_TX_NONE,
     },
 };
 
 struct rte_mempool * l2fwd_pktmbuf_pool = NULL;
 
 /* Per-port statistics struct */
 struct l2fwd_port_statistics {
     uint64_t tx;
     uint64_t rx;
     uint64_t dropped;
 } __rte_cache_aligned;
 struct l2fwd_port_statistics port_statistics[RTE_MAX_ETHPORTS];
 
 /* A tsc-based timer responsible for triggering statistics printout */
#define TIMER_MILLISECOND 2000000ULL /* around 1ms at 2 Ghz */
#define MAX_TIMER_PERIOD 86400 /* 1 day max */
static int64_t timer_period = 10 * TIMER_MILLISECOND * 1000; /* default period is 10 seconds */
 
 /* main processing loop */
 static void
 l2fwd_main_loop(void)
 {
     struct rte_mbuf *pkts_burst[MAX_PKT_BURST];
     struct rte_mbuf *m;
     unsigned lcore_id;
     uint64_t prev_tsc, diff_tsc, cur_tsc, timer_tsc;
     unsigned i, j, portid, nb_rx;
     struct lcore_queue_conf *qconf;
     const uint64_t drain_tsc = (rte_get_tsc_hz() + US_PER_S - 1) / US_PER_S * BURST_TX_DRAIN_US;
 
     prev_tsc = 0;
     timer_tsc = 0;
 
     lcore_id = rte_lcore_id();
     qconf = &lcore_queue_conf[lcore_id];
 
     if (qconf->n_rx_port == 0) {
         RTE_LOG(INFO, L2FWD, "lcore %u has nothing to do\n", lcore_id);
         return;
     }
 
     //RTE_LOG(INFO, L2FWD, "entering main loop on lcore %u\n", lcore_id);
 
     for (i = 0; i < qconf->n_rx_port; i++) {
 
         portid = qconf->rx_port_list[i];
         RTE_LOG(INFO, L2FWD, " -- lcoreid=%u portid=%u\n", lcore_id, portid);
     }
 
     while (1) {
 
         cur_tsc = rte_rdtsc();
#if 0
         /*
          * TX burst queue drain
          */
         diff_tsc = cur_tsc - prev_tsc;
         if (unlikely(diff_tsc > drain_tsc)) {
             for (portid = 0; portid < RTE_MAX_ETHPORTS; portid++) {
                 if (qconf->tx_mbufs[portid].len == 0)
                     continue;
                 l2fwd_send_burst(&lcore_queue_conf[lcore_id],
                          qconf->tx_mbufs[portid].len,
                          (uint8_t) portid);
                 qconf->tx_mbufs[portid].len = 0;
             }
             /* if timer is enabled */
             if (timer_period > 0) {
                 /* advance the timer */
                 timer_tsc += diff_tsc;
                 /* if timer has reached its timeout */
                 if (unlikely(timer_tsc >= (uint64_t) timer_period)) {

                     /* do this only on master core */
                     if (lcore_id == rte_get_master_lcore()) {
                         print_stats();
                         /* reset the timer */
                         timer_tsc = 0;
                     }
                 }
             }
             prev_tsc = cur_tsc;
         }
#endif
         /*
          * Read packet from RX queues
          */
          usleep(1);
         for (i = 0; i < qconf->n_rx_port; i++) {
 
             portid = qconf->rx_port_list[i];
             nb_rx = rte_eth_rx_burst((uint8_t) portid, 0,
                          pkts_burst, MAX_PKT_BURST);
             if (nb_rx <= 0)
             {
                //printf("xxxxxxxxxxxxxxxxxxxx\n");
                break;
             }
             port_statistics[portid].rx += nb_rx;
             printf("rcv packets %d\n", port_statistics[portid].rx);
             printf("this timer packets is %d\n", nb_rx);
             for (j = 0; j < nb_rx; j++) {
                    
                 m = pkts_burst[j];
                 rte_pktmbuf_free(m);
                 
                 //printf("packet %d\n", j);
                 //printf("rcv packets %d\n", port_statistics[portid].rx);
             }
         }
     }
 }
 
 static int l2fwd_launch_one_lcore(__attribute__((unused)) void *dummy)
 {
     l2fwd_main_loop();
     return 0;
 }

 
/* Parse the argument given in the command line of the application */
static int iweb_parse_args(int argc, char **argv)
{
}
 
int
main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv)
{
    struct lcore_queue_conf *qconf;
    struct rte_eth_dev_info dev_info;
    int ret;
    uint8_t nb_ports;
    uint8_t nb_ports_available;
    uint8_t portid, last_port;
    unsigned lcore_id, rx_lcore_id;
    unsigned nb_ports_in_mask = 0;

    /* init EAL */
    ret = rte_eal_init(argc, argv);
    if (ret < 0)
    {
        rte_exit(EXIT_FAILURE, "Invalid EAL arguments\n");
    }
    argc -= ret;
    argv += ret;

    /* parse application arguments (after the EAL ones) */
    ret = iweb_parse_args(argc, argv);
    if (ret < 0)
    {
        rte_exit(EXIT_FAILURE, "Invalid L2FWD arguments\n");
    }
    
    /* create the mbuf pool */
    l2fwd_pktmbuf_pool = rte_mempool_create("mbuf_pool", NB_MBUF, MBUF_SIZE, 32,
            sizeof(struct rte_pktmbuf_pool_private), rte_pktmbuf_pool_init, NULL,
            rte_pktmbuf_init, NULL, rte_socket_id(), 0);

    if (l2fwd_pktmbuf_pool == NULL)
    {
        rte_exit(EXIT_FAILURE, "Cannot init mbuf pool\n");
    }
    
    nb_ports = rte_eth_dev_count();
    if (nb_ports != 1)
    {
        rte_exit(EXIT_FAILURE, "No Ethernet ports - bye\n");
    }
    /* reset l2fwd_dst_ports */
    iweb_dst_ports[0] = 0;  /* 这里其实只有一个port */

    rx_lcore_id = 0;
    qconf = NULL;
    qconf = &lcore_queue_conf[rx_lcore_id];
    qconf->rx_port_list[0] = portid;
    qconf->n_rx_port = 1;

    printf("Initializing port %u... ", (unsigned) portid);
    fflush(stdout);
    ret = rte_eth_dev_configure(portid, 1, 1, &port_conf);
    if (ret < 0)
    {
        rte_exit(EXIT_FAILURE, "Cannot configure device: err=%d, port=%u\n", ret, (unsigned)portid);
    }

    fflush(stdout);
    ret = rte_eth_rx_queue_setup(portid, 0, nb_rxd, rte_eth_dev_socket_id(portid),NULL,l2fwd_pktmbuf_pool);
    if (ret < 0)
    {
     rte_exit(EXIT_FAILURE, "rte_eth_rx_queue_setup:err=%d, port=%u\n", ret, (unsigned)portid);
    }
    
    /* init one TX queue on each port */
    fflush(stdout);
    ret = rte_eth_tx_queue_setup(portid, 0, nb_txd, rte_eth_dev_socket_id(portid), NULL);
    if (ret < 0)
    {
        rte_exit(EXIT_FAILURE, "rte_eth_tx_queue_setup:err=%d, port=%u\n", ret, (unsigned)portid);
    }
    
    /* Start device */
    ret = rte_eth_dev_start(portid);
    if (ret < 0)
    {
        rte_exit(EXIT_FAILURE, "rte_eth_dev_start:err=%d, port=%u\n", ret, (unsigned)portid);
    }
    printf("done: \n");
    
    /* 打开网卡的混杂模式 */
    rte_eth_promiscuous_enable(portid);
    
    /* launch per-lcore init on every lcore */
    rte_eal_mp_remote_launch(l2fwd_launch_one_lcore, NULL, CALL_MASTER);

    RTE_LCORE_FOREACH_SLAVE(lcore_id)
    {
        if (rte_eal_wait_lcore(lcore_id) < 0)
        {
            return -1;
        }
    }

    return 0;
}
```













