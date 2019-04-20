# Linux Kernel 3.0新特性概览(转) - h13 - 博客园
上周五，Linus Torvalds终于发布了备受瞩目的新一代Linux操作系统内核。Linux Kernel 3.0经过了七个RC候选版才推出正式版本，上一个版本是5月19日的2.6.39，也是2.6系列的第39次升级维护。八年来，Linux开发人员一直致力于2.6版本的改进工作，新版本提升到3.0是为了纪念Linux风雨走过的20年，同时也精简了越来越复杂的版本号。
上周五，Linus Torvalds终于发布了备受瞩目的新一代Linux操作系统内核。Linux Kernel 3.0的发布，代表着Linux开源新时代的到来。
之前曾有消息称，Linus Torvalds在其Google+帖子中表示，原定于7月19日发布的Linux kernel 3.0将会延期发布，原因是发现系统仍存在一个小的bug。经过几天的修补，bug基本修复完毕。
Linux Kernel 3.0经过了七个RC候选版才推出正式版本，上一个版本是5月19日的2.6.39，也是2.6系列的第39次升级维护。从版本号来讲，Linux Kernel 3.0本应该命名为2.6.40更合适。
Linus Torvalds在发布声明中说：“2.6的日子结束了，3.0来了。这也为下一个内核版本敞开了大门，也就是3.1。稳定升级版会取第三位小数，所以第一个稳定更新将是3.0.1。”
八年来，Linux开发人员一直致力于2.6版本的改进工作，新版本提升到3.0是为了纪念Linux风雨走过的20年，同时也精简了越来越复杂的版本号，之前稳定的内核版本是2.6.39。
Linux Kernel 3.0改进了对虚拟化和文件系统的支持，主要新特性有：
－ Btrfs实现自动碎片整理、数据校验和检查，并且提升了部分性能
－ 支持sendmmsg()函数调用，UDP发送性能提升20％，接口发送性能提升约30％
－ 支持XEN dom0
－ 支持应用缓存清理(CleanCache)
－ 支持柏克莱封包过滤器(Berkeley Packet Filter)实时过滤，配合libpcap/tcpdump提升包过滤规则的运行效率
－ 支持无线局域网(WLAN)唤醒
－ 支持非特殊授权的ICMP_ECHO函数
－ 支持高精度计时器Alarm-timers
－ 支持setns() syscall，更好地命名空间管理
－ 支持微软Kinect体感设备
－ 支持AMD Llano APU处理器
－ 支持Intel iwlwifi 105/135无线网卡
－ 支持Intel C600 SAS控制器
－ 支持雷凌Ralink RT5370无线网卡
－ 支持多种Realtek RTL81xx系列网卡
－ 大量新驱动
－ 大量bug修正和改进
Linux Kernel 3.0源代码下载：[ftp://ftp.kernel.org/pub/linux/kernel/v3.0/linux-3.0.tar.bz2](ftp://ftp.kernel.org/pub/linux/kernel/v3.0/linux-3.0.tar.bz2)
发布声明：[https://lkml.org/lkml/2011/7/21/455](https://lkml.org/lkml/2011/7/21/455)
更新日志：[http://kernelnewbies.org/Linux_3.0](http://kernelnewbies.org/Linux_3.0)
