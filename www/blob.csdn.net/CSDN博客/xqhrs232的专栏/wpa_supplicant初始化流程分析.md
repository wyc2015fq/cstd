# wpa_supplicant初始化流程分析 - xqhrs232的专栏 - CSDN博客
2012年01月04日 15:52:02[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：824标签：[数据结构																[interface																[socket																[网络																[化工																[user](https://so.csdn.net/so/search/s.do?q=user&t=blog)](https://so.csdn.net/so/search/s.do?q=化工&t=blog)](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)](https://so.csdn.net/so/search/s.do?q=interface&t=blog)](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://biancheng.dnbcw.info/win2003/242212.html](http://biancheng.dnbcw.info/win2003/242212.html)
(以下分析基于 wpa_supplicant 0.5.10 版本)
1. 启动命令
wpa supplicant 在启动时，启动命令可以带有很多参数，目前我们的启动命令如下：
wpa_supplicant /system/bin/wpa_supplicant -Dwext -ieth0 -c/data/wifi/wpa_supplicant.conf -f/data/wifi/wpa_log.txt
wpa_supplicant对于启动命令带的参数，用了两个数据结构来保存，
一个是 wpa_params, 另一个是wpa_interface.
这主要是考虑到wpa_supplicant是可以同时支持多个网络接口的。
wpa_params数据结构主要记录与网络接口无关的一些参数设置。
而每一个网络接口就用一个wpa_interface数据结构来记录。
在启动命令行中，可以用-N来指定将要描述一个新的网络接口，对于一个新的网络接口，可以用下面六个参数描述：
-i<ifname> : 网络接口名称 
-c<conf>: 配置文件名称
-C<ctrl_intf>: 控制接口名称
-D<driver>: 驱动类型
-p<driver_param>: 驱动参数
-b<br_ifname>: 桥接口名称
2. wpa_supplicant 初始化流程
2.1. main()函数：
在这个函数中，主要做了四件事。
a. 解析命令行传进的参数。
b. 调用wpa_supplicant_init()函数，做wpa_supplicant的初始化工作。
c. 调用wpa_supplicant_add_iface()函数，增加网络接口。
d. 调用wpa_supplicant_run()函数，让wpa_supplicant真正的run起来。
2.2. wpa_supplicant_init()函数：
a. 打开debug 文件。
b. 注册EAP peer方法。
c. 申请wpa_global内存，该数据结构作为统领其他数据结构的一个核心， 主要包括四个部分：
wpa_supplicant *ifaces   /*每个网络接口都有一个对应的wpa_supplicant数据结构，该指针指向最近加入的一个，在wpa_supplicant数据结构中有指针指向next*/
wpa_params params   /*启动命令行中带的通用的参数*/
ctrl_iface_global_priv *ctrl_iface  /*global 的控制接口*/
ctrl_iface_dbus_priv *dbus_ctrl_iface  /*dbus 的控制接口*/
d. 设置wpa_global中的wpa_params中的参数。
e. 调用eloop_init函数将全局变量eloop中的user_data指针指向wpa_global。
f. 调用wpa_supplicant_global_ctrl_iface_init函数初始化global 控制接口。
g. 调用wpa_supplicant_dbus_ctrl_iface_init函数初始化dbus 控制接口。
h. 将该daemon的pid写入pid_file中。
2.3. wpa_supplicant_add_iface()函数：
该函数根据启动命令行中带有的参数增加网络接口, 有几个就增加几个。
a. 因为wpa_supplicant是与网络接口对应的重要的数据结构，所以，首先分配一个wpa_supplicant数据结构的内存。
b. 调用wpa_supplicant_init_iface() 函数来做网络接口的初始工作，主要包括：
设置驱动类型，默认是wext；
读取配置文件，并将其中的信息设置到wpa_supplicant数据结构中的conf 指针指向的数据结构，它是一个wpa_config类型；
命令行设置的控制接口ctrl_interface和驱动参数driver_param覆盖配置文件里设置，命令行中的优先；
拷贝网络接口名称和桥接口名称到wpa_config数据结构；
对于网络配置块有两个链表描述它，一个是 config->ssid,它按照配置文件中的顺序依次挂载在这个链表上，还有一个是pssid，它是一个二级指针，指向一个指针数组，该指针数组按照优先级从高到底的顺序依次保存wpa_ssid指针，相同优先级的在同一链表中挂载。
c. 调用wpa_supplicant_init_iface2() 函数，主要包括：
调用wpa_supplicant_init_eapol()函数来初始化eapol；
调用相应类型的driver的init()函数；
设置driver的param参数；
调用wpa_drv_get_ifname()函数获得网络接口的名称，对于wext类型的driver，没有这个接口函数；
调用wpa_supplicant_init_wpa()函数来初始化wpa，并做相应的初始化工作；
调用wpa_supplicant_driver_init()函数，来初始化driver接口参数；在该函数的最后，会
wpa_s->prev_scan_ssid = BROADCAST_SSID_SCAN;
wpa_supplicant_req_scan(wpa_s, interface_count, 100000);
来主动发起scan，
调用wpa_supplicant_ctrl_iface_init()函数，来初始化控制接口；对于UNIX SOCKET这种方式，其本地socket文件是由配置文件里的ctrl_interface参数指定的路径加上网络接口名称；
2.4. wpa_supplicant_run()函数：
初始化完成之后，让wpa_supplicant的main event loop run起来。
在wpa_supplicant中，有许多与外界通信的socket，它们都是需要注册到eloop event模块中的，具体地说，就是在eloop_sock_table中增加一项记录，其中包括了sock_fd, handle, eloop_data, user_data。
eloop event模块就是将这些socket组织起来，统一管理，然后在eloop_run中利用select机制来管理socket的通信。
3. wpa_supplicant 的对外接口分析
对于wpa_supplicant模块的对外接口，主要有以下几种：
3.1. global control interface: 用于配置（增加或删除）网络接口。
3.2. ctrl interface: 与其他外部模块交互的控制接口。
例如，在初始化时，android 平台的wifi.c中的 wifi_connect_to_supplicant函数调用wpa_ctrl_open函数创建两个socket，一个是ctrl interface，另一个就是monitor interface,monitor interface这个接口用于监测从wpa_supplicant发出的event事件。
这两个socket创建成功后，monitor interface 会发送ATTACH到wpa_supplicant模块，wpa_supplicant模块收到后，会将该客户端的socket信息记录下来，用于以后发送事件时用（由于用的是DGRAM的方式）。
3.3. socket for ioctl: 发送命令到kernel space。
3.4. socket (netlink) for interact between kernel and userspace(AF_NETLINK, NETLINK_ROUTE): 接受kernel发送上来的event。
3.5. socket for l2 packet(PF_PACKET): 处理802.1x报文。
