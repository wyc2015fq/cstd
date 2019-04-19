# [GitHub]一个简单的网络驱动 - Koma Hub - CSDN博客
2019年01月27日 19:10:31[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：133
GitHub地址：[https://github.com/gautamanshul/Network-driver](https://github.com/gautamanshul/Network-driver)
项目中的Makefile可能有一定的问题，我重写了一下，并在下面给出network.c和makefile文件；
## network.c
```cpp
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/ip.h>
#include <linux/skbuff.h>
#define RX_INTR 0x0001
#define TX_INTR 0X0002
struct net_device *net0,*net1;
struct os_packet {
	struct net_device *dev;
	int datalen;
	u8 data[ETH_DATA_LEN];
};
struct os_priv { 
	struct net_device_stats stats;
	int status;
	struct os_packet *pkt;
	int rx_int_enabled;
	int tx_packetlen;
	u8 *tx_packetdata;
	struct skbuff *skb;
	spinlock_t lock;
	struct net_device *dev;
};
int os_open(struct net_device *dev) { return 0; }
int os_stop(struct net_device *dev) { return 0; }
static void os_interrupt(int irq, void *dev_id, struct pt_regs *regs) { }
static void os_hw_tx(char *buf, int len, struct net_device *dev) { }
int os_start_xmit(struct sk_buff *skb, struct net_device *dev) {
	return 0;
}
struct net_device_stats *os_stats(struct net_device *dev) {
	return &(((struct os_priv *)netdev_priv(dev))->stats);
} 
int os_header(struct sk_buff *skb, struct net_device *dev, unsigned short type, const void *daddr, const void *saddr, unsigned int len) {
	return 0;
}
static const struct header_ops os_header_ops = {
	.create = os_header,
};
static const struct net_device_ops os_device_ops = {
	.ndo_open = os_open,
	.ndo_stop = os_stop,
	.ndo_start_xmit = os_start_xmit,
	.ndo_get_stats = os_stats,
};
int os_init_mod(void) {
	struct os_priv *priv;
	int i;
	/*net0 and net1 point to net_device structs. Bypassing the PCI as we don't have a real device*/
	net0 = alloc_etherdev(sizeof(struct os_priv));
	net1 = alloc_etherdev(sizeof(struct os_priv));
	for(i = 0; i < 6; i++) net0->dev_addr[i] = (unsigned char)i;
	for(i = 0; i < 6; i++) net0->broadcast[i] = (unsigned char)15;
	net0->hard_header_len = 14;
	
	for(i = 0; i < 6; i++) net1->dev_addr[i] = (unsigned char)i;
	for(i = 0; i < 6; i++) net1->broadcast[i] = (unsigned char)15;
	net1->hard_header_len = 14;
	net1->dev_addr[5]++;
	memcpy(net0->name, "net0\0", 5); 
	memcpy(net0->name, "net1\0", 5);
	net0->netdev_ops = &os_device_ops;
	net0->header_ops = &os_header_ops;
	 
	net1->netdev_ops = &os_device_ops;
	net1->header_ops = &os_header_ops;
	
	net0->flags |= IFF_NOARP;
	net1->flags |= IFF_NOARP;
	priv = netdev_priv(net0);
	memset(priv, 0, sizeof(struct os_priv));
	priv->dev = net0;
	priv->rx_int_enabled = 1;
	priv->pkt = kmalloc(sizeof(struct os_packet), GFP_KERNEL);
	priv->pkt->dev = net0;
	
	priv = netdev_priv(net1);
	memset(priv, 0, sizeof(struct os_priv));
	priv->dev = net1;
	priv->rx_int_enabled = 1;
	priv->pkt = kmalloc(sizeof(struct os_packet), GFP_KERNEL);
	priv->pkt->dev = net1;
	if(register_netdev(net0)) {
		printk(KERN_INFO "net0 not registered\n");
	} else {
		printk(KERN_INFO "net0 registered\n");
	}
	if(register_netdev(net1)) {
		printk(KERN_INFO "net1 not registered\n");
	} else {
		printk(KERN_INFO "net1 registered\n");
	}
	return 0;
}
void os_exit_mod(void)
{
	struct os_priv *priv;
	
	if(net0) {
		priv = netdev_priv(net0);
		kfree(priv->pkt);
		unregister_netdev(net0);
	}
	
	if(net1) {
		priv = netdev_priv(net1);
		kfree(priv->pkt);
		unregister_netdev(net1);
	}
}
MODULE_LICENSE("GPL");
module_init(os_init_mod);
module_exit(os_exit_mod);
```
## makefile
```
ifneq ($(KERNELRELEASE),)
MODULE_NAME = networkmodule
$(MODULE_NAME)-objs := network.o
obj-m := $(MODULE_NAME).o
else
KERNEL_DIR = /lib/modules/`uname -r`/build
MODULEDIR := $(shell pwd)
.PHONY: modules
default: modules
modules:
	make -C $(KERNEL_DIR) M=$(MODULEDIR) modules
clean distclean:
	rm -f *.o *.mod.c .*.*.cmd *.ko
	rm -rf .tmp_versions
endif
```
## 结构体net_device_ops
```
$ pwd
/usr/src/kernels/3.10.0-693.el7.x86_64/include/linux
$ grep net_device_ops netdevice.h --color=auto -n
830: * It is an extension of net_device_ops. Drivers that want to use any of the
831: * fields defined here must initialize net_device_ops->ndo_size to
832: * sizeof(struct net_device_ops).
890:struct net_device_ops_extended {
1197:struct net_device_ops {
1407:	 * net_device_ops_extended. The reserved slots above can be used
1410:	 * net_device_ops, if they allocated the net_device_ops structure
1412:	 * using others' net_device_ops must access the extended fields
1415:	RH_KABI_EXTEND(struct net_device_ops_extended extended)
1419:	const struct net_device_ops *__ops = (ops);			\
1420:	size_t __off = offsetof(struct net_device_ops, extended.field);	\
1612:	const struct net_device_ops *netdev_ops;
3979:static inline netdev_tx_t __netdev_start_xmit(const struct net_device_ops *ops,
3990:	const struct net_device_ops *ops = dev->netdev_ops;
```
## 结构体header_ops
```cpp
./netdevice.h:276:struct header_ops {
./netdevice.h:1616:	const struct header_ops *header_ops;
./netdevice.h:2735:	if (!dev->header_ops || !dev->header_ops->create)
./netdevice.h:2738:	return dev->header_ops->create(skb, dev, type, daddr, saddr, len);
./netdevice.h:2746:	if (!dev->header_ops || !dev->header_ops->parse)
./netdevice.h:2748:	return dev->header_ops->parse(skb, haddr);
./netdevice.h:2755:	if (!dev->header_ops || !dev->header_ops->rebuild)
./netdevice.h:2757:	return dev->header_ops->rebuild(skb);
```
