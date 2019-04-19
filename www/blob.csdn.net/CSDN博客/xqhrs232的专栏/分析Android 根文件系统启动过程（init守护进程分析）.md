# 分析Android 根文件系统启动过程（init守护进程分析） - xqhrs232的专栏 - CSDN博客
2011年03月15日 14:11:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：501标签：[android																[action																[signal																[化工																[server](https://so.csdn.net/so/search/s.do?q=server&t=blog)](https://so.csdn.net/so/search/s.do?q=化工&t=blog)](https://so.csdn.net/so/search/s.do?q=signal&t=blog)](https://so.csdn.net/so/search/s.do?q=action&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/chatliang/archive/2009/07/13/4344432.aspx](http://blog.csdn.net/chatliang/archive/2009/07/13/4344432.aspx)
在Android系统启动时，内核引导参数上一般都会设置 “init=/init”,这样的话，如果内核成功挂载了这个文件系统之后，首先运行的就是这个根目录下的init程序 。这个程序所了什么呢？ 我们只有RFSC（Readthe Fucking Source code）!!
init程序源码在Android官方 源码的system/core/init中，main在init.c里。我们的分析就从main开始。
init： 
（1）安装SIGCHLD信号 。（如果父进程不等待子进程结束，子进程将成为僵尸进程（zombie）从而占用系统资源。因此需要对SIGCHLD信号做出处理，回收僵尸进程的资源，避免造成不必要的资源浪费。
（2）对umask进行清零。
    何为umask，请看[http://www.szstudy.cn/showArticle/53978.shtml](http://www.szstudy.cn/showArticle/53978.shtml)
（3）为root fs建立必要的文件夹，并挂载适当的分区。
    /dev （tmpfs）
        /dev/pts (devpts)
        /dev/socket
    /proc (proc)
    /sys  (sysfs)
  (4)创建/dev/null和/dev/kmsg节点。
（5）解析/init.rc，将所有服务和操作信息加入链表。
  (6)从/proc/cmdline中提取信息内核启动参数,并保存到全局变量。
（7）先从上一步获得的全局变量中获取信息硬件信息和版本号，如果没有则从/proc/cpuinfo中提取,并保存到全局变量。
（8）根据硬件信息选择一个/init.(硬件).rc，并解析，将服务和操作信息加入链表。
         在G1的ramdisk根目录下有两个/init.(硬件).rc：init.goldfish.rc和init.trout.rc，init程序会根据上一步获得的硬件信息选择一个解析。
（9）执行链表中带有“early-init”触发的的命令。
（10）遍历/sys文件夹，是内核产生设备添加事件（为了自动 产生设备节点)。
（11）初始化属性系统，并导入初始化属性文件。
（12）从属性系统中得到ro.debuggable，若为1，則初始化keychord監聽。
（13）打開console,如果cmdline中沒有指定console則打開默認的/dev/console。
（14）讀取/initlogo.rle（一張565 rle 壓縮的位圖），如果成功則在/dev/graphics/fb0顯示Logo,如果失敗則將/dev/tty0設為TEXT模式并打開/dev/tty0,輸出文本“ANDROID”字樣。
（15）判斷cmdline 中的參數，并设置属性系统中的参数:
         1、 如果 bootmode為
         - factory,設置ro.factorytest值為1
         - factory2,設置ro.factorytest值為2
         - 其他的設ro.factorytest值為0
       2、如果有serialno参数，則設置ro.serialno，否則為""
       3、如果有bootmod参数，則設置ro.bootmod，否則為"unknown"
       4、如果有baseband参数，則設置ro.baseband，否則為"unknown"
       5、如果有carrier参数，則設置ro.carrier，否則為"unknown"
       6、如果有bootloader参数，則設置ro.bootloader，否則為"unknown"
       7、通过全局变量（前面从/proc/cpuinfo中提取的）設置ro.hardware和ro.version。
（16）執行所有触发标识为init的action。
（17）開始property服務，讀取一些property文件，這一動作必須在前面那些ro.foo設置后做，以便/data/local.prop不能干預到他們。
      - /system/build.prop
      - /system/default.prop
      - /data/local.prop
      - 在讀取默認的property后讀取presistent propertie，在/data/property中
（18）為sigchld handler創建信號機制。
（19）確認所有初始化工作完成：
          device_fd(device init 完成)
          property_set_fd(property server start 完成)
          signal_recv_fd (信號機制建立)
（20） 執行所有触发标识为early-boot的action
（21） 執行所有触发标识为boot的action
（22）基于當前property狀態，執行所有触发标识为property的action
（23）注冊輪詢事件:
           - device_fd
           - property_set_fd
           -signal_recv_fd
           -如果有keychord，則注冊keychord_fd
（24）如果支持BOOTCHART,則初始化BOOTCHART
（25）進入主進程循環:
      - 重置輪詢事件的接受狀態，revents為0
      - 查詢action隊列，并执行。
      - 重啟需要重啟的服务
      - 輪詢注冊的事件
          - 如果signal_recv_fd的revents為POLLIN，則得到一個信號，獲取并處理
          - 如果device_fd的revents為POLLIN,調用handle_device_fd
          - 如果property_fd的revents為POLLIN,調用handle_property_set_fd
          - 如果keychord_fd的revents為POLLIN,調用handle_keychord
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/chatliang/archive/2009/07/13/4344432.aspx](http://blog.csdn.net/chatliang/archive/2009/07/13/4344432.aspx)

