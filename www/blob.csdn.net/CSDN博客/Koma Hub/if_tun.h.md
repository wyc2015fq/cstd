# if_tun.h - Koma Hub - CSDN博客
2019年01月26日 18:13:18[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：71
Browse the source code of [linux](https://code.woboq.org/linux/linux/)/[include](https://code.woboq.org/linux/linux/include/)/[uapi](https://code.woboq.org/linux/linux/include/uapi/)/[linux](https://code.woboq.org/linux/linux/include/uapi/linux/)/[if_tun.h](https://code.woboq.org/linux/linux/include/uapi/linux/if_tun.h.html)
```
1	/* SPDX-License-Identifier: GPL-2.0+ WITH Linux-syscall-note */
2	/*
3	 *  Universal TUN/TAP device driver.
4	 *  Copyright (C) 1999-2000 Maxim Krasnyansky <max_mk@yahoo.com>
5	 *
6	 *  This program is free software; you can redistribute it and/or modify
7	 *  it under the terms of the GNU General Public License as published by
8	 *  the Free Software Foundation; either version 2 of the License, or
9	 *  (at your option) any later version.
10	 *
11	 *  This program is distributed in the hope that it will be useful,
12	 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
13	 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
14	 *  GNU General Public License for more details.
15	 */
16	
17	#ifndef _UAPI__IF_TUN_H
18	#define _UAPI__IF_TUN_H
19	
20	#include <linux/types.h>
21	#include <linux/if_ether.h>
22	#include <linux/filter.h>
23	
24	/* Read queue size */
25	#define TUN_READQ_SIZE	500
26	/* TUN device type flags: deprecated. Use IFF_TUN/IFF_TAP instead. */
27	#define TUN_TUN_DEV 	IFF_TUN
28	#define TUN_TAP_DEV	IFF_TAP
29	#define TUN_TYPE_MASK   0x000f
30	
31	/* Ioctl defines */
32	#define TUNSETNOCSUM  _IOW('T', 200, int) 
33	#define TUNSETDEBUG   _IOW('T', 201, int) 
34	#define TUNSETIFF     _IOW('T', 202, int) 
35	#define TUNSETPERSIST _IOW('T', 203, int) 
36	#define TUNSETOWNER   _IOW('T', 204, int)
37	#define TUNSETLINK    _IOW('T', 205, int)
38	#define TUNSETGROUP   _IOW('T', 206, int)
39	#define TUNGETFEATURES _IOR('T', 207, unsigned int)
40	#define TUNSETOFFLOAD  _IOW('T', 208, unsigned int)
41	#define TUNSETTXFILTER _IOW('T', 209, unsigned int)
42	#define TUNGETIFF      _IOR('T', 210, unsigned int)
43	#define TUNGETSNDBUF   _IOR('T', 211, int)
44	#define TUNSETSNDBUF   _IOW('T', 212, int)
45	#define TUNATTACHFILTER _IOW('T', 213, struct sock_fprog)
46	#define TUNDETACHFILTER _IOW('T', 214, struct sock_fprog)
47	#define TUNGETVNETHDRSZ _IOR('T', 215, int)
48	#define TUNSETVNETHDRSZ _IOW('T', 216, int)
49	#define TUNSETQUEUE  _IOW('T', 217, int)
50	#define TUNSETIFINDEX	_IOW('T', 218, unsigned int)
51	#define TUNGETFILTER _IOR('T', 219, struct sock_fprog)
52	#define TUNSETVNETLE _IOW('T', 220, int)
53	#define TUNGETVNETLE _IOR('T', 221, int)
54	/* The TUNSETVNETBE and TUNGETVNETBE ioctls are for cross-endian support on
55	 * little-endian hosts. Not all kernel configurations support them, but all
56	 * configurations that support SET also support GET.
57	 */
58	#define TUNSETVNETBE _IOW('T', 222, int)
59	#define TUNGETVNETBE _IOR('T', 223, int)
60	#define TUNSETSTEERINGEBPF _IOR('T', 224, int)
61	#define TUNSETFILTEREBPF _IOR('T', 225, int)
62	
63	/* TUNSETIFF ifr flags */
64	#define IFF_TUN		0x0001
65	#define IFF_TAP		0x0002
66	#define IFF_NAPI	0x0010
67	#define IFF_NAPI_FRAGS	0x0020
68	#define IFF_NO_PI	0x1000
69	/* This flag has no real effect */
70	#define IFF_ONE_QUEUE	0x2000
71	#define IFF_VNET_HDR	0x4000
72	#define IFF_TUN_EXCL	0x8000
73	#define IFF_MULTI_QUEUE 0x0100
74	#define IFF_ATTACH_QUEUE 0x0200
75	#define IFF_DETACH_QUEUE 0x0400
76	/* read-only flag */
77	#define IFF_PERSIST	0x0800
78	#define IFF_NOFILTER	0x1000
79	
80	/* Socket options */
81	#define TUN_TX_TIMESTAMP 1
82	
83	/* Features for GSO (TUNSETOFFLOAD). */
84	#define TUN_F_CSUM	0x01	/* You can hand me unchecksummed packets. */
85	#define TUN_F_TSO4	0x02	/* I can handle TSO for IPv4 packets */
86	#define TUN_F_TSO6	0x04	/* I can handle TSO for IPv6 packets */
87	#define TUN_F_TSO_ECN	0x08	/* I can handle TSO with ECN bits. */
88	#define TUN_F_UFO	0x10	/* I can handle UFO packets */
89	
90	/* Protocol info prepended to the packets (when IFF_NO_PI is not set) */
91	#define TUN_PKT_STRIP	0x0001
92	struct tun_pi {
93		__u16  flags;
94		__be16 proto;
95	};
96	
97	/*
98	 * Filter spec (used for SETXXFILTER ioctls)
99	 * This stuff is applicable only to the TAP (Ethernet) devices.
100	 * If the count is zero the filter is disabled and the driver accepts
101	 * all packets (promisc mode).
102	 * If the filter is enabled in order to accept broadcast packets
103	 * broadcast addr must be explicitly included in the addr list.
104	 */
105	#define TUN_FLT_ALLMULTI 0x0001 /* Accept all multicast packets */
106	struct tun_filter {
107		__u16  flags; /* TUN_FLT_ flags see above */
108		__u16  count; /* Number of addresses */
109		__u8   addr[0][ETH_ALEN];
110	};
111	
112	#endif /* _UAPI__IF_TUN_H */
113
```
## **[[SOLVED] Compiling Opencaster from source](https://bbs.archlinux.org/viewtopic.php?id=182088)**
[SOLVED]:
> 
Change the 15th line of "mpe.c" (Under 'tools/mpe2sec') from
`#include <net/if_tun.h>`
Fixed: 
`#include <linux/if_tun.h>`
Fix courtesy [tomk](https://bbs.archlinux.org/profile.php?id=1822).
So, I am encountering this odd little error when trying to 'make' [Opencaster (Source Code)](http://ftp.de.debian.org/debian/pool/main/o/opencaster/opencaster_3.2.2+dfsg.orig.tar.gz) from source. On previous installs of Arch, this worked without a hitch. Now however, it shows the following:
```
mpe.c:15:24: fatal error: net/if_tun.h: No such file or directory
 #include <net/if_tun.h>
                        ^
```
Here is the entire trace:
```
mpe.c:15:24: fatal error: net/if_tun.h: No such file or directory
 #include <net/if_tun.h>
                        ^
compilation terminated.
Makefile:11: recipe for target 'mpe.o' failed
make[2]: *** [mpe.o] Error 1
make[2]: Leaving directory '/home/ushio/cide/oc/opencaster-3.2.2/tools/mpe2sec'
Makefile:17: recipe for target 'mpe2sec' failed
make[1]: *** [mpe2sec] Error 2
make[1]: Leaving directory '/home/ushio/cide/oc/opencaster-3.2.2/tools'
Makefile:5: recipe for target 'tools' failed
make: *** [tools] Error 2
```
I don't recall ever having to install foreign libs, nor using any makeflags. Any assistance here would be grand. I need to test a patch by the end of this week. (And no my patch is not in this source)  :)
*Last edited by jowijo (2014-05-28 14:33:31)*

