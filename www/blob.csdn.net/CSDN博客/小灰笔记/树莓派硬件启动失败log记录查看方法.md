# 树莓派硬件启动失败log记录查看方法 - 小灰笔记 - CSDN博客





2017年11月10日 23:06:32[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：2040
个人分类：[树莓派](https://blog.csdn.net/grey_csdn/article/category/7277647)









如果硬件启动失败，特别是驱动类的 失效等情况在Linux启动的时候都会记录。但是这个记录在启动阶段的屏幕上刷新非常快，很多时候我们看不到相应的信息。




但是，Linux提供了一个很不错的命令dmesg，使用这个命令可以查看故障缓冲区信息。




相应的信息依然很多，可以将命令输出写入到一个文件中然后进一步查看。下面是我导出的一个例子：

[    0.000000] Booting Linux on physical CPU 0x0

[    0.000000] Initializing cgroup subsys cpuset

[    0.000000] Initializing cgroup subsys cpu

[    0.000000] Initializing cgroup subsys cpuacct

[    0.000000] Linux version 4.1.19+ (dc4@dc4-XPS13-9333) (gcc version 4.9.3 (crosstool-NG crosstool-ng-1.22.0-88-g8460611) ) #858 Tue Mar 15 15:52:03 GMT 2016

[    0.000000] CPU: ARMv6-compatible processor [410fb767] revision 7 (ARMv7), cr=00c5387d

[    0.000000] CPU: PIPT / VIPT nonaliasing data cache, VIPT nonaliasing instruction cache

[    0.000000] Machine model: Raspberry Pi Model B Rev 2

[    0.000000] cma: Reserved 8 MiB at 0x17400000

[    0.000000] Memory policy: Data cache writeback

[    0.000000] On node 0 totalpages: 98304

[    0.000000] free_area_init_node: node 0, pgdat c0839a04, node_mem_map d7c8d000

[    0.000000]   Normal zone: 864 pages used for memmap

[    0.000000]   Normal zone: 0 pages reserved

[    0.000000]   Normal zone: 98304 pages, LIFO batch:31

[    0.000000] pcpu-alloc: s0 r0 d32768 u32768 alloc=1*32768

[    0.000000] pcpu-alloc: [0] 0 

[    0.000000] Built 1 zonelists in Zone order, mobility grouping on.  Total pages: 97440

[    0.000000] Kernel command line: dma.dmachans=0x7f35 bcm2708_fb.fbwidth=1920 bcm2708_fb.fbheight=1080 bcm2708.boardrev=0xe bcm2708.serial=0xf863410c smsc95xx.macaddr=B8:27:EB:63:41:0C bcm2708_fb.fbswap=1 bcm2708.uart_clock=3000000 vc_mem.mem_base=0x1ec00000
 vc_mem.mem_size=0x20000000  dwc_otg.lpm_enable=0 console=ttyAMA0,115200 console=tty1 root=/dev/mmcblk0p2 rootfstype=ext4 elevator=deadline rootwait

[    0.000000] PID hash table entries: 2048 (order: 1, 8192 bytes)

[    0.000000] Dentry cache hash table entries: 65536 (order: 6, 262144 bytes)

[    0.000000] Inode-cache hash table entries: 32768 (order: 5, 131072 bytes)

[    0.000000] Memory: 371876K/393216K available (5796K kernel code, 488K rwdata, 1788K rodata, 348K init, 720K bss, 13148K reserved, 8192K cma-reserved)

[    0.000000] Virtual kernel memory layout:

[    0.000000]     vector  : 0xffff0000 - 0xffff1000   (   4 kB)

[    0.000000]     fixmap  : 0xffc00000 - 0xfff00000   (3072 kB)

[    0.000000]     vmalloc : 0xd8800000 - 0xff000000   ( 616 MB)

[    0.000000]     lowmem  : 0xc0000000 - 0xd8000000   ( 384 MB)

[    0.000000]     modules : 0xbf000000 - 0xc0000000   (  16 MB)

[    0.000000]       .text : 0xc0008000 - 0xc0770240   (7585 kB)

[    0.000000]       .init : 0xc0771000 - 0xc07c8000   ( 348 kB)

[    0.000000]       .data : 0xc07c8000 - 0xc08422f0   ( 489 kB)

[    0.000000]        .bss : 0xc08422f0 - 0xc08f6480   ( 721 kB)

[    0.000000] SLUB: HWalign=32, Order=0-3, MinObjects=0, CPUs=1, Nodes=1

[    0.000000] NR_IRQS:522

[    0.000000] clocksource stc: mask: 0xffffffff max_cycles: 0xffffffff, max_idle_ns: 1911260446275 ns

[    0.000014] sched_clock: 32 bits at 1000kHz, resolution 1000ns, wraps every 2147483647500ns

[    0.000056] Switching to timer-based delay loop, resolution 1000ns

[    0.000337] Console: colour dummy device 80x30

[    0.001305] console [tty1] enabled

[    0.001364] Calibrating delay loop (skipped), value calculated using timer frequency.. 2.00 BogoMIPS (lpj=10000)

[    0.001447] pid_max: default: 32768 minimum: 301

[    0.001824] Mount-cache hash table entries: 1024 (order: 0, 4096 bytes)

[    0.001895] Mountpoint-cache hash table entries: 1024 (order: 0, 4096 bytes)

[    0.002944] Initializing cgroup subsys blkio

[    0.003037] Initializing cgroup subsys memory

[    0.003107] Initializing cgroup subsys devices

[    0.003170] Initializing cgroup subsys freezer

[    0.003231] Initializing cgroup subsys net_cls

[    0.003352] CPU: Testing write buffer coherency: ok

[    0.003470] ftrace: allocating 19751 entries in 58 pages

[    0.106575] Setting up static identity map for 0x81c0 - 0x81f8

[    0.108750] devtmpfs: initialized

[    0.119188] VFP support v0.3: implementor 41 architecture 1 part 20 variant b rev 5

[    0.119631] clocksource jiffies: mask: 0xffffffff max_cycles: 0xffffffff, max_idle_ns: 19112604462750000 ns

[    0.121615] pinctrl core: initialized pinctrl subsystem

[    0.122508] NET: Registered protocol family 16

[    0.127976] DMA: preallocated 4096 KiB pool for atomic coherent allocations

[    0.129446] bcm2708.uart_clock = 3000000

[    0.134864] hw-breakpoint: found 6 breakpoint and 1 watchpoint registers.

[    0.134944] hw-breakpoint: maximum watchpoint size is 4 bytes.

[    0.135167] Serial: AMBA PL011 UART driver

[    0.135461] 20201000.uart: ttyAMA0 at MMIO 0x20201000 (irq = 83, base_baud = 0) is a PL011 rev2

[    0.502230] console [ttyAMA0] enabled

[    0.506619] bcm2835-mbox 2000b880.mailbox: mailbox enabled

[    0.553074] bcm2708-dmaengine 20007000.dma: DMA legacy API manager at f2007000, dmachans=0xf35

[    0.561802] bcm2708-dmaengine 20007000.dma: Initialized 7 DMA channels (+ 1 legacy)

[    0.570399] bcm2708-dmaengine 20007000.dma: Load BCM2835 DMA engine driver

[    0.577331] bcm2708-dmaengine 20007000.dma: dma_debug:0

[    0.583514] SCSI subsystem initialized

[    0.587660] usbcore: registered new interface driver usbfs

[    0.593397] usbcore: registered new interface driver hub

[    0.598917] usbcore: registered new device driver usb

[    0.604944] raspberrypi-firmware soc:firmware: Attached to firmware from 2016-03-15 14:47

[    0.640597] Switched to clocksource stc

[    0.694080] FS-Cache: Loaded

[    0.697401] CacheFiles: Loaded

[    0.715792] NET: Registered protocol family 2

[    0.721714] TCP established hash table entries: 4096 (order: 2, 16384 bytes)

[    0.728907] TCP bind hash table entries: 4096 (order: 2, 16384 bytes)

[    0.735540] TCP: Hash tables configured (established 4096 bind 4096)

[    0.742187] UDP hash table entries: 256 (order: 0, 4096 bytes)

[    0.748086] UDP-Lite hash table entries: 256 (order: 0, 4096 bytes)

[    0.754795] NET: Registered protocol family 1

[    0.759756] RPC: Registered named UNIX socket transport module.

[    0.765808] RPC: Registered udp transport module.

[    0.770544] RPC: Registered tcp transport module.

[    0.775310] RPC: Registered tcp NFSv4.1 backchannel transport module.

[    0.782991] hw perfevents: enabled with armv6_1176 PMU driver, 3 counters available

[    0.792137] futex hash table entries: 256 (order: -1, 3072 bytes)

[    0.814463] VFS: Disk quotas dquot_6.6.0

[    0.818834] VFS: Dquot-cache hash table entries: 1024 (order 0, 4096 bytes)

[    0.828472] FS-Cache: Netfs 'nfs' registered for caching

[    0.835255] NFS: Registering the id_resolver key type

[    0.840432] Key type id_resolver registered

[    0.844771] Key type id_legacy registered

[    0.852592] Block layer SCSI generic (bsg) driver version 0.4 loaded (major 252)

[    0.860493] io scheduler noop registered

[    0.864613] io scheduler deadline registered (default)

[    0.870206] io scheduler cfq registered

[    0.876559] BCM2708FB: allocated DMA memory 57800000

[    0.881730] BCM2708FB: allocated DMA channel 0 @ f2007000

[    0.915060] Console: switching to colour frame buffer device 240x67

[    0.942614] Serial: 8250/16550 driver, 0 ports, IRQ sharing disabled

[    0.950887] vc-cma: Videocore CMA driver

[    0.954944] vc-cma: vc_cma_base      = 0x00000000

[    0.959753] vc-cma: vc_cma_size      = 0x00000000 (0 MiB)

[    0.965329] vc-cma: vc_cma_initial   = 0x00000000 (0 MiB)

[    0.971239] vc-mem: phys_addr:0x00000000 mem_base=0x1ec00000 mem_size:0x20000000(512 MiB)

[    0.999981] brd: module loaded

[    1.012951] loop: module loaded

[    1.017409] vchiq: vchiq_init_state: slot_zero = 0xd7880000, is_master = 0

[    1.026690] Loading iSCSI transport class v2.0-870.

[    1.033347] usbcore: registered new interface driver smsc95xx

[    1.039528] dwc_otg: version 3.00a 10-AUG-2012 (platform bus)

[    1.245927] Core Release: 2.80a

[    1.249186] Setting default values for core params

[    1.254187] Finished setting default values for core params

[    1.460127] Using Buffer DMA mode

[    1.463599] Periodic Transfer Interrupt Enhancement - disabled

[    1.469562] Multiprocessor Interrupt Enhancement - disabled

[    1.475299] OTG VER PARAM: 0, OTG VER FLAG: 0

[    1.479815] Dedicated Tx FIFOs mode

[    1.483914] WARN::dwc_otg_hcd_init:1047: FIQ DMA bounce buffers: virt = 0xd7814000 dma = 0x57814000 len=9024

[    1.494063] FIQ FSM acceleration enabled for :

[    1.494063] Non-periodic Split Transactions

[    1.494063] Periodic Split Transactions

[    1.494063] High-Speed Isochronous Endpoints

[    1.511191] dwc_otg: Microframe scheduler enabled

[    1.511320] WARN::hcd_init_fiq:412: FIQ on core 0 at 0xc03dd8e0

[    1.517383] WARN::hcd_init_fiq:413: FIQ ASM at 0xc03ddbbc length 36

[    1.523790] WARN::hcd_init_fiq:438: MPHI regs_base at 0xd8896000

[    1.530028] dwc_otg 20980000.usb: DWC OTG Controller

[    1.535220] dwc_otg 20980000.usb: new USB bus registered, assigned bus number 1

[    1.542796] dwc_otg 20980000.usb: irq 32, io mem 0x00000000

[    1.548531] Init: Port Power? op_state=1

[    1.558454] Init: Power Port (0)

[    1.567919] usb usb1: New USB device found, idVendor=1d6b, idProduct=0002

[    1.580773] usb usb1: New USB device strings: Mfr=3, Product=2, SerialNumber=1

[    1.593951] usb usb1: Product: DWC OTG Controller

[    1.604522] usb usb1: Manufacturer: Linux 4.1.19+ dwc_otg_hcd

[    1.616190] usb usb1: SerialNumber: 20980000.usb

[    1.627813] hub 1-0:1.0: USB hub found

[    1.637651] hub 1-0:1.0: 1 port detected

[    1.648152] dwc_otg: FIQ enabled

[    1.648178] dwc_otg: NAK holdoff enabled

[    1.648193] dwc_otg: FIQ split-transaction FSM enabled

[    1.648264] Module dwc_common_port init

[    1.648850] usbcore: registered new interface driver usb-storage

[    1.661383] mousedev: PS/2 mouse device common for all mice

[    1.674109] bcm2835-cpufreq: min=700000 max=700000

[    1.685354] sdhci: Secure Digital Host Controller Interface driver

[    1.697600] sdhci: Copyright(c) Pierre Ossman

[    1.708424] sdhost: log_buf @ d7813000 (57813000)

[    1.790678] mmc0: sdhost-bcm2835 loaded - DMA enabled (>1)

[    1.822967] sdhci-pltfm: SDHCI platform and OF driver helper

[    1.839369] ledtrig-cpu: registered to indicate activity on CPUs

[    1.851619] Indeed it is in host mode hprt0 = 00021501

[    1.924976] hidraw: raw HID events driver (C) Jiri Kosina

[    1.941583] usbcore: registered new interface driver usbhid

[    1.953129] usbhid: USB HID core driver

[    1.964398] Initializing XFRM netlink socket

[    1.974783] NET: Registered protocol family 17

[    1.986081] Key type dns_resolver registered

[    1.998765] registered taskstats version 1

[    2.009963] vc-sm: Videocore shared memory driver

[    2.020587] [vc_sm_connected_init]: start

[    2.033100] [vc_sm_connected_init]: end - returning 0

[    2.047768] Waiting for root device /dev/mmcblk0p2...

[    2.065114] mmc0: host does not support reading read-only switch, assuming write-enable

[    2.082768] mmc0: new high speed SDHC card at address aaaa

[    2.095193] mmcblk0: mmc0:aaaa SL16G 14.8 GiB 

[    2.107678]  mmcblk0: p1 p2

[    2.116572] usb 1-1: new high-speed USB device number 2 using dwc_otg

[    2.131003] Indeed it is in host mode hprt0 = 00001101

[    2.205450] EXT4-fs (mmcblk0p2): INFO: recovery required on readonly filesystem

[    2.218922] EXT4-fs (mmcblk0p2): write access will be enabled during recovery

[    2.341219] usb 1-1: New USB device found, idVendor=0424, idProduct=9514

[    2.354192] usb 1-1: New USB device strings: Mfr=0, Product=0, SerialNumber=0

[    2.368958] hub 1-1:1.0: USB hub found

[    2.381019] hub 1-1:1.0: 5 ports detected

[    2.670726] usb 1-1.1: new high-speed USB device number 3 using dwc_otg

[    2.781177] usb 1-1.1: New USB device found, idVendor=0424, idProduct=ec00

[    2.794336] usb 1-1.1: New USB device strings: Mfr=0, Product=0, SerialNumber=0

[    2.812395] smsc95xx v1.0.4

[    2.889081] smsc95xx 1-1.1:1.0 eth0: register 'smsc95xx' at usb-20980000.usb-1.1, smsc95xx USB 2.0 Ethernet, b8:27:eb:63:41:0c

[    3.000799] usb 1-1.2: new high-speed USB device number 4 using dwc_otg

[    3.113055] usb 1-1.2: New USB device found, idVendor=0bda, idProduct=8176

[    3.126275] usb 1-1.2: New USB device strings: Mfr=1, Product=2, SerialNumber=3

[    3.139744] usb 1-1.2: Product: 802.0\x01\x02

[    3.149655] usb 1-1.2: Manufacturer: Realtek

[    3.159888] usb 1-1.2: SerialNumber: 00e04c000001

[    3.270726] usb 1-1.3: new high-speed USB device number 5 using dwc_otg

[    3.407753] usb 1-1.3: New USB device found, idVendor=0bda, idProduct=5411

[    3.420913] usb 1-1.3: New USB device strings: Mfr=1, Product=2, SerialNumber=0

[    3.434290] usb 1-1.3: Product: 4-Port USB 2.0 Hub

[    3.445146] usb 1-1.3: Manufacturer: Generic

[    3.457453] hub 1-1.3:1.0: USB hub found

[    3.468553] hub 1-1.3:1.0: 4 ports detected

[    3.658950] EXT4-fs (mmcblk0p2): orphan cleanup on readonly fs

[    3.671718] EXT4-fs (mmcblk0p2): 1 orphan inode deleted

[    3.683058] EXT4-fs (mmcblk0p2): recovery complete

[    3.723184] EXT4-fs (mmcblk0p2): mounted filesystem with ordered data mode. Opts: (null)

[    3.737534] VFS: Mounted root (ext4 filesystem) readonly on device 179:2.

[    3.752427] devtmpfs: mounted

[    3.762689] Freeing unused kernel memory: 348K (c0771000 - c07c8000)

[    3.775133] usb 1-1.3.1: new low-speed USB device number 6 using dwc_otg

[    3.899115] usb 1-1.3.1: New USB device found, idVendor=17ef, idProduct=6025

[    3.912571] usb 1-1.3.1: New USB device strings: Mfr=0, Product=2, SerialNumber=0

[    3.926236] usb 1-1.3.1: Product: ThinkPad USB Travel Mouse

[    3.952444] input: ThinkPad USB Travel Mouse as /devices/platform/soc/20980000.usb/usb1/1-1/1-1.3/1-1.3.1/1-1.3.1:1.0/0003:17EF:6025.0001/input/input0

[    3.976027] hid-generic 0003:17EF:6025.0001: input,hidraw0: USB HID v11.10 Mouse [ThinkPad USB Travel Mouse] on usb-20980000.usb-1.3.1/input0

[    4.090727] usb 1-1.3.2: new full-speed USB device number 7 using dwc_otg

[    4.282157] usb 1-1.3.2: New USB device found, idVendor=24ae, idProduct=4005

[    4.296011] usb 1-1.3.2: New USB device strings: Mfr=1, Product=2, SerialNumber=0

[    4.310008] usb 1-1.3.2: Product: Rapoo Gaming Keyboard

[    4.321722] usb 1-1.3.2: Manufacturer: Rapoo

[    4.365127] input: Rapoo Rapoo Gaming Keyboard as /devices/platform/soc/20980000.usb/usb1/1-1/1-1.3/1-1.3.2/1-1.3.2:1.0/0003:24AE:4005.0002/input/input1

[    4.441290] hid-generic 0003:24AE:4005.0002: input,hidraw1: USB HID v1.10 Keyboard [Rapoo Rapoo Gaming Keyboard] on usb-20980000.usb-1.3.2/input0

[    4.537068] input: Rapoo Rapoo Gaming Keyboard as /devices/platform/soc/20980000.usb/usb1/1-1/1-1.3/1-1.3.2/1-1.3.2:1.1/0003:24AE:4005.0003/input/input2

[    4.622098] hid-generic 0003:24AE:4005.0003: input,hiddev0,hidraw2: USB HID v1.10 Mouse [Rapoo Rapoo Gaming Keyboard] on usb-20980000.usb-1.3.2/input1

[    4.689189] hid-generic 0003:24AE:4005.0004: hiddev0,hidraw3: USB HID v1.10 Device [Rapoo Rapoo Gaming Keyboard] on usb-20980000.usb-1.3.2/input2

[    5.760035] udevd[156]: starting version 175

[    5.843449] random: nonblocking pool is initialized

[    8.152295] bcm2835-rng 20104000.rng: hwrng registered

[   11.422709] usbcore: registered new interface driver rtl8192cu

[   11.761421] gpiomem-bcm2835 20200000.gpiomem: Initialised: Registers at 0x20200000

[   14.917152] EXT4-fs (mmcblk0p2): re-mounted. Opts: (null)

[   15.514082] EXT4-fs (mmcblk0p2): re-mounted. Opts: (null)

[   28.696770] smsc95xx 1-1.1:1.0 eth0: hardware isn't capable of remote wakeup

[   36.331445] cfg80211: Calling CRDA to update world regulatory domain

[   39.490957] cfg80211: Calling CRDA to update world regulatory domain

[   40.891305] Adding 102396k swap on /var/swap.  Priority:-1 extents:2 across:2134012k SSFS

[   42.650793] cfg80211: Calling CRDA to update world regulatory domain

[   44.108287] uart-pl011 20201000.uart: no DMA platform data

[   45.810906] cfg80211: Calling CRDA to update world regulatory domain

[   48.971177] cfg80211: Calling CRDA to update world regulatory domain

[   52.131131] cfg80211: Calling CRDA to update world regulatory domain

[   55.291248] cfg80211: Calling CRDA to update world regulatory domain

[   58.451257] cfg80211: Calling CRDA to update world regulatory domain

[   61.611450] cfg80211: Calling CRDA to update world regulatory domain

[   64.771548] cfg80211: Calling CRDA to update world regulatory domain

[   67.931617] cfg80211: Calling CRDA to update world regulatory domain

[   71.091692] cfg80211: Exceeded CRDA call max attempts. Not calling CRDA




