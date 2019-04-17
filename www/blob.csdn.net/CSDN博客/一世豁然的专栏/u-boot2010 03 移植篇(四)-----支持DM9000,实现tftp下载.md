# u-boot2010.03 移植篇(四)-----支持DM9000,实现tftp下载 - 一世豁然的专栏 - CSDN博客





2015年06月24日 23:03:44[一世豁然](https://me.csdn.net/Explorer_day)阅读数：513标签：[bootloader																[u-boot](https://so.csdn.net/so/search/s.do?q=u-boot&t=blog)](https://so.csdn.net/so/search/s.do?q=bootloader&t=blog)
个人分类：[linux驱动](https://blog.csdn.net/Explorer_day/article/category/2652125)








本博客转载于：[http://blog.csdn.net/yyttiao/article/details/8145033](http://blog.csdn.net/yyttiao/article/details/8145033)






----------------------------------------------------------
使用环境 
PC:     ubuntu 11.04
kernel: 2.6.32-28-generic
corss:  arm-linux-gcc 4.3.2
arm:    s3c6410
uboot:  uboot-2010-03
----------------------------------------------------------
作者:    LvApp
联系方式: 97164811@qq.com
一切版权均有作者所有，欢迎转载，请指明出处，如何修改请与本人联系，谢谢


哇塞..tftp下载,很多6410买的开发板梦寐以求需要的功能,为嘛?当然为了方便实现系统更新.一键下载之类的..想想都觉得兴奋..那就开始吧

首先.前阵子本人比较忙..所以一直没有更新.再加上.最近在看别的资料.所以在写这些内容的时候,我还得自己实战下.保证写出来没有错误吧..

这篇内容不多.因为uboot已经做的很好了..几乎不需要修改太多的东西,就能支持dm9000这款用的比较多的网卡了..

这篇文章其实有稍许超前了.因为对应的分析贴还没写好.所以这里我会适当加上一些理论补充,比如:我为什么要加这句代码到这个地方?类似于这样的问题

根据smdk6400 copy 过来的那么默认用的就是cs8900这块网卡,可以从configs/smdk6410.h这个文件中看到....这里当然不需要了..所以,果断先把关于8900的宏

全都去掉..如下:




像这样.去掉先...接着就是添加DM9000相关的宏了...这里我也贴出来..





**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/8145033#)[copy](http://blog.csdn.net/yyttiao/article/details/8145033#)



- #define CONFIG_DM9000_NO_SROM           //此处说明,该网卡没有eeprom存储..其实dm9000是有的,只是没用,所以只好说没有了
- #define CONFIG_DM9000                   //定义DM9000 回头初始化驱动,就判断这个宏了
- #define CONFIG_DRIVER_DM9000            //开启DM9000驱动的代码
- #define CONFIG_DM9000_BASE      0x18800300                    //这个宏是定义网卡连接到你的arm上面的地址.属于sram1地址区域
- #define DM9000_IO           CONFIG_DM9000_BASE            //根据网卡手册,指定I/O DATA操作地址
- #define DM9000_DATA         (CONFIG_DM9000_BASE + 0x04)
- #define CONFIG_DM9000_USE_16BIT         //指明使用16位数据模式,该宏其实是为了方便知道网卡信息而已,在代码中没有半毛钱关系...网卡的16位选择,
- //是在内存设置的时候设置的...也就是说设定sram1内存寄存器
- 
- 
- #define CONFIG_ETHADDR      00:40:5C:26:0A:5B        //这里定义一些网络相关参数,这些以后也可以直接手动设定环境变量.
- #define CONFIG_NETMASK      255.255.255.0            //放在这里也是比较方面而已
- #define CONFIG_IPADDR       192.168.1.123
- #define CONFIG_SERVERIP     192.168.1.100
- #define CONFIG_GATEWAYIP    192.168.1.1
- 
- #define CONFIG_DM9000_DEBUG             //开启调试


ok 网卡的配置就好了...非常简单..就是开启一些宏和设定一些相关的参数而已...


下面去添加dm9000的初始化,不然不能用,那么初始化放哪呢?网卡初始化的地方究竟是吧....在第一部分的时候分析.没有发现关于网卡初始化的地方...那么也就是说.网卡

的初始化其实是在第二部分里面完成的?没错....直接定位到start_armboot第二部分代码入口...





**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/8145033#)[copy](http://blog.csdn.net/yyttiao/article/details/8145033#)



- #if defined(CONFIG_CMD_NET)
- #if defined(CONFIG_NET_MULTI)
-     puts ("Net:   ");  
- #endif
-     eth_initialize(gd->bd);      /* 这里就是网卡的初始化函数了 */
- #if defined(CONFIG_RESET_PHY_R)
-     debug ("Reset Ethernet PHY\n");  
-     reset_phy();  
- #endif


直接定位到 eth_initialize函数中...这个函数蛮长的.....






**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/8145033#)[copy](http://blog.csdn.net/yyttiao/article/details/8145033#)



- int eth_initialize(bd_t *bis)  
- {  
-     unsigned char env_enetaddr[6];  
- int eth_number = 0;  
-     eth_devices = NULL;  
-     eth_current = NULL;  
- 
-     show_boot_progress (64);  
- #if defined(CONFIG_MII) || defined(CONFIG_CMD_MII)
-     miiphy_init();  
- #endif
- /* Try board-specific initialization first.  If it fails or isn't
-      * present, try the cpu-specific initialization */
- if (board_eth_init(bis) < 0)        /* 注意这里有个板级初始化调用...board_eth_init(bis) */
-         cpu_eth_init(bis);  
- 
- #if defined(CONFIG_DB64360) || defined(CONFIG_CPCI750)
-     mv6436x_eth_initialize(bis);  
- #endif
- #if defined(CONFIG_DB64460) || defined(CONFIG_P3Mx)
-     mv6446x_eth_initialize(bis);  
- #endif
- if (!eth_devices) {  
-         puts ("No ethernet found.\n");  
-         show_boot_progress (-64);  
-     } else {  
- struct eth_device *dev = eth_devices;  
- char *ethprime = getenv ("ethprime");  
-         show_boot_progress (65);  
- do {  
- if (eth_number)  
-                 puts (", ");  
-             printf("%s", dev->name);  
- if (ethprime && strcmp (dev->name, ethprime) == 0) {  
-                 eth_current = dev;  
-                 puts (" [PRIME]");  
-             }  
-             eth_getenv_enetaddr_by_index(eth_number, env_enetaddr);/* 获取网卡ethaddr的MAC地址 */
- if (memcmp(env_enetaddr, "\0\0\0\0\0\0", 6)) {  
- if (memcmp(dev->enetaddr, "\0\0\0\0\0\0", 6) &&  
-                     memcmp(dev->enetaddr, env_enetaddr, 6))  
-                 {  
-                     printf ("\nWarning: %s MAC addresses don't match:\n",  
-                         dev->name);  
-                     printf ("Address in SROM is         %pM\n",  
-                         dev->enetaddr);  
-                     printf ("Address in environment is  %pM\n",  
-                         env_enetaddr);  
-                 }  
-                 memcpy(dev->enetaddr, env_enetaddr, 6);  
-             }  
-             eth_number++;  
-             dev = dev->next;  
-         } while(dev != eth_devices);  
- #ifdef CONFIG_NET_MULTI
- /* update current ethernet name */
- if (eth_current) {  
- char *act = getenv("ethact");  
- if (act == NULL || strcmp(act, eth_current->name) != 0)  
-                 setenv("ethact", eth_current->name);  
-         } else
-             setenv("ethact", NULL);  
- #endif
-         putc ('\n');  
-     }  
- return eth_number;  
- }  

在上面的代码中看到 board_eth_init 函数的调用,该函数是跟板级相关的网卡初始化,那么肯定在board\samsung\smdk6410\smdk6410.c中了...打开看看..果然发现这么一个接口函数






**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/8145033#)[copy](http://blog.csdn.net/yyttiao/article/details/8145033#)



- #ifdef CONFIG_CMD_NET
- int board_eth_init(bd_t *bis)  
- {  
- int rc = 0;  
- #ifdef CONFIG_CS8900
-     rc = cs8900_initialize(0, CONFIG_CS8900_BASE);  
- #endif
- return rc;  
- }  
- #endif

而且还发现这里面有8900网卡的初始化..那应该没错了...我们就模仿它添加我们的DM9000网卡..如下:






**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/8145033#)[copy](http://blog.csdn.net/yyttiao/article/details/8145033#)



- #ifdef CONFIG_CMD_NET
- int board_eth_init(bd_t *bis)  
- {  
- int rc = 0;  
- #ifdef CONFIG_CS8900
-     rc = cs8900_initialize(0, CONFIG_CS8900_BASE);  
- #endif
- #if defined(CONFIG_DM9000)
-     rc = dm9000_initialize(bis);  
- #endif
- return rc;  
- }  
- #endif



其实我想说,..修改好了...完工了.....真的好了....


![](https://img-my.csdn.net/uploads/201211/04/1351963622_5701.png)


根据添加.发现网卡也有了...网卡的一些环境变量也有了.....

下面是我用tftp功能下载uImage的截图

![](https://img-my.csdn.net/uploads/201211/04/1351963738_2130.png)





最后要说下..ping命令不好使的问题...ping会发出arp获得对方ip的网卡信息..接着在此发送icmp包.才能得到ping的结果...但是根据网卡调试信息

发现发送了arp之后,也获得了mac.但是没有发送icmp包...正在探查该问题所在...有人说修改timeout时间...大家可以试下...因为该bug影响不大.

所以没有深究...

今天就到了...谢谢

Finish!

Thanks a lot~





