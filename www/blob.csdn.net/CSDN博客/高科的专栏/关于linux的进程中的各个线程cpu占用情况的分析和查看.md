
# 关于linux的进程中的各个线程cpu占用情况的分析和查看 - 高科的专栏 - CSDN博客

2014年11月18日 19:36:41[高科](https://me.csdn.net/pbymw8iwm)阅读数：25760


我们经常会在新开的服搭建一个游戏的服务器，有时候要进行压力测试，那么如何来看呢，一般我们会通过top命令查看各个进程的cpu和内存占用情况，获得到了我们的进程id，然后我们也许会通过pstack命令查看里边的各个线程id以及对应的线程现在正在做什么事情，分析多组数据就可以获得哪些线程里有慢操作影响了服务器的性能，从而得到解决方案。比如这样的以组数据：
[root@AY130816144542124256Z bin]\# pstack 30222
Thread 9 (Thread 0x7f729adc1700 (LWP 30251)):
\#0  0x00007f72a429b720 in sem_wait () from /lib64/libpthread.so.0
\#1  0x0000000000ac5eb6 in Semaphore::down() ()
\#2  0x0000000000ac5cac in Queue::get() ()
\#3  0x00000000009a583f in DBManager::processUpdate(Queue*) ()
\#4  0x00000000009a4bfb in dbUpdateThread(void*) ()
\#5  0x00007f72a4295851 in start_thread () from /lib64/libpthread.so.0
\#6  0x00007f72a459267d in clone () from /lib64/libc.so.6
Thread 8 (Thread 0x7f727c579700 (LWP 30252)):
\#0  0x00007f72a429b720 in sem_wait () from /lib64/libpthread.so.0
\#1  0x0000000000ac5eb6 in Semaphore::down() ()
\#2  0x0000000000ac5cac in Queue::get() ()
\#3  0x00000000009a5799 in DBManager::processQuery(Queue*) ()
\#4  0x00000000009a4c3a in dbQueryThread(void*) ()
\#5  0x00007f72a4295851 in start_thread () from /lib64/libpthread.so.0
\#6  0x00007f72a459267d in clone () from /lib64/libc.so.6
Thread 7 (Thread 0x7f7257fff700 (LWP 30253)):
\#0  0x00007f72a42997bb in pthread_cond_timedwait@@GLIBC_2.3.2 () from /lib64/libpthread.so.0
\#1  0x00007f72a549ee08 in utils::thread::condition_impl::timed_wait(int) () from /usr/local/utils-0.0.1/lib/libutils.so.0.0.1
\#2  0x00007f72a549ebd3 in utils::thread::Condition::timed_wait(int) () from /usr/local/utils-0.0.1/lib/libutils.so.0.0.1
\#3  0x00000000009d5f57 in utils::MessageQueue<FightInfo*>::pop() ()
\#4  0x00000000009d5557 in FightReport::svc() ()
\#5  0x00007f72a5494b45 in utils::Task_Base::thread_proc(void*) () from /usr/local/utils-0.0.1/lib/libutils.so.0.0.1
\#6  0x00007f72a4295851 in start_thread () from /lib64/libpthread.so.0
\#7  0x00007f72a459267d in clone () from /lib64/libc.so.6
Thread 6 (Thread 0x7f72397b7700 (LWP 30254)):
\#0  0x00007f72a4588fc3 in poll () from /lib64/libc.so.6
\#1  0x00007f72a0fbded4 in __libc_res_nsend () from /lib64/libresolv.so.2
\#2  0x00007f72a0fba76a in __libc_res_nquery () from /lib64/libresolv.so.2
\#3  0x00007f72a0fbad29 in __libc_res_nquerydomain () from /lib64/libresolv.so.2
\#4  0x00007f72a0fbb9cf in __libc_res_nsearch () from /lib64/libresolv.so.2
\#5  0x00007f729adc37a7 in _nss_dns_gethostbyname4_r () from /lib64/libnss_dns.so.2
\#6  0x00007f72a457a4c3 in gaih_inet () from /lib64/libc.so.6
\#7  0x00007f72a457cb20 in getaddrinfo () from /lib64/libc.so.6
\#8  0x00007f72a56fc782 in Curl_getaddrinfo_ex () from /usr/lib64/libcurl.so.4
\#9  0x00007f72a56f1d42 in Curl_getaddrinfo () from /usr/lib64/libcurl.so.4
\#10 0x00007f72a56c9e77 in Curl_resolv () from /usr/lib64/libcurl.so.4
\#11 0x00007f72a56ca138 in Curl_resolv_timeout () from /usr/lib64/libcurl.so.4
\#12 0x00007f72a56d8d88 in ?? () from /usr/lib64/libcurl.so.4
\#13 0x00007f72a56ddb79 in ?? () from /usr/lib64/libcurl.so.4
\#14 0x00007f72a56de76e in Curl_connect () from /usr/lib64/libcurl.so.4
\#15 0x00007f72a56e69b0 in Curl_perform () from /usr/lib64/libcurl.so.4
\#16 0x0000000000ae6e3d in HttpClient::svc() ()
\#17 0x00007f72a5494b45 in utils::Task_Base::thread_proc(void*) () from /usr/local/utils-0.0.1/lib/libutils.so.0.0.1
\#18 0x00007f72a4295851 in start_thread () from /lib64/libpthread.so.0
\#19 0x00007f72a459267d in clone () from /lib64/libc.so.6
Thread 5 (Thread 0x7f721af6f700 (LWP 30255)):
\#0  0x00007f72a455691d in nanosleep () from /lib64/libc.so.6
\#1  0x000000000098cb8a in Sleep(unsigned long) ()
\#2  0x000000000098b87d in DynResource::svc() ()
\#3  0x00007f72a5494b45 in utils::Task_Base::thread_proc(void*) () from /usr/local/utils-0.0.1/lib/libutils.so.0.0.1
\#4  0x00007f72a4295851 in start_thread () from /lib64/libpthread.so.0
\#5  0x00007f72a459267d in clone () from /lib64/libc.so.6
Thread 4 (Thread 0x7f71fc727700 (LWP 30256)):
\#0  0x00007f72a455691d in nanosleep () from /lib64/libc.so.6
\#1  0x000000000098cb8a in Sleep(unsigned long) ()
\#2  0x0000000000a61516 in PlayerOpLogThread::svc() ()
\#3  0x00007f72a5494b45 in utils::Task_Base::thread_proc(void*) () from /usr/local/utils-0.0.1/lib/libutils.so.0.0.1
\#4  0x00007f72a4295851 in start_thread () from /lib64/libpthread.so.0
\#5  0x00007f72a459267d in clone () from /lib64/libc.so.6
Thread 3 (Thread 0x7f71ddedf700 (LWP 30257)):
\#0  0x00007f72a4592c73 in epoll_wait () from /lib64/libc.so.6
\#1  0x00007f72a51f334f in Epoll_Reactor::run_reactor_event_loop() () from /usr/local/net_manager-0.0.2/lib/libnet_manager.so.0.0.2
\#2  0x00007f72a51f2523 in Net_Thread::svc() () from /usr/local/net_manager-0.0.2/lib/libnet_manager.so.0.0.2
\#3  0x00007f72a5494b45 in utils::Task_Base::thread_proc(void*) () from /usr/local/utils-0.0.1/lib/libutils.so.0.0.1
\#4  0x00007f72a4295851 in start_thread () from /lib64/libpthread.so.0
\#5  0x00007f72a459267d in clone () from /lib64/libc.so.6
Thread 2 (Thread 0x7f71bf697700 (LWP 30258)):
\#0  0x00007f72a4592c73 in epoll_wait () from /lib64/libc.so.6
\#1  0x00007f72a51f334f in Epoll_Reactor::run_reactor_event_loop() () from /usr/local/net_manager-0.0.2/lib/libnet_manager.so.0.0.2
\#2  0x00007f72a51f2523 in Net_Thread::svc() () from /usr/local/net_manager-0.0.2/lib/libnet_manager.so.0.0.2
\#3  0x00007f72a5494b45 in utils::Task_Base::thread_proc(void*) () from /usr/local/utils-0.0.1/lib/libutils.so.0.0.1
\#4  0x00007f72a4295851 in start_thread () from /lib64/libpthread.so.0
\#5  0x00007f72a459267d in clone () from /lib64/libc.so.6
Thread 1 (Thread 0x7f72a60ae7e0 (LWP 30222)):
\#0  0x00007f72a4584c95 in _xstat () from /lib64/libc.so.6
\#1  0x00007f72a45483e0 in __tzfile_read () from /lib64/libc.so.6
\#2  0x00007f72a4547864 in tzset_internal () from /lib64/libc.so.6
\#3  0x00007f72a4547b20 in tzset () from /lib64/libc.so.6
\#4  0x00007f72a4546699 in timelocal () from /lib64/libc.so.6
\#5  0x0000000000b0b08d in Achieve::GetRemainTime(AchieveTemplate*) ()
\#6  0x0000000000b115ca in Achieve::update() ()
\#7  0x0000000000a197ce in Player::update() ()
\#8  0x0000000000b1b272 in PlayerMng::Tick() ()
\#9  0x0000000000a73105 in GameServer::FrameTick(unsigned int) ()
\#10 0x0000000000a6ff80 in GameServer::run() ()
\#11 0x0000000000a773a1 in main ()
[root@AY130816144542124256Z gameserver]\# ps  -eLo pid,lwp,pcpu | grep 30222
30222 30222 31.4
30222 30251  0.0
30222 30252  0.0
30222 30253  0.0
30222 30254  0.0
30222 30255  0.0
30222 30256  1.2
30222 30257  1.2
30222 30258  1.0
多组数据显示表明我们的主逻辑线程的确占用的cpu很高，发现其实在Achieve::update() 的时候做了太多没有用的判断，并且可以减少循环进入的次数的，所以我在这里进行了优化
后来用了一下iptraf命令查看了网络流量：
![](https://img-blog.csdn.net/20141118194027140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGJ5bXc4aXdt/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
第一项：IP流量监控
第二项：常规查看网卡流量状态。只查看各网卡的总流量
第三项：详细查看网卡流量状态。比如按TCP，UDP，ARP等协议查看
选all interfaces，查看所有网卡接口
![](https://img-blog.csdn.net/20141118194113955?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGJ5bXc4aXdt/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20141118194127843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGJ5bXc4aXdt/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
界面分上下两部分，上部分可详细显示哪个与之相连的IP，发了多少包，即时流量是多少，下部分，可以显示udp等信息。
显示连接本地服务器的所有连接流量情况
![](https://img-blog.csdn.net/20141118194555234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGJ5bXc4aXdt/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
另外不得不提到另外一个命令sar：
sar有很多用途，如果要来监控网络流量，使用下面的命令行方式：
sar -n DEV interval count
其中，interval是统计时间间隔，以秒为单位；count是总共统计几次，如果为0就不断的统计直到 Ctrl+C 打断，否则执行count次就退出。
比如：sar -n DEV 1 4
比如：sar -n DEV 10 0
IFACE：LAN接口
rxpck/s：每秒钟接收的数据包
txpck/s：每秒钟发送的数据包
rxbyt/s：每秒钟接收的字节数
txbyt/s：每秒钟发送的字节数
sar -n DEV -u 1 10

07:22:01 PM     IFACE   rxpck/s   txpck/s    rxkB/s    txkB/s   rxcmp/s   txcmp/s  rxmcst/s
07:22:02 PM        lo      0.00      0.00      0.00      0.00      0.00      0.00      0.00
07:22:02 PM      eth0      2.47      0.00      0.11      0.00      0.00      0.00      0.00
07:22:02 PM      eth1     80.25     22.22      3.53      3.52      0.00      0.00      0.00

07:22:02 PM     CPU     %user     %nice   %system   %iowait    %steal     %idle
07:22:03 PM     all      1.85      0.00      1.06      0.00      0.00     97.09

07:22:02 PM     IFACE   rxpck/s   txpck/s    rxkB/s    txkB/s   rxcmp/s   txcmp/s  rxmcst/s
07:22:03 PM        lo      0.00      0.00      0.00      0.00      0.00      0.00      0.00
07:22:03 PM      eth0      0.00      0.00      0.00      0.00      0.00      0.00      0.00
07:22:03 PM      eth1     66.67     19.75      2.95      3.22      0.00      0.00      0.00

07:22:03 PM     CPU     %user     %nice   %system   %iowait    %steal     %idle
07:22:04 PM     all      2.12      0.00      1.06      0.00      0.00     96.82

07:22:03 PM     IFACE   rxpck/s   txpck/s    rxkB/s    txkB/s   rxcmp/s   txcmp/s  rxmcst/s
07:22:04 PM        lo      0.00      0.00      0.00      0.00      0.00      0.00      0.00
07:22:04 PM      eth0      7.32      4.88      0.65      0.38      0.00      0.00      0.00
07:22:04 PM      eth1     85.37     28.05      4.18      3.88      0.00      0.00      0.00

07:22:04 PM     CPU     %user     %nice   %system   %iowait    %steal     %idle
07:22:05 PM     all      3.96      0.00      2.64      0.00      0.00     93.40

07:22:04 PM     IFACE   rxpck/s   txpck/s    rxkB/s    txkB/s   rxcmp/s   txcmp/s  rxmcst/s
07:22:05 PM        lo    412.50    412.50    104.44    104.44      0.00      0.00      0.00
07:22:05 PM      eth0     13.75     12.50      1.32      1.26      0.00      0.00      0.00
07:22:05 PM      eth1    903.75    707.50    174.12     62.42      0.00      0.00      0.00

07:22:05 PM     CPU     %user     %nice   %system   %iowait    %steal     %idle
07:22:06 PM     all      5.25      0.00      4.46      2.10      0.00     88.19

07:22:05 PM     IFACE   rxpck/s   txpck/s    rxkB/s    txkB/s   rxcmp/s   txcmp/s  rxmcst/s
07:22:06 PM        lo    849.41    849.41    154.54    154.54      0.00      0.00      0.00
07:22:06 PM      eth0      0.00      0.00      0.00      0.00      0.00      0.00      0.00
07:22:06 PM      eth1   1287.06   1337.65     84.48    461.76      0.00      0.00      0.00

07:22:06 PM     CPU     %user     %nice   %system   %iowait    %steal     %idle
07:22:07 PM     all      1.85      0.00      1.06      0.00      0.00     97.09

07:22:06 PM     IFACE   rxpck/s   txpck/s    rxkB/s    txkB/s   rxcmp/s   txcmp/s  rxmcst/s
07:22:07 PM        lo      0.00      0.00      0.00      0.00      0.00      0.00      0.00
07:22:07 PM      eth0      4.94      0.00      0.22      0.00      0.00      0.00      0.00
07:22:07 PM      eth1     97.53     19.75      4.32      3.18      0.00      0.00      0.00

07:22:07 PM     CPU     %user     %nice   %system   %iowait    %steal     %idle
07:22:08 PM     all      1.58      0.00      1.32      0.00      0.00     97.11

07:22:07 PM     IFACE   rxpck/s   txpck/s    rxkB/s    txkB/s   rxcmp/s   txcmp/s  rxmcst/s
07:22:08 PM        lo      4.94      4.94      0.31      0.31      0.00      0.00      0.00
07:22:08 PM      eth0      1.23      0.00      0.06      0.00      0.00      0.00      0.00
07:22:08 PM      eth1     67.90     24.69      3.02      3.94      0.00      0.00      0.00

Average:        CPU     %user     %nice   %system   %iowait    %steal     %idle
Average:        all      3.63      0.00      2.55      0.84      0.00     92.98

Average:        IFACE   rxpck/s   txpck/s    rxkB/s    txkB/s   rxcmp/s   txcmp/s  rxmcst/s
Average:           lo    212.61    212.61     50.54     50.54      0.00      0.00      0.00
Average:         eth0     42.79     41.70      3.27     41.84      0.00      0.00      0.00
Average:         eth1    492.48    432.48     53.63    110.40      0.00      0.00      0.00



