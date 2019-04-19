# netintro — introduction to networking facilities：“SIOCIFCREATE” - Koma Hub - CSDN博客
2019年01月27日 10:54:11[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：254
个人分类：[OpenBSD																[计算机网络																[Glib																[Glibc](https://blog.csdn.net/Rong_Toa/article/category/8605571)](https://blog.csdn.net/Rong_Toa/article/category/8611689)](https://blog.csdn.net/Rong_Toa/article/category/7569489)](https://blog.csdn.net/Rong_Toa/article/category/8644442)
# [NAME](https://man.openbsd.org/netintro.4#NAME)
`netintro` — introduction to networking facilities
# [SYNOPSIS](https://man.openbsd.org/netintro.4#SYNOPSIS)
`#include <sys/socket.h>`
`#include <net/route.h>`
`#include <net/if.h>`
# TABLE OF CONTENTS
[NAME](#NAME)
[SYNOPSIS](#SYNOPSIS)
[TABLE OF CONTENTS](#TABLE%20OF%20CONTENTS)
[DESCRIPTION](#DESCRIPTION)
[PROTOCOLS](#PROTOCOLS)
[ADDRESSING](#ADDRESSING)
[ROUTING](#ROUTING)
[INTERFACES](#INTERFACES)
[SEE ALSO](#SEE_ALSO)
[HISTORY](#HISTORY)
# [DESCRIPTION](https://man.openbsd.org/netintro.4#DESCRIPTION)
This section is a general introduction to the networking facilities available in the system. Documentation in this part of section 4 is broken up into three areas: *protocol families* (domains), *protocols*, and *network interfaces*.
All network protocols are associated with a specific *protocol family*. A protocol family provides basic services to the protocol implementation to allow it to function within a specific network environment. These services may include packet fragmentation and reassembly, routing, addressing, and basic transport. A protocol family may support multiple methods of addressing, though the current protocol implementations do not. A protocol family is normally comprised of a number of protocols, one per [socket(2)](https://man.openbsd.org/socket.2) type. It is not required that a protocol family support all socket types. A protocol family may contain multiple protocols supporting the same socket abstraction.
A protocol supports one of the socket abstractions detailed in [socket(2)](https://man.openbsd.org/socket.2). A specific protocol may be accessed either by creating a socket of the appropriate type and protocol family, or by requesting the protocol explicitly when creating a socket. Protocols normally accept only one type of address format, usually determined by the addressing structure inherent in the design of the protocol family/network architecture. Certain semantics of the basic socket abstractions are protocol specific. All protocols are expected to support the basic model for their particular socket type, but may, in addition, provide non-standard facilities or extensions to a mechanism. For example, a protocol supporting the `SOCK_STREAM` abstraction may allow more than one byte of out-of-band data to be transmitted per out-of-band message.
A network interface is similar to a device interface. Network interfaces comprise the lowest layer of the networking subsystem, interacting with the actual transport hardware. An interface may support one or more protocol families and/or address formats. The SYNOPSIS section of each network interface entry gives a sample specification of the related drivers for use in providing a system description to the [config(8)](https://man.openbsd.org/config.8) program. The DIAGNOSTICS section lists messages which may appear on the console and/or in the system error log, /var/log/messages (see [syslogd(8)](https://man.openbsd.org/syslogd.8)), due to errors in device operation.
Network interfaces may be collected together into interface groups. An interface group is a container that can be used generically when referring to any interface related by some criteria. When an action is performed on an interface group, such as packet filtering by the[pf(4)](https://man.openbsd.org/pf.4) subsystem, the operation will be applied to each member interface in the group, if supported by the subsystem. The [ifconfig(8)](https://man.openbsd.org/ifconfig.8)utility can be used to view and assign membership of an interface to an interface group with the `group` modifier.
# [PROTOCOLS](https://man.openbsd.org/netintro.4#PROTOCOLS)
The system currently supports the Internet protocols (IPv4 and IPv6), MPLS, and a few others. Raw socket interfaces are provided to the IP protocol layer of the Internet. Consult the appropriate manual pages in this section for more information regarding the support for each protocol family.
# [ADDRESSING](https://man.openbsd.org/netintro.4#ADDRESSING)
Associated with each protocol family is an address format. All network addresses adhere to a general structure, called a sockaddr, described below. However, each protocol imposes a finer, more specific structure, generally renaming the variant, which is discussed in the protocol family manual page alluded to above.
```cpp
struct sockaddr {
	u_int8_t	sa_len;		/* total length */
	sa_family_t	sa_family;	/* address family */
	char		sa_data[14];	/* actually longer */
};
```
The field sa_len contains the total length of the structure, which may exceed 16 bytes. The following address values for sa_family are known to the system (and additional formats are defined for possible future implementation):
```cpp
#define AF_UNIX		1	/* local to host */
#define AF_INET		2	/* internetwork: UDP, TCP, etc. */
#define AF_INET6	24	/* IPv6 */
#define AF_MPLS		33	/* Multiprotocol Label Switching */
```
The sa_data field contains the actual address value. Note that it may be longer than 14 bytes.
# [ROUTING](https://man.openbsd.org/netintro.4#ROUTING)
OpenBSD provides some packet routing facilities. The kernel maintains a routing information database, which is used in selecting the appropriate network interface when transmitting packets.
A user process (or possibly multiple co-operating processes) maintains this database by sending messages over a special kind of socket. This supplants fixed-size [ioctl(2)](https://man.openbsd.org/ioctl.2)s used in earlier releases.
This facility is described in [route(4)](https://man.openbsd.org/route.4).
# [INTERFACES](https://man.openbsd.org/netintro.4#INTERFACES)
Each network interface in a system corresponds to a path through which messages may be sent and received. A network interface usually has a hardware device associated with it, though certain interfaces such as the loopback interface, [lo(4)](https://man.openbsd.org/lo.4), do not.
The following [ioctl(2)](https://man.openbsd.org/ioctl.2) calls may be used to manipulate network interfaces. The [ioctl(2)](https://man.openbsd.org/ioctl.2) is made on a socket (typically of type `SOCK_DGRAM`) in the desired domain. Most of the requests take an ifreq structure pointer as their parameter. This structure is as follows:
```cpp
struct	ifreq {
#define IFNAMSIZ 16
	char	ifr_name[IFNAMSIZ];	/* if name, e.g. "en0" */
	union {
		struct	sockaddr	ifru_addr;
		struct	sockaddr	ifru_dstaddr;
		struct	sockaddr	ifru_broadaddr;
		short			ifru_flags;
		int			ifru_metric;
		int64_t			ifru_vnetid;
		uint64_t		ifru_media;
		caddr_t			ifru_data;
		unsigned int		ifru_index;
	} ifr_ifru;
#define	ifr_addr	ifr_ifru.ifru_addr	/* address */
#define	ifr_dstaddr	ifr_ifru.ifru_dstaddr	/* other end of p-to-p link */
#define	ifr_broadaddr	ifr_ifru.ifru_broadaddr	/* broadcast address */
#define	ifr_flags	ifr_ifru.ifru_flags	/* flags */
#define	ifr_metric	ifr_ifru.ifru_metric	/* metric */
#define	ifr_mtu		ifr_ifru.ifru_metric	/* mtu (overload) */
#define	ifr_hardmtu	ifr_ifru.ifru_metric	/* hardmtu (overload) */
#define	ifr_media	ifr_ifru.ifru_media	/* media options */
#define	ifr_rdomainid	ifr_ifru.ifru_metric	/* VRF instance (overload) */
#define	ifr_vnetid	ifr_ifru.ifru_vnetid	/* Virtual Net Id */
#define	ifr_ttl		ifr_ifru.ifru_metric	/* tunnel TTL (overload) */
#define	ifr_data	ifr_ifru.ifru_data	/* for use by interface */
#define	ifr_index	ifr_ifru.ifru_index	/* interface index */
#define	ifr_llprio	ifr_ifru.ifru_metric	/* link layer priority */
};
```
The supported [ioctl(2)](https://man.openbsd.org/ioctl.2) requests are:
[`SIOCSIFADDR`](https://man.openbsd.org/netintro.4#SIOCSIFADDR) struct ifreq *
Set the interface address for a protocol family. Following the address assignment, the “initialization” routine for the interface is called.
This call has been deprecated and superseded by the `SIOCAIFADDR` call, described below.
[`SIOCSIFDSTADDR`](https://man.openbsd.org/netintro.4#SIOCSIFDSTADDR) struct ifreq *
Set the point-to-point address for a protocol family and interface.
This call has been deprecated and superseded by the `SIOCAIFADDR` call, described below.
[`SIOCSIFBRDADDR`](https://man.openbsd.org/netintro.4#SIOCSIFBRDADDR) struct ifreq *
Set the broadcast address for a protocol family and interface.
This call has been deprecated and superseded by the `SIOCAIFADDR` call, described below.
[`SIOCGIFADDR`](https://man.openbsd.org/netintro.4#SIOCGIFADDR) struct ifreq *
Get the interface address for a protocol family.
[`SIOCGIFDSTADDR`](https://man.openbsd.org/netintro.4#SIOCGIFDSTADDR) struct ifreq *
Get the point-to-point address for a protocol family and interface.
[`SIOCGIFBRDADDR`](https://man.openbsd.org/netintro.4#SIOCGIFBRDADDR) struct ifreq *
Get the broadcast address for a protocol family and interface.
[`SIOCGIFDESCR`](https://man.openbsd.org/netintro.4#SIOCGIFDESCR) struct ifreq *
Get the interface description, returned in the ifru_data field.
[`SIOCSIFDESCR`](https://man.openbsd.org/netintro.4#SIOCSIFDESCR) struct ifreq *
Set the interface description to the value of the ifru_data field, limited to the size of `IFDESCRSIZE`.
[`SIOCSIFFLAGS`](https://man.openbsd.org/netintro.4#SIOCSIFFLAGS) struct ifreq *
Set the interface flags. If the interface is marked down, any processes currently routing packets through the interface are notified; some interfaces may be reset so that incoming packets are no longer received. When marked up again, the interface is reinitialized.
[`SIOCGIFFLAGS`](https://man.openbsd.org/netintro.4#SIOCGIFFLAGS) struct ifreq *
Get the interface flags.
[`SIOCGIFXFLAGS`](https://man.openbsd.org/netintro.4#SIOCGIFXFLAGS) struct ifreq *
Get the extended interface flags.
[`SIOCGIFMTU`](https://man.openbsd.org/netintro.4#SIOCGIFMTU) struct ifreq *
Get the current MTU of the interface.
[`SIOCGIFHARDMTU`](https://man.openbsd.org/netintro.4#SIOCGIFHARDMTU) struct ifreq *
Get the maximum hardware MTU of the interface.
[`SIOCSIFMEDIA`](https://man.openbsd.org/netintro.4#SIOCSIFMEDIA) struct ifreq *
Set the interface media settings. See [ifmedia(4)](https://man.openbsd.org/ifmedia.4) for possible values.
[`SIOCGIFMEDIA`](https://man.openbsd.org/netintro.4#SIOCGIFMEDIA) struct ifmediareq *
Get the interface media settings. The ifmediareq structure is as follows:
```cpp
struct ifmediareq {
	char		ifm_name[IFNAMSIZ];	/* if name, e.g. "en0" */
	uint64_t	ifm_current;	/* current media options */
	uint64_t	ifm_mask;	/* don't care mask */
	uint64_t	ifm_status;	/* media status */
	uint64_t	ifm_active;	/* active options */
	int		ifm_count;	/* #entries in ifm_ulist array */
	uint64_t	*ifm_ulist;	/* media words */
};
```
See [ifmedia(4)](https://man.openbsd.org/ifmedia.4) for interpreting this value.
[`SIOCSIFMETRIC`](https://man.openbsd.org/netintro.4#SIOCSIFMETRIC) struct ifreq *
Set the interface routing metric. The metric is used only by user-level routers.
[`SIOCGIFMETRIC`](https://man.openbsd.org/netintro.4#SIOCGIFMETRIC) struct ifreq *
Get the interface metric.
[`SIOCSIFPRIORITY`](https://man.openbsd.org/netintro.4#SIOCSIFPRIORITY) struct ifreq *
Set the interface routing priority. The interface routing priority influences the resulting routing priority of new static routes added to the kernel using the specified interface. The value is in the range of 0 to 16 with smaller numbers being better.
[`SIOCGIFPRIORITY`](https://man.openbsd.org/netintro.4#SIOCGIFPRIORITY) struct ifreq *
Get the interface priority.
[`SIOCGIFRDOMAIN`](https://man.openbsd.org/netintro.4#SIOCGIFRDOMAIN) struct ifreq *
Get the interface routing domain. This identifies which routing table is used for the interface.
[`SIOCAIFADDR`](https://man.openbsd.org/netintro.4#SIOCAIFADDR) struct ifaliasreq *
An interface may have more than one address associated with it in some protocols. This request provides a means to add additional addresses (or modify characteristics of the primary address if the default address for the address family is specified).
Rather than making separate calls to set destination or broadcast addresses, or network masks (now an integral feature of multiple protocols), a separate structure, ifaliasreq, is used to specify all three facets simultaneously (see below). One would use a slightly tailored version of this structure specific to each family (replacing each sockaddr by one of the family-specific type). One should always set the length of a sockaddr, as described in [ioctl(2)](https://man.openbsd.org/ioctl.2).
The ifaliasreq structure is as follows:
```cpp
struct ifaliasreq {
	char	ifra_name[IFNAMSIZ];	/* if name, e.g. "en0" */
	struct	sockaddr ifra_addr;
	struct	sockaddr ifra_dstaddr;
#define ifra_broadaddr ifra_dstaddr
	struct	sockaddr ifra_mask;
};
```
[`SIOCDIFADDR`](https://man.openbsd.org/netintro.4#SIOCDIFADDR) struct ifreq *
This request deletes the specified address from the list associated with an interface. It also uses the ifaliasreq structure to allow for the possibility of protocols allowing multiple masks or destination addresses, and also adopts the convention that specification of the default address means to delete the first address for the interface belonging to the address family in which the original socket was opened.
[`SIOCGIFCONF`](https://man.openbsd.org/netintro.4#SIOCGIFCONF) struct ifconf *
Get the interface configuration list. This request takes an ifconf structure (see below) as a value-result parameter. The ifc_len field should be initially set to the size of the buffer pointed to by ifc_buf. On return it will contain the length, in bytes, of the configuration list.
Alternately, if the ifc_len passed in is set to 0, `SIOCGIFCONF` will set ifc_len to the size that ifc_buf needs to be to fit the entire configuration list and will not fill in the other parameters. This is useful for determining the exact size that ifc_buf needs to be in advance. Note, however, that this is an extension that not all operating systems support.
```cpp
struct ifconf {
	int	ifc_len;	  /* size of associated buffer */
	union {
		caddr_t	ifcu_buf;
		struct	ifreq *ifcu_req;
	} ifc_ifcu;
#define ifc_buf ifc_ifcu.ifcu_buf /* buffer address */
#define ifc_req ifc_ifcu.ifcu_req /* array of structures ret'd */
};
```
[`SIOCIFCREATE`](https://man.openbsd.org/netintro.4#SIOCIFCREATE) struct ifreq *
Attempt to create the specified interface.
[`SIOCIFDESTROY`](https://man.openbsd.org/netintro.4#SIOCIFDESTROY) struct ifreq *
Attempt to destroy the specified interface.
[`SIOCIFGCLONERS`](https://man.openbsd.org/netintro.4#SIOCIFGCLONERS) struct if_clonereq *
Get the list of clonable interfaces. This request takes an if_clonereq structure pointer (see below) as a value-result parameter. The ifcr_count field should be set to the number of `IFNAMSIZ`-sized strings that can fit in the buffer pointed to byifcr_buffer. On return, ifcr_total will be set to the number of clonable interfaces, and the buffer pointed to by ifcr_buffer will be filled with the names of clonable interfaces aligned on `IFNAMSIZ` boundaries.
The if_clonereq structure is as follows:
```cpp
struct if_clonereq {
	int   ifcr_total;  /* total cloners (out) */
	int   ifcr_count;  /* room for this many in user buf */
	char *ifcr_buffer; /* buffer for cloner names */
};
```
[`SIOCAIFGROUP`](https://man.openbsd.org/netintro.4#SIOCAIFGROUP) struct ifgroupreq *
Associate the interface named by ifgr_name with the interface group named by ifgr_group. The ifgroupreq structure is as follows:
```cpp
struct ifg_req {
	char			 ifgrq_group[IFNAMSIZ];
};
struct ifgroupreq {
	char	ifgr_name[IFNAMSIZ];
	u_int	ifgr_len;
	union {
		char	ifgru_group[IFNAMSIZ];
		struct	ifg_req *ifgru_groups;
	} ifgr_ifgru;
#define ifgr_group	ifgr_ifgru.ifgru_group
#define ifgr_groups	ifgr_ifgru.ifgru_groups
};
```
[`SIOCGIFGROUP`](https://man.openbsd.org/netintro.4#SIOCGIFGROUP) struct ifgroupreq *
Retrieve the list of groups for which an interface is a member. The interface is named by ifgr_name. On enter, the amount of memory in which the group names will be written is stored in ifgr_len, and the group names themselves will be written to the memory pointed to by ifgr_groups. On return, the amount of memory actually written is returned in ifgr_len.
Alternately, if the ifgr_len passed in is set to 0, `SIOCGIFGROUP` will set ifgr_len to the size that ifgr_groups needs to be to fit the entire group list and will not fill in the other parameters. This is useful for determining the exact size that ifgr_groups needs to be in advance.
[`SIOCDIFGROUP`](https://man.openbsd.org/netintro.4#SIOCDIFGROUP) struct ifgroupreq *
Remove the membership of the interface named by ifgr_name from the group ifgr_group.
# [SEE ALSO](https://man.openbsd.org/netintro.4#SEE_ALSO)
[netstat(1)](https://man.openbsd.org/netstat.1), [ioctl(2)](https://man.openbsd.org/ioctl.2), [socket(2)](https://man.openbsd.org/socket.2), [arp(4)](https://man.openbsd.org/arp.4), [bridge(4)](https://man.openbsd.org/bridge.4), [ifmedia(4)](https://man.openbsd.org/ifmedia.4), [inet(4)](https://man.openbsd.org/inet.4), [intro(4)](https://man.openbsd.org/intro.4), [ip(4)](https://man.openbsd.org/ip.4), [ip6(4)](https://man.openbsd.org/ip6.4), [lo(4)](https://man.openbsd.org/lo.4), [mpe(4)](https://man.openbsd.org/mpe.4), [pf(4)](https://man.openbsd.org/pf.4), [tcp(4)](https://man.openbsd.org/tcp.4), [udp(4)](https://man.openbsd.org/udp.4), [unix(4)](https://man.openbsd.org/unix.4),[hosts(5)](https://man.openbsd.org/hosts.5), [bgpd(8)](https://man.openbsd.org/bgpd.8), [config(8)](https://man.openbsd.org/config.8), [ifconfig(8)](https://man.openbsd.org/ifconfig.8), [mrouted(8)](https://man.openbsd.org/mrouted.8), [netstart(8)](https://man.openbsd.org/netstart.8), [ospfd(8)](https://man.openbsd.org/ospfd.8), [ripd(8)](https://man.openbsd.org/ripd.8), [route(8)](https://man.openbsd.org/route.8)
# [HISTORY](https://man.openbsd.org/netintro.4#HISTORY)
The `netintro` manual appeared in 4.3BSD-Tahoe.
