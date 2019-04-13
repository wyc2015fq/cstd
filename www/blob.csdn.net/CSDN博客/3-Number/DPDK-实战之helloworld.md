
# DPDK-实战之helloworld - 3-Number - CSDN博客


2017年10月12日 16:06:07[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：2223


# 0x01 缘由
“纸上得来终觉浅，绝知此事要躬行”，前面学习了linux tcp/ip协议栈、DPDK基础理论，学下来的感觉是：以前对这几个方面都充满敬畏感，觉得很神秘、很高端，但是学习下来总结了一点----只要不断去摸索去调式去找资料，不懂的都会变得你懂的。仅仅学习理论是不够的，还得在实践中运用。然而在公司产品实践中有时无法接触这方面的知识的应用，所以只能通过一些例子和实现开源解决方案来加强加固。
前面一年或多或少的学习和实践跑起来一些例子，但是没有对代码进行深入的学习。没有学习相关的设计理念和设计者的目的，已经一些压力场景的情况。下面再次复习相关例子，从最简单的开始。
# 0x02 helloworld例子
这个是最简单的使用dpdk开发套件的例程。
源码分析：
```python
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <sys/queue.h>
//以上头开发环境glibc的相关头文件
#include <rte_memory.h>
#include <rte_memzone.h>
#include <rte_launch.h>
#include <rte_eal.h>
#include <rte_per_lcore.h>
#include <rte_lcore.h>
#include <rte_debug.h>
/*以上为dpdk自己编写的一些公共库头文件,如内存池、线程、抽象环境等工具，DPDK有自己的开发风格，适应之。*/
static int
lcore_hello(__attribute__((unused)) void *arg)/* 此处有一个字节对齐操作，此处不做详细分析。*/
{
    unsigned lcore_id;
    lcore_id = rte_lcore_id(); //获取逻辑核编号，并输出逻辑核id，返回，线程退出。
    printf("hello from core %u\n", lcore_id);
    return 0;
}
int
main(int argc, char **argv)
{
    int ret;
    unsigned lcore_id;
    /* 相关初始化工作，如命令含参数处理，自动检测环境相关条件。以及相关库平台初始化工作*/
    ret = rte_eal_init(argc, argv);
    if (ret < 0)
        rte_panic("Cannot init EAL\n");
    /* 每个从逻辑核调用回调函数lcore_hello输出相关信息。 */
    RTE_LCORE_FOREACH_SLAVE(lcore_id) {
        rte_eal_remote_launch(lcore_hello, NULL, lcore_id);
    }
    /* 再次调用主逻辑核输出相关信息。 */
    lcore_hello(NULL);
    /* 等待所有从逻辑核调用返回，相当于主线程阻塞等待。*/
    rte_eal_mp_wait_lcore();
    return 0;
}
Makefile:
#判断相关环境变量是否设置
ifeq ($(RTE_SDK),)
$(error "Please define RTE_SDK environment variable")
endif
# 默认的平台目标
RTE_TARGET ?= x86_64-native-linuxapp-gcc
include $(RTE_SDK)/mk/rte.vars.mk
# binary name
APP = helloworld
# all source are stored in SRCS-y
SRCS-y := main.c
CFLAGS += -O3
CFLAGS += $(WERROR_FLAGS)
include $(RTE_SDK)/mk/rte.extapp.mk
```

# 0x03 运行环境搭建
运行环境已在另一篇博文有讲解，在此不再赘述。[http://blog.csdn.net/pangyemeng/article/details/49883717](http://blog.csdn.net/pangyemeng/article/details/49883717)
# 0x04 运行
运行环境：
Linux Huawei 2.6.32-431.el6.x86_64 \#1 SMP Fri Nov 22 03:15:09 UTC 2013 x86_64 x86_64 x86_64 GNU/Linux
DPDK版本：
dpdk-16.04
## 1.简单默认运行结果
自动检测32个逻辑核，将0核作为主逻辑核。
[root@Huawei build]\# ./helloworld
```python
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
EAL: Ask a virtual area of 0xc00000 bytes
EAL: Virtual area found at 0x7fda71e00000 (size = 0xc00000)
EAL: Ask a virtual area of 0x400000 bytes
EAL: Virtual area found at 0x7fda71800000 (size = 0x400000)
EAL: Ask a virtual area of 0x400000 bytes
EAL: Virtual area found at 0x7fda71200000 (size = 0x400000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda70e00000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda70a00000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda70600000 (size = 0x200000)
EAL: Ask a virtual area of 0x400000 bytes
EAL: Virtual area found at 0x7fda70000000 (size = 0x400000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda6fc00000 (size = 0x200000)
EAL: Ask a virtual area of 0x400000 bytes
EAL: Virtual area found at 0x7fda6f600000 (size = 0x400000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda6f200000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda6ee00000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda6ea00000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda6e600000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda6e200000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda6de00000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda6da00000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda6d600000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda6d200000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda6ce00000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda6ca00000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda6c600000 (size = 0x200000)
EAL: Ask a virtual area of 0x600000 bytes
EAL: Virtual area found at 0x7fda6be00000 (size = 0x600000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda6ba00000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda6b600000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda6b200000 (size = 0x200000)
EAL: Ask a virtual area of 0x400000 bytes
EAL: Virtual area found at 0x7fda6ac00000 (size = 0x400000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda6a800000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda6a400000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda6a000000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda69c00000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda69800000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda69400000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda69000000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda68c00000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda68800000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda68400000 (size = 0x200000)
EAL: Ask a virtual area of 0x400000 bytes
EAL: Virtual area found at 0x7fda67e00000 (size = 0x400000)
EAL: Ask a virtual area of 0x400000 bytes
EAL: Virtual area found at 0x7fda67800000 (size = 0x400000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda67400000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda67000000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda66c00000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda66800000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda66400000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda66000000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda65c00000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda65800000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda65400000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda65000000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda64c00000 (size = 0x200000)
EAL: Ask a virtual area of 0x200000 bytes
EAL: Virtual area found at 0x7fda64800000 (size = 0x200000)
EAL: Requesting 64 pages of size 2MB from socket 0
EAL: TSC frequency is ~2593994 KHz
EAL: Master lcore 0 is ready (tid=72be3880;cpuset=[0])
EAL: lcore 4 is ready (tid=61ffb700;cpuset=[4])
EAL: lcore 15 is ready (tid=5b1f0700;cpuset=[15])
EAL: lcore 27 is ready (tid=539e4700;cpuset=[27])
EAL: lcore 6 is ready (tid=60bf9700;cpuset=[6])
EAL: lcore 8 is ready (tid=5f7f7700;cpuset=[8])
EAL: lcore 10 is ready (tid=5e3f5700;cpuset=[10])
EAL: lcore 12 is ready (tid=5cff3700;cpuset=[12])
EAL: lcore 14 is ready (tid=5bbf1700;cpuset=[14])
EAL: lcore 18 is ready (tid=593ed700;cpuset=[18])
EAL: lcore 16 is ready (tid=5a7ef700;cpuset=[16])
EAL: lcore 19 is ready (tid=589ec700;cpuset=[19])
EAL: lcore 22 is ready (tid=56be9700;cpuset=[22])
EAL: lcore 24 is ready (tid=557e7700;cpuset=[24])
EAL: lcore 25 is ready (tid=54de6700;cpuset=[25])
EAL: lcore 28 is ready (tid=52fe3700;cpuset=[28])
EAL: lcore 5 is ready (tid=615fa700;cpuset=[5])
EAL: lcore 1 is ready (tid=63dfe700;cpuset=[1])
EAL: lcore 7 is ready (tid=601f8700;cpuset=[7])
EAL: lcore 13 is ready (tid=5c5f2700;cpuset=[13])
EAL: lcore 3 is ready (tid=629fc700;cpuset=[3])
EAL: lcore 17 is ready (tid=59dee700;cpuset=[17])
EAL: lcore 23 is ready (tid=561e8700;cpuset=[23])
EAL: lcore 2 is ready (tid=633fd700;cpuset=[2])
EAL: lcore 31 is ready (tid=511e0700;cpuset=[31])
EAL: lcore 9 is ready (tid=5edf6700;cpuset=[9])
EAL: lcore 21 is ready (tid=575ea700;cpuset=[21])
EAL: lcore 26 is ready (tid=543e5700;cpuset=[26])
EAL: lcore 30 is ready (tid=51be1700;cpuset=[30])
EAL: lcore 20 is ready (tid=57feb700;cpuset=[20])
EAL: lcore 11 is ready (tid=5d9f4700;cpuset=[11])
EAL: lcore 29 is ready (tid=525e2700;cpuset=[29])
EAL: PCI device 0000:04:00.0 on NUMA socket 0
EAL:   probe driver: 8086:1521 rte_igb_pmd
EAL:   Not managed by a supported kernel driver, skipped
EAL: PCI device 0000:04:00.1 on NUMA socket 0
EAL:   probe driver: 8086:1521 rte_igb_pmd
EAL:   Not managed by a supported kernel driver, skipped
EAL: PCI device 0000:04:00.2 on NUMA socket 0
EAL:   probe driver: 8086:1521 rte_igb_pmd
EAL:   PCI memory mapped at 0x7fda72a00000
EAL:   PCI memory mapped at 0x7fda72b00000
PMD: eth_igb_dev_init(): port_id 0 vendorID=0x8086 deviceID=0x1521
EAL: PCI device 0000:04:00.3 on NUMA socket 0
EAL:   probe driver: 8086:1521 rte_igb_pmd
EAL:   PCI memory mapped at 0x7fda71d00000
EAL:   PCI memory mapped at 0x7fda71cfc000
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
hello from core 1
hello from core 2
hello from core 3
hello from core 4
hello from core 5
hello from core 6
hello from core 7
hello from core 8
hello from core 9
hello from core 10
hello from core 11
hello from core 12
hello from core 13
hello from core 14
hello from core 15
hello from core 16
hello from core 17
hello from core 18
hello from core 19
hello from core 20
hello from core 21
hello from core 22
hello from core 23
hello from core 24
hello from core 25
hello from core 26
hello from core 27
hello from core 28
hello from core 29
hello from core 30
hello from core 31
hello from core 0
```

# 1.带参数运行 -l
./helloworld -l 0-3
```python
EAL:   Not managed by a supported kernel driver, skipped
EAL: PCI device 0000:05:00.3 on NUMA socket 0
EAL:   probe driver: 8086:1522 rte_igb_pmd
EAL:   Not managed by a supported kernel driver, skipped
hello from core 1
hello from core 2
hello from core 3
hello from core 0
```

# 2.带参数运行 -l --master-lcore
./helloworld -l 0-3 --master-lcore=1
```python
........
```
```python
EAL: PCI device 0000:05:00.3 on NUMA socket 0
EAL:   probe driver: 8086:1522 rte_igb_pmd
EAL:   Not managed by a supported kernel driver, skipped
hello from core 0
hello from core 2
hello from core 3
hello from core 1
```

## 3.其他带参数选项，后期学习时一一解释。
# 0x05 选项解释
```python
[root@dev build]# ./helloworld --help
EAL: Detected lcore 0 as core 0 on socket 0
EAL: Detected lcore 1 as core 1 on socket 0
EAL: Support maximum 128 logical core(s) by configuration.
EAL: Detected 2 lcore(s)
Usage: ./helloworld [options]
EAL common options:
  -c COREMASK         逻辑核16进制掩码
  -l CORELIST         列出运行时逻辑核列表           
                      参数格式 <c1>[-c2][,c3[-c4],...]
                      where c1, c2, etc are core indexes between 0 and 128
  --lcores COREMAP    映射逻辑核到物理逻辑核集合中
                      The argument format is
                            '<lcores[@cpus]>[<,lcores[@cpus]>...]'
                      lcores and cpus list are grouped by '(' and ')'
                      Within the group, '-' is used for range separator,
                      ',' is used for single number separator.
                      '( )' can be omitted for single element group,
                      '@' can be omitted if cpus and lcores have the same value
  --master-lcore ID   指定主线程逻辑核id
  -n CHANNELS         指定内存通道数
  -m MB               指定内存分配 (类似 --socket-mem)
  -r RANKS            Force number of memory ranks (don't detect) 强制内存参数
  -b, --pci-blacklist 将PCI网络设备列入黑名单，防止EAL环境使用这些PCI设备，参数格式为<domain:bus:devid.func>
  -w, --pci-whitelist 将PCI网络设备列入白名单，仅仅用指定的PCI设备，参数格式为<[domain:]bus:devid.func>
  --vdev              添加一块虚拟设备，这个参数格式为 <driver><id>[,key=val,...]
                      (例如: --vdev=eth_pcap0,iface=eth2).
  -d LIB.so|DIR       添加驱动活驱动目录(can be used multiple times)
  --vmware-tsc-map    Use VMware TSC map instead of native RDTSC
  --proc-type         进程的类型 (primary|secondary|auto)
  --syslog            设定syslog日志
  --log-level         设定默认日志级别
  -v                  启动时显示版本信息
  -h, --help          This help
EAL options for DEBUG use only: 调试
  --huge-unlink       在初始化后去掉大页面文件连接
  --no-huge           用 malloc 代替 hugetlbfs
  --no-pci            关闭 PCI
  --no-hpet           关闭 HPET
  --no-shconf         不共享配置(mmap'd files)
EAL Linux options: 选项
  --socket-mem        内存分配
  --huge-dir          大页面挂载目录
  --file-prefix       页表文件前缀
  --base-virtaddr     虚拟地址基址
  --create-uio-dev    Create /dev/uioX (usually done by hotplug)
  --vfio-intr         Interrupt mode for VFIO (legacy|msi|msix)
  --xen-dom0          Support running on Xen dom0 without hugetlbfs
```

# 0x06 总结
有些细节没去认真的分析，细节部分前面也进行了一些理论学习，后面的学习目标是：快速上手，熟悉使用，学习设计思想。

